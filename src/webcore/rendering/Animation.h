/*
 * Agave - A lightweight web browser engine
 *
 * Copyright (c) 2026, Zhang Ji Peng
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Animation_h
#define Animation_h

#include <wtf/Vector.h>

namespace WebCore {

// A CSS timing function (easing). Stored as a cubic-bezier control polygon;
// the named keywords (ease/linear/ease-in/...) resolve to fixed control points.
class TimingFunction {
public:
    TimingFunction()
        : m_x1(0.25), m_y1(0.1), m_x2(0.25), m_y2(1.0) // CSS "ease" default
    {
    }

    TimingFunction(double x1, double y1, double x2, double y2)
        : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
    {
    }

    bool operator==(const TimingFunction& o) const
    {
        return m_x1 == o.m_x1 && m_y1 == o.m_y1 && m_x2 == o.m_x2 && m_y2 == o.m_y2;
    }
    bool operator!=(const TimingFunction& o) const { return !(*this == o); }

    // Named keyword factories.
    static TimingFunction linear() { return TimingFunction(0.0, 0.0, 1.0, 1.0); }
    static TimingFunction ease() { return TimingFunction(0.25, 0.1, 0.25, 1.0); }
    static TimingFunction easeIn() { return TimingFunction(0.42, 0.0, 1.0, 1.0); }
    static TimingFunction easeOut() { return TimingFunction(0.0, 0.0, 0.58, 1.0); }
    static TimingFunction easeInOut() { return TimingFunction(0.42, 0.0, 0.58, 1.0); }

    // Maps a linear time fraction t in [0,1] to the eased output fraction by
    // evaluating the cubic-bezier y for the x that matches t.
    double evaluate(double t) const;

    double x1() const { return m_x1; }
    double y1() const { return m_y1; }
    double x2() const { return m_x2; }
    double y2() const { return m_y2; }

private:
    double m_x1;
    double m_y1;
    double m_x2;
    double m_y2;
};

// One CSS transition declaration entry: which property to transition, how long,
// after what delay, and with which easing. A property id of 0 with m_all set
// means "transition: all".
class Transition {
public:
    Transition()
        : m_property(0)
        , m_all(true)
        , m_duration(0)
        , m_delay(0)
        , m_timingFunction()
    {
    }

    bool operator==(const Transition& o) const
    {
        return m_property == o.m_property && m_all == o.m_all
            && m_duration == o.m_duration && m_delay == o.m_delay
            && m_timingFunction == o.m_timingFunction;
    }
    bool operator!=(const Transition& o) const { return !(*this == o); }

    int property() const { return m_property; }
    bool isAll() const { return m_all; }
    double duration() const { return m_duration; }   // seconds
    double delay() const { return m_delay; }          // seconds
    const TimingFunction& timingFunction() const { return m_timingFunction; }

    void setProperty(int p) { m_property = p; m_all = false; }
    void setAll() { m_property = 0; m_all = true; }
    void setDuration(double d) { m_duration = d; }
    void setDelay(double d) { m_delay = d; }
    void setTimingFunction(const TimingFunction& tf) { m_timingFunction = tf; }

private:
    int m_property;
    bool m_all;
    double m_duration;
    double m_delay;
    TimingFunction m_timingFunction;
};

typedef Vector<Transition> TransitionList;

} // namespace WebCore

#endif // Animation_h
