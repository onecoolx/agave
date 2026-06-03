/* main.cpp - WatchWeb entry point
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#define SDL_MAIN_HANDLED
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <lvgl.h>
#include <src/drivers/sdl/lv_sdl_window.h>
#include <src/drivers/sdl/lv_sdl_mouse.h>
#include <src/drivers/sdl/lv_sdl_keyboard.h>
#include "macross.h"
#include "webview.h"
#include "ui.h"
#include "config.h"

static WebView* g_webview = nullptr;
static WatchUI* g_ui = nullptr;
static volatile bool g_dirty = false; /* engine rendered: paint+blit */
static volatile bool g_blit = false; /* scroll only: blit visible region */
static volatile bool g_state_dirty = false;

static void on_update(void*) { g_dirty = true; }
static void on_blit(void*) { g_blit = true; }
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

    g_webview = new WebView();
    g_ui = new WatchUI();

    g_webview->create(CONTENT_WIDTH, CONTENT_HEIGHT);
    g_webview->setUpdateCb(on_update, nullptr);
    g_webview->setStateCb(on_state, nullptr);
    g_webview->setBlitCb(on_blit, nullptr);

    g_ui->create(g_webview);

    const char* url = (argc > 1) ? argv[1] : "about:blank";
    g_webview->loadUrl(url);

    bool running = true;
    while (running) {
        /* Check for quit before LVGL consumes the event */
        if (SDL_QuitRequested()) {
            running = false;
            break;
        }

        macross_event_dispatch();

        if (g_dirty) {
            g_dirty = false;
            /* Engine signalled content changed (layout done): render tile
               buffer, then blit visible region to canvas. */
            macross_view_update(g_webview->view(), NULL);
            g_ui->updateCanvas();
        } else if (g_blit) {
            g_blit = false;
            /* Scroll within already-rendered tile buffer: blit only, no
               engine paint (engine layout may still be pending). */
            g_ui->updateCanvas();
        }
        if (g_state_dirty) {
            g_state_dirty = false;
            g_ui->updateProgress(g_webview->progress(), g_webview->isLoading());
        }

        uint32_t ms = lv_timer_handler();
        if (ms < 5) { ms = 5; }
        usleep(ms * 1000);
    }

    /* Explicit cleanup while timer system is still valid (avoids static
       destruction order fiasco where timerHeap is gone before Page::~Page) */
    delete g_ui;
    g_ui = nullptr;
    delete g_webview; /* macross_view_destroy -> Page::~Page -> timer stop */
    g_webview = nullptr;
    macross_shutdown();

    return 0;
}
