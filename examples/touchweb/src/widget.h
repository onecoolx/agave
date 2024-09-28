/* widget.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <list>
#include "rect.h"
#include "event.h"

enum {
    WF_VISIBLE = 1,
    WF_ENABLED = 2,
    WF_FOCUSABLE = 4,
    WF_TOPMOST = 8,
    WF_EDITABLE = 16,
    WF_MODAL = 32,
};

class Widget
{
public:
    Widget(Widget*);
    virtual ~Widget();

    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnUpdate(const Rect* r);
    virtual void OnSetFocus(void);
    virtual void OnKillFocus(void);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnKeyEvent(const KeyEvent* e);
    virtual void OnIdle(void);
    virtual void OnChar(uchar_t code);
    virtual void OnShow(void);
    virtual void OnHide(void);
    virtual void OnSize(int w, int h);

    void Paint(ps_context* gc, const Rect* rc);
    void Update(const Rect* r);
    bool SendMouseEvent(MouseEvent* e);
    bool SendKeyEvent(KeyEvent* e);
    void FireIdle(void);
    void CharInput(uchar_t code);
    void Show(void);
    void Hide(void);

    void addChild(Widget* widget);
    void removeChild(Widget* widget);

    Widget* parent(void) const { return m_parent; }
    void setParent(Widget* parent) { m_parent = parent; }
    int width(void) const { return m_bound.w; }
    int height(void) const { return m_bound.h; }
    void setWidth(int w) { m_bound.w = w; }
    void setHeight(int h) { m_bound.h = h; }
    int x(void) const { return m_bound.x; }
    int y(void) const { return m_bound.y; }
    void setX(int x) { m_bound.x = x; }
    void setY(int y) { m_bound.y = y; }
    Rect boundRect(void) { return m_bound; }
    void setBoundRect(const Rect& r) { m_bound = r; }
    void Move(int x, int y, int w, int h);
    void setCapture(void);
    void freeCapture(void);
    bool isCapture(void);

    bool isVisible(void) const { return (m_flags & WF_VISIBLE) ? true : false; }
    bool isFocusable(void) const { return (m_flags & WF_FOCUSABLE) ? true : false; }
    bool isEnable(void) const { return (m_flags & WF_ENABLED) ? true : false; }
    bool isTopmost(void) const { return (m_flags & WF_TOPMOST) ? true : false; }
    bool isEditable(void) const { return (m_flags & WF_EDITABLE) ? true : false; }

    bool hasFocus(void) const { return m_focused; }
    void setFocus(bool focus);

    std::list<Widget*> getChilds(bool topmost) const;
    static Widget* getCapturedWidget(void) { return Widget::g_capturedWidget; }
    static Widget* getFocusedWidget(void) { return Widget::g_focusedWidget; }

    static bool dispatchEvents(void);
    static void postEvent(Widget* target, EventHandler handler, void* param, bool high_level = false);
    static void cancelEvent(Widget* target);
protected:
    void clipRect(ps_context* gc, const Rect& rc);
private:
    Widget* m_parent;
    Rect m_bound;
    uint32_t m_flags;
    bool m_focused;
    std::list<Widget*> m_childs;
    std::list<Widget*> m_topchilds;
    static Widget* g_focusedWidget;
    static Widget* g_capturedWidget;
};

#endif/*_WIDGET_H_*/
