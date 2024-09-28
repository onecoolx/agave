/* toolbar.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <picasso/picasso.h>

#include "toolbar.h"
#include "tabpage.h"
#include "webview.h"
#include "mainwindow.h"
#include "ctrlpanel.h"
#include "pagemgr.h"
#include "application.h"

class ToolBarImpl
{
public:
    ToolBarImpl()
        : g_back(0)
        , g_event(0)
        , t_event(0)
    {
        t_event = ps_matrix_create();
    }

    ~ToolBarImpl()
    {
        if (g_back) {
            ps_gradient_unref(g_back);
        }

        if (g_event) {
            ps_gradient_unref(g_event);
        }

        if (t_event) {
            ps_matrix_unref(t_event);
        }
    }
    ps_gradient* g_back;
    ps_gradient* g_event;
    ps_matrix* t_event;
};

ToolBar::ToolBar(Widget* parent)
    : Widget(parent)
    , m_impl(new ToolBarImpl)
    , m_backBtn(0, 0, 0, 0)
    , m_fordBtn(0, 0, 0, 0)
    , m_zoomBtn(0, 0, 0, 0)
    , m_tabsBtn(0, 0, 0, 0)
    , m_configBtn(0, 0, 0, 0)
    , m_eventRect(0, 0, 0, 0)
    , m_main(0)
    , m_btn(0)
{
}

ToolBar::~ToolBar()
{
    delete m_impl;
}

void ToolBar::setMainWindow(MainWindow* main)
{
    m_main = main;
}

static void draw_back_btn(ps_context* gc, const ps_rect& rt)
{
    int border = TOOLBAR_HEIGHT / 15;
    ps_rect r = {rt.x, rt.y + border, rt.w, rt.h - border * 2};
    ps_point p1 = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3, r.y + r.h / 2};
    ps_point p2 = {r.x + r.w / 2, r.y + border};
    ps_point p3 = {r.x + r.w / 2, r.y + r.h - border};
    ps_rect rp = {r.x + r.w / 2, r.y + r.h / 2 - TOOLBAR_HEIGHT / 6, TOOLBAR_HEIGHT / 3, TOOLBAR_HEIGHT / 3};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_line_to(gc, &p3);
    ps_line_to(gc, &p1);
    ps_rectangle(gc, &rp);
    ps_fill(gc);
}

static void draw_forward_btn(ps_context* gc, const ps_rect& rt)
{
    int border = TOOLBAR_HEIGHT / 15;
    ps_rect r = {rt.x, rt.y + border, rt.w, rt.h - border * 2};
    ps_point p1 = {r.x + r.w - r.w / 2 + TOOLBAR_HEIGHT / 3, r.y + r.h / 2};
    ps_point p2 = {r.x + r.w - r.w / 2, r.y + border};
    ps_point p3 = {r.x + r.w - r.w / 2, r.y + r.h - border};
    ps_rect rp = {r.x + r.w - r.w / 2 - TOOLBAR_HEIGHT / 3, r.y + r.h / 2 - TOOLBAR_HEIGHT / 6, TOOLBAR_HEIGHT / 3, TOOLBAR_HEIGHT / 3};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_line_to(gc, &p3);
    ps_line_to(gc, &p1);
    ps_rectangle(gc, &rp);
    ps_fill(gc);
}

static void draw_zoom_btn(ps_context* gc, const ps_rect& rt, MainWindow* main)
{
    int border = TOOLBAR_HEIGHT / 10;
    int b = TOOLBAR_HEIGHT / 30;
    ps_color c = {1, 1, 1, 1};
    ps_set_stroke_color(gc, &c);
    ps_rect xr = {rt.x + rt.w / 2 - TOOLBAR_HEIGHT / 5 * 2, rt.y + border, rt.h - border * 2, rt.h - border * 2};

    int br = TOOLBAR_HEIGHT / 15;
    ps_set_line_cap(gc, LINE_CAP_ROUND);
    ps_set_line_width(gc, br);

    ps_rect tr = {xr.x + br, xr.y + br, xr.w / 3 * 2, xr.h / 3 * 2};
    ps_ellipse(gc, &tr);
    ps_stroke(gc);

    ps_point p1 = {tr.x + tr.w / 2 + 0.707 * tr.w / 2, tr.y + tr.h / 2 + 0.707 * tr.h / 2};
    ps_point p2 = {xr.x + xr.w - br, xr.y + xr.h - border};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_stroke(gc);

    //draw status
    ps_set_line_width(gc, TOOLBAR_HEIGHT / 30);
    p1.x = tr.x + 5 * b;
    p1.y = tr.y + tr.h / 2;
    p2.x = tr.x + tr.w - 5 * b;
    p2.y = tr.y + tr.h / 2;
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);

    WebView* view = main->getTabs()->getActiveView();
    if (view && view->zoomFactor() == 1.0f) {
        p1.x = tr.x + tr.w / 2;
        p1.y = tr.y + 5 * b;
        p2.x = tr.x + tr.w / 2;
        p2.y = tr.y + tr.h - 5 * b;
        ps_move_to(gc, &p1);
        ps_line_to(gc, &p2);
    }
    ps_stroke(gc);
}

static void draw_tabs_btn(ps_context* gc, const ps_rect& rt)
{
    int border = TOOLBAR_HEIGHT / 10;
    int w = TOOLBAR_HEIGHT / 30;
    int border2 = TOOLBAR_HEIGHT / 15;
    ps_rect r = {rt.x, rt.y + border2, rt.w, rt.h - border2 * 2};
    ps_set_line_width(gc, w);
    ps_rect ry = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3, r.y + border, TOOLBAR_HEIGHT / 2, TOOLBAR_HEIGHT / 2};
    ps_rect rn = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 6, r.y + border * 2.7, TOOLBAR_HEIGHT / 2, TOOLBAR_HEIGHT / 2};
    ps_color c = {0.95, 0.95, 0.95, 1};
    ps_color c2 = {0.1, 0.1, 0.1, 1};
    ps_set_stroke_color(gc, &c);
    ps_rectangle(gc, &ry);
    ps_stroke(gc);
    ps_rectangle(gc, &rn);
    ps_fill(gc);

    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    char num[4] = {0};
    snprintf(num, 3, "%d", Application::getInstance()->getMainWindow()->getTabs()->getCount());
    ps_size s = {0};
    ps_get_text_extent(gc, num, strlen(num), &s);
    ps_text_out_length(gc, rn.x + rn.w / 2 - s.w / 2, rn.y + w, num, strlen(num));

    ps_set_line_width(gc, 1);

    ps_set_font(gc, of);
}

static void draw_config_btn(ps_context* gc, const ps_rect& rt)
{
    int border = TOOLBAR_HEIGHT / 10;
    int border2 = TOOLBAR_HEIGHT / 15;
    ps_rect r = {rt.x, rt.y + border2, rt.w, rt.h - border2 * 2};
    ps_rect ry = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3, r.y + border, TOOLBAR_HEIGHT / 5 * 2, TOOLBAR_HEIGHT / 5 * 3};
    ps_rect rn = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3, r.y + TOOLBAR_HEIGHT / 5 * 3, TOOLBAR_HEIGHT / 3 * 2, TOOLBAR_HEIGHT / 6};
    ps_color c = {0.95, 0.95, 0.95, 1};
    ps_set_stroke_color(gc, &c);
    ps_rectangle(gc, &ry);

    ps_point p1 = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3 + border, r.y + TOOLBAR_HEIGHT / 3 - border};
    ps_point p2 = {r.x + r.w / 2, r.y + TOOLBAR_HEIGHT / 3 - border};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);

    ps_point p3 = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3 + border, r.y + TOOLBAR_HEIGHT / 3};
    ps_point p4 = {r.x + r.w / 2, r.y + TOOLBAR_HEIGHT / 3};
    ps_move_to(gc, &p3);
    ps_line_to(gc, &p4);

    ps_point p5 = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 3 + border, r.y + TOOLBAR_HEIGHT / 3 + border};
    ps_point p6 = {r.x + r.w / 2, r.y + TOOLBAR_HEIGHT / 3 + border};
    ps_move_to(gc, &p5);
    ps_line_to(gc, &p6);

    ps_set_line_width(gc, TOOLBAR_HEIGHT / 30);
    ps_stroke(gc);
    ps_set_line_width(gc, 1);
    ps_rectangle(gc, &rn);
    ps_fill(gc);
}

void ToolBar::draw_buttons(ps_context* gc, const Rect& rc)
{
    ps_color c = {0.95, 0.95, 0.95, 1};
    ps_set_source_color(gc, &c);

    Rect r;
    //backword button
    r = m_backBtn;
    if (r.intersect(rc)) {
        draw_back_btn(gc, m_backBtn);
    }

    //forward button
    r = m_fordBtn;
    if (r.intersect(rc)) {
        draw_forward_btn(gc, m_fordBtn);
    }

    //zoom button
    r = m_zoomBtn;
    if (r.intersect(rc)) {
        draw_zoom_btn(gc, m_zoomBtn, m_main);
    }

    r = m_tabsBtn;
    if (r.intersect(rc)) {
        draw_tabs_btn(gc, m_tabsBtn);
    }

    r = m_configBtn;
    if (r.intersect(rc)) {
        draw_config_btn(gc, m_configBtn);
    }
}

//Event Handler
void ToolBar::OnPaint(ps_context* gc, const Rect* r)
{
    Widget::OnPaint(gc, r);
    //TODO: add paint code here.
    ps_save(gc);

    ps_rect rb = {0, 0, width(), height()};
    ps_rectangle(gc, &rb);
    ps_set_source_gradient(gc, m_impl->g_back);
    ps_fill(gc);

    draw_event(gc);

    draw_buttons(gc, (*r));

    ps_restore(gc);
}

void ToolBar::draw_event(ps_context* gc)
{
    if (!m_eventRect.isEmpty()) {
        int rds = TOOLBAR_HEIGHT / 10;
        int b = TOOLBAR_HEIGHT / 15;
        ps_set_line_width(gc, TOOLBAR_HEIGHT / 30);
        ps_rect r = {m_eventRect.x + TOOLBAR_HEIGHT / 6, m_eventRect.y + b, m_eventRect.w - TOOLBAR_HEIGHT / 3, m_eventRect.h - b * 2};
        ps_color c = {1, 1, 1, 1};
        ps_matrix_translate(m_impl->t_event, r.x, r.y);
        ps_gradient_transform(m_impl->g_event, m_impl->t_event);
        ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_set_source_gradient(gc, m_impl->g_event);
        ps_set_stroke_color(gc, &c);
        ps_paint(gc);
        ps_matrix_translate(m_impl->t_event, -r.x * 2, -r.y * 2);
        ps_gradient_transform(m_impl->g_event, m_impl->t_event);
        ps_matrix_identity(m_impl->t_event);
        ps_set_line_width(gc, 1);
    }
}

void ToolBar::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(f, x, y, w, h);
    // init button rects.

    int box_width = (w - TOOLBAR_HEIGHT / 5 * 2) / 5;
    int box_height = h ;
    int cur_loc = TOOLBAR_HEIGHT / 10;
    int border = TOOLBAR_HEIGHT / 15;

    m_backBtn.x = cur_loc;
    m_backBtn.y = 0;
    m_backBtn.w = box_width;
    m_backBtn.h = box_height;
    cur_loc += (box_width + border);

    m_fordBtn.x = cur_loc;
    m_fordBtn.y = 0;
    m_fordBtn.w = box_width;
    m_fordBtn.h = box_height;
    cur_loc += (box_width + border);

    m_zoomBtn.x = cur_loc;
    m_zoomBtn.y = 0;
    m_zoomBtn.w = box_width;
    m_zoomBtn.h = box_height;
    cur_loc += (box_width + border);

    m_tabsBtn.x = cur_loc;
    m_tabsBtn.y = 0;
    m_tabsBtn.w = box_width;
    m_tabsBtn.h = box_height;
    cur_loc += (box_width + border);

    m_configBtn.x = cur_loc;
    m_configBtn.y = 0;
    m_configBtn.w = box_width;
    m_configBtn.h = box_height;
    cur_loc += (box_width + border);

    //create draw object
    ps_color c1 = {0.45, 0.45, 0.45, 1};
    ps_color c2 = {0, 0, 0, 1};
    ps_point s = {0, 0};
    ps_point e = {0, height()};
    m_impl->g_back = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
    ps_gradient_add_color_stop(m_impl->g_back, 0, &c1);
    ps_gradient_add_color_stop(m_impl->g_back, 1, &c2);

    ps_color c3 = {1, 1, 1, 1};
    ps_color c4 = {0.08, 0.60, 0.08, 1};
    ps_color c5 = {0.05, 0.05, 0.05, 1};
    ps_point s1 = {0, 0};
    ps_point e1 = {box_width, box_height};
    m_impl->g_event = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(m_impl->g_event, 0, &c3);
    ps_gradient_add_color_stop(m_impl->g_event, 0.5, &c4);
    ps_gradient_add_color_stop(m_impl->g_event, 1, &c5);
}

void ToolBar::OnDestroy(void)
{
    Widget::OnDestroy();
}

void ToolBar::notifyButtonUpdate(int btnid)
{
    switch (btnid) {
        case BackBtn:
            Update(&m_backBtn);
            break;
        case FordBtn:
            Update(&m_fordBtn);
            break;
        case ZoomBtn:
            Update(&m_zoomBtn);
            break;
        case PageBtn:
            Update(&m_tabsBtn);
            break;
        case ConfBtn:
            Update(&m_configBtn);
            break;
    }
}

void ToolBar::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_backBtn.contains(e->x(), e->y())) {
            m_eventRect = m_backBtn;
            m_btn = BackBtn;
        } else if (m_fordBtn.contains(e->x(), e->y())) {
            m_eventRect = m_fordBtn;
            m_btn = FordBtn;
        } else if (m_zoomBtn.contains(e->x(), e->y())) {
            m_eventRect = m_zoomBtn;
            m_btn = ZoomBtn;
        } else if (m_tabsBtn.contains(e->x(), e->y())) {
            m_eventRect = m_tabsBtn;
            m_btn = PageBtn;
        } else if (m_configBtn.contains(e->x(), e->y())) {
            m_eventRect = m_configBtn;
            m_btn = ConfBtn;
        } else {
            m_eventRect = Rect(0, 0, 0, 0);
            m_btn = 0;
        }

        if (!m_eventRect.isEmpty()) {
            Update(&m_eventRect);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (!m_eventRect.isEmpty()) {
            Update(&m_eventRect);
            m_eventRect = Rect(0, 0, 0, 0);
            Widget::postEvent(this, EVENT_FUNC(ToolBar, button_event), EVENT_PARAM(m_btn), true);
            m_btn = 0;
        }
    }
}

void ToolBar::button_event(void* param)
{
    WebView* view = m_main->getTabs()->getActiveView();

    int btn = (intptr_t)param;
    if (view) {
        switch (btn) {
            case BackBtn:
                view->goBack();
                break;
            case FordBtn:
                view->goForward();
                break;
            case ZoomBtn: {
                    if (view->zoomFactor() == 1.0f) {
                        view->setZoomFactor(2.0f);
                    } else {
                        view->setZoomFactor(1.0f);
                    }

                    Update(&m_zoomBtn);
                }
                break;
            case PageBtn: {
                    CtrlPanel* pan = m_main->getCtrlPanel();
                    if (pan->isVisible()) {
                        pan->Hide();
                    }

                    PageManager* pmgr = m_main->getPageManager();
                    if (pmgr->isVisible()) {
                        pmgr->Hide();
                    } else {
                        pmgr->Show();
                    }
                }
                break;
            case ConfBtn: {
                    PageManager* pmgr = m_main->getPageManager();
                    if (pmgr->isVisible()) {
                        pmgr->Hide();
                    }

                    CtrlPanel* pan = m_main->getCtrlPanel();
                    if (pan->isVisible()) {
                        pan->Hide();
                    } else {
                        pan->Show();
                    }
                }
                break;
        }
    }
}

class ImToolBarImpl
{
public:
    ImToolBarImpl()
        : g_back(0)
    {
    }

    ~ImToolBarImpl()
    {
        if (g_back) {
            ps_gradient_unref(g_back);
        }
    }

    ps_gradient* g_back;
};

ImToolBar::ImToolBar(Widget* parent)
    : Widget(parent)
    , m_impl(new ImToolBarImpl)
    , m_imbtn(0, 0, 0, 0)
    , m_main(0)
    , m_btn(0)
{
}

ImToolBar::~ImToolBar()
{
    delete m_impl;
}

void ImToolBar::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void ImToolBar::OnPaint(ps_context* gc, const Rect* r)
{
    Widget::OnPaint(gc, r);

    ps_save(gc);

    ps_rect rb = {0, 0, width(), height()};
    ps_rectangle(gc, &rb);
    ps_set_source_gradient(gc, m_impl->g_back);
    ps_fill(gc);

    int b = DASH_TITLE_HEIGHT / 30;
    int rds = DASH_TITLE_HEIGHT / 10;
    ps_color c = {0.5, 0.5, 0.5, 1};
    ps_set_stroke_color(gc, &c);

    Rect rt = *r;
    if (!m_imbtn.isEmpty() && rt.intersect(m_imbtn)) {
        ps_rect rr = m_imbtn;
        ps_rounded_rect(gc, &rr, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_set_line_width(gc, b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 1) {
            ps_rect r2 = {rr.x + b, rr.y + b, rr.w - b, rr.h - b};
            ps_color c1 = {0, 0, 0, 1};
            ps_color c2 = {1, 0, 0, 1};
            ps_rounded_rect(gc, &r2, rds, rds, rds, rds, rds, rds, rds, rds);
            ps_point s = {rr.x, rr.y};
            ps_point e = {rr.x, rr.y + rr.h};
            ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
            ps_gradient_add_color_stop(g, 0, &c1);
            ps_gradient_add_color_stop(g, 1, &c2);
            ps_set_source_gradient(gc, g);
            ps_fill(gc);
            ps_gradient_unref(g);
        }

        draw_btnText(gc, U("Spell^"), m_imbtn);
        draw_lights(gc, m_imbtn);
    }
    ps_restore(gc);
}

void ImToolBar::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{
    if (!text.empty()) {
        //draw title
        ps_color c = {1, 1, 1, 1};
        ps_set_text_color(gc, &c);
        ps_font* f = MainWindow::getUIFont();

        ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
        ps_set_text_antialias(gc, False);
#else
        if (MainWindow::uiFontAntialias()) {
            ps_set_text_antialias(gc, True);
        } else {
            ps_set_text_antialias(gc, False);
        }
#endif

        ps_size sz = {0};
        ps_get_text_extent(gc, text.c_str(), text.length(), &sz);

        ps_wide_text_out_length(gc, r.x + (r.w - sz.w) / 2, r.y + r.h / 2 - DASH_TITLE_HEIGHT / 5,
                                (ps_uchar16*)text.c_str(), text.length());

        ps_set_font(gc, of);
    }
}

void ImToolBar::draw_lights(ps_context* gc, const Rect& r)
{
    int b = DASH_TITLE_HEIGHT / 30;
    int rds = DASH_TITLE_HEIGHT / 10;
    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_rect lrc = {r.x + 2 * b, r.y + 2 * b, r.w - 3 * b, r.h / 2 - 2 * b};
    ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
    ps_point s1 = {lrc.x, lrc.y};
    ps_point e1 = {lrc.x, lrc.y + lrc.h};
    ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(gt, 0, &tc);
    ps_gradient_add_color_stop(gt, 1, &tc2);
    ps_set_source_gradient(gc, gt);
    ps_fill(gc);
    ps_gradient_unref(gt);
}

void ImToolBar::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(f, x, y, w, h);

    int b = DASH_TITLE_HEIGHT / 30;
    int button_width = width() / 5;
    int button_height = height() - 4 * b;
    m_imbtn = Rect(5 * b, 2 * b, button_width, button_height);

    //create draw object
    ps_color c1 = {0.45, 0.45, 0.45, 1};
    ps_color c2 = {0, 0, 0, 1};
    ps_point s = {0, 0};
    ps_point e = {0, height()};
    m_impl->g_back = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
    ps_gradient_add_color_stop(m_impl->g_back, 0, &c1);
    ps_gradient_add_color_stop(m_impl->g_back, 1, &c2);
}

void ImToolBar::OnDestroy(void)
{
    Widget::OnDestroy();
}

void ImToolBar::button_event(void* param)
{
    SetImeStatus(false);
    Hide();
}

void ImToolBar::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_imbtn.contains(e->x(), e->y())) {
            m_btn = 1;
        } else {
            m_btn = 0;
        }

        if (m_btn) {
            Update(&m_imbtn);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {
            Update(&m_imbtn);

            Widget::postEvent(this, EVENT_FUNC(ImToolBar, button_event), EVENT_PARAM(m_btn), true);
            m_btn = 0;
        }
    }
}
