/* panel.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _PANEL_H_
#define _PANEL_H_

#include "topmost.h"

class Panel : public TopWidget
{
public:
	Panel(Widget* parent);
	virtual ~Panel();

	void setTitle(const ustring& title);
	int titleHeight(void) const;

	virtual void OnPaint(ps_context* gc, const Rect* r);
private:
	ustring m_title;
};

#endif/*_PANEL_H_*/

