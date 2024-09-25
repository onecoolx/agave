/* timer_qt4.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _TIMER_QT4_H_
#define _TIMER_QT4_H_

#include <QTimer>

class Timer;

class TimerImpl : public QTimer
{
public:
    TimerImpl(Timer* t);
    virtual ~TimerImpl();

    void start(unsigned int m, void* d);
    void stop(void);

    virtual void OnTimer(void);
protected:
    void timerEvent(QTimerEvent* timer);
private:
    Timer* timer;
    void* data;
};

#endif/*_TIMER_QT4_H_*/
