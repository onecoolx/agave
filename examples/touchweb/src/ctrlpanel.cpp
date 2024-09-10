/* ctrlpanel.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <math.h>
#include "config.h"
#include "picasso.h"
#include "ctrlpanel.h"
#include "tabpage.h"
#include "webview.h"
#include "setting.h"
#include "search.h"
#include "hismgr.h"
#include "favmgr.h"
#include "mainwindow.h"

class CtrlPanelImpl
{
public:
	CtrlPanelImpl()
		: g_light(0)
		, g_event(0)
		, t_mtx(0)
	{
		t_mtx = ps_matrix_create();
	}

	~CtrlPanelImpl()
	{
		if (g_light)
			ps_gradient_unref(g_light);

		if (g_event)
			ps_gradient_unref(g_event);

		if (t_mtx)
			ps_matrix_unref(t_mtx);
	}

	ps_gradient* g_light;
	ps_gradient* g_event;
	ps_matrix* t_mtx;
};

CtrlPanel::CtrlPanel(Widget* parent)
	: Panel(parent)
	, m_impl(new CtrlPanelImpl)
	, m_hisBtn(0,0,0,0)
	, m_favBtn(0,0,0,0)
	, m_fullBtn(0,0,0,0)	
	, m_newBtn(0,0,0,0)
	, m_exitBtn(0,0,0,0)
	, m_confBtn(0,0,0,0)
	, m_modeBtn(0,0,0,0)
	, m_searchBtn(0,0,0,0)
	, m_eventRect(0,0,0,0)
	, m_main(0)
	, m_btn(0)
{
	setTitle(U("功能面板"));
}

CtrlPanel::~CtrlPanel()
{
	delete m_impl;
}

static void draw_history_btn(ps_context* gc, const ps_rect& r)
{
	int br = DASH_TITLE_HEIGHT/15;
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);

	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   								DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};
	ps_ellipse(gc, &xr);
	ps_set_line_width(gc, br);
	ps_stroke(gc);

	ps_point p1 = {xr.x+xr.w/2, xr.y+xr.h/2};
	ps_point p2 = {xr.x+xr.w/2+DASH_TITLE_HEIGHT/6, xr.y+br};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_stroke(gc);

	ps_point p3 = {xr.x+xr.w/2, xr.y+xr.h/2};
	ps_point p4 = {xr.x+br, xr.y+xr.h/2-br};
	ps_move_to(gc, &p3);
	ps_line_to(gc, &p4);
	ps_stroke(gc);

	ps_rect xr2 = {xr.x+xr.w/2-br/2, xr.y+xr.h/2-br/2, br, br};
	ps_ellipse(gc, &xr2);
	ps_stroke(gc);

	ps_set_text_color(gc, &c);
	ps_set_line_width(gc, br/2);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("历史"), 2);
}

static void draw_favorite_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_source_color(gc, &c);

	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};

	ps_point p[5] = {0};
	double angle = 90;

	for (int i = 0; i < 5; i++){
		p[i].x = xr.x + xr.w/2*(1+cos(angle/180*3.14));
		p[i].y = xr.y + xr.h/2*(1-sin(angle/180*3.14));

		angle += 360/5*2;
		if (angle > 360)
			angle -= 360;
	}

	double gamma = ps_set_gamma(gc, 0.3);
	ps_move_to(gc, &p[0]);
	ps_line_to(gc, &p[1]);
	ps_line_to(gc, &p[2]);
	ps_line_to(gc, &p[3]);
	ps_line_to(gc, &p[4]);
	ps_fill(gc);
	ps_set_gamma(gc, gamma);

	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
							r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("收藏"), 2);
}

static void draw_full_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);
	ps_set_source_color(gc, &c);

	int br = DASH_TITLE_HEIGHT/15;
	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};
	ps_rect tr = {xr.x+br, xr.y+br, xr.w-br*2, xr.h-br*2};

	ps_set_line_width(gc, br/2);
	ps_rectangle(gc, &tr);
	ps_stroke(gc);
	ps_set_line_width(gc, 1);
	
	tr.h = DASH_TITLE_HEIGHT/10;
	ps_rectangle(gc, &tr);
	ps_fill(gc);

	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("全屏"), 2);
}

static void draw_newpage_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);

	int br = DASH_TITLE_HEIGHT/30;
	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};

	double gamma = ps_set_gamma(gc, 0.3);
	ps_set_line_width(gc, br*4);
	ps_point p1 = {xr.x+xr.w/2, xr.y+br};
	ps_point p2 = {xr.x+xr.w/2, xr.y+xr.h-br};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_stroke(gc);
	ps_point p3 = {xr.x+br, xr.y+xr.h/2};
	ps_point p4 = {xr.x+xr.w-br, xr.y+xr.h/2};
	ps_move_to(gc, &p3);
	ps_line_to(gc, &p4);
	ps_stroke(gc);
	ps_set_gamma(gc, gamma);

	ps_set_line_width(gc, 1);
	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*3, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("新页面"), 3);
}

static void draw_exit_btn(ps_context* gc, const ps_rect& r)
{
	int br = DASH_TITLE_HEIGHT/15;
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);

	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};
	ps_ellipse(gc, &xr);

	ps_point p1 = {r.x+r.w/2, r.y+DASH_TITLE_HEIGHT/3};
	ps_point p2 = {r.x+r.w/2, r.y+DASH_TITLE_HEIGHT/3*2};

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);

	ps_set_line_cap(gc, LINE_CAP_ROUND);
	ps_set_line_width(gc, br);
	ps_stroke(gc);

	ps_set_line_cap(gc, LINE_CAP_BUTT);
	ps_set_line_width(gc, 1);
	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("退出"), 2);
}

static void draw_config_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_source_color(gc, &c);

	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};

	ps_point p[22] = {
		{xr.w/2, xr.h/20*3},
		{xr.w/2+xr.w/10, xr.h/20},
		{xr.w/2+xr.w/10*3, xr.h/20},
		{xr.w/2+xr.w/10*2, xr.h/20*3},
		{xr.w/2+xr.w/10*2, xr.h/10*3},
		{xr.w/2+xr.w/20*7, xr.h/10*3},
		{xr.w/2+xr.w/20*9, xr.h/10*2},
		{xr.w/2+xr.w/20*9, xr.h/10*4},
		{xr.w/2+xr.w/20*7, xr.h/2},
		{xr.w/2+xr.w/10*2, xr.h/2},
		{xr.w/2, xr.h/2+xr.h/10*2},
		{xr.w/2, xr.h/2+xr.h/20*7},
		{xr.w/10*4, xr.h/2+xr.h/20*9},
		{xr.w/10*2, xr.h/2+xr.h/20*9},
		{xr.w/10*3, xr.h/2+xr.h/20*7},
		{xr.w/10*3, xr.h/2+xr.h/10*2},
		{xr.w/20*3, xr.h/2+xr.h/10*2},
		{xr.w/20*1, xr.h/2+xr.h/10*3},
		{xr.w/20*1, xr.h/2+xr.h/10},
		{xr.w/20*3, xr.h/2},
		{xr.w/20*7, xr.h/2},
		{xr.w/2, xr.h/20*7}};

	ps_point px = {xr.x+p[0].x, xr.y+p[0].y};
	ps_move_to(gc, &px);
	for(int i = 1; i < 22; i++){
		px.x = xr.x+p[i].x;
	   	px.y = xr.y+p[i].y;
		ps_line_to(gc, &px);
	}
	ps_fill(gc);

	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("设置"), 2);
}

static void draw_mode_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);

	int br = DASH_TITLE_HEIGHT/15;
	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};
	ps_rect tr = {xr.x+br, xr.y+br, xr.w-br*2, xr.h-br*2};

	ps_set_line_width(gc, br/2);
	ps_rectangle(gc, &tr);
	ps_stroke(gc);

	ps_point p1 = {tr.x+tr.w*2/5, tr.y};
	ps_point p2 = {tr.x+tr.w*2/5, tr.y+tr.h};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_stroke(gc);

	p1.x = tr.x+tr.w*2/5; p1.y = tr.y+tr.h/5;
	p2.x = tr.x+tr.w*2/5+tr.w/5*3; p2.y = tr.y+tr.h/5;

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);

	p1.x = tr.x+tr.w*2/5; p1.y = tr.y+tr.h/5*2;
	p2.x = tr.x+tr.w*2/5+tr.w/5*3; p2.y = tr.y+tr.h/5*2;

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);

	p1.x = tr.x+tr.w*2/5; p1.y = tr.y+tr.h/5*3;
	p2.x = tr.x+tr.w*2/5+tr.w/5*3; p2.y = tr.y+tr.h/5*3;

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);

	p1.x = tr.x+tr.w*2/5; p1.y = tr.y+tr.h/5*4;
	p2.x = tr.x+tr.w*2/5+tr.w/5*3; p2.y = tr.y+tr.h/5*4;

	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);

	ps_stroke(gc);
	ps_set_line_width(gc, 1);

	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("模式"), 2);
}

static void draw_search_btn(ps_context* gc, const ps_rect& r)
{
	ps_color c = {1, 1, 1, 1};
	ps_set_stroke_color(gc, &c);

	ps_rect xr = {r.x+r.w/2-DASH_TITLE_HEIGHT/3, r.y+DASH_TITLE_HEIGHT/6,
	   						DASH_TITLE_HEIGHT/3*2, DASH_TITLE_HEIGHT/3*2};

	int br = DASH_TITLE_HEIGHT/15;
	ps_set_line_cap(gc, LINE_CAP_ROUND);
	ps_set_line_width(gc, br);

	ps_rect tr = {xr.x+br, xr.y+br, xr.w/5*3, xr.h/5*3};
	ps_ellipse(gc, &tr);
	ps_stroke(gc);

	ps_point p1 = {tr.x+tr.w/2+0.707*tr.w/2, tr.y+tr.h/2+0.707*tr.h/2};
	ps_point p2 = {xr.x+xr.w-br, xr.y+xr.h-br};
	ps_move_to(gc, &p1);
	ps_line_to(gc, &p2);
	ps_stroke(gc);

	ps_set_text_color(gc, &c);
	ps_wide_text_out_length(gc, r.x+r.w/2-DASH_TITLE_HEIGHT/5*2, 
								r.y+r.h-DASH_TITLE_HEIGHT/2, (ps_uchar16*)U("搜索"), 2);
}

void CtrlPanel::draw_buttons(ps_context* gc, const Rect& r)
{
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

	Rect rt;	

	rt = m_hisBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_hisBtn);
		draw_history_btn(gc, m_hisBtn);
	}

	rt = m_favBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_favBtn);
		draw_favorite_btn(gc, m_favBtn);
	}

	rt = m_fullBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_fullBtn);
		draw_full_btn(gc, m_fullBtn);
	}

	rt = m_newBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_newBtn);
		draw_newpage_btn(gc, m_newBtn);
	}

	rt = m_exitBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_exitBtn);
		draw_exit_btn(gc, m_exitBtn);
	}

	rt = m_confBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_confBtn);
		draw_config_btn(gc, m_confBtn);
	}

	rt = m_modeBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_modeBtn);
		draw_mode_btn(gc, m_modeBtn);
	}

	rt = m_searchBtn;
	if (rt.intersect(r)) {
		draw_button_face(gc, m_searchBtn);
		draw_search_btn(gc, m_searchBtn);
	}

	ps_set_font(gc, of);
}

void CtrlPanel::draw_button_face(ps_context* gc, const Rect& rc)
{
	int rds = DASH_TITLE_HEIGHT/10;
	ps_set_line_width(gc, DASH_TITLE_HEIGHT/30);
	ps_rect r = {rc.x+rds, rc.y, rc.w-rds*2, rc.h};
	ps_color c = {0.5, 0.5, 0.5, 1};
	ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
	ps_set_stroke_color(gc, &c);
	ps_stroke(gc);
	ps_set_line_width(gc, 1);

	ps_point t1 = {r.x + rds, r.y + r.h/2-rds*2};
	ps_point t2 = {r.x + rds*8, r.y + rds};
	ps_point t3 = {r.x + rds*4, r.y + r.h/2-rds*5};
	ps_rect tr = {rc.x+rds*2, rc.y+rds, rds*2, rds*2};
	ps_move_to(gc, &t1);
	ps_tangent_arc(gc, &tr, 3.14, 1.57);
	ps_line_to(gc, &t2);
	ps_quad_curve_to(gc, &t3, &t1);

	ps_matrix_translate(m_impl->t_mtx, r.x, r.y);
	ps_gradient_transform(m_impl->g_light, m_impl->t_mtx);
	ps_set_source_gradient(gc, m_impl->g_light);
	ps_fill(gc);
	ps_matrix_translate(m_impl->t_mtx, -r.x*2, -r.y*2);
	ps_gradient_transform(m_impl->g_light, m_impl->t_mtx);
	ps_matrix_reset(m_impl->t_mtx);
}

void CtrlPanel::draw_event(ps_context* gc)
{
	if (!m_eventRect.isEmpty()) {
		int rds = DASH_TITLE_HEIGHT/10;
		ps_rect r = {m_eventRect.x+rds, m_eventRect.y, m_eventRect.w-rds*2, m_eventRect.h};
		ps_rounded_rect(gc, &r, rds, rds, rds, rds, rds, rds, rds, rds);
		ps_matrix_translate(m_impl->t_mtx, r.x, r.y);
		ps_gradient_transform(m_impl->g_event, m_impl->t_mtx);
		ps_set_source_gradient(gc, m_impl->g_event);
		ps_fill(gc);
		ps_matrix_translate(m_impl->t_mtx, -r.x*2, -r.y*2);
		ps_gradient_transform(m_impl->g_event, m_impl->t_mtx);
		ps_matrix_reset(m_impl->t_mtx);
	}
}

void CtrlPanel::OnPaint(ps_context* gc, const Rect* r)
{
	Panel::OnPaint(gc, r);
	//TODO: add paint code here.
	ps_save(gc);

	draw_event(gc);

	draw_buttons(gc, *r);

	ps_restore(gc);
}

void CtrlPanel::OnCreate(uint32_t f, int x, int y, int w, int h)
{
	Panel::OnCreate(f, x, y, w, h);

	int box_width = (w - DASH_TITLE_HEIGHT/3) / 4;
	int box_height = (h - DASH_TITLE_HEIGHT) /2;
	int cur_loc = DASH_TITLE_HEIGHT/15;
	int border = DASH_TITLE_HEIGHT/15;
	int rds = DASH_TITLE_HEIGHT/10;

	m_hisBtn.x = cur_loc; m_hisBtn.y = titleHeight()+border; m_hisBtn.w = box_width; 
	m_hisBtn.h = box_height; cur_loc += (box_width+border);

	m_favBtn.x = cur_loc; m_favBtn.y = titleHeight()+border; m_favBtn.w = box_width; 
	m_favBtn.h = box_height; cur_loc += (box_width+border);

	m_fullBtn.x = cur_loc; m_fullBtn.y = titleHeight()+border; m_fullBtn.w = box_width; 
	m_fullBtn.h = box_height; cur_loc += (box_width+border);

	m_newBtn.x = cur_loc; m_newBtn.y = titleHeight()+border; m_newBtn.w = box_width; 
	m_newBtn.h = box_height; cur_loc += (box_width+border);

	cur_loc = DASH_TITLE_HEIGHT/15;

	m_exitBtn.x = cur_loc; m_exitBtn.y = box_height+titleHeight()+border*2; m_exitBtn.w = box_width; 
	m_exitBtn.h = box_height; cur_loc += (box_width+border);

	m_confBtn.x = cur_loc; m_confBtn.y = box_height+titleHeight()+border*2; m_confBtn.w = box_width; 
	m_confBtn.h = box_height; cur_loc += (box_width+border);

	m_modeBtn.x = cur_loc; m_modeBtn.y = box_height+titleHeight()+border*2; m_modeBtn.w = box_width; 
	m_modeBtn.h = box_height; cur_loc += (box_width+border);

	m_searchBtn.x = cur_loc; m_searchBtn.y = box_height+titleHeight()+border*2; m_searchBtn.w = box_width; 
	m_searchBtn.h = box_height; cur_loc += (box_width+border);

	//draw objects
	ps_color c1 = {0, 0, 0, 1};
	ps_color c2 = {1, 0, 0, 1};
	ps_point s = {0, 0};
	ps_point e = {box_width-rds*2, box_height};
	m_impl->g_event = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s, &e);
	ps_gradient_add_color_stop(m_impl->g_event, 0, &c1);
	ps_gradient_add_color_stop(m_impl->g_event, 0.75, &c2);

	ps_color tc = {1, 1, 1, 1};
	ps_color tc2 = {1, 1, 1, 0};
	ps_point s1 = {rds, rds};
	ps_point e1 = {rds*5, box_height/2};
	m_impl->g_light = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &s1, &e1);
	ps_gradient_add_color_stop(m_impl->g_light, 0, &tc);
	ps_gradient_add_color_stop(m_impl->g_light, 0.6, &tc2);
}

void CtrlPanel::OnDestroy(void)
{
	Panel::OnDestroy();
}

void CtrlPanel::OnMouseEvent(const MouseEvent* e)
{
	Panel::OnMouseEvent(e);

	if (e->type() == MouseEvent::MouseDown) {
		if (m_hisBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_hisBtn;
			m_btn = 1;
		} else if (m_favBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_favBtn;
			m_btn = 2;
		} else if (m_fullBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_fullBtn;
			m_btn = 3;
		} else if (m_newBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_newBtn;
			m_btn = 4;
		} else if (m_exitBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_exitBtn;
			m_btn = 5;
		} else if (m_confBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_confBtn;
			m_btn = 6;
		} else if (m_modeBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_modeBtn;
			m_btn = 7;
		} else if (m_searchBtn.contains(e->x(), e->y()))	{
			m_eventRect = m_searchBtn;
			m_btn = 8;
		} else {
			m_eventRect = Rect(0, 0, 0, 0);
			m_btn = 0;
		}

		if (!m_eventRect.isEmpty()) {
			Update(&m_eventRect);
		}
	} else if (e->type() == MouseEvent::MouseUp) {
		if (!m_eventRect.isEmpty()) {
			Update(&m_eventRect);
			m_eventRect = Rect(0, 0, 0, 0);

			if (m_btn) {
				Widget::postEvent(this, EVENT_FUNC(CtrlPanel, button_event), EVENT_PARAM(m_btn), true);
				m_btn = 0;
			}
		}
	}
}

void CtrlPanel::button_event(void * p)
{
	Hide();

	int btn = (int)p;
	switch (btn)
	{
	case 1:
		{
			m_main->getHistory()->Active();
		}
		break;
	case 2:
		{
			m_main->getFavorites()->Active();
		}
		break;
	case 3:
		{
			m_main->getTabs()->setFullMode(true);
		}
		break;
	case 4:
		{
			m_main->getTabs()->newView(std::string());
			m_main->getTabs()->Update(0);
		}
		break;
	case 5:
		{
			m_main->Destroy();
		}
		break;
	case 6:
		{
			m_main->getSetting()->Active();
		}
		break;
	case 7:
		{
			int mode = m_main->getTabs()->getActiveView()->renderMode();
			if (mode) {
				mode = 0;
			} else {
				mode = 1;
			}
			m_main->getTabs()->getActiveView()->setRenderMode(mode);
		}
		break;
	case 8:
		{
			m_main->getSearch()->Active();
		}
		break;
	}
}

void CtrlPanel::setMainWindow(MainWindow* main)
{
	m_main = main;
}

