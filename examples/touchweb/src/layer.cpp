/* layer.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "layer.h"

Layer::Layer(Widget* parent)
    : Widget(parent)
{
}

Layer::~Layer()
{
}

void Layer::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Widget::OnCreate(f | WF_TOPMOST, 0, 0, parent()->width(), parent()->height());
}
