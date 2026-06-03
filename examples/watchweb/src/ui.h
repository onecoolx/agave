/* ui.h - LVGL full-screen watch browser UI
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

private:
    void createBackground();
    void createCanvas();
    void createProgress();
    void createNavBar();
    void createFab();
    void setNavVisible(bool visible);

    static void on_canvas_press(lv_event_t* e);
    static void on_canvas_move(lv_event_t* e);
    static void on_canvas_release(lv_event_t* e);
    static void on_fab(lv_event_t* e);
    static void on_back(lv_event_t* e);
    static void on_fwd(lv_event_t* e);
    static void on_refresh(lv_event_t* e);

    lv_obj_t* m_bg;
    lv_obj_t* m_canvas_img;
    lv_obj_t* m_progress;
    lv_obj_t* m_nav_bar; /* bottom pop-up navigation panel */
    lv_obj_t* m_fab; /* floating toggle icon */
    WebView* m_wv;
    int m_tx, m_ty;
    bool m_dragging;
    bool m_nav_visible;
};

#endif
