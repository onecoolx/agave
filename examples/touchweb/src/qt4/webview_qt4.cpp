/* webview_qt4.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <QImage>
#include "config.h"
#include "picasso.h"
#include "webview.h"
#include "mainwindow.h"
#include "mainwindow_qt4.h"
#include "webview_qt4.h"

WebViewPlat::WebViewPlat(WebView* view)
    : m_view(view)
    , m_img(0)
    , m_context(0)
    , m_canvas(0)
{
}

WebViewPlat::~WebViewPlat()
{
    if (m_img) {
        delete m_img;
        m_img = 0;
    }
}

bool WebViewPlat::CreateStoreBuffer(int w, int h)
{
    m_img = new QImage(w, h, QImage::Format_RGB32);
    m_img->fill(0xFFFFFFFF);
    m_canvas = ps_canvas_create_with_data((ps_byte*)m_img->bits(), COLOR_FORMAT_BGRA, w, h, w * 4);
    m_context = ps_context_create(m_canvas);
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

    if (m_img) {
        delete m_img;
        m_img = 0;
    }
}

ps_context* WebViewPlat::getContext(void)
{
    return m_context;
}

unsigned char* WebViewPlat::GetBuffer(int* pitch)
{
    if (pitch && m_img) {
        *pitch = m_img->bytesPerLine();
    }

    return m_img ? static_cast<unsigned char*>(m_img->bits()) : 0;
}

void WebViewPlat::Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h)
{
    window->DrawImage(m_img, m_view->parent()->x(), m_view->parent()->y(),
                      m_view->parent()->width(), m_view->parent()->height(), sx, sy, x, y, w, h);
}

bool WebViewPlat::haveMemory(void)
{
    return true;
}
