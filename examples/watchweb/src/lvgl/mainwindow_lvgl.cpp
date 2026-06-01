/* mainwindow_lvgl.cpp - WatchWeb LVGL platform backend
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <stdlib.h>
#include <picasso/picasso.h>

#include "lvgl.h"

#include "event.h"
#include "mainwindow.h"
#include "mainwindow_lvgl.h"
#include "application_lvgl.h"

MainWindowImpl::MainWindowImpl(MainWindow* main)
    : m_window(main)
    , m_gc(nullptr)
    , m_canvas(nullptr)
    , m_buffer(nullptr)
    , m_hInst(nullptr)
    , m_img_obj(nullptr)
{
    memset(&m_img_dsc, 0, sizeof(m_img_dsc));
}

MainWindowImpl::~MainWindowImpl()
{
    Destroy();
}

bool MainWindowImpl::Create(void* hInstance, const char* title, int x, int y, int w, int h)
{
    m_hInst = hInstance;

    /* Allocate framebuffer */
    m_buffer = (unsigned char*)malloc(w * h * 4);
    memset(m_buffer, 0xFF, w * h * 4);

    /* Create Picasso canvas for touchweb widget rendering */
    m_canvas = ps_canvas_create_with_data(m_buffer, COLOR_FORMAT_BGRA, w, h, w * 4);
    m_gc = ps_context_create(m_canvas, 0);

    /* Create LVGL image object to display the framebuffer */
    lv_obj_t* scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

    m_img_obj = lv_image_create(scr);
    lv_obj_set_pos(m_img_obj, 0, 0);
    lv_obj_set_size(m_img_obj, w, h);

    /* Set up image descriptor pointing to our buffer */
    m_img_dsc.header.w = w;
    m_img_dsc.header.h = h;
    m_img_dsc.header.cf = LV_COLOR_FORMAT_ARGB8888;
    m_img_dsc.header.stride = w * 4;
    m_img_dsc.data_size = w * h * 4;
    m_img_dsc.data = m_buffer;
    lv_image_set_src(m_img_obj, &m_img_dsc);

    /* Enable input events on the image */
    lv_obj_add_flag(m_img_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(m_img_obj, sdl_event_cb, LV_EVENT_ALL, this);

    /* Notify touchweb widget system */
    OnCreate(x, y, w, h);

    return true;
}

void MainWindowImpl::Destroy(void)
{
    OnDestroy();

    if (m_gc) {
        ps_context_unref(m_gc);
        m_gc = nullptr;
    }
    if (m_canvas) {
        ps_canvas_unref(m_canvas);
        m_canvas = nullptr;
    }
    if (m_buffer) {
        free(m_buffer);
        m_buffer = nullptr;
    }
}

void MainWindowImpl::OnCreate(int x, int y, int w, int h)
{
    m_window->OnCreate(0, x, y, w, h);
}

void MainWindowImpl::OnDestroy(void)
{
    m_window->OnDestroy();
}

void MainWindowImpl::OnPaint(int x, int y, int w, int h)
{
    Rect rc(x, y, w, h);
    m_window->Paint(m_gc, &rc);
}

void MainWindowImpl::OnUpdate(int x, int y, int w, int h)
{
    /* Repaint the dirty region */
    OnPaint(x, y, w, h);

    /* Tell LVGL to refresh the image */
    if (m_img_obj)
        lv_obj_invalidate(m_img_obj);
}

void MainWindowImpl::OnMouse(int type, unsigned btn, int x, int y)
{
    MouseEvent evt(type, btn, x, y);
    m_window->SendMouseEvent(&evt);
}

void MainWindowImpl::OnKey(int type, unsigned vk)
{
    KeyEvent evt(type, vk);
    m_window->SendKeyEvent(&evt);
}

void MainWindowImpl::OnChar(unsigned int c)
{
    m_window->CharInput(c);
}

void MainWindowImpl::DrawImage(const void* img, int vx, int vy, int cx, int cy,
                               int sx, int sy, int x, int y, int w, int h)
{
    /* Used by WebView for buffer-raise rendering - copy from view buffer to window buffer */
    (void)img; (void)vx; (void)vy; (void)cx; (void)cy;
    (void)sx; (void)sy; (void)x; (void)y; (void)w; (void)h;
}

void MainWindowImpl::processEvents(void)
{
    /* Called from main loop - LVGL handles SDL events internally */
}

/* LVGL event callback - forward touch/mouse to touchweb widget system */
void MainWindowImpl::sdl_event_cb(lv_event_t* e)
{
    MainWindowImpl* self = (MainWindowImpl*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED || code == LV_EVENT_PRESSING || code == LV_EVENT_RELEASED) {
        lv_point_t point;
        lv_indev_get_point(lv_indev_active(), &point);

        if (code == LV_EVENT_PRESSED)
            self->OnMouse(1, 1, point.x, point.y);  /* press */
        else if (code == LV_EVENT_RELEASED)
            self->OnMouse(0, 1, point.x, point.y);  /* release */
        else if (code == LV_EVENT_PRESSING)
            self->OnMouse(2, 0, point.x, point.y);  /* move */
    }
}
