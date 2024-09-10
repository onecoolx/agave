/* pagemgr.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <vector>
#include "config.h"
#include "picasso.h"
#include "pagemgr.h"
#include "webview.h"
#include "tabpage.h"
#include "mainwindow.h"

class PageMgrImpl
{
public:
	struct Item 
	{
		WebView* view;
		Rect	 trect;
		Rect	 crect;
	};
public:
	PageMgrImpl()
		: len(0), idx(0), max_idx(0), cur_item(0), cbtn(0)
		, uprc(0,0,0,0), dwrc(0,0,0,0), udbtn(0), event(0,0,0,0)
	{
	}

	~PageMgrImpl()
	{
		len = 0;
		idx = 0;
		max_idx = 0;
		pages.clear();
		cur_item = 0;
		cbtn = 0;
		uprc = Rect(0,0,0,0);
		dwrc = Rect(0,0,0,0);
		udbtn = 0;
		event = Rect(0,0,0,0);
	}
	unsigned int  len;
	unsigned int  idx;
	unsigned int  max_idx;
	std::vector<Item> pages;
	Item * cur_item;
	int cbtn;
	Rect uprc;
	Rect dwrc;
	int udbtn;
	Rect event;
};

PageManager::PageManager(Widget* parent)
	: Panel(parent)
	, m_impl(new PageMgrImpl)
	, m_main(0)
{
	setTitle(U("Ò³Ãæ¹ÜÀí"));
}

PageManager::~PageManager()
{
	delete m_impl;
}

void PageManager::draw_pageBtn(ps_context* gc, const Rect& r)
{
	int b = DASH_TITLE_HEIGHT/15;
	int rds = DASH_TITLE_HEIGHT/10;
	ps_rect tr = {r.x+b, r.y+b, r.w-b*2, r.h-b*2};

	ps_color c1 = {0.95, 0.95, 0.95, 1};
	ps_color c2 = {0.75, 0.75, 0.75, 1};
	ps_point s1 = {tr.x, tr.y};
	ps_point e1 = {tr.x, tr.y+tr.h/2};
	ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_REFLECT, &s1, &e1);
	ps_gradient_add_color_stop(gt, 0, &c1);
	ps_gradient_add_color_stop(gt, 1, &c2);
	ps_rounded_rect(gc, &tr, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_set_source_gradient(gc, gt);
	ps_fill(gc);
	ps_gradient_unref(gt);
}

void PageManager::draw_pageBtns(ps_context* gc, const Rect& rc)
{
	ps_point p[4] = {0};
	ps_color sc = {0.1, 0.1, 0.1, 1};
	ps_color fc = {0, 0.75, 1, 1};

	Rect rt;
	rt = m_impl->uprc;
	if (rt.intersect(rc)) {
		draw_pageBtn(gc, m_impl->uprc);

		ps_rect ur = m_impl->uprc;

		p[0].x = ur.x+ur.w/2; p[0].y = ur.y+ur.h/10*3;
		p[1].x = ur.x+ur.w/10*3; p[1].y = ur.y+ur.h/10*7;
		p[2].x = ur.x+ur.w/2; p[2].y = ur.y+ur.h/5*3;
		p[3].x = ur.x+ur.w/10*7; p[3].y = ur.y+ur.h/10*7;
		ps_move_to(gc, &p[0]);
		ps_line_to(gc, &p[1]);
		ps_line_to(gc, &p[2]);
		ps_line_to(gc, &p[3]);
		ps_line_to(gc, &p[0]);

		if (m_impl->udbtn == 1) {
			ps_set_source_color(gc, &fc);
			ps_fill(gc);
		} else {
			ps_set_source_color(gc, &sc);
			ps_fill(gc);
		}
	}

	rt = m_impl->dwrc;
	if (rt.intersect(rc)) {
		draw_pageBtn(gc, m_impl->dwrc);

		ps_rect dr = m_impl->dwrc;

		p[0].x = dr.x+dr.w/2; p[0].y = dr.y+dr.h-dr.h/10*3;
		p[1].x = dr.x+dr.w/10*3; p[1].y = dr.y+dr.h-dr.h/10*7;
		p[2].x = dr.x+dr.w/2; p[2].y = dr.y+dr.h-dr.h/5*3;
		p[3].x = dr.x+dr.w/10*7; p[3].y = dr.y+dr.h-dr.h/10*7;
		ps_move_to(gc, &p[0]);
		ps_line_to(gc, &p[1]);
		ps_line_to(gc, &p[2]);
		ps_line_to(gc, &p[3]);
		ps_line_to(gc, &p[0]);

		if (m_impl->udbtn == 2) {
			ps_set_source_color(gc, &fc);
			ps_fill(gc);
		} else {
			ps_set_source_color(gc, &sc);
			ps_fill(gc);
		}
	}
}

void PageManager::draw_lights(ps_context* gc, const Rect& r)
{
	int b = DASH_TITLE_HEIGHT/30;
	int rds = DASH_TITLE_HEIGHT/10;
	ps_color tc = {1, 1, 1, 0.8};
	ps_color tc2 = {1, 1, 1, 0.2};
	ps_rect lrc = {r.x+2*b, r.y+2*b, r.w-5*b, r.h/2-2*b};
	ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
	ps_point s1 = {lrc.x, lrc.y};
	ps_point e1 = {lrc.x, lrc.y+lrc.h};
	ps_gradient* gt = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
	ps_gradient_add_color_stop(gt, 0, &tc);
	ps_gradient_add_color_stop(gt, 1, &tc2);
	ps_set_source_gradient(gc, gt);
	ps_fill(gc);
	ps_gradient_unref(gt);
}

void PageManager::draw_closeBtn(ps_context* gc, const Rect& r)
{
	int b = DASH_TITLE_HEIGHT/30;
	ps_color c = {0.65, 0, 0, 1};
	ps_set_stroke_color(gc, &c);
	ps_set_line_width(gc, 4*b);

	ps_point p1 = {r.x+r.w/2-r.w/4, r.y+r.h/2-r.w/4};
	ps_point p2 = {r.x+r.w/2+r.w/4, r.y+r.h/2+r.w/4};
	ps_point p3 = {r.x+r.w/2+r.w/4, r.y+r.h/2-r.w/4};
	ps_point p4 = {r.x+r.w/2-r.w/4, r.y+r.h/2+r.w/4};

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_move_to(gc, &p3);
	ps_line_to(gc, &p4);

	ps_stroke(gc);
	ps_set_line_width(gc, 1);
}

void PageManager::draw_btnText(ps_context* gc, const ustring& text, const Rect& r)
{	
	int b = DASH_TITLE_HEIGHT/30;
	ps_save(gc);
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

	size_t len = 0;
	ps_size sz = ps_get_text_extent(gc, text.c_str(), text.length());

	ps_rect rc = {r.x, r.y, r.w-b*2, r.h};
	ps_clip_fast_rect(gc, &rc);

	if (sz.w > r.w) {
		len = (size_t)((r.w / (sz.w/text.length())) - 3);
		ustring t = text.substr(0, len) + ustring(U("..."));
		ps_wide_text_out_length(gc, r.x+DASH_TITLE_HEIGHT/5, 
							r.y+r.h/2-DASH_TITLE_HEIGHT/5, (ps_uchar16*)t.c_str(), t.length());
	} else {
		ps_wide_text_out_length(gc, r.x+DASH_TITLE_HEIGHT/5, 
							r.y+r.h/2-DASH_TITLE_HEIGHT/5, (ps_uchar16*)text.c_str(), text.length());
	}
	ps_set_font(gc, of);

	ps_restore(gc);
}

static void draw_current(ps_context* gc, const ps_rect* r)
{
	int rds = DASH_TITLE_HEIGHT/10;
	ps_color pc = {0.75, 0.75, 0.75, 1};
	ps_color c = {1, 1, 1, 1};
	ps_rounded_rect(gc, r, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_point s = {r->x, r->y};
	ps_point e = {r->x, r->y+r->h/2+r->h/3};
	ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
	ps_gradient_add_color_stop(g, 0.5, &pc);
	ps_gradient_add_color_stop(g, 1, &c);
	ps_set_source_gradient(gc, g);
	ps_fill(gc);
	ps_gradient_unref(g);
}

void PageManager::draw_itemBtns(ps_context* gc, const Rect& rc)
{
	int b = DASH_TITLE_HEIGHT/30;
	int rds = DASH_TITLE_HEIGHT/10;
	bool sbtn = false, cbtn = false; 
	unsigned int num_pages = m_impl->pages.size() - m_impl->idx * m_impl->len;
	unsigned int start = m_impl->idx * m_impl->len;
	unsigned int limit = num_pages > m_impl->len ? m_impl->len : num_pages;
	for (unsigned int i = 0; i < limit; i++){
		Rect tcr = m_impl->pages[start+i].trect;
		Rect ccr = m_impl->pages[start+i].crect;

		if (tcr.intersect(rc))
			sbtn = true;
		   
		if (ccr.intersect(rc))
			cbtn = true;

		if (!sbtn && !cbtn)
			continue;

		ps_rect r = m_impl->pages[start+i].trect;
		ps_rect r2 = m_impl->pages[start+i].crect;
		r.y+=b*2; r.h-=b*4;
		r2.y+=b*2; r2.h-=b*4;

		ps_color pc = {0.15, 0.15, 0.15, 1};
		WebView* v = m_impl->pages[start+i].view;

		if (v == m_main->getTabs()->getActiveView()) {
			//current display
			ps_color fc = {0, 0, 0, 1};
			ps_set_text_color(gc, &fc);
			ps_rect hrc = {r.x+b, r.y+b, r.w+r2.w-3*b, r.h-2*b};
			draw_current(gc, &hrc);
		} else {
			ps_color fc = {1, 1, 1, 1};
			ps_set_text_color(gc, &fc);
		}

		if (m_impl->cur_item == &(m_impl->pages[start+i])) {
			ps_color c;

			if (m_impl->cbtn == 1) {
				c.r = 0.3; c.g = 1; c.b = 0.1; c.a = 1;
				ps_rounded_rect(gc, &r, rds, rds, 0, 0, rds, rds, 0, 0);
				ps_color fc = {1, 1, 1, 1};
				ps_set_text_color(gc, &fc);
			} else {
				c.r = 1; c.g = 0; c.b = 0; c.a = 1;
				ps_rounded_rect(gc, &r2, 0, 0, rds, rds, 0, 0, rds, rds);
			}

			ps_point s = {r.x, r.y};
			ps_point e = {r.x, r.y+r.h/2+r.h/3};
			ps_gradient* g = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
			ps_gradient_add_color_stop(g, 0, &pc);
			ps_gradient_add_color_stop(g, 1, &c);
			ps_set_source_gradient(gc, g);
			ps_fill(gc);
			ps_gradient_unref(g);

		}

		ps_rect rc1 = {r.x-b, r.y-b, r.w, r.h+b};
		ps_rect rc2 = {r2.x-b, r2.y-b, r2.w, r2.h+b};
		ps_color sc = {0.5, 0.5, 0.5, 1};
		ps_rounded_rect(gc, &rc1, rds, rds, 0, 0, rds, rds, 0, 0);
		ps_rounded_rect(gc, &rc2, 0, 0, rds, rds, 0, 0, rds, rds);
		ps_set_line_width(gc, b);
		ps_set_stroke_color(gc, &sc);
		ps_stroke(gc);
		ps_set_line_width(gc, 1);

		if (!v->title().empty() && sbtn) {
			draw_btnText(gc, v->title(), r);
		}

		draw_closeBtn(gc, r2);

		ps_rect urc = {r.x-b, r.y-b, r.w+r2.w+2*b, r.h+2*b};
		draw_lights(gc, urc);	
	}
}

void PageManager::OnPaint(ps_context* gc, const Rect* r)
{
	Panel::OnPaint(gc, r);

	ps_save(gc);

	draw_itemBtns(gc, *r);

	draw_pageBtns(gc, *r);

	ps_restore(gc);
}

void PageManager::OnCreate(uint32_t f, int x, int y, int w, int h)
{
	Panel::OnCreate(f, x, y, w, h);

	int d = DASH_TITLE_HEIGHT/15;
	m_impl->uprc = Rect(w-BUTTON_HEIGHT+d, titleHeight()+d*2, BUTTON_HEIGHT-d*2, BUTTON_HEIGHT-d*2);
	m_impl->dwrc = Rect(w-BUTTON_HEIGHT+d, h-BUTTON_HEIGHT-d*2, BUTTON_HEIGHT-d*2, BUTTON_HEIGHT-d*2);
}

void PageManager::OnDestroy(void)
{
	Panel::OnDestroy();
}

void PageManager::OnShow(void)
{
	Panel::OnShow();
	loadBtns();
}

void PageManager::OnHide(void)
{
	Panel::OnHide();
	clearBtns();
}

void PageManager::loadBtns(void)
{
	int d = DASH_TITLE_HEIGHT/15;
	int p = DASH_TITLE_HEIGHT/10;
	int s = DASH_TITLE_HEIGHT/6;
	Rect client_rect(d, titleHeight()+d, width()-p, height()-titleHeight()-p);

	m_impl->len = client_rect.h / BUTTON_HEIGHT;
	int start = (client_rect.h - m_impl->len*BUTTON_HEIGHT)/2;
	unsigned int page_len = 0;
	m_impl->max_idx = 0;

	std::list<WebView*> views = m_main->getTabs()->getTabs();
	for (std::list<WebView*>::iterator it = views.begin(); it != views.end(); ++it) {
		if (page_len == m_impl->len) {
			m_impl->max_idx++;
			page_len = 0;
		}

		PageMgrImpl::Item item;
		item.view = (*it);
		item.trect = Rect(client_rect.x+s, client_rect.y+page_len*BUTTON_HEIGHT+start, 
									client_rect.w-BUTTON_HEIGHT-s*10, BUTTON_HEIGHT);
		item.crect = Rect(item.trect.x+item.trect.w, item.trect.y, BUTTON_HEIGHT*1.1, BUTTON_HEIGHT);
		m_impl->pages.push_back(item);
		page_len++;
	}

}

void PageManager::clearBtns(void)
{
	m_impl->len = 0;
	m_impl->idx = 0;
	m_impl->max_idx = 0;
	m_impl->pages.clear();
	m_impl->cur_item = 0;
	m_impl->cbtn = 0;
	m_impl->udbtn = 0;
}

void PageManager::OnMouseEvent(const MouseEvent* e)
{
	Panel::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		unsigned int num_pages = m_impl->pages.size() - m_impl->idx * m_impl->len;
		unsigned int start = m_impl->idx * m_impl->len;
		unsigned int limit = num_pages > m_impl->len ? m_impl->len : num_pages;
		for (unsigned int i = 0; i < limit; i++){
			if (m_impl->pages[start+i].trect.contains(e->x(), e->y()) 
				|| m_impl->pages[start+i].crect.contains(e->x(), e->y()))
			{
				if (m_impl->pages[start+i].trect.contains(e->x(), e->y())) {
					m_impl->cbtn = 1;
					m_impl->event = m_impl->pages[start+i].trect;
				 } else {
					m_impl->cbtn = 2;
					m_impl->event = m_impl->pages[start+i].crect;
				 }

				m_impl->cur_item = &(m_impl->pages[start+i]);
				break;
			}
		}

		if (m_impl->uprc.contains(e->x(), e->y())) {
			m_impl->udbtn = 1;
			m_impl->event = m_impl->uprc;
		} else if (m_impl->dwrc.contains(e->x(), e->y())) {
			m_impl->udbtn = 2;
			m_impl->event = m_impl->dwrc;
		} else {
			m_impl->udbtn = 0;
		}

		if (m_impl->cur_item || m_impl->udbtn) {
			Update(&m_impl->event);
		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (m_impl->udbtn) {

			if (m_impl->udbtn == 1){
				if (m_impl->idx > 0) {
					m_impl->idx--;
					Update(NULL);
				} else {
					Update(&m_impl->event);
				}
			} else if (m_impl->udbtn == 2){
				if (m_impl->idx < m_impl->max_idx) {
					m_impl->idx++;
					Update(NULL);
				} else {
					Update(&m_impl->event);
				}
			}
			m_impl->udbtn = 0;
			m_impl->event = Rect(0,0,0,0);
		} else if (m_impl->cur_item) {
			PageMgrImpl::Item * p = m_impl->cur_item;
			m_impl->cur_item = 0;

			if (m_impl->cbtn == 1) {
				Update(&m_impl->event);
				Widget::postEvent(this, EVENT_FUNC(PageManager, page_active), EVENT_PARAM(p->view));
			} else if (m_impl->cbtn == 2){
				Update(&m_impl->event);
				Widget::postEvent(this, EVENT_FUNC(PageManager, page_close), EVENT_PARAM(p->view));
			}

			m_impl->cbtn = 0;
			m_impl->event = Rect(0,0,0,0);
		}
	}
}

void PageManager::page_active(void* param)
{
	Hide();
	WebView* view = (WebView*)param;
	if (!m_main->getTabs()->isActiveView(view)){
		m_main->getTabs()->setActiveView(view);
	}
}

void PageManager::page_close(void* param)
{
	Hide();
	WebView* view = (WebView*)param;
	m_main->getTabs()->closeView(view);
}

void PageManager::setMainWindow(MainWindow* main)
{
	m_main = main;
}
