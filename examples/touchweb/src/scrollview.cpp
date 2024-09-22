/* scrollview.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "application.h"
#include "scrollview.h"

ScrollView::ScrollView(Widget* parent)
	: Widget(parent)
	, m_gc(0)
	, m_dc(0)
	, m_off_x(0)
	, m_off_y(0)
	, m_content_w(0)
	, m_content_h(0)
{
}

ScrollView::~ScrollView()
{
}

void ScrollView::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	Widget::OnCreate(flags, x, y, w, h);

	m_content_w = w;
	m_content_h = h;

	m_dc = ps_canvas_create(Application::getInstance()->color_format(), w, h);
	m_gc = ps_context_create(m_dc, 0);

	ps_rect rc = {0, 0, w, h};
	Rect tc = rc;
	ps_color c = {1,1,1,1};
	ps_save(m_gc);
	ps_set_source_color(m_gc, &c);
	ps_scissor_rect(m_gc, &rc);
	ps_clear(m_gc);
	OnPaintContents(m_gc, &tc);
	ps_restore(m_gc);
}

void ScrollView::OnDestroy(void)
{
	if (m_gc){
		ps_context_unref(m_gc);
		m_gc = 0;
	}

	if (m_dc){
		ps_canvas_unref(m_dc);
		m_dc = 0;
	}

}

void ScrollView::OnPaint(ps_context* gc, const Rect* d)
{
	Widget::OnPaint(gc, d);
	ps_point xp = {x()-m_off_x, y()-m_off_y};
	ps_canvas * canvas = ps_context_get_canvas(gc);
	ps_canvas_bitblt(m_dc, 0, canvas, &xp);
}

void ScrollView::OnPaintContents(ps_context* gc, const Rect* r)
{
}

int ScrollView::contentHeight(void) const
{
	return m_content_h;
}

int ScrollView::contentWidth(void) const
{
	return m_content_w;
}

void ScrollView::setContentSize(int w, int h)
{
	if (w > m_content_w || h > m_content_h) {
		ps_context_unref(m_gc);
		ps_canvas_unref(m_dc);

		m_dc = ps_canvas_create(Application::getInstance()->color_format(), w, h);
		m_gc = ps_context_create(m_dc, 0);
	}

	m_content_w = w;
	m_content_h = h;

	ps_size sz = {0};
    ps_canvas_get_size(m_dc, &sz);
	ps_rect rc = {0, 0, sz.w, sz.h};
	Rect tc = rc;
	ps_color c = {1,1,1,1};
	ps_save(m_gc);
	ps_set_source_color(m_gc, &c);
	ps_scissor_rect(m_gc, &rc);
	ps_clear(m_gc);
	OnPaintContents(m_gc, &tc);
	ps_restore(m_gc);

	Update(NULL);
}

int ScrollView::scrollY(void) const
{
	return m_off_y;
}

int ScrollView::scrollX(void) const
{
	return m_off_x;
}

void ScrollView::setScrollPoint(int x, int y)
{
	m_off_x = x;
	m_off_y = y;
}

void ScrollView::scrollContents(int xoffset, int yoffset)
{
	int old_xoffset = m_off_x;
	int old_yoffset = m_off_y;

	m_off_x += xoffset;
	m_off_y += yoffset;

	if (m_off_x < 0) m_off_x = 0;
	if (m_off_y < 0) m_off_y = 0;

	if (m_content_w >= width()) {
		if ((m_off_x + width()) > m_content_w) m_off_x = m_content_w-width();
	} else {
		m_off_x = 0;
	}

	if (m_content_h >= height()) {
		if ((m_off_y + height()) > m_content_h) m_off_y = m_content_h-height();
	} else {
		m_off_y = 0;
	}

	Update(NULL);
}
