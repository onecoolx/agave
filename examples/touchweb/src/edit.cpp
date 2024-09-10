/* edit.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <string>
#include "config.h"
#include "picasso.h"
#include "edit.h"
#include "mainwindow.h"
#include "application.h"

LineEdit::LineEdit(Widget* parent)
	: Widget(parent)
	, m_ispwd(false)
	, m_hasbtn(true)
	, m_border(false)
	, m_clear(0,0,0,0)
	, m_btn(0)
{
}

LineEdit::~LineEdit()
{
}

void LineEdit::setTipText(const ustring& s)
{
	m_tip = s;
}

ustring LineEdit::getTipText(void) const
{
	return m_tip;
}

ustring LineEdit::getText(void) const
{
	return m_text;
}

void LineEdit::setText(const ustring& s)
{
	m_text = s;
}

void LineEdit::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	Widget::OnCreate(flags, x, y, w, h);
	
	int b = TOOLBAR_HEIGHT/30;
	int bh = h-b*8;

	m_clear = Rect(w-bh-b*2, b*4, bh, bh);
}

static void draw_clear_btn(ps_context* gc, const Rect& r, bool highlight)
{
	int b = DASH_TITLE_HEIGHT/30;

	if (highlight) {
		ps_color c = {0.05,0.18,0.35,1};
		ps_set_stroke_color(gc, &c);
	} else {
		ps_color c = {0.18,0.49,0.87,1};
		ps_set_stroke_color(gc, &c);
	}	

	ps_point p1 = {r.x+b*4, r.y+b*4};
	ps_point p2 = {r.x+r.w-b*4, r.y+r.h-b*4};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_point p3 = {r.x+r.w-b*4, r.y+b*4};
	ps_point p4 = {r.x+b*4, r.y+r.h-b*4};
	ps_move_to(gc, &p3);
	ps_line_to(gc, &p4);

	ps_set_line_width(gc, b*2);
	ps_stroke(gc);
	ps_set_line_width(gc, 1);
}

void LineEdit::OnPaint(ps_context* gc, const Rect* d)
{
	Widget::OnPaint(gc, d);

	ps_save(gc);

	int b = TOOLBAR_HEIGHT/30;
	int rds = TOOLBAR_HEIGHT/15;
	int box_len = width()-b*8;

	unsigned int m_pos = 0;
	unsigned int m_cur = 0;
	size_t	m_len = 0;

	ps_color tc = {1, 1, 1, 1};	
	ps_color sc = {0, 0, 0, 1};	
	ps_rect tr = {b, b, width()-b*3, height()-b*3};
	ps_rounded_rect(gc, &tr, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_set_source_color(gc, &tc);
	ps_set_stroke_color(gc, &sc);
	ps_set_line_width(gc, b);

	if (m_border)
		ps_paint(gc);
	else
		ps_fill(gc);

	ps_set_line_width(gc, 1);

	if (m_hasbtn && !m_text.empty()) {
		box_len -= m_clear.w;
		draw_clear_btn(gc, m_clear, m_btn ? true : false);
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

	ps_size sz = ps_get_text_extent(gc, m_text.c_str(), m_text.length());
	if (sz.w <= box_len) {
		m_cur = sz.w;
		m_len = m_text.length();
		m_pos = 0;
	} else {
		size_t tl = 0;	
		int tw = 0;
		while(tw < box_len) {
			ps_size tz = ps_get_text_extent(gc, m_text.c_str(), tl+1);
			tw = tz.w; 
			tl++;
		}
		m_len = tl-1;

		tl = 0;
		tw = 0;
		while(tw < box_len) {
			ps_size tz = ps_get_text_extent(gc, m_text.c_str()+m_text.length()-tl-1, tl+1);
			tw = tz.w; 
			tl++;
		}
		m_pos = m_text.length()-tl+1;

		ps_size lz = ps_get_text_extent(gc, m_text.c_str()+m_pos, m_text.length()-m_pos);
		m_cur = lz.w;
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

		ps_point p1 = {m_cur+b*4, b*4};
		ps_point p2 = {m_cur+b*4, height()-b*4};
		ps_move_to(gc, &p1);
		ps_line_to(gc, &p2);
		ps_stroke(gc);
	}

	int fs = TOOLBAR_HEIGHT/5*2;
	int ft = (height()-fs)/2;
	//draw text
	if (!m_text.empty()) {
		if (m_ispwd) {
			ps_size sz = ps_get_text_extent(gc, U("*"), 1);
			for (unsigned int i=0; i<m_len; i++)
				ps_wide_text_out_length(gc, b*4+sz.w*i, ft, (ps_uchar16*)U("*"), 1);
		} else {
			if (hasFocus())
				ps_wide_text_out_length(gc, b*4, ft, (ps_uchar16*)m_text.c_str()+m_pos, m_text.length()-m_pos);
			else
				ps_wide_text_out_length(gc, b*4, ft, (ps_uchar16*)m_text.c_str(), m_len);
		}
	}

	if (!hasFocus() && m_text.empty()) {
		ps_color tic = {0.75, 0.75, 0.68, 1};
		ps_set_text_color(gc, &tic);
		ps_wide_text_out_length(gc, b*4, ft, (ps_uchar16*)m_tip.c_str(), m_tip.length());
	}

	ps_set_font(gc, of);
	ps_restore(gc);
}

void LineEdit::OnSetFocus(void)
{
	Widget::OnSetFocus();
	Update(NULL);
	SetImeStatus(true);
}

void LineEdit::OnKillFocus(void)
{
	Widget::OnKillFocus();
	Update(NULL);
	SetImeStatus(false);
}

void LineEdit::OnChar(uchar_t code)
{
	Widget::OnChar(code);

	if (IS_PRINT(code)) {
		m_text += code;
		Update(NULL);
	} else if (code == 0x08) {
		if (m_text.length()) {
			m_text.erase(m_text.end()-1);
			Update(NULL);
		}
	}
}

void LineEdit::OnMouseEvent(const MouseEvent* e)
{
	Widget::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		if (m_clear.contains(e->x(), e->y()) && m_hasbtn) {
			m_btn = 1;
		} else {
			m_btn = 0;
		}

		if (m_btn) {
			Update(&m_clear);
		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (m_btn) {
			m_btn = 0;
			m_text = ustring();
			Update(NULL);
		}
	}
}

