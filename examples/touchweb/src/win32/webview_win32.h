/* webview_win32.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _WEBVIEW_WIN32_H_
#define _WEBVIEW_WIN32_H_

#include <windows.h>

class WebView;
class MainWindowImpl;

class WebViewPlat
{
public:
    WebViewPlat(WebView* view);
    ~WebViewPlat();

    bool CreateStoreBuffer(int width, int height);
    void DestroyStoreBuffer(void);

    void Paint(MainWindowImpl* window, int sx, int sy, int x, int y, int w, int h);

    unsigned char* GetBuffer(int* pitch);

    ps_context* getContext(void);
    static bool haveMemory(void);
private:
    WebView* m_view;
    BITMAP* m_bmp;
    ps_context* m_context;
    ps_canvas* m_canvas;
};

#endif/*_WEBVIEW_WIN32_H_*/
