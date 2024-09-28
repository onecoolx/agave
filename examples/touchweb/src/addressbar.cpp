/* addressbar.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "addressbar.h"
#include "tabpage.h"
#include "webview.h"
#include "url.h"
#include "engine.h"
#include "unicode.h"
#include "mainwindow.h"
#include "application.h"

#if defined(WIN32)
    #include "application_win32.h"
#endif

#ifdef GTK2
    #include "application_gtk2.h"
#endif

NavButton::NavButton(Widget* parent)
    : Widget(parent)
    , m_main(0)
    , m_type(0)
    , m_click(0)
{
}

NavButton::~NavButton()
{
}

int NavButton::buttonType(void)
{
    return m_type;
}

void NavButton::setButtonType(int t)
{
    if (m_type != t) {
        m_type = t;
        Update(NULL);
    }
}

void NavButton::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void NavButton::draw_navbtn(ps_context* gc, ps_rect& r)
{
    int b = TOOLBAR_HEIGHT / 30;
    ps_color c = {0, 0.8, 0, 1};
    ps_set_source_color(gc, &c);

    ps_point p1 = {r.x + r.w / 3 + TOOLBAR_HEIGHT / 3, r.y + r.h / 2};
    ps_point p2 = {r.x + r.w / 3, r.y + b * 2};
    ps_point p3 = {r.x + r.w / 3, r.y + r.h - b * 2};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_line_to(gc, &p3);
    ps_line_to(gc, &p1);

    ps_fill(gc);
}

void NavButton::draw_stopbtn(ps_context* gc, ps_rect& r)
{
    int b = TOOLBAR_HEIGHT / 30;
    ps_color c = {0.65, 0, 0, 1};
    ps_set_stroke_color(gc, &c);
    ps_set_line_width(gc, 4 * b);

    ps_point p1 = {r.x + r.w / 2 - r.w / 4, r.y + r.h / 2 - r.w / 4};
    ps_point p2 = {r.x + r.w / 2 + r.w / 4, r.y + r.h / 2 + r.w / 4};
    ps_point p3 = {r.x + r.w / 2 + r.w / 4, r.y + r.h / 2 - r.w / 4};
    ps_point p4 = {r.x + r.w / 2 - r.w / 4, r.y + r.h / 2 + r.w / 4};

    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_move_to(gc, &p3);
    ps_line_to(gc, &p4);

    ps_stroke(gc);
}

void NavButton::draw_reloadbtn(ps_context* gc, ps_rect& r)
{
    int b = TOOLBAR_HEIGHT / 15;
    ps_color c = {1, 1, 1, 1};
    ps_set_source_color(gc, &c);
    ps_rect ry = {r.x + r.w / 2 - TOOLBAR_HEIGHT / 10 * 3.0f, r.y + b, TOOLBAR_HEIGHT / 5 * 3.0f, TOOLBAR_HEIGHT / 5 * 3.0f};
    ps_rect rn = {r.x + r.w / 2 - b * 2.2f, r.y + b * 3.5f, TOOLBAR_HEIGHT / 10 * 3.0f, TOOLBAR_HEIGHT / 10 * 3.0f};
    ps_tangent_arc(gc, &ry, -1.57f, 4.71f);
    ps_tangent_arc(gc, &rn, 3.14f, -4.71f);
    ps_new_sub_path(gc);
    ps_point p1 = {r.x + r.w / 2 - b * 2, r.y + TOOLBAR_HEIGHT / 6};
    ps_point p2 = {r.x + r.w / 2, r.y};
    ps_point p3 = {r.x + r.w / 2, r.y + b * 4.4f};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_line_to(gc, &p3);
    ps_line_to(gc, &p1);
    ps_set_fill_rule(gc, FILL_RULE_EVEN_ODD);
    ps_fill(gc);
}

void NavButton::OnPaint(ps_context* gc, const Rect* d)
{
    Widget::OnPaint(gc, d);

    ps_save(gc);

    if (m_click) {
        int rds = TOOLBAR_HEIGHT / 10;
        ps_color cb = {0.56f, 0.56f, 0.56f, 1};
        ps_rect rb = {rds / 3.0f, rds / 3.0f, width() - rds, height() - rds};
        ps_set_line_width(gc, TOOLBAR_HEIGHT / 30);
        ps_set_stroke_color(gc, &cb);
        ps_color c1 = {0.95, 0.95, 0.95, 1};
        ps_color c2 = {0.30, 0.30, 0.30, 1};
        ps_point s = {rb.x, rb.y};
        ps_point e = {rb.x, rb.y + rb.h};
        ps_rounded_rect(gc, &rb, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
        ps_gradient_add_color_stop(g, 0, &c1);
        ps_gradient_add_color_stop(g, 1, &c2);
        ps_set_source_gradient(gc, g);
        ps_paint(gc);
        ps_gradient_unref(g);
        ps_set_line_width(gc, 1);
    }

    int b = TOOLBAR_HEIGHT / 30;
    ps_rect r = {b, b, width() - b * 2, height() - b * 2};
    if (m_type == LoadBtn) { //load button
        draw_navbtn(gc, r);
    } else if (m_type == StopBtn) { //stop button
        draw_stopbtn(gc, r);
    } else { //reload btn
        draw_reloadbtn(gc, r);
    }

    ps_restore(gc);
}

void NavButton::button_event(void* param)
{
    int type = (intptr_t)param;
    if (type == LoadBtn) {
        //load url
        m_main->getAddressBar()->loadUrl();
    } else if (type == StopBtn) {
        //stop load
        m_main->getTabs()->getActiveView()->stopLoad();
    } else {
        // reload
        m_main->getTabs()->getActiveView()->reLoad();
    }
}

void NavButton::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);
    if (e->type() == MouseEvent::MouseDown) {
        m_click = 1;
        Update(NULL);
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_click) {
            m_click = 0;
            Update(NULL);

            Widget::postEvent(this, EVENT_FUNC(NavButton, button_event), EVENT_PARAM(m_type), true);
        }
    }
}

//AddressBar
AddressBar::AddressBar(Widget* parent)
    : Widget(parent)
    , m_main(0)
    , m_btn(0)
    , m_loadpr(0)
    , m_clear(0, 0, 0, 0)
    , m_clsbtn(0)
    , m_select(false)
    , m_capture(false)
    , m_update(false)
    , m_sx(0)
    , m_sy(0)
{
    m_btn = new NavButton(this);
    m_tip = ustring(U("Enter url or search keyword"));
}

AddressBar::~AddressBar()
{
    delete m_btn;
}

static void draw_clear_btn(ps_context* gc, const Rect& r, bool highlight)
{
    int b = DASH_TITLE_HEIGHT / 30;

    if (highlight) {
        ps_color c = {0.05, 0.18, 0.35, 1};
        ps_set_stroke_color(gc, &c);
    } else {
        ps_color c = {0.18, 0.49, 0.87, 1};
        ps_set_stroke_color(gc, &c);
    }

    ps_point p1 = {r.x + b * 4, r.y + b * 4};
    ps_point p2 = {r.x + r.w - b * 4, r.y + r.h - b * 4};
    ps_move_to(gc, &p1);
    ps_line_to(gc, &p2);
    ps_point p3 = {r.x + r.w - b * 4, r.y + b * 4};
    ps_point p4 = {r.x + b * 4, r.y + r.h - b * 4};
    ps_move_to(gc, &p3);
    ps_line_to(gc, &p4);

    ps_set_line_width(gc, b * 2);
    ps_stroke(gc);
    ps_set_line_width(gc, 1);
}

//Event Handler
void AddressBar::OnPaint(ps_context* gc, const Rect* d)
{
    Widget::OnPaint(gc, d);
    //TODO: add paint code here.
    ps_save(gc);

    ps_color c = {0.1, 0.1, 0.1, 1};
    ps_rect r = {0, 0, width(), height()};
    ps_rectangle(gc, &r);
    ps_set_source_color(gc, &c);
    ps_fill(gc);

    int rds = TOOLBAR_HEIGHT / 15;
    int btnw = TOOLBAR_HEIGHT - TOOLBAR_HEIGHT / 5;
    int box_len = width() - TOOLBAR_HEIGHT / 2 - btnw;

    unsigned int m_pos = 0;
    unsigned int m_cur = 0;
    size_t m_len = 0;

    ps_color tc = {1, 1, 1, 1};
    ps_rect tr = {TOOLBAR_HEIGHT / 6, TOOLBAR_HEIGHT / 6, width() - TOOLBAR_HEIGHT / 3 - btnw, height() - TOOLBAR_HEIGHT / 3};
    ps_rounded_rect(gc, &tr, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_set_source_color(gc, &tc);
    ps_fill(gc);

    if (!m_text.empty()) {
        box_len -= m_clear.w;
        draw_clear_btn(gc, m_clear, m_clsbtn ? true : false);
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

    ps_size sz = {0};
    ps_get_text_extent(gc, m_text.c_str(), m_text.length(), &sz);
    if (sz.w <= box_len) {
        m_cur = sz.w;
        m_len = m_text.length();
        m_pos = 0;
    } else {
        m_cur = box_len;
        m_len = m_text.length() * box_len / sz.w;
        m_pos = m_text.length() - m_len;
    }

    //draw progress
    if (m_main->getTabs()->getActiveView()->loading()) {
        double pg = m_main->getTabs()->getActiveView()->loading();
        ps_rect pr = tr;
        pr.w = pr.w * pg / 100;
        ps_rounded_rect(gc, &pr, rds, rds, rds, rds, rds, rds, rds, rds);

        ps_set_source_gradient(gc, m_loadpr);
        ps_fill(gc);
    }

    //draw focus
    if (hasFocus()) {
        ps_color fc = {0.08, 0.45, 1, 1};
        ps_path* p = ps_path_create();
        ps_path_add_rounded_rect(p, &tr, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_set_path(gc, p);
        ps_set_line_width(gc, rds);
        ps_set_stroke_color(gc, &fc);
        ps_stroke(gc);
        ps_path_unref(p);

        ps_color cs = {0, 0, 0, 1};
        ps_set_stroke_color(gc, &cs);
        ps_set_line_width(gc, 1);

        ps_point p1 = {m_cur + rds * 4, rds * 4};
        ps_point p2 = {m_cur + rds * 4, rds * 11};
        ps_move_to(gc, &p1);
        ps_line_to(gc, &p2);
        ps_stroke(gc);
    }

    //draw text
    if (!m_text.empty()) {
        if (m_select) {
            int sw = MIN((tr.w - tr.x - m_clear.w), sz.w);
            ps_rect sr = {tr.x + 2 * rds, tr.y + rds, sw, sz.h + 2 * rds};
            ps_color sc = {0.75, 0.75, 0.75, 1};
            ps_set_source_color(gc, &sc);
            ps_rectangle(gc, &sr);
            ps_fill(gc);

            ps_color tic = {1, 1, 1, 1};
            ps_set_text_color(gc, &tic);
        }
        ps_save(gc);

        Rect ctr(tr);
        clipRect(gc, ctr);

        if (hasFocus()) {
            ps_wide_text_out_length(gc, rds * 4, rds * 4, (ps_uchar16*)(m_text.c_str() + m_pos), m_len);
        } else {
            ps_wide_text_out_length(gc, rds * 4, rds * 4, (ps_uchar16*)m_text.c_str(), m_len);
        }

        ps_restore(gc);
    }

    if (!hasFocus() && m_text.empty()) {
        ps_color tic = {0.75, 0.75, 0.68, 1};
        ps_set_text_color(gc, &tic);
        ps_wide_text_out_length(gc, rds * 4, rds * 4, (ps_uchar16*)m_tip.c_str(), m_tip.length());
    }

    ps_set_font(gc, of);
    ps_restore(gc);
}

void AddressBar::setLoading(bool b)
{
    m_btn->setButtonType(b ? NavButton::StopBtn : (isNewUrl() ? NavButton::LoadBtn : NavButton::ReloadBtn));
    m_update = true;
}

void AddressBar::setText(const ustring& text)
{
    m_text = text;
    Update(NULL);
}

void AddressBar::setUpdate(void)
{
    m_update = true;
}

void AddressBar::load_url(void* param)
{
    loadUrl();
}

void AddressBar::OnIdle(void)
{
    Widget::OnIdle();

    if (m_update) {
        m_update = false;

        if (m_main->getTabs()->getActiveView()->loading()) {
            m_btn->setButtonType(NavButton::StopBtn);
        } else {
            m_btn->setButtonType(isNewUrl() ? NavButton::LoadBtn : NavButton::ReloadBtn);
        }

        Update(NULL);
    }
}

void AddressBar::OnChar(uchar_t code)
{
    Widget::OnChar(code);

    if (IS_PRINT(code)) {
        if (m_select) {
            m_select = false;
            m_text = ustring();
        }
        setNewUrl(true);
        m_text += code;
        Update(NULL);
    } else if (code == 0x08) {
        if (m_text.length()) {
            setNewUrl(true);
            if (m_select) {
                m_select = false;
                m_text = ustring();
            } else {
                m_text.erase(m_text.end() - 1);
            }
            Update(NULL);
        }
    } else if (code == 0x0D) {
        if (m_text.length()) {
            Widget::postEvent(this, EVENT_FUNC(AddressBar, load_url), EVENT_PARAM(0), true);
        }
    }
}

void AddressBar::OnSetFocus(void)
{
    Widget::OnSetFocus();
    Update(NULL);
    SetImeStatus(true);
}

void AddressBar::OnKillFocus(void)
{
    Widget::OnKillFocus();
    m_select = false;
    Update(NULL);
    SetImeStatus(false);
}

void AddressBar::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(f, x, y, w, h);

    int b = TOOLBAR_HEIGHT / 30;
    int bh = h - b * 14;
    int btnw = TOOLBAR_HEIGHT - TOOLBAR_HEIGHT / 15;

    m_clear = Rect(w - bh - b * 2 - btnw, b * 7, bh, bh);
    m_btn->OnCreate(WF_ENABLED | WF_VISIBLE, w - btnw, TOOLBAR_HEIGHT / 10, btnw, height() - TOOLBAR_HEIGHT / 5);

    addChild(m_btn);

    int bw = TOOLBAR_HEIGHT - TOOLBAR_HEIGHT / 5;
    ps_rect tr = {TOOLBAR_HEIGHT / 6, TOOLBAR_HEIGHT / 6, width() - TOOLBAR_HEIGHT / 3 - bw, height() - TOOLBAR_HEIGHT / 3};
    ps_color c1 = {0.9, 0.95, 0.99, 1};
    ps_color c2 = {0.57, 0.83, 0.9, 1};
    ps_point s = {tr.x, tr.y};
    ps_point e = {tr.x, tr.y + tr.h / 2};
    m_loadpr = ps_gradient_create_linear(GRADIENT_SPREAD_REFLECT, &s, &e);
    ps_gradient_add_color_stop(m_loadpr, 0, &c1);
    ps_gradient_add_color_stop(m_loadpr, 1, &c2);
}

void AddressBar::OnDestroy(void)
{
    Widget::OnDestroy();

    if (m_loadpr) {
        ps_gradient_unref(m_loadpr);
        m_loadpr = 0;
    }
}

void AddressBar::setMainWindow(MainWindow* main)
{
    m_main = main;
    m_btn->setMainWindow(main);
}

bool AddressBar::isNewUrl(void)
{
    return m_main->getTabs()->getActiveView()->isNewUrl();
}

void AddressBar::setNewUrl(bool b)
{
    m_btn->setButtonType(b ? NavButton::LoadBtn : NavButton::ReloadBtn);
    m_main->getTabs()->getActiveView()->setNewUrl(b);
}

void AddressBar::loadUrl(void)
{
    setFocus(false);

    std::string url = Unicode::ConvertUTF16ToUTF8(m_text);

    if (urlIsValid(url)) {
        if (!isNewUrl()) {
            m_main->getTabs()->getActiveView()->reLoad();
        } else {
            m_main->getTabs()->getActiveView()->loadUrl(url);
        }
    } else {
        std::string query = Application::getInstance()->getSearchEngine()->searchByDefault(url);
        m_main->getTabs()->getActiveView()->loadUrl(query);
    }
}

void AddressBar::OnMouseEvent(const MouseEvent* e)
{
    Widget::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_clear.contains(e->x(), e->y())) {
            m_clsbtn = 1;
        } else {
            m_clsbtn = 0;
            m_capture = true;
            if (m_select) {
                Update(NULL);
            }

            m_select = false;
            m_sx = e->x();
            m_sy = e->y();
        }

        if (m_clsbtn) {
            Update(&m_clear);
        }
    } else if (e->type() == MouseEvent::MouseMove) {
        if (m_capture) {
            int xpos = e->x();
            int ypos = e->y();
            int offsetX = m_sx - xpos;
            int offsetY = m_sy - ypos;
            if ((ABS(offsetX) > MAX_DRAG_STEP || ABS(offsetY) > MAX_DRAG_STEP) && !m_select) {
                if (!ImeIsShow()) {
                    SetImeStatus(true);
                }
                m_select = true;
                m_capture = false;
                Update(NULL);
            }
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_clsbtn) {
            m_clsbtn = 0;
            m_text = ustring();
            Update(NULL);
        } else {
            m_capture = false;
            m_sx = 0;
            m_sy = 0;
        }
    }
}
