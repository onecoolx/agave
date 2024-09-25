/* setting.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <picasso/picasso.h>

#include "application.h"
#include "about.h"
#include "secmgr.h"
#include "tabpage.h"
#include "setting.h"
#include "extracfg.h"
#include "files.h"
#include "mainwindow.h"

Setting::Setting(Widget* parent)
    : DashLayer(parent)
    , m_main(0)
    , m_imgshow(0, 0, 0, 0)
    , m_script(0, 0, 0, 0)
    , m_cookie(0, 0, 0, 0)
    , m_about(0, 0, 0, 0)
    , m_antitxt(0, 0, 0, 0)
    , m_userdata(0, 0, 0, 0)
    , m_event(0, 0, 0, 0)
    , m_btn(0)
    , m_config(0)
    , m_refresh(false)
{
    setTitle(U("ÉèÖÃ"));
    setCommitText(U("±£´æ"));
    setCancelText(U("È¡Ïû"));

    m_config = (MC_CONFIG*)malloc(sizeof(MC_CONFIG));
    memset(m_config, 0, sizeof(MC_CONFIG));
}

Setting::~Setting()
{
    free(m_config);
}

void Setting::setMainWindow(MainWindow* main)
{
    m_main = main;
}

void Setting::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
    DashLayer::OnCreate(flags, x, y, w, h);

    int hc = int(DASH_TITLE_HEIGHT * 1.2 - DASH_TITLE_HEIGHT / 30 * 8);
    int cell_height = int(DASH_TITLE_HEIGHT * 1.2);
    int border = DASH_TITLE_HEIGHT / 30;

    m_imgshow = Rect(width() / 3 * 2, DASH_TITLE_HEIGHT + (cell_height - hc) / 2, width() / 3 - border * 10, hc);
    m_script = Rect(width() / 3 * 2, DASH_TITLE_HEIGHT + cell_height + (cell_height - hc) / 2, width() / 3 - border * 10, hc);
    m_cookie = Rect(width() / 3 * 2, DASH_TITLE_HEIGHT + cell_height * 2 + (cell_height - hc) / 2, width() / 3 - border * 10, hc);
    m_antitxt = Rect(width() / 3 * 2, DASH_TITLE_HEIGHT + cell_height * 3 + (cell_height - hc) / 2, width() / 3 - border * 10, hc);
    m_userdata = Rect(width() / 3 * 2, DASH_TITLE_HEIGHT + cell_height * 4 + (cell_height - hc) / 2, width() / 3 - border * 10, hc);

    m_about = Rect(width() / 2 - (width() / 3 - border * 10) / 2, h - cell_height - DASH_TITLE_HEIGHT, width() / 3 - border * 10, hc);
}

void Setting::draw_button(ps_context* gc, const Rect& r, bool enable, bool highlight)
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
        if (highlight) {
            ps_color c = {1, 0, 0, 1};
            ps_set_source_color(gc, &c);
        } else {
            ps_color c = {0.6, 0, 0, 1};
            ps_set_source_color(gc, &c);
        }
    }
    ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_fill(gc);

    if (enable) {
        ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 12, r.y + r.h / 2 - b * 6, (ps_uchar16*)U("ÆôÓÃ"), 2);
    } else {
        ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 12, r.y + r.h / 2 - b * 6, (ps_uchar16*)U("½ûÓÃ"), 2);
    }

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

void Setting::draw_about_button(ps_context* gc, const Rect& r, bool highlight)
{
    int rds = DASH_TITLE_HEIGHT / 5;
    int b = DASH_TITLE_HEIGHT / 30;
    ps_rect rc = r;
    if (highlight) {
        ps_color c = {0.3, 0.8, 1, 1};
        ps_set_source_color(gc, &c);
    } else {
        ps_color c = {0, 0.5, 1, 1};
        ps_set_source_color(gc, &c);
    }
    ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_fill(gc);

    ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 12, r.y + r.h / 2 - b * 6, (ps_uchar16*)U("¹ØÓÚ"), 2);

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

void Setting::draw_user_button(ps_context* gc, const Rect& r, bool highlight)
{
    int rds = DASH_TITLE_HEIGHT / 5;
    int b = DASH_TITLE_HEIGHT / 30;
    ps_rect rc = r;
    if (highlight) {
        ps_color c = {0.3, 0.8, 1, 1};
        ps_set_source_color(gc, &c);
    } else {
        ps_color c = {0, 0.5, 1, 1};
        ps_set_source_color(gc, &c);
    }
    ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
    ps_fill(gc);

    ps_wide_text_out_length(gc, r.x + r.w / 2 - b * 12, r.y + r.h / 2 - b * 6, (ps_uchar16*)U("  >>"), 4);

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
void Setting::draw_buttons(ps_context* gc, const Rect& r)
{
    Rect rc = r;
    if (rc.intersect(m_imgshow)) {
        draw_button(gc, m_imgshow, m_config->autoload_images ? true : false, (m_btn == 1) ? true : false);
    }

    rc = r;
    if (rc.intersect(m_script)) {
        draw_button(gc, m_script, m_config->enable_javascript ? true : false, (m_btn == 2) ? true : false);
    }

    rc = r;
    if (rc.intersect(m_cookie)) {
        draw_button(gc, m_cookie, m_config->enable_cookies ? true : false, (m_btn == 3) ? true : false);
    }

    rc = r;
    if (rc.intersect(m_antitxt)) {
        draw_button(gc, m_antitxt, m_config->text_antialias ? true : false, (m_btn == 4) ? true : false);
    }

    rc = r;
    if (rc.intersect(m_userdata)) {
        draw_user_button(gc, m_userdata, (m_btn == 5) ? true : false);
    }

    rc = r;
    if (rc.intersect(m_about)) {
        draw_about_button(gc, m_about, (m_btn == 99) ? true : false);
    }
}

void Setting::OnPaint(ps_context* gc, const Rect* r)
{
    DashLayer::OnPaint(gc, r);

    ps_save(gc);

    int cell_height = int(DASH_TITLE_HEIGHT * 1.2);
    int num_rc = (height() - DASH_TITLE_HEIGHT * 2) / cell_height;

    for (int i = 0; i < num_rc; i++) {
        ps_rect rc = {0, DASH_TITLE_HEIGHT + i * cell_height, width(), cell_height};
        Rect trc(rc);
        if (trc.intersect(*r)) {
            ps_rectangle(gc, &rc);
            if (i % 2) {
                ps_color c = {0.8, 0.8, 0.8, 1};
                ps_set_source_color(gc, &c);
            } else {
                ps_color c = {1, 1, 1, 1};
                ps_set_source_color(gc, &c);
            }
            ps_fill(gc);
        }
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

    int border = DASH_TITLE_HEIGHT / 30;

    ps_wide_text_out_length(gc, border * 8, DASH_TITLE_HEIGHT
                            + cell_height / 2 - cell_height / 4, (ps_uchar16*)U("ÏÔÊ¾Í¼Æ¬"), 4);

    ps_wide_text_out_length(gc, border * 8, DASH_TITLE_HEIGHT
                            + cell_height + cell_height / 2 - cell_height / 4, (ps_uchar16*)U("JavaScriptÖ§³Ö"), 12);

    ps_wide_text_out_length(gc, border * 8, DASH_TITLE_HEIGHT
                            + cell_height * 2 + cell_height / 2 - cell_height / 4, (ps_uchar16*)U("CookiesÖ§³Ö"), 9);

    ps_wide_text_out_length(gc, border * 8, DASH_TITLE_HEIGHT
                            + cell_height * 3 + cell_height / 2 - cell_height / 4, (ps_uchar16*)U("×ÖÌåÆ½»¬"), 4);

    ps_wide_text_out_length(gc, border * 8, DASH_TITLE_HEIGHT
                            + cell_height * 4 + cell_height / 2 - cell_height / 4, (ps_uchar16*)U("ÓÃ»§ÒþË½"), 4);

    draw_buttons(gc, *r);

    ps_set_font(gc, of);

    ps_restore(gc);
}

void Setting::OnMouseEvent(const MouseEvent* e)
{
    DashLayer::OnMouseEvent(e);

    if (e->type() == MouseEvent::MouseDown) {
        if (m_imgshow.contains(e->x(), e->y())) {
            m_event = m_imgshow;
            m_btn = 1;
        } else if (m_script.contains(e->x(), e->y())) {
            m_event = m_script;
            m_btn = 2;
        } else if (m_cookie.contains(e->x(), e->y())) {
            m_event = m_cookie;
            m_btn = 3;
        } else if (m_antitxt.contains(e->x(), e->y())) {
            m_event = m_antitxt;
            m_btn = 4;
        } else if (m_userdata.contains(e->x(), e->y())) {
            m_event = m_userdata;
            m_btn = 5;
        } else if (m_about.contains(e->x(), e->y())) {
            m_event = m_about;
            m_btn = 99;
        } else {
            m_event = Rect(0, 0, 0, 0);
            m_btn = 0;
        }

        if (m_btn) {
            Update(&m_event);
        }
    } else if (e->type() == MouseEvent::MouseUp) {
        if (m_btn) {
            Widget::postEvent(this, EVENT_FUNC(Setting, button_event), EVENT_PARAM(m_btn));
            m_btn = 0;
        }
    }
}

void Setting::button_event(void* param)
{
    int btn = (intptr_t)param;
    switch (btn) {
        case 1:
            m_config->autoload_images = m_config->autoload_images ? False : True;
            break;
        case 2:
            m_config->enable_javascript = m_config->enable_javascript ? False : True;
            break;
        case 3:
            m_config->enable_cookies = m_config->enable_cookies ? False : True;
            break;
        case 4: {
                m_config->text_antialias = m_config->text_antialias ? False : True;

                MainWindow::setUIFontAntialias(m_config->text_antialias ? true : false);
                Application::getInstance()->getMainWindow()->Update(NULL);
                m_refresh = true;
            }
            break;
        case 5: {
                Application::getInstance()->getMainWindow()->getSecirtyData()->Active();
                Cancel();
            }
            break;
        case 99: {
                Application::getInstance()->getMainWindow()->getAbout()->Active();
                Cancel();
            }
            break;
    }
    Update(&m_event);
    m_event = Rect(0, 0, 0, 0);
}

void Setting::OnCommit(void)
{
    DashLayer::OnCommit();
    macross_set_config(m_config);
    if (m_refresh) {
        m_refresh = false;
        Widget::postEvent(Application::getInstance()->getMainWindow()->getTabs(),
                          EVENT_FUNC(TabPage, repaintAllPage), 0);
    }
    File* f = File::OpenFile(DATA_PATH("config.ini"), File::READ_WRITE, File::BINARY);
    if (f) {
        f->WriteData((unsigned char*)m_config, sizeof(MC_CONFIG));
        File::CloseFile(f);
    }
}

void Setting::OnShow(void)
{
    DashLayer::OnShow();
    if (macross_get_config(m_config)) {
        File* f = File::OpenFile(DATA_PATH("config.ini"), File::READ_ONLY, File::BINARY);
        if (f) {
            f->ReadData((unsigned char*)m_config, sizeof(MC_CONFIG));
            File::CloseFile(f);
        }
    }
}
