/* ui.h - LVGL full-screen watch browser UI
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_UI_H_
#define _WATCHWEB_UI_H_

#include "config.h"
#include <stdint.h>
#include "lvgl.h"

class WebView;

class WatchUI
{
public:
    WatchUI();
    ~WatchUI();

    void create(WebView* wv);
    void updateCanvas();
    void updateProgress(unsigned int pct, bool loading);

private:
    void createBackground();
    void createCanvas();
    void createProgress();
    void createPageKeyboard();
    void showPageKeyboard(bool show);
    void createToolLayer();
    static void progress_anim_exec(void* obj, int32_t v);
    static void progress_anim_done(lv_anim_t* a);
    static void on_page_kb(lv_event_t* e);
    static void on_ime(void* ud, bool enable);
    void createAddrLayer();
    void createFab();
    void showToolLayer(bool show);
    void showAddrLayer(bool show);

    static void on_canvas_press(lv_event_t* e);
    static void on_canvas_move(lv_event_t* e);
    static void on_canvas_release(lv_event_t* e);
    static void on_fab(lv_event_t* e);
    static void on_back(lv_event_t* e);
    static void on_fwd(lv_event_t* e);
    static void on_refresh(lv_event_t* e);
    static void on_settings(lv_event_t* e);
    static void on_close_tool(lv_event_t* e);
    static void on_open_addr(lv_event_t* e);
    static void on_addr_go(lv_event_t* e);
    static void on_addr_cancel(lv_event_t* e);

    lv_obj_t* m_bg;
    lv_obj_t* m_canvas_img;
    uint8_t* m_canvas_buf;
    lv_obj_t* m_progress;
    bool m_was_loading; /* previous loading state, to detect finish edge */
    bool m_finishing; /* progress arc is animating to 99% then hiding */
    lv_obj_t* m_tool_layer; /* full-screen translucent toolbar overlay */
    lv_obj_t* m_addr_layer; /* full-screen translucent URL-input overlay */
    lv_obj_t* m_addr_ta; /* URL text area */
    lv_obj_t* m_addr_kb; /* on-screen keyboard */
    lv_obj_t* m_page_kb; /* on-screen keyboard for page input fields */
    lv_obj_t* m_fab; /* floating toggle icon */
    WebView* m_wv;
    int m_tx, m_ty;
    bool m_dragging;
};

#endif
