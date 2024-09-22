/* hismgr.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "hismgr.h"
#include "history.h"
#include "mainwindow.h"
#include "application.h"
#include "dialog.h"
#include "unicode.h"
#include "webview.h"
#include "tabpage.h"
#include "dialog.h"

class HistoryViewImpl
{
public:
	HistoryViewImpl()
		: g_icon(0)
		, g_btn(0)
		, t_mtx(0)
	{
		t_mtx = ps_matrix_create();
	}

	~HistoryViewImpl()
	{
		if (g_icon)
			ps_gradient_unref(g_icon);

		if (g_btn)
			ps_gradient_unref(g_btn);

		if (t_mtx)
			ps_matrix_unref(t_mtx);
	}

	ps_gradient* g_icon;
	ps_gradient* g_btn;
	ps_matrix* t_mtx;
};


HistoryView::HistoryView(Widget* parent)
	: ScrollView(parent)
	, m_impl(new HistoryViewImpl)
	, m_main(0)
	, m_page(0)
	, m_pageup(0,0,0,0)
	, m_pagedown(0,0,0,0)
	, m_enableup(false)
	, m_enabledown(false)
	, m_event(0,0,0,0)
	, m_btn(0)
	, m_pagebtn(0)
	, m_startX(0)
	, m_startY(0)
	, m_capture(false)
	, m_drag(false)
	, m_ticket(0)
	, m_time(1)
	, m_drx(0)
	, m_dry(0)
	, m_scx(0)
	, m_scy(0)
{
}

HistoryView::~HistoryView()
{
	delete m_impl;
}

void HistoryView::setMainWindow(MainWindow* main)
{
	m_main = main;
}

void HistoryView::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	ScrollView::OnCreate(flags, x, y, w, h);
	//icon gradient
	int b = TOOLBAR_HEIGHT/30;
	int hc = DASH_TITLE_HEIGHT*1.2-DASH_TITLE_HEIGHT/30*8;
	ps_color c1 = {1, 1, 1, 1};
	ps_color c2 = {0.5, 0.8, 1, 1};
	ps_point s = {0, 0};
	ps_point e = {b*9, b*12};
	m_impl->g_icon = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
	ps_gradient_add_color_stop(m_impl->g_icon, 0.3, &c1);
	ps_gradient_add_color_stop(m_impl->g_icon, 1, &c2);

	//btn gradient
	ps_color tc = {1, 1, 1, 0.8};
	ps_color tc2 = {1, 1, 1, 0.2};
	ps_point s1 = {0, 0};
	ps_point e1 = {0, hc/2-2*b};
	m_impl->g_btn = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
	ps_gradient_add_color_stop(m_impl->g_btn, 0, &tc);
	ps_gradient_add_color_stop(m_impl->g_btn, 1, &tc2);
}

void HistoryView::OnDestroy(void)
{
	ScrollView::OnDestroy();
}

void HistoryView::draw_icon(ps_context* gc, const ps_rect* r)
{
	int b = TOOLBAR_HEIGHT/30;
	ps_color c = {0.5,0.5,0.5,1};
	ps_set_stroke_color(gc, &c);
	ps_set_line_width(gc, b);
	ps_rect rc = {r->x+4*b, r->y+4*b, b*9, b*12};

	ps_point pt = {0, 0};

	pt.x = rc.x; pt.y = rc.y;
	ps_move_to(gc, &pt);

	pt.x = rc.x; pt.y = rc.y+rc.h;
	ps_line_to(gc, &pt);

	pt.x = rc.x+rc.w; pt.y = rc.y+rc.h;
	ps_line_to(gc, &pt);

	pt.x = rc.x+rc.w; pt.y = rc.y+rc.h/4;
	ps_line_to(gc, &pt);

	pt.x = rc.x+rc.w/4*3; pt.y = rc.y;
	ps_line_to(gc, &pt);

	pt.x = rc.x; pt.y = rc.y;
	ps_line_to(gc, &pt);

	pt.x = rc.x+rc.w/4*3; pt.y = rc.y;
	ps_move_to(gc, &pt);

	pt.x = rc.x+rc.w/4*3; pt.y = rc.y+rc.h/4;
	ps_line_to(gc, &pt);

	pt.x = rc.x+rc.w; pt.y = rc.y+rc.h/4;
	ps_line_to(gc, &pt);

	ps_matrix_translate(m_impl->t_mtx, rc.x, rc.y);
	ps_gradient_transform(m_impl->g_icon, m_impl->t_mtx);
	ps_set_source_gradient(gc, m_impl->g_icon);
	ps_paint(gc);
	ps_matrix_translate(m_impl->t_mtx, -rc.x*2, -rc.y*2);
	ps_gradient_transform(m_impl->g_icon, m_impl->t_mtx);
	ps_matrix_identity(m_impl->t_mtx);
	ps_set_line_width(gc, 1);
}

void HistoryView::OnPaint(ps_context* gc, const Rect* r)
{
	ScrollView::OnPaint(gc, r);

	if (!m_event.isEmpty()) {
		ps_font* f = MainWindow::getUIBoldFont();
		ps_font* uf = MainWindow::getUIFont();
#ifdef FONT_NO_ANTIALIAS
		ps_set_text_antialias(gc, False);
#else
		if (MainWindow::uiFontAntialias())
			ps_set_text_antialias(gc, True);
		else
			ps_set_text_antialias(gc, False);
#endif

		Rect tr;
		Rect pr = *r;
		pr.x += scrollX(); pr.y += scrollY();
		int b = TOOLBAR_HEIGHT/30;

		for(size_t i = 0; i < m_items.size(); i++) {
			ps_font* of = ps_set_font(gc, f);
			tr = m_items[i].rect;
			if (tr.intersect(pr)) {
				ps_rect rc = m_items[i].rect;
				rc.x -= scrollX(); rc.y -= scrollY();

				if (m_btn == m_items[i].id) {
					ps_color fc = {0.5,0.8,1,1};
					ps_rectangle(gc, &rc);
					ps_set_source_color(gc, &fc);
					ps_fill(gc);

					ps_rectangle(gc, &rc);
					ps_color c = {0.5,0.5,0.5,1};
					ps_set_line_width(gc, b);
					ps_set_stroke_color(gc, &c);
					ps_stroke(gc);
					ps_set_line_width(gc, 1);

					ps_color tc = {0,0,0,1};
					ps_set_text_color(gc, &tc);

					ps_size sz = {0};
                    ps_get_text_extent(gc, m_items[i].title.c_str(), m_items[i].title.length(), &sz);
					if (sz.w <= (rc.w-40*b)) {
						ps_wide_text_out_length(gc, b*20, rc.y+b*5, 
								(ps_uchar16*)m_items[i].title.c_str(), m_items[i].title.length());
					} else {
						size_t len = (rc.w-80*b) / (sz.w / m_items[i].title.length());
						ustring s = m_items[i].title.substr(0, len) + ustring(U("..."));
						ps_wide_text_out_length(gc, b*20, rc.y+b*5, (ps_uchar16*)s.c_str(), s.length());
					}

					ps_color uc = {0,0,1,1};
					ps_set_text_color(gc, &uc);

					ps_set_font(gc, of);
					ps_font* of = ps_set_font(gc, uf);

					ps_get_text_extent(gc, m_items[i].url.c_str(), m_items[i].url.length(), &sz);
					if (sz.w <= (rc.w-40*b)) {
						ps_wide_text_out_length(gc, b*20, rc.y+b*8+b*12, 
								(ps_uchar16*)m_items[i].url.c_str(), m_items[i].url.length());
					} else {
						size_t len = (rc.w-40*b) / (sz.w / m_items[i].url.length());
						ustring s = m_items[i].url.substr(0, len) + ustring(U("..."));
						ps_wide_text_out_length(gc, b*20, rc.y+b*8+b*12, (ps_uchar16*)s.c_str(), s.length());
					}

					draw_icon(gc, &rc);
				}	
			}
			ps_set_font(gc, of);
		}

		ps_font* of = ps_set_font(gc, uf);

		tr = m_pageup;
		if (tr.intersect(pr)) {
			Rect brc = m_pageup;
			brc.x -= scrollX(); brc.y -= scrollY();
			draw_button(gc, U("<<ÉÏÒ»Ò³"), brc, m_enableup, (m_pagebtn == 1) ? true : false);
		}

		tr = m_pagedown;
		if (tr.intersect(pr)) {
			Rect brc = m_pagedown;
			brc.x -= scrollX(); brc.y -= scrollY();
			draw_button(gc, U("ÏÂÒ»Ò³>>"), brc, m_enabledown, (m_pagebtn == 2) ? true : false);
		}

		ps_set_font(gc, of);
	}
}

void HistoryView::OnPaintContents(ps_context* gc, const Rect* r)
{
	ps_font* f = MainWindow::getUIBoldFont();
	ps_font* uf = MainWindow::getUIFont();
#ifdef FONT_NO_ANTIALIAS
	ps_set_text_antialias(gc, False);
#else
	if (MainWindow::uiFontAntialias())
		ps_set_text_antialias(gc, True);
	else
		ps_set_text_antialias(gc, False);
#endif
	
	Rect tr;
	int b = TOOLBAR_HEIGHT/30;

	for(size_t i = 0; i < m_items.size(); i++) {
		ps_font* of = ps_set_font(gc, f);
		tr = m_items[i].rect;
		if (tr.intersect(*r)) {
			ps_rect rc = m_items[i].rect;

			ps_rectangle(gc, &rc);
			ps_color c = {0.5,0.5,0.5,1};
			ps_set_line_width(gc, b);
			ps_set_stroke_color(gc, &c);
			ps_stroke(gc);
			ps_set_line_width(gc, 1);

			ps_color tc = {0,0,0,1};
			ps_set_text_color(gc, &tc);

			ps_size sz = {0};
            ps_get_text_extent(gc, m_items[i].title.c_str(), m_items[i].title.length(), &sz);
			if (sz.w <= (rc.w-40*b)) {
				ps_wide_text_out_length(gc, b*20, rc.y+b*5, 
								(ps_uchar16*)m_items[i].title.c_str(), m_items[i].title.length());
			} else {
				size_t len = (rc.w-80*b) / (sz.w / m_items[i].title.length());
				ustring s = m_items[i].title.substr(0, len) + ustring(U("..."));
				ps_wide_text_out_length(gc, b*20, rc.y+b*5, (ps_uchar16*)s.c_str(), s.length());
			}

			ps_color uc = {0,0,1,1};
			ps_set_text_color(gc, &uc);

			ps_set_font(gc, of);
			ps_font* of = ps_set_font(gc, uf);

			ps_get_text_extent(gc, m_items[i].url.c_str(), m_items[i].url.length(), &sz);
			if (sz.w <= (rc.w-40*b)) {
				ps_wide_text_out_length(gc, b*20, rc.y+b*8+b*12, 
							(ps_uchar16*)m_items[i].url.c_str(), m_items[i].url.length());
			} else {
				size_t len = (rc.w-40*b) / (sz.w / m_items[i].url.length());
				ustring s = m_items[i].url.substr(0, len) + ustring(U("..."));
				ps_wide_text_out_length(gc, b*20, rc.y+b*8+b*12, (ps_uchar16*)s.c_str(), s.length());
			}

			draw_icon(gc, &rc);
		}
		ps_set_font(gc, of);
	}

	ps_font* of = ps_set_font(gc, uf);

	tr = m_pageup;
	if (tr.intersect(*r))
		draw_button(gc, U("<<ÉÏÒ»Ò³"), m_pageup, m_enableup, (m_pagebtn == 1) ? true : false);

	tr = m_pagedown;
	if (tr.intersect(*r))
		draw_button(gc, U("ÏÂÒ»Ò³>>"), m_pagedown, m_enabledown, (m_pagebtn == 2) ? true : false);

	ps_set_font(gc, of);
}

void HistoryView::draw_button(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight)
{
	int rds = DASH_TITLE_HEIGHT/5;
	int b = DASH_TITLE_HEIGHT/30;
	ps_rect rc = r;
	if (enable) {
		if (highlight) {
			ps_color c = {0, 1, 0, 1};	
			ps_set_source_color(gc, &c);
		} else {
			ps_color c = {0, 0.6, 0, 1};	
			ps_set_source_color(gc, &c);
		}
	} else {
		ps_color c = {0.5, 0.5, 0.5, 1};	
		ps_set_source_color(gc, &c);
	}
	ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_fill(gc);

	ps_color cc = {0,0,0,1};
	ps_set_text_color(gc, &cc);

	ps_wide_text_out_length(gc, r.x+r.w/2-b*25, r.y+r.h/2-b*6, (ps_uchar16*)text.c_str(), text.length());

	ps_rect lrc = {r.x+b, r.y+b, r.w-2*b, r.h/2-2*b};
	ps_rounded_rect(gc, &lrc, rds, rds, rds, rds, 0, 0, 0, 0);
	ps_matrix_translate(m_impl->t_mtx, lrc.x, lrc.y);
	ps_gradient_transform(m_impl->g_btn, m_impl->t_mtx);
	ps_set_source_gradient(gc, m_impl->g_btn);
	ps_fill(gc);
	ps_matrix_translate(m_impl->t_mtx, -lrc.x*2, -lrc.y*2);
	ps_gradient_transform(m_impl->g_btn, m_impl->t_mtx);
	ps_matrix_identity(m_impl->t_mtx);
}

void HistoryView::build(int offset)
{
	m_items.clear();

	size_t i = 0;
	std::vector<History::Item> items = Application::getInstance()->getHistory()->getHistoryItems(m_page*PAGE_ITEMS, PAGE_ITEMS+1);
	for (i=0; i<items.size(); i++){
		if (i == PAGE_ITEMS+1)
			break;
		HistoryItem item;
		item.id = items[i].id;
		item.rect = Rect(0, i*TOOLBAR_HEIGHT*1.2, width()-1, TOOLBAR_HEIGHT*1.2);
		item.title = Unicode::ConvertUTF8ToUTF16(items[i].title);
		item.url = Unicode::ConvertUTF8ToUTF16(items[i].url);
		m_items.push_back(item);
	}

	int view_height = i*TOOLBAR_HEIGHT*1.2;
	int hc = DASH_TITLE_HEIGHT*1.2-DASH_TITLE_HEIGHT/30*8;
	int cell_height = DASH_TITLE_HEIGHT*1.2;
	int border = DASH_TITLE_HEIGHT/30;
	m_pageup = Rect(border*10, view_height+(cell_height-hc)/2, width()/3-border*10, hc);
	m_pagedown = Rect(width()/3*2, view_height+(cell_height-hc)/2, width()/3-border*10, hc);

	if (offset)
		m_enableup = true;
	else
		m_enableup = false;

	if (items.size() > 20)
		m_enabledown = true;
	else
		m_enabledown = false;

	setContentSize(width(), i*TOOLBAR_HEIGHT*1.2 + DASH_TITLE_HEIGHT*1.2);
}

void HistoryView::reset(void* p)
{
	m_page = 0;
	refresh(p);
}

void HistoryView::refresh(void* p)
{
	setScrollPoint(0, 0);
	m_scx = 0;
	m_scy = 0;
	build(m_page);
	Update(NULL);
}

void HistoryView::OnShow(void)
{
	ScrollView::OnShow();
	m_scx = 0;
	m_scy = 0;
	setScrollPoint(0, 0);
	m_page = 0;
	build(m_page);
}

void HistoryView::OnIdle(void)
{
	ScrollView::OnIdle();
	m_ticket++;
	if (m_ticket > 20) {
		m_ticket = 0;
		m_time++;
	}

	if (m_scx || m_scy) {
		int b = TOOLBAR_HEIGHT/30;
		scrollContents(-m_scx/5, -m_scy/5);
		if (m_scx) ABS_DEC(m_scx, 10*b);
		if (m_scy) ABS_DEC(m_scy, 10*b);
	}
}

void HistoryView::item_click(void* p)
{
	int i = (intptr_t)p;
	std::string url = Unicode::ConvertUTF16ToUTF8(m_items[i].url);
	m_main->getTabs()->getActiveView()->loadUrl(url);
}

void HistoryView::OnMouseEvent(const MouseEvent* e)
{
	ScrollView::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		for (size_t i=0; i<m_items.size(); i++) {
			if (m_items[i].rect.contains(e->x()+scrollX(), e->y()+scrollY())) {
				m_btn = m_items[i].id;
				m_event = m_items[i].rect;
				m_event.x -= scrollX(); m_event.y -= scrollY();
			}
		}

		if (!m_event.isEmpty() && m_btn) {
			Update(&m_event);	
		} else if (m_pageup.contains(e->x()+scrollX(), e->y()+scrollY()) && m_enableup) {
			m_pagebtn = 1;
			m_event = m_pageup;
			m_event.x -= scrollX(); m_event.y -= scrollY();
			Update(&m_event);
		} else if (m_pagedown.contains(e->x()+scrollX(), e->y()+scrollY()) && m_enabledown) {
			m_pagebtn = 2;
			m_event = m_pagedown;
			m_event.x -= scrollX(); m_event.y -= scrollY();
			Update(&m_event);
		}

		m_drx = e->x();
		m_dry = e->y();
		m_time = 1;
		m_startX = e->x();
		m_startY = e->y();
		m_capture = true;
	} else if (e->type() == MouseEvent::MouseMove) {
		if (m_capture) {
			int xpos = e->x();
			int ypos = e->y();

			int offsetX = m_startX - xpos;
			int offsetY = m_startY - ypos;

			if (ABS(offsetX) > MAX_DRAG_STEP || ABS(offsetY) > MAX_DRAG_STEP) {
				m_btn = 0;
				m_event = Rect(0,0,0,0);
				scrollContents(offsetX, offsetY);
				m_startX = e->x();
				m_startY = e->y();
				m_drag = true;
			}

		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (m_drag) {
			m_drag = false;
			int b = TOOLBAR_HEIGHT/30;

			int xs = (e->x() - m_drx) / m_time;
			int ys = (e->y() - m_dry) / m_time;

			if (xs) {
				int vxs = (ABS(xs)-50*b > 0) ? (ABS(xs)-50*b) : 0;
				m_scx = xs/ABS(xs) * vxs;
			} else {
				m_scx = 0;
			}

			if (ys) {
				int vys = (ABS(ys)-50*b > 0) ? (ABS(ys)-50*b) : 0;
				m_scy = ys/ABS(ys) * vys;
			} else {
				m_scy = 0;
			}
		} else {

			if (!m_event.isEmpty() && m_btn) {
				int sid = m_btn;
				Update(&m_event);
				m_btn = 0;
				m_event = Rect(0,0,0,0);
				//load the url
				for (size_t i=0; i<m_items.size(); i++) {
					if (m_items[i].id == sid) {
						Widget::postEvent(this, EVENT_FUNC(HistoryView, item_click), EVENT_PARAM(i));
						parent()->Hide();
						break;
					}
				}
			} else if (m_pagebtn) {
				int btn = m_pagebtn;
				m_pagebtn = 0;
				if (btn == 1) {
					m_page--;
					Update(&m_event);
				} else if (btn == 2) {
					m_page++;
					Update(&m_event);
				}

				m_event = Rect(0,0,0,0);

				Widget::postEvent(this, EVENT_FUNC(HistoryView, refresh), 0);
			}
		}

		m_drx = 0;
		m_dry = 0;
		m_time = 1;
		m_startX = 0;
		m_startY = 0;
		m_capture = false;
	}
}

//history manager
HistoryManager::HistoryManager(Widget* parent)
	: DashLayer(parent)
	, m_main(0)
	, m_view(0)
	, m_clear(0,0,0,0)
	, m_event(0,0,0,0)
	, m_enable(false)
	, m_btn(0)
{
	setTitle(U("ÀúÊ·"));
	setCommitText(U("·µ»Ø"));
	setCancel(false);

	m_view = new HistoryView(this);
}

HistoryManager::~HistoryManager()
{
	delete m_view;
}

void HistoryManager::setMainWindow(MainWindow* main)
{
	m_main = main;
	m_view->setMainWindow(main);
}

void HistoryManager::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	DashLayer::OnCreate(flags, x, y, w, h);

	int hc = DASH_TITLE_HEIGHT*1.2-DASH_TITLE_HEIGHT/30*8;
	int cell_height = DASH_TITLE_HEIGHT*1.2;
	int border = DASH_TITLE_HEIGHT/30;

	m_clear = Rect(width()/3*2, DASH_TITLE_HEIGHT+(cell_height-hc)/2, width()/3-border*10, hc);

	m_view->OnCreate(WF_VISIBLE|WF_ENABLED, 0, DASH_TITLE_HEIGHT+cell_height, w, h-DASH_TITLE_HEIGHT*2-cell_height);

	addChild(m_view);
}

void HistoryManager::OnShow(void)
{
	DashLayer::OnShow();
	if (Application::getInstance()->getHistory()->getHistoryCount())
		m_enable = true;
	else
		m_enable = false;

	m_view->OnShow();
}

void HistoryManager::OnMouseEvent(const MouseEvent* e)
{
	DashLayer::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		if (m_clear.contains(e->x(), e->y()) && m_enable) {
			m_event = m_clear;
			m_btn = 1;
		} else {
			m_event = Rect(0,0,0,0);
			m_btn = 0;
		}

		if (m_btn) {
			Update(&m_event);
		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (m_btn) {
			m_btn = 0;

			if (Dialog::ConfirmBox(m_main, U("È·¶¨ÒªÉ¾³ýÂð?"), U("ÌáÊ¾"))) {
				Application::getInstance()->getHistory()->clearAllHistory();
				m_enable = false;
				Widget::postEvent(m_view, EVENT_FUNC(HistoryView, reset), 0);
			}
			
			Update(&m_event);
			m_event = Rect(0,0,0,0);
		}
	}
}

void HistoryManager::OnPaint(ps_context* gc, const Rect* r)
{
	DashLayer::OnPaint(gc, r);

	ps_save(gc);

	Rect tr;
	int cell_height = DASH_TITLE_HEIGHT*1.2;
	ps_rect rc = {0, DASH_TITLE_HEIGHT, width(), cell_height};

	tr = rc;
	if (tr.intersect(*r)) {
		ps_rectangle(gc, &rc);
		ps_color c = {0.8, 0.8, 0.8, 1};
		ps_set_source_color(gc, &c);
		ps_fill(gc);
	}

	tr = m_clear;
	if (tr.intersect(*r)) {
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
		draw_button(gc, m_clear, m_enable, (m_btn == 1) ? true : false);
		ps_set_font(gc, of);
	}
	ps_restore(gc);
}

void HistoryManager::draw_button(ps_context* gc, const Rect& r, bool enable, bool highlight)
{
	int rds = DASH_TITLE_HEIGHT/5;
	int b = DASH_TITLE_HEIGHT/30;
	ps_rect rc = r;
	if (enable) {
		if (highlight) {
			ps_color c = {0.3, 0.8, 1, 1};	
			ps_set_source_color(gc, &c);
		} else {
			ps_color c = {0, 0.5, 1, 1};	
			ps_set_source_color(gc, &c);
		}
	} else {
		ps_color c = {0.5, 0.5, 0.5, 1};	
		ps_set_source_color(gc, &c);
	}
	ps_rounded_rect(gc, &rc, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_fill(gc);

	ps_wide_text_out_length(gc, r.x+r.w/2-b*12, r.y+r.h/2-b*6, (ps_uchar16*)U("Çå³ý"), 2);

	ps_color tc = {1, 1, 1, 0.8};
	ps_color tc2 = {1, 1, 1, 0.2};
	ps_rect lrc = {r.x+b, r.y+b, r.w-2*b, r.h/2-2*b};
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
