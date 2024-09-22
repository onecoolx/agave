/* netstat.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "netstat.h"
#include "network.h"
#include "dialog.h"
#include "application.h"
#include "extracfg.h"
#include "mainwindow.h"

NetStatPanel::NetStatPanel(Widget* parent)
	: Panel(parent)
	, m_main(0)
	, m_count(0)
	, m_flat(0)
{
	setTitle(U("ÍøÂçÁ¬½Ó"));
}

NetStatPanel::~NetStatPanel()
{
}

void NetStatPanel::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void NetStatPanel::OnPaint(ps_context* gc, const Rect* r)
{
	Panel::OnPaint(gc, r);

	ps_save(gc);

	int b = TITLE_HEIGHT/22;
	ps_color c = {1, 1, 1, 1};
	ps_set_text_color(gc, &c);
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

	if (m_flat == 1) {
		ps_wide_text_out_length(gc, b*50, b*35+titleHeight(), P16(U("ÕýÔÚÁ¬½ÓÍøÂç.")), 7);
	} else if (m_flat == 2) {
		ps_wide_text_out_length(gc, b*50, b*35+titleHeight(), P16(U("ÕýÔÚÁ¬½ÓÍøÂç..")), 8);
	} else if (m_flat == 3) {
		ps_wide_text_out_length(gc, b*50, b*35+titleHeight(), P16(U("ÕýÔÚÁ¬½ÓÍøÂç...")), 9);
	} else {
		ps_wide_text_out_length(gc, b*50, b*35+titleHeight(), P16(U("ÕýÔÚÁ¬½ÓÍøÂç")), 6);
	}

	ps_set_font(gc, of);

	m_flat++;
	if (m_flat == 4) 
		m_flat = 0;

	ps_restore(gc);
}

void NetStatPanel::OnTimer(void *data)
{
	if (m_count > 80) {
		stop();
		Hide();
		Dialog::AlertBox(m_main, U("ÎÞ·¨Á¬½ÓÍøÂç,Çë¼ì²éÍøÂçÉèÖÃ"), U("ÌáÊ¾"));
		return;
	}

	NetService * net = Application::getInstance()->getNetService();
	if (net->isConnected()) {
		stop();

		if (Application::getInstance()->getExtraConfig()->needReport()) {
			Application::getInstance()->getExtraConfig()->setNeedReport(false);
			// add report code here
			Application::getInstance()->report();
		}
		Hide();
	} else {
		m_count++;
		Update(NULL);
	}
}

void NetStatPanel::startNetWorkCheck(void)
{
	NetService * net = Application::getInstance()->getNetService();
	if (!net->isConnected()) {
		net->establishConnection();
		m_count = 0;
		Show();	
		start(1000, 0);
	}
}

