/* webview_win32.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "webview.h"
#include "mainwindow.h"
#include "mainwindow_win32.h"
#include "webview_win32.h"
#include "application.h"
#include "application_win32.h"

WebViewPlat::WebViewPlat(WebView* view)
    : m_view(view)
    , m_bmp(0)
    , m_context(0)
    , m_canvas(0)
{
}

WebViewPlat::~WebViewPlat()
{
    if (m_bmp) {
        free(m_bmp->bmBits);
        delete m_bmp;
        m_bmp = 0;
    }
}

bool WebViewPlat::CreateStoreBuffer(int w, int h)
{
    ps_color_format fmt;
    int bit = Application::getInstance()->bits_pixel();
    int byte = Application::getInstance()->bytes_pixel();

    if (byte == 4) {
        fmt = COLOR_FORMAT_BGRA;
    } else if (byte == 2) {
        fmt = COLOR_FORMAT_RGB565;
    } else {
        return false;
    }

    m_bmp = new BITMAP;
    m_bmp->bmType = 0;
    m_bmp->bmWidth = w;
    m_bmp->bmHeight = h;
    m_bmp->bmWidthBytes = w * byte;
    m_bmp->bmPlanes = 1;
    m_bmp->bmBitsPixel = bit;
    m_bmp->bmBits = malloc(w * h * byte);
    memset(m_bmp->bmBits, 0xFF, w * h * byte);

    m_canvas = ps_canvas_create_with_data((ps_byte*)m_bmp->bmBits, fmt, w, h, w * byte);
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

    if (m_bmp) {
        free(m_bmp->bmBits);
        delete m_bmp;
        m_bmp = 0;
    }
}

ps_context* WebViewPlat::getContext(void)
{
    return m_context;
}

unsigned char* WebViewPlat::GetBuffer(int* pitch)
{
    if (pitch && m_bmp) {
        *pitch = m_bmp->bmWidthBytes;
    }

    return m_bmp ? static_cast<unsigned char*>(m_bmp->bmBits) : 0;
}

void WebViewPlat::Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h)
{
    window->DrawImage(m_bmp, m_view->parent()->x(), m_view->parent()->y(),
                      m_view->parent()->width(), m_view->parent()->height(), sx, sy, x, y, w, h);
}

bool WebViewPlat::haveMemory(void)
{
#if defined(WINCE)
    MEMORYSTATUS mem;
    GlobalMemoryStatus(&mem);
    return ((mem.dwAvailVirtual / 1024) > 1024) ? true : false; //free memory > 1MB
#else
    return true;
#endif
}
