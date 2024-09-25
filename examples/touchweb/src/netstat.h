/* netstat.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _NETSTATUS_H_
#define _NETSTATUS_H_

#include "panel.h"
#include "timer.h"

class MainWindow;
class NetStatPanel : public Panel, public Timer
{
public:
    NetStatPanel(Widget* parent);
    virtual ~NetStatPanel();

    virtual void OnPaint(ps_context* gc, const Rect* r);

    void setMainWindow(MainWindow* main);

    void startNetWorkCheck(void);
protected:
    virtual void OnTimer(void* data);
private:
    MainWindow* m_main;
    int m_count;
    int m_flat;
};

#endif/*_NETSTATUS_H_*/
