/* webview_lvgl.cpp - WatchWeb LVGL backend (replaces Qt5 WebViewPlat) */
#include <stdlib.h>
#include <string.h>
#include <picasso/picasso.h>

#include "config.h"
#include "webview.h"
#include "mainwindow.h"
#include "mainwindow_lvgl.h"
#include "webview_qt5.h"

WebViewPlat::WebViewPlat(WebView* view)
    : m_view(view)
    , m_buffer(nullptr)
    , m_width(0)
    , m_height(0)
    , m_context(nullptr)
    , m_canvas(nullptr)
{
}

WebViewPlat::~WebViewPlat()
{
    DestroyStoreBuffer();
}

bool WebViewPlat::CreateStoreBuffer(int w, int h)
{
    m_width = w;
    m_height = h;
    m_buffer = (unsigned char*)malloc(w * h * 4);
    memset(m_buffer, 0xFF, w * h * 4);
    m_canvas = ps_canvas_create_with_data(m_buffer, COLOR_FORMAT_BGRA, w, h, w * 4);
    m_context = ps_context_create(m_canvas, 0);
    return true;
}

void WebViewPlat::DestroyStoreBuffer(void)
{
    if (m_context) { ps_context_unref(m_context); m_context = nullptr; }
    if (m_canvas) { ps_canvas_unref(m_canvas); m_canvas = nullptr; }
    if (m_buffer) { free(m_buffer); m_buffer = nullptr; }
}

ps_context* WebViewPlat::getContext(void)
{
    return m_context;
}

unsigned char* WebViewPlat::GetBuffer(int* pitch)
{
    if (pitch) *pitch = m_width * 4;
    return m_buffer;
}

void WebViewPlat::Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h)
{
    window->DrawImage(m_buffer, m_view->parent()->x(), m_view->parent()->y(),
                      m_view->parent()->width(), m_view->parent()->height(), sx, sy, x, y, w, h);
}

bool WebViewPlat::haveMemory(void)
{
    return true;
}
