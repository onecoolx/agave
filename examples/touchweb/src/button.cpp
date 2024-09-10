/* vbutton.cpp - MaCross application
 *
 * Vector Graphic Component button.
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <string>
#include "config.h"
#include "picasso.h"
#include "button.h"

VButton::VButton(Widget* parent)
	: Widget(parent)
	, m_target(0)
	, m_handle(0)
	, m_state(0)
{
	setText(U("Button"));
}

VButton::~VButton()
{
}

void VButton::clearHandle(void)
{
	m_target = 0;
	m_handle = 0;
}

void VButton::setClickHandle(Widget* t, EventSlot f)
{
	m_target = t;
	m_handle = f;
}

void VButton::click_event_task(void* p)
{
	if (m_target && m_handle) {
		(m_target->*m_handle)(); //emit button event.
	}
}

ustring VButton::getText(void) const
{
	return m_text;
}

void VButton::setText(const ustring& text)
{
	m_text = text;
}

void VButton::OnPaint(ps_context* gc, const Rect* r)
{
	Widget::OnPaint(gc, r);

	ps_save(gc);

	ps_rect rc = {0, 0, width(), height()};
	ps_rectangle(gc, &rc);

	if (m_state & BS_PRESS) {
		ps_color c = {0.85, 0.85, 0.85, 1};
		ps_set_source_color(gc, &c);
	} else {
		ps_color c = {0.5, 0.5, 0.5, 1};
		ps_set_source_color(gc, &c);
	}

	ps_fill(gc);

	// draw text
	if (!m_text.empty()) {
		ps_color c = {0, 0, 0, 1};
		ps_set_text_color(gc, &c);

		ps_size sz = ps_get_text_extent(gc, (ps_uchar16*)m_text.c_str(), m_text.length());

		ps_wide_text_out_length(gc, (rc.w-sz.w)/2, (rc.h-sz.h)/2, 
												(ps_uchar16*)m_text.c_str(), m_text.length());
	}

	ps_restore(gc);
}

void VButton::OnMouseEvent(const MouseEvent* e)
{
	Widget::OnMouseEvent(e);
	if (e->type() == MouseEvent::MouseDown) {
		m_state |= BS_PRESS;
		Update(NULL);
	} else if (e->type() == MouseEvent::MouseUp) {
		if (m_state & BS_PRESS) {
			m_state &= ~BS_PRESS;
			Update(NULL);
			
			Widget::postEvent(this, EVENT_FUNC(VButton, click_event_task), EVENT_PARAM(0));
		}
	}
}

