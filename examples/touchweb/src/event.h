/* event.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _EVENT_H_
#define _EVENT_H_

#include <vgcl.h>

class MouseEvent : public VGCL::VEvent
{
public:
	enum {
		NoneBtn 	= 0,
		LeftBtn		= 1,
		RightBtn	= 2,
	};

	enum {
		MouseUp		= 0,
		MouseDown	= 1,
		MouseMove	= 2,
		MouseDClick	= 3,
	};
public:
	MouseEvent(int type, unsigned btn, int x, int y) 
		: m_type(type), m_btns(btn), m_x(x), m_y(y)
	{
	}

	virtual ~MouseEvent()
	{
	}

	int	 type() const { return m_type; }
	unsigned btns() const { return m_btns; }
	int x() const { return m_x; } 
	int y() const { return m_y; } 
	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }
private:
	int			m_type;
	unsigned	m_btns;
	int 		m_x;
	int 		m_y;
};

class KeyEvent : public VGCL::VEvent
{
public:
	enum {
		KeyUp	= 0,
		KeyDown	= 1,
	};
public:
	KeyEvent(int type, unsigned vk) 
		: m_type(type), m_vk(vk)
	{
	}

	virtual ~KeyEvent() 
	{
	}

	int type() const { return m_type; }
	unsigned value() const { return m_vk; }
private:
	int m_type;
	unsigned m_vk;
};


class Widget;

typedef void (Widget::*EventSlot)(void);
#define EVENT_OBJECT(o) ((Widget*)o)
#define EVENT_OBJECT_FUNC(o, f) ((EventSlot)(&o::f))

typedef void (Widget::*EventHandler)(void * param);
#define EVENT_FUNC(t, f) ((EventHandler)(&t::f))
#define EVENT_PARAM(p) ((void*)p)

class PostEvent : public VGCL::VEvent
{
public:
	PostEvent() 
		: m_target(0), m_handler(0), m_param(0)
	{
		// empty event
	}

	PostEvent(Widget* target, EventHandler handler, void* param)
		: m_target(target), m_handler(handler), m_param(param)
	{
		// post event
	}

	virtual ~PostEvent()
	{
	}
	
	PostEvent(const PostEvent& o)
	{
		m_target = o.m_target;
		m_handler = o.m_handler;
		m_param = o.m_param;
	}

	PostEvent& operator=(const PostEvent& o)
	{
		if (this == &o)
			return *this;

		m_target = o.m_target;
		m_handler = o.m_handler;
		m_param = o.m_param;

		return *this;
	}

	bool isEmpty(void) const { return (0 == m_target) || (0 == m_handler); }
	EventHandler getHandler(void) const { return m_handler; }
	Widget* getTarget(void) const { return m_target; }
	void* getParam(void) const { return m_param; }
private:
	Widget* m_target;
	EventHandler m_handler;
	void* m_param;
};

#endif/*_EVENT_H_*/
