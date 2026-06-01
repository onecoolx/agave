/* timer_qt5.h - WatchWeb LVGL backend */
#ifndef _TIMER_LVGL_H_
#define _TIMER_LVGL_H_

#include "lvgl.h"

class Timer;

class TimerImpl {
public:
    TimerImpl(Timer* t) : m_timer(t), m_lv_timer(nullptr), m_data(nullptr) {}
    ~TimerImpl() { stop(); }

    void start(unsigned int ms, void* d) {
        stop();
        m_data = d;
        m_lv_timer = lv_timer_create(lv_timer_cb, ms, this);
    }

    void stop(void) {
        if (m_lv_timer) { lv_timer_delete(m_lv_timer); m_lv_timer = nullptr; }
    }

    void OnTimer(void);

private:
    static void lv_timer_cb(lv_timer_t* t) {
        TimerImpl* self = (TimerImpl*)lv_timer_get_user_data(t);
        self->OnTimer();
    }
    Timer* m_timer;
    lv_timer_t* m_lv_timer;
    void* m_data;
};

#endif
