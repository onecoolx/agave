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

#ifndef AnimationController_h
#define AnimationController_h

#include "config.h"

#if ENABLE(CSS_TRANSITIONS)

#include "Animation.h"
#include "Timer.h"
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace WebCore {

class Document;
class RenderObject;
class RenderStyle;

// One in-flight property transition on a renderer: it interpolates a single CSS
// property from a start style to an end style over [startTime+delay, +duration].
class RunningTransition {
public:
    RunningTransition()
        : m_property(0)
        , m_startTime(0)
        , m_delay(0)
        , m_duration(0)
        , m_fromStyle(0)
        , m_toStyle(0)
    {
    }

    int m_property;          // CSS_PROP_* being animated
    double m_startTime;      // wall-clock seconds when the transition was created
    double m_delay;          // seconds
    double m_duration;       // seconds
    TimingFunction m_timingFunction;
    RenderStyle* m_fromStyle; // ref'd start style
    RenderStyle* m_toStyle;   // ref'd target style
};

// Document-level controller that drives CSS transitions with a single shared
// timer, mirroring the GIF animation timer model (BitmapImage). It owns the set
// of running transitions per renderer and advances them frame by frame.
class AnimationController {
public:
    AnimationController(Document*);
    ~AnimationController();

    // Called from RenderObject::setStyle. Compares the renderer's current style
    // (oldStyle) with the incoming newStyle for transition-declared, animatable
    // properties. For each property that changes, starts (or retargets) a
    // running transition. Returns a freshly allocated blended style for the
    // current instant that the caller should apply instead of newStyle, or 0 if
    // no transition applies (caller keeps newStyle).
    RenderStyle* updateTransitions(RenderObject*, RenderStyle* oldStyle, RenderStyle* newStyle);

    // Drops all running transitions for a renderer being destroyed.
    void clearRenderer(RenderObject*);

    bool hasRunningTransitions() const { return !m_transitions.isEmpty(); }

private:
    void animationTimerFired(Timer<AnimationController>*);
    void startTimerIfNeeded();

    // Builds a blended style for the renderer at the current time from its
    // running transitions, starting from base. Returns a new ref'd style.
    RenderStyle* blendedStyle(RenderObject*, RenderStyle* base, double now, bool& anyActive);

    Document* m_document;
    Timer<AnimationController> m_timer;
    HashMap<RenderObject*, Vector<RunningTransition>*> m_transitions;
};

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)

#endif // AnimationController_h
