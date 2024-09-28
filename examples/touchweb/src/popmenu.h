/* popmenu.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _POPMENU_H_
#define _POPMENU_H_

#include "macross.h"
#include "topmost.h"

class PopMenuImpl;
class MainWindow;

class PopMenu : public TopWidget
{
public:
    PopMenu(Widget* parent);
    virtual ~PopMenu();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnKillFocus(void);

    MC_POPUP_MENU* getPopMenu(void);

    void setMainWindow(MainWindow* main);

    void size(int w, int h);
    void create(void);
    void release(void);
private:
    PopMenuImpl* m_impl;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MC_POPUP_MENU* CreatePopMenu(const MC_RECT*);

void DestroyPopMenu(MC_POPUP_MENU*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/*_POPMENU_H_*/
