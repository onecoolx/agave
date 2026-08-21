# Agave Web 引擎 — API 数据手册

> 版本 1.0 · 2026 年 6 月  
> 头文件：`include/macross.h`

---

## 数据类型

### `MaCrossView`
```c
typedef struct _mc_view MaCrossView;
```
表示单个 Web View 实例的不透明句柄。所有 View 操作均接受指向此类型的指针。

---

### `MC_STATUS`
```c
typedef enum { MC_STATUS_SUCCESS = 0, MC_STATUS_FAILED = -1 } MC_STATUS;
```
大多数 API 函数的返回状态码。

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
矩形，原点 `(x, y)`，宽 `w`，高 `h`。

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
PIXEL_FORMAT_BGRA32   // 32 位，字节序 B-G-R-A
PIXEL_FORMAT_RGBA32   // 32 位，字节序 R-G-B-A
PIXEL_FORMAT_BGR24    // 24 位，字节序 B-G-R
PIXEL_FORMAT_RGB24    // 24 位，字节序 R-G-B
PIXEL_FORMAT_RGB16    // 16 位，5-6-5 位域
```

---

## 引擎生命周期

### `macross_initialize`
```c
MC_STATUS macross_initialize(MC_PIXEL_FORMAT format, int screen_width, int screen_height);
```
初始化引擎。必须在其他所有 API 调用之前调用一次。

| 参数 | 类型 | 说明 |
|------|------|------|
| `format` | `MC_PIXEL_FORMAT` | 本进程所有 View 缓冲区的像素格式 |
| `screen_width` | `int` | 逻辑屏幕宽度（像素） |
| `screen_height` | `int` | 逻辑屏幕高度（像素） |

**返回值：** 成功返回 `MC_STATUS_SUCCESS`，失败返回 `MC_STATUS_FAILED`。

---

### `macross_shutdown`
```c
void macross_shutdown(void);
```
释放所有引擎资源。销毁所有 View 后、进程退出前调用。

---

### `macross_event_dispatch`
```c
void macross_event_dispatch(void);
```
处理一批待处理的引擎内部事件：定时器、动画帧、网络回调、布局失效。**必须周期性调用**（通常每渲染帧一次），否则引擎将停止响应。

---

## View 管理

### `macross_view_create`
```c
MaCrossView* macross_view_create(MC_BYTE* buffer, int width, int height, int pitch, void* user_data);
```
创建新的 Web View。

| 参数 | 类型 | 说明 |
|------|------|------|
| `buffer` | `MC_BYTE*` | 引擎渲染目标的像素缓冲区。调用 `macross_view_destroy` 之前必须保持有效 |
| `width` | `int` | 缓冲区宽度（像素） |
| `height` | `int` | 缓冲区高度（像素） |
| `pitch` | `int` | 每扫描行字节数（≥ width × bytes_per_pixel） |
| `user_data` | `void*` | 存储在 View 中的任意指针；通过 `macross_view_additional_data` 取回 |

**返回值：** 成功返回新 `MaCrossView*`，失败返回 `NULL`。

---

### `macross_view_destroy`
```c
MC_STATUS macross_view_destroy(MaCrossView* view);
```
销毁 Web View 并释放所有关联资源。**不会**释放像素缓冲区。

---

### `macross_view_additional_data`
```c
void* macross_view_additional_data(MaCrossView* view);
```
取回传给 `macross_view_create` 的 `user_data` 指针。在回调中用于识别是哪个 View 触发的。

---

### `macross_view_resize`
```c
MC_STATUS macross_view_resize(MaCrossView* view, MC_BYTE* buffer, int width, int height, int pitch);
```
替换 View 的像素缓冲区并调整尺寸。此调用后旧缓冲区不再被使用。

---

### `macross_view_set_minimum_layout_size`
```c
MC_STATUS macross_view_set_minimum_layout_size(MaCrossView* view, const MC_SIZE* size);
```
设置最小布局宽度/高度。当 View 缓冲区比可见视口更大时（Tile Buffer 模式）非常有用：页面按此最小尺寸布局，而非缓冲区尺寸。传 `NULL` 清除限制，使用缓冲区尺寸。

---

### `macross_view_update`
```c
MC_STATUS macross_view_update(MaCrossView* view, const MC_RECT* rect);
```
将页面的指定矩形绘制到像素缓冲区。`rect` 为 `NULL` 时重绘整个缓冲区。在 `cb_invalidate_rect` 信号待处理重绘后调用。

---

## 导航

### `macross_view_open_url`
```c
MC_STATUS macross_view_open_url(MaCrossView* view, const char* utf8_url);
```
在 View 中加载指定 URL，记录历史条目。

---

### `macross_view_open_url_quietly`
```c
MC_STATUS macross_view_open_url_quietly(MaCrossView* view, const char* utf8_url);
```
加载 URL，**不**添加到导航历史栈。

---

### `macross_view_load_html`
```c
MC_STATUS macross_view_load_html(MaCrossView* view, const char* html_string, const char* utf8_refurl);
```
直接加载 HTML 字符串。

| 参数 | 说明 |
|------|------|
| `html_string` | UTF-8 编码的 HTML 源码 |
| `utf8_refurl` | 解析相对链接用的参考/基础 URL（如 `"http://localhost/"`） |

---

### `macross_view_forward`
```c
MC_STATUS macross_view_forward(MaCrossView* view);
```
在历史中向前导航。

---

### `macross_view_backward`
```c
MC_STATUS macross_view_backward(MaCrossView* view);
```
在历史中向后导航。

---

### `macross_view_stop`
```c
MC_STATUS macross_view_stop(MaCrossView* view);
```
中止当前加载。

---

### `macross_view_reload`
```c
MC_STATUS macross_view_reload(MaCrossView* view);
```
从网络/缓存重新加载当前页面。

---

### `macross_view_reflow`
```c
MC_STATUS macross_view_reflow(MaCrossView* view);
```
不从网络重新加载，仅重新布局当前页面。配置更改（如字号）后使用。

---

### `macross_view_repaint`
```c
MC_STATUS macross_view_repaint(MaCrossView* view);
```
强制全量重绘，不重新加载或重新布局。

---

## 滚动与位置

### `macross_view_set_position`
```c
MC_STATUS macross_view_set_position(MaCrossView* view, int x, int y);
```
设置渲染页面区域的左上角。引擎将从页面坐标 `(x, y)` 开始渲染到像素缓冲区。用于 Tile Buffer 滚动。

| 参数 | 说明 |
|------|------|
| `x` | 水平页面偏移（像素，≥ 0） |
| `y` | 垂直页面偏移（像素，≥ 0） |

---

### `macross_view_get_position`
```c
MC_STATUS macross_view_get_position(MaCrossView* view, int* x, int* y);
```
获取当前渲染位置。

---

### `macross_view_get_contents_size`
```c
MC_STATUS macross_view_get_contents_size(MaCrossView* view, MC_SIZE* size);
```
返回布局后的完整内容尺寸（总页面宽高）。用于计算最大滚动范围：`maxScrollY = size.h - viewport_h`。

---

## View 设置

### `macross_view_set_scale_factor`
```c
MC_STATUS macross_view_set_scale_factor(MaCrossView* view, int percent);
```
设置缩放级别。`percent=100` 为 100%（默认）。`percent=150` 为 150% 缩放。

---

### `macross_view_get_scale_factor`
```c
MC_STATUS macross_view_get_scale_factor(MaCrossView* view, int* percent);
```
获取当前缩放级别（百分比）。

---

### `macross_view_set_text_multiplier`
```c
MC_BOOL macross_view_set_text_multiplier(MaCrossView* view, int factor);
```
独立于页面缩放调整文字大小。`factor=100` 为默认。

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
覆盖当前页面的字符编码。使用 `CHARSET_AUTO_DETECT` 恢复自动检测。

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

| `mode` 值 | 说明 |
|-----------|------|
| `MC_RENDERING_NORMAL` | 原始页面布局 |
| `MC_RENDERING_SMARTFIT` | 收缩布局以避免水平滚动；`space` 设置布局宽度 |

---

### `macross_view_set_view_source`
```c
MC_STATUS macross_view_set_view_source(MaCrossView* view, MC_BOOL enable);
```
启用"查看源码"模式——页面以原始 HTML 文本显示。

---

## 焦点与输入法

### `macross_view_set_focus`
```c
MC_STATUS macross_view_set_focus(MaCrossView* view);
```
将键盘焦点给予 View。键盘事件被处理前必须先调用此函数。

---

### `macross_view_kill_focus`
```c
MC_STATUS macross_view_kill_focus(MaCrossView* view);
```
移除 View 的键盘焦点。

---

### `macross_view_input_text`
```c
MC_STATUS macross_view_input_text(MaCrossView* view, const char* utf8_text);
```
将 UTF-8 文本插入当前聚焦的可编辑元素（input、textarea、contenteditable）。这是软键盘字符输入的正确 API。若无可编辑元素获得焦点则返回 `MC_STATUS_FAILED`。

> **注意：** 所有可打印字符均应使用此函数。`macross_keyboard_event` 仅用于控制键（退格、回车、方向键等）。

---

## 文本搜索

### `macross_view_search_text`
```c
MC_BOOL macross_view_search_text(MaCrossView* view, const char* utf8_str,
                                  MC_BOOL forward, MC_BOOL caseSensitive, MC_BOOL wrap);
```
查找并滚动到 `utf8_str` 的下一处/上一处出现位置。

| 参数 | 说明 |
|------|------|
| `utf8_str` | 搜索字符串（UTF-8） |
| `forward` | `TRUE`=向前搜索；`FALSE`=向后 |
| `caseSensitive` | `TRUE`=区分大小写 |
| `wrap` | `TRUE`=到达末尾/开头时循环 |

**返回值：** 找到返回 `TRUE`，未找到返回 `FALSE`。

---

### `macross_view_mark_all_matches_text`
```c
int macross_view_mark_all_matches_text(MaCrossView* view, const char* utf8_str,
                                        MC_BOOL caseSensitive, MC_BOOL highlight, unsigned int limit);
```
查找所有出现位置并可选高亮。`limit=0` 表示无限制。

**返回值：** 匹配数量，无匹配返回 `-1`。

---

### `macross_view_unmark_all_matches`
```c
MC_BOOL macross_view_unmark_all_matches(MaCrossView* view);
```
清除所有搜索高亮。

---

## JavaScript

### `macross_view_eval_script`
```c
MC_BOOL macross_view_eval_script(MaCrossView* view, const char* utf8_script);
```
在 View 的全局上下文中执行 JavaScript 字符串，运行于页面的 JS 环境中。

**返回值：** 成功返回 `TRUE`，JS 禁用或执行失败返回 `FALSE`。

---

## 事件

### `macross_mouse_event`
```c
MC_STATUS macross_mouse_event(MaCrossView* view, const MC_MOUSE_EVENT* event);
```
传递鼠标或触控事件。

**`MC_MOUSE_EVENT` 字段：**

| 字段 | 类型 | 说明 |
|------|------|------|
| `type` | `MC_MOUSE_EVENT_TYPE` | `EVT_MOUSE_DOWN`、`EVT_MOUSE_UP`、`EVT_MOUSE_MOVE`、`EVT_MOUSE_WHEEL` |
| `button` | `MC_MOUSE_BUTTON` | `MOUSE_BTN_NONE/LEFT/RIGHT/MIDDLE`、`MOUSE_WHEEL_UP/DOWN` |
| `point` | `MC_POINT` | 页面坐标位置（使用 Tile Buffer 时需叠加偏移量） |
| `modifier` | `unsigned int` | `MC_MODIFIER` 位域（`MF_ALT`、`MF_CTRL`、`MF_SHIFT`、`MF_META`） |

---

### `macross_keyboard_event`
```c
MC_STATUS macross_keyboard_event(MaCrossView* view, const MC_KEY_EVENT* event);
```
传递键盘事件，用于控制键。可打印字符请改用 `macross_view_input_text`。

**`MC_KEY_EVENT` 字段：**

| 字段 | 类型 | 说明 |
|------|------|------|
| `type` | `MC_KEY_EVENT_TYPE` | `EVT_KEY_DOWN` 或 `EVT_KEY_UP` |
| `key` | `MC_VIRTUAL_KEY` | 虚拟键码（参见键码表） |
| `modifier` | `unsigned int` | `MC_MODIFIER` 位域 |

---

### `macross_context_menu_event`
```c
MC_STATUS macross_context_menu_event(MaCrossView* view, const MC_CONTEXT_EVENT* event);
```
在指定页面坐标触发上下文菜单。

---

## 回调 — `macross_set_callback`

```c
MC_STATUS macross_set_callback(const MC_CALLBACK_INFO* info);
```
注册全局回调表。先用 `memset(&cb, 0, sizeof(cb))` 清零，再填入需要的回调。回调是全局的，所有 View 共享。

---

### 回调参考

#### `cb_invalidate_rect`
```c
void (*cb_invalidate_rect)(MaCrossView* view, const MC_RECT* rect);
```
引擎更新了页面某个区域，请调度重绘。**不要在此回调中直接调用 `macross_view_update`**——设置标志，从主循环执行重绘。

| 参数 | 说明 |
|------|------|
| `view` | 发生变化的 View |
| `rect` | 需要重绘的区域 |

---

#### `cb_update_view_now`
```c
void (*cb_update_view_now)(MaCrossView* view);
```
引擎请求立即重绘。调用 `macross_view_update(view, NULL)` 并将内容输出到屏幕。

---

#### `cb_position_changed`
```c
void (*cb_position_changed)(MaCrossView* view, int ox, int oy, int nx, int ny);
```
内容滚动位置发生变化。`(ox, oy)` 为旧位置，`(nx, ny)` 为新位置。

---

#### `cb_set_title`
```c
void (*cb_set_title)(MaCrossView* view, const char* utf8_title);
```
页面标题变化，`utf8_title` 为 UTF-8 编码。

---

#### `cb_set_location`
```c
void (*cb_set_location)(MaCrossView* view, const char* utf8_location);
```
当前 URL 变化（导航或重定向）。

---

#### `cb_set_status`
```c
void (*cb_set_status)(MaCrossView* view, const char* utf8_text);
```
状态栏文字变化（如悬停链接 URL）。

---

#### `cb_start_layout`
```c
void (*cb_start_layout)(MaCrossView* view);
```
主框架完成首次布局——页面已可视化显示。每次导航触发一次，早于所有子资源加载完成。适合隐藏加载指示器。

---

#### `cb_loading_progress`
```c
void (*cb_loading_progress)(MaCrossView* view, unsigned int progress, MC_BOOL finish);
```
加载进度更新。

| 参数 | 说明 |
|------|------|
| `progress` | 0–100 的百分比估算 |
| `finish` | 所有主框架资源加载完成时为 `TRUE` |

---

#### `cb_history_status`
```c
void (*cb_history_status)(MaCrossView* view, unsigned int back_count, unsigned int forward_count);
```
导航历史变化，更新后退/前进按钮状态。

---

#### `cb_set_focus` / `cb_kill_focus`
```c
void (*cb_set_focus)(MaCrossView* view);
void (*cb_kill_focus)(MaCrossView* view);
```
View 获得或失去键盘焦点。

---

#### `cb_change_focus`
```c
void (*cb_change_focus)(MaCrossView* view, const MC_RECT* rect);
```
聚焦元素变化。`rect` 为新焦点元素在页面坐标中的边界框。

---

#### `cb_set_ime_enable`
```c
void (*cb_set_ime_enable)(MC_BOOL enable);
```
页面可编辑元素（`<input>`、`<textarea>`、contenteditable）获得（`enable=TRUE`）或失去（`enable=FALSE`）焦点。相应地显示或隐藏软键盘。使用 `macross_view_input_text` 传递键入的字符。

---

#### `cb_alert_box` / `cb_confirm_box` / `cb_prompt_box`
```c
void   (*cb_alert_box)   (MaCrossView* view, const char* utf8_message);
MC_BOOL(*cb_confirm_box) (MaCrossView* view, const char* utf8_text);
char*  (*cb_prompt_box)  (MaCrossView* view, const char* utf8_message, const char* utf8_default);
```
JavaScript `alert()`、`confirm()`、`prompt()` 对话框。`cb_confirm_box` 点击 OK 返回 `TRUE`。`cb_prompt_box` 返回用户输入字符串（UTF-8，调用方不负责释放），取消返回 `NULL`。

---

#### `cb_context_menu`
```c
void (*cb_context_menu)(MaCrossView* view, const MC_CONTEXT_MENU_DATA* menu);
```
右键或长按上下文菜单。`menu->type` 是 `MC_CONTEXT_MENU_TYPE` 标志的位域。

---

#### `cb_open_window` / `cb_close_window`
```c
MaCrossView* (*cb_open_window)(const char* utf8_url, unsigned int flags, const MC_RECT* rect);
void         (*cb_close_window)(MaCrossView* view);
```
JavaScript `window.open()` / `window.close()`。`flags` 是 `MC_WINDOW_FLAGS` 位域。从 `cb_open_window` 返回新 `MaCrossView*`，或返回 `NULL` 阻止弹窗。

---

#### `cb_create_popup_menu` / `cb_destroy_popup_menu`
```c
MC_POPUP_MENU* (*cb_create_popup_menu)(const MC_RECT* rect);
void           (*cb_destroy_popup_menu)(MC_POPUP_MENU* menu);
```
创建/销毁 `<select>` 下拉列表控件。提供宿主平台的弹出菜单实现。

---

#### `cb_set_cursor`
```c
void (*cb_set_cursor)(MC_CURSOR cursor);
```
鼠标光标需要改变。`cursor` 为 `MC_CURSOR` 枚举值之一。

---

#### `cb_error`
```c
void (*cb_error)(int err, const char* utf8_url);
```
发生网络或加载错误。`err` 为 `MC_ERROR_CODE` 枚举值。

---

#### `cb_url_is_visited`
```c
MC_BOOL (*cb_url_is_visited)(const char* utf8_url);
```
引擎查询某个 URL 是否应显示为"已访问"（`:visited` CSS 伪类）。URL 在历史记录中则返回 `TRUE`。

---

#### `cb_allow_main_request`
```c
MC_BOOL (*cb_allow_main_request)(const char* utf8_url);
```
每次主框架导航前调用。返回 `FALSE` 阻止该请求。可用于内容过滤或家长控制。

---

#### `cb_provide_auth`
```c
MC_BOOL (*cb_provide_auth)(const char* utf8_title, char* username, unsigned int usize,
                            char* password, unsigned int psize);
```
需要 HTTP Basic/Digest 认证。填写 `username` 和 `password` 缓冲区。返回 `TRUE` 继续，`FALSE` 取消。

---

#### `cb_user_agent_custom`
```c
char* (*cb_user_agent_custom)(const char* utf8_url);
```
为 `utf8_url` 返回自定义 User-Agent 字符串，或返回 `NULL` 使用默认值。

---

#### `cb_write_file_data`
```c
MC_BOOL (*cb_write_file_data)(int id, const char* utf8_url, const void* data,
                               unsigned long data_len, unsigned long total_len);
```
不支持的 MIME 类型的下载处理器。随数据到达被反复调用。`total_len=0` 表示总大小未知。返回 `FALSE` 中止下载。

---

#### `cb_clipboard_set_data` / `cb_clipboard_data_len` / `cb_clipboard_get_data`
```c
void         (*cb_clipboard_set_data)(void* data, unsigned int data_len);
unsigned int (*cb_clipboard_data_len)(void);
unsigned int (*cb_clipboard_get_data)(void* data_buffer, unsigned int buffer_len);
```
剪贴板集成（复制/粘贴）。`set_data` 写入剪贴板；`data_len` 返回当前剪贴板数据大小；`get_data` 从中读取。

---

#### `cb_choose_file`
```c
MC_BOOL (*cb_choose_file)(char* utf8_file_name, unsigned int buffer_size, MC_BOOL is_save);
```
为 `<input type="file">` 呈现文件选择对话框。将选中路径写入 `utf8_file_name`。用户确认返回 `TRUE`。

---

#### `cb_choose_date`
```c
MC_BOOL (*cb_choose_date)(char* value, unsigned int buffer_size, const char* initial_value);
```
为 `<input type="date">` 呈现日期选择器。以 `YYYY-MM-DD` 格式写入选中日期。

---

#### `cb_choose_color`
```c
MC_BOOL (*cb_choose_color)(char* value, unsigned int buffer_size, const char* initial_value);
```
为 `<input type="color">` 呈现颜色选择器。以 `#rrggbb` 格式写入选中颜色。

---

## 偏好设置

### `macross_get_config` / `macross_set_config`
```c
MC_BOOL macross_get_config(MC_CONFIG* config);
MC_BOOL macross_set_config(const MC_CONFIG* config);
```
读取或写入全局引擎配置。

**`MC_CONFIG` 字段：**

| 字段 | 默认值 | 说明 |
|------|-------|------|
| `language[64]` | `"en"` | `Accept-Language` 请求头中的默认语言 |
| `std_font[64]` | `"Arial"` | 标准比例字体名称 |
| `serif_font[64]` | `"Times New Roman"` | 衬线字体名称 |
| `fixed_font[64]` | `"Courier New"` | 等宽字体名称 |
| `sans_serif_font[64]` | `"Arial"` | 无衬线字体名称 |
| `fontsize` | `14` | CSS 像素默认字号 |
| `fixed_fontsize` | `14` | 等宽字体默认字号（CSS 像素） |
| `scrollbar_width` | `16` | 内置滚动条宽度（像素）；`0` 禁用滚动条 |
| `default_encoding` | `CHARSET_ISO8859_1` | 默认字符编码 |
| `autodetect_encoding` | `TRUE` | 从 BOM / meta charset 自动检测字符编码 |
| `autoload_images` | `TRUE` | 自动加载图片 |
| `enable_javascript` | `TRUE` | 启用 JavaScript 执行 |
| `enable_cookies` | `TRUE` | 启用 Cookie 存储 |
| `enable_plugin` | `TRUE` | 启用插件 |
| `block_popup` | `FALSE` | 阻止 `window.open()` 调用 |
| `show_at_once` | `FALSE` | 等完整布局完成后再显示页面 |
| `text_antialias` | `TRUE` | 启用字体抗锯齿 |

---

## 网络

### `macross_set_proxy`
```c
MC_BOOL macross_set_proxy(const MC_PROXY* proxy);
```
配置网络代理。传 `NULL` 禁用代理。

**`MC_PROXY` 字段：**

| 字段 | 说明 |
|------|------|
| `Type` | `PROXY_HTTP`、`PROXY_SOCKS4`、`PROXY_SOCKS5` |
| `Host` | 代理服务器主机名或 IP 地址 |
| `Port` | 代理服务器端口（字符串，如 `"8080"`） |
| `UserName` | 代理认证用户名（或 `NULL`） |
| `PassWord` | 代理认证密码（或 `NULL`） |

---

### `macross_set_certificate_dir`
```c
MC_BOOL macross_set_certificate_dir(const char* utf8_path);
```
HTTPS 对端验证用的 CA 证书目录。

---

### `macross_set_disk_cache_size`
```c
unsigned long macross_set_disk_cache_size(unsigned long size);
```
设置最大磁盘缓存大小（字节，默认 16 MB）。**返回**旧缓存大小。

---

### `macross_clear_disk_cache`
```c
MC_STATUS macross_clear_disk_cache(void);
```
从磁盘删除所有缓存资源。

---

### `macross_clear_cookies`
```c
MC_STATUS macross_clear_cookies(void);
```
删除所有 Cookie。

---

### `macross_shrink_cache_memory`
```c
MC_STATUS macross_shrink_cache_memory(void);
```
释放未使用的内存缓存条目（已解码图片、脚本等）。内存紧张时调用。

---

### `macross_get_cache_memory_size`
```c
unsigned long macross_get_cache_memory_size(void);
```
**返回**当前内存缓存大小（字节）。

---

### `macross_network_request`
```c
MC_RESPONSE macross_network_request(MC_REQUEST_TYPE type, const char* utf8_url,
                                     cb_network_callback cb, unsigned int timeout);
```
在页面加载之外执行独立的 HTTP 请求。

| 参数 | 说明 |
|------|------|
| `type` | `REQUEST_GET` 或 `REQUEST_POST` |
| `utf8_url` | 带可选查询字符串的完整 URL |
| `cb` | 接收响应数据块的回调；`NULL` 表示发后不管 |
| `timeout` | 超时时间（秒，5–300） |

**返回值：** `RESPONSE_OK`（200）、`RESPONSE_ERROR`（999）或 `RESPONSE_INVALID_URL`（0）。

回调签名：
```c
MC_BOOL cb(unsigned int code, unsigned char* data, unsigned int len, MC_BOOL finish);
// 返回 TRUE 继续接收数据，FALSE 中止。
```

---

## 弹出菜单

用于 `<select>` 下拉列表。宿主在 `cb_create_popup_menu` 中创建 `MC_POPUP_MENU`，通过以下调用进行更新。

### `macross_menu_update`
```c
MC_STATUS macross_menu_update(MC_POPUP_MENU* menu, const MC_RECT* rect);
```
重绘弹出菜单（`rect` 非 `NULL` 时仅重绘子矩形）。

### `macross_menu_resize`
```c
MC_STATUS macross_menu_resize(MC_POPUP_MENU* menu, int width, int height);
```
调整弹出菜单像素缓冲区大小。

### `macross_menu_hide`
```c
MC_STATUS macross_menu_hide(MC_POPUP_MENU* menu);
```
隐藏弹出菜单。

### `macross_menu_mouse_event` / `macross_menu_keyboard_event`
```c
MC_STATUS macross_menu_mouse_event(MC_POPUP_MENU* menu, const MC_MOUSE_EVENT* event);
MC_STATUS macross_menu_keyboard_event(MC_POPUP_MENU* menu, const MC_KEY_EVENT* event);
```
弹出菜单可见时，将输入事件转发给它。

---

## 自定义 URL Scheme

### `macross_register_scheme`
```c
MC_BOOL macross_register_scheme(const char* scheme, cb_scheme_callback cb, void* data);
```
注册自定义 URL 协议的处理器。引擎导航到此 scheme 的 URL 时，`cb` 被调用并传入 URL 和用户数据。

```c
typedef MC_BOOL (*cb_scheme_callback)(const char* utf8_url, void* data);
```

### `macross_unregister_scheme`
```c
MC_BOOL macross_unregister_scheme(const char* scheme);
```
移除之前注册的 scheme 处理器。

---

## 键码参考

常用 `MC_VIRTUAL_KEY` 值：

| 常量 | 值 | 键位 |
|------|-----|------|
| `KEY_BACK` | 0x08 | 退格 |
| `KEY_TAB` | 0x09 | Tab |
| `KEY_ENTER` | 0x0D | 回车 |
| `KEY_ESCAPE` | 0x1B | Esc |
| `KEY_SPACE` | 0x20 | 空格 |
| `KEY_PAGEUP` | 0x21 | Page Up |
| `KEY_PAGEDOWN` | 0x22 | Page Down |
| `KEY_END` | 0x23 | End |
| `KEY_HOME` | 0x24 | Home |
| `KEY_LEFT` | 0x25 | 左方向键 |
| `KEY_UP` | 0x26 | 上方向键 |
| `KEY_RIGHT` | 0x27 | 右方向键 |
| `KEY_DOWN` | 0x28 | 下方向键 |
| `KEY_INSERT` | 0x2D | Insert |
| `KEY_DELETE` | 0x2E | Delete |
| `KEY_0`…`KEY_9` | 0x30–0x39 | 数字键 |
| `KEY_A`…`KEY_Z` | 0x41–0x5A | 字母键（大写码点） |
| `KEY_F1`…`KEY_F12` | 0x70–0x7B | 功能键 |

软键盘的可打印字符**不要**使用虚拟键码。应改用 `macross_view_input_text` 传入 UTF-8 字符串。
