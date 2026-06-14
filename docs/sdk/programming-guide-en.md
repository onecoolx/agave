# Agave Web Engine — Programming Guide

> Version 1.0 · June 2026

---

## 1. Introduction

This guide walks you through building a browser or browser-like application using the Agave public C API (`include/macross.h`).  The `watchweb` example (in `examples/watchweb/`) is used as a concrete reference throughout — it is a fully-featured round-screen watch browser built on Agave.

### 1.1 What You Need

- `include/macross.h` — the only public header you include
- `libagave.so` / `libagave.a` — the engine library
- A pixel buffer (any BGRA32/RGBA32/RGB16 memory region) for the engine to draw into
- An event loop in your application that dispatches events to the engine

---

## 2. Engine Lifecycle

### 2.1 Initialize

Before creating any view, call `macross_initialize` once with your display's pixel format and dimensions:

```c
#include "macross.h"

int main(void) {
    // Initialize with 32-bit BGRA pixel format, 480×480 screen
    MC_STATUS status = macross_initialize(PIXEL_FORMAT_BGRA32, 480, 480);
    if (status != MC_STATUS_SUCCESS) {
        // handle error
        return 1;
    }
    // ... create views, run event loop ...
    macross_shutdown();
    return 0;
}
```

`macross_initialize` sets up the rendering backend, JavaScript engine, network stack, and font system.  Call it exactly once per process.

### 2.2 Shutdown

```c
macross_shutdown();
```

Call after destroying all views and before exiting.

---

## 3. Creating a WebView

A WebView is the central object.  You create it with a pixel buffer that the engine will render into:

```c
// Allocate a pixel buffer: width × height × 4 bytes (BGRA32)
int width = 480, height = 480;
uint8_t* buffer = malloc(width * height * 4);

MaCrossView* view = macross_view_create(
    buffer,       // pixel buffer
    width,        // buffer width in pixels
    height,       // buffer height in pixels
    width * 4,    // pitch (bytes per scanline)
    my_app_data   // opaque user data, retrieved later via macross_view_additional_data()
);
```

**Tile buffer pattern** (recommended for scrollable pages): allocate a buffer larger than the visible area, so scrolling within the buffer requires no engine repaint.  `watchweb` uses `width×2` by `height×3`:

```c
// Tile buffer: 2× wide, 3× tall
int tile_w = view_w * 2;
int tile_h = view_h * 3;
uint8_t* tile_buf = malloc(tile_w * tile_h * 4);

MaCrossView* view = macross_view_create(tile_buf, tile_w, tile_h, tile_w * 4, app);

// Tell the engine the minimum layout width (the actual viewport width)
MC_SIZE lsize = { view_w, view_h };
macross_view_set_minimum_layout_size(view, &lsize);
```

### 3.1 Resize

If the display area changes at runtime:

```c
macross_view_resize(view, new_buffer, new_width, new_height, new_pitch);
```

### 3.2 Destroy

```c
macross_view_destroy(view);
free(buffer);
```

---

## 4. Registering Callbacks

Before loading any URL, register a `MC_CALLBACK_INFO` structure with all the callbacks your application handles.  You only need to fill in the ones you care about — all fields are function pointers, default `NULL` is safe.

```c
static MC_CALLBACK_INFO g_cb = {};

// Called when any region of a view needs repainting
static void on_invalidate(MaCrossView* view, const MC_RECT* rect) {
    // Mark a repaint pending; do NOT paint synchronously here
    mark_dirty(view);
}

// Called when the engine wants an immediate flush
static void on_update_now(MaCrossView* view) {
    do_repaint(view);
}

// Called when page title changes
static void on_title(MaCrossView* view, const char* title) {
    set_window_title(title);
}

// Called when URL changes (navigation)
static void on_url(MaCrossView* view, const char* url) {
    update_address_bar(url);
}

// Called with loading progress [0–100] and finish flag
static void on_progress(MaCrossView* view, unsigned int pct, MC_BOOL finish) {
    update_progress_bar(pct, finish);
}

// Called when first layout is done (page is visually ready)
static void on_start_layout(MaCrossView* view) {
    hide_loading_indicator();
}

// Called when a page input field gains/loses focus (show/hide IME)
static void on_ime(MC_BOOL enable) {
    show_soft_keyboard(enable);
}

// Called when the cursor should change
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

> **Important:** Callbacks are global (not per-view).  If you have multiple views, use `macross_view_additional_data(view)` inside each callback to identify which view fired.

---

## 5. Loading Content

### 5.1 Load a URL

```c
macross_view_open_url(view, "https://example.com");
```

### 5.2 Load HTML String

```c
const char* html = "<html><body><h1>Hello, Agave!</h1></body></html>";
macross_view_load_html(view, html, "http://localhost/");
```

### 5.3 Navigation

```c
macross_view_backward(view);  // go back
macross_view_forward(view);   // go forward
macross_view_reload(view);    // reload current page
macross_view_stop(view);      // stop loading
```

---

## 6. The Event Loop

Agave requires a regular call to `macross_event_dispatch()` to process timers, network events, and layout callbacks.  Integrate this into your application's main loop:

```c
// Typical event loop (watchweb pattern)
while (running) {
    // 1. Process pending Agave internal events (timers, network, etc.)
    macross_event_dispatch();

    // 2. If the engine invalidated a region, repaint
    if (g_dirty) {
        g_dirty = false;
        macross_view_update(view, NULL);  // paint the whole buffer
        blit_to_display(tile_buf, view_off_x, view_off_y, view_w, view_h);
    }

    // 3. Process platform input events and dispatch to Agave
    handle_platform_events(view);

    // 4. Sleep or yield (let other tasks run)
    usleep(16000);  // ~60 Hz
}
```

**`macross_view_update(view, rect)`** triggers the engine to render the given rectangle (or the whole buffer if `rect` is NULL) into the pixel buffer.

---

## 7. Handling Input

### 7.1 Mouse / Touch

Map your platform events to `MC_MOUSE_EVENT` and call `macross_mouse_event`:

```c
MC_MOUSE_EVENT e = {};

// Touch press
e.type     = EVT_MOUSE_DOWN;
e.button   = MOUSE_BTN_LEFT;
e.point.x  = touch_x;
e.point.y  = touch_y;
e.modifier = 0;
macross_mouse_event(view, &e);

// Touch release
e.type = EVT_MOUSE_UP;
macross_mouse_event(view, &e);
```

> **Tile buffer offset:** If the view position within the tile buffer is `(off_x, off_y)`, add the offset to coordinates before sending:
>
> ```c
> e.point.x = touch_x + off_x;
> e.point.y = touch_y + off_y;
> ```

### 7.2 Keyboard

```c
MC_KEY_EVENT ke = {};
ke.type     = EVT_KEY_DOWN;
ke.key      = KEY_ENTER;       // MC_VIRTUAL_KEY enum
ke.modifier = 0;
macross_keyboard_event(view, &ke);

ke.type = EVT_KEY_UP;
macross_keyboard_event(view, &ke);
```

### 7.3 IME / Soft Keyboard Text Input

For printable characters from a soft keyboard, use the dedicated text-input API (not `macross_keyboard_event`):

```c
// Insert a UTF-8 string directly into the focused input field
macross_view_input_text(view, "Hello");
```

This goes through the IME composition path (`Editor::confirmComposition`) and correctly inserts the text into whatever editable element has focus.  Use `macross_keyboard_event` only for control keys (Backspace, Enter, arrows).

---

## 8. Scrolling with a Tile Buffer

The tile buffer pattern enables fast scrolling without engine repaint.  The principle:

```
page content (very tall)
┌─────────────────────────────────────────┐
│                                         │
│  ┌──────────────────────────────────┐   │
│  │       tile buffer (w×2, h×3)     │   │
│  │  ┌────────────────────────────┐  │   │
│  │  │  visible viewport (w×h)    │  │   │
│  │  │                            │  │   │
│  │  │    off_x, off_y            │  │   │
│  │  └────────────────────────────┘  │   │
│  └──────────────────────────────────┘   │
│   engine renders from (engine_x, y)     │
└─────────────────────────────────────────┘
```

On scroll:

```c
// Update scroll position
pos_y += delta_y;
clamp(&pos_y, 0, content_height - view_h);

// Compute where the viewport sits in the tile buffer (3-zone algorithm)
off_y = start_offset(pos_y, view_h, tile_h, content_height);
engine_y = pos_y - off_y;
if (engine_y < 0) engine_y = 0;

if (off_y is within buffer) {
    // Just blit — no engine repaint needed
    blit_to_display(tile_buf + off_y * stride, view_w, view_h);
} else {
    // Move the engine render position and trigger repaint
    macross_view_set_position(view, engine_x, engine_y);
    macross_view_update(view, NULL);
    blit_to_display(tile_buf + off_y * stride, view_w, view_h);
}
```

The **3-zone offset algorithm** (`start_offset`) determines the optimal tile buffer offset:
- Near top/left: offset grows from 0 (content origin reachable)
- Middle: viewport centered in buffer (room to scroll either way without repaint)
- Near bottom/right: `endPos - remaining` (page bottom/right edge reachable)

See `examples/watchweb/src/webview.cpp` for the complete implementation.

---

## 9. Displaying to Screen

The engine renders into a raw pixel buffer.  How you get that to screen is platform-dependent:

**LVGL Canvas (watchweb approach):**

```c
// Copy from tile buffer to LVGL canvas, applying scroll offset
uint8_t* dst = lv_canvas_get_buf(canvas);
const uint8_t* src = tile_buf + off_y * tile_stride + off_x * 4;
for (int row = 0; row < view_h; row++) {
    memcpy(dst + row * view_stride, src + row * tile_stride, view_w * 4);
}
lv_obj_invalidate(canvas);
```

**Linux framebuffer:**

```c
uint8_t* fb = mmap(NULL, fb_size, PROT_WRITE, MAP_SHARED, fb_fd, 0);
memcpy(fb + off_y * fb_stride, tile_buf, view_w * view_h * 4);
```

**SDL texture:**

```c
SDL_UpdateTexture(texture, NULL, tile_buf + off_y * tile_stride, tile_stride);
SDL_RenderCopy(renderer, texture, NULL, NULL);
SDL_RenderPresent(renderer);
```

---

## 10. Configuration

Customize engine behavior before loading any content:

```c
MC_CONFIG cfg;
macross_get_config(&cfg);           // read current defaults

cfg.fontsize       = 16;            // base font size (px)
cfg.enable_javascript = TRUE;
cfg.enable_cookies    = TRUE;
cfg.autoload_images   = TRUE;
cfg.text_antialias    = TRUE;
cfg.scrollbar_width   = 0;          // 0 = no built-in scrollbars
strncpy(cfg.std_font, "Arial", sizeof(cfg.std_font));

macross_set_config(&cfg);
```

---

## 11. Network Settings

### 11.1 Proxy

```c
MC_PROXY proxy = {
    .Type     = PROXY_HTTP,
    .Host     = "192.168.1.1",
    .Port     = "8080",
    .UserName = NULL,
    .PassWord = NULL,
};
macross_set_proxy(&proxy);

// Disable proxy
macross_set_proxy(NULL);
```

### 11.2 SSL Certificates

```c
macross_set_certificate_dir("/etc/ssl/certs");
```

### 11.3 Cache

```c
macross_set_disk_cache_size(32 * 1024 * 1024);  // 32 MB disk cache
macross_clear_disk_cache();                       // clear all cached resources
macross_clear_cookies();                          // clear all cookies
```

---

## 12. JavaScript Integration

### 12.1 Execute Script

```c
macross_view_eval_script(view, "document.body.style.background = 'red';");
```

### 12.2 Intercept Navigation

```c
static MC_BOOL on_allow_request(const char* url) {
    if (strstr(url, "ads.tracker.com")) return FALSE;  // block trackers
    return TRUE;
}
g_cb.cb_allow_main_request = on_allow_request;
```

### 12.3 Custom User-Agent

```c
static char* on_user_agent(const char* url) {
    return "MyBrowser/1.0 (Agave; Linux)";
}
g_cb.cb_user_agent_custom = on_user_agent;
```

---

## 13. Text Search

```c
// Find next occurrence of "Agave", forward, case-insensitive, wrap
MC_BOOL found = macross_view_search_text(view, "Agave", TRUE, FALSE, TRUE);

// Highlight all matches (up to 100)
int count = macross_view_mark_all_matches_text(view, "Agave", FALSE, TRUE, 100);

// Clear highlights
macross_view_unmark_all_matches(view);
```

---

## 14. Memory Management

```c
// Shrink the engine's in-memory resource cache
macross_shrink_cache_memory();

// Query current cache size
unsigned long cache_bytes = macross_get_cache_memory_size();
```

---

## 15. watchweb Case Study

`watchweb` (`examples/watchweb/`) is a complete browser for a 480×480 round watch display, built entirely on the Agave public API.  Key design decisions:

| Feature | Implementation |
|---------|---------------|
| Tile buffer | `TILE_BUF = view_w×2 × view_h×3`; 3-zone offset algorithm |
| Scrolling | `WebView::scrollBy` computes offset; blit if in-buffer, else `macross_view_set_position` + repaint |
| Progress bar | End at `cb_start_layout` (first layout = page visually ready), glide animation to 99% then hide |
| Soft keyboard | `cb_set_ime_enable` callback → LVGL keyboard; keys sent via `macross_view_input_text` |
| LVGL integration | Canvas blit from tile buffer; LVGL handles touch events which are forwarded to `macross_mouse_event` |
| Event loop | `macross_event_dispatch` + `lv_timer_handler` interleaved; volatile bool flags separate engine callbacks from UI thread |

File structure:

```
examples/watchweb/src/
  main.cpp     — entry point, event loop, platform integration
  webview.cpp  — Agave API wrapper (tile buffer, scroll, callbacks)
  webview.h    — WebView class interface
  ui.cpp       — LVGL UI (canvas, toolbar, keyboard, progress bar)
  ui.h         — WatchUI class interface
  config.h     — screen size, tile buffer size, zoom constants
```

---

## 16. Custom URL Scheme (Optional)

Register a handler for a custom protocol (requires `ENABLE_SCHEMEEXTENSION`):

```c
static MC_BOOL my_scheme_handler(const char* url, void* data) {
    // url is "myapp://some/path"
    // Load content and inject via macross_view_load_html
    char* html = generate_content(url);
    macross_view_load_html(view, html, url);
    return TRUE;
}

macross_register_scheme("myapp", my_scheme_handler, NULL);
// ...
macross_unregister_scheme("myapp");
```

---

## 17. Common Pitfalls

1. **Do not paint synchronously in `cb_invalidate_rect`.**  The engine may be mid-layout.  Set a flag and repaint from your main loop.
2. **Use `macross_view_input_text` for printable characters**, not `macross_keyboard_event`. The latter only handles control keys (arrows, Enter, Backspace).
3. **Tile buffer coordinates:** Always add `off_x`/`off_y` when converting display coordinates to page coordinates for mouse events.
4. **`macross_event_dispatch` must be called regularly** (every frame or at least 20 Hz) or timers, animations, and XHR callbacks will stall.
5. **Callbacks are global.** All views share the same callback table. Use `macross_view_additional_data` to dispatch per-view.
6. **`cb_start_layout` fires once per navigation**, not once per scroll. Use it to signal that the page is ready, then use `cb_loading_progress` with `finish=TRUE` or your own timeout to know when all subresources are done.
