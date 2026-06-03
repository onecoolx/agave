# WatchWeb 渲染线程方案设计

> 状态：设计阶段（未实现）
> 目标：在提供线程机制的 RTOS 上，将引擎渲染移入独立线程，提升 UI 流畅度
> 前提：单线程模式已稳定可用，本方案作为可选的编译期增强

## 1. 背景与动机

### 1.1 当前单线程模型

watchweb 当前是单线程：LVGL 主循环里顺序执行引擎事件分发、引擎渲染、屏幕绘制。

```
主循环每轮:
  macross_event_dispatch()          // 引擎 timer/layout
  if (dirty) macross_view_update()  // 引擎渲染进 tile buffer
  updateCanvas()                    // tile buffer → canvas (一次拷贝)
  lv_timer_handler()                // LVGL 绘制 canvas 到屏幕
```

问题：引擎渲染（layout + paint，可能耗时数十毫秒）与 UI 在同一线程，渲染时 UI 卡顿、触摸不响应。

### 1.2 touchweb 的渲染线程模型

touchweb 在 `THREAD_RENDER` 编译开关下，把所有引擎操作放进专用渲染线程：

- UI 线程通过事件队列（mutex 保护）向渲染线程投递命令：CreateView / DestroyView / LoadUrl / PaintView
- 渲染线程循环：取一个事件 → 处理 → `macross_event_dispatch()` → sleep(10ms)
- 事件分级：HighLevel（paint）插队首，DefaultLevel（load）排队尾
- 销毁 view 时清除该 view 的待处理事件（removeAllEventsByView）

**关键收益**：
1. 引擎渲染不阻塞 UI 线程，触摸/动画流畅
2. 所有引擎访问串行在单一线程 → layout 与 paint 天然不竞态（这也是单线程模式崩溃问题的另一种解法）

## 2. 设计目标

| 目标 | 说明 |
|------|------|
| 双模式共存 | 编译开关切换单线程/渲染线程，单线程路径保持现状 |
| 跨 RTOS | 用 LVGL OS 抽象，支持 FreeRTOS/RT-Thread/CMSIS-RTOS2/pthread |
| 接口不变 | WebView 对外接口（loadUrl/reload/scrollBy 等）不变，内部按模式分派 |
| 内存可控 | 表盘内存紧张，tile buffer 同步策略需省内存 |
| 无新依赖 | 复用已有的 LVGL 依赖，不引入额外线程库 |

## 3. 关键技术选型

### 3.1 线程原语：LVGL OS 抽象

LVGL v9 提供 `src/osal/lv_os.h`，封装多种 RTOS：

```c
lv_result_t lv_thread_init(lv_thread_t*, lv_thread_prio_t, void(*cb)(void*),
                           size_t stack_size, void* user_data);
lv_result_t lv_mutex_init(lv_mutex_t*);
lv_result_t lv_mutex_lock(lv_mutex_t*);
lv_result_t lv_mutex_unlock(lv_mutex_t*);
```

通过 `lv_conf.h` 的 `LV_USE_OS` 选择后端：
- 桌面验证：`LV_OS_PTHREAD`
- RTOS 部署：`LV_OS_FREERTOS` / `LV_OS_RTTHREAD` / `LV_OS_CMSIS_RTOS2`

当前 `LV_USE_OS = LV_OS_NONE`（单线程）。

**优势**：不引入新依赖，跨 RTOS，与 LVGL 同生命周期管理。

### 3.2 引擎线程约束

Agave 引擎（macross）的 view 实例**必须单线程访问**——所有 `macross_view_*` 调用和 `macross_event_dispatch()` 都在渲染线程。UI 线程绝不直接碰引擎，只通过事件队列通信。

## 4. 架构设计

### 4.1 总体结构

```
┌─────────────── UI 线程 (LVGL 主循环) ───────────────┐
│  - 触摸/手势 → 投递事件 (LoadUrl/Scroll/Paint)       │
│  - lv_webview DRAW_MAIN: 读 tile buffer blit 到屏幕  │
│  - lv_timer_handler                                  │
└──────────────┬───────────────────────▲──────────────┘
               │ sendEvent (lv_mutex)   │ frame-ready 通知
               ▼                        │
┌─────────────── 渲染线程 ──────────────┴──────────────┐
│  循环: 取事件 → 引擎操作 → macross_event_dispatch    │
│  - create/destroy/load/reload/scroll/paint           │
│  - 所有 macross_view_* 仅在此线程                    │
│  - 渲染完一帧 → 标记 tile buffer ready               │
└──────────────────────────────────────────────────────┘
```

### 4.2 事件队列

复用 touchweb 模型：

```c
enum EventType { CreateView, DestroyView, LoadUrl, Reload, Scroll, PaintView };
struct WebEvent { EventType type; /* 参数: url / dx,dy / dirty rect */ };

// 优先级: PaintView/Scroll 为 HighLevel 插队首, LoadUrl 为 DefaultLevel 排队尾
// 合并优化: 连续的 Scroll/Paint 事件可合并, 避免队列堆积
```

队列用 `lv_mutex` 保护。渲染线程空闲时 sleep，有事件时处理。

### 4.3 tile buffer 同步策略

这是与 touchweb 最大的差异点——watchweb 用 LVGL 绘制，tile buffer 需跨线程读写：
- 渲染线程：`macross_view_update` **写** tile buffer
- UI 线程：`lv_webview` DRAW_MAIN **读** tile buffer blit 到屏幕

**方案对比：**

| 方案 | 锁粒度 | 内存 | 性能 | 适用 |
|------|--------|------|------|------|
| (a) 双缓冲 | 仅 swap 瞬间 | tile buffer ×2 | 最佳，读写不互斥 | 内存宽裕 |
| (b) 单缓冲+锁 | 整个 blit/render | ×1 | render 与 blit 互斥，可能卡顿 | 内存紧张 |

**推荐 (b) 单缓冲 + 细粒度锁**（表盘内存紧张，对齐 touchweb 单缓冲风格）：
- DRAW_MAIN 持锁时间 = 一次 blit（仅可见区，约 480×480×4，快）
- 渲染线程写 buffer 时持同一锁
- 用 frame-ready 标志避免无效 blit

若实测 blit/render 互斥导致明显卡顿，再升级为 (a) 双缓冲。

### 4.4 帧就绪通知

渲染线程渲染完一帧后，需让 UI 线程重绘：
- 渲染线程：渲染完 → 设 `frame_ready` 标志（mutex 保护）
- UI 线程：主循环轮询标志 → `lv_obj_invalidate(webview)` → 下一次 refresh 时 DRAW_MAIN 读 buffer

> ⚠️ LVGL 对象操作通常要求在 LVGL 线程（UI 线程）执行。跨线程的 `lv_obj_invalidate` 不安全，
> 应通过线程安全的标志位 + UI 线程轮询，由 UI 线程自己 invalidate。

## 5. 编译开关与代码组织

```c
// config.h
#define WATCHWEB_THREAD_RENDER  0   // 0=单线程(默认), 1=渲染线程
```

```
webview.h/cpp:
  WebView 接口不变 (loadUrl/reload/scrollBy/requestPaint...)
  内部:
    #if WATCHWEB_THREAD_RENDER
      操作 → renderThread->sendEvent(...)
    #else
      操作 → 直接调引擎 (当前实现)
    #endif

renderthread.h/cpp (仅 THREAD_RENDER 编译):
  RenderThread: lv_thread + lv_mutex + 事件队列
  线程函数: 取事件 → 引擎操作 → macross_event_dispatch
```

main.cpp 主循环：
- 单线程模式：现状（dispatch + paint + lv_timer_handler）
- 线程模式：只跑 `lv_timer_handler` + 投递事件 + 轮询 frame-ready；引擎相关移到渲染线程

## 6. 与单线程崩溃问题的关系

单线程模式下，`cb_invalidate_rect` 在 layout pending 时触发 paint 会撞 `RenderView::paint` 的 `!needsLayout()` 断言。已修复：invalidate 只标脏，paint 由 `cb_update_view_now` 驱动。

渲染线程模式下，**此问题天然消失**：load 和 paint 都是串行事件，paint 事件处理时前序 load + `macross_event_dispatch`（layout）已完成。但仍应保持"invalidate 不直接 paint"的原则，避免渲染线程内的中间态 paint。

## 7. 风险与待验证项

| 项 | 风险 | 验证方法 |
|----|------|---------|
| LVGL 跨线程 invalidate | 对象操作非线程安全 | 用标志位 + UI 线程轮询，不跨线程调 LVGL |
| tile buffer 锁竞争 | blit/render 互斥卡顿 | 实测帧率；必要时改双缓冲 |
| 引擎 timer 精度 | 渲染线程 sleep 影响 layout timer | 调 sleep 粒度；用条件变量替代 sleep |
| 事件堆积 | 快速滚动产生大量事件 | Scroll/Paint 事件合并去重 |
| 销毁时序 | view 销毁时渲染线程仍在用 | 销毁前清队列 + join 线程（对齐 touchweb） |
| LVGL OS 后端 | RTOS 上 LV_USE_OS 配置 | 各目标平台分别验证 |

## 8. 实施步骤（建议顺序）

1. 切 `LV_USE_OS = LV_OS_PTHREAD`，验证 lv_thread/lv_mutex 在桌面可用
2. 搭 RenderThread 骨架（事件队列 + 线程循环），CreateView/LoadUrl 走线程
3. tile buffer 单缓冲 + 锁，PaintView 事件 + 帧就绪通知
4. 接 lv_webview DRAW_MAIN 读 buffer（配合自定义 widget 方案，见 §9）
5. 编译开关切换，确保单线程路径不受影响
6. 桌面 pthread 验证流畅度与正确性
7. RTOS 平台（FreeRTOS/RT-Thread）适配验证

## 9. 关联设计

- **LVGL 自定义 webview widget**：用 `lv_draw_image` 直接从 tile buffer 绘制，省去 canvas 中转拷贝，并提供 invalidate 驱动的异步绘制。与渲染线程方案正交，可独立或配合实施。
- **单线程绘制修复**：invalidate/update 分离（已实现），是渲染线程模式设计原则的延续。
