/* shadebtn.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "shadebtn.h"
#include "mainwindow.h"

class ShadeButtonImpl
{
public:
	ShadeButtonImpl()
		: mtx(0)
		, high_gradient(0)
		, light_gradient(0)
	{

		mtx = ps_matrix_create();
	}

	~ShadeButtonImpl()
	{
		if (mtx)
			ps_matrix_unref(mtx);

		if (high_gradient)
			ps_gradient_unref(high_gradient);

		if (light_gradient)
			ps_gradient_unref(light_gradient);
	}

	ps_matrix * mtx;
	ps_gradient * high_gradient;
	ps_gradient * light_gradient;
};


ShadeButton::ShadeButton(Widget* parent, int s)
	: VButton(parent)
	, m_impl(new ShadeButtonImpl)
	, m_style(s)
{
}

ShadeButton::~ShadeButton()
{
	delete m_impl;
}

void ShadeButton::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	VButton::OnCreate(flags, x, y, w, h);

	int b = TITLE_HEIGHT/22;
	int ht = TITLE_HEIGHT-4*b;

	//high gradient
	ps_color c1 = {0, 0, 0, 1};
	ps_color c2;
  	if (m_style == GreenBtn) {
		c2.r = 0; c2.g = 1; c2.b = 0; c2.a = 1;
	} else if (m_style == RedBtn) {
		c2.r = 1; c2.g = 0; c2.b = 0; c2.a = 1;
	} else if (m_style == BlueBtn) {
		c2.r = 0; c2.g = 0; c2.b = 1; c2.a = 1;
	}

	ps_point s3 = {0, 0};
	ps_point e3 = {0, h};
	m_impl->high_gradient = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s3, &e3);
	ps_gradient_add_color_stop(m_impl->high_gradient, 0, &c1);
	ps_gradient_add_color_stop(m_impl->high_gradient, 1, &c2);

	//light gradient
	ps_color tc = {1, 1, 1, 0.8};
	ps_color tc2 = {1, 1, 1, 0.2};
	ps_point s1 = {0, 0};
	ps_point e1 = {0, ht/2-2*b};
	m_impl->light_gradient = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
	ps_gradient_add_color_stop(m_impl->light_gradient, 0, &tc);
	ps_gradient_add_color_stop(m_impl->light_gradient, 1, &tc2);
}

void ShadeButton::OnPaint(ps_context* gc, const Rect* r)
{
	ps_save(gc);

	int b = TITLE_HEIGHT/22;
	int rds = b * 3;
	ps_rect rc = {0, 0, width(), height()};
	ps_color c = {0.5, 0.5, 0.5, 1};
	ps_set_stroke_color(gc, &c);

	ps_set_line_width(gc, b);
	ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_stroke(gc);
	ps_set_line_width(gc, 1);

	if (m_state & BS_PRESS) {
		ps_rect r2 = {rc.x+b, rc.y+b, rc.w-b, rc.h-b};
		ps_rounded_rect(gc, &r2, rds, rds, rds, rds, rds, rds, rds, rds);
		ps_matrix_translate(m_impl->mtx, rc.x, rc.y);
		ps_gradient_transform(m_impl->high_gradient, m_impl->mtx);
		ps_set_source_gradient(gc, m_impl->high_gradient);
		ps_fill(gc);
		ps_matrix_translate(m_impl->mtx, -rc.x*2, -(rc.y)*2);
		ps_gradient_transform(m_impl->high_gradient, m_impl->mtx);
		ps_matrix_identity(m_impl->mtx);
	}

	// draw text
	if (!m_text.empty()) {
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
		ps_size sz = {0};
        ps_get_text_extent(gc, m_text.c_str(), m_text.length(), &sz);

		ps_wide_text_out_length(gc, rc.x+(rc.w-sz.w)/2, rc.y+rc.h/2-DASH_TITLE_HEIGHT/5, 
												(ps_uchar16*)m_text.c_str(), m_text.length());

		ps_set_font(gc, of);
	}

	// draw button light

	ps_rect lrc = {rc.x+2*b, rc.y+2*b, rc.w-3*b, rc.h/2-2*b};
	ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);

	ps_matrix_translate(m_impl->mtx, lrc.x, lrc.y);
	ps_gradient_transform(m_impl->light_gradient, m_impl->mtx);
	ps_set_source_gradient(gc, m_impl->light_gradient);
	ps_fill(gc);
	ps_matrix_translate(m_impl->mtx, -lrc.x*2, -lrc.y*2);
	ps_gradient_transform(m_impl->light_gradient, m_impl->mtx);
	ps_matrix_identity(m_impl->mtx);

	ps_restore(gc);
}

