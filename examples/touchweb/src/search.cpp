/* search.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "search.h"
#include "edit.h"
#include "engine.h"
#include "tabpage.h"
#include "webview.h"
#include "unicode.h"
#include "mainwindow.h"
#include "application.h"

Search::Search(Widget* parent)
    : ImClient(parent)
    , m_main(0)
    , m_words(0)
    , m_sebtn(0, 0, 0, 0)
    , m_event(0, 0, 0, 0)
    , m_btn(0)
    , m_ebtn(0)
{
    setTitle(U("ËÑË÷"));
    setCommitText(U("·µ»Ø"));
    setCancel(false);

    m_words = new LineEdit(this);
}

Search::~Search()
{
    delete m_words;
}

void Search::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void Search::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    ImClient::OnCreate(flags, x, y, w, h);

    int b = DASH_TITLE_HEIGHT / 30;
    std::vector<Engine::Item> items = Application::getInstance()->getSearchEngine()->getEngines();
    size_t n = items.size();
    size_t m = (width() / n - 45 * b) / 2;
    for (size_t i = 0; i < n; i++) {
        SeItem item;
        item.id = items[i].id;
        item.name = items[i].name;
        item.rect = Rect(m + i * width() / n, DASH_TITLE_HEIGHT * 2 + 30 * b, 45 * b, 45 * b);
        item.icon = items[i].img;
        item.w = items[i].w;
        item.h = items[i].h;
        item.p = items[i].p;
        item.c = items[i].c;
        item.page_fold = items[i].page_fold;
        m_items.push_back(item);
    }

    m_sebtn = Rect(width() - 50 * b, DASH_TITLE_HEIGHT + b * 20, 45 * b, DASH_TITLE_HEIGHT - b * 4);
    m_words->OnCreate(WF_VISIBLE | WF_ENABLED | WF_FOCUSABLE | WF_EDITABLE, b * 5,
                      DASH_TITLE_HEIGHT + b * 20, width() - b * 12 - m_sebtn.w, DASH_TITLE_HEIGHT - b * 4);

    m_words->setHasBorder(true);

    addChild(m_words);
}

void Search::draw_engine_btns(ps_context* gc)
{
    int rds = DASH_TITLE_HEIGHT / 5;
    int b = DASH_TITLE_HEIGHT / 30;
    for (size_t i = 0; i < m_items.size(); i++) {
        ps_rect r = m_items[i].rect;
        if (m_ebtn && m_items[i].id == (m_ebtn - 1)) {
            ps_color c = {0.3, 0.8, 1, 1};
            ps_set_source_color(gc, &c);
        } else {
            if (m_items[i].id == Application::getInstance()->getSearchEngine()->getDefaultId()) {
                ps_set_source_color(gc, &m_items[i].c);
            } else {
                ps_color c = {0.6, 0.6, 0.6, 1};
                ps_set_source_color(gc, &c);
            }
        }
        ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_fill(gc);

        ps_image* pm = ps_image_create_with_data(m_items[i].icon, Application::getInstance()->color_format(),
                                                 m_items[i].w, m_items[i].h, m_items[i].p);
        ps_image_set_allow_transparent(pm, True);
        ps_rect mr = {r.x + (r.w - 16 * b) / 2, r.y + 6 * b, 16 * b, 16 * b};
        ps_rectangle(gc, &mr);
        ps_set_source_image(gc, pm);
        ps_fill(gc);
        ps_image_unref(pm);

        ps_size sz = {0};
        ps_get_text_extent(gc, m_items[i].name.c_str(), m_items[i].name.length(), &sz);
        ps_wide_text_out_length(gc, r.x + r.w / 2 - sz.w / 2, r.y + r.h - b * 18,
                                (ps_uchar16*)m_items[i].name.c_str(), m_items[i].name.length());

        ps_color tc = {1, 1, 1, 0.8};
        ps_color tc2 = {1, 1, 1, 0.2};
        ps_rect lrc = {r.x + b, r.y + b, r.w - 2 * b, r.h / 2 - 2 * b};
        ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
        ps_point s1 = {lrc.x, lrc.y};
        ps_point e1 = {lrc.x, lrc.y + lrc.h};
        ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
        ps_gradient_add_color_stop(gt, 0, &tc);
        ps_gradient_add_color_stop(gt, 1, &tc2);
        ps_set_source_gradient(gc, gt);
        ps_fill(gc);
        ps_gradient_unref(gt);
    }
}

void Search::OnPaint(ps_context* gc, const Rect* d)
{
    ImClient::OnPaint(gc, d);

    ps_save(gc);

    ps_font* f = MainWindow::getUIFont();
#ifdef FONT_NO_ANTIALIAS
    ps_set_text_antialias(gc, False);
#else
    if (MainWindow::uiFontAntialias()) {
        ps_set_text_antialias(gc, True);
    } else {
        ps_set_text_antialias(gc, False);
    }
#endif

    ps_font* of = ps_set_font(gc, f);

    draw_sebtn(gc, U("ËÑË÷"), m_sebtn, true, m_btn ? true : false);

    draw_engine_btns(gc);

    ps_set_font(gc, of);
    ps_restore(gc);
}

void Search::draw_sebtn(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight)
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

    ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 12, r.y + r.h / 2 - b * 6, (ps_uchar16*)text.c_str(), text.length());

    ps_color tc = {1, 1, 1, 0.8};
    ps_color tc2 = {1, 1, 1, 0.2};
    ps_rect lrc = {r.x + b, r.y + b, r.w - 2 * b, r.h / 2 - 2 * b};
    ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
    ps_point s1 = {lrc.x, lrc.y};
    ps_point e1 = {lrc.x, lrc.y + lrc.h};
    ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
    ps_gradient_add_color_stop(gt, 0, &tc);
    ps_gradient_add_color_stop(gt, 1, &tc2);
    ps_set_source_gradient(gc, gt);
    ps_fill(gc);
    ps_gradient_unref(gt);
}

void Search::OnShow(void)
{
    ImClient::OnShow();
    m_words->setTipText(Application::getInstance()->getSearchEngine()->getDefaultName());
    m_words->setText(ustring());
}

void Search::OnHide(void)
{
    ImClient::OnHide();
    if (m_words->hasFocus()) {
        m_words->setFocus(false);
    }
}

void Search::OnMouseEvent(const MouseEvent* e)
{
    ImClient::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        for (size_t i = 0; i < m_items.size(); i++) {
            if (m_items[i].rect.contains(e->x(), e->y())) {
                m_ebtn = m_items[i].id + 1;
                m_event = m_items[i].rect;
            }
        }

        if (!m_ebtn && m_sebtn.contains(e->x(), e->y())) {
            m_btn = 1;
            m_event = m_sebtn;
        }

        if (m_btn || m_ebtn) {
            Update(&m_event);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn || m_ebtn) {
            int sd = m_btn;
            int ed = m_ebtn;

            m_btn = 0;
            m_ebtn = 0;

            if (sd) {
                Update(&m_event);
            }

            if (sd) {
                Widget::postEvent(this, EVENT_FUNC(Search, button_search), EVENT_PARAM(sd));
            } else if (ed) {
                Widget::postEvent(this, EVENT_FUNC(Search, button_set), EVENT_PARAM(ed));
            }

            m_event = Rect(0, 0, 0, 0);
        }
    }
}

void Search::button_search(void* param)
{
    ustring str = m_words->getText();
    if (!str.empty()) {
        bool fold;
        std::string query = Application::getInstance()->getSearchEngine()->searchByDefault(Unicode::ConvertUTF16ToUTF8(str), &fold);
        if (!query.empty()) {
            if (m_main->getTabs()->getCount() == MAX_TABS) {
                m_main->getTabs()->getActiveView()->loadUrl(query);
            } else {
                m_main->getTabs()->newView(query);
            }
            if (fold) {
                m_main->getTabs()->getActiveView()->setRenderMode(WebView::Render_SmartFit);
            }
            Hide();
        }
    }
}

void Search::button_set(void* param)
{
    int ed = (intptr_t)param;
    if (ed) {
        Application::getInstance()->getSearchEngine()->setDefaultById(ed - 1);
        m_words->setTipText(Application::getInstance()->getSearchEngine()->getDefaultName());
        m_words->Update(NULL);
        Update(NULL);
    }
}
