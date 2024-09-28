/* layer.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _LAYER_H_
#define _LAYER_H_

#include "widget.h"

class Layer : public Widget
{
public:
    Layer(Widget* parent);
    virtual ~Layer();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
};

#endif/*_LAYER_H_*/
