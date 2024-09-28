/* vbutton.h - Agave application
 *
 * Vector Graphic Component button.
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _VBUTTON_H_
#define _VBUTTON_H_

#include "widget.h"

class VButton : public Widget
{
public:
    enum {
        BS_NONE = 0,
        BS_PRESS = 1,
    };
public:
    VButton(Widget* parent);
    virtual ~VButton();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);

    ustring getText(void) const;
    void setText(const ustring& text);

    void setClickHandle(Widget* t, EventSlot f);
    void clearHandle(void);
protected:
    void click_event_task(void* p);
    Widget* m_target;
    EventSlot m_handle;
    unsigned int m_state;

    ustring m_text;
};

#endif /*_VBUTTON_H_*/
