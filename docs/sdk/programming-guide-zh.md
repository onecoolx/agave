# Agave Web 引擎 — 编程指南

> 版本 1.0 · 2026 年 6 月

---

## 1. 简介

本指南介绍如何使用 Agave 公开 C API（`include/macross.h`）构建浏览器或类浏览器应用程序。全程以 `watchweb` 示例（位于 `examples/watchweb/`）作为具体参考——这是一款完全基于 Agave API 构建的圆形表盘浏览器，功能完整。

### 1.1 所需材料

- `include/macross.h` — 唯一需要包含的公开头文件
- `libagave.so` / `libagave.a` — 引擎库文件
- 像素缓冲区（任意 BGRA32/RGBA32/RGB16 内存区域），引擎将渲染到此
- 应用程序的事件循环，用于向引擎分发事件

---

## 2. 引擎生命周期

### 2.1 初始化

在创建任何 View 之前，用屏幕像素格式和尺寸调用一次 `macross_initialize`：

```c
#include "macross.h"

int main(void) {
    // 以 32 位 BGRA 像素格式、480×480 屏幕初始化
    MC_STATUS status = macross_initialize(PIXEL_FORMAT_BGRA32, 480, 480);
    if (status != MC_STATUS_SUCCESS) {
        return 1;  // 处理错误
    }
    // ... 创建 View，运行事件循环 ...
    macross_shutdown();
    return 0;
}
```

`macross_initialize` 负责建立渲染后端、JavaScript 引擎、网络栈和字体系统。每个进程仅调用一次。

### 2.2 关闭

```c
macross_shutdown();
```

销毁所有 View 后、退出前调用。

---

## 3. 创建 WebView

WebView 是核心对象。创建时提供引擎渲染目标的像素缓冲区：

```c
// 分配像素缓冲区：宽 × 高 × 4 字节（BGRA32）
int width = 480, height = 480;
uint8_t* buffer = malloc(width * height * 4);

MaCrossView* view = macross_view_create(
    buffer,       // 像素缓冲区
    width,        // 缓冲区宽度（像素）
    height,       // 缓冲区高度（像素）
    width * 4,    // pitch（每扫描行字节数）
    my_app_data   // 用户数据，可通过 macross_view_additional_data() 取回
);
```

**Tile Buffer 模式（推荐用于可滚动页面）：** 分配比可见区域更大的缓冲区，在缓冲区内滚动无需引擎重绘。`watchweb` 使用 `宽×2` × `高×3` 的 tile buffer：

```c
// Tile buffer：宽度×2，高度×3
int tile_w = view_w * 2;
int tile_h = view_h * 3;
uint8_t* tile_buf = malloc(tile_w * tile_h * 4);

MaCrossView* view = macross_view_create(tile_buf, tile_w, tile_h, tile_w * 4, app);

// 告诉引擎最小布局宽度（实际视口宽度）
MC_SIZE lsize = { view_w, view_h };
macross_view_set_minimum_layout_size(view, &lsize);
```

### 3.1 调整大小

运行时显示区域变化时：

```c
macross_view_resize(view, new_buffer, new_width, new_height, new_pitch);
```

### 3.2 销毁

```c
macross_view_destroy(view);
free(buffer);
```

---

## 4. 注册回调

加载任何 URL 前，用所有需要处理的回调填充一个 `MC_CALLBACK_INFO` 结构，然后注册。不需要的字段保持 `NULL` 即可。

```c
static MC_CALLBACK_INFO g_cb = {};

// 有任意区域需要重绘时调用
static void on_invalidate(MaCrossView* view, const MC_RECT* rect) {
    // 标记需要重绘；不要在此处同步绘制
    mark_dirty(view);
}

// 引擎需要立即刷新时调用
static void on_update_now(MaCrossView* view) {
    do_repaint(view);
}

// 页面标题变化时调用
static void on_title(MaCrossView* view, const char* title) {
    set_window_title(title);
}

// URL 变化（导航）时调用
static void on_url(MaCrossView* view, const char* url) {
    update_address_bar(url);
}

// 加载进度 [0–100] 和完成标志
static void on_progress(MaCrossView* view, unsigned int pct, MC_BOOL finish) {
    update_progress_bar(pct, finish);
}

// 首次布局完成（页面已可视）时调用
static void on_start_layout(MaCrossView* view) {
    hide_loading_indicator();
}

// 页面输入框获得/失去焦点时调用（显示/隐藏输入法）
static void on_ime(MC_BOOL enable) {
    show_soft_keyboard(enable);
}

// 光标需要改变时调用
static void on_cursor(MC_CURSOR cur) {
    set_system_cursor(cur);
}

void setup_callbacks(void) {
    memset(&g_cb, 0, sizeof(g_cb));
    g_cb.cb_invalidate_rect  = on_invalidate;
    g_cb.cb_update_view_now  = on_update_now;
    g_cb.cb_set_title        = on_title;
    g_cb.cb_set_location     = on_url;
    g_cb.cb_loading_progress = on_progress;
    g_cb.cb_start_layout     = on_start_layout;
    g_cb.cb_set_ime_enable   = on_ime;
    g_cb.cb_set_cursor       = on_cursor;
    macross_set_callback(&g_cb);
}
```

> **重要：** 回调是全局的（而非每个 View 独立）。如果有多个 View，在回调内通过 `macross_view_additional_data(view)` 区分是哪个 View 触发的。

---

## 5. 加载内容

### 5.1 加载 URL

```c
macross_view_open_url(view, "https://example.com");
```

### 5.2 加载 HTML 字符串

```c
const char* html = "<html><body><h1>你好，Agave！</h1></body></html>";
macross_view_load_html(view, html, "http://localhost/");
```

### 5.3 导航控制

```c
macross_view_backward(view);  // 后退
macross_view_forward(view);   // 前进
macross_view_reload(view);    // 重载当前页面
macross_view_stop(view);      // 停止加载
```

---

## 6. 事件循环

Agave 需要应用程序周期性地调用 `macross_event_dispatch()` 来处理定时器、网络事件和布局回调。将其集成到主循环中：

```c
// 典型事件循环（watchweb 模式）
while (running) {
    // 1. 处理 Agave 内部待处理事件（定时器、网络等）
    macross_event_dispatch();

    // 2. 如果引擎使某个区域失效，执行重绘
    if (g_dirty) {
        g_dirty = false;
        macross_view_update(view, NULL);  // 绘制整个缓冲区
        blit_to_display(tile_buf, view_off_x, view_off_y, view_w, view_h);
    }

    // 3. 处理平台输入事件并分发给 Agave
    handle_platform_events(view);

    // 4. 睡眠或让出 CPU
    usleep(16000);  // ~60 Hz
}
```

**`macross_view_update(view, rect)`** 触发引擎将指定矩形（`rect` 为 NULL 则为整个缓冲区）渲染到像素缓冲区。

---

## 7. 输入处理

### 7.1 鼠标 / 触控

将平台事件映射为 `MC_MOUSE_EVENT`，调用 `macross_mouse_event`：

```c
MC_MOUSE_EVENT e = {};

// 触控按下
e.type     = EVT_MOUSE_DOWN;
e.button   = MOUSE_BTN_LEFT;
e.point.x  = touch_x;
e.point.y  = touch_y;
e.modifier = 0;
macross_mouse_event(view, &e);

// 触控抬起
e.type = EVT_MOUSE_UP;
macross_mouse_event(view, &e);
```

> **Tile buffer 偏移：** 如果视口在 tile buffer 中的位置为 `(off_x, off_y)`，发送前需叠加偏移：
>
> ```c
> e.point.x = touch_x + off_x;
> e.point.y = touch_y + off_y;
> ```

### 7.2 键盘

```c
MC_KEY_EVENT ke = {};
ke.type     = EVT_KEY_DOWN;
ke.key      = KEY_ENTER;       // MC_VIRTUAL_KEY 枚举
ke.modifier = 0;
macross_keyboard_event(view, &ke);

ke.type = EVT_KEY_UP;
macross_keyboard_event(view, &ke);
```

### 7.3 IME / 软键盘文本输入

软键盘的可打印字符应使用专用文本输入 API（而非 `macross_keyboard_event`）：

```c
// 将 UTF-8 字符串直接插入到已聚焦的输入框
macross_view_input_text(view, "你好世界");
```

这条路径经过 IME 合成处理（`Editor::confirmComposition`），能正确将文本插入当前聚焦的可编辑元素。`macross_keyboard_event` 仅用于控制键（退格、回车、方向键）。

---

## 8. Tile Buffer 滚动

Tile Buffer 模式支持无需引擎重绘的快速滚动，原理：

```
页面内容（高度很大）
┌─────────────────────────────────────────┐
│                                         │
│  ┌──────────────────────────────────┐   │
│  │     tile buffer（w×2, h×3）      │   │
│  │  ┌────────────────────────────┐  │   │
│  │  │    可见视口（w×h）         │  │   │
│  │  │                            │  │   │
│  │  │    off_x, off_y            │  │   │
│  │  └────────────────────────────┘  │   │
│  └──────────────────────────────────┘   │
│   引擎从 (engine_x, engine_y) 开始渲染  │
└─────────────────────────────────────────┘
```

滚动时：

```c
// 更新滚动位置
pos_y += delta_y;
clamp(&pos_y, 0, content_height - view_h);

// 三区算法计算视口在 tile buffer 中的偏移
off_y = start_offset(pos_y, view_h, tile_h, content_height);
engine_y = pos_y - off_y;
if (engine_y < 0) engine_y = 0;

if (视口在缓冲区范围内) {
    // 仅 blit，无需引擎重绘
    blit_to_display(tile_buf + off_y * stride, view_w, view_h);
} else {
    // 移动引擎渲染位置并触发重绘
    macross_view_set_position(view, engine_x, engine_y);
    macross_view_update(view, NULL);
    blit_to_display(tile_buf + off_y * stride, view_w, view_h);
}
```

**三区偏移算法**（`start_offset`）的三个区域：
- 近顶/左：偏移从 0 增长（内容起点可达）
- 中段：视口居中于缓冲区（双向滚动都有余量，无需重绘）
- 近底/右：`endPos - 剩余距离`（页面底部/右边缘可达）

完整实现参见 `examples/watchweb/src/webview.cpp`。

---

## 9. 输出到屏幕

引擎渲染到原始像素缓冲区，如何送到屏幕因平台而异：

**LVGL Canvas（watchweb 方式）：**

```c
// 从 tile buffer 复制到 LVGL canvas（叠加滚动偏移）
uint8_t* dst = lv_canvas_get_buf(canvas);
const uint8_t* src = tile_buf + off_y * tile_stride + off_x * 4;
for (int row = 0; row < view_h; row++) {
    memcpy(dst + row * view_stride, src + row * tile_stride, view_w * 4);
}
lv_obj_invalidate(canvas);
```

**Linux 帧缓冲：**

```c
uint8_t* fb = mmap(NULL, fb_size, PROT_WRITE, MAP_SHARED, fb_fd, 0);
memcpy(fb + off_y * fb_stride, tile_buf, view_w * view_h * 4);
```

**SDL 纹理：**

```c
SDL_UpdateTexture(texture, NULL, tile_buf + off_y * tile_stride, tile_stride);
SDL_RenderCopy(renderer, texture, NULL, NULL);
SDL_RenderPresent(renderer);
```

---

## 10. 配置

在加载任何内容前自定义引擎行为：

```c
MC_CONFIG cfg;
macross_get_config(&cfg);           // 读取当前默认配置

cfg.fontsize          = 16;         // 基础字号（像素）
cfg.enable_javascript = TRUE;
cfg.enable_cookies    = TRUE;
cfg.autoload_images   = TRUE;
cfg.text_antialias    = TRUE;
cfg.scrollbar_width   = 0;          // 0 = 不显示内置滚动条
strncpy(cfg.std_font, "Arial", sizeof(cfg.std_font));

macross_set_config(&cfg);
```

---

## 11. 网络设置

### 11.1 代理

```c
MC_PROXY proxy = {
    .Type     = PROXY_HTTP,
    .Host     = "192.168.1.1",
    .Port     = "8080",
    .UserName = NULL,
    .PassWord = NULL,
};
macross_set_proxy(&proxy);

// 禁用代理
macross_set_proxy(NULL);
```

### 11.2 SSL 证书

```c
macross_set_certificate_dir("/etc/ssl/certs");
```

### 11.3 缓存管理

```c
macross_set_disk_cache_size(32 * 1024 * 1024);  // 32 MB 磁盘缓存
macross_clear_disk_cache();                       // 清除全部缓存资源
macross_clear_cookies();                          // 清除全部 Cookie
```

---

## 12. JavaScript 集成

### 12.1 执行脚本

```c
macross_view_eval_script(view, "document.body.style.background = 'red';");
```

### 12.2 拦截导航请求

```c
static MC_BOOL on_allow_request(const char* url) {
    if (strstr(url, "ads.tracker.com")) return FALSE;  // 拦截追踪器
    return TRUE;
}
g_cb.cb_allow_main_request = on_allow_request;
```

### 12.3 自定义 User-Agent

```c
static char* on_user_agent(const char* url) {
    return "MyBrowser/1.0 (Agave; Linux)";
}
g_cb.cb_user_agent_custom = on_user_agent;
```

---

## 13. 文本搜索

```c
// 查找下一处 "Agave"，向前搜索，不区分大小写，循环
MC_BOOL found = macross_view_search_text(view, "Agave", TRUE, FALSE, TRUE);

// 高亮所有匹配项（最多 100 处）
int count = macross_view_mark_all_matches_text(view, "Agave", FALSE, TRUE, 100);

// 清除所有高亮
macross_view_unmark_all_matches(view);
```

---

## 14. 内存管理

```c
// 收缩引擎的内存资源缓存
macross_shrink_cache_memory();

// 查询当前缓存大小
unsigned long cache_bytes = macross_get_cache_memory_size();
```

---

## 15. watchweb 案例分析

`watchweb`（`examples/watchweb/`）是完全基于 Agave 公开 API 构建的 480×480 圆形表盘浏览器。关键设计决策：

| 功能 | 实现方式 |
|------|---------|
| Tile buffer | `TILE_BUF = view_w×2 × view_h×3`；三区偏移算法 |
| 滚动 | `WebView::scrollBy` 计算偏移；在缓冲区内则 blit，否则 `macross_view_set_position` + 重绘 |
| 进度条 | 在 `cb_start_layout`（首次布局 = 页面已可视）时结束，LVGL 动画平滑爬到 99% 再隐藏 |
| 软键盘 | `cb_set_ime_enable` 回调 → LVGL 键盘；按键通过 `macross_view_input_text` 送入引擎 |
| LVGL 集成 | 从 tile buffer blit 到 Canvas；LVGL 处理触控事件并转发给 `macross_mouse_event` |
| 事件循环 | `macross_event_dispatch` + `lv_timer_handler` 交替执行；volatile bool 标志隔离引擎回调与 UI 线程 |

文件结构：

```
examples/watchweb/src/
  main.cpp     — 入口、事件循环、平台集成
  webview.cpp  — Agave API 封装（tile buffer、滚动、回调）
  webview.h    — WebView 类接口
  ui.cpp       — LVGL UI（Canvas、工具栏、键盘、进度条）
  ui.h         — WatchUI 类接口
  config.h     — 屏幕尺寸、tile buffer 尺寸、缩放常量
```

---

## 16. 自定义 URL Scheme（可选）

为自定义协议注册处理器（需要 `ENABLE_SCHEMEEXTENSION`）：

```c
static MC_BOOL my_scheme_handler(const char* url, void* data) {
    // url 如 "myapp://some/path"
    char* html = generate_content(url);
    macross_view_load_html(view, html, url);
    return TRUE;
}

macross_register_scheme("myapp", my_scheme_handler, NULL);
// ...
macross_unregister_scheme("myapp");
```

---

## 17. 常见陷阱

1. **不要在 `cb_invalidate_rect` 里同步绘制。** 引擎可能正在布局中。设置标志，从主循环重绘。
2. **可打印字符用 `macross_view_input_text`**，而非 `macross_keyboard_event`。后者仅处理控制键（方向键、回车、退格）。
3. **Tile buffer 坐标偏移：** 向引擎发送鼠标事件时，始终叠加 `off_x`/`off_y`。
4. **`macross_event_dispatch` 必须周期性调用**（每帧或至少 20 Hz），否则定时器、动画和 XHR 回调将停滞。
5. **回调是全局的。** 所有 View 共用同一张回调表。用 `macross_view_additional_data` 区分不同 View。
6. **`cb_start_layout` 每次导航只触发一次**，不是每次滚动。用它标记页面已就绪；用 `cb_loading_progress`（`finish=TRUE`）或超时判断所有子资源是否完成。
