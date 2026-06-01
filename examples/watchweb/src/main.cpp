/* main.cpp - WatchWeb entry point
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#define SDL_MAIN_HANDLED
#include "lvgl.h"
#include "macross.h"
#include "webview.h"
#include "ui.h"
#include "config.h"

static WebView g_webview;
static WatchUI g_ui;

static void on_update(void*) { g_ui.updateCanvas(); }
static void on_state(void*)
{
    g_ui.updateProgress(g_webview.progress(), g_webview.isLoading());
    g_ui.updateUrl(g_webview.url());
}

static void engine_tick(lv_timer_t*) { macross_event_dispatch(); }

int main(int argc, char** argv)
{
    lv_init();
    lv_sdl_window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_sdl_mouse_create();
    lv_sdl_keyboard_create();

    macross_initialize(PIXEL_FORMAT_BGRA32, SCREEN_WIDTH, SCREEN_HEIGHT);

    g_webview.create(CONTENT_WIDTH, CONTENT_HEIGHT);
    g_webview.setUpdateCb(on_update, nullptr);
    g_webview.setStateCb(on_state, nullptr);

    g_ui.create(&g_webview);

    const char* url = (argc > 1) ? argv[1] : "https://www.baidu.com";
    g_webview.loadUrl(url);
    g_ui.updateUrl(url);

    lv_timer_create(engine_tick, 16, nullptr);

    while (1) {
        uint32_t ms = lv_timer_handler();
        lv_delay_ms(ms < 5 ? 5 : ms);
    }
    return 0;
}
