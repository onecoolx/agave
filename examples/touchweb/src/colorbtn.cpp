/* colorbtn.cpp - MaCross application
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "picasso.h"
#include "colorbtn.h"
#include "mainwindow.h"


ColorButton::ColorButton(Widget* parent, const Color& c)
	: VButton(parent)
	, m_color(c)
	, m_mtx(0)
	, m_grd(0)
{

	m_mtx = ps_matrix_create();
}

ColorButton::~ColorButton()
{
	if (m_mtx)
		ps_matrix_unref(m_mtx);

	if (m_grd)
		ps_gradient_unref(m_grd);
}

void ColorButton::OnPaint(ps_context* gc, const Rect* r)
{
	ps_save(gc);

	int rds = DASH_TITLE_HEIGHT/5;
	int b = DASH_TITLE_HEIGHT/30;
	ps_rect rc = {0, 0, width(), height()};
	ps_color c = m_color;

	if (m_state & BS_PRESS) {
		if (c.r < 1) c.r += 0.3;		
		if (c.g < 1) c.g += 0.3;		
		if (c.b < 1) c.b += 0.3;		
	}

	ps_set_source_color(gc, &c);

	ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_fill(gc);

	// draw text
	if (!m_text.empty()) {
		ps_color c = {0, 0, 0, 1};
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
		ps_size sz = ps_get_text_extent(gc, m_text.c_str(), m_text.length());

		ps_wide_text_out_length(gc, rc.x+(rc.w-sz.w)/2, rc.y+rc.h/2-DASH_TITLE_HEIGHT/5, 
												(ps_uchar16*)m_text.c_str(), m_text.length());

		ps_set_font(gc, of);
	}

	// draw button light

	ps_rect lrc = {rc.x+b, rc.y+b, rc.w-2*b, rc.h/2-2*b};
	ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);

	ps_matrix_translate(m_mtx, lrc.x, lrc.y);
	ps_gradient_transform(m_grd, m_mtx);
	ps_set_source_gradient(gc, m_grd);
	ps_fill(gc);
	ps_matrix_translate(m_mtx, -lrc.x*2, -lrc.y*2);
	ps_gradient_transform(m_grd, m_mtx);
	ps_matrix_reset(m_mtx);

	ps_restore(gc);
}

void ColorButton::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	VButton::OnCreate(flags, x, y, w, h);

	int b = DASH_TITLE_HEIGHT/30;
	ps_color tc = {1, 1, 1, 0.8};
	ps_color tc2 = {1, 1, 1, 0.2};
	ps_point s1 = {0, 0};
	ps_point e1 = {0, h/2-2*b};
	m_grd = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
	ps_gradient_add_color_stop(m_grd, 0, &tc);
	ps_gradient_add_color_stop(m_grd, 1, &tc2);
}

