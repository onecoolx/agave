/* main.cpp - WatchWeb entry point
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#define SDL_MAIN_HANDLED
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lvgl.h"
#include "macross.h"
#include "webview.h"
#include "ui.h"
#include "config.h"

static WebView g_webview;
static WatchUI g_ui;
static volatile bool g_dirty = false;
static volatile bool g_state_dirty = false;

static void on_update(void*) { g_dirty = true; }
static void on_state(void*) { g_state_dirty = true; }

int main(int argc, char** argv)
{
    lv_init();
    lv_sdl_window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_sdl_mouse_create();
    lv_sdl_keyboard_create();

    if (macross_initialize(PIXEL_FORMAT_BGRA32, SCREEN_WIDTH, SCREEN_HEIGHT) != MC_STATUS_SUCCESS) {
        fprintf(stderr, "Failed to initialize Agave engine\n");
        return -1;
    }

    g_webview.create(CONTENT_WIDTH, CONTENT_HEIGHT);
    g_webview.setUpdateCb(on_update, nullptr);
    g_webview.setStateCb(on_state, nullptr);

    g_ui.create(&g_webview);

    const char* url = (argc > 1) ? argv[1] : "about:blank";
    g_webview.loadUrl(url);
    g_ui.updateUrl(url);

    while (1) {
        /* Dispatch Agave engine events */
        macross_event_dispatch();

        /* Check if webview needs repaint */
        if (g_dirty) {
            g_dirty = false;
            macross_view_update(g_webview.view(), NULL);
            g_ui.updateCanvas();
        }
        if (g_state_dirty) {
            g_state_dirty = false;
            g_ui.updateProgress(g_webview.progress(), g_webview.isLoading());
            g_ui.updateUrl(g_webview.url());
        }

        /* Process LVGL (renders + handles SDL input) */
        uint32_t ms = lv_timer_handler();
        if (ms < 5) ms = 5;
        usleep(ms * 1000);
    }
    return 0;
}
