/* webview.h - Agave engine wrapper with tile buffer rendering
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_WEBVIEW_H_
#define _WATCHWEB_WEBVIEW_H_

#include "config.h"
#include <stdint.h>
#include "macross.h"

class WebView {
public:
    WebView();
    ~WebView();

    void create(int view_w, int view_h);
    void destroy();

    void loadUrl(const char* url);
    void goBack();
    void goForward();
    void reload();
    void stop();

    void scrollBy(int dx, int dy);
    int scrollX() const { return m_pos_x; }
    int scrollY() const { return m_pos_y; }

    void setZoom(float factor);
    float zoom() const { return m_zoom; }

    int contentsWidth() const;
    int contentsHeight() const;
    bool isLoading() const { return m_loading; }
    unsigned int progress() const { return m_progress; }
    const char* title() const { return m_title; }
    const char* url() const { return m_url; }

    /* Visible region in tile buffer */
    const uint8_t* buffer() const { return m_buffer; }
    int bufferWidth() const { return TILE_BUF_W; }
    int bufferHeight() const { return TILE_BUF_H; }
    int offsetX() const { return m_off_x; }
    int offsetY() const { return m_off_y; }

    void mousePress(int x, int y);
    void mouseMove(int x, int y);
    void mouseRelease(int x, int y);

    typedef void (*Callback)(void*);
    void setUpdateCb(Callback cb, void* d) { m_on_update = cb; m_ud = d; }
    void setStateCb(Callback cb, void* d) { m_on_state = cb; m_sd = d; }

private:
    void recalcOffset();
    static void s_dirty(MaCrossView*, const MC_RECT*);
    static void s_update(MaCrossView*);
    static void s_loading(MaCrossView*, unsigned int, MC_BOOL);
    static void s_title(MaCrossView*, const char*);
    static void s_url(MaCrossView*, const char*);

    MaCrossView* m_view;
    uint8_t* m_buffer;
    int m_view_w, m_view_h;
    int m_pos_x, m_pos_y;
    int m_off_x, m_off_y;
    float m_zoom;
    bool m_loading;
    unsigned int m_progress;
    char m_title[256];
    char m_url[1024];
    Callback m_on_update; void* m_ud;
    Callback m_on_state; void* m_sd;
};

#endif
