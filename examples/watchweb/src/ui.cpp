/* ui.cpp - LVGL full-screen watch browser UI
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#include <string.h>
#include <stdlib.h>
#include "lvgl.h"
#include "ui.h"
#include "webview.h"

WatchUI::WatchUI()
    : m_bg(nullptr), m_canvas_img(nullptr), m_progress(nullptr)
    , m_nav_bar(nullptr), m_fab(nullptr), m_wv(nullptr)
    , m_tx(0), m_ty(0), m_dragging(false), m_nav_visible(false)
{
}

WatchUI::~WatchUI() {}

void WatchUI::create(WebView* wv)
{
    m_wv = wv;
    createBackground();
    createCanvas();
    createProgress();
    createNavBar();
    createFab();
}

void WatchUI::createBackground()
{
    m_bg = lv_obj_create(lv_screen_active());
    lv_obj_set_size(m_bg, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(m_bg, 0, 0);
    lv_obj_set_style_radius(m_bg, 0, 0);
    lv_obj_set_style_bg_color(m_bg, lv_color_make(18, 18, 18), 0);
    lv_obj_set_style_border_width(m_bg, 0, 0);
    lv_obj_set_style_pad_all(m_bg, 0, 0);
    lv_obj_clear_flag(m_bg, LV_OBJ_FLAG_SCROLLABLE);
}

void WatchUI::createCanvas()
{
    /* Full-screen canvas; the round panel clips the corners in hardware. */
    m_canvas_img = lv_canvas_create(m_bg);
    lv_obj_set_pos(m_canvas_img, 0, 0);
    lv_obj_set_size(m_canvas_img, CONTENT_WIDTH, CONTENT_HEIGHT);
    lv_obj_add_flag(m_canvas_img, LV_OBJ_FLAG_CLICKABLE);

    static uint8_t* canvas_buf = (uint8_t*)malloc(CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    memset(canvas_buf, 0xFF, CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    lv_canvas_set_buffer(m_canvas_img, canvas_buf, CONTENT_WIDTH, CONTENT_HEIGHT, LV_COLOR_FORMAT_ARGB8888);

    lv_obj_add_event_cb(m_canvas_img, on_canvas_press, LV_EVENT_PRESSED, this);
    lv_obj_add_event_cb(m_canvas_img, on_canvas_move, LV_EVENT_PRESSING, this);
    lv_obj_add_event_cb(m_canvas_img, on_canvas_release, LV_EVENT_RELEASED, this);
}

void WatchUI::createProgress()
{
    m_progress = lv_arc_create(m_bg);
    lv_obj_set_size(m_progress, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4);
    lv_obj_center(m_progress);
    lv_arc_set_rotation(m_progress, 270);
    lv_arc_set_bg_angles(m_progress, 0, 360);
    lv_arc_set_range(m_progress, 0, 100);
    lv_arc_set_value(m_progress, 0);
    lv_obj_remove_style(m_progress, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(m_progress, PROGRESS_WIDTH, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(m_progress, lv_color_make(0, 150, 255), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(m_progress, LV_OPA_0, LV_PART_MAIN);
    lv_obj_clear_flag(m_progress, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
}

void WatchUI::createNavBar()
{
    /* Bottom panel that slides up when the FAB is tapped. Hidden by default. */
    m_nav_bar = lv_obj_create(m_bg);
    lv_obj_set_size(m_nav_bar, SCREEN_WIDTH, NAV_BAR_H);
    lv_obj_set_pos(m_nav_bar, 0, SCREEN_HEIGHT); /* off-screen (hidden) */
    lv_obj_set_style_radius(m_nav_bar, 0, 0);
    lv_obj_set_style_bg_color(m_nav_bar, lv_color_make(30, 30, 30), 0);
    lv_obj_set_style_bg_opa(m_nav_bar, LV_OPA_90, 0);
    lv_obj_set_style_border_width(m_nav_bar, 0, 0);
    lv_obj_set_style_pad_all(m_nav_bar, 0, 0);
    lv_obj_clear_flag(m_nav_bar, LV_OBJ_FLAG_SCROLLABLE);

    int cx = SCREEN_WIDTH / 2;
    int y = (NAV_BAR_H - NAV_BTN_SIZE) / 2;
    int gap = NAV_BTN_SIZE + 40;

    auto make_btn = [&](int x, const char* icon, lv_event_cb_t cb) {
        lv_obj_t* btn = lv_button_create(m_nav_bar);
        lv_obj_set_size(btn, NAV_BTN_SIZE, NAV_BTN_SIZE);
        lv_obj_set_pos(btn, x - NAV_BTN_SIZE / 2, y);
        lv_obj_set_style_radius(btn, NAV_BTN_SIZE / 2, 0);
        lv_obj_set_style_bg_color(btn, lv_color_make(60, 60, 60), 0);
        lv_obj_set_style_bg_color(btn, lv_color_make(90, 90, 90), LV_STATE_PRESSED);
        lv_obj_t* lbl = lv_label_create(btn);
        lv_label_set_text(lbl, icon);
        lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, this);
    };

    make_btn(cx - gap, LV_SYMBOL_LEFT, on_back);
    make_btn(cx, LV_SYMBOL_REFRESH, on_refresh);
    make_btn(cx + gap, LV_SYMBOL_RIGHT, on_fwd);
}

void WatchUI::createFab()
{
    /* Small floating toggle at the bottom-center; tapping shows/hides nav. */
    m_fab = lv_button_create(m_bg);
    lv_obj_set_size(m_fab, FAB_SIZE, FAB_SIZE);
    lv_obj_set_pos(m_fab, (SCREEN_WIDTH - FAB_SIZE) / 2,
                   SCREEN_HEIGHT - FAB_SIZE - FAB_MARGIN);
    lv_obj_set_style_radius(m_fab, FAB_SIZE / 2, 0);
    lv_obj_set_style_bg_color(m_fab, lv_color_make(0, 150, 255), 0);
    lv_obj_set_style_bg_opa(m_fab, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(m_fab, lv_color_make(0, 110, 200), LV_STATE_PRESSED);
    lv_obj_t* lbl = lv_label_create(m_fab);
    lv_label_set_text(lbl, LV_SYMBOL_UP);
    lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
    lv_obj_center(lbl);
    lv_obj_add_event_cb(m_fab, on_fab, LV_EVENT_CLICKED, this);
}

void WatchUI::setNavVisible(bool visible)
{
    m_nav_visible = visible;
    /* Slide the bar in/out and flip the FAB arrow + reposition above the bar. */
    lv_obj_set_y(m_nav_bar, visible ? (SCREEN_HEIGHT - NAV_BAR_H) : SCREEN_HEIGHT);
    int fab_y = visible ? (SCREEN_HEIGHT - NAV_BAR_H - FAB_SIZE - FAB_MARGIN)
                : (SCREEN_HEIGHT - FAB_SIZE - FAB_MARGIN);
    lv_obj_set_y(m_fab, fab_y);
    lv_obj_t* lbl = lv_obj_get_child(m_fab, 0);
    if (lbl) { lv_label_set_text(lbl, visible ? LV_SYMBOL_DOWN : LV_SYMBOL_UP); }
}

/* --- Public update methods --- */

void WatchUI::updateCanvas()
{
    uint8_t* dst = (uint8_t*)lv_canvas_get_buf(m_canvas_img);
    const uint8_t* src = m_wv->buffer();
    int ox = m_wv->offsetX();
    int oy = m_wv->offsetY();
    int src_stride = TILE_BUF_W * 4;
    int dst_stride = CONTENT_WIDTH * 4;

    for (int row = 0; row < CONTENT_HEIGHT; row++) {
        memcpy(dst + row * dst_stride,
               src + (oy + row) * src_stride + ox * 4,
               dst_stride);
    }
    lv_obj_invalidate(m_canvas_img);
}

void WatchUI::updateProgress(unsigned int pct, bool loading)
{
    if (loading) {
        lv_obj_clear_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
        lv_arc_set_value(m_progress, pct);
    } else {
        lv_obj_add_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
    }
}

/* --- Event handlers --- */

void WatchUI::on_canvas_press(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    lv_point_t p;
    lv_indev_get_point(lv_indev_active(), &p);
    ui->m_tx = p.x;
    ui->m_ty = p.y;
    ui->m_dragging = false;
}

void WatchUI::on_canvas_move(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    lv_point_t p;
    lv_indev_get_point(lv_indev_active(), &p);
    int dx = ui->m_tx - p.x;
    int dy = ui->m_ty - p.y;

    if (!ui->m_dragging && (abs(dx) > 5 || abs(dy) > 5)) {
        ui->m_dragging = true;
    }

    if (ui->m_dragging) {
        ui->m_wv->scrollBy(dx, dy);
        ui->m_tx = p.x;
        ui->m_ty = p.y;
    }
}

void WatchUI::on_canvas_release(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    if (!ui->m_dragging) {
        lv_point_t p;
        lv_indev_get_point(lv_indev_active(), &p);
        ui->m_wv->mousePress(p.x, p.y);
        ui->m_wv->mouseRelease(p.x, p.y);
    }
    ui->m_dragging = false;
}

void WatchUI::on_fab(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->setNavVisible(!ui->m_nav_visible);
}

void WatchUI::on_back(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->goBack();
}

void WatchUI::on_fwd(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->goForward();
}

void WatchUI::on_refresh(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->reload();
}
