/* webview_qt4.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _WEBVIEW_QT4_H_
#define _WEBVIEW_QT4_H_

class WebView;
class MainWindowImpl;

class QImage;

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
    QImage* m_img;
    ps_context* m_context;
    ps_canvas* m_canvas;
};

#endif/*_WEBVIEW_QT4_H_*/
