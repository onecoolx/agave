/* shadebtn.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SHADEBUTTON_H_
#define _SHADEBUTTON_H_

#include "button.h"

class ShadeButtonImpl;

class ShadeButton : public VButton
{
public:
    enum {
        GreenBtn = 1,
        RedBtn = 2,
        BlueBtn = 3,
    };

    ShadeButton(Widget* parent, int style);
    virtual ~ShadeButton();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);

private:
    ShadeButtonImpl* m_impl;
    int m_style;
};

#endif /*_SHADEBUTTON_H_*/
