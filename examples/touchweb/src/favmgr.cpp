/* favmgr.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "favmgr.h"
#include "favorites.h"
#include "mainwindow.h"
#include "application.h"
#include "unicode.h"
#include "edit.h"
#include "webview.h"
#include "tabpage.h"
#include "dialog.h"

static void draw_button(ps_context*, const Rect&, const ustring&, bool, bool);

FavoritesPanel::FavoritesPanel(Widget* parent, FavoritesManager* mgr)
    : Widget(parent)
    , m_favmgr(mgr)
    , m_main(0)
    , m_title(0)
    , m_addr(0)
    , m_tags(0)
    , m_commit(0, 0, 0, 0)
    , m_cancel(0, 0, 0, 0)
    , m_event(0, 0, 0, 0)
    , m_btn(0)
{
    m_title = new LineEdit(this);
    m_addr = new LineEdit(this);
    m_tags = new LineEdit(this);
}

FavoritesPanel::~FavoritesPanel()
{
    delete m_tags;
    delete m_addr;
    delete m_title;
}

void FavoritesPanel::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    Widget::OnCreate(flags, x, y, w, h);

    int b = DASH_TITLE_HEIGHT / 30;
    int hc = DASH_TITLE_HEIGHT - DASH_TITLE_HEIGHT / 30 * 8;
    int bc = DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8;
    m_title->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, b * 5, b * 4, width() - b * 10, hc);
    m_addr->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, b * 5, b * 5 + hc, width() - b * 10, hc);
    m_tags->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, b * 5, b * 6 + hc * 2, width() - b * 10, hc);
    m_tags->setTipText(U("Use commas to separate tags"));

    m_commit = Rect(b * 50, hc * 3 + b * 20, b * 100, bc);
    m_cancel = Rect(width() - b * 150, hc * 3 + b * 20, b * 100, bc);

    addChild(m_title);
    addChild(m_addr);
    addChild(m_tags);
}

void FavoritesPanel::OnPaint(ps_context* gc, const Rect* d)
{
    Widget::OnPaint(gc, d);
    ps_save(gc);

    ps_color c = {0.8f, 0.8f, 0.8f, 1};
    ps_rect rc = {0, 0, width(), height()};
    ps_rectangle(gc, &rc);
    ps_set_source_color(gc, &c);
    ps_fill(gc);

    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    draw_button(gc, m_cancel, U("Cancel"), true, (m_btn == 1) ? true : false);
    draw_button(gc, m_commit, U("Ok"), true, (m_btn == 2) ? true : false);

    ps_set_font(gc, of);
    ps_restore(gc);
}

void FavoritesPanel::OnShow(void)
{
    Widget::OnShow();

    WebView* web = Application::getInstance()->getMainWindow()->getTabs()->getActiveView();
    m_title->setText(web->title());
    m_addr->setText(web->location());
    m_tags->setText(ustring());
}

void FavoritesPanel::OnHide(void)
{
    Widget::OnHide();

    if (m_title->hasFocus()) {
        m_title->setFocus(false);
    }

    if (m_addr->hasFocus()) {
        m_addr->setFocus(false);
    }

    if (m_tags->hasFocus()) {
        m_tags->setFocus(false);
    }
}

void FavoritesPanel::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void FavoritesPanel::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_cancel.contains(e->x(), e->y())) {
            m_event = m_cancel;
            m_btn = 1;
        } else if (m_commit.contains(e->x(), e->y())) {
            m_event = m_commit;
            m_btn = 2;
        } else {
            m_event = Rect(0, 0, 0, 0);
            m_btn = 0;
        }

        if (m_btn) {
            Update(&m_event);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {

            if (m_btn == 2) {
                Favorites* fav = Application::getInstance()->getFavorites();
                fav->addFavorites(
                    Unicode::ConvertUTF16ToUTF8(m_title->getText()),
                    Unicode::ConvertUTF16ToUTF8(m_addr->getText()),
                    Unicode::ConvertUTF16ToUTF8(m_tags->getText()));
                Widget::postEvent(m_favmgr->m_view, EVENT_FUNC(FavoritesView, refresh), 0);
            }

            m_btn = 0;
            Update(&m_event);
            m_event = Rect(0, 0, 0, 0);
            m_favmgr->hidePanel();
        }
    }
}

class FavoritesViewImpl
{
public:
    FavoritesViewImpl()
        : g_icon(0)
        , g_btn(0)
        , t_mtx(0)
    {
        t_mtx = ps_matrix_create();
    }

    ~FavoritesViewImpl()
    {
        if (g_icon) {
            ps_gradient_unref(g_icon);
        }

        if (g_btn) {
            ps_gradient_unref(g_btn);
        }

        if (t_mtx) {
            ps_matrix_unref(t_mtx);
        }
    }

    ps_gradient* g_icon;
    ps_gradient* g_btn;
    ps_matrix* t_mtx;
};

//favorites view
FavoritesView::FavoritesView(Widget* parent)
    : ScrollView(parent)
    , m_impl(new FavoritesViewImpl)
    , m_main(0)
    , m_page(0)
    , m_pageup(0, 0, 0, 0)
    , m_pagedown(0, 0, 0, 0)
    , m_enableup(false)
    , m_enabledown(false)
    , m_event(0, 0, 0, 0)
    , m_btn(0)
    , m_pagebtn(0)
    , m_cbtn(0)
    , m_startX(0)
    , m_startY(0)
    , m_capture(false)
    , m_drag(false)
    , m_ticket(0)
    , m_time(1)
    , m_drx(0)
    , m_dry(0)
    , m_scx(0)
    , m_scy(0)
{
}

FavoritesView::~FavoritesView()
{
    delete m_impl;
}

void FavoritesView::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void FavoritesView::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    ScrollView::OnCreate(flags, x, y, w, h);
    //icon gradient
    int b = TOOLBAR_HEIGHT / 30;
    int hc = DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8;
    ps_color c1 = {1, 1, 1, 1};
    ps_color c2 = {1, 0.84, 0.24, 1};
    ps_point s = {0, 0};
    ps_point e = {b * 14, b * 14};
    m_impl->g_icon = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
    ps_gradient_add_color_stop(m_impl->g_icon, 0.4, &c1);
    ps_gradient_add_color_stop(m_impl->g_icon, 1, &c2);

    //btn gradient
    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_point s1 = {0, 0};
    ps_point e1 = {0, hc / 2 - 2 * b};
    m_impl->g_btn = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(m_impl->g_btn, 0, &tc);
    ps_gradient_add_color_stop(m_impl->g_btn, 1, &tc2);
}

void FavoritesView::OnDestroy(void)
{
    ScrollView::OnDestroy();
}

void FavoritesView::draw_icon(ps_context* gc, const ps_rect* r)
{
    int b = TOOLBAR_HEIGHT / 30;
    ps_rect rc = {r->x + 3 * b, r->y + 3 * b, b * 14, b * 14};
    ps_set_line_width(gc, b);
    ps_set_line_join(gc, LINE_JOIN_ROUND);
    ps_color fc = {0.9, 0.7, 0.15, 1};
    ps_set_stroke_color(gc, &fc);

    ps_point pt[10] = {
        {rc.x, rc.y + b * 6},
        {rc.x + b * 5, rc.y + b * 5},
        {rc.x + b * 7, rc.y},
        {rc.x + b * 9, rc.y + b * 5},
        {rc.x + b * 14, rc.y + b * 6},
        {rc.x + b * 10, rc.y + b * 9},
        {rc.x + b * 11, rc.y + b * 14},
        {rc.x + b * 7, rc.y + b * 11},
        {rc.x + b * 3, rc.y + b * 14},
        {rc.x + b * 4, rc.y + b * 9},
    };

    ps_move_to(gc, &pt[0]);
    for (int i = 1; i < 10; i++) {
        ps_line_to(gc, &pt[i]);
    }
    ps_line_to(gc, &pt[0]);

    ps_matrix_translate(m_impl->t_mtx, rc.x, rc.y);
    ps_gradient_transform(m_impl->g_icon, m_impl->t_mtx);
    ps_set_source_gradient(gc, m_impl->g_icon);
    ps_paint(gc);
    ps_matrix_translate(m_impl->t_mtx, -rc.x * 2, -rc.y * 2);
    ps_gradient_transform(m_impl->g_icon, m_impl->t_mtx);
    ps_matrix_identity(m_impl->t_mtx);
    ps_set_line_width(gc, 1);
}

void FavoritesView::OnPaint(ps_context* gc, const Rect* r)
{
    ScrollView::OnPaint(gc, r);

    if (!m_event.isEmpty()) {
        ps_font* f = MainWindow::getUIBoldFont();
        ps_font* uf = MainWindow::getUIFont();
#ifdef FONT_NO_ANTIALIAS
        ps_set_text_antialias(gc, False);
#else
        if (MainWindow::uiFontAntialias()) {
            ps_set_text_antialias(gc, True);
        } else {
            ps_set_text_antialias(gc, False);
        }
#endif

        Rect tr;
        Rect pr = *r;
        pr.x += scrollX();
        pr.y += scrollY();
        int b = TOOLBAR_HEIGHT / 30;

        for (size_t i = 0; i < m_items.size(); i++) {
            ps_font* of = ps_set_font(gc, f);
            tr = m_items[i].rect;
            if (tr.intersect(pr)) {
                ps_rect rc = m_items[i].rect;
                rc.x -= scrollX();
                rc.y -= scrollY();

                if (m_btn == m_items[i].id) {
                    ps_color fc = {0.5, 0.8, 1, 1};
                    ps_rectangle(gc, &rc);
                    ps_set_source_color(gc, &fc);
                    ps_fill(gc);

                    ps_rectangle(gc, &rc);
                    ps_color c = {0.5, 0.5, 0.5, 1};
                    ps_set_line_width(gc, b);
                    ps_set_stroke_color(gc, &c);
                    ps_stroke(gc);
                    ps_set_line_width(gc, 1);

                    ps_color tc = {0, 0, 0, 1};
                    ps_set_text_color(gc, &tc);

                    int tip_w = m_items[i].rect.w / 4;

                    ps_size sz = {0};
                    ps_get_text_extent(gc, m_items[i].title.c_str(), m_items[i].title.length(), &sz);
                    if (sz.w <= (rc.w - 45 * b - m_items[i].closebtn.w - tip_w)) {
                        ps_wide_text_out_length(gc, b * 20, rc.y + b * 5,
                                                (ps_uchar16*)m_items[i].title.c_str(), m_items[i].title.length());
                    } else {
                        size_t len = (rc.w - 45 * b - m_items[i].closebtn.w - tip_w) / (sz.w / m_items[i].title.length());
                        ustring s = m_items[i].title.substr(0, len) + ustring(U("..."));
                        ps_wide_text_out_length(gc, b * 20, rc.y + b * 5, (ps_uchar16*)s.c_str(), s.length());
                    }

                    ps_set_font(gc, of);
                    ps_font* of = ps_set_font(gc, uf);

                    if (!m_items[i].tags.empty()) {
                        ps_get_text_extent(gc, m_items[i].tags.c_str(), m_items[i].tags.length(), &sz);

                        if (sz.w <= tip_w - m_items[i].closebtn.w) {
                            ps_wide_text_out_length(gc, m_items[i].rect.w - tip_w - m_items[i].closebtn.w, rc.y + b * 5,
                                                    (ps_uchar16*)m_items[i].tags.c_str(), m_items[i].tags.length());
                        } else {
                            size_t len = (tip_w - m_items[i].closebtn.w) / (sz.w / m_items[i].tags.length());
                            ustring s = m_items[i].tags.substr(0, len) + ustring(U("..."));
                            ps_wide_text_out_length(gc, m_items[i].rect.w - tip_w - m_items[i].closebtn.w, rc.y + b * 5,
                                                    (ps_uchar16*)s.c_str(), s.length());
                        }
                    }

                    ps_color uc = {0, 0, 1, 1};
                    ps_set_text_color(gc, &uc);

                    ps_get_text_extent(gc, m_items[i].url.c_str(), m_items[i].url.length(), &sz);
                    if (sz.w <= (rc.w - 45 * b - m_items[i].closebtn.w)) {
                        ps_wide_text_out_length(gc, b * 20, rc.y + b * 8 + b * 12,
                                                (ps_uchar16*)m_items[i].url.c_str(), m_items[i].url.length());
                    } else {
                        size_t len = (rc.w - 45 * b - m_items[i].closebtn.w) / (sz.w / m_items[i].url.length());
                        ustring s = m_items[i].url.substr(0, len) + ustring(U("..."));
                        ps_wide_text_out_length(gc, b * 20, rc.y + b * 8 + b * 12, (ps_uchar16*)s.c_str(), s.length());
                    }

                    draw_icon(gc, &rc);
                }

                if (m_cbtn == m_items[i].id) {
                    Rect brc = m_items[i].closebtn;
                    brc.x -= scrollX();
                    brc.y -= scrollY();
                    draw_closebtn(gc, brc, true);
                } else {
                    Rect brc = m_items[i].closebtn;
                    brc.x -= scrollX();
                    brc.y -= scrollY();
                    draw_closebtn(gc, brc, false);
                }
            }
            ps_set_font(gc, of);
        }

        ps_font* of = ps_set_font(gc, uf);

        tr = m_pageup;
        if (tr.intersect(pr)) {
            Rect brc = m_pageup;
            brc.x -= scrollX();
            brc.y -= scrollY();
            draw_pagebtn(gc, U("<<Previous"), brc, m_enableup, (m_pagebtn == 1) ? true : false);
        }

        tr = m_pagedown;
        if (tr.intersect(pr)) {
            Rect brc = m_pagedown;
            brc.x -= scrollX();
            brc.y -= scrollY();
            draw_pagebtn(gc, U("Next>>"), brc, m_enabledown, (m_pagebtn == 2) ? true : false);
        }
        ps_set_font(gc, of);
    }
}

void FavoritesView::OnPaintContents(ps_context* gc, const Rect* r)
{
    ps_font* f = MainWindow::getUIBoldFont();
    ps_font* uf = MainWindow::getUIFont();
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    Rect tr;
    int b = TOOLBAR_HEIGHT / 30;

    for (size_t i = 0; i < m_items.size(); i++) {
        ps_font* of = ps_set_font(gc, f);
        tr = m_items[i].rect;
        if (tr.intersect(*r)) {
            ps_rect rc = m_items[i].rect;

            ps_rectangle(gc, &rc);
            ps_color c = {0.5, 0.5, 0.5, 1};
            ps_set_line_width(gc, b);
            ps_set_stroke_color(gc, &c);
            ps_stroke(gc);
            ps_set_line_width(gc, 1);

            ps_color tc = {0, 0, 0, 1};
            ps_set_text_color(gc, &tc);

            int tip_w = m_items[i].rect.w / 4;

            ps_size sz = {0};
            ps_get_text_extent(gc, m_items[i].title.c_str(), m_items[i].title.length(), &sz);
            if (sz.w <= (rc.w - 45 * b - m_items[i].closebtn.w - tip_w)) {
                ps_wide_text_out_length(gc, b * 20, rc.y + b * 5,
                                        (ps_uchar16*)m_items[i].title.c_str(), m_items[i].title.length());
            } else {
                size_t len = (rc.w - 45 * b - m_items[i].closebtn.w - tip_w) / (sz.w / m_items[i].title.length());
                ustring s = m_items[i].title.substr(0, len) + ustring(U("..."));
                ps_wide_text_out_length(gc, b * 20, rc.y + b * 5, (ps_uchar16*)s.c_str(), s.length());
            }

            ps_set_font(gc, of);
            ps_font* of = ps_set_font(gc, uf);

            if (!m_items[i].tags.empty()) {
                ps_get_text_extent(gc, m_items[i].tags.c_str(), m_items[i].tags.length(), &sz);

                if (sz.w <= tip_w - m_items[i].closebtn.w) {
                    ps_wide_text_out_length(gc, m_items[i].rect.w - tip_w - m_items[i].closebtn.w, rc.y + b * 5,
                                            (ps_uchar16*)m_items[i].tags.c_str(), m_items[i].tags.length());
                } else {
                    size_t len = (tip_w - m_items[i].closebtn.w) / (sz.w / m_items[i].tags.length());
                    ustring s = m_items[i].tags.substr(0, len) + ustring(U("..."));
                    ps_wide_text_out_length(gc, m_items[i].rect.w - tip_w - m_items[i].closebtn.w, rc.y + b * 5,
                                            (ps_uchar16*)s.c_str(), s.length());
                }
            }

            ps_color uc = {0, 0, 1, 1};
            ps_set_text_color(gc, &uc);

            ps_get_text_extent(gc, m_items[i].url.c_str(), m_items[i].url.length(), &sz);
            if (sz.w <= (rc.w - 45 * b - m_items[i].closebtn.w)) {
                ps_wide_text_out_length(gc, b * 20, rc.y + b * 8 + b * 12,
                                        (ps_uchar16*)m_items[i].url.c_str(), m_items[i].url.length());
            } else {
                size_t len = (rc.w - 45 * b - m_items[i].closebtn.w) / (sz.w / m_items[i].url.length());
                ustring s = m_items[i].url.substr(0, len) + ustring(U("..."));
                ps_wide_text_out_length(gc, b * 20, rc.y + b * 8 + b * 12, (ps_uchar16*)s.c_str(), s.length());
            }

            draw_icon(gc, &rc);

            draw_closebtn(gc, m_items[i].closebtn, false);
        }
        ps_set_font(gc, of);
    }

    ps_font* of = ps_set_font(gc, uf);

    tr = m_pageup;
    if (tr.intersect(*r)) {
        draw_pagebtn(gc, U("<<Previous"), m_pageup, m_enableup, (m_pagebtn == 1) ? true : false);
    }

    tr = m_pagedown;
    if (tr.intersect(*r)) {
        draw_pagebtn(gc, U("Next>>"), m_pagedown, m_enabledown, (m_pagebtn == 2) ? true : false);
    }

    ps_set_font(gc, of);
}

void FavoritesView::draw_closebtn(ps_context* gc, const Rect& r, bool highlight)
{
    int b = DASH_TITLE_HEIGHT / 30;

    if (highlight) {
        ps_rect rc = {r.x + b, r.y + b, r.w - b, r.h - b};
        ps_color dc = {0.65, 0, 0, 1};
        ps_set_source_color(gc, &dc);
        ps_ellipse(gc, &rc);
        ps_fill(gc);

        ps_color c = {1, 1, 1, 1};
        ps_set_stroke_color(gc, &c);
    } else {
        ps_color c = {0, 0, 0.2, 1};
        ps_set_stroke_color(gc, &c);
    }

    ps_point p1 = {r.x + b * 6, r.y + b * 6};
    ps_point p2 = {r.x + r.w - b * 6, r.y + r.h - b * 6};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_point p3 = {r.x + r.w - b * 6, r.y + b * 6};
    ps_point p4 = {r.x + b * 6, r.y + r.h - b * 6};
    ps_move_to(gc, &p3);
    ps_line_to(gc, &p4);

    ps_set_line_width(gc, b * 2);
    ps_stroke(gc);
    ps_set_line_width(gc, 1);
}

void FavoritesView::draw_pagebtn(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight)
{
    int rds = DASH_TITLE_HEIGHT / 5;
    int b = DASH_TITLE_HEIGHT / 30;
    ps_rect rc = r;
    if (enable) {
        if (highlight) {
            ps_color c = {0, 1, 0, 1};
            ps_set_source_color(gc, &c);
        } else {
            ps_color c = {0, 0.6, 0, 1};
            ps_set_source_color(gc, &c);
        }
    } else {
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_source_color(gc, &c);
    }
    ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_fill(gc);

    ps_color cc = {0, 0, 0, 1};
    ps_set_text_color(gc, &cc);

    size_t len = text.length();

    ps_wide_text_out_length(gc, r.x + r.w / 2 - b * len * 5, r.y + r.h / 2 - b * 6, (ps_uchar16*)text.c_str(), len);

    ps_rect lrc = {r.x + b, r.y + b, r.w - 2 * b, r.h / 2 - 2 * b};
    ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
    ps_matrix_translate(m_impl->t_mtx, lrc.x, lrc.y);
    ps_gradient_transform(m_impl->g_btn, m_impl->t_mtx);
    ps_set_source_gradient(gc, m_impl->g_btn);
    ps_fill(gc);
    ps_matrix_translate(m_impl->t_mtx, -lrc.x * 2, -lrc.y * 2);
    ps_gradient_transform(m_impl->g_btn, m_impl->t_mtx);
    ps_matrix_identity(m_impl->t_mtx);
}

void FavoritesView::initFilter(void)
{
    m_filter = ustring();
}

void FavoritesView::setFilter(const ustring& key)
{
    m_filter = key;
    reset(0);
}

void FavoritesView::build(int offset)
{
    m_items.clear();
    int b = DASH_TITLE_HEIGHT / 30;
    size_t i = 0;
    std::vector<Favorites::Item> items;

    if (m_filter.empty()) {
        items = Application::getInstance()->getFavorites()->getFavoritesItems(m_page * PAGE_ITEMS, PAGE_ITEMS + 1);
    } else {
        items = Application::getInstance()->getFavorites()->searchFavoritesItems(Unicode::ConvertUTF16ToUTF8(m_filter), m_page * PAGE_ITEMS, PAGE_ITEMS + 1);
    }

    for (i = 0; i < items.size(); i++) {
        if (i == PAGE_ITEMS + 1) {
            break;
        }
        FavoritesItem item;
        item.id = items[i].id;
        item.rect = Rect(0, i * TOOLBAR_HEIGHT * 1.2, width() - 1, TOOLBAR_HEIGHT * 1.2);
        item.closebtn = Rect(width() - TOOLBAR_HEIGHT * 0.7,
                             item.rect.y + TOOLBAR_HEIGHT * 0.3, TOOLBAR_HEIGHT * 0.6, TOOLBAR_HEIGHT * 0.6);
        item.title = Unicode::ConvertUTF8ToUTF16(items[i].title);
        item.url = Unicode::ConvertUTF8ToUTF16(items[i].url);
        item.tags = Unicode::ConvertUTF8ToUTF16(items[i].tags);
        m_items.push_back(item);
    }

    int view_height = i * TOOLBAR_HEIGHT * 1.2;
    int hc = DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8;
    int cell_height = DASH_TITLE_HEIGHT * 1.2;
    int border = DASH_TITLE_HEIGHT / 30;
    m_pageup = Rect(border * 10, view_height + (cell_height - hc) / 2, width() / 3 - border * 10, hc);
    m_pagedown = Rect(width() / 3 * 2, view_height + (cell_height - hc) / 2, width() / 3 - border * 10, hc);

    if (offset) {
        m_enableup = true;
    } else {
        m_enableup = false;
    }

    if (items.size() > 20) {
        m_enabledown = true;
    } else {
        m_enabledown = false;
    }

    setContentSize(width(), i * TOOLBAR_HEIGHT * 1.2 + DASH_TITLE_HEIGHT * 1.2);
}

void FavoritesView::reset(void* p)
{
    m_page = 0;
    refresh(p);
}

void FavoritesView::refresh(void* p)
{
    setScrollPoint(0, 0);
    m_scx = 0;
    m_scy = 0;
    build(m_page);
    Update(NULL);
}

void FavoritesView::OnShow(void)
{
    ScrollView::OnShow();
    m_scx = 0;
    m_scy = 0;
    setScrollPoint(0, 0);
    m_page = 0;
    build(m_page);
}

void FavoritesView::OnIdle(void)
{
    ScrollView::OnIdle();
    m_ticket++;
    if (m_ticket > 20) {
        m_ticket = 0;
        m_time++;
    }

    if (m_scx || m_scy) {
        int b = TOOLBAR_HEIGHT / 30;
        scrollContents(-m_scx / 5, -m_scy / 5);
        if (m_scx) { ABS_DEC(m_scx, 10 * b); }
        if (m_scy) { ABS_DEC(m_scy, 10 * b); }
    }
}

void FavoritesView::item_click(void* p)
{
    int i = (intptr_t)p;
    std::string url = Unicode::ConvertUTF16ToUTF8(m_items[i].url);
    m_main->getTabs()->getActiveView()->loadUrl(url);
}

void FavoritesView::OnMouseEvent(const MouseEvent* e)
{
    ScrollView::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        for (size_t i = 0; i < m_items.size(); i++) {
            if (m_items[i].closebtn.contains(e->x() + scrollX(), e->y() + scrollY())) {
                m_cbtn = m_items[i].id;
                m_event = m_items[i].closebtn;
                m_event.x -= scrollX();
                m_event.y -= scrollY();
            } else if (m_items[i].rect.contains(e->x() + scrollX(), e->y() + scrollY())) {
                m_btn = m_items[i].id;
                m_event = m_items[i].rect;
                m_event.x -= scrollX();
                m_event.y -= scrollY();
            }
        }

        if (!m_event.isEmpty() && (m_btn || m_cbtn)) {
            Update(&m_event);
        } else if (m_pageup.contains(e->x() + scrollX(), e->y() + scrollY()) && m_enableup) {
            m_pagebtn = 1;
            m_event = m_pageup;
            m_event.x -= scrollX();
            m_event.y -= scrollY();
            Update(&m_event);
        } else if (m_pagedown.contains(e->x() + scrollX(), e->y() + scrollY()) && m_enabledown) {
            m_pagebtn = 2;
            m_event = m_pagedown;
            m_event.x -= scrollX();
            m_event.y -= scrollY();
            Update(&m_event);
        }

        m_drx = e->x();
        m_dry = e->y();
        m_time = 1;
        m_startX = e->x();
        m_startY = e->y();
        m_capture = true;
    } else if (e->type() == MouseEvent::MouseMove) {
        if (m_capture) {
            int xpos = e->x();
            int ypos = e->y();

            int offsetX = m_startX - xpos;
            int offsetY = m_startY - ypos;

            if (ABS(offsetX) > MAX_DRAG_STEP || ABS(offsetY) > MAX_DRAG_STEP) {
                m_btn = 0;
                m_cbtn = 0;
                m_event = Rect(0, 0, 0, 0);
                scrollContents(offsetX, offsetY);
                m_startX = e->x();
                m_startY = e->y();
                m_drag = true;
            }
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_drag) {
            m_drag = false;
            int b = TOOLBAR_HEIGHT / 30;

            int xs = (e->x() - m_drx) / m_time;
            int ys = (e->y() - m_dry) / m_time;

            if (xs) {
                int vxs = (ABS(xs) - 50 * b > 0) ? (ABS(xs) - 50 * b) : 0;
                m_scx = xs / ABS(xs) * vxs;
            } else {
                m_scx = 0;
            }

            if (ys) {
                int vys = (ABS(ys) - 50 * b > 0) ? (ABS(ys) - 50 * b) : 0;
                m_scy = ys / ABS(ys) * vys;
            } else {
                m_scy = 0;
            }
        } else {

            if (!m_event.isEmpty() && m_btn) {
                int sid = m_btn;
                Update(&m_event);
                m_btn = 0;
                m_event = Rect(0, 0, 0, 0);
                //load the url
                for (size_t i = 0; i < m_items.size(); i++) {
                    if (m_items[i].id == sid) {
                        Application::getInstance()->getFavorites()->increaseCount(sid);
                        Widget::postEvent(this, EVENT_FUNC(FavoritesView, item_click), EVENT_PARAM(i));
                        parent()->Hide();
                        break;
                    }
                }
            } else if (!m_event.isEmpty() && m_cbtn) {
                int sid = m_cbtn;
                Update(&m_event);
                m_cbtn = 0;
                m_event = Rect(0, 0, 0, 0);

                if (Dialog::ConfirmBox(m_main, U("Are you sure you want to delete?"), U("Tips"))) {
                    Application::getInstance()->getFavorites()->deleteFavortesByID(sid);
                    build(m_page);
                    Update(NULL);
                }
            } else if (m_pagebtn) {
                int btn = m_pagebtn;
                m_pagebtn = 0;
                if (btn == 1) {
                    m_page--;
                    Update(&m_event);
                } else if (btn == 2) {
                    m_page++;
                    Update(&m_event);
                }

                m_event = Rect(0, 0, 0, 0);

                Widget::postEvent(this, EVENT_FUNC(FavoritesView, refresh), 0);
            }
        }

        m_drx = 0;
        m_dry = 0;
        m_time = 1;
        m_startX = 0;
        m_startY = 0;
        m_capture = false;
    }
}

//favorites manager
FavoritesManager::FavoritesManager(Widget* parent)
    : ImClient(parent)
    , m_main(0)
    , m_fltbox(0)
    , m_view(0)
    , m_panel(0)
    , m_listbtn(0, 0, 0, 0)
    , m_addbtn(0, 0, 0, 0)
    , m_event(0, 0, 0, 0)
    , m_btn(0)
{
    setTitle(U("Favorites"));
    setCommitText(U("Back"));
    setCancel(false);

    m_fltbox = new LineEdit(this);
    m_view = new FavoritesView(this);
    m_panel = new FavoritesPanel(this, this);
}

FavoritesManager::~FavoritesManager()
{
    delete m_panel;
    delete m_view;
    delete m_fltbox;
}

void FavoritesManager::setMainWindow(MainWindow* main)
{
    m_main = main;
    m_view->setMainWindow(main);
    m_panel->setMainWindow(main);
}

void FavoritesManager::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    ImClient::OnCreate(flags, x, y, w, h);

    int hc = DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8;
    int cell_height = DASH_TITLE_HEIGHT * 1.2;
    int b = DASH_TITLE_HEIGHT / 30;

    m_fltbox->OnCreate(WF_ENABLED | WF_VISIBLE | WF_FOCUSABLE | WF_EDITABLE, b * 5, DASH_TITLE_HEIGHT + (cell_height - hc) / 2 + b * 2, width() / 3 * 2 - 30 * b, hc - b * 5);
    m_fltbox->setTipText(U("Search Bookmarks"));

    m_listbtn = Rect(m_fltbox->width() + 10 * b, DASH_TITLE_HEIGHT + (cell_height - hc) / 2, (width() - m_fltbox->width()) / 2 - 10 * b, hc);
    m_addbtn = Rect(m_listbtn.x + m_listbtn.w + 5 * b, DASH_TITLE_HEIGHT + (cell_height - hc) / 2, (width() - m_fltbox->width()) / 2 - 10 * b, hc);

    m_view->OnCreate(WF_VISIBLE | WF_ENABLED, 0, DASH_TITLE_HEIGHT + cell_height, w, h - DASH_TITLE_HEIGHT * 2 - cell_height);
    m_panel->OnCreate(WF_ENABLED, 0, DASH_TITLE_HEIGHT + cell_height, w, cell_height * 3.5f);

    addChild(m_fltbox);
    addChild(m_view);
    addChild(m_panel);
}

void FavoritesManager::OnShow(void)
{
    ImClient::OnShow();

    m_fltbox->setText(ustring());
    m_view->initFilter();

    m_view->OnShow();
}

void FavoritesManager::hidePanel(void)
{
    if (m_panel->isVisible()) {
        int cell_height = DASH_TITLE_HEIGHT * 1.2;
        m_view->Move(0, DASH_TITLE_HEIGHT + cell_height, width(), height() - DASH_TITLE_HEIGHT * 2 - cell_height);
        m_panel->Hide();
        Update(&m_addbtn);
    }
}

void FavoritesManager::OnHide(void)
{
    ImClient::OnHide();
    hidePanel();

    if (m_fltbox->hasFocus()) {
        m_fltbox->setFocus(false);
    }
}

void FavoritesManager::OnMouseEvent(const MouseEvent* e)
{
    ImClient::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_listbtn.contains(e->x(), e->y())) {
            m_event = m_listbtn;
            m_btn = 1;
        } else if (m_addbtn.contains(e->x(), e->y())) {
            m_event = m_addbtn;
            m_btn = 2;
        } else {
            m_event = Rect(0, 0, 0, 0);
            m_btn = 0;
        }

        if (m_btn) {
            Update(&m_event);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {

            if (m_btn == 2) {
                int cell_height = DASH_TITLE_HEIGHT * 1.2;
                if (!m_panel->isVisible()) {
                    m_view->Move(0, DASH_TITLE_HEIGHT + cell_height + m_panel->height(), width(),
                                 height() - DASH_TITLE_HEIGHT * 2 - cell_height - m_panel->height());
                    m_panel->Show();
                }
            } else if (m_btn == 1) {
                m_view->setFilter(m_fltbox->getText());
            }

            m_btn = 0;
            Update(&m_event);
            m_event = Rect(0, 0, 0, 0);
        }
    }
}

void FavoritesManager::OnPaint(ps_context* gc, const Rect* r)
{
    ImClient::OnPaint(gc, r);

    ps_save(gc);

    Rect tr;
    int cell_height = DASH_TITLE_HEIGHT * 1.2;
    ps_rect rc = {0, DASH_TITLE_HEIGHT, width(), cell_height};

    tr = rc;
    if (tr.intersect(*r)) {
        ps_rectangle(gc, &rc);
        ps_color c = {0.8, 0.8, 0.8, 1};
        ps_set_source_color(gc, &c);
        ps_fill(gc);
    }

    ps_font* f = MainWindow::getUIFont();
    ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    tr = m_listbtn;
    if (tr.intersect(*r)) {
        draw_button(gc, m_listbtn, U("Search"), true, (m_btn == 1) ? true : false);
    }

    tr = m_addbtn;
    if (tr.intersect(*r)) {
        bool click = (m_btn == 2) ? true : false;
        if (m_panel->isVisible()) {
            click = true;
        }
        draw_button(gc, m_addbtn, U("Add"), true, click);
    }
    ps_set_font(gc, of);
    ps_restore(gc);
}

static ps_gradient* g_btn_grad = 0;
static ps_matrix* g_mtx = 0;
static void draw_button(ps_context* gc, const Rect& r, const ustring& text, bool enable, bool light)
{
    int rds = DASH_TITLE_HEIGHT / 5;
    int b = DASH_TITLE_HEIGHT / 30;
    int bc = DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8;
    ps_rect rc = r;
    if (enable) {
        if (light) {
            ps_color c = {0.3, 0.8, 1, 1};
            ps_set_source_color(gc, &c);
        } else {
            ps_color c = {0, 0.5, 1, 1};
            ps_set_source_color(gc, &c);
        }
    } else {
        ps_color c = {0.5, 0.5, 0.5, 1};
        ps_set_source_color(gc, &c);
    }
    ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_fill(gc);

    ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 4 * text.size(), r.y + r.h / 2 - b * 12, (ps_uchar16*)text.c_str(), text.length());

    ps_rect lrc = {r.x + b, r.y + b, r.w - 2 * b, r.h / 2 - 2 * b};
    ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
    if (!g_btn_grad) {
        ps_color tc = {1, 1, 1, 0.8};
        ps_color tc2 = {1, 1, 1, 0.2};
        ps_point s1 = {0, 0};
        ps_point e1 = {0, bc / 2 - 2 * b};
        g_mtx = ps_matrix_create();
        g_btn_grad = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
        ps_gradient_add_color_stop(g_btn_grad, 0, &tc);
        ps_gradient_add_color_stop(g_btn_grad, 1, &tc2);
    }

    ps_matrix_translate(g_mtx, lrc.x, lrc.y);
    ps_gradient_transform(g_btn_grad, g_mtx);
    ps_set_source_gradient(gc, g_btn_grad);
    ps_fill(gc);
    ps_matrix_translate(g_mtx, -lrc.x * 2, -lrc.y * 2);
    ps_gradient_transform(g_btn_grad, g_mtx);
    ps_matrix_identity(g_mtx);
}
