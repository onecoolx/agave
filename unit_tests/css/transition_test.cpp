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

#endif // ENABLE(CSS_TRANSITIONS)
