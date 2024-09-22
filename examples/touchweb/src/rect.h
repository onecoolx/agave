/* rect.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "config.h"
#include <picasso/picasso.h>

struct Rect 
{
	Rect()
		: x(0), y(0), w(0), h(0)
	{
	}

	Rect(int x1, int y1, int w1, int h1)
		: x(x1), y(y1), w(w1), h(h1)
	{
	}

	Rect(const ps_rect& r)
		: x((int)r.x), y((int)r.y), w((int)r.w), h((int)r.h)
	{
	}

	bool isEmpty(void) const
	{
		return (w<=0) || (h<=0);
	}

	bool intersect(const Rect& rt) 
	{
		if (!isEmpty() && !rt.isEmpty()) {
			int l = MAX(x, rt.x);
			int t = MAX(y, rt.y);
			int r = MIN(x+w, rt.x+rt.w);
			int b = MIN(y+h, rt.y+rt.h);
			if (l >= r || t >= b)
				return false;
			x = l; y = t; w = r - l; h = b - t;
			return true;
		}
		return false;
	}

    bool contains(int px, int py) const
	{
        return px >= x && px < (x+w) && py >= y && py < (y+h); 
	}

	bool contents(const Rect& o) const
	{
		return x <= o.x && y <= o.y && (x+w) >= (o.x+o.w) && (y+h) >= (o.y+o.h); 
	}

	operator ps_rect() const
	{
		ps_rect r;
		r.x = x; r.y = y; r.w = w; r.h = h;
		return r;
	}

	int x;
	int y;
	int w;
	int h;
};

inline bool operator==(const Rect& a, const Rect& b)
{
	return (a.x==b.x) && (a.y==b.y) && (a.w==b.w) && (a.h==b.h);
}

#endif/*_RECTANGLE_H_*/
