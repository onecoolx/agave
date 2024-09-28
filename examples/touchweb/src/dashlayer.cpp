/* Dashlayer.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "dashlayer.h"
#include "mainwindow.h"

#define LIGHT_GRADIENT 0
#define BAR_GRADIENT 1
#define COMMIT_GRADIENT 2
#define CANCEL_GRADIENT 3

ps_font* DashLayer::tfont = 0;
ps_gradient* DashLayer::grads[4] = {0};
ps_matrix* DashLayer::mtx = 0;
bool DashLayer::init = false;

DashLayer::DashLayer(Widget* parent)
    : Layer(parent)
    , m_commit(0, 0, 0, 0)
    , m_cancel(0, 0, 0, 0)
    , m_event(0, 0, 0, 0)
    , m_scombtn(true)
    , m_scanbtn(true)
    , m_btn(0)
{
    setCommitText(U("Ok"));
    setCancelText(U("Cancel"));
}

DashLayer::~DashLayer()
{
}

void DashLayer::setTitle(const ustring& title)
{
    m_title = title;
}

void DashLayer::setCommitText(const ustring& text)
{
    m_comText = text;
}

void DashLayer::setCancelText(const ustring& text)
{
    m_canText = text;
}

void DashLayer::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{
    if (!text.empty()) {
        //draw title
        ps_color c = {1, 1, 1, 1};
        ps_set_text_color(gc, &c);
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
        ps_get_text_extent(gc, text.c_str(), text.length(), &sz);

        ps_wide_text_out_length(gc, r.x + (r.w - sz.w) / 2, r.y + r.h / 2 - DASH_TITLE_HEIGHT / 5,
                                (ps_uchar16*)text.c_str(), text.length());

        ps_set_font(gc, of);
    }
}

void DashLayer::draw_lights(ps_context* gc, const Rect& r)
{
    int b = DASH_TITLE_HEIGHT / 30;
    int rds = DASH_TITLE_HEIGHT / 10;
    ps_rect lrc = {r.x + 2 * b, r.y + 2 * b, r.w - 3 * b, r.h / 2 - 2 * b};
    ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);

    ps_matrix_translate(DashLayer::mtx, lrc.x, lrc.y);
    ps_gradient_transform(DashLayer::grads[LIGHT_GRADIENT], DashLayer::mtx);
    ps_set_source_gradient(gc, DashLayer::grads[LIGHT_GRADIENT]);
    ps_fill(gc);
    ps_matrix_translate(DashLayer::mtx, -lrc.x * 2, -lrc.y * 2);
    ps_gradient_transform(DashLayer::grads[LIGHT_GRADIENT], DashLayer::mtx);
    ps_matrix_identity(DashLayer::mtx);
}

void DashLayer::draw_btns(ps_context* gc, const Rect& rc)
{
    int b = DASH_TITLE_HEIGHT / 30;
    int rds = DASH_TITLE_HEIGHT / 10;
    ps_color c = {0.5, 0.5, 0.5, 1};
    ps_set_stroke_color(gc, &c);

    Rect rt = rc;

    if (!m_commit.isEmpty() && rt.intersect(m_commit)) {
        ps_rect r = m_commit;
        ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_set_line_width(gc, b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 1) {
            ps_rect r2 = {r.x + b, r.y + b, r.w - b, r.h - b};
            ps_rounded_rect(gc, &r2, rds, rds, rds, rds, rds, rds, rds, rds);
            ps_matrix_translate(DashLayer::mtx, r.x, r.y);
            ps_gradient_transform(DashLayer::grads[COMMIT_GRADIENT], DashLayer::mtx);
            ps_set_source_gradient(gc, DashLayer::grads[COMMIT_GRADIENT]);
            ps_fill(gc);
            ps_matrix_translate(DashLayer::mtx, -r.x * 2, -(r.y) * 2);
            ps_gradient_transform(DashLayer::grads[COMMIT_GRADIENT], DashLayer::mtx);
            ps_matrix_identity(DashLayer::mtx);
        }

        draw_btnText(gc, m_comText, m_commit);
        draw_lights(gc, m_commit);
    }

    rt = rc;
    if (!m_cancel.isEmpty() && rt.intersect(m_cancel)) {
        ps_rect r = m_cancel;
        ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
        ps_set_line_width(gc, b);
        ps_stroke(gc);
        ps_set_line_width(gc, 1);

        if (m_btn == 2) {
            ps_rect r2 = {r.x + b, r.y + b, r.w - b, r.h - b};
            ps_rounded_rect(gc, &r2, rds, rds, rds, rds, rds, rds, rds, rds);
            ps_matrix_translate(DashLayer::mtx, r.x, r.y);
            ps_gradient_transform(DashLayer::grads[CANCEL_GRADIENT], DashLayer::mtx);
            ps_set_source_gradient(gc, DashLayer::grads[CANCEL_GRADIENT]);
            ps_fill(gc);
            ps_matrix_translate(DashLayer::mtx, -r.x * 2, -(r.y) * 2);
            ps_gradient_transform(DashLayer::grads[CANCEL_GRADIENT], DashLayer::mtx);
            ps_matrix_identity(DashLayer::mtx);
        }

        draw_btnText(gc, m_canText, m_cancel);
        draw_lights(gc, m_cancel);
    }
}

void DashLayer::draw_title(ps_context* gc)
{
    if (!m_title.empty()) {
        //draw title
        ps_color c = {1, 1, 1, 1};
        ps_set_text_color(gc, &c);
        ps_font* tf = DashLayer::getTitleFont();
        ps_font* of = ps_set_font(gc, tf);
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
        ps_get_text_extent(gc, m_title.c_str(), m_title.length(), &sz);

        ps_wide_text_out_length(gc, (width() - sz.w) / 2,
                                DASH_TITLE_HEIGHT / 2 - DASH_TITLE_HEIGHT / 30 * 7, (ps_uchar16*)m_title.c_str(), m_title.length());

        ps_set_font(gc, of);
    }
}

void DashLayer::OnPaint(ps_context* gc, const Rect* r)
{
    Layer::OnPaint(gc, r);
    //add paint code here
    ps_save(gc);

    Rect xr = *r;
    ps_rect cr = {0, DASH_TITLE_HEIGHT, width(), height() - DASH_TITLE_HEIGHT * 2};
    if (xr.intersect(cr)) {
        ps_color c = {1, 1, 1, 1};
        ps_rectangle(gc, &cr);
        ps_set_source_color(gc, &c);
        ps_fill(gc);
    }

    xr = *r;
    ps_rect tr = {0, 0, width(), DASH_TITLE_HEIGHT};
    if (xr.intersect(tr)) {
        ps_matrix_translate(DashLayer::mtx, tr.x, tr.y);
        ps_gradient_transform(DashLayer::grads[BAR_GRADIENT], DashLayer::mtx);
        ps_set_source_gradient(gc, DashLayer::grads[BAR_GRADIENT]);
        ps_rectangle(gc, &tr);
        ps_fill(gc);
        ps_matrix_translate(DashLayer::mtx, -tr.x * 2, -tr.y * 2);
        ps_gradient_transform(DashLayer::grads[BAR_GRADIENT], DashLayer::mtx);
        ps_matrix_identity(DashLayer::mtx);

        draw_title(gc);
    }

    xr = *r;
    ps_rect br = {0, cr.h + tr.h, width(), DASH_TITLE_HEIGHT};
    if (xr.intersect(br)) {
        ps_matrix_translate(DashLayer::mtx, br.x, br.y);
        ps_gradient_transform(DashLayer::grads[BAR_GRADIENT], DashLayer::mtx);
        ps_set_source_gradient(gc, DashLayer::grads[BAR_GRADIENT]);
        ps_rectangle(gc, &br);
        ps_fill(gc);
        ps_matrix_translate(DashLayer::mtx, -br.x * 2, -br.y * 2);
        ps_gradient_transform(DashLayer::grads[BAR_GRADIENT], DashLayer::mtx);
        ps_matrix_identity(DashLayer::mtx);

        draw_btns(gc, *r);
    }

    ps_restore(gc);
}

void DashLayer::init_layer(void)
{
    if (!DashLayer::init) {
        //init codes
        DashLayer::mtx = ps_matrix_create();//global matrix

        int b = DASH_TITLE_HEIGHT / 30;
        int h = DASH_TITLE_HEIGHT - 4 * b;

        //light gradient
        ps_color tc = {1, 1, 1, 0.8};
        ps_color tc2 = {1, 1, 1, 0.2};
        ps_point s1 = {0, 0};
        ps_point e1 = {0, h / 2 - 2 * b};
        DashLayer::grads[LIGHT_GRADIENT] = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
        ps_gradient_add_color_stop(DashLayer::grads[LIGHT_GRADIENT], 0, &tc);
        ps_gradient_add_color_stop(DashLayer::grads[LIGHT_GRADIENT], 1, &tc2);

        //bar gradient
        ps_color bc = {0.3, 0.3, 0.3, 1};
        ps_color bc2 = {0, 0, 0, 1};
        ps_point s2 = {0, 0};
        ps_point e2 = {0, DASH_TITLE_HEIGHT};
        DashLayer::grads[BAR_GRADIENT] = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s2, &e2);
        ps_gradient_add_color_stop(DashLayer::grads[BAR_GRADIENT], 0, &bc);
        ps_gradient_add_color_stop(DashLayer::grads[BAR_GRADIENT], 1, &bc2);

        //commit gradient
        ps_color c1 = {0, 0, 0, 1};
        ps_color c2 = {0, 1, 0, 1};
        ps_point s3 = {0, 0};
        ps_point e3 = {0, h};
        DashLayer::grads[COMMIT_GRADIENT] = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s3, &e3);
        ps_gradient_add_color_stop(DashLayer::grads[COMMIT_GRADIENT], 0, &c1);
        ps_gradient_add_color_stop(DashLayer::grads[COMMIT_GRADIENT], 1, &c2);

        //cancel gradient
        ps_color c3 = {0, 0, 0, 1};
        ps_color c4 = {1, 0, 0, 1};
        ps_point s4 = {0, 0};
        ps_point e4 = {0, h};
        DashLayer::grads[CANCEL_GRADIENT] = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s4, &e4);
        ps_gradient_add_color_stop(DashLayer::grads[CANCEL_GRADIENT], 0, &c3);
        ps_gradient_add_color_stop(DashLayer::grads[CANCEL_GRADIENT], 1, &c4);

        DashLayer::init = true;
    }
}

void DashLayer::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Layer::OnCreate(f, x, y, w, h);
    //add create code here

    init_layer();

    int b = DASH_TITLE_HEIGHT / 30;
    int button_width = width() / 5;
    int button_height = DASH_TITLE_HEIGHT - 4 * b;

    if (m_scombtn) {
        m_commit = Rect(width() - button_width - 5 * b, h - DASH_TITLE_HEIGHT + 2 * b, button_width, button_height);
    }

    if (m_scanbtn) {
        m_cancel = Rect(5 * b, h - DASH_TITLE_HEIGHT + 2 * b, button_width, button_height);
    }

}

void DashLayer::button_event(void* param)
{
    int btn = (intptr_t)param;
    if (btn == 1) {
        Commit();
    } else if (btn == 2) {
        Cancel();
    }
}

void DashLayer::OnMouseEvent(const MouseEvent* e)
{
    Layer::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_commit.contains(e->x(), e->y())) {
            m_event = m_commit;
            m_btn = 1;
        } else if (m_cancel.contains(e->x(), e->y())) {
            m_event = m_cancel;
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
            Update(&m_event);

            Widget::postEvent(this, EVENT_FUNC(DashLayer, button_event), EVENT_PARAM(m_btn));

            m_btn = 0;
            m_event = Rect(0, 0, 0, 0);
        }
    }
}

void DashLayer::OnCommit(void)
{
}

void DashLayer::OnCancel(void)
{
}

void DashLayer::Active(void)
{
    Show();
}

void DashLayer::Commit(void)
{
    OnCommit();
    Hide();
}

void DashLayer::Cancel(void)
{
    OnCancel();
    Hide();
}

void DashLayer::Show(void)
{
    Layer::Show();
}

void DashLayer::Hide(void)
{
    Layer::Hide();
}

ps_font* DashLayer::getTitleFont(void)
{
    if (!tfont) {
        tfont = ps_font_create("ËÎÌå", CHARSET_UNICODE, DASH_TITLE_HEIGHT / 15 * 7, FONT_WEIGHT_BOLD, False);
    }
    return tfont;
}
