/* panel.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "panel.h"
#include "mainwindow.h"

Panel::Panel(Widget* parent)
	:TopWidget(parent)
{
}

Panel::~Panel()
{
}

int Panel::titleHeight(void) const
{
	return TITLE_HEIGHT;
}

void Panel::setTitle(const ustring& title)
{
	m_title = title;
}

void Panel::OnPaint(ps_context* gc, const Rect* d)
{
	TopWidget::OnPaint(gc, d);
	//draw paint code
	ps_save(gc);

	ps_color c = {0.1, 0.1, 0.1, 1};
	ps_color s = {0.8, 0.8, 0.8, 1};
	ps_color t = {0.2, 0.2, 0.2, 1};
	ps_rect r = {0, 0, width()-1, height()-1};
	ps_rectangle(gc, &r);
	ps_set_source_color(gc, &c);
	ps_set_stroke_color(gc, &t);
	ps_paint(gc);

	ps_set_stroke_color(gc, &s);
	ps_point p1 = {0, TITLE_HEIGHT};
	ps_point p2 = {width(), TITLE_HEIGHT};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_stroke(gc);

	Rect trc(0, 0, width(), TITLE_HEIGHT);
	if (!m_title.empty() && trc.intersect(*d)) {
		ps_font* f = MainWindow::getUIFont();
		ps_font* of = ps_set_font(gc, f);
		ps_set_text_color(gc, &s);
#ifdef FONT_NO_ANTIALIAS
		ps_set_text_antialias(gc, False);
#else
		if (MainWindow::uiFontAntialias())
			ps_set_text_antialias(gc, True);
		else
			ps_set_text_antialias(gc, False);
#endif
		ps_wide_text_out_length(gc, TITLE_HEIGHT/2, TITLE_HEIGHT/4, 
											(ps_uchar16*)m_title.c_str(), m_title.size());
		ps_set_font(gc, of);
	}

	ps_restore(gc);
}

