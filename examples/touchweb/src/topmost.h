/* topmost.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TOPMOSTWIDGET_H_
#define _TOPMOSTWIDGET_H_

#include "widget.h"

class TopWidget : public Widget
{
public:
    TopWidget(Widget* parent);
    virtual ~TopWidget();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
};

#endif/*_TOPMOSTWIDGET_H_*/
