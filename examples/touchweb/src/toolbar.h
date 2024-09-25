/* toolbar.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "widget.h"

class MainWindow;

class ToolBarImpl;
class ToolBar : public Widget
{
public:
    typedef enum {
        BackBtn = 1,
        FordBtn = 2,
        ZoomBtn = 3,
        PageBtn = 4,
        ConfBtn = 5,
    } BtnId;

    ToolBar(Widget* parent);
    virtual ~ToolBar();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);

    void setMainWindow(MainWindow* main);

    void notifyButtonUpdate(int btnid);
private:
    void draw_buttons(ps_context* gc, const Rect& rc);
    void draw_event(ps_context* gc);
    void button_event(void* p);
    ToolBarImpl* m_impl;
    Rect m_backBtn;
    Rect m_fordBtn;
    Rect m_zoomBtn;
    Rect m_tabsBtn;
    Rect m_configBtn;
    Rect m_eventRect;
    MainWindow* m_main;
    int m_btn;
};

class ImToolBarImpl;
class ImToolBar : public Widget
{
public:
    ImToolBar(Widget* parent);
    virtual ~ImToolBar();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);

    void setMainWindow(MainWindow* main);
private:
    void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
    void draw_lights(ps_context* gc, const Rect& r);
    void button_event(void* p);
    ImToolBarImpl* m_impl;
    Rect m_imbtn;
    MainWindow* m_main;
    int m_btn;
};

#endif/*_TOOLBAR_H_*/
