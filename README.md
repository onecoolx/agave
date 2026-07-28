[![Windows](https://github.com/onecoolx/agave/actions/workflows/windows.yml/badge.svg)](https://github.com/onecoolx/agave/actions/workflows/windows.yml)
[![Linux](https://github.com/onecoolx/agave/actions/workflows/linux.yml/badge.svg)](https://github.com/onecoolx/agave/actions/workflows/linux.yml)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/onecoolx/agave)

# Agave Web Engine

A fast, lightweight and self-contained web rendering engine for embedded systems and resource-constrained platforms. Agave provides a complete browser engine stack — HTML/CSS parsing, JavaScript execution (QuickJS / ES2020), and pure-software rasterization (Picasso 2D) — exposed through a clean C API.

## Features

- **HTML5 / CSS3** — Flexbox, Grid, transitions, animations, `@keyframes`, filters, gradients, `clip-path`, `mask-image`, `mix-blend-mode`, CSS custom properties (`var()`), `calc()`, media queries
- **JavaScript** — QuickJS engine (ES2020): `async/await`, `Promise`, `class`, optional chaining, nullish coalescing
- **Web APIs** — `fetch`, `XMLHttpRequest`, `WebSocket`, `localStorage`/`sessionStorage` (SQLite-backed), `MutationObserver`, `querySelector`/`querySelectorAll`, `classList`, `dataset`, `getBoundingClientRect`, `getComputedStyle`, `navigator`, `location`, `console`
- **Pure software rendering** — No GPU required; works on any SoC with a framebuffer
- **Tile-buffer scrolling** — Decouples page layout size from display resolution for smooth scrolling
- **Configurable pixel formats** — BGRA32, RGBA32, BGR24, RGB24, RGB16(565)
- **Multi-platform** — Linux, Windows; integrates with LVGL, Qt5, SDL, or any custom framebuffer

## Example Applications

| Example | Description |
|---------|-------------|
| `watchweb` | Round-screen watch browser (480×480, LVGL) |
| `touchweb` | Mobile touch browser (Qt5) |
| `agave_test` | Headless test harness |

## Requirements

- CMake ≥ 3.16
- C++11 compiler (GCC / Clang / MSVC)
- libcurl (network)
- FreeType2 (fonts)
- SQLite3 (Web Storage)
- Qt5 (touchweb only)
- LVGL (watchweb only)

## Build

### Linux (with ASan for development)

```bash
mkdir proj && cd proj
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### Linux (release / no ASan)

```bash
mkdir proj_no_asan && cd proj_no_asan
cmake .. -DCMAKE_BUILD_TYPE=Release -DOPT_USE_ASAN=OFF
make -j$(nproc)
```

### Build a specific target

```bash
make watchweb -j$(nproc)    # watch-face browser (LVGL)
make touchweb -j$(nproc)    # mobile browser (Qt5)
make agave_test -j$(nproc)  # headless test runner
make unit_tests -j$(nproc)  # unit test suite
```

### Windows (MSVC)

```bat
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Run

```bash
# watchweb — round-screen browser (SDL headless for CI)
env SDL_VIDEODRIVER=dummy ./watchweb https://example.com

# touchweb — Qt5 browser
./touchweb https://example.com

# unit tests
./unit_tests
```

## API Overview

The public API is in `include/macross.h`. Key entry points:

```c
#include "macross.h"

// 1. Initialize engine (once per process)
macross_initialize(PIXEL_FORMAT_BGRA32, width, height);

// 2. Register callbacks
MC_CALLBACK_INFO cb = {};
cb.cb_invalidate_rect = on_dirty;   // schedule repaint
cb.cb_loading_progress = on_progress;
cb.cb_set_ime_enable = on_ime;      // show/hide soft keyboard
macross_set_callback(&cb);

// 3. Create a view backed by a pixel buffer
uint8_t* buf = malloc(width * height * 4);
MaCrossView* view = macross_view_create(buf, width, height, width*4, userdata);

// 4. Load content
macross_view_open_url(view, "https://example.com");

// 5. Main loop
while (running) {
    macross_event_dispatch();       // process timers / network / layout
    if (dirty) {
        macross_view_update(view, NULL);   // render into buf
        blit_to_screen(buf);
    }
}

// 6. Cleanup
macross_view_destroy(view);
macross_shutdown();
```

Full API documentation: [`docs/sdk/api-datasheet-en.md`](docs/sdk/api-datasheet-en.md)  
Programming guide: [`docs/sdk/programming-guide-en.md`](docs/sdk/programming-guide-en.md)  
Technical whitepaper: [`docs/sdk/whitepaper-en.md`](docs/sdk/whitepaper-en.md)

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `OPT_USE_ASAN` | ON | Enable AddressSanitizer |
| `OPT_USE_QJS` | ON | Use QuickJS (ES2020) |
| `OPT_MODERN_FLEXBOX` | ON | Modern CSS Flexbox layout |
| `OPT_UNITTEST` | ON | Build unit tests |
| `OPT_FREE_TYPE2` | ON | FreeType2 font rendering |
| `OPT_FONT_CONFIG` | OFF | Use fontconfig (OFF = built-in font config) |

## License

Copyright © 2009–2026 Zhang Ji Peng. All rights reserved.

Third-party components: QuickJS (MIT), Picasso (BSD), libcurl (curl), FreeType2 (FTL/GPLv2), SQLite3 (public domain).
