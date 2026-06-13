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

// Verifies CSS aspect-ratio layout geometry and object-fit parsing.

class AspectRatioTest : public ::testing::Test
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

    RenderObject* rendererById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        return (el && el->renderer()) ? el->renderer() : 0;
    }

    int heightById(const char* id)
    {
        RenderObject* r = rendererById(id);
        return r ? r->height() : -1;
    }

    TestWebView* view;
};

TEST_F(AspectRatioTest, RatioSlashSyntaxDerivesHeight)
{
    // width 200px, aspect-ratio 2/1 -> content height 100px (no border/padding).
    loadHtml("<div id='a' style='width:200px; aspect-ratio:2/1;'></div>");
    EXPECT_EQ(heightById("a"), 100);
}

TEST_F(AspectRatioTest, SingleNumberRatio)
{
    // aspect-ratio: 4 means 4/1 -> 200/4 = 50.
    loadHtml("<div id='a' style='width:200px; aspect-ratio:4;'></div>");
    EXPECT_EQ(heightById("a"), 50);
}

TEST_F(AspectRatioTest, ExplicitHeightWins)
{
    // A definite height overrides aspect-ratio derivation.
    loadHtml("<div id='a' style='width:200px; height:300px; aspect-ratio:2/1;'></div>");
    EXPECT_EQ(heightById("a"), 300);
}

TEST_F(AspectRatioTest, RatioIncludesPaddingInBox)
{
    // box-sizing content-box (default): content height = 200/2 = 100, plus
    // 10px padding top+bottom -> border-box height 120.
    loadHtml("<div id='a' style='width:200px; padding:10px; aspect-ratio:2/1;'></div>");
    EXPECT_EQ(heightById("a"), 120);
}

TEST_F(AspectRatioTest, AutoMeansNoRatio)
{
    loadHtml("<div id='a' style='width:200px; aspect-ratio:auto;'></div>");
    RenderObject* r = rendererById("a");
    ASSERT_TRUE(r != 0);
    EXPECT_FALSE(r->style()->hasAspectRatio());
}

TEST_F(AspectRatioTest, StyleStoresRatioValue)
{
    loadHtml("<div id='a' style='width:200px; aspect-ratio:16/9;'></div>");
    RenderObject* r = rendererById("a");
    ASSERT_TRUE(r != 0);
    EXPECT_TRUE(r->style()->hasAspectRatio());
    EXPECT_NEAR(r->style()->aspectRatio(), 16.0f / 9.0f, 0.001f);
}

TEST_F(AspectRatioTest, ObjectFitParsesKeywords)
{
    loadHtml("<img id='i1' style='object-fit:contain;'>"
             "<img id='i2' style='object-fit:cover;'>"
             "<img id='i3' style='object-fit:none;'>"
             "<img id='i4' style='object-fit:scale-down;'>"
             "<img id='i5' style='object-fit:fill;'>"
             "<img id='i6'>");
    EXPECT_EQ(rendererById("i1")->style()->objectFit(), OF_CONTAIN);
    EXPECT_EQ(rendererById("i2")->style()->objectFit(), OF_COVER);
    EXPECT_EQ(rendererById("i3")->style()->objectFit(), OF_NONE);
    EXPECT_EQ(rendererById("i4")->style()->objectFit(), OF_SCALE_DOWN);
    EXPECT_EQ(rendererById("i5")->style()->objectFit(), OF_FILL);
    // Default when unspecified is fill.
    EXPECT_EQ(rendererById("i6")->style()->objectFit(), OF_FILL);
}

TEST_F(AspectRatioTest, ObjectFitRejectsInvalid)
{
    // An invalid keyword must not change the initial value (fill).
    loadHtml("<img id='i1' style='object-fit:wobble;'>");
    EXPECT_EQ(rendererById("i1")->style()->objectFit(), OF_FILL);
}

TEST_F(AspectRatioTest, AspectRatioRejectsZeroAndNegative)
{
    // Zero or negative components are invalid; ratio stays none.
    loadHtml("<div id='a' style='width:200px; aspect-ratio:0/1;'></div>"
             "<div id='b' style='width:200px; aspect-ratio:-2;'></div>");
    EXPECT_FALSE(rendererById("a")->style()->hasAspectRatio());
    EXPECT_FALSE(rendererById("b")->style()->hasAspectRatio());
}
