/* hismgr.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _HISTORY_MANAGER_H_
#define _HISTORY_MANAGER_H_

#include <vector>
#include <string>
#include "dashlayer.h"
#include "scrollview.h"

class MainWindow;
class HistoryViewImpl;
class HistoryView : public ScrollView
{
public:
    typedef struct {
        int id;
        Rect rect;
        ustring title;
        ustring url;
    } HistoryItem;

    HistoryView(Widget* parent);
    virtual ~HistoryView();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnIdle(void);
    virtual void OnShow(void);

    void refresh(void* p);
    void reset(void* p);

    void setMainWindow(MainWindow* main);
protected:
    virtual void OnPaintContents(ps_context* gc, const Rect* r);
private:
    void draw_button(ps_context* gc, const ustring& text, const Rect& r, bool enable, bool highlight);
    void draw_icon(ps_context* gc, const ps_rect* r);
    void build(int offset);
    void item_click(void* p);
    HistoryViewImpl* m_impl;
    MainWindow* m_main;
    std::vector<HistoryItem> m_items;
    int m_page;
    Rect m_pageup;
    Rect m_pagedown;
    bool m_enableup;
    bool m_enabledown;
    Rect m_event;
    int m_btn;
    int m_pagebtn;
    int m_startX;
    int m_startY;
    bool m_capture;
    bool m_drag;
    //smooth scroll
    volatile int m_ticket;
    volatile int m_time;
    int m_drx;
    int m_dry;
    int m_scx;
    int m_scy;
};

class HistoryManager : public DashLayer
{
public:
    HistoryManager(Widget* parent);
    virtual ~HistoryManager();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnShow(void);

    void setMainWindow(MainWindow* main);
private:
    void draw_button(ps_context* gc, const Rect& r, bool enable, bool highlight);
    MainWindow* m_main;
    HistoryView* m_view;
    Rect m_clear;
    Rect m_event;
    bool m_enable;
    int m_btn;
};

#endif/*_HISTORY_MANAGER_H_*/
