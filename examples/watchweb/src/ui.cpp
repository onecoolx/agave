/* ui.cpp - LVGL circular watch browser UI
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#include <string.h>
#include <stdlib.h>
#include "lvgl.h"
#include "ui.h"
#include "webview.h"

WatchUI::WatchUI()
    : m_bg(nullptr), m_addr(nullptr), m_canvas_img(nullptr)
    , m_progress(nullptr), m_nav_back(nullptr), m_nav_fwd(nullptr)
    , m_nav_reload(nullptr), m_wv(nullptr)
    , m_tx(0), m_ty(0), m_dragging(false)
{
    memset(&m_img_dsc, 0, sizeof(m_img_dsc));
}

WatchUI::~WatchUI() {}

void WatchUI::create(WebView* wv)
{
    m_wv = wv;
    createBackground();
    createProgress();
    createAddressBar();
    createCanvas();
    createNavBar();
}

void WatchUI::createBackground()
{
    m_bg = lv_obj_create(lv_screen_active());
    lv_obj_set_size(m_bg, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_center(m_bg);
    lv_obj_set_style_radius(m_bg, SCREEN_RADIUS, 0);
    lv_obj_set_style_bg_color(m_bg, lv_color_make(18, 18, 18), 0);
    lv_obj_set_style_border_width(m_bg, 0, 0);
    lv_obj_set_style_pad_all(m_bg, 0, 0);
    lv_obj_clear_flag(m_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_clip_corner(m_bg, true, 0);
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
    lv_obj_set_style_arc_width(m_progress, PROGRESS_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(m_progress, LV_OPA_0, LV_PART_MAIN);
    lv_obj_clear_flag(m_progress, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
}

void WatchUI::createAddressBar()
{
    m_addr = lv_textarea_create(m_bg);
    lv_obj_set_pos(m_addr, ADDR_BAR_X, ADDR_BAR_Y);
    lv_obj_set_size(m_addr, ADDR_BAR_W, ADDR_BAR_H);
    lv_textarea_set_one_line(m_addr, true);
    lv_textarea_set_placeholder_text(m_addr, "Search or URL");
    lv_obj_set_style_radius(m_addr, ADDR_BAR_H / 2, 0);
    lv_obj_set_style_bg_color(m_addr, lv_color_make(40, 40, 40), 0);
    lv_obj_set_style_text_color(m_addr, lv_color_white(), 0);
    lv_obj_set_style_border_color(m_addr, lv_color_make(60, 60, 60), 0);
    lv_obj_set_style_border_width(m_addr, 1, 0);
    lv_obj_set_style_text_font(m_addr, &lv_font_montserrat_14, 0);
    lv_obj_add_event_cb(m_addr, on_addr_ready, LV_EVENT_READY, this);
}

void WatchUI::createCanvas()
{
    /* Use canvas with its own buffer - we blit from tile buffer into it */
    m_canvas_img = lv_canvas_create(m_bg);
    lv_obj_set_pos(m_canvas_img, CONTENT_INSET, CONTENT_TOP);
    lv_obj_set_size(m_canvas_img, CONTENT_WIDTH, CONTENT_HEIGHT);
    lv_obj_set_style_radius(m_canvas_img, 8, 0);
    lv_obj_set_style_clip_corner(m_canvas_img, true, 0);
    lv_obj_add_flag(m_canvas_img, LV_OBJ_FLAG_CLICKABLE);

    static uint8_t* canvas_buf = (uint8_t*)malloc(CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    memset(canvas_buf, 0xFF, CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    lv_canvas_set_buffer(m_canvas_img, canvas_buf, CONTENT_WIDTH, CONTENT_HEIGHT, LV_COLOR_FORMAT_ARGB8888);

    /* Touch events */
    lv_obj_add_event_cb(m_canvas_img, on_canvas_press, LV_EVENT_PRESSED, this);
    lv_obj_add_event_cb(m_canvas_img, on_canvas_move, LV_EVENT_PRESSING, this);
    lv_obj_add_event_cb(m_canvas_img, on_canvas_release, LV_EVENT_RELEASED, this);
}

void WatchUI::createNavBar()
{
    int y = SCREEN_HEIGHT - CONTENT_INSET - NAV_BTN_SIZE - 2;
    int cx = SCREEN_WIDTH / 2;
    int gap = NAV_BTN_SIZE + 16;

    auto make_btn = [&](lv_obj_t** btn, int x, const char* icon, lv_event_cb_t cb) {
        *btn = lv_button_create(m_bg);
        lv_obj_set_size(*btn, NAV_BTN_SIZE, NAV_BTN_SIZE);
        lv_obj_set_pos(*btn, x - NAV_BTN_SIZE / 2, y);
        lv_obj_set_style_radius(*btn, NAV_BTN_SIZE / 2, 0);
        lv_obj_set_style_bg_color(*btn, lv_color_make(50, 50, 50), 0);
        lv_obj_set_style_bg_color(*btn, lv_color_make(80, 80, 80), LV_STATE_PRESSED);
        lv_obj_t* lbl = lv_label_create(*btn);
        lv_label_set_text(lbl, icon);
        lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(*btn, cb, LV_EVENT_CLICKED, this);
    };

    make_btn(&m_nav_back, cx - gap, LV_SYMBOL_LEFT, on_back);
    make_btn(&m_nav_reload, cx, LV_SYMBOL_REFRESH, on_refresh);
    make_btn(&m_nav_fwd, cx + gap, LV_SYMBOL_RIGHT, on_fwd);
}

/* --- Public update methods --- */

void WatchUI::updateCanvas()
{
    /* Blit visible region from tile buffer into canvas buffer */
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

void WatchUI::updateUrl(const char* url)
{
    lv_textarea_set_text(m_addr, url ? url : "");
}

/* --- Event handlers --- */

void WatchUI::on_addr_ready(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    const char* text = lv_textarea_get_text(ui->m_addr);
    if (text && text[0])
        ui->m_wv->loadUrl(text);
}

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

    if (!ui->m_dragging && (abs(dx) > 5 || abs(dy) > 5))
        ui->m_dragging = true;

    if (ui->m_dragging) {
        ui->m_wv->scrollBy(dx, dy);
        ui->m_tx = p.x;
        ui->m_ty = p.y;
        ui->updateCanvas();
    }
}

void WatchUI::on_canvas_release(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    if (!ui->m_dragging) {
        /* Tap - forward as click to engine */
        lv_point_t p;
        lv_indev_get_point(lv_indev_active(), &p);
        int lx = p.x - CONTENT_INSET;
        int ly = p.y - CONTENT_TOP;
        ui->m_wv->mousePress(lx, ly);
        ui->m_wv->mouseRelease(lx, ly);
    }
    ui->m_dragging = false;
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
