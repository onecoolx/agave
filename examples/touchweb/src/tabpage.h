/* tabpage.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TABPAGE_H_
#define _TABPAGE_H_

#include <list>
#include "widget.h"

class WebView;
class MainWindow;

class TabPage : public Widget
{
public:
    TabPage(Widget* parent);
    virtual ~TabPage();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
    virtual void OnDestroy(void);
    virtual void OnPaint(ps_context* gc, const Rect* r);
    virtual void OnSetFocus(void);
    virtual void OnKillFocus(void);
    virtual void OnMouseEvent(const MouseEvent* e);
    virtual void OnKeyEvent(const KeyEvent* e);
    virtual void OnUpdate(const Rect* r);
    virtual void OnIdle(void);
    virtual void OnChar(uchar_t code);

    bool newView(const std::string& url);
    bool newViewBack(const std::string& url);
    void closeView(WebView* view);

    WebView* getActiveView(void) const;
    void setActiveView(WebView* view);
    void locationChanged(WebView* view);
    bool isActiveView(const WebView* view) const;

    void setFullMode(bool full);
    bool isFullMode(void) const;

    size_t getCount(void) const;
    std::list<WebView*> getTabs(void) const;

    void setMainWindow(MainWindow* main);
    void repaintAllPage(void* p);
private:
    void sizeChanged(int x, int y, int w, int h);
    std::list<WebView*> m_tabs;
    WebView* m_current;
    WebView* m_focused;
    MainWindow* m_main;
    bool m_fullMode;
};

#endif/*_TABPAGE_H_*/
