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
#include "CSSKeyframeRule.h"
#include "CSSKeyframesRule.h"
#include "CSSStyleSelector.h"
#include "Document.h"
#include "Element.h"
#include "Length.h"
#include "Node.h"
#include "RenderObject.h"
#include "RenderStyle.h"
#include "SystemTime.h"
#include <math.h>

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

// The full set of properties this engine can interpolate. Shared by transitions
// (which filter by declared transition-property) and keyframe animations.
static const int kAnimatableProps[] = {
    CSS_PROP_OPACITY, CSS_PROP_WIDTH, CSS_PROP_HEIGHT,
    CSS_PROP_MARGIN_TOP, CSS_PROP_MARGIN_BOTTOM, CSS_PROP_MARGIN_LEFT, CSS_PROP_MARGIN_RIGHT,
    CSS_PROP_PADDING_TOP, CSS_PROP_PADDING_BOTTOM, CSS_PROP_PADDING_LEFT, CSS_PROP_PADDING_RIGHT,
    CSS_PROP_COLOR
};
static const int kNumAnimatableProps = sizeof(kAnimatableProps) / sizeof(kAnimatableProps[0]);

// Interpolates every animatable property that differs between from and to,
// writing the result into dst.
static void blendAllProperties(RenderStyle* dst, RenderStyle* from, RenderStyle* to, double p)
{
    for (int i = 0; i < kNumAnimatableProps; ++i) {
        int prop = kAnimatableProps[i];
        if (propertyDiffers(prop, from, to))
            applyBlendedProperty(prop, dst, from, to, p);
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

    HashMap<RenderObject*, Vector<RunningKeyframeAnimation>*>::iterator kend = m_keyframeAnimations.end();
    for (HashMap<RenderObject*, Vector<RunningKeyframeAnimation>*>::iterator it = m_keyframeAnimations.begin(); it != kend; ++it) {
        Vector<RunningKeyframeAnimation>* anims = it->second;
        for (size_t i = 0; i < anims->size(); ++i)
            clearKeyframeStyles(anims->at(i));
        delete anims;
    }
    m_keyframeAnimations.clear();
}

void AnimationController::startTimerIfNeeded()
{
    if ((!m_transitions.isEmpty() || !m_keyframeAnimations.isEmpty()) && !m_timer.isActive())
        m_timer.startRepeating(kAnimationInterval);
}

void AnimationController::clearKeyframeStyles(RunningKeyframeAnimation& anim)
{
    for (size_t i = 0; i < anim.m_styles.size(); ++i) {
        if (anim.m_styles[i])
            anim.m_styles[i]->deref(m_document->renderArena());
    }
    anim.m_styles.clear();
    anim.m_offsets.clear();
}

void AnimationController::clearRenderer(RenderObject* renderer)
{
    Vector<RunningTransition>* list = m_transitions.get(renderer);
    if (list) {
        for (size_t i = 0; i < list->size(); ++i) {
            if (list->at(i).m_fromStyle)
                list->at(i).m_fromStyle->deref(m_document->renderArena());
            if (list->at(i).m_toStyle)
                list->at(i).m_toStyle->deref(m_document->renderArena());
        }
        delete list;
        m_transitions.remove(renderer);
    }

    Vector<RunningKeyframeAnimation>* anims = m_keyframeAnimations.get(renderer);
    if (anims) {
        for (size_t i = 0; i < anims->size(); ++i)
            clearKeyframeStyles(anims->at(i));
        delete anims;
        m_keyframeAnimations.remove(renderer);
    }

    if (m_transitions.isEmpty() && m_keyframeAnimations.isEmpty())
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

    bool startedAny = false;
    for (int i = 0; i < kNumAnimatableProps; ++i) {
        int prop = kAnimatableProps[i];
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

// Computes the iteration-local progress in [0,1] for a keyframe animation at
// time now, honoring delay, iteration count and direction. Sets finished=true
// once the animation has completed all iterations.
static double keyframeProgress(const RunningKeyframeAnimation& anim, double now, bool& finished)
{
    finished = false;
    const KeyframeAnimation& p = anim.m_params;
    double elapsed = now - anim.m_startTime - p.delay();
    if (elapsed <= 0)
        return 0.0; // delay phase: hold first keyframe

    if (p.duration() <= 0) {
        finished = true;
        return 1.0;
    }

    double rawIteration = elapsed / p.duration();
    double totalIterations = p.isInfinite() ? 1e18 : p.iterationCount();
    if (!p.isInfinite() && rawIteration >= totalIterations) {
        finished = true;
        rawIteration = totalIterations; // clamp to the end
    }

    double iterationIndex = (rawIteration < totalIterations) ? floor(rawIteration) : floor(totalIterations - 1e-9);
    double frac = rawIteration - iterationIndex;
    if (frac < 0) frac = 0;
    if (frac > 1) frac = 1;

    // Direction handling: reverse / alternate.
    EAnimationDirection dir = p.direction();
    bool reverse = false;
    if (dir == AnimDirReverse)
        reverse = true;
    else if (dir == AnimDirAlternate)
        reverse = (((long)iterationIndex) & 1) != 0;
    else if (dir == AnimDirAlternateReverse)
        reverse = (((long)iterationIndex) & 1) == 0;

    return reverse ? (1.0 - frac) : frac;
}

RenderStyle* AnimationController::animatedKeyframeStyle(RenderObject* renderer, RenderStyle* base,
                                                        double now, bool& anyActive)
{
    Vector<RunningKeyframeAnimation>* anims = m_keyframeAnimations.get(renderer);
    if (!anims || anims->isEmpty()) {
        anyActive = false;
        return 0;
    }

    RenderStyle* result = new (m_document->renderArena()) RenderStyle(*base);
    result->ref();

    anyActive = false;
    for (size_t a = 0; a < anims->size(); ++a) {
        RunningKeyframeAnimation& anim = anims->at(a);
        if (anim.m_offsets.size() < 2)
            continue;
        if (anim.m_params.playState() == AnimPlayPaused) {
            anyActive = true; // paused animations remain "active" (held)
        }

        bool finished = false;
        double prog = keyframeProgress(anim, now, finished);
        if (!finished && anim.m_params.playState() != AnimPlayPaused)
            anyActive = true;

        // Apply timing function to the progress within the iteration.
        prog = anim.m_params.timingFunction().evaluate(prog);

        // Find the two surrounding keyframes for the (eased) progress.
        size_t hi = anim.m_offsets.size() - 1;
        for (size_t i = 0; i < anim.m_offsets.size(); ++i) {
            if (anim.m_offsets[i] >= prog) { hi = i; break; }
        }
        size_t lo = (hi == 0) ? 0 : hi - 1;

        RenderStyle* fromStyle = anim.m_styles[lo];
        RenderStyle* toStyle = anim.m_styles[hi];
        double segLen = anim.m_offsets[hi] - anim.m_offsets[lo];
        double localP = (segLen > 0) ? (prog - anim.m_offsets[lo]) / segLen : 0.0;
        if (localP < 0) localP = 0;
        if (localP > 1) localP = 1;

        if (fromStyle && toStyle)
            blendAllProperties(result, fromStyle, toStyle, localP);
    }

    return result;
}

RenderStyle* AnimationController::updateAnimations(RenderObject* renderer, RenderStyle* newStyle)
{
    if (!newStyle || !newStyle->hasAnimations())
        return 0;

    Element* element = 0;
    if (renderer->node() && renderer->node()->isElementNode())
        element = static_cast<Element*>(renderer->node());
    if (!element)
        return 0;

    CSSStyleSelector* selector = m_document->styleSelector();
    if (!selector)
        return 0;

    const AnimationList& decls = newStyle->animations();
    Vector<RunningKeyframeAnimation>* anims = m_keyframeAnimations.get(renderer);

    double now = currentTime();
    bool startedAny = false;

    for (size_t i = 0; i < decls.size(); ++i) {
        const KeyframeAnimation& decl = decls[i];
        if (decl.name().isEmpty() || decl.duration() <= 0)
            continue;

        // Already running with the same name? Skip (don't restart each restyle).
        bool alreadyRunning = false;
        if (anims) {
            for (size_t j = 0; j < anims->size(); ++j) {
                if (anims->at(j).m_name == decl.name()) { alreadyRunning = true; break; }
            }
        }
        if (alreadyRunning)
            continue;

        CSSKeyframesRule* rule = selector->keyframesRule(decl.name());
        if (!rule || rule->length() == 0)
            continue;

        // Resolve each keyframe into a style, expanding multi-key keyframes into
        // individual (offset, style) pairs, then sort ascending by offset.
        RunningKeyframeAnimation anim;
        anim.m_name = decl.name();
        anim.m_startTime = now;
        anim.m_params = decl;

        for (unsigned k = 0; k < rule->length(); ++k) {
            CSSKeyframeRule* kf = rule->item(k);
            if (!kf)
                continue;
            RenderStyle* kfStyle = selector->styleForKeyframe(element, newStyle, kf);
            if (!kfStyle)
                continue;
            const Vector<float>& keys = kf->keys();
            for (size_t ki = 0; ki < keys.size(); ++ki) {
                // Insert (key, style) keeping m_offsets ascending. Ref the style
                // once per insertion (each slot owns a ref).
                float key = keys[ki];
                size_t pos = anim.m_offsets.size();
                for (size_t p = 0; p < anim.m_offsets.size(); ++p) {
                    if (key < anim.m_offsets[p]) { pos = p; break; }
                }
                kfStyle->ref();
                anim.m_offsets.insert(pos, key);
                anim.m_styles.insert(pos, kfStyle);
            }
            kfStyle->deref(m_document->renderArena()); // drop the creation ref
        }

        if (anim.m_offsets.size() < 2) {
            clearKeyframeStyles(anim);
            continue;
        }

        if (!anims) {
            anims = new Vector<RunningKeyframeAnimation>();
            m_keyframeAnimations.set(renderer, anims);
        }
        anims->append(anim);
        startedAny = true;
    }

    if (!startedAny && (!anims || anims->isEmpty()))
        return 0;

    startTimerIfNeeded();

    bool anyActive = false;
    return animatedKeyframeStyle(renderer, newStyle, now, anyActive);
}

void AnimationController::animationTimerFired(Timer<AnimationController>*)
{
    double now = currentTime();

    // Snapshot the union of renderers with transitions and/or keyframe
    // animations, since applying styles may mutate the maps.
    Vector<RenderObject*> renderers;
    {
        HashMap<RenderObject*, Vector<RunningTransition>*>::iterator end = m_transitions.end();
        for (HashMap<RenderObject*, Vector<RunningTransition>*>::iterator it = m_transitions.begin(); it != end; ++it)
            renderers.append(it->first);
        HashMap<RenderObject*, Vector<RunningKeyframeAnimation>*>::iterator kend = m_keyframeAnimations.end();
        for (HashMap<RenderObject*, Vector<RunningKeyframeAnimation>*>::iterator it = m_keyframeAnimations.begin(); it != kend; ++it) {
            if (!m_transitions.contains(it->first))
                renderers.append(it->first);
        }
    }

    for (size_t r = 0; r < renderers.size(); ++r) {
        RenderObject* renderer = renderers[r];

        // Compose keyframe animation first (it overlays the base style), then
        // transitions on top, so an explicit transition can still tween.
        bool kfActive = false;
        RenderStyle* afterKeyframes = animatedKeyframeStyle(renderer, renderer->style(), now, kfActive);
        RenderStyle* baseForTransition = afterKeyframes ? afterKeyframes : renderer->style();

        bool trActive = false;
        RenderStyle* blended = blendedStyle(renderer, baseForTransition, now, trActive);

        RenderStyle* toApply = blended ? blended : afterKeyframes;
        if (toApply) {
            renderer->setAnimatedStyle(toApply);
            if (blended)
                blended->deref(m_document->renderArena());
            if (afterKeyframes)
                afterKeyframes->deref(m_document->renderArena());
        }

        // Drop finished transitions.
        Vector<RunningTransition>* list = m_transitions.get(renderer);
        if (list) {
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

        // Drop finished keyframe animations (infinite ones never finish).
        Vector<RunningKeyframeAnimation>* anims = m_keyframeAnimations.get(renderer);
        if (anims) {
            for (size_t i = 0; i < anims->size();) {
                bool finished = false;
                keyframeProgress(anims->at(i), now, finished);
                if (finished) {
                    clearKeyframeStyles(anims->at(i));
                    anims->remove(i);
                } else
                    ++i;
            }
            if (anims->isEmpty()) {
                delete anims;
                m_keyframeAnimations.remove(renderer);
            }
        }
    }

    if (m_transitions.isEmpty() && m_keyframeAnimations.isEmpty())
        m_timer.stop();
}

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)
