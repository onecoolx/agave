/* timer_lvgl.cpp - TimerImpl::OnTimer bridge */
#include "config.h"
#include "lvgl.h"
#include "timer.h"
#include "timer_qt5.h"

void TimerImpl::OnTimer(void)
{
    m_timer->OnTimer(m_data);
}
