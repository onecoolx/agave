/* pagemgr.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _PAGEMANAGER_H_
#define _PAGEMANAGER_H_

#include <string>
#include "panel.h"

class MainWindow;
class PageMgrImpl;

class PageManager : public Panel
{
public:
    PageManager(Widget* parent);
    virtual ~PageManager();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnShow(void);
    virtual void OnHide(void);

    void setMainWindow(MainWindow* main);
private:
    void draw_pageBtns(ps_context* gc, const Rect& r);
    void draw_pageBtn(ps_context* gc, const Rect& r);
    void draw_itemBtns(ps_context* gc, const Rect& r);
    void draw_lights(ps_context* gc, const Rect& r);
    void draw_closeBtn(ps_context* gc, const Rect& r);
    void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
    void loadBtns(void);
    void clearBtns(void);
    void page_active(void* p);
    void page_close(void* p);
    PageMgrImpl* m_impl;
    MainWindow* m_main;
};

#endif/*_PAGEMANAGER_H_*/
