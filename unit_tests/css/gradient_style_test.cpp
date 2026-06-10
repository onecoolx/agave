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

#if ENABLE(MODERN_CSS3)

class GradientStyleTest : public ::testing::Test
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

    StyleGradient* gradientById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        if (!el || !el->renderStyle()) {
            return 0;
        }
        return el->renderStyle()->backgroundLayers()->backgroundGradient();
    }

    TestWebView* view;
};

TEST_F(GradientStyleTest, LinearGradientTwoStops)
{
    loadHtml("<div id='g' style='background-image:linear-gradient(red, blue);'>X</div>");
    StyleGradient* g = gradientById("g");
    ASSERT_TRUE(g);
    EXPECT_EQ(g->type, StyleGradient::Linear);
    ASSERT_EQ(g->stops.size(), 2u);
    // Default direction is "to bottom" => 180deg.
    EXPECT_FLOAT_EQ(g->angle, 180.0f);
}

TEST_F(GradientStyleTest, LinearGradientWithAngle)
{
    loadHtml("<div id='g' style='background-image:linear-gradient(45deg, red, blue);'>X</div>");
    StyleGradient* g = gradientById("g");
    ASSERT_TRUE(g);
    EXPECT_EQ(g->type, StyleGradient::Linear);
    EXPECT_FLOAT_EQ(g->angle, 45.0f);
    EXPECT_EQ(g->stops.size(), 2u);
}

TEST_F(GradientStyleTest, LinearGradientToRight)
{
    loadHtml("<div id='g' style='background-image:linear-gradient(to right, red, blue);'>X</div>");
    StyleGradient* g = gradientById("g");
    ASSERT_TRUE(g);
    EXPECT_FLOAT_EQ(g->angle, 90.0f);
}

TEST_F(GradientStyleTest, LinearGradientThreeStopsWithPositions)
{
    loadHtml("<div id='g' style='background-image:linear-gradient(red 0%, green 50%, blue 100%);'>X</div>");
    StyleGradient* g = gradientById("g");
    ASSERT_TRUE(g);
    ASSERT_EQ(g->stops.size(), 3u);
    EXPECT_FLOAT_EQ(g->stops[0].position, 0.0f);
    EXPECT_FLOAT_EQ(g->stops[1].position, 0.5f);
    EXPECT_FLOAT_EQ(g->stops[2].position, 1.0f);
}

TEST_F(GradientStyleTest, RadialGradient)
{
    loadHtml("<div id='g' style='background-image:radial-gradient(red, blue);'>X</div>");
    StyleGradient* g = gradientById("g");
    ASSERT_TRUE(g);
    EXPECT_EQ(g->type, StyleGradient::Radial);
    EXPECT_EQ(g->stops.size(), 2u);
}

TEST_F(GradientStyleTest, SingleStopRejected)
{
    // A gradient needs at least two color stops; a single-stop value is invalid
    // and must not produce a gradient.
    loadHtml("<div id='g' style='background-image:linear-gradient(red);'>X</div>");
    EXPECT_FALSE(gradientById("g"));
}

#endif // ENABLE(MODERN_CSS3)
