/* webview.cpp - Agave engine wrapper with tile buffer
 * Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
 */
#include <stdlib.h>
#include <string.h>
#include "webview.h"

/* Tile-buffer offset model (ported from touchweb's start_offset_X/Y).
 *
 * The tile buffer is view*2 wide and view*3 tall. start_offset returns where
 * the viewport sits inside the buffer for a given scroll position, in three
 * zones so the engine renders the surrounding region correctly:
 *   - near the start: offset grows from 0 (top/left edge fully reachable);
 *   - middle: viewport centered in the buffer (room to scroll either way);
 *   - near the end: offset = (buffer extent) - (remaining content), so the
 *     bottom/right edge is reachable without rendering past content.
 * The engine render position is then (scrollPos - offset), and the blit reads
 * the visible region from the buffer at 'offset'. This keeps the blit window
 * inside [0, bufferExtent - view] for every scroll position.
 */
static int startOffset(MaCrossView* v, int currentPos, int view, int bufExtent, bool horizontal)
{
    MC_SIZE size = {0, 0};
    macross_view_get_contents_size(v, &size);
    int contentExtent = horizontal ? size.w : size.h;

    if (currentPos < 0) { currentPos = 0; }
    int last = (contentExtent - currentPos - view) > 0 ? (contentExtent - currentPos - view) : 0;

    int center = (bufExtent - view) / 2; /* viewport centered in the buffer */
    int endPos = bufExtent - view; /* viewport at the buffer's far edge */

    if (currentPos < center) {
        /* Near the start: place the viewport at its true distance from origin. */
        return currentPos;
    } else if (last < (bufExtent - view - center)) {
        /* Near the end: anchor so the last 'last' px of content stays visible
           and the engine never has to render past the content bottom/right. */
        int off = endPos - last;
        if (off < center) { off = center; }
        if (off > endPos) { off = endPos; }
        return off;
    }
    /* Middle: keep the viewport centered. */
    return center;
}

WebView::WebView()
    : m_view(nullptr), m_buffer(nullptr)
    , m_view_w(0), m_view_h(0)
    , m_pos_x(0), m_pos_y(0)
    , m_engine_x(0), m_engine_y(0)
    , m_off_x(0), m_off_y(0), m_engine_repaint(false)
    , m_zoom(DEFAULT_ZOOM), m_loading(false), m_progress(0)
    , m_on_update(nullptr), m_ud(nullptr)
    , m_on_state(nullptr), m_sd(nullptr)
    , m_on_blit(nullptr), m_bd(nullptr)
{
    m_title[0] = 0;
    m_url[0] = 0;
}

WebView::~WebView() { destroy(); }

void WebView::create(int vw, int vh)
{
    m_view_w = vw;
    m_view_h = vh;
    m_buffer = (uint8_t*)malloc(TILE_BUF_W * TILE_BUF_H * 4);
    if (!m_buffer) { return; }
    memset(m_buffer, 0xFF, TILE_BUF_W * TILE_BUF_H * 4);

    /* Register global callbacks */
    MC_CALLBACK_INFO cb = {};
    cb.cb_invalidate_rect = s_dirty;
    cb.cb_update_view_now = s_update;
    cb.cb_loading_progress = s_loading;
    cb.cb_set_title = s_title;
    cb.cb_set_location = s_url;
    macross_set_callback(&cb);

    /* Tile-buffer mode scrolls the whole page itself; the engine must not draw
       its own scrollbars (they would smear across the tile buffer). */
    MC_CONFIG config;
    if (macross_get_config(&config)) {
        config.scrollbar_width = 0;
        macross_set_config(&config);
    }

    MC_SIZE lsize = {vw, vh};
    m_view = macross_view_create(m_buffer, TILE_BUF_W, TILE_BUF_H, TILE_BUF_W * 4, this);
    macross_view_set_minimum_layout_size(m_view, &lsize);
    macross_view_set_scale_factor(m_view, (int)(100 * m_zoom));
    m_engine_x = 0;
    m_engine_y = 0;
    m_off_x = 0;
    m_off_y = 0;
}

void WebView::destroy()
{
    if (m_view) { macross_view_destroy(m_view); m_view = nullptr; }
    if (m_buffer) { free(m_buffer); m_buffer = nullptr; }
}

void WebView::loadUrl(const char* url) { if (m_view) macross_view_open_url(m_view, url); }
void WebView::goBack() { if (m_view) macross_view_backward(m_view); }
void WebView::goForward() { if (m_view) macross_view_forward(m_view); }
void WebView::reload() { if (m_view) macross_view_reload(m_view); }
void WebView::stop() { if (m_view) macross_view_stop(m_view); }

void WebView::scrollBy(int dx, int dy)
{
    MC_SIZE sz = {0, 0};
    macross_view_get_contents_size(m_view, &sz);

    m_pos_x += dx;
    m_pos_y += dy;
    if (m_pos_x < 0) { m_pos_x = 0; }
    if (m_pos_y < 0) { m_pos_y = 0; }
    if (sz.w > m_view_w && m_pos_x > sz.w - m_view_w) { m_pos_x = sz.w - m_view_w; }
    if (sz.h > m_view_h && m_pos_y > sz.h - m_view_h) { m_pos_y = sz.h - m_view_h; }
    if (m_pos_x < 0) { m_pos_x = 0; }
    if (m_pos_y < 0) { m_pos_y = 0; }

    /* Offset of viewport within the tile buffer (relative to engine render pos) */
    m_off_x = m_pos_x - m_engine_x;
    m_off_y = m_pos_y - m_engine_y;

    /* If viewport still fits inside the rendered tile buffer, just blit (no
       engine repaint - avoids triggering re-layout during drag). */
    if (m_off_x >= 0 && m_off_y >= 0
        && m_off_x <= TILE_BUF_W - m_view_w
        && m_off_y <= TILE_BUF_H - m_view_h) {
        if (m_on_blit) { m_on_blit(m_bd); }
        return;
    }

    /* Viewport moved outside the buffered tile: re-center engine render
       position and request a fresh engine paint. */
    repositionEngine();
}

void WebView::renderTile()
{
    if (!m_view) { return; }
    /* Paint the whole tile-buffer region, not just the engine's accumulated
       dirty rect. After a reposition the engine renders a new slice of the page
       into the buffer; forcing a full-buffer paint guarantees the entire buffer
       holds fresh content so later blit-only scrolls never expose stale/blank
       areas. */
    MC_RECT full = {0, 0, TILE_BUF_W, TILE_BUF_H};
    macross_view_update(m_view, &full);
    m_engine_repaint = false;
}

void WebView::repositionEngine()
{
    /* Compute where the viewport sits inside the tile buffer using the
       three-zone model, then render the engine at (pos - offset) so the buffer
       holds the region around the viewport. This makes the top, middle and the
       bottom/right edge all reachable (the naive "always center" version could
       not place the engine to cover content near the bottom). */
    m_off_x = startOffset(m_view, m_pos_x, m_view_w, TILE_BUF_W, true);
    m_off_y = startOffset(m_view, m_pos_y, m_view_h, TILE_BUF_H, false);

    m_engine_x = m_pos_x - m_off_x;
    m_engine_y = m_pos_y - m_off_y;
    if (m_engine_x < 0) { m_engine_x = 0; m_off_x = m_pos_x; }
    if (m_engine_y < 0) { m_engine_y = 0; m_off_y = m_pos_y; }

    macross_view_set_position(m_view, m_engine_x, m_engine_y);
    m_engine_repaint = true;
    if (m_on_update) { m_on_update(m_ud); }
}

void WebView::setZoom(float f)
{
    if (f < MIN_ZOOM) { f = MIN_ZOOM; }
    if (f > MAX_ZOOM) { f = MAX_ZOOM; }
    m_zoom = f;
    if (m_view) { macross_view_set_scale_factor(m_view, (int)(100 * f)); }
    m_pos_x = m_pos_y = 0;
    m_engine_x = m_engine_y = 0;
    m_off_x = m_off_y = 0;
    if (m_view) { macross_view_set_position(m_view, 0, 0); }
}

int WebView::contentsWidth() const
{
    MC_SIZE s = {0, 0};
    if (m_view) { macross_view_get_contents_size(m_view, &s); }
    return s.w;
}
int WebView::contentsHeight() const
{
    MC_SIZE s = {0, 0};
    if (m_view) { macross_view_get_contents_size(m_view, &s); }
    return s.h;
}

void WebView::mousePress(int x, int y)
{
    MC_MOUSE_EVENT e = {};
    e.type = EVT_MOUSE_DOWN;
    e.button = MOUSE_BTN_LEFT;
    e.point.x = x + m_off_x;
    e.point.y = y + m_off_y;
    macross_mouse_event(m_view, &e);
}
void WebView::mouseMove(int x, int y)
{
    MC_MOUSE_EVENT e = {};
    e.type = EVT_MOUSE_MOVE;
    e.button = MOUSE_BTN_NONE;
    e.point.x = x + m_off_x;
    e.point.y = y + m_off_y;
    macross_mouse_event(m_view, &e);
}
void WebView::mouseRelease(int x, int y)
{
    MC_MOUSE_EVENT e = {};
    e.type = EVT_MOUSE_UP;
    e.button = MOUSE_BTN_LEFT;
    e.point.x = x + m_off_x;
    e.point.y = y + m_off_y;
    macross_mouse_event(m_view, &e);
}

/* Static callbacks */
void WebView::s_dirty(MaCrossView* v, const MC_RECT* r)
{
    /* cb_invalidate_rect marks a region dirty (like Qt's QWidget::update). It is
       the primary repaint trigger: during a page load most content updates come
       through here with the engine's "now == false" path, while
       cb_update_view_now (s_update) only fires for the rarer "paint immediately"
       case. So we MUST schedule a paint here too, otherwise a page that finishes
       loading via invalidate-only updates never renders (it just sits blank).

       It is safe to schedule a paint even if layout is still pending: the actual
       paint goes through macross_view_update -> WebView::paintView(), which
       calls layoutIfNeededRecursive() before drawing. We only set a flag here;
       the main loop performs the paint. */
    WebView* self = (WebView*)macross_view_additional_data(v);
    if (self && self->m_on_update) { self->m_on_update(self->m_ud); }
}
void WebView::s_update(MaCrossView* v)
{
    WebView* self = (WebView*)macross_view_additional_data(v);
    if (self->m_on_update) { self->m_on_update(self->m_ud); }
}
void WebView::s_loading(MaCrossView* v, unsigned int prog, MC_BOOL finish)
{
    WebView* self = (WebView*)macross_view_additional_data(v);
    self->m_progress = prog;
    self->m_loading = !finish;
    if (self->m_on_state) { self->m_on_state(self->m_sd); }
}
void WebView::s_title(MaCrossView* v, const char* t)
{
    WebView* self = (WebView*)macross_view_additional_data(v);
    strncpy(self->m_title, t ? t : "", sizeof(self->m_title) - 1);
    if (self->m_on_state) { self->m_on_state(self->m_sd); }
}
void WebView::s_url(MaCrossView* v, const char* u)
{
    WebView* self = (WebView*)macross_view_additional_data(v);
    strncpy(self->m_url, u ? u : "", sizeof(self->m_url) - 1);
    if (self->m_on_state) { self->m_on_state(self->m_sd); }
}
