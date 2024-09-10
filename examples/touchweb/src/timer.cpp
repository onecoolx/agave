/* timer.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "timer.h"

#if defined(WIN32) || defined(WINCE)
#include "timer_win32.h"
#endif

#ifdef QT4
#include "timer_qt4.h"
#endif

Timer::Timer()
	: m_impl(0)
{
	m_impl = new TimerImpl(this);
}

Timer::~Timer()
{
	delete m_impl;
}

void Timer::start(unsigned int ms, void* d)
{
	m_impl->start(ms, d);
}

void Timer::stop(void)
{
	m_impl->stop();
}

void Timer::OnTimer(void* d)
{
}
