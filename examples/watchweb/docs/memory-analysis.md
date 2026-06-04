# WatchWeb 内存占用分析报告

> 状态：分析存档（实测于桌面 Linux x86_64，2026-06-04）
> 目的：摸清浏览器各部分内存构成，为 MCU 环境优化提供依据
> 方法：/proc/PID/smaps 实测 RSS + 引擎 API 查询 + 代码结构分析

## 1. 实测数据（运行时 RSS）

| 页面 | contents 尺寸 | WebKit 图片 cache | heap | 总 RSS |
|------|--------------|-------------------|------|--------|
| google.com | 575×480 | 556 KB | 32 MB | 120 MB |
| sohu.com | 1217×1763 | 1139 KB | 55 MB | 180 MB |

采集方式：在 `cb_loading_progress(finish)` 回调里调 `macross_get_cache_memory_size()` 取
WebKit 资源缓存；`/proc/PID/smaps` 取 RSS 与 [heap] 段；buffer 大小由编译期常量计算。

## 2. 内存构成细分（google.com）

| 部分 | 占用 | 性质 | MCU 实际 RAM |
|------|------|------|-------------|
| .so 代码段 | 76 MB | 多进程共享，桌面 mmap 整库文件 | Flash 中执行，≈0 RAM |
| 引擎 heap | 32 MB | DOM + Render + CSS + JS + QuickJS | 占 RAM |
| tile buffer | 5.4 MB | 应用层 framebuffer (960×1440×4) | 占 RAM |
| canvas buffer | 0.9 MB | LVGL 中转 (480×480×4) | 占 RAM |
| 字体 | 4.7 MB | mmap 中文字体文件 | 部分 RAM |
| WebKit 图片 cache | 0.56 MB | 含在 heap 内，解码后位图 | — |

**MCU 实际 RAM 占用 ≈ 总 RSS − .so 共享(76MB) ≈ 44 MB**（google 级）；sohu 级 ≈ 100 MB。

> .so 在桌面是 mmap 整个动态库（含未执行代码页），RSS 计入但多进程共享；
> MCU 上代码在 Flash 就地执行（XIP）或按需载入，不应计为应用 RAM 成本。

## 3. heap（32MB）构成分析

heap 是真正的内存大头。基于 google/sohu 对比（sohu contents 面积约 google 的 7.7 倍，
heap 多 23MB）+ 代码结构推断：

| 子项 | 估算 | 依据 |
|------|------|------|
| WebCore DOM / RenderObject | ~12-18 MB | 随 DOM 节点数与渲染树规模线性增长（sohu 主增量来源） |
| QuickJS runtime + JS 堆 | ~5-10 MB | JS 上下文、对象、字符串、字节码 |
| CSS 样式 / 规则 | ~3-5 MB | StyleSheet + 计算样式 |
| 字体字形缓存 | ~2-3 MB | 光栅化字形位图 |
| WebKit 图片 cache | 0.56 MB | 实测 |
| 其他引擎内部 | ~3-5 MB | 网络 buffer、解析临时对象 |

**关键洞察**：图片 cache 仅 0.56-1.1MB，**heap 大头是 DOM/Render/CSS/JS 这些"页面复杂度"成本**，
不是图片缓存。压缩图片缓存对总内存影响有限；真正决定 heap 的是网页本身的结构复杂度。

## 4. 引擎可调参数（已确认的抓手）

| 参数 | 默认值 | 接口 | 位置 |
|------|--------|------|------|
| WebKit 资源 Cache 容量 | MOBILE 4MB / 非 8MB | `Cache::setCapacities(minDead,maxDead,total)` | Cache.cpp `cDefaultCacheCapacity` |
| 主动收缩缓存 | — | `macross_shrink_cache_memory()` | macross.h |
| 查询缓存大小 | — | `macross_get_cache_memory_size()` | macross.h |
| 磁盘 cache 容量 | 16 MB | `macross_set_disk_cache_size(bytes)` | macross.h |
| PageCache（前进后退） | — | `PageCache::setCapacity(n)` | PageCache.cpp |
| BackForwardList | DefaultCapacity | `setCapacity(n)` | BackForwardList.cpp |

## 5. 优化方向（按 ROI 排序）

### 🔴 高收益 — 应用层可控

**A. tile buffer 瘦身：5.4MB → 2.7MB（省 2.7MB）**
- 现 `TILE_BUF_W = CONTENT_WIDTH*2 = 960`，横向 2× 余量
- fit-to-width 模式页面宽=屏幕宽，无横向滚动，横向余量浪费
- 改 `TILE_BUF_W = CONTENT_WIDTH`(480)，保留纵向余量
- ⚠️ 风险：曾在 buffer=480 触发 needsLayout 崩溃；该崩溃已由 invalidate/paint 分离修复，
  需重新验证 buffer=480 在导航/刷新场景下稳定

**B. 自定义 LVGL widget 消除 canvas：省 0.9MB + 一次 memcpy**
- DRAW_MAIN 用 `lv_draw_image` 直接画 tile buffer，去掉 canvas 中转
- 关联：docs/render-thread-design.md §9

### 🟡 中收益 — 引擎配置

**C. WebKit 资源 Cache 上限调小**
- 默认 MOBILE 4MB，实测 google 仅用 556KB
- MCU 上 `setCapacities(0, 1MB, 2MB)` + 适时 `macross_shrink_cache_memory()`

**D. 字体按需加载：省 2-4MB**
- 现整个中文字体 mmap(4.7MB)，改按页面用到的字符动态载入字形

**E. 磁盘 cache 调小**
- `macross_set_disk_cache_size(2MB)`，MCU Flash 有限

### 🟢 架构级 — 超低内存 MCU

**F. 禁用 PageCache**
- `PageCache::setCapacity(0)`，省前进后退的整页状态缓存

**G. 视口外内容延迟构建 / 分页**
- sohu 级长页面为整页建 RenderObject 树；视口外延迟构建可省内存
- WebKit 架构改动大，长期方向

## 6. 推荐的 MCU 配置组合（16MB PSRAM 表盘）

```c
// 应用层
TILE_BUF_W = CONTENT_WIDTH;          // tile 5.4MB → 2.7MB
// 自定义 widget 去 canvas            // 省 0.9MB

// 引擎配置
Cache::setCapacities(0, 1MB, 2MB);   // WebKit 资源 cache 上限 2MB
macross_set_disk_cache_size(2MB);    // 磁盘 cache 2MB
PageCache::setCapacity(0);           // 禁前进后退缓存
// 字体按需加载                        // 省 ~3MB
```

**预期效果**：
- framebuffer 类应用层占用：6.3MB → 2.7MB
- google 级页面 RAM：~44MB → ~36MB

## 7. 重要结论

1. **应用层可优化空间有限**（framebuffer ~6MB），主要靠 tile/canvas 瘦身省 ~3.6MB。
2. **heap 中 DOM/Render/CSS/JS（~25MB）是页面复杂度的硬成本**——应用层与引擎配置都改不掉，
   取决于访问的网页本身。
3. **图片缓存不是大头**（<1.1MB），压缩它收益有限。
4. **MCU 上的根本对策是控制页面复杂度**：面向 LLM 输出的简洁 HTML/CSS，避免重型站点；
   而非试图压缩引擎本身。
5. **复杂页面（sohu 级）heap 仍会到 50MB+**，这类页面不适合低内存 MCU，应在产品层面限制
   或提供"精简模式"。

## 8. 待办关联

- tile buffer 瘦身 / 自定义 widget → TODO.md P4
- 渲染线程 → docs/render-thread-design.md
- 引擎 Cache 配置接口封装 → 需在 WebView 层暴露设置入口（待设计）
