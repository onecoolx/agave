/* popmenu.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "picasso.h"
#include "popmenu.h"
#include "tabpage.h"
#include "webview.h"
#include "mainwindow.h"
#include "application.h"

class PopMenuImpl
{
public:
	PopMenuImpl()
		: menu(0), main(0), buffer(0)
	{
	}

	~PopMenuImpl()
	{
	}

	MC_POPUP_MENU* menu;
	MainWindow* main;
	unsigned char* buffer;
};

PopMenu::PopMenu(Widget* parent)
	: TopWidget(parent)
	, m_impl(new PopMenuImpl)
{
}

PopMenu::~PopMenu()
{
	delete m_impl;
}

MC_POPUP_MENU* PopMenu::getPopMenu(void)
{
	return m_impl->menu;
}

void PopMenu::setMainWindow(MainWindow* main)
{
	m_impl->main = main;
}

void PopMenu::OnPaint(ps_context* gc, const Rect* r)
{
	TopWidget::OnPaint(gc, r);

	MC_RECT rt = {r->x, r->y, r->w, r->h};
	macross_menu_update(m_impl->menu, &rt);

	ps_save(gc);

	ps_color_format fmt;	
	int byte = Application::getInstance()->bytes_pixel();
	if (byte == 4)
		fmt = COLOR_FORMAT_BGRA;
	else if (byte == 2)
		fmt = COLOR_FORMAT_RGB565;
	ps_image * img = ps_image_create_with_data(m_impl->menu->buffer, 
						fmt, width(), height(), width()*byte);

	ps_rect rc = {0 , 0 , width(), height()};
	ps_set_source_image(gc, img);
	ps_rectangle(gc, &rc);
	ps_fill(gc);

	ps_image_unref(img);

	ps_rect tc = {0 , 0 , width()-1, height()-1};
	ps_color c = {0.5, 0.5, 0.5, 1};
	ps_set_source_color(gc, &c);
	ps_rectangle(gc, &tc);
	ps_stroke(gc);

	ps_restore(gc);
}

void PopMenu::OnKillFocus(void)
{
	TopWidget::OnKillFocus();

	macross_menu_hide(m_impl->menu);
}

void PopMenu::create(void)
{
	MainWindow* m = Application::getInstance()->getMainWindow();
	setParent(m);
	m->addChild(this);
	this->setFocus(true);
}

void PopMenu::release(void)
{
	TabPage* p = Application::getInstance()->getMainWindow()->getTabs();
	p->setFocus(true);
}

void PopMenu::size(int w, int h)
{
	free(m_impl->buffer);
	int cbyte = Application::getInstance()->bytes_pixel();
	m_impl->buffer = (unsigned char*)malloc(w*h*cbyte);
	memset(m_impl->buffer, 0xFF, w*h*cbyte);
	m_impl->menu->buffer = m_impl->buffer;
	m_impl->menu->pitch = w*cbyte;
	macross_menu_resize(m_impl->menu, w, h);
}

static void show(MC_POPUP_MENU* m)
{
	PopMenu* p = (PopMenu*)m->data;
	p->create();
	p->Show();
}

static void hide(MC_POPUP_MENU* m)
{
	PopMenu* p = (PopMenu*)m->data;
	p->Hide();
	p->release();
}

static void move(MC_POPUP_MENU* m, const MC_RECT* r)
{
	MC_RECT rb = {r->x, r->y, r->w, r->h};
	m->rect = rb;
	PopMenu* p = (PopMenu*)m->data;
	WebView* web = Application::getInstance()->getMainWindow()->getTabs()->getActiveView();
	Rect rc(r->x-web->virtualOffsetX(), 
			r->y+Application::getInstance()->getMainWindow()->getTabs()->y()+1-web->virtualOffsetY(), r->w, r->h);	
	p->setBoundRect(rc);
	p->size(rc.w, rc.h);
	p->Update(NULL);
}

static void dirty(MC_POPUP_MENU* m, const MC_RECT* r)
{
	PopMenu* p = (PopMenu*)m->data;
	if (r) {
		Rect rc(r->x, r->y, r->w, r->h);	
		p->Update(&rc);
	} else {
		p->Update(NULL);
	}
}

static void update(MC_POPUP_MENU* m)
{
	PopMenu* p = (PopMenu*)m->data;
	p->Update(NULL);
}

static void capture(MC_POPUP_MENU* m)
{
	PopMenu* p = (PopMenu*)m->data;
	p->setFocus(true);
}

void PopMenu::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	TopWidget::OnCreate(flags, x, y, w, h);

	int cbyte = Application::getInstance()->bytes_pixel();

	MC_RECT rc = {x, y-m_impl->main->getTabs()->y()-1, w, h};
	
	m_impl->buffer = (unsigned char*)malloc(w*h*cbyte);
	memset(m_impl->buffer, 0xFF, w*h*cbyte);
	
	m_impl->menu = (MC_POPUP_MENU*)malloc(sizeof(MC_POPUP_MENU));
	memset(m_impl->menu, 0, sizeof(MC_POPUP_MENU));

	m_impl->menu->show = show;
	m_impl->menu->hide = hide;
	m_impl->menu->move = move;
	m_impl->menu->dirty = dirty;
	m_impl->menu->update = update;
	m_impl->menu->capture = capture;

	m_impl->menu->rect = rc;
	m_impl->menu->buffer = m_impl->buffer;
	m_impl->menu->pitch = w*cbyte;
	m_impl->menu->data = this;
}

void PopMenu::OnDestroy(void)
{
	TopWidget::OnDestroy();

	free(m_impl->menu);
	free(m_impl->buffer);
}

void PopMenu::OnMouseEvent(const MouseEvent* e)
{
	TopWidget::OnMouseEvent(e);

	MC_MOUSE_EVENT evt;
	if (e->type() == MouseEvent::MouseDown) {
		evt.type = EVT_MOUSE_DOWN;
		evt.button = MOUSE_BTN_LEFT;
		evt.point.x = e->x();
		evt.point.y = e->y();
		evt.modifier = 0;
		macross_menu_mouse_event(m_impl->menu, &evt);
	} else if (e->type() == MouseEvent::MouseUp) {
		evt.type = EVT_MOUSE_UP;
		evt.button = MOUSE_BTN_LEFT;
		evt.point.x = e->x();
		evt.point.y = e->y();
		evt.modifier = 0;
		macross_menu_mouse_event(m_impl->menu, &evt);
	} else if (e->type() == MouseEvent::MouseMove) {
		evt.type = EVT_MOUSE_MOVE;
		evt.button = MOUSE_BTN_NONE;
		evt.point.x = e->x();
		evt.point.y = e->y();
		evt.modifier = 0;
		macross_menu_mouse_event(m_impl->menu, &evt);
	}
}

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MC_POPUP_MENU* CreatePopMenu(const MC_RECT* r)
{
	PopMenu* menu = new PopMenu(0);
	WebView* web = Application::getInstance()->getMainWindow()->getTabs()->getActiveView();
	menu->setMainWindow(Application::getInstance()->getMainWindow());
	menu->OnCreate(WF_ENABLED|WF_FOCUSABLE, r->x-web->virtualOffsetX(), 
		r->y+Application::getInstance()->getMainWindow()->getTabs()->y()+1-web->virtualOffsetY(), r->w, r->h);

	return menu->getPopMenu();
}

void DestroyPopMenu(MC_POPUP_MENU* m)
{
	//NOTE: not need this.
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

