/* topmost.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"
#include "picasso.h"
#include "topmost.h"

TopWidget::TopWidget(Widget* parent)
	: Widget(parent)
{
}

TopWidget::~TopWidget()
{
}

void TopWidget::OnCreate(uint32_t f, int x, int y, int w, int h)
{
	Widget::OnCreate(f|WF_TOPMOST, x, y, w, h);
}
