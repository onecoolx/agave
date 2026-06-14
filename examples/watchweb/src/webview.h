/* webview.h - Agave engine wrapper with tile buffer rendering
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#ifndef _WATCHWEB_WEBVIEW_H_
#define _WATCHWEB_WEBVIEW_H_

#include "config.h"
#include <stdint.h>
#include "macross.h"

class WebView
{
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

    /* Repaint the whole tile buffer from the engine (call when the engine has
       signalled content/position changed). Paints the full buffer region so
       scrolling within the buffer always finds rendered content. */
    void renderTile();

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

    /* Forward typed input from an on-screen keyboard to the page. sendChar
       inserts a character (Unicode code point); sendKey sends a special key
       such as Backspace or Enter. */
    void sendChar(unsigned int codepoint);
    void sendText(const char* utf8_text);
    void sendKey(int vkey);

    MaCrossView* view() const { return m_view; }

    typedef void (*Callback)(void*);
    typedef void (*ImeCallback)(void*, bool enable);
    void setUpdateCb(Callback cb, void* d) { m_on_update = cb; m_ud = d; }
    void setStateCb(Callback cb, void* d) { m_on_state = cb; m_sd = d; }
    void setBlitCb(Callback cb, void* d) { m_on_blit = cb; m_bd = d; }
    /* Notified when a page editable field gains (enable=true) or loses focus,
       so the UI can show/hide the on-screen keyboard. */
    void setImeCb(ImeCallback cb, void* d) { m_on_ime = cb; m_id = d; }

private:
    void repositionEngine();
    static void s_dirty(MaCrossView*, const MC_RECT*);
    static void s_update(MaCrossView*);
    static void s_layout(MaCrossView*);
    static void s_loading(MaCrossView*, unsigned int, MC_BOOL);
    static void s_title(MaCrossView*, const char*);
    static void s_url(MaCrossView*, const char*);
    static void s_ime(MC_BOOL enable);

    static WebView* s_instance; /* for the view-less IME callback */

    MaCrossView* m_view;
    uint8_t* m_buffer;
    int m_view_w, m_view_h;
    int m_pos_x, m_pos_y; /* viewport position in content coords */
    int m_engine_x, m_engine_y; /* tile buffer top-left in content coords */
    int m_off_x, m_off_y; /* viewport offset within tile buffer */
    bool m_engine_repaint;
    float m_zoom;
    bool m_loading;
    bool m_first_layout_done; /* page visually rendered: end the progress bar */
    unsigned int m_progress;
    char m_title[256];
    char m_url[1024];
    Callback m_on_update;
    void* m_ud;
    Callback m_on_state;
    void* m_sd;
    Callback m_on_blit;
    void* m_bd;
    ImeCallback m_on_ime;
    void* m_id;
};

#endif
