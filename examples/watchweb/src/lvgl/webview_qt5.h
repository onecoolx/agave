/* webview_qt5.h - WatchWeb LVGL backend (replaces Qt5 WebViewPlat) */
#ifndef _WEBVIEW_LVGL_H_
#define _WEBVIEW_LVGL_H_

class WebView;
class MainWindowImpl;

class WebViewPlat {
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
    unsigned char* m_buffer;
    int m_width;
    int m_height;
    ps_context* m_context;
    ps_canvas* m_canvas;
};

#endif /* _WEBVIEW_LVGL_H_ */
