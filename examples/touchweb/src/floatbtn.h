/* floatbtn.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _FLOATBUTTON_H_
#define _FLOATBUTTON_H_

#include "topmost.h"

class MainWindow;

class FloatButton : public TopWidget
{
public:
    FloatButton(Widget* parent);
    virtual ~FloatButton();
};

class ModeButton : public FloatButton
{
public:
    ModeButton(Widget* parent);
    virtual ~ModeButton();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    void setMainWindow(MainWindow* main);
private:
    void button_event(void* p);
    MainWindow* m_main;
    ps_canvas* m_face;
    bool m_down;
};

class BackButton : public FloatButton
{
public:
    BackButton(Widget* parent);
    virtual ~BackButton();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    void setMainWindow(MainWindow* main);
private:
    void button_event(void* p);
    MainWindow* m_main;
    ps_canvas* m_face;
    bool m_down;
};

#endif/*_FLOATBUTTON_H_*/
