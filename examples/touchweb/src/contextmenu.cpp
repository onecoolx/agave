/* contextmenu.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "contextmenu.h"
#include "tabpage.h"
#include "webview.h"
#include "mainwindow.h"

ContextMenu::ContextMenu(Widget* parent)
	: TopWidget(parent)
	, m_main(0)
	, m_x(0)
	, m_y(0)
	, m_btn(0)
	, m_open(0,0,0,0)
	, m_nwin(0,0,0,0)
	, m_bopen(0,0,0,0)
	, m_imgopen(0,0,0,0)
	, m_imgwin(0,0,0,0)
	, m_imgbopen(0,0,0,0)
	, m_event(0,0,0,0)
	, m_easy(false)
{
}

ContextMenu::~ContextMenu()
{
}

void ContextMenu::clear(void)
{
	m_link = std::string();
	m_imglink = std::string();

	m_open = Rect(0,0,0,0);
	m_nwin = Rect(0,0,0,0);
	m_bopen = Rect(0,0,0,0);
	m_imgopen = Rect(0,0,0,0);
	m_imgwin = Rect(0,0,0,0);
	m_imgbopen = Rect(0,0,0,0);
	m_event = Rect(0,0,0,0);
}

void ContextMenu::create(void)
{
	int pos = 0;
	if (!m_link.empty()) {
		m_open = Rect(0, pos, width(), TOOLBAR_HEIGHT);
		pos += TOOLBAR_HEIGHT;

		if (!m_easy) {
			m_nwin = Rect(0, pos, width(), TOOLBAR_HEIGHT);
			pos += TOOLBAR_HEIGHT;

			m_bopen = Rect(0, pos, width(), TOOLBAR_HEIGHT);
			pos += TOOLBAR_HEIGHT;
		}
	}

	if (!m_imglink.empty()) {
		m_imgopen = Rect(0, pos, width(), TOOLBAR_HEIGHT);
		pos += TOOLBAR_HEIGHT;

		if (!m_easy) {
			m_imgwin = Rect(0, pos, width(), TOOLBAR_HEIGHT);
			pos += TOOLBAR_HEIGHT;

			m_imgbopen = Rect(0, pos, width(), TOOLBAR_HEIGHT);
			pos += TOOLBAR_HEIGHT;
		}
	}
}

void ContextMenu::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void ContextMenu::OnPaint(ps_context* gc, const Rect* r)
{
	TopWidget::OnPaint(gc, r);

	ps_save(gc);

	ps_rect rc = {0, 0, width(), height()};
	ps_color c = {0.1, 0.1, 0.1, 1};
	ps_rectangle(gc, &rc);
	ps_set_source_color(gc, &c);
	ps_fill(gc);

	int b = TOOLBAR_HEIGHT/15;
	int s = TOOLBAR_HEIGHT/5*2;
	if (!m_event.isEmpty()) {
		ps_color sc = {0.22, 0.45, 0.85, 1};
		ps_rect r = {m_event.x+b, m_event.y+b, m_event.w-2*b, m_event.h-2*b};
		ps_rectangle(gc, &r);
		ps_set_source_color(gc, &sc);
		ps_fill(gc);
	}

	ps_color fc = {1, 1, 1, 1};
	ps_set_text_color(gc, &fc);
	ps_font* f = MainWindow::getUIFont();
	ps_font* of = ps_set_font(gc, f);
#ifdef FONT_NO_ANTIALIAS
	ps_set_text_antialias(gc, False);
#else
	if (MainWindow::uiFontAntialias())
		ps_set_text_antialias(gc, True);
	else
		ps_set_text_antialias(gc, False);
#endif

	if (!m_link.empty()) {
		ps_wide_text_out_length(gc, m_open.x+2*b, m_open.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("´ò¿ªÁ¬½Ó"), 4);
		if (!m_easy) {
			ps_wide_text_out_length(gc, m_nwin.x+2*b, m_nwin.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("ÔÚÐÂ´°¿Ú´ò¿ª"), 6);
			ps_wide_text_out_length(gc, m_bopen.x+2*b, m_bopen.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("ÔÚºóÌ¨´ò¿ª"), 5);
		}
	}

	if (!m_imglink.empty()) {
		ps_wide_text_out_length(gc, m_imgopen.x+2*b, m_imgopen.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("´ò¿ªÍ¼Æ¬"), 4);
		if (!m_easy) {
			ps_wide_text_out_length(gc, m_imgwin.x+2*b, m_imgwin.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("ÔÚÐÂ´°¿Ú´ò¿ªÍ¼Æ¬"), 8);
			ps_wide_text_out_length(gc, m_imgbopen.x+2*b, m_imgbopen.y+(TOOLBAR_HEIGHT-s)/2, (ps_uchar16*)U("ÔÚºóÌ¨´ò¿ªÍ¼Æ¬"), 7);
		}
	}

	ps_set_font(gc, of);
	ps_restore(gc);
}

void ContextMenu::OnMouseEvent(const MouseEvent* e)
{
	TopWidget::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		if (m_open.contains(e->x(), e->y())) {
			m_event = m_open;
			m_btn = 1;
		} else if (m_nwin.contains(e->x(), e->y())) {
			m_event = m_nwin;
			m_btn = 2;
		} else if (m_bopen.contains(e->x(), e->y())) {
			m_event = m_bopen;
			m_btn = 3;
		} else if (m_imgopen.contains(e->x(), e->y())) {
			m_event = m_imgopen;
			m_btn = 4;
		} else if (m_imgwin.contains(e->x(), e->y())) {
			m_event = m_imgwin;
			m_btn = 5;
		} else if (m_imgbopen.contains(e->x(), e->y())) {
			m_event = m_imgbopen;
			m_btn = 6;
		} else {
			m_event = Rect(0, 0, 0, 0);
			m_btn = 0;
		}

		if (!m_event.isEmpty()) {
			Update(NULL);
		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (!m_event.isEmpty()) {
			m_event = Rect(0, 0, 0, 0);
			Update(NULL);

			if (m_btn) {
				Widget::postEvent(this, EVENT_FUNC(ContextMenu, button_event), EVENT_PARAM(m_btn));
				m_btn = 0;
			}
		}
	}
}

void ContextMenu::setEasyMode(bool b)
{
	m_easy = b;
}

void ContextMenu::button_event(void* p)
{
	Hide();

	int btn = (intptr_t)p;
	switch(btn)
	{
	case 1:
		{
			m_main->getTabs()->getActiveView()->loadUrl(m_link);
		}
		break;
	case 2:
		{
			m_main->getTabs()->newView(m_link);
		}
		break;
	case 3:
		{
			m_main->getTabs()->newViewBack(m_link);
		}
		break;
	case 4:
		{
			m_main->getTabs()->getActiveView()->loadUrl(m_imglink);
		}
		break;
	case 5:
		{
			m_main->getTabs()->newView(m_imglink);
		}
		break;
	case 6:
		{
			m_main->getTabs()->newViewBack(m_imglink);
		}
		break;
	}
}
