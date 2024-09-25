/* webview_gtk2.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "webview.h"
#include "mainwindow.h"
#include "mainwindow_gtk2.h"
#include "webview_gtk2.h"

WebViewPlat::WebViewPlat(WebView* view)
    : m_view(view)
    , m_buf(NULL)
    , m_context(NULL)
    , m_canvas(NULL)
{
}

WebViewPlat::~WebViewPlat()
{
    if (m_buf) {
        g_object_unref(m_buf);
        m_buf = NULL;
    }
}

bool WebViewPlat::CreateStoreBuffer(int w, int h)
{
    m_buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, w, h);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(m_buf);
    gint stride = gdk_pixbuf_get_rowstride(m_buf);
    gdk_pixbuf_fill(m_buf, 0xFFFFFFFF);

    m_canvas = ps_canvas_create_with_data((ps_byte*)buf, COLOR_FORMAT_RGB, w, h, stride);
    m_context = ps_context_create(m_canvas, 0);
    return true;
}

void WebViewPlat::DestroyStoreBuffer(void)
{
    if (m_context) {
        ps_context_unref(m_context);
    }

    if (m_canvas) {
        ps_canvas_unref(m_canvas);
    }

    if (m_buf) {
        g_object_unref(m_buf);
        m_buf = NULL;
    }
}

ps_context* WebViewPlat::getContext(void)
{
    return m_context;
}

unsigned char* WebViewPlat::GetBuffer(int* pitch)
{
    if (pitch && m_buf) {
        *pitch = gdk_pixbuf_get_rowstride(m_buf);
    }

    return m_buf ? static_cast<unsigned char*>(gdk_pixbuf_get_pixels(m_buf)) : 0;
}

void WebViewPlat::Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h)
{
    window->DrawImage(m_buf, m_view->parent()->x(), m_view->parent()->y(),
                      m_view->parent()->width(), m_view->parent()->height(), sx, sy, x, y, w, h);
}

bool WebViewPlat::haveMemory(void)
{
    return true;
}
