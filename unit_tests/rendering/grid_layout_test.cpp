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

#if ENABLE(MODERN_GRID)

class GridLayoutTest : public ::testing::Test
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

    RenderObject* renderer(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        return el ? el->renderer() : 0;
    }

    TestWebView* view;
};

TEST_F(GridLayoutTest, FixedTracks)
{
    loadHtml("<div style='display:grid; grid-template-columns:100px 200px; grid-template-rows:50px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 200, 2);
    EXPECT_NEAR(b->xPos() - a->xPos(), 100, 2);
}

TEST_F(GridLayoutTest, EqualFrTracks)
{
    loadHtml("<div style='display:grid; grid-template-columns:1fr 1fr; grid-template-rows:60px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 200, 2);
    EXPECT_NEAR(b->width(), 200, 2);
}

TEST_F(GridLayoutTest, FrRatio)
{
    loadHtml("<div style='display:grid; grid-template-columns:1fr 3fr; grid-template-rows:40px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 300, 2);
}

TEST_F(GridLayoutTest, FixedPlusFr)
{
    loadHtml("<div style='display:grid; grid-template-columns:100px 1fr; grid-template-rows:40px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 300, 2);
}

TEST_F(GridLayoutTest, ColumnGap)
{
    loadHtml("<div style='display:grid; grid-template-columns:1fr 1fr; grid-template-rows:50px; width:410px; column-gap:10px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 200, 2);
    EXPECT_NEAR(b->width(), 200, 2);
    EXPECT_NEAR(b->xPos() - a->xPos(), 210, 2);
}

TEST_F(GridLayoutTest, TwoByTwoWithRows)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:1fr 1fr; grid-template-rows:50px 80px; width:200px;'>"
             "<div id='a'>1</div><div id='b'>2</div><div id='c'>3</div><div id='d'>4</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(a && d);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(a->height(), 50, 2);
    EXPECT_NEAR(d->xPos() - a->xPos(), 100, 2);
    EXPECT_NEAR(d->yPos() - a->yPos(), 50, 2);
    EXPECT_NEAR(d->height(), 80, 2);
}

TEST_F(GridLayoutTest, PercentTracks)
{
    loadHtml("<div style='display:grid; grid-template-columns:25% 75%; grid-template-rows:40px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 300, 2);
}

TEST_F(GridLayoutTest, RepeatTracks)
{
    loadHtml("<div style='display:grid; grid-template-columns:repeat(4, 1fr); grid-template-rows:40px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div><div id='c'>C</div><div id='d'>D</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(a && d);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(d->xPos() - a->xPos(), 300, 2);
}

#endif // ENABLE(MODERN_GRID)
