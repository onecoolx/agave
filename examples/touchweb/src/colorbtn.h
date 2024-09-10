/* colorbtn.h - MaCross application
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _COLORBUTTON_H_
#define _COLORBUTTON_H_

#include "button.h"
#include "color.h"

class ColorButton :public VButton
{
public:
	ColorButton(Widget* parent, const Color& c);
	virtual ~ColorButton();

	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
private:
	Color m_color;
	ps_matrix* m_mtx;
	ps_gradient* m_grd;
};

#endif /*_COLORBUTTON_H_*/

