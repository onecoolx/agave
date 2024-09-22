/* floatbtn.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "floatbtn.h"
#include "mainwindow.h"
#include "tabpage.h"
#include "application.h"
#include "webview.h"

FloatButton::FloatButton(Widget* parent)
	: TopWidget(parent)
{
}

FloatButton::~FloatButton()
{
}

static void draw_border(ps_context* gc, const Rect* r)
{
	int b = TOOLBAR_HEIGHT/30;
	ps_rect rc = {r->x+b*2, r->y+b*2, r->w-b*5, r->h-b*5};
	ps_set_line_width(gc, b);
	ps_color c1 = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c1);
	ps_ellipse(gc, &rc);
	ps_stroke(gc);
	ps_set_line_width(gc, 1);
}

static void draw_event(ps_context* gc, const Rect* r)
{
	int b = TOOLBAR_HEIGHT/30;
	ps_rect rc = {r->x+b*2, r->y+b*2, r->w-b*5, r->h-b*5};

	ps_set_line_width(gc, b);
	ps_color c1 = {1, 1, 1, 1};
	ps_color c2 = {0.08, 0.60, 0.08, 1};
	ps_color c3 = {0.05, 0.05, 0.05, 1};
	ps_point s = {rc.x, rc.y};
	ps_point e = {rc.x+rc.w, rc.y+rc.h};
	ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
	ps_gradient_add_color_stop(g, 0, &c1);
	ps_gradient_add_color_stop(g, 0.5, &c2);
	ps_gradient_add_color_stop(g, 1, &c3);
	ps_set_source_gradient(gc, g);
	ps_set_stroke_color(gc, &c1);
	ps_ellipse(gc, &rc);
	ps_paint(gc);
	ps_gradient_unref(g);
	ps_set_line_width(gc, 1);
}

static void draw_light(ps_context* gc, const Rect* r)
{
}

//mode button
ModeButton::ModeButton(Widget* parent)
	: FloatButton(parent)
	, m_main(0)
	, m_face(0)
	, m_down(false)
{
}

ModeButton::~ModeButton()
{
}

void ModeButton::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void ModeButton::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	FloatButton::OnCreate(flags, x, y, w, h);

	m_face = ps_canvas_create(Application::getInstance()->color_format(), w, h);
	ps_context * gc = ps_context_create(m_face, 0);

	Rect client(0, 0, width(), height());
	Rect * r = &client;
	ps_color cb = {0.1, 0.1, 0.1, 1};
	ps_color t = {0.2, 0.2, 0.2, 1};
	ps_rect rc = {0, 0, width()-1, height()-1};
	ps_rectangle(gc, &rc);
	ps_set_source_color(gc, &cb);
	ps_set_stroke_color(gc, &t);
	ps_paint(gc);

	draw_border(gc, r);

	int b = TOOLBAR_HEIGHT/30;
	ps_color c = {1, 1, 1, 1};
	ps_point p1 = {r->x+r->w/2, r->y+b*6};
	ps_point p2 = {r->x+b*6, r->y+r->h-b*12};
	ps_point p3 = {r->x+r->w-b*6, r->y+r->h-b*12};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_line_to(gc, &p3);
	ps_line_to(gc, &p1);
	ps_set_source_color(gc, &c);
	ps_fill(gc);

	ps_rect r2 = {r->x+r->w/2-b*4, r->y+r->h-b*10, b*8, b};
	ps_rectangle(gc, &r2);
	ps_fill(gc);

	ps_rect r3 = {r->x+r->w/2-b*2, r->y+r->h-b*8, b*4, b};
	ps_rectangle(gc, &r3);
	ps_fill(gc);

	draw_light(gc, r);

	ps_context_unref(gc);
}

void ModeButton::OnDestroy(void)
{
	FloatButton::OnDestroy();
	ps_canvas_unref(m_face);
}

void ModeButton::OnPaint(ps_context* gc, const Rect* d)
{
	FloatButton::OnPaint(gc, d);

	Rect client(0, 0, width(), height());
	Rect * r = &client;

	if (m_down) {
		ps_save(gc);
		ps_color cb = {0.1, 0.1, 0.1, 1};
		ps_color t = {0.2, 0.2, 0.2, 1};
		ps_rect rc = {0, 0, width()-1, height()-1};
		ps_rectangle(gc, &rc);
		ps_set_source_color(gc, &cb);
		ps_set_stroke_color(gc, &t);
		ps_paint(gc);

		draw_border(gc, r);
		draw_event(gc, r);

		int b = TOOLBAR_HEIGHT/30;
		ps_color c = {1, 1, 1, 1};
		ps_point p1 = {r->x+r->w/2, r->y+b*6};
		ps_point p2 = {r->x+b*6, r->y+r->h-b*12};
		ps_point p3 = {r->x+r->w-b*6, r->y+r->h-b*12};
		ps_move_to(gc, &p1);
		ps_line_to(gc, &p2);
		ps_line_to(gc, &p3);
		ps_line_to(gc, &p1);
		ps_set_source_color(gc, &c);
		ps_fill(gc);

		ps_rect r2 = {r->x+r->w/2-b*4, r->y+r->h-b*10, b*8, b};
		ps_rectangle(gc, &r2);
		ps_fill(gc);

		ps_rect r3 = {r->x+r->w/2-b*2, r->y+r->h-b*8, b*4, b};
		ps_rectangle(gc, &r3);
		ps_fill(gc);

		draw_light(gc, r);
		ps_restore(gc);
	} else {
		ps_point xp = {x(), y()};
		ps_canvas * canvas = ps_context_get_canvas(gc);
		ps_canvas_bitblt(m_face, 0, canvas, &xp);
	}
}

void ModeButton::OnMouseEvent(const MouseEvent* e)
{
	FloatButton::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		m_down = true;
		Update(NULL);
	} else if (e->type() == MouseEvent::MouseUp) {
		m_down = false;
		Update(NULL);

		Widget::postEvent(this, EVENT_FUNC(ModeButton, button_event), 0);
	}
}

void ModeButton::button_event(void * p)
{
	m_main->getTabs()->setFullMode(false);
}

//back button
BackButton::BackButton(Widget* parent)
	: FloatButton(parent)
	, m_main(0)
	, m_face(0)
	, m_down(false)
{
}

BackButton::~BackButton()
{
}

void BackButton::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void BackButton::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	FloatButton::OnCreate(flags, x, y, w, h);

	m_face = ps_canvas_create(Application::getInstance()->color_format(), w, h);
	ps_context * gc = ps_context_create(m_face, 0);

	Rect client(0, 0, width(), height());
	Rect * r = &client;

	ps_color cb = {0.1, 0.1, 0.1, 1};
	ps_color t = {0.2, 0.2, 0.2, 1};
	ps_rect rc = {0, 0, width()-1, height()-1};
	ps_rectangle(gc, &rc);
	ps_set_source_color(gc, &cb);
	ps_set_stroke_color(gc, &t);
	ps_paint(gc);

	draw_border(gc, r);

	int b = TOOLBAR_HEIGHT/15;
	ps_color c = {1, 1, 1, 1};
	ps_point p1 = {r->x+b*3, r->y+r->h/2};
	ps_point p2 = {r->x+r->w/2, r->y+b*3};
	ps_point p3 = {r->x+r->w/2, r->y+r->h-b*3};
	ps_rect rp = {r->x+r->w/2, r->y+r->h/2-b/2*3, b*3, b*3};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_line_to(gc, &p3);
	ps_line_to(gc, &p1);
	ps_rectangle(gc, &rp);
	ps_set_source_color(gc, &c);
	ps_fill(gc);

	draw_light(gc, r);

	ps_context_unref(gc);
}

void BackButton::OnDestroy(void)
{
	FloatButton::OnDestroy();
	ps_canvas_unref(m_face);
}

void BackButton::OnPaint(ps_context* gc, const Rect* d)
{
	FloatButton::OnPaint(gc, d);

	Rect client(0, 0, width(), height());
	Rect * r = &client;

	if (m_down) {
		ps_save(gc);
		ps_color cb = {0.1, 0.1, 0.1, 1};
		ps_color t = {0.2, 0.2, 0.2, 1};
		ps_rect rc = {0, 0, width()-1, height()-1};
		ps_rectangle(gc, &rc);
		ps_set_source_color(gc, &cb);
		ps_set_stroke_color(gc, &t);
		ps_paint(gc);

		draw_border(gc, r);
		draw_event(gc, r);

		int b = TOOLBAR_HEIGHT/15;
		ps_color c = {1, 1, 1, 1};
		ps_point p1 = {r->x+b*3, r->y+r->h/2};
		ps_point p2 = {r->x+r->w/2, r->y+b*3};
		ps_point p3 = {r->x+r->w/2, r->y+r->h-b*3};
		ps_rect rp = {r->x+r->w/2, r->y+r->h/2-b/2*3, b*3, b*3};
		ps_move_to(gc, &p1);
		ps_line_to(gc, &p2);
		ps_line_to(gc, &p3);
		ps_line_to(gc, &p1);
		ps_rectangle(gc, &rp);
		ps_set_source_color(gc, &c);
		ps_fill(gc);

		draw_light(gc, r);
		ps_restore(gc);
	} else {
		ps_point xp = {x(), y()};
		ps_canvas * canvas = ps_context_get_canvas(gc);
		ps_canvas_bitblt(m_face, 0, canvas, &xp);
	}
}

void BackButton::OnMouseEvent(const MouseEvent* e)
{
	FloatButton::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		m_down = true;
		Update(NULL);
	} else if (e->type() == MouseEvent::MouseUp) {
		m_down = false;
		Update(NULL);

		Widget::postEvent(this, EVENT_FUNC(BackButton, button_event), 0);
	}
}

void BackButton::button_event(void * p)
{
	m_main->getTabs()->getActiveView()->goBack();
}

