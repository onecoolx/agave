/* addressbar.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _ADDRESSBAR_H_
#define _ADDRESSBAR_H_

#include "widget.h"

class MainWindow;

class NavButton : public Widget
{
public:
    enum {
        LoadBtn = 0,
        StopBtn = 1,
        ReloadBtn = 2,
    };

    NavButton(Widget* parent);
    virtual ~NavButton();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);

    void setButtonType(int type);
    int buttonType(void);
    void setMainWindow(MainWindow* main);
private:
    void draw_navbtn(ps_context* gc, ps_rect& r);
    void draw_stopbtn(ps_context* gc, ps_rect& r);
    void draw_reloadbtn(ps_context* gc, ps_rect& r);
    void button_event(void* p);
    MainWindow* m_main;
    int m_type;
    int m_click;
};

class AddressBar : public Widget
{
public:
    AddressBar(Widget* parent);
    virtual ~AddressBar();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnDestroy(void);
    virtual void OnSetFocus(void);
    virtual void OnKillFocus(void);
    virtual void OnChar(uchar_t code);
    virtual void OnIdle(void);

    void setLoading(bool b);
    void setText(const ustring& text);
    void setUpdate(void);
    void setMainWindow(MainWindow* main);
private:
    friend class NavButton;
    void loadUrl(void);
    void setNewUrl(bool b);
    bool isNewUrl(void);
    void load_url(void* param);
    MainWindow* m_main;
    NavButton* m_btn;
    ps_gradient* m_loadpr;
    ustring m_text;
    ustring m_tip;
    Rect m_clear;
    int m_clsbtn;
    //select
    bool m_select;
    bool m_capture;
    bool m_update;
    int m_sx;
    int m_sy;
};

#endif/*_ADDRESSBAR_H_*/
