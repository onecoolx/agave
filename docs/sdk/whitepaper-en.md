# Agave Web Engine — Technical White Paper

> Version 1.0 · June 2026

---

## 1. Overview

Agave is a lightweight, self-contained web rendering engine designed for embedded systems and resource-constrained platforms.  It provides a complete browser engine stack — HTML parsing, CSS layout, JavaScript execution, and software rasterization — in a single library with a clean C API.

Agave is intentionally positioned as a **focused, high-quality engine for trusted/semi-trusted content rendering**, not a general-purpose internet browser.  Its pure software rendering architecture means it runs on hardware without a GPU, making it suitable for industrial HMI panels, wearable devices, smart appliances, and any mid-range SoC running a real-time or embedded OS.

---

## 2. Architecture

```
┌──────────────────────────────────────────────┐
│                 Host Application              │
│          (watchweb / custom browser)          │
└──────────────┬───────────────────────────────┘
               │  C API  (include/macross.h)
┌──────────────▼───────────────────────────────┐
│               Agave Engine Core               │
│  ┌───────────┐ ┌──────────┐ ┌─────────────┐  │
│  │ HTML/CSS  │ │ Layout   │ │  JavaScript │  │
│  │  Parser   │ │ Engine   │ │  (QuickJS)  │  │
│  └─────┬─────┘ └────┬─────┘ └──────┬──────┘  │
│        └────────────┼──────────────┘          │
│             ┌───────▼──────┐                  │
│             │  Render Tree │                  │
│             └───────┬──────┘                  │
│             ┌───────▼──────┐                  │
│             │   Picasso    │  (2D rasterizer) │
│             └───────┬──────┘                  │
└─────────────────────┼────────────────────────┘
                       │  BGRA32/RGBA32/RGB16 pixel buffer
               ┌───────▼──────┐
               │  Host Buffer │  (framebuffer / LCD / LVGL canvas)
               └──────────────┘
```

### 2.1 Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| Pure software rendering (Picasso 2D) | No GPU dependency; works on any SoC |
| Tile-buffer rendering model | Decouples page layout size from display resolution |
| QuickJS JavaScript engine | ES2020-level JS in ~230 KB of compiled code |
| C public API | Binary-compatible with C, C++, and any FFI |
| Statically linked | One `.so` / `.a`, no runtime framework dependency |

---

## 3. HTML / CSS Support

### 3.1 HTML Standards

- Full HTML 4.01 parsing with error recovery
- HTML5 structural elements: `<header>`, `<footer>`, `<article>`, `<section>`, `<nav>`, `<main>`, `<aside>`, `<figure>`, `<figcaption>`, `<time>`, `<mark>`
- HTML5 forms: `<input>` with types `text`, `number`, `email`, `url`, `tel`, `search`, `password`, `checkbox`, `radio`, `range`, `date`, `color`, `file`, `hidden`, `submit`, `reset`, `button`; `<textarea>`, `<select>`, `<datalist>`, `<fieldset>`, `<legend>`, `<output>`
- HTML5 constraint validation API (`required`, `pattern`, `min`, `max`, `step`, `minlength`, `maxlength`)
- `<canvas>` element with 2D drawing API (Canvas 2D Context)
- `<script>` (sync and async), `<link>`, `<style>`, `<meta>`, `<base>`
- `<img>` with `object-fit`, `object-position`, `aspect-ratio`
- `<iframe>` for embedded sub-frames
- `<table>` with full colspan/rowspan layout

### 3.2 CSS Layout

| Feature | Status |
|---------|--------|
| Normal flow (block / inline / inline-block) | ✅ Full |
| **Modern CSS Flexbox** (display: flex/inline-flex) | ✅ Full |
| CSS Grid (display: grid/inline-grid) | ✅ Implemented |
| CSS Tables | ✅ Full |
| Float & clear | ✅ Full |
| Absolute / fixed / relative / sticky positioning | ✅ Full |
| Multi-column layout | ✅ |
| `box-sizing: border-box` | ✅ |

**Flexbox details:** `flex-direction`, `flex-wrap`, `flex-flow`, `flex-grow`, `flex-shrink`, `flex-basis`, `flex` (shorthand), `justify-content`, `align-items`, `align-content`, `align-self`, `order`, auto margins.  All standard values including `space-between`, `space-around`, `wrap-reverse`, `column-reverse`.

### 3.3 CSS Visual

| Feature | Status |
|---------|--------|
| Box model (margin, border, padding, width, height) | ✅ Full |
| `border-radius` (per-corner) | ✅ |
| `box-shadow` (multiple, inset) | ✅ |
| `background` (color, image, linear-gradient, radial-gradient, conic-gradient) | ✅ |
| `background-size`, `background-position`, `background-repeat`, `background-origin`, `background-clip` | ✅ |
| `color`, `opacity` | ✅ |
| `transform` (translate, scale, rotate, skew, matrix, 3D subset) | ✅ |
| `filter` (blur, drop-shadow, opacity, brightness, contrast, saturate, grayscale, hue-rotate, sepia, invert) | ✅ |
| `mix-blend-mode` | ✅ |
| `clip-path` (basic shapes: circle, ellipse, inset, polygon) | ✅ |
| `mask-image` (linear-gradient based masks) | ✅ |
| `object-fit` / `object-position` | ✅ |
| `aspect-ratio` | ✅ |
| `visibility`, `display`, `overflow` | ✅ |
| `cursor` | ✅ |
| `outline` | ✅ |
| `transition` (property, duration, delay, timing-function) | ✅ |
| `animation` / `@keyframes` | ✅ |

### 3.4 CSS Selectors

- All CSS 2.1 selectors (type, class, id, descendant, child, adjacent, attribute)
- CSS 3 pseudo-classes: `:hover`, `:focus`, `:active`, `:visited`, `:first-child`, `:last-child`, `:nth-child(An+B)`, `:nth-of-type`, `:not()`, `:empty`, `:checked`, `:disabled`, `:enabled`
- Modern: `:is()`, `:where()`, `:has()`
- Pseudo-elements: `::before`, `::after`, `::first-line`, `::first-letter`, `::selection`

### 3.5 CSS Values & Units

- All standard length units: `px`, `em`, `rem`, `%`, `vw`, `vh`, `vmin`, `vmax`, `ex`, `ch`
- `calc()` with full arithmetic expression support
- CSS Custom Properties (`--var-name`) and `var()` with fallback
- Color formats: named, hex (#rgb, #rrggbb, #rrggbbaa), `rgb()`, `rgba()`, `hsl()`, `hsla()`
- `currentColor`, `inherit`, `initial`, `unset`

### 3.6 CSS At-Rules

- `@media` with standard media features (screen, width, height, min-/max-width, orientation, etc.)
- `@keyframes`
- `@import`
- `@font-face` (with FreeType2 font loading)
- `@charset`

---

## 4. JavaScript Engine (QuickJS)

Agave uses **QuickJS** as its JavaScript engine, providing ES2020-level language support:

- ES2020 language features: arrow functions, `class`, destructuring, spread/rest, template literals, `async`/`await`, `Promise`, `Symbol`, `BigInt`, optional chaining (`?.`), nullish coalescing (`??`)
- Full ES2015–2020 standard library: `Map`, `Set`, `WeakMap`, `WeakSet`, `Proxy`, `Reflect`, `ArrayBuffer`, typed arrays, `JSON`, `RegExp`

### 4.1 DOM API

| API | Status |
|-----|--------|
| `document.getElementById/getElementsByTagName/getElementsByClassName` | ✅ |
| `document.querySelector` / `querySelectorAll` | ✅ |
| `document.createElement` / `appendChild` / `removeChild` / `insertBefore` | ✅ |
| `element.innerHTML` / `textContent` / `innerText` | ✅ |
| `element.classList` (`add`, `remove`, `toggle`, `contains`, `replace`) | ✅ |
| `element.dataset` | ✅ |
| `element.getAttribute` / `setAttribute` / `removeAttribute` | ✅ |
| `element.getBoundingClientRect()` | ✅ |
| `element.closest()` / `element.matches()` | ✅ |
| `element.scrollIntoView()` | ✅ |
| `window.getComputedStyle()` | ✅ |
| `addEventListener` / `removeEventListener` / `dispatchEvent` | ✅ |
| `MutationObserver` | ✅ |
| `window.navigator` (userAgent, platform, language, cookieEnabled, onLine) | ✅ |
| `window.location` | ✅ |
| `window.history` (back, forward, go) | ✅ |
| `window.localStorage` / `window.sessionStorage` | ✅ (SQLite-backed) |
| `window.alert` / `confirm` / `prompt` | ✅ (via callbacks) |
| `setTimeout` / `setInterval` / `clearTimeout` / `clearInterval` | ✅ |
| `window.requestAnimationFrame` | ✅ |

### 4.2 Web APIs

| API | Status |
|-----|--------|
| `XMLHttpRequest` (XHR) | ✅ |
| `fetch()` | ✅ (libcurl-based) |
| `WebSocket` | ✅ (libcurl-based) |
| `localStorage` / `sessionStorage` | ✅ (SQLite-backed) |
| `Canvas 2D Context` | ✅ |
| `CSS Transitions / Animations` | ✅ (software, main-thread) |
| `FormData` | ✅ |

---

## 5. Text & Fonts

- FreeType2 font rasterization with optional anti-aliasing
- Multi-language text support: Latin, CJK (Chinese / Japanese / Korean), Arabic, Cyrillic, Greek, Hebrew, Thai
- Character encoding: auto-detect + explicit UTF-8, GBK, Big5, EUC-KR, EUC-JP, Shift-JIS, ISO-8859-x
- Font configuration via `font_config.cfg` — no Fontconfig daemon dependency
- Configurable font families: standard, serif, sans-serif, monospace
- Text rendering modes: normal, anti-aliased
- Bi-directional text (bidi) support for RTL content

---

## 6. Networking

- HTTP/HTTPS via libcurl (TLS 1.2/1.3 with OpenSSL)
- HTTP/2 (via libcurl)
- FTP / FILE protocol support
- Proxy: HTTP, SOCKS4, SOCKS5
- Custom SSL certificate directory
- Cookie management (enable/disable, clear)
- Disk cache: configurable size (default 16 MB), clearable
- Custom User-Agent per URL (via callback)
- Request interception: `cb_allow_main_request`
- Custom URL scheme handlers (`macross_register_scheme`)
- Built-in HTTP request API: `macross_network_request` (GET/POST with callback)

---

## 7. Rendering Pipeline

The rendering pipeline is fully synchronous and CPU-based:

1. **Layout** — `layoutIfNeededRecursive()` computes geometry for the entire page.
2. **Paint** — Picasso 2D draws into a host-provided pixel buffer (any size up to engine limits).
3. **Tile Buffer** (optional) — Applications may maintain a tile buffer larger than the visible viewport, enabling fast scroll without re-layout. The tile buffer position is controlled via `macross_view_set_position`.
4. **Invalidate / Repaint** — The engine signals dirty regions via `cb_invalidate_rect`; the host schedules a repaint at its convenience. For immediate flush: `cb_update_view_now`.

**Pixel formats supported:** BGRA32, RGBA32, BGR24, RGB24, RGB16 (565).

---

## 8. Input Handling

- Mouse events: down, up, move, wheel (with modifier keys)
- Keyboard events: key down, key up (virtual key codes)
- IME text input: `macross_view_input_text` for composing characters from a soft keyboard
- Focus management: `macross_view_set_focus` / `macross_view_kill_focus`
- IME state notifications via `cb_set_ime_enable` callback

---

## 9. Performance Characteristics

| Metric | Typical Value |
|--------|---------------|
| Cold startup (engine init) | < 50 ms |
| First layout (simple page) | < 100 ms |
| Memory footprint (engine only, no page) | ~4–8 MB |
| Memory per loaded page | 8–40 MB depending on DOM complexity |
| Pixel throughput | Scales with CPU; ~30 FPS on 480×480 at 400 MHz |
| JavaScript heap | Configurable; default ~32 MB |

Agave uses dirty-rect repaint: only regions invalidated by the engine are repainted, minimizing CPU load during animation and scrolling.

---

## 10. Platform Requirements

| Requirement | Minimum |
|-------------|---------|
| OS | Any POSIX-compatible OS (Linux, RTOS with POSIX subset) |
| CPU | ARMv7-A / ARMv8-A / x86-64; any 32-bit+ with FPU recommended |
| RAM | 16 MB minimum; 64 MB recommended for real pages |
| Display | Any framebuffer or GPU surface accepting a pixel buffer |
| GPU | **Not required** — pure software rendering |
| Dependencies | libcurl (network), FreeType2 (fonts), SQLite3 (storage) |

---

## 11. Deployment Scenarios

- **Watch / Wearable HMI** — Circular or square 480×480 display; see `examples/watchweb`
- **Industrial panel** — Touch screen HMI with web-based UI served from local storage
- **Smart appliance** — Controlled web content (e.g. recipes, status dashboards) from a manufacturer server
- **LLM output renderer** — Structured HTML output from a local or remote LLM
- **Embedded kiosk** — Single-purpose web application on a headless or semi-headless device

---

## 12. Security Posture

Agave renders **trusted / semi-trusted content**. Its security model:

- **Layer 1 (Positioning):** Intended for controlled content, not arbitrary internet pages.  The primary attack-surface reduction is scope-limiting.
- **Layer 2 (Code hardening):** QuickJS DOM wrapper UAF eliminated; cache out-of-bounds fixed; SQLite memory comparison hardened.  Ongoing audit against known WebKit CVE patterns.
- **Layer 3 (Runtime limits):** Configurable resource limits (JS heap, DOM node caps), strict redirect controls, timeout enforcement on all network operations.

---

## 13. Licensing

Agave engine core: Copyright © 2009–2026 Zhang Ji Peng. All rights reserved.

Third-party components: QuickJS (MIT), Picasso (LGPL), libcurl (curl license), FreeType2 (FTL/GPLv2), SQLite3 (public domain).
