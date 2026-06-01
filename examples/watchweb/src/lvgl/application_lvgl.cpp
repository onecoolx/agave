/* application_lvgl.cpp - WatchWeb LVGL platform backend
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <time.h>

#define SDL_MAIN_HANDLED
#include "lvgl.h"

#include "macross.h"
#include "application.h"
#include "application_lvgl.h"

ApplicationImpl::ApplicationImpl(Application* app)
    : m_app(app)
    , m_running(false)
{
    lv_init();
    macross_initialize(PIXEL_FORMAT_BGRA32, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

ApplicationImpl::~ApplicationImpl()
{
    macross_shutdown();
    lv_deinit();
}

void ApplicationImpl::init(void)
{
    /* Create SDL window via LVGL SDL driver */
    lv_sdl_window_create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    lv_sdl_mouse_create();
    lv_sdl_keyboard_create();
}

int ApplicationImpl::run_loop(void)
{
    m_running = true;
    while (m_running) {
        /* Process LVGL timers (handles SDL events internally) */
        uint32_t delay = lv_timer_handler();

        /* Drive touchweb event system */
        if (!m_app->event_loop()) {
            m_app->idle_loop();
        }
        m_app->loop_callback();

        lv_delay_ms(delay < 5 ? 5 : delay);
    }
    return 0;
}

unsigned long ApplicationImpl::tickCount(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int ApplicationImpl::today(void) const
{
    time_t tm = time(0);
    struct tm* ptm = localtime(&tm);
    int today = (ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday;
    return today;
}

void SetImeStatus_platform(bool b) { (void)b; }
bool ImeIsShow_platform(void) { return false; }
void System_init(void) {}

int get_virtual_key(int pk)
{
    /* SDL keycodes map to touchweb virtual keys */
    /* TODO: full key mapping */
    return pk;
}
