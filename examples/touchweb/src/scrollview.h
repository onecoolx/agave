/* scrollview.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SCROLLVIEW_H_
#define _SCROLLVIEW_H_

#include "widget.h"

class ScrollView : public Widget
{
public:
    ScrollView(Widget* parent);
    virtual ~ScrollView();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);

    int contentHeight(void) const;
    int contentWidth(void) const;
    void setContentSize(int w, int h);

    int scrollY(void) const;
    int scrollX(void) const;
    void setScrollPoint(int x, int y);

    void scrollContents(int xoffset, int yoffset);
protected:
    virtual void OnPaintContents(ps_context* gc, const Rect* r);
private:
    ps_context* m_gc;
    ps_canvas* m_dc;
    int m_off_x;
    int m_off_y;
    int m_content_w;
    int m_content_h;
};

#endif/*_SCROLLVIEW_H_*/
