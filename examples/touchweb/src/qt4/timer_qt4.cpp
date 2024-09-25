/* timer_qt4.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "timer.h"
#include "timer_qt4.h"

TimerImpl::TimerImpl(Timer* t)
    : timer(t)
    , data(0)
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

void TimerImpl::start(unsigned int ms, void* d)
{
    stop();
    data = d;
    QTimer::start(ms);
}

void TimerImpl::stop(void)
{
    QTimer::stop();
}

void TimerImpl::timerEvent(QTimerEvent* timer)
{
    OnTimer();
}
