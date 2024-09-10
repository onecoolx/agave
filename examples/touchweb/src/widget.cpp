/* widget.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <deque>
#include <vgcl.h>
#include "config.h"
#include "picasso.h"
#include "widget.h"

Widget* Widget::g_focusedWidget = NULL;
Widget* Widget::g_capturedWidget = NULL;

//post event queue
static std::deque<PostEvent> g_events;
static VGCL::VMutex g_mutex;
#define EVENT_LOCK g_mutex.Lock()
#define EVENT_UNLOCK g_mutex.UnLock()

Widget::Widget(Widget* parent)
	: m_parent(parent)
	, m_bound(0, 0, 0, 0)
	, m_flags(0)
	, m_focused(false)
{
}

Widget::~Widget()
{
	if (hasFocus()) {
		setFocus(false);
	}

	cancelEvent(this);
}

bool Widget::SendKeyEvent(KeyEvent* e)
{
	if (!isEnable() || !isVisible()) 
		return false;

	Widget * w = Widget::g_focusedWidget;

	if (w && w->isEditable())
		w->OnKeyEvent(e);

	return true;
}

bool Widget::SendMouseEvent(MouseEvent* e)
{
	if (Widget* widget = Widget::getCapturedWidget()) {

		if ((e->type() == MouseEvent::MouseDown) && isFocusable() && isEnable()) {
			setFocus(true);
		}
		int x = e->x();
		int y = e->y();

		for (Widget* p = widget; p != NULL; p = p->parent()) {
			x -= p->x();
			y -= p->y();
		}

		MouseEvent ev(e->type(), e->btns(), x, y);
		widget->OnMouseEvent(&ev);
		return true;
	}
	
	if (!isEnable() || !isVisible()) 
		return false;

	bool child = false;

	for (std::list<Widget*>::reverse_iterator tit = m_topchilds.rbegin(); tit != m_topchilds.rend(); tit++) {
		if ((*tit)->isEnable() && (*tit)->boundRect().contains(e->x(), e->y())) {
			int x = e->x() - (*tit)->x();
			int y = e->y() - (*tit)->y();
			MouseEvent ev(e->type(), e->btns(), x, y);
			if (child = (*tit)->SendMouseEvent(&ev))
				break;
			else
				continue;
		}
	}


	if (!child) {
		for (std::list<Widget*>::reverse_iterator it = m_childs.rbegin(); it != m_childs.rend(); it++) {
			if ((*it)->isEnable() && (*it)->boundRect().contains(e->x(), e->y())) {
				int x = e->x() - (*it)->x();
				int y = e->y() - (*it)->y();
				MouseEvent ev(e->type(), e->btns(), x, y);
				if (child = (*it)->SendMouseEvent(&ev))
					break;
				else
					continue;
			}
		}
	}

	if (!child) {
		if ((e->type() == MouseEvent::MouseDown) && isFocusable() && isEnable()) {
			setFocus(true);
		}
		OnMouseEvent(e);
		child = true;
	}
	return child;
}

void Widget::Update(const Rect* r)
{
	if (!isVisible())
		return;

	Rect rect(0, 0, width(), height());

	if (r && !rect.intersect(*r))
		return;	

	OnUpdate(&rect);

	Widget* p = parent();

	if (p && p->isVisible()) {
		rect.x += m_bound.x;
		rect.y += m_bound.y;	
		p->Update(&rect);
	}
}

void Widget::CharInput(uchar_t code)
{
	if (!isEnable() || !isVisible()) 
		return;

	Widget * w = Widget::g_focusedWidget;

	if (w && w->isEditable())
		w->OnChar(code);
}

void Widget::postEvent(Widget* target, EventHandler handler, void* param, bool h)
{
	PostEvent e(target, handler, param);
	EVENT_LOCK;
	if (h)
		g_events.push_front(e);
	else
		g_events.push_back(e);
	EVENT_UNLOCK;
}

bool Widget::dispatchEvents(void)
{
	if (!g_events.empty()) {
		PostEvent e = g_events.front();

		EVENT_LOCK;
		g_events.pop_front();
		EVENT_UNLOCK;

		if (!e.isEmpty()) {
			Widget* t = e.getTarget();
			EventHandler fn = e.getHandler();
			void * p = e.getParam();
			(t->*fn)(p);
		}
		return true;
	} else {
		return false;
	}
}

void Widget::cancelEvent(Widget* target)
{
	if (!g_events.empty()) {
		EVENT_LOCK;
		std::deque<PostEvent>::iterator itr = g_events.begin();
		while (itr != g_events.end()) {
			Widget * p = itr->getTarget();
			if (itr->getTarget() == target) {
				itr = g_events.erase(itr);
			} else {
				itr++;
			}
		}
		EVENT_UNLOCK;
	}
}

void Widget::FireIdle(void)
{
	if (!isEnable())
		return;

	OnIdle();

	for (std::list<Widget*>::reverse_iterator tit = m_topchilds.rbegin(); tit != m_topchilds.rend(); tit++) {
		(*tit)->FireIdle();
	}

	for (std::list<Widget*>::reverse_iterator it = m_childs.rbegin(); it != m_childs.rend(); it++) {
		(*it)->FireIdle();
	}
}

void Widget::Paint(ps_context* gc, const Rect* rc)
{
	if (!m_bound.isEmpty() && isVisible()) 
	{
		Rect trc = m_bound;
		if (trc.intersect(*rc)) {
			ps_rect r = trc;
			ps_save(gc);
			ps_clip_fast_rect(gc, &r);
			ps_translate(gc, m_bound.x, m_bound.y);

			trc.x -= m_bound.x;
			trc.y -= m_bound.y;

			OnPaint(gc, &trc);

			for (std::list<Widget*>::iterator it = m_childs.begin(); it != m_childs.end(); it++) {
				(*it)->Paint(gc, &trc);
			}

			for (std::list<Widget*>::iterator tit = m_topchilds.begin(); tit != m_topchilds.end(); tit++) {
				(*tit)->Paint(gc, &trc);
			}

			ps_restore(gc);
		}
	}
}

void Widget::Show(void)
{
	if (!isVisible()) {
		m_flags |= WF_VISIBLE;
		if (isTopmost()){
			parent()->removeChild(this);
			parent()->addChild(this);
		}

		OnShow();
		Update(NULL);
	}
}

void Widget::Hide(void)
{
	if (isVisible()) {
		OnHide();
		Update(NULL);
		if (hasFocus()) {
			setFocus(false);
		}
		m_flags &= ~WF_VISIBLE;
	}
}

void Widget::setFocus(bool f)
{
	if (isFocusable()) {
		if (m_focused != f) {
			m_focused = f;
			if (f) {
				if (Widget::g_focusedWidget)
					Widget::g_focusedWidget->setFocus(false);
				Widget::g_focusedWidget = this;
				OnSetFocus();
			} else {
				OnKillFocus();
				Widget::g_focusedWidget = NULL;
			}
		}
	}
}

void Widget::Move(int x, int y, int w, int h)
{
	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
	OnSize(w, h);
	Update(NULL);
}

void Widget::OnKeyEvent(const KeyEvent* e)
{
}

void Widget::OnMouseEvent(const MouseEvent* e)
{
	if (e->type() == MouseEvent::MouseDown) {
		setCapture();
	} else if (e->type() == MouseEvent::MouseUp) {
		freeCapture();
	}
}

void Widget::OnPaint(ps_context* gc, const Rect* r)
{
}

void Widget::OnUpdate(const Rect* r)
{
}

void Widget::OnSetFocus(void)
{
}

void Widget::OnKillFocus(void)
{
}

void Widget::OnIdle(void)
{
}

void Widget::OnChar(uchar_t code)
{
}

void Widget::OnShow(void)
{
}

void Widget::OnHide(void)
{
}

void Widget::OnSize(int w, int h)
{
}

void Widget::OnCreate(uint32_t flags, int x, int y, int w, int h)
{
	m_flags = flags;
	m_bound.x = x; m_bound.y = y; m_bound.w = w; m_bound.h = h;
}

void Widget::addChild(Widget* widget)
{
	if (widget) {
		if (widget->isTopmost())
			m_topchilds.push_back(widget);
		else
			m_childs.push_back(widget);
	}
}

void Widget::removeChild(Widget* widget)
{
	if (widget) {
		if (widget->isTopmost())
			m_topchilds.remove(widget);
		else
			m_childs.remove(widget);
	}
}

void Widget::OnDestroy(void)
{
	for (std::list<Widget*>::iterator tit = m_topchilds.begin(); tit != m_topchilds.end(); tit++) {
		(*tit)->OnDestroy();
	}
	m_topchilds.clear();

	for (std::list<Widget*>::iterator it = m_childs.begin(); it != m_childs.end(); it++) {
		(*it)->OnDestroy();
	}
	m_childs.clear();

	cancelEvent(this);
}


std::list<Widget*> Widget::getChilds(bool t) const
{
	if (t) {
		return m_topchilds;
	} else {
		return m_childs;
	}
}

void Widget::setCapture(void)
{
	Widget::g_capturedWidget = this;
}

void Widget::freeCapture(void)
{
	Widget::g_capturedWidget = NULL;
}

bool Widget::isCapture(void)
{
	return (Widget::g_capturedWidget == this);
}

