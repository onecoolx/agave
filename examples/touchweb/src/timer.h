/* timer.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TIMER_H_
#define _TIMER_H_

class TimerImpl;

class Timer
{
public:
    Timer();
    virtual ~Timer();

    void start(unsigned int ms, void* data);
    void stop(void);

protected:
    virtual void OnTimer(void* data);
    friend class TimerImpl;
private:
    TimerImpl* m_impl;
};

#endif/*_TIMER_H_*/
