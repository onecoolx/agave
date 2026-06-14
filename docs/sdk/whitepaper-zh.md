# Agave Web 引擎 — 技术白皮书

> 版本 1.0 · 2026 年 6 月

---

## 1. 概述

Agave 是一款面向嵌入式系统和资源受限平台设计的轻量级、自包含 Web 渲染引擎。它以单一库文件的形式提供完整的浏览器引擎栈——HTML 解析、CSS 布局、JavaScript 执行与纯软件光栅化——并通过简洁的 C API 对外暴露。

Agave 定位为**面向可信/半可信内容的精准渲染引擎**，而非通用互联网浏览器。其纯软件渲染架构意味着无需 GPU 即可运行，非常适合工业 HMI 面板、可穿戴设备、智能家电以及任何运行实时或嵌入式操作系统的中端 SoC。

---

## 2. 架构

```
┌──────────────────────────────────────────────┐
│                  宿主应用程序                  │
│          (watchweb / 自定义浏览器)             │
└──────────────┬───────────────────────────────┘
               │  C API  (include/macross.h)
┌──────────────▼───────────────────────────────┐
│               Agave 引擎核心                   │
│  ┌───────────┐ ┌──────────┐ ┌─────────────┐  │
│  │ HTML/CSS  │ │  布局    │ │  JavaScript │  │
│  │  解析器   │ │  引擎    │ │  (QuickJS)  │  │
│  └─────┬─────┘ └────┬─────┘ └──────┬──────┘  │
│        └────────────┼──────────────┘          │
│             ┌───────▼──────┐                  │
│             │  渲染树      │                  │
│             └───────┬──────┘                  │
│             ┌───────▼──────┐                  │
│             │   Picasso    │  (2D 光栅化器)    │
│             └───────┬──────┘                  │
└─────────────────────┼────────────────────────┘
                       │  BGRA32/RGBA32/RGB16 像素缓冲
               ┌───────▼──────┐
               │  宿主缓冲区  │  (帧缓冲 / LCD / LVGL Canvas)
               └──────────────┘
```

### 2.1 关键设计决策

| 决策 | 设计理由 |
|------|---------|
| 纯软件渲染（Picasso 2D） | 无 GPU 依赖；可运行于任意 SoC |
| Tile Buffer 渲染模型 | 将页面布局尺寸与显示分辨率解耦 |
| QuickJS JavaScript 引擎 | ES2020 级别的 JS 支持，编译体积仅约 230 KB |
| C 公开 API | 与 C、C++ 及任意 FFI 二进制兼容 |
| 静态链接 | 单 `.so` / `.a`，无运行时框架依赖 |

---

## 3. HTML / CSS 支持

### 3.1 HTML 标准

- 完整 HTML 4.01 解析，含错误恢复
- HTML5 语义化元素：`<header>`、`<footer>`、`<article>`、`<section>`、`<nav>`、`<main>`、`<aside>`、`<figure>`、`<figcaption>`、`<time>`、`<mark>`
- HTML5 表单：`<input>` 支持 text、number、email、url、tel、search、password、checkbox、radio、range、date、color、file、hidden、submit、reset、button 等类型；`<textarea>`、`<select>`、`<datalist>`、`<fieldset>`、`<legend>`、`<output>`
- HTML5 约束验证 API（`required`、`pattern`、`min`、`max`、`step`、`minlength`、`maxlength`）
- 带 2D 绘图 API 的 `<canvas>` 元素
- `<script>`（同步与异步）、`<link>`、`<style>`、`<meta>`、`<base>`
- 支持 `object-fit`、`object-position`、`aspect-ratio` 的 `<img>`
- `<iframe>` 嵌套子框架
- 完整支持 colspan/rowspan 的 `<table>`

### 3.2 CSS 布局

| 功能 | 状态 |
|------|------|
| 普通流（块级/行内/inline-block） | ✅ 完整 |
| **现代 CSS Flexbox**（display: flex/inline-flex） | ✅ 完整 |
| CSS Grid（display: grid/inline-grid） | ✅ 已实现 |
| CSS 表格布局 | ✅ 完整 |
| Float 与 clear | ✅ 完整 |
| 绝对/固定/相对/sticky 定位 | ✅ 完整 |
| 多列布局 | ✅ |
| `box-sizing: border-box` | ✅ |

**Flexbox 详情：** `flex-direction`、`flex-wrap`、`flex-flow`、`flex-grow`、`flex-shrink`、`flex-basis`、`flex` 简写、`justify-content`、`align-items`、`align-content`、`align-self`、`order`、auto margin。支持全部标准值，包括 `space-between`、`space-around`、`wrap-reverse`、`column-reverse`。

### 3.3 CSS 视觉效果

| 功能 | 状态 |
|------|------|
| 盒模型（margin、border、padding、width、height） | ✅ 完整 |
| `border-radius`（独立四角） | ✅ |
| `box-shadow`（多重、inset） | ✅ |
| `background`（颜色、图片、线性渐变、径向渐变、锥形渐变） | ✅ |
| background-size/position/repeat/origin/clip | ✅ |
| `color`、`opacity` | ✅ |
| `transform`（translate、scale、rotate、skew、matrix、3D 子集） | ✅ |
| `filter`（blur、drop-shadow、opacity、brightness、contrast、saturate、grayscale、hue-rotate、sepia、invert） | ✅ |
| `mix-blend-mode` | ✅ |
| `clip-path`（circle、ellipse、inset、polygon） | ✅ |
| `mask-image`（基于线性渐变的遮罩） | ✅ |
| `object-fit` / `object-position` | ✅ |
| `aspect-ratio` | ✅ |
| `visibility`、`display`、`overflow` | ✅ |
| `transition`（属性、时长、延迟、时间函数） | ✅ |
| `animation` / `@keyframes` | ✅ |

### 3.4 CSS 选择器

- 全部 CSS 2.1 选择器（类型、类、ID、后代、子级、相邻、属性）
- CSS 3 伪类：`:hover`、`:focus`、`:active`、`:visited`、`:first-child`、`:last-child`、`:nth-child(An+B)`、`:nth-of-type`、`:not()`、`:empty`、`:checked`、`:disabled`、`:enabled`
- 现代：`:is()`、`:where()`、`:has()`
- 伪元素：`::before`、`::after`、`::first-line`、`::first-letter`、`::selection`

### 3.5 CSS 值与单位

- 全部标准长度单位：`px`、`em`、`rem`、`%`、`vw`、`vh`、`vmin`、`vmax`、`ex`、`ch`
- `calc()` 完整算术表达式支持
- CSS 自定义属性（`--var-name`）与 `var()` 含回退值
- 颜色格式：命名色、十六进制（#rgb、#rrggbb、#rrggbbaa）、`rgb()`、`rgba()`、`hsl()`、`hsla()`

### 3.6 CSS At 规则

- `@media` 含标准媒体特性（screen、width、height、min-/max-width、orientation 等）
- `@keyframes`
- `@import`
- `@font-face`（FreeType2 字体加载）
- `@charset`

---

## 4. JavaScript 引擎（QuickJS）

Agave 使用 **QuickJS** 作为 JavaScript 引擎，提供 ES2020 级别的语言支持：

- ES2020 语言特性：箭头函数、`class`、解构、spread/rest、模板字面量、`async`/`await`、`Promise`、`Symbol`、`BigInt`、可选链（`?.`）、空值合并（`??`）
- 完整 ES2015–2020 标准库：`Map`、`Set`、`WeakMap`、`WeakSet`、`Proxy`、`Reflect`、`ArrayBuffer`、类型化数组、`JSON`、`RegExp`

### 4.1 DOM API

| API | 状态 |
|-----|------|
| getElementById / getElementsByTagName / getElementsByClassName | ✅ |
| querySelector / querySelectorAll | ✅ |
| createElement / appendChild / removeChild / insertBefore | ✅ |
| innerHTML / textContent / innerText | ✅ |
| classList（add/remove/toggle/contains/replace） | ✅ |
| dataset | ✅ |
| getAttribute / setAttribute / removeAttribute | ✅ |
| getBoundingClientRect() | ✅ |
| closest() / matches() | ✅ |
| scrollIntoView() | ✅ |
| window.getComputedStyle() | ✅ |
| addEventListener / removeEventListener / dispatchEvent | ✅ |
| MutationObserver | ✅ |
| window.navigator（userAgent、platform、language 等） | ✅ |
| window.location / history | ✅ |
| window.localStorage / sessionStorage | ✅（SQLite 持久化） |
| setTimeout / setInterval / requestAnimationFrame | ✅ |

### 4.2 Web API

| API | 状态 |
|-----|------|
| XMLHttpRequest（XHR） | ✅ |
| fetch() | ✅（libcurl） |
| WebSocket | ✅（libcurl） |
| Canvas 2D Context | ✅ |
| CSS Transitions / Animations | ✅（软件渲染，主线程执行） |
| FormData | ✅ |

---

## 5. 文本与字体

- FreeType2 字体光栅化，支持可选抗锯齿
- 多语言文本支持：拉丁、CJK（中/日/韩）、阿拉伯、西里尔、希腊、希伯来、泰文
- 字符编码：自动检测 + 显式指定 UTF-8、GBK、Big5、EUC-KR、EUC-JP、Shift-JIS、ISO-8859-x
- 通过 `font_config.cfg` 配置字体，无需 Fontconfig 守护进程
- 可配置字体族：标准、衬线、无衬线、等宽
- 双向文本（bidi）支持 RTL 内容

---

## 6. 网络

- HTTP/HTTPS（libcurl，TLS 1.2/1.3，OpenSSL）
- HTTP/2（libcurl）
- FTP / FILE 协议
- 代理：HTTP、SOCKS4、SOCKS5
- 自定义 SSL 证书目录
- Cookie 管理（启用/禁用/清除）
- 磁盘缓存：可配置大小（默认 16 MB），可清除
- 自定义 User-Agent（按 URL，通过回调）
- 请求拦截：`cb_allow_main_request`
- 自定义 URL scheme 处理器（`macross_register_scheme`）
- 内置 HTTP 请求 API：`macross_network_request`（GET/POST）

---

## 7. 渲染管线

渲染管线完全同步、基于 CPU：

1. **布局** — `layoutIfNeededRecursive()` 计算整个页面的几何信息。
2. **绘制** — Picasso 2D 绘入宿主提供的像素缓冲（尺寸任意，不超出引擎限制）。
3. **Tile Buffer（可选）** — 应用可维护比可见视口更大的 tile buffer，实现无需重布局的快速滚动。Tile buffer 位置通过 `macross_view_set_position` 控制。
4. **失效/重绘** — 引擎通过 `cb_invalidate_rect` 通知脏区；宿主按需调度重绘。立即刷新使用 `cb_update_view_now`。

**支持的像素格式：** BGRA32、RGBA32、BGR24、RGB24、RGB16（565）。

---

## 8. 输入处理

- 鼠标事件：按下、抬起、移动、滚轮（含修饰键）
- 键盘事件：按键按下/抬起（虚拟键码）
- IME 文本输入：`macross_view_input_text`，用于软键盘字符合成输入
- 焦点管理：`macross_view_set_focus` / `macross_view_kill_focus`
- IME 状态通知：`cb_set_ime_enable` 回调

---

## 9. 性能特征

| 指标 | 典型值 |
|------|-------|
| 冷启动（引擎初始化） | < 50 ms |
| 首次布局（简单页面） | < 100 ms |
| 内存占用（引擎本体，无页面） | ~4–8 MB |
| 每个页面内存占用 | 8–40 MB（视 DOM 复杂度） |
| 像素吞吐量 | 随 CPU 线性扩展；400 MHz 下 480×480 约 30 FPS |
| JavaScript 堆 | 可配置，默认约 32 MB |

Agave 使用脏矩形重绘：仅重绘引擎标记为脏的区域，最大程度降低动画和滚动时的 CPU 负载。

---

## 10. 平台要求

| 要求 | 最低配置 |
|------|---------|
| 操作系统 | 任意 POSIX 兼容 OS（Linux、带 POSIX 子集的 RTOS） |
| CPU | ARMv7-A / ARMv8-A / x86-64；推荐带 FPU 的 32 位+ |
| RAM | 最低 16 MB；实际页面推荐 64 MB |
| 显示 | 任意接受像素缓冲的帧缓冲或 GPU Surface |
| GPU | **不需要** — 纯软件渲染 |
| 依赖 | libcurl（网络）、FreeType2（字体）、SQLite3（存储） |

---

## 11. 典型应用场景

- **手表 / 可穿戴 HMI** — 圆形或方形 480×480 显示屏；参见 `examples/watchweb`
- **工业面板** — 基于 Web UI 的触控 HMI，内容来自本地存储
- **智能家电** — 来自厂商服务器的受控 Web 内容（食谱、状态仪表盘等）
- **LLM 输出渲染器** — 本地或远端 LLM 的结构化 HTML 输出渲染
- **嵌入式信息亭** — 无头或半无头设备上的单一用途 Web 应用

---

## 12. 安全性

Agave 渲染**可信/半可信内容**，安全模型为：

- **第 1 层（定位约束）：** 面向受控内容，非任意互联网页面。主要的攻击面削减措施是范围限定。
- **第 2 层（代码加固）：** 已修复 QuickJS DOM wrapper UAF、缓存越界、SQLite 内存比较漏洞。持续对照已知 WebKit CVE 模式进行审计。
- **第 3 层（运行时限制）：** 可配置资源上限（JS 堆、DOM 节点数量上限），严格重定向控制，所有网络操作强制超时。

---

## 13. 授权

Agave 引擎核心：版权所有 © 2009–2026 张继鹏。保留所有权利。

第三方组件：QuickJS（MIT）、Picasso（LGPL）、libcurl（curl 许可证）、FreeType2（FTL/GPLv2）、SQLite3（公共领域）。
