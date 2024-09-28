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

#if defined(MOBILE_PHONE)
    #define APP_NAME U("Agave Mobile (Mobile)")
    #define APP_NAME_LEN 19
#elif defined(WINCE)
    #define APP_NAME U("Agave Mobile (WinCE)")
    #define APP_NAME_LEN 22
#elif defined(__linux)
    #define APP_NAME U("Agave Mobile (Linux)")
    #define APP_NAME_LEN 22
#elif defined(WIN32)
    #define APP_NAME U("Agave Mobile (WIN32)")
    #define APP_NAME_LEN 22
#endif

About::About(Widget* parent)
    : DashLayer(parent)
    , m_main(0)
    , m_email(0, 0, 0, 0)
    , m_click(0)
{
    setTitle(U("About"));
    setCommitText(U("Back"));
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
    m_email = Rect(b * 75, b * 150, b * 135, b * 12);
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

    ps_wide_text_out_length(gc, b * 15, b * 150, P16(U("Email:")), 6);
    if (m_click) {
        ps_color tc = {1, 0, 0, 1};
        ps_set_text_color(gc, &tc);
    } else {
        ps_color tc = {0, 0, 1, 1};
        ps_set_text_color(gc, &tc);
    }

    rr = *r;
    if (rr.intersect(m_email)) {
        ps_wide_text_out_length(gc, b * 75, b * 150, P16(U("onecoolx@gmail.com")), 18);
    }

    ps_set_font(gc, of);

    ps_restore(gc);
}

void About::link_click(void* p)
{
    int click = (intptr_t)p;

    if (click == 1) {
        Update(&m_email);
        TabPage* p = Application::getInstance()->getMainWindow()->getTabs();
        if (!p->newView("MailTo")) {
            p->getActiveView()->loadUrl("http://mail.google.com");
        }
    }
    Commit();

}

void About::OnMouseEvent(const MouseEvent* e)
{
    DashLayer::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        if (m_email.contains(e->x(), e->y())) {
            m_click = 1;
        } else {
            m_click = 0;
        }

        if (m_click == 1) {
            Update(&m_email);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_click) {
            Widget::postEvent(this, EVENT_FUNC(About, link_click), EVENT_PARAM(m_click));
            m_click = 0;
        }
    }
}
