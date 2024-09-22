/* timer_gtk2.cpp - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "timer.h"
#include "timer_gtk2.h"

TimerImpl::TimerImpl(Timer* t)
	: timer(t)
	, data(0)
    , tid(0)
{
}

TimerImpl::~TimerImpl()
{
	stop();
}

void TimerImpl::OnTimer(void)
{
	timer->OnTimer(data);
}

gboolean TimerImpl::TimerCb(gpointer data)
{
    TimerImpl * t = (TimerImpl*)data;
    t->OnTimer();
    return TRUE;
}

void TimerImpl::start(unsigned int ms, void* d)
{
	stop();
	data = d;
    tid = g_timeout_add(ms, TimerImpl::TimerCb, this);
}

void TimerImpl::stop(void)
{
    g_source_remove(tid);
    tid = 0;
}

