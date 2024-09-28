/* about.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _ABOUT_H_
#define _ABOUT_H_

#include "dashlayer.h"

class MainWindow;

class About : public DashLayer
{
public:
    About(Widget* parent);
    virtual ~About();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);

    void setMainWindow(MainWindow* main);
private:
    void link_click(void* p);
    MainWindow* m_main;
    Rect m_email;
    int m_click;
};

#endif /*_ABOUT_H_*/
