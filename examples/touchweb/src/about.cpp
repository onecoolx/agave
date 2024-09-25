/* about.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <string.h>
#include "config.h"
#include "about.h"
#include "webview.h"
#include "tabpage.h"
#include "mainwindow.h"
#include "application.h"

#if defined(M8)
    #define APP_NAME U("MaCross Mobile (M8)")
    #define APP_NAME_LEN 19
#elif defined(WINCE)
    #define APP_NAME U("MaCross Mobile (WinCE)")
    #define APP_NAME_LEN 22
#elif defined(__linux)
    #define APP_NAME U("MaCross Mobile (Linux)")
    #define APP_NAME_LEN 22
#elif defined(WIN32)
    #define APP_NAME U("MaCross Mobile (WIN32)")
    #define APP_NAME_LEN 22
#endif

About::About(Widget* parent)
    : DashLayer(parent)
    , m_main(0)
    , m_url(0, 0, 0, 0)
    , m_conn(0, 0, 0, 0)
    , m_click(0)
{
    setTitle(U("¹ØÓÚ"));
    setCommitText(U("·µ»Ø"));
    setCancel(false);
}

About::~About()
{
}

void About::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void About::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    DashLayer::OnCreate(flags, x, y, w, h);

    int b = DASH_TITLE_HEIGHT / 30;
    m_url = Rect(b * 75, b * 150, b * 135, b * 12);
    m_conn = Rect(b * 75, b * 170, b * 140, b * 12);
}

void About::OnPaint(ps_context* gc, const Rect* r)
{
    DashLayer::OnPaint(gc, r);

    ps_save(gc);

    Rect rr(0, 0, 0, 0);
    ps_font* f = DashLayer::getTitleFont();
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
    int b = DASH_TITLE_HEIGHT / 30;

    ps_wide_text_out_length(gc, b * 40, b * 80, P16(APP_NAME), APP_NAME_LEN);

    ps_set_font(gc, of);

    f = MainWindow::getUIFont();
    of = ps_set_font(gc, f);
    ps_text_out_length(gc, b * 85, b * 100, VERSION_STRING, strlen(VERSION_STRING));

    ps_wide_text_out_length(gc, b * 15, b * 150, P16(U("¹Ù·½Ö÷Ò³:")), 5);
    ps_wide_text_out_length(gc, b * 15, b * 170, P16(U("ÁªÏµÎÒÃÇ:")), 5);
    if (m_click) {
        ps_color tc = {1, 0, 0, 1};
        ps_set_text_color(gc, &tc);
    } else {
        ps_color tc = {0, 0, 1, 1};
        ps_set_text_color(gc, &tc);
    }

    rr = *r;
    if (rr.intersect(m_url)) {
        ps_wide_text_out_length(gc, b * 75, b * 150, P16(U("http://www.zncsoft.com")), 22);
    }

    rr = *r;
    if (rr.intersect(m_conn)) {
        ps_wide_text_out_length(gc, b * 75, b * 170, P16(U("http://conn.zncsoft.com")), 23);
    }

    ps_set_font(gc, of);

    ps_restore(gc);
}

void About::link_click(void* p)
{
    int click = (intptr_t)p;

    if (click == 1) {
        Update(&m_url);
        TabPage* p = Application::getInstance()->getMainWindow()->getTabs();
        if (!p->newView("http://www.zncsoft.com")) {
            p->getActiveView()->loadUrl("http://www.zncsoft.com");
        }
    } else if (click == 2) {
        Update(&m_conn);
        TabPage* p = Application::getInstance()->getMainWindow()->getTabs();
        if (!p->newView("http://www.zncsoft.com/concent.html")) {
            p->getActiveView()->loadUrl("http://www.zncsoft.com/concent.html");
        }
    }
    Commit();

}

void About::OnMouseEvent(const MouseEvent* e)
{
    DashLayer::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        if (m_url.contains(e->x(), e->y())) {
            m_click = 1;
        } else if (m_conn.contains(e->x(), e->y())) {
            m_click = 2;
        } else {
            m_click = 0;
        }

        if (m_click == 1) {
            Update(&m_url);
        } else if (m_click == 2) {
            Update(&m_conn);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_click) {
            Widget::postEvent(this, EVENT_FUNC(About, link_click), EVENT_PARAM(m_click));
            m_click = 0;
        }
    }
}
