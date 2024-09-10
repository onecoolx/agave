/* timer_win32.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "timer.h"
#include "timer_win32.h"

#include <windows.h>
#pragma warning(disable:4786)
#include <map>

static std::map<UINT, TimerImpl*> g_timer_map;

VOID CALLBACK TimerProc(HWND h, UINT msg, UINT id, DWORD t)
{
	TimerImpl* timer = g_timer_map.find(id)->second;
	timer->OnTimer();
}

TimerImpl::TimerImpl(Timer* t)
	: timer(t)
	, id(0)
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
	id = SetTimer(0, 0, ms, TimerProc);
	g_timer_map[id] = this;
}

void TimerImpl::stop(void)
{
	if (id) {
		KillTimer(0, id);
		g_timer_map.erase(id);
		data = 0;
		id = 0;
	}
}
