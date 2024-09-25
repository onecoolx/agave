/* color.h - Agave application
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _COLORDEF_H_
#define _COLORDEF_H_

#include "config.h"
#include <picasso/picasso.h>

struct Color {
    Color()
        : r(0.0), g(0.0), b(0.0), a(0.0)
    {
    }

    Color(double r1, double g1, double b1, double a1)
        : r(r1), g(g1), b(b1), a(a1)
    {
    }

    Color(double r1, double g1, double b1)
        : r(r1), g(g1), b(b1), a(1.0)
    {
    }

    Color(const ps_color& c)
        : r(c.r), g(c.g), b(c.b), a(c.a)
    {
    }

    operator ps_color() const
    {
        ps_color c;
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = a;
        return c;
    }

    double r;
    double g;
    double b;
    double a;
};

#endif /*_COLORDEF_H_*/
