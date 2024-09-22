/* secmgr.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "macross.h"
#include "secmgr.h"
#include "autofill.h"
#include "dialog.h"
#include "colorbtn.h"
#include "mainwindow.h"
#include "application.h"

SecirtyData::SecirtyData(Widget* parent)
	: DashLayer(parent)
	, m_main(0)
	, m_clearCookie(0)
	, m_clearCache(0)
	, m_clearPasswd(0)
{
	setTitle(U("ÒþË½"));
	setCommitText(U("·µ»Ø"));
	setCancel(false);

	m_clearCookie = new ColorButton(this, Color(0, 0.5, 1));
	m_clearCookie->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(SecirtyData, clear_cookies));
	m_clearCookie->setText(U("Çå³ý"));

	m_clearCache = new ColorButton(this, Color(0, 0.5, 1));
	m_clearCache->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(SecirtyData, clear_cache));
	m_clearCache->setText(U("Çå³ý"));

	m_clearPasswd = new ColorButton(this, Color(0, 0.5, 1));
	m_clearPasswd->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(SecirtyData, clear_passwd));
	m_clearPasswd->setText(U("Çå³ý"));
}

SecirtyData::~SecirtyData()
{
	delete m_clearPasswd;
	delete m_clearCache;
	delete m_clearCookie;
}

void SecirtyData::clear_cookies(void)
{
	macross_clear_cookies();
	Dialog::AlertBox(m_main, U("Çå³ý³É¹¦£¡"), U("ÐÅÏ¢"));
}

void SecirtyData::clear_cache(void)
{
	macross_clear_disk_cache();
	Dialog::AlertBox(m_main, U("Çå³ý³É¹¦£¡"), U("ÐÅÏ¢"));
}

void SecirtyData::clear_passwd(void)
{
	Application::getInstance()->getAutofill()->clearAllFormData();
	Dialog::AlertBox(m_main, U("Çå³ý³É¹¦£¡"), U("ÐÅÏ¢"));
}

void SecirtyData::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void SecirtyData::OnCreate(uint32_t f, int x, int y, int w, int h)
{
	DashLayer::OnCreate(f, x, y, w, h);

	int hc = int(DASH_TITLE_HEIGHT*1.2-DASH_TITLE_HEIGHT/30*8);
	int cell_height = int(DASH_TITLE_HEIGHT*1.2);
	int border = DASH_TITLE_HEIGHT/30;

	m_clearCookie->OnCreate(WF_ENABLED|WF_VISIBLE, width()/3*2, DASH_TITLE_HEIGHT+(cell_height-hc)/2, width()/3-border*10, hc);
	m_clearCache->OnCreate(WF_ENABLED|WF_VISIBLE, width()/3*2, DASH_TITLE_HEIGHT+cell_height+(cell_height-hc)/2, width()/3-border*10, hc);
	m_clearPasswd->OnCreate(WF_ENABLED|WF_VISIBLE, width()/3*2, DASH_TITLE_HEIGHT+cell_height*2+(cell_height-hc)/2, width()/3-border*10, hc);

	addChild(m_clearCookie);
	addChild(m_clearCache);
	addChild(m_clearPasswd);
}

void SecirtyData::OnPaint(ps_context* gc, const Rect* r)
{
	DashLayer::OnPaint(gc, r);

	ps_save(gc);

	int cell_height = int(DASH_TITLE_HEIGHT*1.2);
	int num_rc = (height()-DASH_TITLE_HEIGHT*2)/cell_height;

	for (int i = 0; i < num_rc; i++) {
		ps_rect rc = {0, DASH_TITLE_HEIGHT+i*cell_height, width(), cell_height};
		Rect trc(rc);
		if (trc.intersect(*r)) {
			ps_rectangle(gc, &rc);
			if (i%2) {
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
	if (MainWindow::uiFontAntialias())
		ps_set_text_antialias(gc, True);
	else
		ps_set_text_antialias(gc, False);
#endif

	int border = DASH_TITLE_HEIGHT/30;

	ps_wide_text_out_length(gc, border*8, DASH_TITLE_HEIGHT
						+cell_height/2-cell_height/4, (ps_uchar16*)U("Çå³ýCookie"), 8);

	ps_wide_text_out_length(gc, border*8, DASH_TITLE_HEIGHT
						+cell_height+cell_height/2-cell_height/4, (ps_uchar16*)U("Çå³ý»º´æ"), 4);

	ps_wide_text_out_length(gc, border*8, DASH_TITLE_HEIGHT
						+cell_height*2+cell_height/2-cell_height/4, (ps_uchar16*)U("Çå³ýÒÑ±£´æÃÜÂë"), 7);


	ps_set_font(gc, of);

	ps_restore(gc);
}

void SecirtyData::OnShow(void)
{
	DashLayer::OnShow();
}

