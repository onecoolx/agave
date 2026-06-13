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

#include "config.h"
#include "AnimationController.h"

#if ENABLE(CSS_TRANSITIONS)

#include "CSSPropertyNames.h"
#include "Color.h"
#include "Document.h"
#include "Length.h"
#include "RenderObject.h"
#include "RenderStyle.h"
#include "SystemTime.h"

namespace WebCore {

// Frame interval for the shared transition timer (~60fps). Low-end devices may
// not keep up; this is a target, not a guarantee.
static const double kAnimationInterval = 0.016;

// ---- Interpolation helpers -------------------------------------------------

static int blendInt(int from, int to, double p)
{
    return static_cast<int>(from + (to - from) * p + (to >= from ? 0.5 : -0.5));
}

static double blendDouble(double from, double to, double p)
{
    return from + (to - from) * p;
}

// Interpolates two Lengths of compatible type. Falls back to the endpoint when
// the types differ or are not numerically interpolable (auto, etc.).
static Length blendLength(const Length& from, const Length& to, double p)
{
    if (from.type() == to.type() && (from.isFixed() || from.isPercent()))
        return Length(blendInt(from.value(), to.value(), p), from.type());
    // Non-interpolable (auto/relative/etc.): jump at the end.
    return (p < 1.0) ? from : to;
}

static Color blendColor(const Color& from, const Color& to, double p)
{
    return Color(blendInt(from.red(), to.red(), p),
                 blendInt(from.green(), to.green(), p),
                 blendInt(from.blue(), to.blue(), p),
                 blendInt(from.alpha(), to.alpha(), p));
}

// Returns true if the named property differs between the two styles in a way we
// know how to interpolate.
static bool propertyDiffers(int property, RenderStyle* a, RenderStyle* b)
{
    switch (property) {
        case CSS_PROP_OPACITY: return a->opacity() != b->opacity();
        case CSS_PROP_WIDTH: return a->width() != b->width();
        case CSS_PROP_HEIGHT: return a->height() != b->height();
        case CSS_PROP_MARGIN_TOP: return a->marginTop() != b->marginTop();
        case CSS_PROP_MARGIN_BOTTOM: return a->marginBottom() != b->marginBottom();
        case CSS_PROP_MARGIN_LEFT: return a->marginLeft() != b->marginLeft();
        case CSS_PROP_MARGIN_RIGHT: return a->marginRight() != b->marginRight();
        case CSS_PROP_PADDING_TOP: return a->paddingTop() != b->paddingTop();
        case CSS_PROP_PADDING_BOTTOM: return a->paddingBottom() != b->paddingBottom();
        case CSS_PROP_PADDING_LEFT: return a->paddingLeft() != b->paddingLeft();
        case CSS_PROP_PADDING_RIGHT: return a->paddingRight() != b->paddingRight();
        case CSS_PROP_COLOR: return !(a->color() == b->color());
        default: return false;
    }
}

// Writes the interpolated value for one property into dst, reading endpoints
// from the transition's from/to styles.
static void applyBlendedProperty(int property, RenderStyle* dst,
                                 RenderStyle* from, RenderStyle* to, double p)
{
    switch (property) {
        case CSS_PROP_OPACITY:
            dst->setOpacity((float)blendDouble(from->opacity(), to->opacity(), p));
            break;
        case CSS_PROP_WIDTH:
            dst->setWidth(blendLength(from->width(), to->width(), p));
            break;
        case CSS_PROP_HEIGHT:
            dst->setHeight(blendLength(from->height(), to->height(), p));
            break;
        case CSS_PROP_MARGIN_TOP:
            dst->setMarginTop(blendLength(from->marginTop(), to->marginTop(), p));
            break;
        case CSS_PROP_MARGIN_BOTTOM:
            dst->setMarginBottom(blendLength(from->marginBottom(), to->marginBottom(), p));
            break;
        case CSS_PROP_MARGIN_LEFT:
            dst->setMarginLeft(blendLength(from->marginLeft(), to->marginLeft(), p));
            break;
        case CSS_PROP_MARGIN_RIGHT:
            dst->setMarginRight(blendLength(from->marginRight(), to->marginRight(), p));
            break;
        case CSS_PROP_PADDING_TOP:
            dst->setPaddingTop(blendLength(from->paddingTop(), to->paddingTop(), p));
            break;
        case CSS_PROP_PADDING_BOTTOM:
            dst->setPaddingBottom(blendLength(from->paddingBottom(), to->paddingBottom(), p));
            break;
        case CSS_PROP_PADDING_LEFT:
            dst->setPaddingLeft(blendLength(from->paddingLeft(), to->paddingLeft(), p));
            break;
        case CSS_PROP_PADDING_RIGHT:
            dst->setPaddingRight(blendLength(from->paddingRight(), to->paddingRight(), p));
            break;
        case CSS_PROP_COLOR:
            dst->setColor(blendColor(from->color(), to->color(), p));
            break;
        default:
            break;
    }
}

// ---- AnimationController ---------------------------------------------------

AnimationController::AnimationController(Document* doc)
    : m_document(doc)
    , m_timer(this, &AnimationController::animationTimerFired)
{
}

AnimationController::~AnimationController()
{
    HashMap<RenderObject*, Vector<RunningTransition>*>::iterator end = m_transitions.end();
    for (HashMap<RenderObject*, Vector<RunningTransition>*>::iterator it = m_transitions.begin(); it != end; ++it) {
        Vector<RunningTransition>* list = it->second;
        for (size_t i = 0; i < list->size(); ++i) {
            if (list->at(i).m_fromStyle)
                list->at(i).m_fromStyle->deref(m_document->renderArena());
            if (list->at(i).m_toStyle)
                list->at(i).m_toStyle->deref(m_document->renderArena());
        }
        delete list;
    }
    m_transitions.clear();
}

void AnimationController::startTimerIfNeeded()
{
    if (!m_transitions.isEmpty() && !m_timer.isActive())
        m_timer.startRepeating(kAnimationInterval);
}

void AnimationController::clearRenderer(RenderObject* renderer)
{
    Vector<RunningTransition>* list = m_transitions.get(renderer);
    if (!list)
        return;
    for (size_t i = 0; i < list->size(); ++i) {
        if (list->at(i).m_fromStyle)
            list->at(i).m_fromStyle->deref(m_document->renderArena());
        if (list->at(i).m_toStyle)
            list->at(i).m_toStyle->deref(m_document->renderArena());
    }
    delete list;
    m_transitions.remove(renderer);
    if (m_transitions.isEmpty())
        m_timer.stop();
}

// Finds the matching transition entry for a property: an explicit per-property
// entry wins over an "all" entry. Returns true and fills out if found.
static bool findTransitionForProperty(const TransitionList& list, int property, Transition& out)
{
    bool found = false;
    for (size_t i = 0; i < list.size(); ++i) {
        const Transition& t = list[i];
        if (t.isAll()) {
            if (!found) { out = t; found = true; }
        } else if (t.property() == property) {
            out = t;
            return true; // explicit match takes precedence
        }
    }
    return found;
}

RenderStyle* AnimationController::updateTransitions(RenderObject* renderer,
                                                    RenderStyle* oldStyle, RenderStyle* newStyle)
{
    if (!oldStyle || !newStyle || !newStyle->hasTransitions())
        return 0;

    const TransitionList& declarations = newStyle->transitions();
    double now = currentTime();

    Vector<RunningTransition>* list = m_transitions.get(renderer);

    // Candidate properties: the union of what we can animate and what changed.
    static const int kProps[] = {
        CSS_PROP_OPACITY, CSS_PROP_WIDTH, CSS_PROP_HEIGHT,
        CSS_PROP_MARGIN_TOP, CSS_PROP_MARGIN_BOTTOM, CSS_PROP_MARGIN_LEFT, CSS_PROP_MARGIN_RIGHT,
        CSS_PROP_PADDING_TOP, CSS_PROP_PADDING_BOTTOM, CSS_PROP_PADDING_LEFT, CSS_PROP_PADDING_RIGHT,
        CSS_PROP_COLOR
    };
    const int kNumProps = sizeof(kProps) / sizeof(kProps[0]);

    bool startedAny = false;
    for (int i = 0; i < kNumProps; ++i) {
        int prop = kProps[i];
        if (!propertyDiffers(prop, oldStyle, newStyle))
            continue;

        Transition decl;
        if (!findTransitionForProperty(declarations, prop, decl))
            continue;
        if (decl.duration() <= 0)
            continue; // zero-duration: no animation, let the new value apply directly

        if (!list) {
            list = new Vector<RunningTransition>();
            m_transitions.set(renderer, list);
        }

        // Remove any existing running transition for this property (retarget).
        for (size_t j = 0; j < list->size(); ++j) {
            if (list->at(j).m_property == prop) {
                if (list->at(j).m_fromStyle)
                    list->at(j).m_fromStyle->deref(m_document->renderArena());
                if (list->at(j).m_toStyle)
                    list->at(j).m_toStyle->deref(m_document->renderArena());
                list->remove(j);
                break;
            }
        }

        RunningTransition rt;
        rt.m_property = prop;
        rt.m_startTime = now;
        rt.m_delay = decl.delay();
        rt.m_duration = decl.duration();
        rt.m_timingFunction = decl.timingFunction();
        rt.m_fromStyle = new (m_document->renderArena()) RenderStyle(*oldStyle);
        rt.m_fromStyle->ref();
        rt.m_toStyle = new (m_document->renderArena()) RenderStyle(*newStyle);
        rt.m_toStyle->ref();
        list->append(rt);
        startedAny = true;
    }

    if (!startedAny && (!list || list->isEmpty()))
        return 0;

    startTimerIfNeeded();

    // Produce the blended style for "now" so the first painted frame already
    // reflects the start of the transition rather than the target value.
    bool anyActive = false;
    return blendedStyle(renderer, newStyle, now, anyActive);
}

RenderStyle* AnimationController::blendedStyle(RenderObject* renderer, RenderStyle* base,
                                               double now, bool& anyActive)
{
    Vector<RunningTransition>* list = m_transitions.get(renderer);
    if (!list || list->isEmpty()) {
        anyActive = false;
        return 0;
    }

    RenderStyle* result = new (m_document->renderArena()) RenderStyle(*base);
    result->ref();

    anyActive = false;
    for (size_t i = 0; i < list->size(); ++i) {
        RunningTransition& rt = list->at(i);
        double elapsed = now - rt.m_startTime - rt.m_delay;
        double fraction;
        if (elapsed <= 0) {
            fraction = 0.0;       // still in delay phase: hold start value
            anyActive = true;
        } else if (elapsed >= rt.m_duration) {
            fraction = 1.0;       // finished
        } else {
            fraction = rt.m_timingFunction.evaluate(elapsed / rt.m_duration);
            anyActive = true;
        }
        applyBlendedProperty(rt.m_property, result, rt.m_fromStyle, rt.m_toStyle, fraction);
    }
    return result;
}

void AnimationController::animationTimerFired(Timer<AnimationController>*)
{
    double now = currentTime();

    // Iterate over a snapshot of renderers, since applying styles may mutate the
    // map (finished transitions get removed).
    Vector<RenderObject*> renderers;
    HashMap<RenderObject*, Vector<RunningTransition>*>::iterator end = m_transitions.end();
    for (HashMap<RenderObject*, Vector<RunningTransition>*>::iterator it = m_transitions.begin(); it != end; ++it)
        renderers.append(it->first);

    for (size_t r = 0; r < renderers.size(); ++r) {
        RenderObject* renderer = renderers[r];
        Vector<RunningTransition>* list = m_transitions.get(renderer);
        if (!list)
            continue;

        bool anyActive = false;
        RenderStyle* blended = blendedStyle(renderer, renderer->style(), now, anyActive);
        if (blended) {
            // Apply without re-entering the transition machinery.
            renderer->setAnimatedStyle(blended);
            blended->deref(m_document->renderArena());
        }

        // Drop finished transitions.
        for (size_t i = 0; i < list->size();) {
            RunningTransition& rt = list->at(i);
            if (now - rt.m_startTime - rt.m_delay >= rt.m_duration) {
                if (rt.m_fromStyle)
                    rt.m_fromStyle->deref(m_document->renderArena());
                if (rt.m_toStyle)
                    rt.m_toStyle->deref(m_document->renderArena());
                list->remove(i);
            } else
                ++i;
        }
        if (list->isEmpty()) {
            delete list;
            m_transitions.remove(renderer);
        }
    }

    if (m_transitions.isEmpty())
        m_timer.stop();
}

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)
