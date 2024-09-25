/* timer_gtk2.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TIMER_GTK2_H_
#define _TIMER_GTK2_H_

#include <gtk/gtk.h>

class Timer;

class TimerImpl
{
public:
    TimerImpl(Timer* t);
    ~TimerImpl();

    void start(unsigned int m, void* d);
    void stop(void);

    void OnTimer(void);
protected:
    static gboolean TimerCb(gpointer data);
private:
    Timer* timer;
    void* data;
    gint tid;
};

#endif/*_TIMER_GTK2_H_*/
