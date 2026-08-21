# Agave Web Engine — API Datasheet

> Version 1.0 · June 2026  
> Header: `include/macross.h`

---

## Data Types

### `MaCrossView`
```c
typedef struct _mc_view MaCrossView;
```
Opaque handle representing a single web view instance.  All view operations take a pointer to this type.

---

### `MC_STATUS`
```c
typedef enum { MC_STATUS_SUCCESS = 0, MC_STATUS_FAILED = -1 } MC_STATUS;
```
Return status for most API functions.

---

### `MC_BOOL`
```c
typedef int MC_BOOL;  // TRUE=1, FALSE=0
```

---

### `MC_RECT`
```c
typedef struct { int x, y, w, h; } MC_RECT;
```
Rectangle with origin `(x, y)`, width `w`, height `h`.

---

### `MC_SIZE`
```c
typedef struct { int w, h; } MC_SIZE;
```

---

### `MC_POINT`
```c
typedef struct { int x, y; } MC_POINT;
```

---

### `MC_PIXEL_FORMAT`
```c
PIXEL_FORMAT_BGRA32   // 32 bpp, byte order B-G-R-A
PIXEL_FORMAT_RGBA32   // 32 bpp, byte order R-G-B-A
PIXEL_FORMAT_BGR24    // 24 bpp, byte order B-G-R
PIXEL_FORMAT_RGB24    // 24 bpp, byte order R-G-B
PIXEL_FORMAT_RGB16    // 16 bpp, 5-6-5 bit fields
```

---

## Engine Lifecycle

### `macross_initialize`
```c
MC_STATUS macross_initialize(MC_PIXEL_FORMAT format, int screen_width, int screen_height);
```
Initialize the engine.  Must be called once before any other API.

| Parameter | Type | Description |
|-----------|------|-------------|
| `format` | `MC_PIXEL_FORMAT` | Pixel format of all view buffers in this process |
| `screen_width` | `int` | Logical screen width (pixels) |
| `screen_height` | `int` | Logical screen height (pixels) |

**Returns:** `MC_STATUS_SUCCESS` on success, `MC_STATUS_FAILED` on error.

---

### `macross_shutdown`
```c
void macross_shutdown(void);
```
Release all engine resources.  Call after destroying all views and before process exit.

---

### `macross_event_dispatch`
```c
void macross_event_dispatch(void);
```
Process one batch of pending engine events: timers, animation frames, network callbacks, layout invalidations.  **Must be called regularly** (typically every rendering frame) or the engine will appear frozen.

---

## View Management

### `macross_view_create`
```c
MaCrossView* macross_view_create(MC_BYTE* buffer, int width, int height, int pitch, void* user_data);
```
Create a new web view.

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `MC_BYTE*` | Pixel buffer the engine renders into.  Must remain valid until `macross_view_destroy`. |
| `width` | `int` | Buffer width in pixels |
| `height` | `int` | Buffer height in pixels |
| `pitch` | `int` | Bytes per scanline (≥ width × bytes_per_pixel) |
| `user_data` | `void*` | Arbitrary pointer stored in the view; retrieved via `macross_view_additional_data` |

**Returns:** New `MaCrossView*` on success, `NULL` on failure.

---

### `macross_view_destroy`
```c
MC_STATUS macross_view_destroy(MaCrossView* view);
```
Destroy a web view and free all associated resources.  The pixel buffer is **not** freed by this call.

---

### `macross_view_additional_data`
```c
void* macross_view_additional_data(MaCrossView* view);
```
Retrieve the `user_data` pointer passed to `macross_view_create`.  Use this inside callbacks to identify which view fired.

---

### `macross_view_resize`
```c
MC_STATUS macross_view_resize(MaCrossView* view, MC_BYTE* buffer, int width, int height, int pitch);
```
Replace the view's pixel buffer and resize it.  The old buffer is no longer used after this call.

---

### `macross_view_set_minimum_layout_size`
```c
MC_STATUS macross_view_set_minimum_layout_size(MaCrossView* view, const MC_SIZE* size);
```
Set the minimum layout width/height.  Useful when the view buffer is larger than the visible viewport (tile buffer pattern): the page lays out at this minimum size, not the buffer size.  Pass `NULL` to clear and use the buffer dimensions.

---

### `macross_view_update`
```c
MC_STATUS macross_view_update(MaCrossView* view, const MC_RECT* rect);
```
Paint the specified rectangle of the page into the pixel buffer.  If `rect` is `NULL`, the entire buffer is repainted.  Call this after `cb_invalidate_rect` signals a pending repaint.

---

## Navigation

### `macross_view_open_url`
```c
MC_STATUS macross_view_open_url(MaCrossView* view, const char* utf8_url);
```
Load the specified URL in the view.  Records a history entry.

---

### `macross_view_open_url_quietly`
```c
MC_STATUS macross_view_open_url_quietly(MaCrossView* view, const char* utf8_url);
```
Load a URL **without** adding to the navigation history stack.

---

### `macross_view_load_html`
```c
MC_STATUS macross_view_load_html(MaCrossView* view, const char* html_string, const char* utf8_refurl);
```
Load an HTML string directly.

| Parameter | Description |
|-----------|-------------|
| `html_string` | UTF-8 HTML source |
| `utf8_refurl` | Reference/base URL for resolving relative links (e.g. `"http://localhost/"`) |

---

### `macross_view_forward`
```c
MC_STATUS macross_view_forward(MaCrossView* view);
```
Navigate forward in history.

---

### `macross_view_backward`
```c
MC_STATUS macross_view_backward(MaCrossView* view);
```
Navigate backward in history.

---

### `macross_view_stop`
```c
MC_STATUS macross_view_stop(MaCrossView* view);
```
Abort the current load.

---

### `macross_view_reload`
```c
MC_STATUS macross_view_reload(MaCrossView* view);
```
Reload the current page from network/cache.

---

### `macross_view_reflow`
```c
MC_STATUS macross_view_reflow(MaCrossView* view);
```
Re-layout the current page without reloading it from the network.  Useful after a configuration change (e.g. font size).

---

### `macross_view_repaint`
```c
MC_STATUS macross_view_repaint(MaCrossView* view);
```
Force a full repaint without reloading or re-layout.

---

## Scroll & Position

### `macross_view_set_position`
```c
MC_STATUS macross_view_set_position(MaCrossView* view, int x, int y);
```
Set the top-left corner of the rendered page region.  The engine will render starting at page coordinate `(x, y)` into the pixel buffer.  Used for tile-buffer scrolling.

| Parameter | Description |
|-----------|-------------|
| `x` | Horizontal page offset (pixels, ≥ 0) |
| `y` | Vertical page offset (pixels, ≥ 0) |

---

### `macross_view_get_position`
```c
MC_STATUS macross_view_get_position(MaCrossView* view, int* x, int* y);
```
Retrieve the current render position.

---

### `macross_view_get_contents_size`
```c
MC_STATUS macross_view_get_contents_size(MaCrossView* view, MC_SIZE* size);
```
Return the full content dimensions (total page width and height after layout).  Use to compute the maximum scroll range: `maxScrollY = size.h - viewport_h`.

---

## View Settings

### `macross_view_set_scale_factor`
```c
MC_STATUS macross_view_set_scale_factor(MaCrossView* view, int percent);
```
Set the zoom level.  `percent=100` is 100% (default).  `percent=150` is 150% zoom.

---

### `macross_view_get_scale_factor`
```c
MC_STATUS macross_view_get_scale_factor(MaCrossView* view, int* percent);
```
Get the current zoom level in percent.

---

### `macross_view_set_text_multiplier`
```c
MC_BOOL macross_view_set_text_multiplier(MaCrossView* view, int factor);
```
Scale text size independently of page zoom.  `factor=100` is default.

---

### `macross_view_get_text_multiplier`
```c
MC_BOOL macross_view_get_text_multiplier(MaCrossView* view, int* factor);
```

---

### `macross_view_set_encoding`
```c
MC_STATUS macross_view_set_encoding(MaCrossView* view, MC_ENCODING_NAME encoding);
```
Override the character encoding for the current page.  Use `CHARSET_AUTO_DETECT` to restore auto-detection.

---

### `macross_view_get_encoding`
```c
MC_STATUS macross_view_get_encoding(MaCrossView* view, MC_ENCODING_NAME* encoding);
```

---

### `macross_view_set_rendering_mode`
```c
MC_STATUS macross_view_set_rendering_mode(MaCrossView* view, MC_RENDERING_MODE mode, int space);
```

| `mode` | Description |
|--------|-------------|
| `MC_RENDERING_NORMAL` | Original page layout |
| `MC_RENDERING_SMARTFIT` | Shrink layout to avoid horizontal scrolling; `space` sets the layout width |

---

### `macross_view_set_view_source`
```c
MC_STATUS macross_view_set_view_source(MaCrossView* view, MC_BOOL enable);
```
Enable "View Source" mode — the page is displayed as raw HTML text.

---

## Focus & IME

### `macross_view_set_focus`
```c
MC_STATUS macross_view_set_focus(MaCrossView* view);
```
Give keyboard focus to the view.  Required before keyboard events are processed.

---

### `macross_view_kill_focus`
```c
MC_STATUS macross_view_kill_focus(MaCrossView* view);
```
Remove keyboard focus from the view.

---

### `macross_view_input_text`
```c
MC_STATUS macross_view_input_text(MaCrossView* view, const char* utf8_text);
```
Insert UTF-8 text into the currently focused editable element (input, textarea, contenteditable).  This is the correct API for soft keyboard character input.  Returns `MC_STATUS_FAILED` if no editable element has focus.

> **Note:** Use this for all printable characters.  Use `macross_keyboard_event` only for control keys (Backspace, Enter, arrow keys, etc.).

---

## Text Search

### `macross_view_search_text`
```c
MC_BOOL macross_view_search_text(MaCrossView* view, const char* utf8_str,
                                  MC_BOOL forward, MC_BOOL caseSensitive, MC_BOOL wrap);
```
Find and scroll to the next/previous occurrence of `utf8_str`.

| Parameter | Description |
|-----------|-------------|
| `utf8_str` | Search string (UTF-8) |
| `forward` | `TRUE` = search forward; `FALSE` = backward |
| `caseSensitive` | `TRUE` = case-sensitive match |
| `wrap` | `TRUE` = wrap around at end/start |

**Returns:** `TRUE` if found, `FALSE` if not.

---

### `macross_view_mark_all_matches_text`
```c
int macross_view_mark_all_matches_text(MaCrossView* view, const char* utf8_str,
                                        MC_BOOL caseSensitive, MC_BOOL highlight, unsigned int limit);
```
Find all occurrences and optionally highlight them.  `limit=0` means no limit.

**Returns:** Number of matches found, `-1` if none.

---

### `macross_view_unmark_all_matches`
```c
MC_BOOL macross_view_unmark_all_matches(MaCrossView* view);
```
Clear all search highlights.

---

## JavaScript

### `macross_view_eval_script`
```c
MC_BOOL macross_view_eval_script(MaCrossView* view, const char* utf8_script);
```
Execute a JavaScript string in the view's global context.  The script runs in the page's JavaScript environment.

**Returns:** `TRUE` on success, `FALSE` if JS is disabled or execution fails.

---

## Events

### `macross_mouse_event`
```c
MC_STATUS macross_mouse_event(MaCrossView* view, const MC_MOUSE_EVENT* event);
```
Deliver a mouse or touch event.

**`MC_MOUSE_EVENT` fields:**

| Field | Type | Description |
|-------|------|-------------|
| `type` | `MC_MOUSE_EVENT_TYPE` | `EVT_MOUSE_DOWN`, `EVT_MOUSE_UP`, `EVT_MOUSE_MOVE`, `EVT_MOUSE_WHEEL` |
| `button` | `MC_MOUSE_BUTTON` | `MOUSE_BTN_NONE/LEFT/RIGHT/MIDDLE`, `MOUSE_WHEEL_UP/DOWN` |
| `point` | `MC_POINT` | Position in page coordinates (apply tile buffer offset if used) |
| `modifier` | `unsigned int` | Bitfield of `MC_MODIFIER` values (`MF_ALT`, `MF_CTRL`, `MF_SHIFT`, `MF_META`) |

---

### `macross_keyboard_event`
```c
MC_STATUS macross_keyboard_event(MaCrossView* view, const MC_KEY_EVENT* event);
```
Deliver a keyboard event.  Used for control keys.  For printable characters, use `macross_view_input_text` instead.

**`MC_KEY_EVENT` fields:**

| Field | Type | Description |
|-------|------|-------------|
| `type` | `MC_KEY_EVENT_TYPE` | `EVT_KEY_DOWN` or `EVT_KEY_UP` |
| `key` | `MC_VIRTUAL_KEY` | Virtual key code (see Key Codes section) |
| `modifier` | `unsigned int` | Bitfield of `MC_MODIFIER` values |

---

### `macross_context_menu_event`
```c
MC_STATUS macross_context_menu_event(MaCrossView* view, const MC_CONTEXT_EVENT* event);
```
Trigger a context menu at the specified page coordinate.

---

## Callbacks — `macross_set_callback`

```c
MC_STATUS macross_set_callback(const MC_CALLBACK_INFO* info);
```
Register the global callback table.  Zero-initialize the struct (`memset(&cb, 0, sizeof(cb))`) before filling in only the callbacks you need.  Callbacks are global and shared by all views.

---

### Callback Reference

#### `cb_invalidate_rect`
```c
void (*cb_invalidate_rect)(MaCrossView* view, const MC_RECT* rect);
```
The engine has updated a region of the page.  Schedule a repaint.  **Do not call `macross_view_update` directly from this callback** — set a flag and repaint from your main loop.

| Parameter | Description |
|-----------|-------------|
| `view` | The view that changed |
| `rect` | Region that needs repainting |

---

#### `cb_update_view_now`
```c
void (*cb_update_view_now)(MaCrossView* view);
```
The engine requests an immediate repaint.  Call `macross_view_update(view, NULL)` and blit to screen.

---

#### `cb_position_changed`
```c
void (*cb_position_changed)(MaCrossView* view, int ox, int oy, int nx, int ny);
```
The content scroll position changed.  `(ox, oy)` = old position, `(nx, ny)` = new position.

---

#### `cb_set_title`
```c
void (*cb_set_title)(MaCrossView* view, const char* utf8_title);
```
The page title changed.  `utf8_title` is UTF-8 encoded.

---

#### `cb_set_location`
```c
void (*cb_set_location)(MaCrossView* view, const char* utf8_location);
```
The current URL changed (navigation or redirect).

---

#### `cb_set_status`
```c
void (*cb_set_status)(MaCrossView* view, const char* utf8_text);
```
Status bar text changed (e.g. hovered link URL).

---

#### `cb_start_layout`
```c
void (*cb_start_layout)(MaCrossView* view);
```
The main frame has completed its first layout — the page is visually ready.  This fires once per navigation, before all sub-resources finish loading.  Good moment to hide a loading indicator.

---

#### `cb_loading_progress`
```c
void (*cb_loading_progress)(MaCrossView* view, unsigned int progress, MC_BOOL finish);
```
Loading progress update.

| Parameter | Description |
|-----------|-------------|
| `progress` | 0–100 percent estimate |
| `finish` | `TRUE` when all main-frame resources have loaded |

---

#### `cb_history_status`
```c
void (*cb_history_status)(MaCrossView* view, unsigned int back_count, unsigned int forward_count);
```
Navigation history changed.  Update back/forward button states.

---

#### `cb_set_focus` / `cb_kill_focus`
```c
void (*cb_set_focus)(MaCrossView* view);
void (*cb_kill_focus)(MaCrossView* view);
```
The view gained or lost keyboard focus.

---

#### `cb_change_focus`
```c
void (*cb_change_focus)(MaCrossView* view, const MC_RECT* rect);
```
The focused element changed.  `rect` is the bounding box of the new focus in page coordinates.

---

#### `cb_set_ime_enable`
```c
void (*cb_set_ime_enable)(MC_BOOL enable);
```
A page editable element (`<input>`, `<textarea>`, contenteditable) gained (`enable=TRUE`) or lost (`enable=FALSE`) focus.  Show or hide the on-screen keyboard accordingly.  Call `macross_view_input_text` to deliver typed characters.

---

#### `cb_alert_box` / `cb_confirm_box` / `cb_prompt_box`
```c
void   (*cb_alert_box)   (MaCrossView* view, const char* utf8_message);
MC_BOOL(*cb_confirm_box) (MaCrossView* view, const char* utf8_text);
char*  (*cb_prompt_box)  (MaCrossView* view, const char* utf8_message, const char* utf8_default);
```
JavaScript `alert()`, `confirm()`, `prompt()` dialogs.  `cb_confirm_box` returns `TRUE` for OK.  `cb_prompt_box` returns the user input string (UTF-8, caller does not free), or `NULL` for cancel.

---

#### `cb_context_menu`
```c
void (*cb_context_menu)(MaCrossView* view, const MC_CONTEXT_MENU_DATA* menu);
```
Right-click or long-press context menu.  `menu->type` is a bitfield of `MC_CONTEXT_MENU_TYPE` flags.

---

#### `cb_open_window` / `cb_close_window`
```c
MaCrossView* (*cb_open_window)(const char* utf8_url, unsigned int flags, const MC_RECT* rect);
void         (*cb_close_window)(MaCrossView* view);
```
JavaScript `window.open()` / `window.close()`.  `flags` is a bitfield of `MC_WINDOW_FLAGS`.  Return a new `MaCrossView*` from `cb_open_window` or `NULL` to block.

---

#### `cb_create_popup_menu` / `cb_destroy_popup_menu`
```c
MC_POPUP_MENU* (*cb_create_popup_menu)(const MC_RECT* rect);
void           (*cb_destroy_popup_menu)(MC_POPUP_MENU* menu);
```
Create / destroy a `<select>` drop-down list widget.  Provide your platform's popup menu implementation.

---

#### `cb_set_cursor`
```c
void (*cb_set_cursor)(MC_CURSOR cursor);
```
The mouse cursor should change.  `cursor` is one of the `MC_CURSOR` enum values.

---

#### `cb_error`
```c
void (*cb_error)(int err, const char* utf8_url);
```
A network or loading error occurred.  `err` is an `MC_ERROR_CODE` value.

---

#### `cb_url_is_visited`
```c
MC_BOOL (*cb_url_is_visited)(const char* utf8_url);
```
The engine queries whether a URL should be styled as "visited" (`:visited` CSS pseudo-class).  Return `TRUE` if the URL is in your history store.

---

#### `cb_allow_main_request`
```c
MC_BOOL (*cb_allow_main_request)(const char* utf8_url);
```
Called before each main-frame navigation.  Return `FALSE` to block the request.  Use for content filtering or parental controls.

---

#### `cb_provide_auth`
```c
MC_BOOL (*cb_provide_auth)(const char* utf8_title, char* username, unsigned int usize,
                            char* password, unsigned int psize);
```
HTTP Basic/Digest authentication required.  Fill `username` and `password` buffers.  Return `TRUE` to proceed, `FALSE` to cancel.

---

#### `cb_user_agent_custom`
```c
char* (*cb_user_agent_custom)(const char* utf8_url);
```
Return a custom User-Agent string for `utf8_url`, or `NULL` to use the default.

---

#### `cb_write_file_data`
```c
MC_BOOL (*cb_write_file_data)(int id, const char* utf8_url, const void* data,
                               unsigned long data_len, unsigned long total_len);
```
Download handler for unsupported MIME types.  Called repeatedly as data arrives.  `total_len=0` means total size unknown.  Return `FALSE` to abort the download.

---

#### `cb_clipboard_set_data` / `cb_clipboard_data_len` / `cb_clipboard_get_data`
```c
void         (*cb_clipboard_set_data)(void* data, unsigned int data_len);
unsigned int (*cb_clipboard_data_len)(void);
unsigned int (*cb_clipboard_get_data)(void* data_buffer, unsigned int buffer_len);
```
Clipboard integration for copy/paste.  `set_data` writes to the clipboard; `data_len` returns the current clipboard data size; `get_data` reads from it.

---

#### `cb_choose_file`
```c
MC_BOOL (*cb_choose_file)(char* utf8_file_name, unsigned int buffer_size, MC_BOOL is_save);
```
Present a file picker for `<input type="file">`.  Write the selected path to `utf8_file_name`.  Return `TRUE` if user confirmed.

---

#### `cb_choose_date`
```c
MC_BOOL (*cb_choose_date)(char* value, unsigned int buffer_size, const char* initial_value);
```
Present a date picker for `<input type="date">`.  Write the chosen date in `YYYY-MM-DD` format.

---

#### `cb_choose_color`
```c
MC_BOOL (*cb_choose_color)(char* value, unsigned int buffer_size, const char* initial_value);
```
Present a color picker for `<input type="color">`.  Write the chosen color in `#rrggbb` format.

---

## Preferences

### `macross_get_config` / `macross_set_config`
```c
MC_BOOL macross_get_config(MC_CONFIG* config);
MC_BOOL macross_set_config(const MC_CONFIG* config);
```
Read or write the global engine configuration.

**`MC_CONFIG` fields:**

| Field | Default | Description |
|-------|---------|-------------|
| `language[64]` | `"en"` | Default language for `Accept-Language` header |
| `std_font[64]` | `"Arial"` | Standard proportional font name |
| `serif_font[64]` | `"Times New Roman"` | Serif font name |
| `fixed_font[64]` | `"Courier New"` | Monospace font name |
| `sans_serif_font[64]` | `"Arial"` | Sans-serif font name |
| `fontsize` | `14` | Default font size in CSS pixels |
| `fixed_fontsize` | `14` | Default monospace font size in CSS pixels |
| `scrollbar_width` | `16` | Built-in scrollbar width in pixels; `0` disables scrollbars |
| `default_encoding` | `CHARSET_ISO8859_1` | Default character encoding |
| `autodetect_encoding` | `TRUE` | Auto-detect character encoding from BOM / meta charset |
| `autoload_images` | `TRUE` | Automatically load images |
| `enable_javascript` | `TRUE` | Enable JavaScript execution |
| `enable_cookies` | `TRUE` | Enable cookie storage |
| `enable_plugin` | `TRUE` | Enable plugins |
| `block_popup` | `FALSE` | Block `window.open()` calls |
| `show_at_once` | `FALSE` | Show page only after full layout completes |
| `text_antialias` | `TRUE` | Enable font anti-aliasing |

---

## Network

### `macross_set_proxy`
```c
MC_BOOL macross_set_proxy(const MC_PROXY* proxy);
```
Configure a network proxy.  Pass `NULL` to disable the proxy.

**`MC_PROXY` fields:**

| Field | Description |
|-------|-------------|
| `Type` | `PROXY_HTTP`, `PROXY_SOCKS4`, `PROXY_SOCKS5` |
| `Host` | Proxy server hostname or IP address |
| `Port` | Proxy server port (as string, e.g. `"8080"`) |
| `UserName` | Username for proxy authentication (or `NULL`) |
| `PassWord` | Password for proxy authentication (or `NULL`) |

---

### `macross_set_certificate_dir`
```c
MC_BOOL macross_set_certificate_dir(const char* utf8_path);
```
Directory of CA certificates for HTTPS peer verification.

---

### `macross_set_disk_cache_size`
```c
unsigned long macross_set_disk_cache_size(unsigned long size);
```
Set maximum disk cache size in bytes (default 16 MB).  **Returns** the previous cache size.

---

### `macross_clear_disk_cache`
```c
MC_STATUS macross_clear_disk_cache(void);
```
Delete all cached resources from disk.

---

### `macross_clear_cookies`
```c
MC_STATUS macross_clear_cookies(void);
```
Delete all cookies.

---

### `macross_shrink_cache_memory`
```c
MC_STATUS macross_shrink_cache_memory(void);
```
Release unused in-memory resource cache entries (decoded images, scripts, etc.).  Call when memory is tight.

---

### `macross_get_cache_memory_size`
```c
unsigned long macross_get_cache_memory_size(void);
```
**Returns** current in-memory cache size in bytes.

---

### `macross_network_request`
```c
MC_RESPONSE macross_network_request(MC_REQUEST_TYPE type, const char* utf8_url,
                                     cb_network_callback cb, unsigned int timeout);
```
Perform a standalone HTTP request outside of page loading.

| Parameter | Description |
|-----------|-------------|
| `type` | `REQUEST_GET` or `REQUEST_POST` |
| `utf8_url` | Full URL with optional query string |
| `cb` | Callback invoked with response chunks; `NULL` for fire-and-forget |
| `timeout` | Timeout in seconds (5–300) |

**Returns:** `RESPONSE_OK` (200), `RESPONSE_ERROR` (999), or `RESPONSE_INVALID_URL` (0).

The callback signature:
```c
MC_BOOL cb(unsigned int code, unsigned char* data, unsigned int len, MC_BOOL finish);
// Return TRUE to continue receiving data, FALSE to abort.
```

---

## Popup Menu

Used for `<select>` drop-down lists.  The host creates the `MC_POPUP_MENU` in `cb_create_popup_menu` and updates it via these calls.

### `macross_menu_update`
```c
MC_STATUS macross_menu_update(MC_POPUP_MENU* menu, const MC_RECT* rect);
```
Repaint the popup menu (or a sub-rectangle if `rect` is not `NULL`).

---

### `macross_menu_resize`
```c
MC_STATUS macross_menu_resize(MC_POPUP_MENU* menu, int width, int height);
```
Resize the popup menu's pixel buffer.

---

### `macross_menu_hide`
```c
MC_STATUS macross_menu_hide(MC_POPUP_MENU* menu);
```
Hide the popup menu.

---

### `macross_menu_mouse_event` / `macross_menu_keyboard_event`
```c
MC_STATUS macross_menu_mouse_event(MC_POPUP_MENU* menu, const MC_MOUSE_EVENT* event);
MC_STATUS macross_menu_keyboard_event(MC_POPUP_MENU* menu, const MC_KEY_EVENT* event);
```
Forward input events to the popup menu while it is visible.

---

## Custom URL Scheme

### `macross_register_scheme`
```c
MC_BOOL macross_register_scheme(const char* scheme, cb_scheme_callback cb, void* data);
```
Register a handler for a custom URL protocol.  When the engine navigates to a URL with this scheme, `cb` is called with the URL and user data.

```c
typedef MC_BOOL (*cb_scheme_callback)(const char* utf8_url, void* data);
```

### `macross_unregister_scheme`
```c
MC_BOOL macross_unregister_scheme(const char* scheme);
```
Remove a previously registered scheme handler.

---

## Key Code Reference

Selected `MC_VIRTUAL_KEY` values for common keys:

| Constant | Value | Key |
|----------|-------|-----|
| `KEY_BACK` | 0x08 | Backspace |
| `KEY_TAB` | 0x09 | Tab |
| `KEY_ENTER` | 0x0D | Enter / Return |
| `KEY_ESCAPE` | 0x1B | Escape |
| `KEY_SPACE` | 0x20 | Space |
| `KEY_PAGEUP` | 0x21 | Page Up |
| `KEY_PAGEDOWN` | 0x22 | Page Down |
| `KEY_END` | 0x23 | End |
| `KEY_HOME` | 0x24 | Home |
| `KEY_LEFT` | 0x25 | Left Arrow |
| `KEY_UP` | 0x26 | Up Arrow |
| `KEY_RIGHT` | 0x27 | Right Arrow |
| `KEY_DOWN` | 0x28 | Down Arrow |
| `KEY_INSERT` | 0x2D | Insert |
| `KEY_DELETE` | 0x2E | Delete |
| `KEY_0`…`KEY_9` | 0x30–0x39 | Digit keys |
| `KEY_A`…`KEY_Z` | 0x41–0x5A | Letter keys (uppercase code points) |
| `KEY_F1`…`KEY_F12` | 0x70–0x7B | Function keys |

For printable characters from a soft keyboard, do **not** use virtual key codes.  Use `macross_view_input_text` with the UTF-8 character string instead.
