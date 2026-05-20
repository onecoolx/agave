# VG GUI Framework — 概要设计文档

> 面向 RTOS + MCU 平台的高性能矢量图形用户界面框架
>
> 版本：1.0 | 日期：2026-05-15

## 1. 概述

### 1.1 项目目标

设计一套面向 RTOS + MCU 平台的高性能现代 GUI 框架。采用**双层架构**：底层为独立完整的 Core GUI 引擎（命令式 API），上层为可选的声明式 DSL 层（响应式绑定、组合式组件）。融合 SwiftUI/Rive/Slint 等现代 GUI 系统的设计理念，在嵌入式约束下实现流畅、绚丽的用户界面。

### 1.2 目标硬件平台

| 参数 | 规格 |
|------|------|
| CPU | ARM Cortex-M55 @ 200MHz（支持 MVE/Helium 向量扩展） |
| RAM | 640KB ~ 1MB |
| GPU | 2.5D GPU 加速（DMA2D/NemaGFX 级别） |
| 显示 | 典型 480×480 / 466×466 圆屏，16bpp/32bpp |
| OS | RTOS（NuttX/FreeRTOS） |
| 输入 | 电容触摸屏、旋转编码器、物理按键 |

### 1.3 核心设计原则

1. **双层分离** — Core GUI 独立完整可运行，DSL 层可选叠加
2. **响应式驱动** — 属性变化自动触发 UI 更新，减少手动同步
3. **状态机动画** — 超越简单缓动，支持状态图驱动的复杂交互动画
4. **GPU 优先** — 充分利用 2.5D GPU 和 MVE 指令加速渲染
5. **矢量 + 位图混合** — 矢量图元为主，支持位图资源（图标、图片）
6. **平台无关** — 通过 HAL 层隔离硬件差异，支持 PC 模拟器开发
7. **可裁剪** — 每个模块可独立启用/禁用，按需编译

---

### 1.4 API 命名规范

Core GUI 对外公开 API 统一使用 `vg_` 前缀，采用 C 语言面向对象风格。HAL 南向接口不受此规范约束。

#### 命名模式

```
vg_<模块>_<动作>(对象指针, 参数...)
```

对象指针始终是第一个参数（等价于 this）。

#### 模块前缀表

| 模块 | 前缀 | 示例 |
|------|------|------|
| 图形上下文 | `vg_context_` | `vg_context_fill_path(ctx, path)` |
| 路径 | `vg_path_` | `vg_path_cubic_to(path, ...)` |
| 渐变 | `vg_gradient_` | `vg_gradient_create_linear(...)` |
| 图案 | `vg_pattern_` | `vg_pattern_create(img, repeat)` |
| 图像 | `vg_image_` | `vg_image_create_from_data(...)` |
| 矩阵 | `vg_matrix_` | `vg_matrix_rotate(m, angle)` |
| 字体 | `vg_font_` | `vg_font_create(family, size, ...)` |
| 文本排版 | `vg_text_` | `vg_text_layout(...)` |
| 颜色 | `vg_color_` | `vg_color_rgba(r, g, b, a)` |
| Widget 基础 | `vg_widget_` | `vg_widget_create(cls, parent)` |
| 样式 | `vg_style_` | `vg_style_set_bg_color(s, c)` |
| 动画 | `vg_anim_` | `vg_anim_start(anim)` |
| 事件 | `vg_event_` | `vg_event_get_code(e)` |
| 输入设备 | `vg_indev_` | `vg_indev_register(type, ...)` |
| 定时器 | `vg_timer_` | `vg_timer_create(ms, cb, ...)` |
| 主题 | `vg_theme_` | `vg_theme_set(theme)` |
| 具体控件 | `vg_<控件>_` | `vg_button_set_text(btn, text)` |

#### 类型命名

```
类型:       vg_<名称>_t          vg_context_t, vg_path_t, vg_widget_t
枚举/常量:  VG_<模块>_<名称>     VG_EVENT_CLICKED, VG_STATE_PRESSED, VG_ALIGN_CENTER
回调类型:   vg_<名称>_cb_t       vg_event_cb_t, vg_anim_cb_t
错误码:     vg_result_t          VG_OK, VG_ERR_NOMEM, VG_ERR_PARAM
```

#### 面向对象实现模式

**不透明指针（封装私有数据）：**
```c
// 公开头文件
typedef struct vg_path_t vg_path_t;   // 前向声明
vg_path_t* vg_path_create(void);
void vg_path_line_to(vg_path_t* path, float x, float y);
```

**继承（首成员嵌入基类）：**
```c
typedef struct vg_button_t {
    vg_widget_t base;    // 必须第一个成员，可安全强转为 vg_widget_t*
    char* text;
    vg_image_t* icon;
} vg_button_t;
```

**多态（WidgetClass 虚函数表）：**
```c
const vg_widget_class_t vg_button_class = {
    .base_class     = &vg_widget_base_class,
    .instance_size  = sizeof(vg_button_t),
    .name           = "Button",
    .constructor    = button_ctor,
    .on_paint       = button_paint,
    .on_event       = button_event,
};
```

#### 适用范围

- ✅ Core GUI 对外 API（Graphics / Widget / Style / Animation / Event / 控件）
- ❌ HAL 南向接口（由平台实现者自行命名，如 `hal_display_flush`、`bsp_lcd_init` 等）
- ❌ 内部私有函数（可使用无前缀或 `_vg_` 下划线前缀）

---

## 2. 系统架构

### 2.1 双层架构总览

```
┌─────────────────────────────────────────────────────────┐
│  Application (用户应用)                                   │
│  页面逻辑 / 业务代码 / 数据源                             │
├─────────────────────────────────────────────────────────┤
│  DSL Layer (声明式开发层) [可选，可替换]                   │
│  UI 描述 DSL / 响应式属性绑定 / 组合式组件                │
│  动画状态机编排 / 页面路由与过渡 / 数据驱动               │
╞═════════════════════════════════════════════════════════╡  ← 清晰 API 边界
│                                                         │
│  Core GUI (独立完整的 GUI 引擎)                          │
│  ┌─────────────────────────────────────────────────┐   │
│  │ Widget Layer (内置组件)                           │   │
│  │ Button/Label/Edit/List/Panel/ScrollView/Chart... │   │
│  ├─────────────────────────────────────────────────┤   │
│  │ Core Layer (核心机制)                            │   │
│  │ Widget 树 / 事件分发 / 布局引擎 / 动画引擎       │   │
│  │ 定时器 / 样式系统 / 属性通知 / 焦点管理          │   │
│  ├─────────────────────────────────────────────────┤   │
│  │ Graphics Layer (图形引擎)                        │   │
│  │ Draw Pipeline / 矢量渲染 / 位图渲染 / 字体引擎   │   │
│  │ GPU 加速后端 / 抗锯齿 / 渐变 / 路径 / 混合       │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
├─────────────────────────────────────────────────────────┤
│  HAL Layer (硬件抽象层)                                   │
│  Display / Input / Tick / Memory / GPU / OS Abstraction  │
└─────────────────────────────────────────────────────────┘
```

### 2.2 双层设计原则

**Core GUI 层：**
- 独立完整，可单独编译运行，不依赖 DSL 层
- 提供纯命令式 C API，无魔法、可预测
- 包含 GUI 系统的全部传统功能：控件、事件、绘图、布局、动画
- 为 DSL 层预留钩子（属性变化通知、生命周期回调、批量更新）

**DSL 层：**
- 可选叠加，单向依赖 Core GUI API
- 提供声明式开发体验，降低应用开发复杂度
- 可替换：C 宏 DSL / 外部 .ui 文件 / 脚本绑定 / 可视化工具生成代码
- 实现响应式绑定、组合式组件、状态机动画编排、页面过渡

### 2.3 模块依赖关系

```
Application
    │ (可选路径 A: 通过 DSL 层)
    ▼
DSL Layer ──────────┐
    │               │
    ▼               ▼
Core GUI ═══════════╡ (DSL 调用 Core API，Core 不知道 DSL 存在)
    │
    ▼
HAL Layer
```

### 2.4 两种使用模式

**模式 A：直接使用 Core API（极致控制）**
```
// 命令式，类似传统 GUI 编程
widget_t* btn = button_create(parent);
button_set_text(btn, "Click me");
widget_set_pos(btn, 10, 20);
widget_set_size(btn, 120, 40);
button_set_on_click(btn, my_handler, user_data);
```

**模式 B：通过 DSL 层（高效开发）**
```
// 声明式，描述 UI 应该是什么样
UI_SCREEN(home,
    UI_COLUMN(spacing=8,
        UI_LABEL(bind=temperature, format="%d°C", style=STYLE_TITLE)
        UI_SLIDER(bind=target_temp, min=16, max=30)
        UI_BUTTON(text="Apply", on_click=apply_settings)
    )
)
```


---

## 3. 硬件抽象层（HAL）

HAL 层定义平台无关的接口，移植时只需实现这些接口即可适配新硬件。

### 3.1 Display 接口

```
hal_display_init(config)          — 初始化显示设备
hal_display_flush(area, buffer)   — 将缓冲区内容刷新到屏幕指定区域
hal_display_get_info() → info     — 获取屏幕宽高、色深、方向
hal_display_wait_vsync()          — 等待垂直同步
hal_display_set_backlight(level)  — 背光控制
```

支持双缓冲模式（典型配置）：
- **全帧双缓冲**：两块完整帧缓冲交替使用，GPU 渲染到后台缓冲，VSync 时切换
- **部分缓冲**：按脏区域分块渲染 + DMA 传输，适合 SPI 屏

### 3.2 GPU 接口

```
hal_gpu_init()
hal_gpu_fill_rect(dst, area, color)              — 硬件矩形填充
hal_gpu_blit(src, dst, area)                     — 硬件位块传输
hal_gpu_blend(src, dst, area, opacity)           — 带 Alpha 混合的 Blit
hal_gpu_rotate(src, dst, angle, pivot)           — 硬件旋转
hal_gpu_draw_path(path, fill_color, stroke)      — 硬件路径绘制（2.5D GPU）
hal_gpu_wait_complete()                          — 等待 GPU 操作完成
hal_gpu_is_busy() → bool                        — 查询 GPU 状态
```

GPU 加速策略：能用硬件做的绝不用软件——矩形填充、位图混合、旋转、路径渲染优先走 GPU。

### 3.3 Input 接口

```
hal_input_init()
hal_input_read(event_buf, max_count) → count    — 读取输入事件（非阻塞）
```

统一输入事件结构：
```
InputEvent {
    type:      TOUCH_DOWN | TOUCH_UP | TOUCH_MOVE | KEY_DOWN | KEY_UP
             | ENCODER_CW | ENCODER_CCW | ENCODER_PRESS
    x, y:     int16_t     (触摸坐标)
    key:      uint16_t    (键值)
    timestamp: uint32_t   (毫秒时间戳)
}
```

### 3.4 Tick/Timer 接口

```
hal_tick_get() → uint32_t         — 获取系统 tick（毫秒）
hal_delay(ms)                     — 阻塞延时
```

### 3.5 Memory 接口

```
hal_mem_alloc(size) → ptr         — 分配内存（从 GUI 专用堆）
hal_mem_free(ptr)                 — 释放内存
hal_mem_alloc_aligned(size, align) → ptr  — 对齐分配（GPU 缓冲区需要）
```

典型内存布局（1MB RAM）：
- 系统 + RTOS：~128KB
- 帧缓冲 ×2：480×480×2(RGB565) × 2 = ~900KB，或 480×480×4(ARGB) × 1 = ~900KB
- GUI 堆：~200-400KB（Widget、动画、字体缓存）

### 3.6 OS 抽象接口

```
hal_mutex_create() → handle
hal_mutex_lock(handle)
hal_mutex_unlock(handle)
hal_thread_create(func, arg, stack_size, priority) → handle
hal_thread_sleep(ms)
hal_event_wait(event, timeout_ms) → bool
hal_event_signal(event)
```

---

### 3.7 Framebuffer、GPU 与 Display 的协作关系

#### 3.7.1 整体数据流

```
Core GUI (Widget 绘制)
    │
    ▼ 绑定渲染目标
GraphicContext Backend
    │
    ├─ CPU 软件渲染 ──→ 写入后台 Framebuffer 内存
    │
    └─ GPU 硬件加速 ──→ GPU DMA 直接写入同一块后台 Framebuffer 内存
                              │
                              ▼
                    ┌──────────────────┐
                    │ 后台 FB (Back)    │ ← GUI 绘制目标
                    └────────┬─────────┘
                             │ VSync 时交换
                    ┌────────▼─────────┐
                    │ 前台 FB (Front)   │ ← Display Controller 持续输出到屏幕
                    └──────────────────┘
```

#### 3.7.2 双缓冲机制

```
FB0, FB1: 两块等大的连续内存（480×480×2 = ~450KB each，或共用一块交替区域）

时刻 T:
  Display Controller 硬件从 FB0 地址持续读取像素 → 屏幕
  GUI + GPU 绘制到 FB1

VSync 中断到来:
  交换指针: Display Controller 切换到从 FB1 读取
  GUI 开始绘制到 FB0

效果: 屏幕永远显示完整的一帧，绘制过程不可见（无撕裂）
```

#### 3.7.3 更新后的 HAL Display 接口

```
hal_display_init(config)                  — 分配 FB0/FB1，配置 Display Controller
hal_display_get_draw_buffer() → void*     — 获取当前后台缓冲地址（GUI 绘制目标）
hal_display_get_info() → { width, height, stride, pixel_format }
hal_display_flush_ready()                 — 通知一帧绘制完成，请求 VSync 交换
hal_display_wait_vsync()                  — 阻塞等待交换完成
hal_display_set_backlight(level)          — 背光控制
```

#### 3.7.4 GPU 与 Framebuffer 的关系

```
GPU 操作目标 = hal_display_get_draw_buffer() 返回的地址

即: GPU 和 CPU 软件渲染写入同一块后台 FB 内存
    GPU 通过 DMA 直接访问该内存，速度远快于 CPU 逐像素写入

关键约束:
  1. GPU 目标地址必须对齐（通常 64 字节对齐）
  2. 同一区域不能 CPU 和 GPU 同时写入 → 需要 hal_gpu_wait_complete()
  3. flush_ready() 必须在所有 GPU 操作完成之后调用
```

#### 3.7.5 一帧渲染的完整时序

```
hal_display_wait_vsync()              // 等待上一帧交换完成
fb = hal_display_get_draw_buffer()    // 获取后台缓冲

gc_bindSurface(gc, fb)                // GraphicContext 绑定到后台 FB
hal_gpu_bindTarget(fb, w, h, stride)  // GPU 也绑定到同一块 FB

// --- 绘制阶段 ---
for each dirty_widget:
    widget.on_paint(gc, dirty_rect)   // 内部混合 CPU 和 GPU 操作
    // GPU 操作异步提交，CPU 操作同步执行

hal_gpu_wait_complete()               // 确保所有 GPU 操作完成
hal_display_flush_ready()             // 通知可以交换了
// → 下一个 VSync 中断时硬件自动交换 FB0↔FB1
```

### 3.8 输入设备处理层（Input Device Layer）

HAL Input 提供原始事件，Core GUI 内部的输入设备处理层负责将原始事件转换为高级 GUI 事件。

#### 3.8.1 完整链路

```
硬件中断/轮询
    │
    ▼
hal_input_read() → 原始 InputEvent[]
    │
    ▼
InputDevice 处理层 (Core GUI 内部)
    ├─ TouchDevice:  坐标滤波 → 去抖 → 手势识别 → GUI 事件
    ├─ KeyDevice:    去抖 → 长按检测 → 重复触发 → GUI 事件
    └─ EncoderDevice: 计数 → 加速 → 导航/编辑模式 → GUI 事件
    │
    ▼
事件分发 (Hit-Test / 焦点路由)
    │
    ▼
Widget.on_event / 外部回调 / 冒泡
```

#### 3.8.2 InputDevice 注册

```
InputDevice {
    type:       INDEV_TOUCH | INDEV_KEYPAD | INDEV_ENCODER | INDEV_BUTTON
    read:       func(device) → InputEvent    // 读取回调（封装 hal_input_read）
    group:      FocusGroup*                  // 关联的焦点组（按键/编码器用）
    state:      DeviceState                  // 内部状态（手势识别用）
}

indev_register(type, read_cb, group) → InputDevice*
indev_unregister(device)
indev_set_group(device, group)       // 绑定焦点组
```

#### 3.8.3 触摸处理

```
TouchDevice 内部状态机:

  IDLE → (touch_down) → PRESSED → (移动距离 > 阈值?) 
                              ├─ 否，时间 < 300ms → WAIT_RELEASE
                              │     └─ release → 生成 EVENT_CLICKED
                              ├─ 否，时间 ≥ long_press_time → 生成 EVENT_LONG_PRESSED
                              └─ 是 → SCROLLING
                                    └─ release → 惯性滚动 → EVENT_SCROLL_THROW

坐标处理:
  raw_xy → 滤波(移动平均/卡尔曼) → 屏幕旋转映射 → 最终坐标

生成的事件:
  EVENT_PRESSED, EVENT_PRESSING, EVENT_RELEASED
  EVENT_CLICKED, EVENT_SHORT_CLICKED, EVENT_LONG_PRESSED
  EVENT_SCROLL_BEGIN, EVENT_SCROLL, EVENT_SCROLL_END
  EVENT_GESTURE(direction: LEFT/RIGHT/UP/DOWN)
```

#### 3.8.4 手势识别

```
手势判定算法:

滑动方向:
  dx = end_x - start_x
  dy = end_y - start_y
  if abs(dx) > abs(dy): 水平滑动 (dx>0 RIGHT, dx<0 LEFT)
  else: 垂直滑动 (dy>0 DOWN, dy<0 UP)
  条件: 移动距离 > GESTURE_THRESHOLD && 速度 > MIN_VELOCITY

长按:
  按下后静止 > LONG_PRESS_TIME(默认 400ms) → EVENT_LONG_PRESSED
  持续静止每 REPEAT_TIME(默认 100ms) → EVENT_LONG_PRESSED_REPEAT

双击:
  两次 CLICKED 间隔 < DOUBLE_CLICK_TIME(默认 300ms)
  且距离 < DOUBLE_CLICK_DIST → EVENT_DOUBLE_CLICKED
```

#### 3.8.5 编码器处理

```
EncoderDevice 状态机:

  旋转事件:
    count = 累计脉冲数（正=顺时针，负=逆时针）
    加速: if 旋转速度 > 阈值 → step = count * acceleration_factor
    
    导航模式: → focus_navigate(NEXT/PREV, step)
    编辑模式: → widget_send_event(focused, EVENT_KEY, KEY_UP/DOWN * step)

  按下事件:
    短按: 导航模式 → EVENT_CLICKED 或进入编辑模式
          编辑模式 → 退出编辑模式
    长按: EVENT_LONG_PRESSED

加速算法:
  dt = 当前脉冲间隔
  if dt < FAST_THRESHOLD:   step = 4
  elif dt < MEDIUM_THRESHOLD: step = 2
  else: step = 1
```

#### 3.8.6 按键处理

```
KeyDevice:
  去抖: 状态变化后等待 DEBOUNCE_TIME(默认 20ms) 确认
  长按: 按住 > LONG_PRESS_TIME → EVENT_LONG_PRESSED
  重复: 长按后每 REPEAT_TIME → EVENT_KEY_REPEAT
  释放: EVENT_KEY_UP

生成事件路由到当前焦点 Widget。
```

#### 3.8.7 输入处理在主循环中的位置

```
gui_main_loop():
    while (running):
        // 1. 输入处理（每帧执行）
        for each registered indev:
            raw_event = indev->read(indev)
            processed_event = indev_process(indev, raw_event)  // 滤波/手势/加速
            if processed_event:
                event_dispatch(root, processed_event)           // 分发到 Widget

        // 2. 异步事件队列
        // 3. 定时器
        // 4. 动画
        // 5. 刷新显示
```

---

## 4. 图形引擎（GraphicContext）

Graphics 层的核心是 **GraphicContext** — 一个功能完整的矢量图形绘图上下文抽象。它是整个 GUI 系统的图形底座，类似于 Apple CoreGraphics (CGContext)、Android Skia (SkCanvas)、WebKit GraphicsContext 的角色。

上层 Widget 只面对 GraphicContext 统一接口，底层可对接不同渲染后端：

```
Widget on_paint(gc)
        │
        ▼
┌─────────────────────────────┐
│  GraphicContext (统一抽象)    │
│  Path / Gradient / Pattern   │
│  Image / Matrix / Font       │
│  Blend / Clip / Shadow       │
└──────────────┬──────────────┘
               │ (编译时或运行时选择后端)
    ┌──────────┼──────────────┐
    ▼          ▼              ▼
┌────────┐ ┌────────┐ ┌────────────┐
│Software│ │VGLite  │ │OpenGL ES   │
│(矢量)  │ │(2.5D)  │ │(未来)      │
└────────┘ └────────┘ └────────────┘
```

### 4.1 GraphicContext 核心 API

参考 WebKit GraphicsContext + Apple CoreGraphics 设计，完整 API 如下：

#### 4.1.1 上下文管理与状态栈

```
gc_create(target_surface) → GraphicContext*
gc_destroy(gc)

gc_save(gc)                          — 保存当前状态（变换、裁剪、画笔、阴影等）入栈
gc_restore(gc)                       — 恢复上一个保存的状态

gc_bindSurface(gc, surface)          — 切换渲染目标
gc_getSurface(gc) → surface
```

#### 4.1.2 变换（Matrix / CTM）

```
gc_translate(gc, tx, ty)
gc_scale(gc, sx, sy)
gc_rotate(gc, angle_radians)
gc_concatMatrix(gc, matrix)          — 连接仿射变换矩阵
gc_setMatrix(gc, matrix)             — 直接设置 CTM
gc_getMatrix(gc) → matrix            — 获取当前 CTM
gc_resetMatrix(gc)                   — 重置为单位矩阵
```

Matrix 对象：
```
matrix_create() → Matrix*
matrix_translate(m, tx, ty)
matrix_scale(m, sx, sy)
matrix_rotate(m, angle)
matrix_invert(m)
matrix_multiply(m, other)
matrix_transformPoint(m, x, y) → (x', y')
```

#### 4.1.3 路径（Path）

Path 是矢量绘图的核心，所有形状最终都是路径：

```
path_create() → Path*
path_destroy(path)
path_reset(path)

// 路径构建
path_moveTo(path, x, y)
path_lineTo(path, x, y)
path_quadTo(path, cx, cy, x, y)          — 二次贝塞尔曲线
path_cubicTo(path, c1x, c1y, c2x, c2y, x, y)  — 三次贝塞尔曲线
path_arcTo(path, rx, ry, angle, large_arc, sweep, x, y)  — 椭圆弧（SVG 兼容）
path_close(path)

// 便捷形状（内部转为路径命令）
path_addRect(path, x, y, w, h)
path_addRoundRect(path, x, y, w, h, rx, ry)
path_addEllipse(path, cx, cy, rx, ry)
path_addCircle(path, cx, cy, r)
path_addArc(path, cx, cy, r, start_angle, sweep_angle)

// 路径查询
path_getBounds(path) → Rect
path_contains(path, x, y) → bool
path_isEmpty(path) → bool
```

#### 4.1.4 绘制操作（Stroke & Fill）

```
gc_setStrokeColor(gc, color)
gc_setStrokeGradient(gc, gradient)
gc_setStrokePattern(gc, pattern)
gc_setStrokeWidth(gc, width)
gc_setLineCap(gc, CAP_BUTT | CAP_ROUND | CAP_SQUARE)
gc_setLineJoin(gc, JOIN_MITER | JOIN_ROUND | JOIN_BEVEL)
gc_setMiterLimit(gc, limit)
gc_setDash(gc, pattern[], count, offset)     — 虚线模式

gc_setFillColor(gc, color)
gc_setFillGradient(gc, gradient)
gc_setFillPattern(gc, pattern)
gc_setFillRule(gc, FILL_WINDING | FILL_EVEN_ODD)

gc_strokePath(gc, path)              — 描边路径
gc_fillPath(gc, path)                — 填充路径
gc_strokeAndFillPath(gc, path)       — 同时描边和填充

// 便捷绘制（无需显式创建 Path）
gc_drawLine(gc, x1, y1, x2, y2)
gc_drawRect(gc, x, y, w, h)
gc_fillRect(gc, x, y, w, h)
gc_drawRoundRect(gc, x, y, w, h, rx, ry)
gc_fillRoundRect(gc, x, y, w, h, rx, ry)
gc_drawEllipse(gc, cx, cy, rx, ry)
gc_fillEllipse(gc, cx, cy, rx, ry)
gc_drawArc(gc, cx, cy, r, start, sweep)
gc_clearRect(gc, x, y, w, h)        — 清除区域（透明）
```

#### 4.1.5 渐变（Gradient）

```
gradient_createLinear(x1, y1, x2, y2) → Gradient*
gradient_createRadial(cx, cy, r, fx, fy, fr) → Gradient*
gradient_createConic(cx, cy, start_angle) → Gradient*   — 锥形渐变
gradient_addColorStop(grad, offset, color)               — offset: 0.0~1.0
gradient_setSpread(grad, SPREAD_PAD | SPREAD_REFLECT | SPREAD_REPEAT)
gradient_setMatrix(grad, matrix)                         — 渐变变换
gradient_destroy(grad)
```

#### 4.1.6 图案（Pattern）

```
pattern_create(image, REPEAT_X | REPEAT_Y | REPEAT_XY | NO_REPEAT) → Pattern*
pattern_setMatrix(pattern, matrix)
pattern_destroy(pattern)
```

#### 4.1.7 图像（Image）

```
image_createFromData(data, len, format) → Image*
image_createFromFile(path) → Image*
image_createEmpty(w, h, format) → Image*         — 创建空位图（可作为渲染目标）
image_getWidth(img) → int
image_getHeight(img) → int
image_destroy(img)

gc_drawImage(gc, img, x, y)
gc_drawImageRect(gc, img, src_rect, dst_rect)    — 源矩形→目标矩形
gc_drawImageScaled(gc, img, dst_rect)
gc_drawTiledImage(gc, img, dst_rect, src_rect, tile_rule_h, tile_rule_v)
gc_drawNineSlice(gc, img, dst_rect, insets)      — 九宫格绘制
```

#### 4.1.8 裁剪（Clip）

```
gc_clipRect(gc, x, y, w, h)         — 矩形裁剪（与当前裁剪区域取交集）
gc_clipPath(gc, path)                — 路径裁剪
gc_clipRoundRect(gc, x, y, w, h, rx, ry)
gc_clipOut(gc, rect)                 — 排除矩形区域
gc_clipOutPath(gc, path)             — 排除路径区域
gc_resetClip(gc)
```

#### 4.1.9 混合与透明（Blend & Alpha）

```
gc_setAlpha(gc, alpha)               — 全局透明度 (0.0~1.0)
gc_setCompositeOp(gc,
    COMPOSITE_SRC_OVER |             — 默认（正常绘制）
    COMPOSITE_SRC_IN |
    COMPOSITE_SRC_OUT |
    COMPOSITE_DST_OVER |
    COMPOSITE_DST_IN |
    COMPOSITE_MULTIPLY |
    COMPOSITE_SCREEN |
    COMPOSITE_OVERLAY |
    COMPOSITE_CLEAR
)
```

#### 4.1.10 阴影（Shadow）

```
gc_setShadow(gc, offset_x, offset_y, blur_radius, color)
gc_clearShadow(gc)
```

阴影自动应用于后续所有绘制操作（路径、文字、图像），直到 clearShadow 或 restore。

#### 4.1.11 透明图层（Transparency Layer）

```
gc_beginTransparencyLayer(gc, opacity)   — 开始离屏渲染图层
gc_endTransparencyLayer(gc)              — 结束图层，以指定透明度合成回主画布
```

用于实现组合透明效果（一组元素整体半透明，而非各自半透明叠加）。

#### 4.1.12 文字绘制

```
gc_setFont(gc, font)
gc_setTextColor(gc, color)
gc_drawText(gc, text, len, x, y)
gc_drawTextInRect(gc, text, len, rect, align, valign, wrap)
gc_measureText(gc, text, len) → Size
```

Font 对象：
```
font_create(family, size, weight, italic) → Font*
font_createFromData(data, len, size) → Font*
font_destroy(font)
font_getMetrics(font) → { ascent, descent, lineHeight }
```

### 4.2 后端实现接口

每个后端实现一套函数表：

```
GraphicBackend {
    name:           "software" | "vglite" | "opengles"
    create_context: func(surface) → impl_context
    destroy_context:func(impl)
    save/restore:   ...
    strokePath:     func(impl, path, stroke_state)
    fillPath:       func(impl, path, fill_state)
    drawImage:      func(impl, image, src, dst, composite_op)
    clipRect:       func(impl, rect)
    ...
}
```

GraphicContext 内部持有后端指针，所有操作转发到具体实现：
- **软件矢量后端**：纯软件矢量渲染，功能最完整，作为参考实现
- **VGLite 后端**：2.5D GPU 加速，路径/填充/混合走硬件，不支持的操作回退软件渲染
- **OpenGL ES 后端**：未来扩展，适合有 GPU 的高端平台

### 4.3 渲染模式（Immediate / Deferred / RenderThread）

GraphicContext 支持三种渲染模式，对上层 API 完全透明（Widget 代码无需修改）：

#### 4.3.1 三种模式

```
┌─────────────────────────────────────────────────────────────────┐
│ Widget.on_paint(gc, ...)    ← 上层代码完全相同                    │
└──────────────────────┬──────────────────────────────────────────┘
                       │
          ┌────────────┼────────────────┐
          ▼            ▼                ▼
   Immediate       Deferred         RenderThread
   (直接执行)      (延迟渲染)       (异步渲染线程)
                       │                │
                       ▼                ▼
                  RenderList        RenderList
                  (指令缓冲)        (指令缓冲)
                       │                │
                       ▼                │ 提交给渲染线程
                  Optimize Pass         ▼
                       │           Optimize Pass
                       ▼                │
                  Backend 执行          ▼
                                   Backend 执行（异步）
```

| 模式 | 特点 | 适用场景 |
|------|------|---------|
| Immediate | 调用即执行，零延迟，最简单 | 开发调试、简单 UI、无优化需求 |
| Deferred | 记录指令 → 优化 → 批量执行 | 复杂 UI、需要遮挡剔除/合并优化 |
| RenderThread | 主线程记录 → 渲染线程异步执行 | 高帧率要求、主线程不等 GPU |

```c
vg_context_set_mode(ctx, VG_MODE_IMMEDIATE);     // 默认
vg_context_set_mode(ctx, VG_MODE_DEFERRED);
vg_context_set_mode(ctx, VG_MODE_RENDER_THREAD);
```

#### 4.3.2 RenderList（指令缓冲区）

```c
// 指令格式（紧凑，cache 友好）
typedef struct {
    uint8_t  opcode;         // VG_CMD_FILL_RECT / VG_CMD_FILL_PATH / VG_CMD_DRAW_IMAGE ...
    uint8_t  flags;          // CMD_FLAG_OPAQUE | CMD_FLAG_HAS_CLIP | CMD_FLAG_HAS_MATRIX
    uint16_t param_offset;   // 参数在 param_buf 中的偏移
} vg_render_cmd_t;

// 指令列表
typedef struct {
    vg_render_cmd_t* cmds;       // 指令数组
    uint32_t         cmd_count;
    uint32_t         cmd_capacity;
    uint8_t*         param_buf;  // 参数数据（颜色、坐标、路径点...紧凑排列）
    uint32_t         param_size;
    uint32_t         param_capacity;
    vg_rect_t        bounds;     // 整个列表的包围盒
} vg_render_list_t;
```

Deferred 模式下 GraphicContext 的每次绘制调用（fill_path/draw_image/...）转为一条 cmd 写入 RenderList，纯内存操作，极快。

#### 4.3.3 优化 Pass

绘制完成后、后端执行前，对 RenderList 进行优化：

**Pass 1：遮挡剔除（Occlusion Culling）**
```
从后向前扫描:
  for i = cmd_count-1 downto 0:
      if cmds[i].flags & OPAQUE && is_rect_fill(cmds[i]):
          cover_rect = get_bounds(cmds[i])
          for j = 0 to i-1:
              if get_bounds(cmds[j]) 完全被 cover_rect 包含:
                  cmds[j].opcode = VG_CMD_NOP  // 标记跳过
```

效果：后绘制的不透明矩形覆盖了前面的小区域绘制 → 前面的指令直接跳过，省去无意义的渲染。

**Pass 2：同类合并（Batch Merge）**
```
扫描连续的同类指令:
  连续多个 fill_rect + 相同颜色 + 无重叠 → 合并为一次 GPU 批量填充
  连续多个 draw_image + 同一图集 → 合并为一次批量 Blit
```

**Pass 3：状态排序（State Sort，可选）**
```
按 GPU 状态分组（减少状态切换）:
  所有不透明填充 → 一组
  所有带 Alpha 混合 → 一组
  所有图像绘制 → 一组
注意: 必须保证不改变视觉结果（只对无重叠的指令重排）
```

#### 4.3.4 RenderThread 模式时序

```
主线程                              渲染线程
────────                            ────────
帧 N:
  事件处理 / 动画计算
  Widget.on_paint → 写入 RenderList_A
  提交 RenderList_A ──────────────→ 收到 RenderList_A
  开始帧 N+1 的事件处理              Optimize Pass
  （不等待 GPU）                     Backend 执行（GPU）
                                    hal_gpu_wait_complete()
                                    hal_display_flush_ready()
                                    ←──────────── 通知完成
帧 N+1:
  Widget.on_paint → 写入 RenderList_B
  提交 RenderList_B ──────────────→ ...
```

双 RenderList 乒乓：主线程写 A 时渲染线程执行 B，下一帧交换。

#### 4.3.5 与 LVGL DrawPipeline 的对比

| | VG RenderList | LVGL DrawTask Pipeline |
|--|---|---|
| 指令粒度 | 纯绘图原语（fill_rect/fill_path/draw_image） | Widget 级描述（draw_rect_dsc/draw_label_dsc） |
| 数据布局 | 紧凑线性缓冲区（连续内存，cache 友好） | 链表 + 独立 Descriptor 分配（碎片化） |
| 优化能力 | 遮挡剔除 + 合并 + 状态排序 | 主要是 cover_check（Widget 级遮挡） |
| 模式灵活性 | Immediate / Deferred / RenderThread 三选一 | 仅 Deferred |
| 多线程 | 原生支持 RenderThread | 需外部实现 |
| 上层感知 | 完全透明（API 不变） | 完全透明 |

#### 4.3.6 实现优先级

```
P0: Immediate 模式（直接执行，先跑起来）
P2: Deferred + RenderList + Optimize Pass（性能优化）
P3: RenderThread（多线程异步渲染）
```

### 4.4 设计要点

1. **Path 是一等公民** — 所有形状都是路径，圆角矩形/圆/弧只是便捷 API
2. **状态栈语义** — save/restore 保存完整绘图状态（CTM、裁剪、画笔、阴影），与 WebKit/CoreGraphics 一致
3. **Gradient/Pattern 作为填充源** — 可替代纯色作为 stroke/fill 的源，统一处理
4. **CompositeOp 控制混合** — Porter-Duff 合成模式，支持复杂图层效果
5. **Shadow 自动应用** — 设置后对所有后续绘制生效，简化调用方
6. **TransparencyLayer** — 离屏渲染+合成，实现组合透明和复杂视觉效果
7. **后端无感切换** — 上层代码零修改，编译时选择后端即可

### 4.5 字体引擎与文本排版

作为纯矢量 GUI 系统，矢量字体是文本渲染的核心。字体子系统分为四层：

```
┌─────────────────────────────────────┐
│ Text Layout Engine (文本排版引擎)     │
│ 断行/对齐/换行/省略/BiDi            │
├─────────────────────────────────────┤
│ Font System (字体系统)               │
│ FontFamily/FontFace/Fallback Chain  │
├─────────────────────────────────────┤
│ Glyph Cache (字形缓存)              │
│ LRU 位图缓存 / 字形度量缓存         │
├─────────────────────────────────────┤
│ Font Backend (字体后端)              │
│ TrueType 解析 / 光栅化 / Hinting    │
└─────────────────────────────────────┘
```

#### 4.4.1 Font Backend（字体后端）

负责 TrueType/OpenType 字体文件的解析和字形光栅化：

```
font_backend_open(data, data_size) → FontFace*
font_backend_close(face)
font_backend_get_glyph_id(face, codepoint) → glyph_id
font_backend_get_metrics(face, size) → { ascent, descent, line_gap }
font_backend_get_advance(face, glyph_id, size) → advance_width
font_backend_get_kerning(face, glyph1, glyph2, size) → kern_offset
font_backend_rasterize(face, glyph_id, size, antialias) → GlyphBitmap
font_backend_get_outline(face, glyph_id, size) → Path*   // 矢量轮廓（GPU 路径渲染用）
```

后端实现选择：
- **TinyTTF**：极轻量（~10KB 代码），支持 TrueType 基本功能，适合默认集成
- **FreeType 子集**：功能更完整（hinting、OpenType 特性），代码量较大（~100KB）
- **自研精简解析器**：只解析 cmap/glyf/head/hhea/hmtx 表，最小化实现

#### 4.4.2 Glyph Cache（字形缓存）

矢量字体光栅化开销大，必须缓存已渲染字形：

```
GlyphCache {
    entries:    GlyphEntry[CACHE_SIZE]   // LRU 缓存池
    hash_table: uint16_t[]              // 快速查找
    atlas:      uint8_t*                // 字形图集位图（紧凑排列）
    atlas_w, atlas_h: uint16_t
    cursor_x, cursor_y: uint16_t       // 图集当前写入位置
}

GlyphEntry {
    key:    { font_id, glyph_id, size }  // 缓存键
    rect:   { x, y, w, h }              // 在图集中的位置
    metrics:{ bearing_x, bearing_y, advance }
    lru:    uint32_t                     // 最近使用时间戳
}

glyph_cache_get(font, glyph_id, size) → GlyphEntry*
  - 命中：更新 LRU 时间戳，返回缓存条目
  - 未命中：调用 backend 光栅化，写入图集，淘汰最久未用条目
```

缓存策略：
- **图集大小**：典型 256×256 或 512×512（A8 格式，~64-256KB）
- **LRU 淘汰**：图集满时淘汰最久未使用的字形，腾出空间
- **预热**：启动时预渲染 ASCII + 常用中文字符
- **多尺寸独立缓存**：同一字符不同字号分别缓存

#### 4.4.3 Font System（字体系统）

管理字体族、字重、Fallback 链：

```
FontFamily {
    name:       const char*          // "sans-serif", "monospace", 或自定义名
    faces:      FontFace*[]          // 不同字重/样式的字体面
}

FontFace {
    family:     FontFamily*
    weight:     WEIGHT_LIGHT | WEIGHT_REGULAR | WEIGHT_BOLD
    style:      STYLE_NORMAL | STYLE_ITALIC
    source:     FontSource            // 字体数据来源
    backend:    void*                 // 后端句柄
}

FontSource {
    type:   SOURCE_MEMORY |           // 内存中的字体数据（Flash 映射）
            SOURCE_FILE |             // 文件系统路径
            SOURCE_EXTERNAL_CHIP      // 外部字库芯片（SPI 读取）
    data:   void*
    size:   uint32_t
}
```

Font 对象（用户面对的接口）：
```
Font {
    family:     FontFamily*
    size:       uint8_t              // 像素大小
    weight:     uint8_t
    style:      uint8_t
}

font_create(family_name, size, weight, style) → Font*
font_destroy(font)
font_get_metrics(font) → { ascent, descent, line_height }
font_measure_char(font, codepoint) → advance
font_measure_text(font, text, len) → { width, height }
```

#### 4.4.4 Font Fallback（字体回退链）

单个字体无法覆盖所有 Unicode 字符（尤其 CJK + Emoji），需要 Fallback 机制：

```
Fallback Chain 配置：
  sans-serif → [Roboto, NotoSansCJK, NotoEmoji, LastResort]

查找流程：
  render_glyph(font, codepoint):
    1. 在 font 对应的 FontFace 中查找 glyph_id
    2. 若 glyph_id == 0（字形不存在）→ 尝试 Fallback 链下一个字体
    3. 逐个尝试直到找到，或使用 .notdef 占位符（□）

// API
font_system_set_fallback(family_name, fallback_list[], count)
font_system_add_font(family_name, weight, style, source)
font_system_set_default(family_name)
```

典型配置（中文智能手表）：
```
默认字体:    "system-ui" → 精简版思源黑体（常用 3000 字，~1MB Flash）
Fallback 1:  外部字库芯片（完整 CJK，通过 SPI 按需读取）
Fallback 2:  内置 LastResort（□ 占位符）
```

#### 4.4.5 Text Layout Engine（文本排版引擎）

将字符串转换为可绘制的字形序列+位置：

```
TextLayout {
    // 输入
    text:       const char* (UTF-8)
    font:       Font*
    max_width:  int16_t         // 最大宽度（用于换行）
    align:      ALIGN_LEFT | ALIGN_CENTER | ALIGN_RIGHT
    overflow:   OVERFLOW_CLIP | OVERFLOW_ELLIPSIS | OVERFLOW_SCROLL
    line_space: int8_t          // 额外行间距
    letter_space: int8_t        // 字间距

    // 输出
    lines:      TextLine[]
    total_height: int16_t
}

TextLine {
    glyphs:     GlyphRun[]      // 字形序列
    width:      int16_t         // 行宽
    baseline_y: int16_t         // 基线 Y 坐标
}

GlyphRun {
    font_face:  FontFace*       // 实际使用的字体（可能是 fallback 的）
    glyph_ids:  uint16_t[]
    positions:  { x, y }[]     // 每个字形的位置
    count:      uint16_t
}
```

排版流程：
```
text_layout(text, font, max_width, options) → TextLayout:
  1. UTF-8 解码 → codepoint 序列
  2. 逐字符查找字形（含 Fallback）
  3. 应用 Kerning（字偶间距调整）
  4. 断行算法：
     - 贪心断行：逐词累加宽度，超出则换行
     - 断行点：空格、CJK 字符间、连字符后
  5. 对齐：根据 align 计算每行起始 X 偏移
  6. 省略号：最后一行超出时截断 + 追加 "..."
  7. 输出 GlyphRun 序列
```

#### 4.4.6 文本渲染（双路径策略）

字体渲染根据字号自动选择最优路径：

```
┌─────────────────────────────────────────────────────────────┐
│ 路径 A：位图缓存 + GPU Blit（默认，字号 ≤ 阈值）             │
│                                                             │
│ TTF → 光栅化为 A8 位图 → Glyph Cache 图集                   │
│ → gc_blendGlyph() → GPU Alpha Blend 到帧缓冲               │
│                                                             │
│ 优点：极快（一次 DMA Blit/字形），适合常规 UI 文字            │
│ 缺点：缩放/旋转会模糊                                       │
├─────────────────────────────────────────────────────────────┤
│ 路径 B：矢量路径 + GPU 路径填充（大字号 > 阈值或有变换）      │
│                                                             │
│ TTF → 提取 glyf 轮廓 → 转换为 Path 对象                    │
│ → gc_fillPath(gc, glyph_path) → VGLite GPU 路径填充         │
│                                                             │
│ 优点：无损缩放/旋转，任意变换下保持清晰                      │
│ 缺点：每字形一次路径填充，开销大于 Blit                      │
└─────────────────────────────────────────────────────────────┘

路径选择逻辑：
  if (font_size > GUI_CFG_GLYPH_PATH_THRESHOLD    // 默认 48px
      || gc_has_rotation(gc)
      || gc_has_scale(gc)):
      使用路径 B（矢量）
  else:
      使用路径 A（位图缓存）
```

GPU 加速点：

| 环节 | 方式 | GPU 参与 |
|------|------|---------|
| 光栅化 | Font Backend CPU 执行，结果写入 Glyph Cache | 否（一次性） |
| 小字绘制 | 从图集 Blit A8 位图 + text_color 混合 | ✅ GPU Alpha Blend |
| 大字绘制 | Path 轮廓 → gc_fillPath | ✅ GPU 路径填充 |
| 阴影文字 | 偏移绘制一次（模糊色）+ 正常绘制一次 | ✅ 两次 Blit/Fill |

绘制实现：
```
gc_drawText(gc, font, text, len, x, y):
    layout = text_layout_simple(text, font, len)  // 单行快速排版
    draw_glyph_runs(gc, layout)

draw_glyph_runs(gc, layout):
    for each run in layout.glyph_runs:
        if use_path_rendering(run.font, gc):
            // 路径 B
            for each glyph in run:
                path = font_backend_get_outline(run.face, glyph.id, size)
                gc_save(gc)
                gc_translate(gc, glyph.pos_x, glyph.pos_y)
                gc_fillPath(gc, path)
                gc_restore(gc)
        else:
            // 路径 A
            for each glyph in run:
                entry = glyph_cache_get(run.face, glyph.id, size)
                gc_blendGlyph(gc, entry, glyph.pos_x, glyph.pos_y)
```

#### 4.4.7 Kerning 处理

Kerning（字偶间距）在 Text Layout 阶段处理，不在渲染阶段。

数据来源：
- `kern` 表（传统 TrueType）：排序的字符对→偏移量映射，O(log n) 二分查找
- `GPOS` 表（OpenType）：更复杂的定位规则（可选支持）

处理流程：
```
text_layout_compute_positions(text, font):
    prev_glyph = 0
    cursor_x = 0

    for each codepoint in text:
        glyph_id = font_backend_get_glyph_id(face, codepoint)

        // Kerning 调整
        if prev_glyph != 0:
            kern = get_kerning(face, prev_glyph, glyph_id, size)
            cursor_x += kern    // 通常为负值（字符靠近）

        // 记录字形位置
        positions[i].x = cursor_x + bearing_x
        positions[i].y = baseline_y - bearing_y
        cursor_x += advance_width + letter_spacing

        prev_glyph = glyph_id
```

性能优化：
```
get_kerning(face, glyph1, glyph2, size):
    // CJK 字符间无 kerning，直接跳过
    if is_cjk_range(glyph1) && is_cjk_range(glyph2):
        return 0

    // 查 kern 表（二分查找，几百对，极快）
    return font_backend_get_kerning(face, glyph1, glyph2, size)
```

#### 4.4.8 字体资源管理策略

```
字体存储分层：
┌──────────────────────────────────────────────────┐
│ 内部 Flash（XIP 直接映射，零拷贝访问）             │
│ - 系统 UI 字体子集（ASCII + 常用汉字 Top 3000）   │
│ - 约 500KB-1MB                                   │
├──────────────────────────────────────────────────┤
│ 外部 Flash / 字库芯片（SPI 按需读取）              │
│ - 完整 CJK 字库（GB2312/GBK/Unicode CJK）        │
│ - 约 5-10MB                                      │
│ - 按需加载：首次使用某字形时读取并缓存             │
├──────────────────────────────────────────────────┤
│ 文件系统（可选）                                   │
│ - OTA 更新字体 / 用户自定义字体                   │
└──────────────────────────────────────────────────┘

离线工具链：
  font_subset  — 从完整 TTF 提取指定字符集，生成精简字体
  font_convert — 转换为框架优化的二进制格式（快速 mmap 加载）
  font_preview — PC 端预览字体渲染效果
```



---


### 4.6 图像系统（Image System）

图像系统以**插件式解码器框架**为核心，SVG 作为一等公民，同时支持光栅图片格式。

#### 4.5.1 架构总览

```
┌─────────────────────────────────────────────────────┐
│ Image API（上层接口）                                 │
│ image_create / image_draw / image_get_frame          │
├─────────────────────────────────────────────────────┤
│ Image Object（图像对象，统一抽象）                     │
│ 多帧管理 / 动画控制 / 尺寸查询                        │
├─────────────────────────────────────────────────────┤
│ Decoded Frame Cache（解码帧缓存，LRU，内存可控）       │
├─────────────────────────────────────────────────────┤
│ ImageDecoder 插件框架                                │
│ ┌───────┐ ┌──────┐ ┌──────┐ ┌─────┐ ┌─────┐       │
│ │SVG    │ │PNG   │ │JPEG  │ │WebP │ │GIF  │ ...   │
│ │Decoder│ │Decode│ │Decode│ │Decod│ │Decod│       │
│ └───────┘ └──────┘ └──────┘ └─────┘ └─────┘       │
├─────────────────────────────────────────────────────┤
│ GraphicContext（绘制输出）                            │
└─────────────────────────────────────────────────────┘
```

#### 4.5.2 ImageDecoder 插件接口

参考 WebKit ImageDecoder 设计，每种格式实现统一接口：

```
ImageDecoder {
    can_decode(data, size) → bool             // magic bytes 识别格式
    set_data(data, size, all_received)        // 设置源数据（支持渐进加载）
    is_size_available() → bool
    get_size() → { width, height }
    get_frame_count() → int                   // 多帧图片的帧数
    get_repetition_count() → int              // 动画循环次数（-1=无限）
    decode_frame(index) → DecodedFrame*       // 解码指定帧
    get_frame_duration(index) → uint32_t      // 帧间隔 ms
    get_frame_disposal(index) → DisposalMethod
    supports_alpha() → bool
    destroy()
}

// 注册
image_register_decoder(decoder_vtable)
// 框架自动匹配：遍历已注册解码器，调用 can_decode() 找到能处理的
```

#### 4.5.3 SVG 解码器（一等公民）

SVG Tiny 1.2 子集作为系统图标和 Theme 资源的主要格式：

```
SVGDecoder : ImageDecoder {
    decode_frame(index):
        解析 SVG → 生成 Path/Gradient 绘制指令序列
        缓存指令序列（非位图）

    is_vector() → true
    get_frame_count():
        有 SMIL 动画 → 按帧率计算帧数
        静态 → 1

    // 特有接口：矢量直绘
    draw_vector(gc, dst_rect, override_color)
        回放 Path/Fill/Stroke 指令到 GraphicContext
}
```

SVG 两种渲染模式：
- **矢量直绘**：回放 Path 指令到 gc（缩放/旋转无损，动画必须用此模式）
- **缓存光栅化**：首次光栅化为位图缓存，后续 Blit（静态图标最快）

自动选择：
```
if animated || size_changed || has_transform:
    矢量直绘
else:
    缓存光栅化（Blit）
```

#### 4.5.4 帧缓存（Decoded Frame Cache）

```
DecodedFrame {
    pixels:     uint8_t*         // ARGB8888 像素
    width:      uint16_t
    height:     uint16_t
    stride:     uint16_t
    has_alpha:  bool
    duration:   uint16_t         // 帧显示时长 ms
    disposal:   DisposalMethod   // GIF 帧处置方式
}

FrameCache {
    max_memory: uint32_t         // 上限（GUI_CFG_IMAGE_CACHE_SIZE，默认 200KB）
    used_memory:uint32_t
    entries:    LRU 链表

    get(image_id, frame_index) → DecodedFrame*   // 命中返回，未命中解码并缓存
    evict_lru()                                  // 超限时淘汰最久未用帧
}
```

内存控制策略：
- 缓存总量上限可配置，超限自动淘汰
- GIF 多帧只缓存当前帧 + 前一帧（disposal 需要），不缓存全部
- SVG 矢量模式不占帧缓存（指令序列远小于位图）
- 解码按需触发，不预解码所有帧

#### 4.5.5 Image 对象

```
Image {
    source:         { type, data, size }   // MEMORY | FILE | ASSET_ID
    decoder:        ImageDecoder*
    width, height:  uint16_t
    frame_count:    uint16_t
    is_animated:    bool
    is_vector:      bool                   // SVG 标记

    // 动画状态
    current_frame:  uint16_t
    frame_timer:    uint32_t
    repeat_count:   int16_t
    playing:        bool
}
```

#### 4.5.6 Image API

```
// 创建
image_create_from_data(data, size) → Image*
image_create_from_file(path) → Image*
image_create_from_asset(asset_id) → Image*

// 动画控制
image_play(img) / image_pause(img) / image_stop(img)
image_set_frame(img, index)
image_tick(img, now_ms)              // 主循环推进动画帧

// 通过 GraphicContext 绘制
gc_drawImage(gc, img, x, y)
gc_drawImageRect(gc, img, src_rect, dst_rect)
gc_drawImageScaled(gc, img, dst_rect)
gc_drawImageRotated(gc, img, cx, cy, angle)
gc_drawNineSlice(gc, img, dst_rect, insets)

// 销毁
image_destroy(img)
```

#### 4.5.7 SVG 在 Theme 中的应用

```
// 图标资源：SVG 文件，编译时转为二进制资源
ThemeIcons {
    icon_back, icon_home, icon_settings, icon_battery[], ...
}

// 绘制时自动适配尺寸和颜色
gc_drawIcon(gc, theme->icon_back, rect, current_color)
  → SVG 矢量直绘，fill 颜色替换为 current_color
  → 天然适配任意分辨率和主题色切换
```

## 5. 核心框架（Core Framework）

Core 层是框架的心脏，定义 Widget 基类、事件分发、布局计算和主循环。

### 5.1 Widget 基类

所有 UI 组件继承自 Widget 基类，采用**双轨模型**：vtable 定义类型行为（高性能），外部事件回调提供应用扩展（高灵活性）。

#### 5.1.1 Widget 数据结构

```
Widget {
    // 树形关系
    parent:     Widget*
    children:   Widget*  (链表头，侵入式单链表)
    next:       Widget*  (兄弟节点)

    // 几何属性
    x, y:       int16_t  (相对父控件的坐标)
    w, h:       int16_t  (宽高)

    // 状态标志 (位域，2字节)
    flags:      uint16_t
        VISIBLE     : 1
        ENABLED     : 1
        FOCUSABLE   : 1
        TOPMOST     : 1
        EDITABLE    : 1
        DIRTY       : 1
        CAPTURED    : 1
        SCROLLABLE  : 1
        EVENT_BUBBLE: 1   // 事件冒泡到父控件
        reserved    : 7

    // 类型定义（vtable，编译时确定）
    class:      WidgetClass*

    // 外部事件回调链（运行时动态添加）
    events:     EventCB*  (链表头，可为 NULL)

    // 样式（可选，NULL 则使用 Theme 默认）
    style:      Style*

    // 用户数据
    user_data:  void*
}
```

#### 5.1.2 WidgetClass（类型定义，替代纯 vtable）

```
WidgetClass {
    base_class:     WidgetClass*       // 父类（支持继承链）
    instance_size:  uint16_t           // 实例大小（含私有数据）
    name:           const char*        // 类型名（调试用）

    // 生命周期
    constructor:    func(widget)
    destructor:     func(widget)

    // 核心虚函数（框架内部调用，高性能路径）
    on_paint:       func(widget, gc, dirty_rect)
    on_event:       func(widget, event) → bool
    on_layout:      func(widget)
    on_resize:      func(widget, w, h)

    // 默认尺寸
    default_width:  int16_t
    default_height: int16_t
}
```

继承链示例：`lv_obj_class → button_class → icon_button_class`
子类 on_paint 可调用 `base_class->on_paint(widget, gc, rect)` 实现继承绘制。

#### 5.1.3 外部事件回调（应用层扩展）

```
EventCB {
    next:       EventCB*           // 链表
    handler:    func(event)        // 回调函数
    filter:     uint16_t           // 事件过滤码（EVENT_ALL 接收全部）
    user_data:  void*              // 用户数据
}

// API
widget_add_event_cb(widget, handler, event_code, user_data) → EventCB*
widget_remove_event_cb(widget, cb)
widget_send_event(widget, event_code, param)
```

#### 5.1.4 事件分发流程（vtable + 外部回调融合）

```
事件到达 Widget:
  1. 调用 class->on_event(widget, event)     ← vtable 处理（类型内置行为）
     - 返回 true 表示已消费，跳过后续
  2. 遍历 events 链表，匹配 filter 的回调依次调用  ← 外部回调（应用逻辑）
  3. 若 flags & EVENT_BUBBLE 且未消费 → 传递给 parent
```

#### 5.1.5 事件冒泡

```
widget_add_flag(widget, FLAG_EVENT_BUBBLE)

// 父容器统一处理所有子控件的点击：
widget_add_event_cb(container, on_child_click, EVENT_CLICKED, NULL);

// 在回调中区分来源：
void on_child_click(Event* e) {
    Widget* clicked = event_get_target(e);       // 原始触发者
    Widget* self = event_get_current_target(e);  // 当前处理者（container）
    // ...
}
```

#### 5.1.6 Widget 树操作

```
widget_create(class, parent) → Widget*       // 创建（自动调用 constructor）
widget_destroy(widget)                        // 销毁（自动调用 destructor + 递归销毁子控件）
widget_add_child(parent, child)
widget_remove_child(parent, child)
widget_set_parent(widget, new_parent)         // 重新挂载

widget_set_pos(widget, x, y)
widget_set_size(widget, w, h)
widget_invalidate(widget, rect)               // 标记脏区域
widget_show(widget) / widget_hide(widget)
widget_set_focus(widget)

widget_add_flag(widget, flag)
widget_clear_flag(widget, flag)
widget_has_flag(widget, flag) → bool
widget_get_state(widget) → state              // NORMAL/PRESSED/FOCUSED/DISABLED
```

#### 5.1.7 自定义 Widget 示例

```c
// 1. 定义私有数据
typedef struct {
    Widget base;          // 必须是第一个成员
    int32_t value;
    int32_t min, max;
} MySlider;

// 2. 实现核心函数
static void slider_paint(Widget* w, GraphicContext* gc, Rect* r) {
    MySlider* s = (MySlider*)w;
    // 绘制轨道
    gc_setFillColor(gc, theme->color_surface);
    gc_fillRoundRect(gc, 0, h/2-4, w->w, 8, 4, 4);
    // 绘制滑块
    float ratio = (float)(s->value - s->min) / (s->max - s->min);
    int knob_x = ratio * (w->w - 20);
    gc_setFillColor(gc, theme->color_primary);
    gc_fillCircle(gc, knob_x + 10, h/2, 10);
}

static bool slider_event(Widget* w, Event* e) {
    MySlider* s = (MySlider*)w;
    if (e->type == EVENT_TOUCH_MOVE && widget_has_flag(w, FLAG_CAPTURED)) {
        s->value = s->min + (e->x * (s->max - s->min)) / w->w;
        widget_invalidate(w, NULL);
        widget_send_event(w, EVENT_VALUE_CHANGED, &s->value);
        return true;
    }
    return false;
}

// 3. 定义 WidgetClass
const WidgetClass slider_class = {
    .base_class = &base_widget_class,
    .instance_size = sizeof(MySlider),
    .name = "Slider",
    .constructor = slider_constructor,
    .on_paint = slider_paint,
    .on_event = slider_event,
    .default_width = 200,
    .default_height = 40,
};

// 4. 使用
Widget* slider = widget_create(&slider_class, parent);
widget_add_event_cb(slider, on_temp_changed, EVENT_VALUE_CHANGED, &temperature);
```

#### 5.1.8 与 LVGL 对比总结

| 方面 | 我们的设计 | LVGL |
|------|-----------|------|
| 类型定义 | WidgetClass (vtable + 元信息) | lv_obj_class_t |
| 内置行为 | vtable 虚函数（直接调用，零分发开销） | 统一 event_cb（switch/case 分发） |
| 应用扩展 | widget_add_event_cb（外部回调链） | lv_obj_add_event_cb（相同） |
| 事件冒泡 | 支持（FLAG_EVENT_BUBBLE） | 支持（LV_OBJ_FLAG_EVENT_BUBBLE） |
| 绘制方式 | on_paint 直接调用 GraphicContext | DRAW 事件 + Draw Descriptor |
| 继承 | base_class 指针链 | base_class 指针链（相同） |
| 优势 | 绘制路径更短、类型行为更高效 | 样式系统更强大、Parts 机制 |

### 5.2 事件系统

借鉴 TouchWeb 的双轨事件模型：同步分发 + 异步队列。

#### 5.2.1 事件类型

```
Event {
    type:  EVENT_TOUCH_DOWN | EVENT_TOUCH_UP | EVENT_TOUCH_MOVE
         | EVENT_KEY_DOWN | EVENT_KEY_UP
         | EVENT_FOCUS_IN | EVENT_FOCUS_OUT
         | EVENT_TIMER | EVENT_CUSTOM
    union {
        touch: { x, y, id }
        key:   { code, modifiers }
        timer: { timer_id }
        custom:{ param1, param2 }
    }
}
```

#### 5.2.2 同步事件分发（Hit-Test 冒泡）

触摸/鼠标事件的分发流程：

```
input_dispatch(root, event):
    1. 若有 captured_widget → 直接转发（坐标转换为局部坐标）
    2. 从 root 开始递归 Hit-Test:
       a. 逆序遍历 topmost 子控件
       b. 逆序遍历普通子控件
       c. 检查 bounds.contains(x, y) && visible && enabled
       d. 坐标转换: child_x = x - child.x, child_y = y - child.y
       e. 递归进入命中的子控件
    3. 若无子控件命中 → 当前控件处理 on_event()
```

#### 5.2.3 异步事件队列（PostEvent）

```
PostEvent {
    target:   Widget*
    handler:  function_ptr
    param:    void*
}

event_post(target, handler, param, priority)  — 投递事件
event_dispatch_pending() → bool               — 处理一个排队事件
event_cancel(target)                          — 取消目标的所有待处理事件
```

队列实现：静态环形缓冲区（大小可配置，默认 32 条），避免动态分配。

#### 5.2.4 事件传播三阶段（Capture → Target → Bubble）

参考 DOM 事件模型，事件传播分三个阶段：

```
事件分发流程:

  1. Capture 阶段（从根到目标，祖先可拦截）
     path = [root → ... → parent → target]
     for ancestor in path (根→目标方向):
         if ancestor.flags & FLAG_EVENT_CAPTURE:
             if ancestor.on_event(event, PHASE_CAPTURE) == consumed:
                 return  // 被拦截，事件终止

  2. Target 阶段（目标控件处理）
     target.class->on_event(target, event)     // vtable
     遍历 target.events 外部回调链              // 外部回调

  3. Bubble 阶段（从目标向上冒泡）
     if target.flags & FLAG_EVENT_BUBBLE:
         parent.class->on_event(parent, event)
         遍历 parent.events 外部回调链
         继续向上，直到某层返回 consumed 或到达根
```

**Capture 的典型用途 — ScrollView 手势拦截：**

```
场景: ScrollView 内有 Button，用户触摸 Button 区域

  touch_down:
    Capture 阶段 → ScrollView 拦截，暂不分发给 Button
    启动手势判定计时器

  后续 touch_move 距离 > 阈值:
    确认是滑动 → ScrollView 自己处理滚动
    Button 收到 EVENT_PRESS_LOST

  后续 touch_up 且未移动:
    确认是点击 → 放行，Button 收到完整的 DOWN + UP + CLICKED
```

**API：**

```
vg_widget_add_flag(widget, VG_FLAG_EVENT_CAPTURE)   // 启用 Capture 拦截
vg_widget_add_flag(widget, VG_FLAG_EVENT_BUBBLE)    // 启用冒泡
```

#### 5.2.5 触摸 Capture（指针捕获）

独立于事件传播的三阶段，指针捕获用于拖拽场景：

```
vg_widget_capture_touch(widget)     — 后续所有触摸事件直接发给该控件（跳过 Hit-Test）
vg_widget_release_touch()           — 释放捕获，恢复正常 Hit-Test

用途: 拖拽滑块时，手指移出控件边界仍需持续接收 touch_move
```

### 5.3 绘制流程

#### 5.3.1 脏矩形管理

```
invalidate(widget, rect):
    1. 将 rect 转换为屏幕绝对坐标
    2. 与现有脏区域合并（取并集的外接矩形）
    3. 标记 widget.flags |= DIRTY
```

#### 5.3.2 绘制遍历

```
paint_tree(root, dc, clip_rect):
    if !root.visible || !bounds_intersect(root, clip_rect):
        return
    dc_save(dc)
    dc_setClipRect(dc, intersect(root.bounds, clip_rect))
    dc_translate(dc, root.x, root.y)

    // 绘制自身
    local_clip = to_local(clip_rect)
    root.vtable->on_paint(root, dc, local_clip)

    // 绘制普通子控件（正序，先添加的先绘制）
    for child in root.children (non-topmost):
        paint_tree(child, dc, local_clip)

    // 绘制置顶子控件
    for child in root.children (topmost):
        paint_tree(child, dc, local_clip)

    dc_restore(dc)
```

#### 5.3.3 刷新策略

```
refresh():
    if no dirty region: return
    dc_bindBuffer(framebuffer)
    paint_tree(root, dc, dirty_region)
    hal_display_flush(dirty_region, framebuffer)
    clear dirty_region
```

### 5.4 布局系统

#### 5.3.4 Layer 缓存（P3 优化方向）

可选的离屏缓存机制，标记为后续优化方向：

```
vg_widget_add_flag(widget, VG_FLAG_CACHE_LAYER)

行为:
  首次绘制/内容变化时: on_paint → 渲染到离屏 Surface → 标记缓存有效
  后续帧(缓存有效时):  跳过 on_paint → GPU Blit Surface 到 FB（极快）
  位移/旋转/透明度动画: 只改 Blit 参数，不触发 on_paint

缓存失效条件:
  - 内容变化（文本/值/子控件改变）
  - 尺寸变化
  - 手动 vg_widget_invalidate()

代价: 每个缓存 Widget 额外占用 w×h×bpp 内存（空间换时间）
适用: 静态背景、复杂矢量图标、页面过渡时的整页截图
```


提供两种布局模式，可混合使用：

#### 5.4.1 绝对布局（默认）

直接设置 x, y, w, h，与 TouchWeb 一致，零开销。

#### 5.4.2 简单自动布局（可选模块）

```
Layout {
    type:    LAYOUT_VERTICAL | LAYOUT_HORIZONTAL | LAYOUT_GRID
    padding: int8_t [4]   // top, right, bottom, left
    spacing: int8_t       // 子控件间距
    align:   ALIGN_START | ALIGN_CENTER | ALIGN_END
}

widget_set_layout(parent, layout)
widget_layout_children(parent)    — 重新计算子控件位置
```

布局计算在 `on_resize` 或显式调用时触发，不会每帧执行。

### 5.5 主循环

```
gui_main_loop():
    while (running):
        // 1. 读取输入
        count = hal_input_read(event_buf)
        for i in 0..count:
            input_dispatch(root, event_buf[i])

        // 2. 处理异步事件队列
        while event_dispatch_pending():
            pass

        // 3. 定时器检查
        timer_check_expired()

        // 4. 空闲回调（动画、后台任务）
        widget_fire_idle(root)

        // 5. 刷新显示
        refresh()

        // 6. 低功耗等待（可选）
        hal_wait_for_event_or_timeout(next_timer_ms)
```

### 5.6 定时器

```
timer_create(interval_ms, callback, param, repeat) → timer_id
timer_delete(timer_id)
timer_reset(timer_id)
timer_check_expired()   — 在主循环中调用，触发到期回调
```

实现：静态数组（默认 8 个定时器槽位），基于 `hal_tick_get()` 判断到期。

### 5.7 样式系统

#### 5.7.1 设计目标

- Widget 外观与逻辑分离，切换主题不改代码
- Part × State 组合，一个 Widget 的不同部件在不同状态下有不同外观
- 状态切换自动触发过渡动画
- 非强制：开发者可选择用样式系统自动绘制，也可在 on_paint 中手动绘制

#### 5.7.2 核心概念

```
Part（部件）— 一个 Widget 的可独立设置样式的组成部分：
  PART_MAIN        — 主体/背景
  PART_INDICATOR   — 指示器（如进度条已填充部分）
  PART_KNOB        — 旋钮/滑块
  PART_SCROLLBAR   — 滚动条
  PART_HEADER      — 标题栏
  PART_ITEMS       — 列表项

State（状态）— 可组合的位标志：
  STATE_NORMAL     = 0x00
  STATE_PRESSED    = 0x01
  STATE_FOCUSED    = 0x02
  STATE_DISABLED   = 0x04
  STATE_CHECKED    = 0x08
  STATE_HOVERED    = 0x10
```

#### 5.7.3 样式属性

```
StyleProperty 枚举（约 40 个，按类别分组）：

// 背景
PROP_BG_COLOR           color_t
PROP_BG_OPACITY         uint8_t (0-255)
PROP_BG_GRADIENT        Gradient*

// 边框
PROP_BORDER_COLOR       color_t
PROP_BORDER_WIDTH       uint8_t
PROP_BORDER_OPACITY     uint8_t
PROP_BORDER_SIDE        uint8_t (TOP|RIGHT|BOTTOM|LEFT)

// 圆角
PROP_RADIUS             uint16_t

// 轮廓（focus 指示环）
PROP_OUTLINE_COLOR      color_t
PROP_OUTLINE_WIDTH      uint8_t
PROP_OUTLINE_OFFSET     int8_t

// 阴影
PROP_SHADOW_COLOR       color_t
PROP_SHADOW_WIDTH       uint8_t
PROP_SHADOW_OFS_X       int8_t
PROP_SHADOW_OFS_Y       int8_t
PROP_SHADOW_SPREAD      int8_t

// 文字
PROP_TEXT_COLOR         color_t
PROP_TEXT_FONT          Font*
PROP_TEXT_ALIGN         uint8_t (LEFT|CENTER|RIGHT)
PROP_TEXT_LINE_SPACE    int8_t

// 内边距
PROP_PAD_TOP            int16_t
PROP_PAD_BOTTOM         int16_t
PROP_PAD_LEFT           int16_t
PROP_PAD_RIGHT          int16_t
PROP_PAD_ROW            int16_t  (子控件行间距，用于布局)
PROP_PAD_COLUMN         int16_t  (子控件列间距)

// 尺寸
PROP_MIN_WIDTH          int16_t
PROP_MAX_WIDTH          int16_t
PROP_MIN_HEIGHT         int16_t
PROP_MAX_HEIGHT         int16_t

// 变换
PROP_TRANSFORM_SCALE    uint16_t (256 = 1.0x)
PROP_TRANSFORM_ROTATION int16_t  (0.1度单位)

// 过渡
PROP_TRANSITION_TIME    uint16_t (ms)
PROP_TRANSITION_EASING  uint8_t
```

#### 5.7.4 样式存储（稀疏 Key-Value）

每个 Style 对象只存储被显式设置的属性，未设置的属性向上级查找：

```
StyleProp {
    id:     uint8_t     // StyleProperty 枚举值
    value:  union {     // 4 字节值
        color_t  color;
        int32_t  num;
        void*    ptr;
    }
}

Style {
    props:      StyleProp*   // 属性数组
    prop_count: uint8_t      // 已设置属性数
    part:       uint8_t      // 适用部件
    state:      uint8_t      // 适用状态
}
```

典型内存占用：一个 Style 设置 6 个属性 = 6 × 5 + 3 = 33 字节。

#### 5.7.5 样式解析（三级查找）

```
resolve_property(widget, part, state, prop_id) → value:
    1. Local Styles — widget 实例上挂载的样式链（最高优先级）
       遍历 widget->styles[]，匹配 part + state + prop_id
    2. Class Style — WidgetClass 上定义的默认样式
       查找 widget->class->default_styles[]
    3. Theme — 全局主题兜底
       查找 theme->get_default(widget->class, part, state, prop_id)
    4. 若都未找到 → 返回属性的硬编码默认值
```

#### 5.7.6 样式 API

```
// 创建和设置
style_init(style)
style_set_bg_color(style, color)
style_set_radius(style, radius)
style_set_border_width(style, width)
style_set_text_font(style, font)
style_set_pad_all(style, pad)
// ... 每个属性一个 setter

// 挂载到 Widget
widget_add_style(widget, style, part | state)
widget_remove_style(widget, style)
widget_remove_all_styles(widget)

// 局部样式（直接设置到 widget，无需创建 Style 对象）
widget_set_style_bg_color(widget, color, part | state)
widget_set_style_radius(widget, radius, part | state)

// 查询
widget_get_style_bg_color(widget, part) → color  // 自动考虑当前 state
```

#### 5.7.7 状态过渡动画

样式属性可定义过渡参数，状态变化时自动动画插值：

```
// 定义过渡：bg_color 和 opacity 在状态切换时 200ms 缓动过渡
StyleTransition {
    props:    uint8_t[]     // 参与过渡的属性 ID 列表
    count:    uint8_t
    duration: uint16_t      // ms
    delay:    uint16_t      // ms
    easing:   easing_func
}

style_set_transition(style, &transition)

// 效果：按钮从 NORMAL → PRESSED 时，bg_color 平滑过渡而非瞬变
```

实现原理：状态变化时，对参与过渡的属性记录 {旧值, 新值, 开始时间}，每帧插值计算当前值。

#### 5.7.8 Theme（全局主题）

```
Theme {
    // 颜色调色板
    color_primary:      color_t
    color_secondary:    color_t
    color_background:   color_t
    color_surface:      color_t
    color_on_primary:   color_t   // primary 上的文字色
    color_on_surface:   color_t
    color_border:       color_t
    color_disabled:     color_t

    // 字体
    font_small:         Font*
    font_normal:        Font*
    font_large:         Font*

    // 全局参数
    radius_default:     uint8_t
    pad_default:        uint8_t
    transition_default: uint16_t  // 默认过渡时长

    // 为每种 WidgetClass 提供默认样式
    apply:  func(theme, widget)   // widget 创建时自动调用
}

gui_set_theme(theme)              // 切换主题，所有 Widget 重新应用样式
```

#### 5.7.9 样式系统与 on_paint 的关系

Widget 的 on_paint 可以选择两种模式：

**模式 A：样式驱动绘制（推荐，大多数 Widget）**
```c
static void button_paint(Widget* w, GraphicContext* gc, Rect* r) {
    // 框架提供的辅助函数，根据样式自动绘制背景/边框/阴影
    widget_draw_background(w, gc, PART_MAIN);
    widget_draw_border(w, gc, PART_MAIN);
    widget_draw_shadow(w, gc, PART_MAIN);
    // 只需手动绘制特殊内容（如文字）
    widget_draw_label(w, gc, btn->text);
}
```

**模式 B：完全手动绘制（特殊 Widget）**
```c
static void gauge_paint(Widget* w, GraphicContext* gc, Rect* r) {
    // 完全自定义，直接调用 GraphicContext API
    gc_setFillColor(gc, ...);
    gc_fillArc(gc, ...);
    // 不使用样式系统
}
```

两种模式可混合：用样式画背景，手动画内容。


---


### 5.8 焦点与导航系统

#### 5.8.1 概述

焦点系统管理哪个 Widget 当前接收键盘/编码器输入，并提供方向导航能力。支持触摸屏（点击获焦）和非触摸输入（编码器旋转/按键 Tab 切换焦点）两种模式。

#### 5.8.2 焦点组（Focus Group）

将可聚焦的 Widget 组织为焦点组，编码器/按键在组内导航：

```
FocusGroup {
    widgets:    Widget*[]        // 组内可聚焦控件（有序）
    focused:    Widget*          // 当前焦点控件
    wrap:       bool             // 到末尾是否循环回首部
    editing:    bool             // 编辑模式（编码器旋转改值而非切换焦点）
}

focus_group_create() → FocusGroup*
focus_group_add(group, widget)
focus_group_remove(group, widget)
focus_group_set_active(group)        // 设为当前活跃组
focus_group_get_focused(group) → Widget*
```

#### 5.8.3 焦点导航

```
// 导航方向
NAV_NEXT        — 下一个（编码器顺时针 / Tab）
NAV_PREV        — 上一个（编码器逆时针 / Shift+Tab）
NAV_UP / NAV_DOWN / NAV_LEFT / NAV_RIGHT  — 方向键导航

// 导航 API
focus_navigate(group, direction)      // 切换焦点到指定方向的下一个控件
focus_set(widget)                     // 直接设置焦点
focus_clear(group)                    // 清除焦点

// 导航策略
导航顺序由 widgets[] 数组顺序决定（即添加顺序）。
方向导航（UP/DOWN/LEFT/RIGHT）根据控件几何位置自动查找最近邻。
```

#### 5.8.4 编码器交互模型

编码器有两种模式，通过按下编码器按钮切换：

```
导航模式（默认）：
  旋转 → focus_navigate(NEXT/PREV)，切换焦点
  按下 → 进入编辑模式（或触发 EVENT_CLICKED）

编辑模式：
  旋转 → 发送 EVENT_KEY(KEY_UP/KEY_DOWN) 给焦点控件（如 Slider 改值）
  按下 → 退出编辑模式，回到导航模式

focus_group.editing 标记当前模式。
```

#### 5.8.5 焦点事件

```
EVENT_FOCUSED       — Widget 获得焦点
EVENT_DEFOCUSED     — Widget 失去焦点
EVENT_KEY           — 按键/编码器输入（焦点控件接收）
```

#### 5.8.6 焦点指示样式

焦点控件通过样式系统自动显示焦点指示（无需控件自己处理）：

```
// 样式中 STATE_FOCUSED 状态自动生效
style_set_outline_color(style, theme->color_primary)   // 焦点环颜色
style_set_outline_width(style, 2)                      // 焦点环宽度
style_set_outline_offset(style, 2)                     // 焦点环偏移

// Widget 获焦时自动切换到 STATE_FOCUSED，样式系统自动绘制 outline
// 失焦时自动恢复，支持 transition 过渡动画
```

#### 5.8.7 焦点与触摸的协作

```
触摸按下时：
  1. 命中的 Widget 若 FLAG_FOCUSABLE → 自动获焦
  2. 之前的焦点控件自动失焦
  3. 焦点组的 editing 模式自动退出

// 触摸和编码器可同时存在，互不冲突
// 触摸操作后编码器从当前触摸焦点继续导航
```

## 6. 内置控件

Core GUI 层提供一组开箱即用的基础控件，每个控件都是一个 WidgetClass 定义。

### 6.1 控件总览

| 分类 | 控件 | 说明 |
|------|------|------|
| **基础** | BaseWidget | 所有控件的基类，可直接用作容器 |
| **显示** | Label | 文本标签（单行/多行/省略号） |
| | Image | 图片显示（PNG/JPEG/SVG/GIF 动画） |
| | Icon | SVG 图标（支持主题色替换） |
| | Arc | 弧形/圆环（仪表盘、进度环） |
| | Line | 线段/折线 |
| | Canvas | 自定义绘制画布 |
| **输入** | Button | 按钮（文字/图标/文字+图标） |
| | Switch | 开关 |
| | Checkbox | 复选框 |
| | Radio | 单选按钮 |
| | Slider | 滑块（水平/垂直/弧形） |
| | Spinbox | 数字步进器 |
| | TextArea | 多行文本输入 |
| | LineEdit | 单行文本输入 |
| **容器** | Panel | 带标题的面板 |
| | ScrollView | 可滚动容器（惯性滚动） |
| | ListView | 虚拟化列表（大数据量） |
| | GridView | 网格视图 |
| | TabView | 标签页容器 |
| | TileView | 平铺页面（左右/上下滑动切换） |
| **反馈** | ProgressBar | 进度条（线性/圆形） |
| | Spinner | 加载动画 |
| | MessageBox | 消息对话框 |
| | Toast | 轻提示 |
| **导航** | TopBar | 顶部导航栏 |
| | BottomNav | 底部导航栏 |
| | PopupMenu | 弹出菜单 |
| | Dropdown | 下拉选择 |
| **高级** | Chart | 图表（折线/柱状/饼图） |
| | Gauge | 仪表盘 |
| | Roller | 滚轮选择器（日期/时间） |
| | Keyboard | 虚拟键盘 |
| | Calendar | 日历 |

### 6.2 核心控件设计

#### 6.2.1 BaseWidget（基础容器）

所有控件的基类，可直接作为容器使用：
```
WidgetClass base_widget_class:
    - 样式驱动绘制背景/边框/阴影/圆角
    - 支持 Flex/Grid 布局子控件
    - 支持滚动（FLAG_SCROLLABLE）
    - Parts: MAIN, SCROLLBAR
```

#### 6.2.2 Label

```
Label : BaseWidget {
    text:           char* (UTF-8)
    max_lines:      uint8_t (0=无限)
    overflow:       CLIP | ELLIPSIS | SCROLL | WRAP
    text_align:     LEFT | CENTER | RIGHT

    Parts: MAIN
    特殊能力: 文本滚动动画（长文本自动滚动）
}
```

#### 6.2.3 Button

```
Button : BaseWidget {
    text:           char*
    icon:           Image* (可选)
    icon_position:  LEFT | RIGHT | TOP | BOTTOM

    Parts: MAIN
    States: NORMAL → PRESSED → NORMAL (自动过渡动画)
    事件: EVENT_CLICKED, EVENT_LONG_PRESSED
}
```

#### 6.2.4 Slider

```
Slider : BaseWidget {
    value:          int32_t
    min, max:       int32_t
    step:           int32_t
    orientation:    HORIZONTAL | VERTICAL | ARC

    Parts: MAIN(轨道), INDICATOR(已填充), KNOB(滑块)
    事件: EVENT_VALUE_CHANGED
    动画: KNOB 跟随触摸，弹簧物理回弹
}
```

#### 6.2.5 Arc

```
Arc : BaseWidget {
    value:          int32_t
    min, max:       int32_t
    start_angle:    int16_t
    end_angle:      int16_t
    width:          uint8_t (弧线宽度)
    rounded:        bool    (端点圆角)

    Parts: MAIN(背景弧), INDICATOR(前景弧), KNOB(可选)
    绘制: gc_drawArc() 矢量绘制，GPU 路径加速
}
```

#### 6.2.6 ScrollView

```
ScrollView : BaseWidget {
    scroll_x, scroll_y:  int16_t
    content_w, content_h: int16_t
    snap:               NONE | START | CENTER | END
    elastic:            bool (边缘弹性效果)

    Parts: MAIN, SCROLLBAR
    手势: 触摸拖拽 → 惯性滚动（速度衰减） → 边缘回弹（弹簧动画）
}
```

#### 6.2.7 ListView

```
ListView : ScrollView {
    adapter:        ListAdapter*
    item_height:    uint16_t (固定行高) 或 DYNAMIC
    separator:      bool

    虚拟化: 只创建可视区域 + 缓冲区的 item Widget
    复用: 滚出屏幕的 item 回收，滚入时重新绑定数据
    事件: EVENT_ITEM_CLICKED(index)
}

ListAdapter {
    get_count() → int
    create_item(index) → Widget*
    bind_item(widget, index)
    get_item_height(index) → int (DYNAMIC 模式)
}
```

#### 6.2.8 Chart

```
Chart : BaseWidget {
    type:           LINE | BAR | PIE | AREA
    series[]:       { data[], count, color }
    x_axis, y_axis: { min, max, divisions, labels }
    animated:       bool (数据变化时动画过渡)

    Parts: MAIN(背景), INDICATOR(数据线/柱)
    绘制: Path 绘制折线/面积，gc_fillPath() GPU 加速
}
```

#### 6.2.9 Gauge

```
Gauge : BaseWidget {
    value:          int32_t
    min, max:       int32_t
    start_angle:    int16_t
    sweep_angle:    int16_t
    needle_color:   color_t
    scale_lines:    uint8_t

    Parts: MAIN(表盘), INDICATOR(刻度), KNOB(指针)
    动画: 指针旋转用弹簧动画，自然过冲效果
}
```

#### 6.2.10 TileView

```
TileView : BaseWidget {
    tiles[][]:      Widget* (二维网格)
    current:        { row, col }
    direction:      HORIZONTAL | VERTICAL | BOTH

    手势: 滑动切换页面，带过渡动画
    事件: EVENT_TILE_CHANGED(row, col)
}
```

### 6.3 控件开发规范

自定义控件只需：
1. 定义 WidgetClass（继承 base_widget_class 或其他控件 class）
2. 实现 on_paint（调用 GraphicContext API 或使用样式辅助函数）
3. 实现 on_event（处理触摸/按键交互）
4. 定义 Parts 和对应的默认样式

无需修改框架代码，无需注册——创建时传入 WidgetClass 指针即可使用。


---

## 7. 内存管理

### 7.1 内存模型

目标平台 640KB~1MB RAM，采用分区管理：

| 分区 | 大小（典型 1MB） | 用途 |
|------|-----------------|------|
| 系统 + RTOS | ~128KB | 内核、任务栈、驱动 |
| 帧缓冲 | ~450KB | 双缓冲 480×480×RGB565，或单缓冲 ARGB8888 |
| GUI 堆 | ~300KB | Widget 实例、动画、样式、字形缓存 |
| 图层缓存 | ~100KB | 离屏渲染缓冲、过渡动画临时缓冲 |

### 7.2 GUI 堆管理

采用 TLSF（Two-Level Segregated Fit）算法：
- O(1) 分配/释放，无碎片化
- 适合实时系统，分配时间可预测
- 支持内存使用统计和泄漏检测（调试模式）

### 7.3 图层缓存策略

```
Layer Cache:
  - 静态内容（不常变化的 Widget）缓存为位图
  - 过渡动画时分配临时缓冲（旧页面截图）
  - LRU 淘汰策略，内存紧张时自动释放缓存
  - GPU 可直接操作缓存位图（Blit/旋转/混合）
```

---

## 8. 动画引擎

### 8.1 动画模型

支持三种动画类型：

**属性动画（Property Animation）：**
```
Animation {
    target:     Widget*
    property:   ANIM_X | ANIM_Y | ANIM_W | ANIM_H | ANIM_OPACITY
              | ANIM_SCALE | ANIM_ROTATION | ANIM_COLOR | ANIM_CUSTOM
    from, to:   int32_t
    duration:   uint16_t (ms)
    delay:      uint16_t (ms)
    easing:     easing_func
    on_complete:callback
    repeat:     ONCE | LOOP | PING_PONG
}
```

**弹簧动画（Spring Animation）：**
```
SpringAnim {
    target:     Widget*
    property:   uint8_t
    target_value: int32_t
    stiffness:  uint16_t    // 刚度 (100-1000)
    damping:    uint16_t    // 阻尼比 (10-100, 100=临界阻尼)
    mass:       uint8_t     // 质量
    velocity:   int32_t     // 当前速度（自动跟踪）
}
```

弹簧动画无需指定 duration，物理模拟自动收敛到目标值，效果比缓动函数更自然。

**动画状态机（Animation State Machine）：**
```
AnimStateMachine {
    states[]:       AnimState
    transitions[]:  AnimTransition
    current_state:  uint8_t
    layers:         uint8_t (最多 4 层并行)
}

AnimState {
    id:         uint8_t
    animation:  Animation*       // 该状态播放的动画（可为空=静止）
    on_enter:   callback
    on_exit:    callback
}

AnimTransition {
    from, to:   uint8_t
    condition:  condition_func   // 转换条件
    duration:   uint16_t        // 过渡混合时长
    easing:     easing_func
}
```

### 8.2 缓动函数库

```
ease_linear
ease_in_quad / ease_out_quad / ease_in_out_quad
ease_in_cubic / ease_out_cubic / ease_in_out_cubic
ease_in_back / ease_out_back          // 回弹效果
ease_in_elastic / ease_out_elastic    // 弹性效果
ease_in_bounce / ease_out_bounce      // 弹跳效果
ease_bezier(x1, y1, x2, y2)          // 自定义贝塞尔曲线
```

### 8.3 页面过渡

```
Transition {
    type:   SLIDE_LEFT | SLIDE_RIGHT | SLIDE_UP | SLIDE_DOWN
          | FADE | SCALE_IN | SCALE_OUT | NONE
    duration: uint16_t
    easing:   easing_func
}

screen_push(new_screen, transition)
screen_pop(transition)
screen_replace(new_screen, transition)
```

过渡实现：截取当前屏幕为位图缓存，新屏幕渲染到后台缓冲，GPU 做混合/滑动/缩放动画。

### 8.4 动画调度

```
anim_tick(delta_ms):    — 每帧调用（目标 60fps = 16.6ms 间隔）
    for each active animation:
        update elapsed / spring physics
        calculate current value
        apply to target widget property
        if completed: fire callback, remove
    for each state machine:
        evaluate transition conditions
        if transition triggered: blend between states
```

并发动画数：无硬性限制（动态分配），典型场景 8-16 个同时活跃。

---

## 9. 平台移植

### 9.1 移植步骤

移植到新平台需实现 HAL 层的 6 个接口模块：

```
步骤 1: hal_display — 显示驱动（flush/get_info/vsync）
步骤 2: hal_gpu     — GPU 加速（fill/blit/blend/path，无 GPU 则提供空实现）
步骤 3: hal_input   — 输入读取（触摸/按键/编码器）
步骤 4: hal_tick    — 毫秒级时钟源
步骤 5: hal_mem     — 内存分配（对齐分配支持）
步骤 6: hal_os      — RTOS 互斥锁、线程、事件
```

### 9.2 典型平台适配

| 平台 | Display | GPU | 帧缓冲 |
|------|---------|-----|--------|
| Cortex-M55 + 2.5D GPU | RGB/DSI 接口 | NemaGFX/Dave2D | 全帧双缓冲 |
| STM32U5 + ChromART | LTDC | DMA2D | 全帧双缓冲 |
| ESP32-S3 + SPI LCD | SPI DMA | PPA | 部分缓冲 |
| Linux/SDL2 | SDL_Surface | 软件 | 全帧缓冲（PC 模拟器） |

### 9.3 编译配置

```
GUI_CFG_COLOR_DEPTH       16          // 色深: 16(RGB565) / 32(ARGB8888)
GUI_CFG_SCREEN_WIDTH      480
GUI_CFG_SCREEN_HEIGHT     480
GUI_CFG_DOUBLE_BUFFER     1           // 双缓冲
GUI_CFG_GPU_ACCEL         1           // GPU 加速
GUI_CFG_ANTIALIAS         1           // 抗锯齿
GUI_CFG_FONT_ENGINE       VECTOR      // 字体: BITMAP / VECTOR
GUI_CFG_IMAGE_SUPPORT     1           // 图像解码
GUI_CFG_ANIM_SPRING       1           // 弹簧动画
GUI_CFG_ANIM_STATE_MACHINE 1          // 动画状态机
GUI_CFG_DSL_LAYER         1           // 启用 DSL 层
GUI_CFG_TRANSITION        1           // 页面过渡
GUI_CFG_SHADOW            1           // 阴影效果
GUI_CFG_BLUR              0           // 模糊（较耗资源）
```

---

---

# 附录

## A. 与 TouchWeb GUI 的对比

| 特性 | TouchWeb GUI | 本框架 |
|------|-------------|--------|
| 目标平台 | 嵌入式 Linux / Windows | MCU / RTOS / 裸机 |
| 渲染引擎 | 软件矢量引擎 | 自研轻量 2D 渲染器 |
| 内存模型 | C++ new/delete | 静态池 / 小堆 / 系统堆可选 |
| 事件系统 | std::deque + mutex | 静态环形缓冲 + 中断锁 |
| Widget 树 | std::list | 侵入式链表 |
| 布局 | 纯绝对定位 | 绝对定位 + 可选自动布局 |
| 多线程 | 渲染线程分离 | 单线程为主，可选 RTOS 多任务 |
| 字体 | 系统字体 (ps_font) | 内置点阵 / 可选矢量 |
| 动画 | 无内置 | 内置缓动动画引擎 |
| 主题 | 硬编码颜色 | 全局 Theme 配置 |
| 虚拟化列表 | 无 | ListView 虚拟化渲染 |

---

## B. 后续扩展方向

1. **实现语言选择** — C (最大可移植性) vs C子集+宏 vs Rust (安全性)
2. **具体 API 命名规范** — 前缀风格、命名约定
3. **字体方案细化** — 点阵字体格式定义、中文字库方案
4. **触摸手势识别** — 长按、双击、滑动方向判定算法
5. **屏幕旋转支持** — 横竖屏切换的坐标变换
6. **多语言/国际化** — 字符串资源管理方案
7. **开发工具链** — UI 设计器、模拟器、资源转换工具
8. **性能基准测试** — 定义 benchmark 场景和目标帧率

---

## C. 业界图形库对比分析

### C.1 主要竞品概览

| 特性 | LVGL (v9.x) | TouchGFX | emWin (SEGGER) | 本框架设计 |
|------|-------------|----------|----------------|-----------|
| 开源协议 | MIT | 免费(仅限STM32) | 商业授权 | MIT |
| 语言 | C (C99) | C++ | C | 待定 |
| 最小 RAM | 16KB | 约 20KB | 约 20KB | 目标 8-16KB |
| 最小 Flash | 64KB | 约 100KB | 约 80KB | 目标 32-64KB |
| 平台限制 | 无 | 仅 STM32 | 需授权 | 无 |
| Widget 数量 | 30+ | 20+ | 20+ | 15+(核心集) |
| 布局系统 | Flex + Grid | 绝对定位 | 绝对定位 | 绝对 + 简单Flex |
| 样式系统 | CSS-like 属性 | 皮肤/位图 | 皮肤 | 轻量 Theme |
| 动画 | 内置 Timeline | 内置 | 有限 | 内置缓动 |
| 渲染模式 | 全帧/部分/Tiled | 全帧/部分 | 全帧/Band | 全帧/部分/行 |
| GPU 加速 | DMA2D/VGLite/NemaGFX | ChromART(DMA2D) | 有限 | 预留接口 |
| 字体 | 位图+TinyTTF | 位图 | 位图+TTF | 位图+可选矢量 |
| 设计工具 | SquareLine Studio | TouchGFX Designer | GUIBuilder | 暂无(规划中) |
| 多语言 | UTF-8 完整支持 | UTF-8 | UTF-8 | UTF-8 |
| 社区生态 | 非常活跃 | ST 官方维护 | 商业支持 | 新项目 |

### C.2 LVGL 的优势与我们可借鉴之处

**LVGL 的核心优势：**

1. **Flex/Grid 布局引擎** — 借鉴 CSS Flexbox 和 Grid，自动计算子控件位置，大幅减少手动坐标计算
2. **CSS-like 样式系统** — 支持状态（pressed/focused/disabled）+ 部件（main/indicator/knob）的组合样式，样式可继承和覆盖
3. **Draw Pipeline 架构** — 渲染管线可插拔，支持软件渲染、DMA2D、VGLite、NemaGFX 等多种后端
4. **Tiled Rendering** — v9 新增，将脏区域分割为多个 tile 并行渲染，充分利用多核
5. **丰富的 Widget** — 30+ 内置组件，覆盖绝大多数嵌入式 UI 场景
6. **Observer 模式** — 数据绑定机制，UI 自动响应数据变化
7. **XML 声明式 UI** — v9.3 新增，支持用 XML 描述界面结构

**我们应借鉴的：**

- 引入简化版 Flex 布局（不需要完整 CSS Grid，但垂直/水平自动排列很实用）
- Draw Pipeline 可插拔设计（预留 GPU 加速接口）
- 样式的状态机制（normal/pressed/focused/disabled 自动切换）
- Observer/数据绑定思想（减少手动 Update 调用）

### C.3 TouchGFX 的优势与我们可借鉴之处

**TouchGFX 的核心优势：**

1. **极致的硬件优化** — 针对 STM32 的 ChromART (DMA2D) 深度优化，帧率极高
2. **可视化设计器** — TouchGFX Designer 所见即所得，拖拽生成代码
3. **Partial Framebuffer** — 智能部分缓冲策略，最小化 RAM 使用
4. **MVC 架构** — 清晰的 Model-View-Presenter 分离，生成代码与用户代码隔离
5. **位图缓存** — 智能管理位图资源在 RAM/Flash 间的加载

**我们应借鉴的：**

- Partial Framebuffer 的智能分块策略（不是简单的行缓冲，而是按脏区域动态分配）
- MVP 架构思想（将 UI 展示与业务逻辑分离）
- 硬件加速的深度集成模式（不只是预留接口，而是设计时就考虑 DMA 并行）

### C.4 emWin 的优势与我们可借鉴之处

**emWin 的核心优势：**

1. **Window Manager** — 完整的窗口管理器，支持窗口叠加、裁剪、重绘管理
2. **Anti-aliasing** — 高质量抗锯齿渲染
3. **Memory Device** — 离屏绘制设备，避免闪烁
4. **VNC 支持** — 远程 UI 调试

**我们应借鉴的：**

- Memory Device 概念（我们的 ScrollView 离屏缓冲可以泛化为通用机制）
- 窗口管理器的裁剪优化算法

### C.5 我们设计的差异化优势

| 优势点 | 说明 |
|--------|------|
| 更极致的内存控制 | 静态池模式，零动态分配，比 LVGL 的 TLSF 堆更可预测 |
| 更小的最小集 | 目标 32KB Flash，比 LVGL 的 64KB 更适合超低端 MCU |
| 行缓冲渲染 | 支持无全帧缓冲运行，LVGL 最少需要 10 行缓冲 |
| 侵入式链表 | 零额外内存分配的 Widget 树，比 LVGL 的动态数组更省 |
| 纯矢量 UI | 无位图资源依赖，Flash 占用极小 |
| 裸机友好 | 无 OS 依赖，无 malloc 依赖，中断安全 |

### C.6 我们设计的劣势

| 劣势点 | 对比对象 | 改进方向 |
|--------|---------|---------|
| 无可视化设计工具 | TouchGFX Designer, SquareLine | 规划 PC 模拟器 + 简单布局工具 |
| 组件数量少 | LVGL 30+ | 先做精核心集，后续扩展 |
| 无 CSS-like 样式 | LVGL 强大的样式系统 | 引入轻量样式状态机 |
| 无数据绑定 | LVGL Observer | 引入简单的属性观察机制 |
| 无 GPU 加速实现 | TouchGFX ChromART | 设计时预留，后续实现 |
| 社区生态为零 | LVGL 活跃社区 | 需要时间积累 |
| 无抗锯齿 | LVGL/emWin | 可选模块，按需启用 |


### C.7 基于对比分析的设计优化建议

根据以上对比，对我们的框架设计提出以下优化方向：

#### 优化 1：引入轻量样式状态机

借鉴 LVGL 的样式系统，但大幅简化：

```
StyleState: NORMAL | PRESSED | FOCUSED | DISABLED

WidgetStyle {
    bg_color[4]:      color_t     // 每个状态一个背景色
    text_color[4]:    color_t
    border_color[4]:  color_t
    border_width:     uint8_t
    radius:           uint8_t
    padding:          uint8_t
    opacity:          uint8_t
}
```

- 不做 CSS 属性继承（太复杂），只做状态自动切换
- Widget 按下/聚焦时自动查表切换颜色，无需手动重绘逻辑
- 内存代价：每个带样式的 Widget 增加约 40 字节

#### 优化 2：引入简单数据绑定

借鉴 LVGL Observer，实现最小化的属性观察：

```
Subject {
    value:      int32_t (或 void*)
    observers:  Observer[MAX_OBSERVERS]  // 静态数组，默认 4 个
}

subject_set(subject, value)       — 设置值并通知所有观察者
subject_bind(subject, widget, property)  — 绑定到 Widget 属性
```

场景示例：温度传感器值变化 → 自动更新 Label 文本 + ProgressBar 进度，无需手动调用 Update。

#### 优化 3：Draw Unit 可插拔架构

借鉴 LVGL Draw Pipeline，将渲染器设计为可替换单元：

```
DrawUnit {
    name:           char*
    can_draw(task) → bool         // 判断是否能处理该绘制任务
    draw(task)                    // 执行绘制
    wait()                        // 等待异步绘制完成（DMA）
}

// 注册多个 DrawUnit，按优先级尝试
draw_register_unit(unit, priority)
```

默认提供软件渲染 DrawUnit，硬件加速（DMA2D/PPA）作为高优先级 Unit 注册，能处理的任务优先走硬件。

#### 优化 4：智能 Partial Buffer

借鉴 TouchGFX 的部分缓冲策略，不是固定 N 行，而是按脏区域动态分配：

```
刷新流程:
  1. 收集所有脏矩形
  2. 合并重叠区域
  3. 按缓冲区大小分割为多个 band
  4. 逐 band 渲染 + flush（可与 DMA 传输并行）
```

这比固定行缓冲更灵活——小区域更新只需一次 flush，大区域更新自动分块。

#### 优化 5：增加抗锯齿支持（可选）

作为编译选项，提供基本的抗锯齿能力：

```
GUI_CFG_ANTIALIAS  0/1

// 实现方式：2x2 超采样（简单但有效）
// 对圆角、斜线、圆弧启用
// 代价：渲染速度降低约 30-50%，适合高端配置
```

#### 优化 6：PC 模拟器优先

开发初期就提供 SDL2 后端的 PC 模拟器：

```
hal_sdl2/
  hal_display_sdl2.c   — SDL_Surface 作为帧缓冲
  hal_input_sdl2.c     — SDL_Event 转换为触摸/按键
  hal_tick_sdl2.c      — SDL_GetTicks()
```

好处：
- 无需硬件即可开发和调试 UI
- 可集成到 CI 做自动化截图测试
- 降低开发者入门门槛

---

## D. 实现优先级

| 优先级 | 模块 | 理由 |
|--------|------|------|
| P0 | HAL 层 + SDL2 PC 模拟器 | 开发基础设施 |
| P0 | Graphics Layer（SW 渲染器 + Draw Pipeline） | 能看到东西 |
| P0 | Core Layer（Widget 树 + 事件分发 + 主循环） | 框架核心 |
| P0 | 基础 Widget（Label/Button/Image） | 最小可用集 |
| P1 | GPU DrawUnit（2.5D GPU 加速后端） | 性能关键 |
| P1 | 样式系统 + 属性通知机制 | Core 层为 DSL 预留钩子 |
| P1 | 动画引擎（属性动画 + 弹簧 + 缓动库） | 流畅体验 |
| P1 | 布局引擎（Flex） | 减少手动坐标 |
| P1 | 更多 Widget（Edit/Slider/Switch/List/ScrollView） | 完善组件集 |
| P2 | 页面过渡系统 | 绚丽效果 |
| P2 | 动画状态机 | 复杂交互 |
| P2 | 矢量字体 + 字形缓存 | 高质量文字 |
| P2 | DSL 层（响应式绑定 + 声明式描述） | 开发效率 |
| P3 | 组合式组件框架 | DSL 层高级特性 |
| P3 | 阴影/模糊/高级视觉效果 | 锦上添花 |
| P3 | 可视化设计工具 / .ui 文件格式 | 生态建设 |

---

## E. 现代 GUI 系统调研与设计借鉴

### E.1 调研对象

| 系统 | 核心理念 | 关键创新 |
|------|---------|---------|
| SwiftUI | 声明式 + 状态驱动 | View = f(State)，自动 Diff 更新 |
| Flutter/Impeller | Widget 树 + 场景图 + GPU 渲染 | 三棵树架构、AOT 着色器编译 |
| Rive | 可视化状态机 + 矢量动画运行时 | 设计即产品、State Machine 驱动交互 |
| Slint | 声明式 DSL + 嵌入式优先 | .slint 标记语言、编译时优化、MCU 支持 |

### E.2 SwiftUI 的核心思想

**View = f(State)** — UI 是状态的纯函数：

```
// SwiftUI 伪代码
struct TemperatureView: View {
    @State var temp: Int = 25

    var body: some View {
        VStack {
            Text("Temperature: \(temp)°C")
            Slider(value: $temp, in: 0...50)
            if temp > 35 { Text("⚠️ High!").foregroundColor(.red) }
        }
    }
}
```

关键机制：
1. **声明式描述** — 开发者只描述"UI 应该长什么样"，不写命令式更新逻辑
2. **自动 Diff** — 状态变化时，框架对比新旧 View 树，只更新差异部分
3. **值类型 View** — View 是轻量 struct，频繁重建无开销
4. **属性包装器** — @State/@Binding/@Observable 自动追踪依赖关系
5. **隐式动画** — `.animation(.spring)` 一行代码即可为任何状态变化添加动画

**对我们的启示：** 传统命令式 GUI（包括我们当前设计）需要手动调用 `widget_invalidate()` 和 `Update()`，容易遗漏导致 UI 不同步。声明式模型从根本上消除了这类 bug。

### E.3 Flutter 三棵树架构

```
Widget Tree (声明式描述，轻量，频繁重建)
    ↓ diff
Element Tree (持久化，管理生命周期，复用)
    ↓ layout + paint
RenderObject Tree (实际布局计算和绘制)
```

关键机制：
1. **Widget 不可变** — 每次 setState 重建 Widget 子树，但 Element 层做 diff 复用
2. **约束传递布局** — 父传约束(constraints)给子，子返回尺寸(size)给父
3. **Impeller 渲染器** — AOT 编译着色器，消除运行时编译卡顿；tile-based retained-mode 渲染

**对我们的启示：** 分离"描述层"和"渲染层"可以让 UI 更新更高效——不需要每次都重绘整个控件。

### E.4 Rive 状态机动画系统

Rive 的革命性在于：**动画不是代码写的，而是设计师在可视化编辑器中构建的状态机**。

核心概念：
1. **State Machine** — 动画状态 + 转换条件的有向图
2. **Inputs** — 设计与开发的契约（Boolean/Number/Trigger），运行时由代码驱动
3. **Blend States** — 多个动画按权重混合（如角色表情 = 50%开心 + 50%惊讶）
4. **Listeners** — 响应 Pointer Enter/Exit/Click，触发状态转换
5. **Data Binding** — 外部数据变化自动驱动动画状态切换
6. **Layers** — 多个状态机层并行运行（如：身体动画层 + 表情层 + 特效层）
7. **矢量渲染器** — GPU 加速，120fps，无损缩放

**对我们的启示：** 传统嵌入式 GUI 的动画是"属性从 A 到 B 的线性插值"，太原始。Rive 的状态机模型让复杂交互动画变得可管理。

### E.5 Slint 的嵌入式声明式方案

Slint 证明了**声明式 UI 可以在 MCU 上运行**：

```slint
// .slint 标记语言
export component MainWindow inherits Window {
    property <int> counter: 0;

    VerticalLayout {
        Text { text: "Count: " + counter; }
        Button {
            text: "Increment";
            clicked => { counter += 1; }
        }
    }
}
```

关键特性：
- 编译时将 .slint 文件转为优化的 C++/Rust 代码
- 支持裸机 MCU（Cortex-M、ESP32）
- 内置属性绑定和动画
- 最小 RAM ~128KB（含帧缓冲）

---

### E.6 我们当前设计的不足（对比现代系统）

| 问题 | 现代系统的做法 | 我们当前设计 |
|------|-------------|-------------|
| 手动更新 UI | 状态变化自动触发重绘 | 需手动调用 invalidate/Update |
| 动画原始 | 状态机驱动、混合、弹簧物理 | 仅线性/缓动属性插值 |
| UI 描述方式 | 声明式（描述目标状态） | 命令式（描述操作步骤） |
| 交互逻辑 | 状态机可视化编排 | 硬编码 if/else |
| 数据→UI 同步 | 自动绑定/响应式 | 手动同步 |
| 组件复用 | 组合式/嵌套组件 | 继承式 |
| 过渡动画 | 页面切换自动动画 | 无内置过渡 |

---

### E.7 改进方向：融合现代理念到嵌入式约束

#### 改进 A：响应式属性系统（借鉴 SwiftUI/@State + Slint 属性绑定）

核心思想：属性变化自动触发依赖控件重绘，无需手动 Update。

```
// 响应式属性
Property {
    value:       int32_t
    bindings:    Binding[MAX_BINDINGS]   // 静态数组，默认 4
    dirty_flag:  bool
}

// 绑定关系
Binding {
    target_widget:   Widget*
    target_property: uint8_t (枚举: PROP_TEXT / PROP_VALUE / PROP_VISIBLE ...)
    transform:       transform_fn (可选，如 value → "温度: %d°C")
}

// 使用方式
property_set(&temperature, 36);
// → 自动标记绑定的 Label 为 dirty
// → 下一帧自动重绘，无需手动调用
```

内存代价：每个 Property 约 20 字节 + 每个 Binding 12 字节。
适合嵌入式：静态分配，无动态内存，编译时可确定绑定数量。

#### 改进 B：动画状态机（借鉴 Rive State Machine）

将动画从"属性插值"升级为"状态机驱动"：

```
AnimStateMachine {
    states:      AnimState[MAX_STATES]      // 如: Idle, Pressed, Loading, Error
    transitions: AnimTransition[MAX_TRANS]
    current:     uint8_t
    layers:      uint8_t                    // 支持多层并行动画
}

AnimState {
    name:        uint8_t (枚举)
    animation:   Animation*                 // 该状态播放的动画
    on_enter:    callback                   // 进入状态回调
}

AnimTransition {
    from, to:    uint8_t
    condition:   condition_fn               // 转换条件（如: input > threshold）
    duration:    uint16_t                   // 过渡时长
    easing:      uint8_t
}

// 使用示例：按钮状态机
Button states: Normal → Pressed → Normal
                     → Loading → Success → Normal
                              → Error → Normal
```

对比传统方式：
- 传统：`if (pressed) { color = highlight; } else { color = normal; }` — 硬编码
- 状态机：定义状态图，运行时自动管理过渡动画，设计师可调参数

#### 改进 C：声明式 UI 描述层（借鉴 Slint）

提供编译时 UI 描述宏/DSL，生成优化的 Widget 创建代码：

```
// 编译时 UI 描述（C 宏实现或外部 DSL 文件）
UI_DEFINE(main_screen,
    UI_VBOX(padding=8, spacing=4,
        UI_LABEL(id=title, text="Dashboard", font=FONT_LARGE)
        UI_HBOX(spacing=8,
            UI_LABEL(id=temp_label, bind=temperature, format="%.1f°C")
            UI_PROGRESS(id=temp_bar, bind=temperature, max=50)
        )
        UI_BUTTON(id=btn_refresh, text="Refresh", on_click=refresh_handler)
    )
)
```

编译后生成：
- 静态 Widget 实例数组
- 自动布局计算代码
- 属性绑定注册代码

好处：
- 开发者不写 `widget_create` / `widget_add_child` / `widget_set_pos` 样板代码
- 编译时确定内存需求
- 可配合 PC 工具预览

#### 改进 D：过渡动画系统（借鉴 SwiftUI 隐式动画）

页面/状态切换时自动应用过渡效果：

```
Transition {
    type:     TRANSITION_SLIDE_LEFT | TRANSITION_FADE | TRANSITION_SCALE | TRANSITION_NONE
    duration: uint16_t
    easing:   uint8_t
}

// 页面切换
screen_push(new_screen, TRANSITION_SLIDE_LEFT, 300ms)
screen_pop(TRANSITION_SLIDE_RIGHT, 300ms)

// Widget 显示/隐藏过渡
widget_show_animated(widget, TRANSITION_FADE, 200ms)
widget_hide_animated(widget, TRANSITION_SCALE, 150ms)
```

实现方式：过渡期间同时保留新旧内容，用离屏缓冲做混合渲染。

#### 改进 E：动画混合与弹簧物理（借鉴 Rive Blend + SwiftUI Spring）

```
// 弹簧动画（比线性缓动更自然）
SpringAnimation {
    stiffness:   uint16_t    // 刚度
    damping:     uint16_t    // 阻尼
    mass:        uint8_t     // 质量
    velocity:    int16_t     // 初始速度
}

// 动画混合（多个动画按权重叠加）
AnimBlend {
    anims[]:     Animation*
    weights[]:   uint8_t (0-255)    // 各动画权重
}
```

弹簧物理用整数定点数实现，无需浮点。效果远比 ease-in-out 自然。

#### 改进 F：组件组合模式（借鉴 SwiftUI/Flutter 组合优于继承）

当前设计用继承构建组件（Button : Widget），改为支持**组合式组件**：

```
// 组合式：一个 Switch 由多个基础元素组合而成
Switch = Container {
    background: RoundRect(bind=state, color_on=green, color_off=gray)
    thumb:      Circle(bind=state, anim=spring, x_on=right, x_off=left)
}
```

好处：
- 新组件不需要写新类/新 vtable
- 基础图元（Rect/Circle/Text/Image）+ 组合规则 = 无限组件
- 更接近设计师思维

---

### E.8 修订后的架构（融合现代理念）

```
┌─────────────────────────────────────────────────────────┐
│  Application Layer                                       │
│  Screen 定义 / 业务逻辑 / 数据源                          │
├─────────────────────────────────────────────────────────┤
│  Declarative Layer (声明式描述层) [新增]                   │
│  UI DSL / 属性绑定 / 组合式组件定义                       │
├─────────────────────────────────────────────────────────┤
│  Reactive Core (响应式核心) [重构]                        │
│  Property 系统 / 自动 Diff / 脏标记传播                   │
│  动画状态机 / 过渡管理 / 弹簧物理                         │
├─────────────────────────────────────────────────────────┤
│  Widget Layer (组件层)                                    │
│  基础图元 + 组合式高级组件                                │
├─────────────────────────────────────────────────────────┤
│  Render Layer (渲染层)                                    │
│  Draw Pipeline / 脏矩形 / 离屏缓冲 / GPU 加速            │
├─────────────────────────────────────────────────────────┤
│  HAL Layer                                               │
│  Display / Input / Tick / Memory                         │
└─────────────────────────────────────────────────────────┘
```

关键变化：
1. 新增 **Declarative Layer** — UI 描述与渲染分离
2. Core 层升级为 **Reactive Core** — 属性变化自动驱动更新
3. Widget 层从"继承体系"转向"图元 + 组合"
4. 动画从"简单缓动"升级为"状态机 + 混合 + 弹簧"

### E.9 目标平台下的实现策略

Cortex-M55 200MHz + 2.5D GPU + 640KB~1MB RAM 的平台能力远超传统 MCU，可以更激进地采用现代特性：

| 现代特性 | 实现策略 |
|---------|---------|
| 响应式属性绑定 | 直接实现，内存充足可支持大量绑定关系 |
| 声明式 DSL | 编译时展开为 C 代码（零运行时解析开销） |
| 弹簧物理动画 | 利用 M55 浮点单元直接计算，无需定点数 |
| 动画混合/状态机 | 多层并行，GPU 做图层混合 |
| 页面过渡动画 | 双缓冲 + GPU Blit/旋转/Alpha 混合，轻松 60fps |
| 阴影/模糊效果 | GPU 加速高斯模糊（降采样→模糊→升采样） |
| 组合式组件 | 内存充足，嵌套深度不受限 |
| 矢量路径渲染 | 2.5D GPU 硬件路径绘制 |
| 抗锯齿 | MVE/Helium SIMD 加速多采样 |

最终目标：**在 RTOS MCU 上实现媲美智能手表（Apple Watch / Wear OS）级别的 UI 流畅度和视觉效果**——60fps 动画、弹簧物理、页面过渡、阴影渐变、矢量图标，同时保持实时性和低功耗。
