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

#include "test.h"
#include "RenderObject.h"
#include "RenderStyle.h"
#include "Animation.h"
#include "Color.h"
#include "CSSStyleDeclaration.h"
#include "Element.h"
#include "ExceptionCode.h"

#if ENABLE(CSS_TRANSITIONS)

using namespace WebCore;

// --- TimingFunction math (no DOM needed) ------------------------------------

class TimingFunctionTest : public ::testing::Test
{
};

TEST_F(TimingFunctionTest, LinearIsIdentity)
{
    TimingFunction tf = TimingFunction::linear();
    EXPECT_NEAR(tf.evaluate(0.0), 0.0, 0.001);
    EXPECT_NEAR(tf.evaluate(0.25), 0.25, 0.01);
    EXPECT_NEAR(tf.evaluate(0.5), 0.5, 0.01);
    EXPECT_NEAR(tf.evaluate(0.75), 0.75, 0.01);
    EXPECT_NEAR(tf.evaluate(1.0), 1.0, 0.001);
}

TEST_F(TimingFunctionTest, EndpointsClamp)
{
    TimingFunction tf = TimingFunction::ease();
    EXPECT_EQ(tf.evaluate(-1.0), 0.0);
    EXPECT_EQ(tf.evaluate(2.0), 1.0);
    EXPECT_NEAR(tf.evaluate(0.0), 0.0, 0.001);
    EXPECT_NEAR(tf.evaluate(1.0), 1.0, 0.001);
}

TEST_F(TimingFunctionTest, EaseInStartsSlow)
{
    // ease-in: output at the midpoint should be below linear (slow start).
    TimingFunction tf = TimingFunction::easeIn();
    EXPECT_LT(tf.evaluate(0.5), 0.5);
}

TEST_F(TimingFunctionTest, EaseOutEndsSlow)
{
    // ease-out: output at the midpoint should be above linear (fast start).
    TimingFunction tf = TimingFunction::easeOut();
    EXPECT_GT(tf.evaluate(0.5), 0.5);
}

TEST_F(TimingFunctionTest, MonotonicNonDecreasing)
{
    TimingFunction tf = TimingFunction::ease();
    double prev = tf.evaluate(0.0);
    for (int i = 1; i <= 20; ++i) {
        double v = tf.evaluate(i / 20.0);
        EXPECT_GE(v, prev - 0.001);
        prev = v;
    }
}

// --- Transition parsing (needs the style pipeline) --------------------------

class TransitionParseTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite() { Test_Init(); }
    static void TearDownTestSuite() { Test_Shutdown(); }

    void SetUp() override { view = new TestWebView(); }
    void TearDown() override { delete view; }

    void loadHtml(const char* html)
    {
        view->loadHtml(html, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    RenderStyle* styleById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        return (el && el->renderer()) ? el->renderer()->style() : 0;
    }

    TestWebView* view;
};

TEST_F(TransitionParseTest, ShorthandSingle)
{
    loadHtml("<div id='a' style='transition: opacity 0.5s ease-in 0.1s;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    ASSERT_TRUE(s->hasTransitions());
    const TransitionList& list = s->transitions();
    ASSERT_EQ(list.size(), (size_t)1);
    EXPECT_FALSE(list[0].isAll());
    EXPECT_NEAR(list[0].duration(), 0.5, 0.001);
    EXPECT_NEAR(list[0].delay(), 0.1, 0.001);
}

TEST_F(TransitionParseTest, ShorthandMultiple)
{
    loadHtml("<div id='a' style='transition: opacity 0.5s linear, width 1s ease;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const TransitionList& list = s->transitions();
    ASSERT_EQ(list.size(), (size_t)2);
    EXPECT_NEAR(list[0].duration(), 0.5, 0.001);
    EXPECT_NEAR(list[1].duration(), 1.0, 0.001);
}

TEST_F(TransitionParseTest, MillisecondUnit)
{
    loadHtml("<div id='a' style='transition: opacity 250ms linear;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const TransitionList& list = s->transitions();
    ASSERT_EQ(list.size(), (size_t)1);
    EXPECT_NEAR(list[0].duration(), 0.25, 0.001);
}

TEST_F(TransitionParseTest, AllKeyword)
{
    loadHtml("<div id='a' style='transition: all 0.3s linear;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const TransitionList& list = s->transitions();
    ASSERT_EQ(list.size(), (size_t)1);
    EXPECT_TRUE(list[0].isAll());
}

TEST_F(TransitionParseTest, CubicBezierTimingFunction)
{
    loadHtml("<div id='a' style='transition: opacity 1s cubic-bezier(0.1, 0.2, 0.3, 0.4);'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const TransitionList& list = s->transitions();
    ASSERT_EQ(list.size(), (size_t)1);
    const TimingFunction& tf = list[0].timingFunction();
    EXPECT_NEAR(tf.x1(), 0.1, 0.001);
    EXPECT_NEAR(tf.y1(), 0.2, 0.001);
    EXPECT_NEAR(tf.x2(), 0.3, 0.001);
    EXPECT_NEAR(tf.y2(), 0.4, 0.001);
}

TEST_F(TransitionParseTest, LonghandDuration)
{
    loadHtml("<div id='a' style='transition-property: opacity; transition-duration: 0.7s;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    ASSERT_TRUE(s->hasTransitions());
}

// --- Animation parsing ------------------------------------------------------

class AnimationParseTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite() { Test_Init(); }
    static void TearDownTestSuite() { Test_Shutdown(); }

    void SetUp() override { view = new TestWebView(); }
    void TearDown() override { delete view; }

    void loadHtml(const char* html)
    {
        view->loadHtml(html, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    RenderStyle* styleById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        return (el && el->renderer()) ? el->renderer()->style() : 0;
    }

    TestWebView* view;
};

TEST_F(AnimationParseTest, ShorthandFull)
{
    loadHtml("<style>@keyframes k{from{opacity:1;}to{opacity:0;}}</style>"
             "<div id='a' style='animation: k 2s ease-in 0.5s 3 alternate both;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    ASSERT_TRUE(s->hasAnimations());
    const AnimationList& list = s->animations();
    ASSERT_EQ(list.size(), (size_t)1);
    EXPECT_EQ(list[0].name(), String("k"));
    EXPECT_NEAR(list[0].duration(), 2.0, 0.001);
    EXPECT_NEAR(list[0].delay(), 0.5, 0.001);
    EXPECT_NEAR(list[0].iterationCount(), 3.0, 0.001);
    EXPECT_EQ(list[0].direction(), AnimDirAlternate);
    EXPECT_EQ(list[0].fillMode(), AnimFillBoth);
}

TEST_F(AnimationParseTest, InfiniteIteration)
{
    loadHtml("<div id='a' style='animation: spin 1s linear infinite;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const AnimationList& list = s->animations();
    ASSERT_EQ(list.size(), (size_t)1);
    EXPECT_TRUE(list[0].isInfinite());
}

TEST_F(AnimationParseTest, Longhands)
{
    loadHtml("<div id='a' style='animation-name: foo; animation-duration: 1.5s; "
             "animation-direction: reverse; animation-play-state: paused;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    ASSERT_TRUE(s->hasAnimations());
    const AnimationList& list = s->animations();
    ASSERT_GE(list.size(), (size_t)1);
}

TEST_F(AnimationParseTest, MultipleAnimations)
{
    loadHtml("<div id='a' style='animation: a 1s linear, b 2s ease;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    const AnimationList& list = s->animations();
    ASSERT_EQ(list.size(), (size_t)2);
    EXPECT_EQ(list[0].name(), String("a"));
    EXPECT_EQ(list[1].name(), String("b"));
}

// --- Transform interpolation (runs the controller over real time) -----------

TEST_F(AnimationParseTest, TransformKeyframeInterpolates)
{
    loadHtml("<style>@keyframes mv { from { -webkit-transform: translate(0px, 0px); }"
             " to { -webkit-transform: translate(200px, 0px); } }</style>"
             "<div id='a' style='width:50px;height:50px; animation: mv 1s linear;'>x</div>");

    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    // Pump events for ~500ms of wall-clock time so the timer advances the
    // animation roughly to its midpoint.
    for (int i = 0; i < 60; ++i) {
        Test_EventDispatchOnce();
        usleep(9000); // ~9ms; 60 iterations ~= 540ms
    }

    RenderStyle* mid = styleById("a");
    ASSERT_TRUE(mid != 0);
    ASSERT_TRUE(mid->hasTransform());
    const Vector<TransformOperation>& ops = mid->transformOperations();
    ASSERT_EQ(ops.size(), (size_t)1);
    EXPECT_EQ(ops[0].type, TransformOperation::TranslateOp);
    // Midpoint of 0 -> 200 is ~100; allow generous tolerance for timing jitter.
    EXPECT_GT(ops[0].x, 40.0f);
    EXPECT_LT(ops[0].x, 170.0f);
}

TEST_F(AnimationParseTest, OffsetAndBorderKeyframeInterpolate)
{
    loadHtml("<style>@keyframes mv {"
             " from { left: 0px; border-top-width: 0px; }"
             " to { left: 200px; border-top-width: 40px; } }</style>"
             "<div id='a' style='position:absolute; left:0px; border:0px solid black;"
             " width:50px;height:50px; animation: mv 1s linear;'>x</div>");

    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    for (int i = 0; i < 60; ++i) {
        Test_EventDispatchOnce();
        usleep(9000); // ~540ms
    }

    RenderStyle* mid = styleById("a");
    ASSERT_TRUE(mid != 0);
    // left ~100 (0->200 linear, half).
    ASSERT_TRUE(mid->left().isFixed());
    EXPECT_GT(mid->left().value(), 40);
    EXPECT_LT(mid->left().value(), 170);
    // border-top-width ~20 (0->40 linear, half).
    EXPECT_GT((int)mid->borderTopWidth(), 8);
    EXPECT_LT((int)mid->borderTopWidth(), 34);
}

TEST_F(AnimationParseTest, BackgroundColorKeyframeInterpolates)
{
    loadHtml("<style>@keyframes bg {"
             " from { background-color: rgb(0,0,0); }"
             " to { background-color: rgb(200,100,50); } }</style>"
             "<div id='a' style='width:50px;height:50px; background-color:rgb(0,0,0);"
             " animation: bg 1s linear;'>x</div>");

    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    for (int i = 0; i < 60; ++i) {
        Test_EventDispatchOnce();
        usleep(9000);
    }

    RenderStyle* mid = styleById("a");
    ASSERT_TRUE(mid != 0);
    Color c = mid->backgroundColor();
    // Midpoint ~ rgb(100, 50, 25); generous tolerance for timing jitter.
    EXPECT_GT(c.red(), 40);
    EXPECT_LT(c.red(), 160);
    EXPECT_GT(c.green(), 18);
    EXPECT_LT(c.green(), 82);
}

TEST_F(AnimationParseTest, FillModeForwardsRetainsFinalFrame)
{
    loadHtml("<style>@keyframes grow { from { width:100px; } to { width:300px; } }</style>"
             "<div id='a' style='width:100px;height:40px;"
             " animation: grow 0.2s linear forwards;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    // Run well past the 0.2s animation; forwards retains the last keyframe.
    for (int i = 0; i < 60; ++i) {
        Test_EventDispatchOnce();
        usleep(9000); // ~540ms >> 200ms
    }

    RenderStyle* end = styleById("a");
    ASSERT_TRUE(end != 0);
    ASSERT_TRUE(end->width().isFixed());
    EXPECT_EQ(end->width().value(), 300);
}

TEST_F(AnimationParseTest, FillModeNoneRevertsToBase)
{
    loadHtml("<style>@keyframes grow { from { width:100px; } to { width:300px; } }</style>"
             "<div id='a' style='width:100px;height:40px;"
             " animation: grow 0.2s linear;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    for (int i = 0; i < 60; ++i) {
        Test_EventDispatchOnce();
        usleep(9000);
    }

    RenderStyle* end = styleById("a");
    ASSERT_TRUE(end != 0);
    ASSERT_TRUE(end->width().isFixed());
    // Without fill-mode, the element reverts to its declared base width.
    EXPECT_EQ(end->width().value(), 100);
}

TEST_F(AnimationParseTest, ZIndexDiscreteSwitch)
{
    loadHtml("<style>@keyframes zi { from { z-index:0; } to { z-index:100; } }</style>"
             "<div id='a' style='position:absolute; width:40px;height:40px; z-index:0;"
             " animation: zi 0.4s linear forwards;'>x</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);

    // Sample early (~100ms, < 50%): still the from value.
    for (int i = 0; i < 11; ++i) { Test_EventDispatchOnce(); usleep(9000); }
    RenderStyle* early = styleById("a");
    ASSERT_TRUE(early != 0);
    EXPECT_EQ(early->zIndex(), 0);

    // Run past the end; forwards retains the to value.
    for (int i = 0; i < 45; ++i) { Test_EventDispatchOnce(); usleep(9000); }
    RenderStyle* late = styleById("a");
    ASSERT_TRUE(late != 0);
    EXPECT_EQ(late->zIndex(), 100);
}

TEST_F(AnimationParseTest, PlayStatePauseFreezesAndResumeContinues)
{
    loadHtml("<style>@keyframes grow { from { width:100px; } to { width:500px; } }</style>"
             "<div id='a' style='width:100px;height:40px; animation: grow 2s linear;'>x</div>");
    Element* el = view->mainframe()->document()->getElementById(String("a"));
    ASSERT_TRUE(el != 0);

    // Run ~400ms (~20% of 2s -> width ~180).
    for (int i = 0; i < 44; ++i) { Test_EventDispatchOnce(); usleep(9000); }

    // Pause via the inline style longhand (the common JS pattern).
    ExceptionCode ec = 0;
    el->style()->setProperty(String("animation-play-state"), String("paused"), ec);
    el->document()->updateRendering();
    Test_EventDispatchOnce();
    usleep(20000);
    Test_EventDispatchOnce();

    int frozen = el->renderer()->style()->width().value();

    // Wait ~400ms while paused; the width must not advance.
    for (int i = 0; i < 44; ++i) { Test_EventDispatchOnce(); usleep(9000); }
    int stillFrozen = el->renderer()->style()->width().value();
    EXPECT_NEAR(stillFrozen, frozen, 12); // frozen (small jitter tolerance)

    // Resume; the animation should continue advancing from the frozen point.
    el->style()->setProperty(String("animation-play-state"), String("running"), ec);
    el->document()->updateRendering();
    Test_EventDispatchOnce();
    for (int i = 0; i < 33; ++i) { Test_EventDispatchOnce(); usleep(9000); }
    int resumed = el->renderer()->style()->width().value();
    EXPECT_GT(resumed, frozen + 20); // advanced past the frozen value
}

#endif // ENABLE(CSS_TRANSITIONS)
