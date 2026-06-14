/* ui.cpp - LVGL full-screen watch browser UI
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#include <string.h>
#include <stdlib.h>
#include "lvgl.h"
#include "ui.h"
#include "webview.h"

WatchUI::WatchUI()
    : m_bg(nullptr), m_canvas_img(nullptr), m_canvas_buf(nullptr)
    , m_progress(nullptr)
    , m_was_loading(false), m_finishing(false)
    , m_tool_layer(nullptr), m_addr_layer(nullptr)
    , m_addr_ta(nullptr), m_addr_kb(nullptr), m_fab(nullptr), m_wv(nullptr)
    , m_tx(0), m_ty(0), m_dragging(false)
{
}

WatchUI::~WatchUI()
{
    if (m_canvas_buf) { free(m_canvas_buf); m_canvas_buf = nullptr; }
}

void WatchUI::create(WebView* wv)
{
    m_wv = wv;
    createBackground();
    createCanvas();
    createProgress();
    createToolLayer();
    createAddrLayer();
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

    m_canvas_buf = (uint8_t*)malloc(CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    if (!m_canvas_buf) { return; }
    memset(m_canvas_buf, 0xFF, CONTENT_WIDTH * CONTENT_HEIGHT * 4);
    lv_canvas_set_buffer(m_canvas_img, m_canvas_buf, CONTENT_WIDTH, CONTENT_HEIGHT, LV_COLOR_FORMAT_ARGB8888);

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

/* Helper: make a round icon button on a parent at (x,y). */
static lv_obj_t* make_icon_btn(lv_obj_t* parent, int x, int y, int size,
                               const char* icon, lv_color_t bg,
                               lv_event_cb_t cb, void* ud)
{
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_size(btn, size, size);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_style_radius(btn, size / 2, 0);
    lv_obj_set_style_bg_color(btn, bg, 0);
    lv_obj_set_style_bg_color(btn, lv_color_lighten(bg, 40), LV_STATE_PRESSED);
    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, icon);
    lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, ud);
    return btn;
}

void WatchUI::createToolLayer()
{
    /* Full-screen translucent overlay holding the navigation buttons. The
       buttons sit in a centered 2x3 grid so they stay inside the round
       display's inscribed safe area. Hidden by default. */
    m_tool_layer = lv_obj_create(m_bg);
    lv_obj_set_size(m_tool_layer, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(m_tool_layer, 0, 0);
    lv_obj_set_style_radius(m_tool_layer, 0, 0);
    lv_obj_set_style_bg_color(m_tool_layer, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(m_tool_layer, LV_OPA_70, 0);
    lv_obj_set_style_border_width(m_tool_layer, 0, 0);
    lv_obj_set_style_pad_all(m_tool_layer, 0, 0);
    lv_obj_clear_flag(m_tool_layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(m_tool_layer, LV_OBJ_FLAG_HIDDEN);

    int gw = GRID_COLS * NAV_BTN_SIZE + (GRID_COLS - 1) * GRID_HGAP;
    int gh = GRID_ROWS * NAV_BTN_SIZE + (GRID_ROWS - 1) * GRID_VGAP;
    int x0 = (SCREEN_WIDTH - gw) / 2;
    int y0 = (SCREEN_HEIGHT - gh) / 2;
    int dx = NAV_BTN_SIZE + GRID_HGAP;
    int dy = NAV_BTN_SIZE + GRID_VGAP;

    lv_color_t nav = lv_color_make(60, 60, 60);
    lv_color_t act = lv_color_make(0, 110, 200);
    lv_color_t red = lv_color_make(150, 50, 50);

    /* Row 0: back, forward, refresh */
    make_icon_btn(m_tool_layer, x0, y0, NAV_BTN_SIZE, LV_SYMBOL_LEFT, nav, on_back, this);
    make_icon_btn(m_tool_layer, x0 + dx, y0, NAV_BTN_SIZE, LV_SYMBOL_RIGHT, nav, on_fwd, this);
    make_icon_btn(m_tool_layer, x0 + 2 * dx, y0, NAV_BTN_SIZE, LV_SYMBOL_REFRESH, nav, on_refresh, this);
    /* Row 1: URL (globe), settings, close */
    make_icon_btn(m_tool_layer, x0, y0 + dy, NAV_BTN_SIZE, LV_SYMBOL_GPS, act, on_open_addr, this);
    make_icon_btn(m_tool_layer, x0 + dx, y0 + dy, NAV_BTN_SIZE, LV_SYMBOL_SETTINGS, nav, on_settings, this);
    make_icon_btn(m_tool_layer, x0 + 2 * dx, y0 + dy, NAV_BTN_SIZE, LV_SYMBOL_CLOSE, red, on_close_tool, this);
}

void WatchUI::createAddrLayer()
{
    /* Full-screen translucent overlay for URL input. Hidden by default. */
    m_addr_layer = lv_obj_create(m_bg);
    lv_obj_set_size(m_addr_layer, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(m_addr_layer, 0, 0);
    lv_obj_set_style_radius(m_addr_layer, 0, 0);
    lv_obj_set_style_bg_color(m_addr_layer, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(m_addr_layer, LV_OPA_80, 0);
    lv_obj_set_style_border_width(m_addr_layer, 0, 0);
    lv_obj_set_style_pad_all(m_addr_layer, 0, 0);
    lv_obj_clear_flag(m_addr_layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(m_addr_layer, LV_OBJ_FLAG_HIDDEN);

    /* URL text area near the top (above the keyboard). */
    m_addr_ta = lv_textarea_create(m_addr_layer);
    lv_textarea_set_one_line(m_addr_ta, true);
    lv_textarea_set_placeholder_text(m_addr_ta, "Enter URL");
    lv_obj_set_size(m_addr_ta, SCREEN_WIDTH - 80, 44);
    lv_obj_set_pos(m_addr_ta, 40, 40);
    lv_obj_set_style_text_font(m_addr_ta, &lv_font_montserrat_14, 0);

    /* Go / Cancel buttons below the text area. */
    int bw = 110, bh = 44, by = 96;
    lv_obj_t* go = lv_button_create(m_addr_layer);
    lv_obj_set_size(go, bw, bh);
    lv_obj_set_pos(go, SCREEN_WIDTH / 2 - bw - 8, by);
    lv_obj_set_style_bg_color(go, lv_color_make(0, 110, 200), 0);
    lv_obj_t* gl = lv_label_create(go);
    lv_label_set_text(gl, LV_SYMBOL_OK " Go");
    lv_obj_center(gl);
    lv_obj_add_event_cb(go, on_addr_go, LV_EVENT_CLICKED, this);

    lv_obj_t* cancel = lv_button_create(m_addr_layer);
    lv_obj_set_size(cancel, bw, bh);
    lv_obj_set_pos(cancel, SCREEN_WIDTH / 2 + 8, by);
    lv_obj_set_style_bg_color(cancel, lv_color_make(80, 80, 80), 0);
    lv_obj_t* cl = lv_label_create(cancel);
    lv_label_set_text(cl, LV_SYMBOL_CLOSE " Cancel");
    lv_obj_center(cl);
    lv_obj_add_event_cb(cancel, on_addr_cancel, LV_EVENT_CLICKED, this);

    /* On-screen keyboard occupying the lower half. */
    m_addr_kb = lv_keyboard_create(m_addr_layer);
    lv_obj_set_size(m_addr_kb, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
    lv_obj_align(m_addr_kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_textarea(m_addr_kb, m_addr_ta);
}

void WatchUI::createFab()
{
    /* Small floating toggle at the bottom-center; tapping opens the tool layer. */
    m_fab = lv_button_create(m_bg);
    lv_obj_set_size(m_fab, FAB_SIZE, FAB_SIZE);
    lv_obj_set_pos(m_fab, (SCREEN_WIDTH - FAB_SIZE) / 2,
                   SCREEN_HEIGHT - FAB_SIZE - FAB_MARGIN);
    lv_obj_set_style_radius(m_fab, FAB_SIZE / 2, 0);
    lv_obj_set_style_bg_color(m_fab, lv_color_make(0, 150, 255), 0);
    lv_obj_set_style_bg_opa(m_fab, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(m_fab, lv_color_make(0, 110, 200), LV_STATE_PRESSED);
    lv_obj_t* lbl = lv_label_create(m_fab);
    lv_label_set_text(lbl, LV_SYMBOL_BARS);
    lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
    lv_obj_center(lbl);
    lv_obj_add_event_cb(m_fab, on_fab, LV_EVENT_CLICKED, this);
}

void WatchUI::showToolLayer(bool show)
{
    if (show) {
        lv_obj_clear_flag(m_tool_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(m_fab, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(m_tool_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(m_fab, LV_OBJ_FLAG_HIDDEN);
    }
}

void WatchUI::showAddrLayer(bool show)
{
    if (show) {
        lv_obj_add_flag(m_tool_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(m_addr_layer, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(m_addr_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(m_fab, LV_OBJ_FLAG_HIDDEN);
    }
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
        /* Active load: cancel any pending finish animation and track real
           progress. Cap at 95 so the arc keeps a little headroom to glide to
           99% on finish (avoids an abrupt jump). */
        if (m_finishing) {
            lv_anim_delete(m_progress, progress_anim_exec);
            m_finishing = false;
        }
        lv_obj_clear_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
        unsigned int v = pct > 95 ? 95 : pct;
        lv_arc_set_value(m_progress, v);
    } else if (m_was_loading && !m_finishing) {
        /* Just finished (main frame laid out / page visible): smoothly glide
           the arc from its current value to 99%, then hide it. This reads as a
           natural "almost done" finish instead of vanishing mid-way. */
        m_finishing = true;
        lv_obj_clear_flag(m_progress, LV_OBJ_FLAG_HIDDEN);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, m_progress);
        lv_anim_set_exec_cb(&a, progress_anim_exec);
        lv_anim_set_completed_cb(&a, progress_anim_done);
        lv_anim_set_values(&a, lv_arc_get_value(m_progress), 99);
        lv_anim_set_duration(&a, 350);
        lv_anim_start(&a);
    }
    m_was_loading = loading;
}

void WatchUI::progress_anim_exec(void* obj, int32_t v)
{
    lv_arc_set_value((lv_obj_t*)obj, v);
}

void WatchUI::progress_anim_done(lv_anim_t* a)
{
    lv_obj_add_flag((lv_obj_t*)a->var, LV_OBJ_FLAG_HIDDEN);
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
    ui->showToolLayer(true);
}

void WatchUI::on_back(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->goBack();
    ui->showToolLayer(false);
}

void WatchUI::on_fwd(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->goForward();
    ui->showToolLayer(false);
}

void WatchUI::on_refresh(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->m_wv->reload();
    ui->showToolLayer(false);
}

void WatchUI::on_settings(lv_event_t* e)
{
    /* Settings page not implemented yet; just close the tool layer for now. */
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->showToolLayer(false);
}

void WatchUI::on_close_tool(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->showToolLayer(false);
}

void WatchUI::on_open_addr(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    const char* cur = ui->m_wv->url();
    lv_textarea_set_text(ui->m_addr_ta, cur ? cur : "");
    ui->showAddrLayer(true);
}

void WatchUI::on_addr_go(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    const char* url = lv_textarea_get_text(ui->m_addr_ta);
    if (url && url[0]) { ui->m_wv->loadUrl(url); }
    ui->showAddrLayer(false);
}

void WatchUI::on_addr_cancel(lv_event_t* e)
{
    WatchUI* ui = (WatchUI*)lv_event_get_user_data(e);
    ui->showAddrLayer(false);
}
