/* ui.h - LVGL circular watch browser UI
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_UI_H_
#define _WATCHWEB_UI_H_

#include "config.h"
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
    void updateUrl(const char* url);

private:
    void createBackground();
    void createAddressBar();
    void createCanvas();
    void createNavBar();
    void createProgress();

    static void on_addr_ready(lv_event_t* e);
    static void on_canvas_press(lv_event_t* e);
    static void on_canvas_move(lv_event_t* e);
    static void on_canvas_release(lv_event_t* e);
    static void on_back(lv_event_t* e);
    static void on_fwd(lv_event_t* e);
    static void on_refresh(lv_event_t* e);

    lv_obj_t* m_bg;
    lv_obj_t* m_addr;
    lv_obj_t* m_canvas_img;
    lv_obj_t* m_progress;
    lv_obj_t* m_nav_back;
    lv_obj_t* m_nav_fwd;
    lv_obj_t* m_nav_reload;
    lv_image_dsc_t m_img_dsc;
    WebView* m_wv;
    int m_tx, m_ty;
    bool m_dragging;
};

#endif
