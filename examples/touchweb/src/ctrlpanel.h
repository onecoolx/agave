/* ctrlpanel.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _CTRLPANEL_H_
#define _CTRLPANEL_H_

#include "panel.h"

class MainWindow;
class CtrlPanelImpl;
class CtrlPanel : public Panel
{
public:
    CtrlPanel(Widget* parent);
    virtual ~CtrlPanel();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);

    void setMainWindow(MainWindow* main);
private:
    void draw_buttons(ps_context* gc, const Rect& rc);
    void draw_button_face(ps_context* gc, const Rect& rc);
    void draw_event(ps_context* gc);
    void button_event(void* p);
    CtrlPanelImpl* m_impl;
    Rect m_hisBtn;
    Rect m_favBtn;
    Rect m_fullBtn;
    Rect m_newBtn;
    Rect m_exitBtn;
    Rect m_confBtn;
    Rect m_modeBtn;
    Rect m_searchBtn;
    Rect m_eventRect;
    MainWindow* m_main;
    int m_btn;
};

#endif/*_CTRLPANEL_H_*/
