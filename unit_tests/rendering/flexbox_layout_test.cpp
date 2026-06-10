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

#if ENABLE(MODERN_FLEXBOX)

class FlexboxLayoutTest : public ::testing::Test
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

    Element* byId(const char* id)
    {
        return view->mainframe()->document()->getElementById(String(id));
    }

    RenderObject* renderer(const char* id)
    {
        Element* el = byId(id);
        return el ? el->renderer() : 0;
    }

    TestWebView* view;
};

// --- CSS Property Parsing ---

TEST_F(FlexboxLayoutTest, DisplayFlexCreatesFlexibleBox)
{
    loadHtml("<div id='f' style='display:flex;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_TRUE(r->isFlexibleBox());
    EXPECT_EQ(r->style()->display(), FLEX);
}

TEST_F(FlexboxLayoutTest, DisplayInlineFlexCreatesFlexibleBox)
{
    loadHtml("<span id='f' style='display:inline-flex;'><div>A</div></span>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_TRUE(r->isFlexibleBox());
    EXPECT_EQ(r->style()->display(), INLINE_FLEX);
}

TEST_F(FlexboxLayoutTest, FlexDirectionParsing)
{
    loadHtml("<div id='f' style='display:flex; flex-direction:column;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->flexDirection(), FlowColumn);
}

TEST_F(FlexboxLayoutTest, FlexWrapParsing)
{
    loadHtml("<div id='f' style='display:flex; flex-wrap:wrap;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->flexWrap(), FlexWrap);
}

TEST_F(FlexboxLayoutTest, JustifyContentParsing)
{
    loadHtml("<div id='f' style='display:flex; justify-content:space-between;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->justifyContent(), JustifySpaceBetween);
}

TEST_F(FlexboxLayoutTest, AlignItemsParsing)
{
    loadHtml("<div id='f' style='display:flex; align-items:center;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->alignItems(), AlignCenter);
}

TEST_F(FlexboxLayoutTest, AlignContentParsing)
{
    loadHtml("<div id='f' style='display:flex; flex-wrap:wrap; align-content:space-around;'><div>A</div></div>");
    RenderObject* r = renderer("f");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->alignContent(), ContentSpaceAround);
}

TEST_F(FlexboxLayoutTest, FlexGrowShrinkBasisParsing)
{
    loadHtml("<div style='display:flex;'><div id='a' style='flex-grow:2; flex-shrink:3; flex-basis:50px;'>A</div></div>");
    RenderObject* r = renderer("a");
    ASSERT_TRUE(r);
    EXPECT_FLOAT_EQ(r->style()->flexGrow(), 2.0f);
    EXPECT_FLOAT_EQ(r->style()->flexShrink(), 3.0f);
    EXPECT_EQ(r->style()->flexBasis().value(), 50);
}

// --- Layout Geometry ---

TEST_F(FlexboxLayoutTest, EqualGrowSplit)
{
    loadHtml("<div style='display:flex; width:400px;'>"
             "<div id='a' style='flex:1;'>A</div>"
             "<div id='b' style='flex:1;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 200, 2);
    EXPECT_NEAR(b->width(), 200, 2);
}

TEST_F(FlexboxLayoutTest, GrowRatio)
{
    loadHtml("<div style='display:flex; width:300px;'>"
             "<div id='a' style='flex-grow:1; flex-basis:0%;'>A</div>"
             "<div id='b' style='flex-grow:2; flex-basis:0%;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 200, 2);
}

TEST_F(FlexboxLayoutTest, FlexShrink)
{
    loadHtml("<div style='display:flex; width:200px;'>"
             "<div id='a' style='width:150px; flex-shrink:1;'>A</div>"
             "<div id='b' style='width:150px; flex-shrink:1;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 100, 2);
}

TEST_F(FlexboxLayoutTest, JustifyContentCenter)
{
    loadHtml("<div id='c' style='display:flex; width:400px; justify-content:center;'>"
             "<div id='a' style='width:100px; height:50px;'>A</div>"
             "<div id='b' style='width:100px; height:50px;'>B</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(c && a);
    EXPECT_NEAR(a->xPos() - c->xPos(), 100, 2);
}

TEST_F(FlexboxLayoutTest, AlignItemsCenter)
{
    loadHtml("<div id='c' style='display:flex; width:400px; height:100px; align-items:center;'>"
             "<div id='a' style='width:50px; height:30px;'>A</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(c && a);
    EXPECT_NEAR(a->yPos() - c->yPos(), 35, 2);
}

TEST_F(FlexboxLayoutTest, FlexDirectionColumn)
{
    loadHtml("<div style='display:flex; flex-direction:column; width:100px; height:300px;'>"
             "<div id='a' style='flex:1;'>A</div>"
             "<div id='b' style='flex:1;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->height(), 150, 2);
    EXPECT_NEAR(b->height(), 150, 2);
}

TEST_F(FlexboxLayoutTest, FlexWrapMultiLine)
{
    loadHtml("<div id='c' style='display:flex; flex-wrap:wrap; width:200px;'>"
             "<div id='a' style='width:90px; height:40px;'>A</div>"
             "<div id='b' style='width:90px; height:40px;'>B</div>"
             "<div id='d' style='width:90px; height:40px;'>C</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* b = renderer("b");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(c && b && d);
    // 200px fits two 90px items per line: a,b on line 1; d on line 2.
    EXPECT_NEAR(b->yPos() - c->yPos(), 0, 2);
    EXPECT_NEAR(d->yPos() - c->yPos(), 40, 2);
}

TEST_F(FlexboxLayoutTest, MinWidthConstraint)
{
    loadHtml("<div style='display:flex; width:200px;'>"
             "<div id='a' style='flex:1; min-width:120px;'>A</div>"
             "<div id='b' style='flex:1;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 120, 2);
    EXPECT_NEAR(b->width(), 80, 2);
}

TEST_F(FlexboxLayoutTest, MaxWidthConstraint)
{
    loadHtml("<div style='display:flex; width:400px;'>"
             "<div id='a' style='flex-grow:1; max-width:100px;'>A</div>"
             "<div id='b' style='flex-grow:1;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2);
    EXPECT_NEAR(b->width(), 300, 2);
}

TEST_F(FlexboxLayoutTest, AlignContentSpaceBetween)
{
    loadHtml("<div id='c' style='display:flex; flex-wrap:wrap; width:200px; height:300px; align-content:space-between;'>"
             "<div id='a' style='width:80px; height:40px;'>1</div>"
             "<div id='b' style='width:80px; height:40px;'>2</div>"
             "<div id='d' style='width:80px; height:40px;'>3</div>"
             "<div id='e' style='width:80px; height:40px;'>4</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(c && a && d);
    EXPECT_NEAR(a->yPos() - c->yPos(), 0, 2);
    EXPECT_NEAR(d->yPos() - c->yPos(), 260, 2);
}

TEST_F(FlexboxLayoutTest, OrderParsing)
{
    loadHtml("<div style='display:flex;'><div id='a' style='order:3;'>A</div></div>");
    RenderObject* r = renderer("a");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->flexOrder(), 3);
}

TEST_F(FlexboxLayoutTest, AlignSelfParsing)
{
    loadHtml("<div style='display:flex;'><div id='a' style='align-self:flex-end;'>A</div></div>");
    RenderObject* r = renderer("a");
    ASSERT_TRUE(r);
    EXPECT_EQ(r->style()->alignSelf(), AlignSelfFlexEnd);
}

TEST_F(FlexboxLayoutTest, OrderReorder)
{
    loadHtml("<div id='c' style='display:flex; width:300px;'>"
             "<div id='a' style='width:100px; height:30px; order:2;'>A</div>"
             "<div id='b' style='width:100px; height:30px; order:1;'>B</div>"
             "<div id='d' style='width:100px; height:30px; order:3;'>C</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(c && a && b && d);
    // Visual order: B(1), A(2), C(3)
    EXPECT_NEAR(b->xPos() - c->xPos(), 0, 2);
    EXPECT_NEAR(a->xPos() - c->xPos(), 100, 2);
    EXPECT_NEAR(d->xPos() - c->xPos(), 200, 2);
}

TEST_F(FlexboxLayoutTest, AlignSelfOverride)
{
    loadHtml("<div id='c' style='display:flex; width:300px; height:100px; align-items:flex-start;'>"
             "<div id='a' style='width:50px; height:30px;'>A</div>"
             "<div id='b' style='width:50px; height:30px; align-self:flex-end;'>B</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(c && a && b);
    EXPECT_NEAR(a->yPos() - c->yPos(), 0, 2); // container align-items:flex-start
    EXPECT_NEAR(b->yPos() - c->yPos(), 70, 2); // overridden to flex-end (100-30)
}

TEST_F(FlexboxLayoutTest, AutoMarginPushRight)
{
    loadHtml("<div id='c' style='display:flex; width:400px;'>"
             "<div id='a' style='width:80px; height:30px;'>A</div>"
             "<div id='b' style='width:80px; height:30px; margin-left:auto;'>B</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(c && a && b);
    EXPECT_NEAR(a->xPos() - c->xPos(), 0, 2);
    EXPECT_NEAR(b->xPos() - c->xPos(), 320, 2); // pushed to right edge
}

TEST_F(FlexboxLayoutTest, AutoMarginCenter)
{
    loadHtml("<div id='c' style='display:flex; width:400px;'>"
             "<div id='a' style='width:100px; height:30px; margin-left:auto; margin-right:auto;'>A</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(c && a);
    EXPECT_NEAR(a->xPos() - c->xPos(), 150, 2); // (400-100)/2
}

TEST_F(FlexboxLayoutTest, PercentageBasis)
{
    loadHtml("<div style='display:flex; width:400px;'>"
             "<div id='a' style='flex-basis:25%;'>A</div>"
             "<div id='b' style='flex-basis:50%;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 100, 2); // 25% of 400
    EXPECT_NEAR(b->width(), 200, 2); // 50% of 400
}

TEST_F(FlexboxLayoutTest, ColumnPercentageBasis)
{
    loadHtml("<div style='display:flex; flex-direction:column; width:100px; height:400px;'>"
             "<div id='a' style='flex-basis:25%;'>A</div>"
             "<div id='b' style='flex-basis:50%;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->height(), 100, 2); // 25% of 400
    EXPECT_NEAR(b->height(), 200, 2); // 50% of 400
}

TEST_F(FlexboxLayoutTest, NestedFlex)
{
    loadHtml("<div style='display:flex; width:400px; height:100px;'>"
             "<div id='inner' style='display:flex; flex:1;'>"
             "<div id='i1' style='flex:1;'>A</div>"
             "<div id='i2' style='flex:1;'>B</div></div>"
             "<div id='fixed' style='width:100px;'>F</div></div>");
    RenderObject* inner = renderer("inner");
    RenderObject* i1 = renderer("i1");
    ASSERT_TRUE(inner && i1);
    EXPECT_NEAR(inner->width(), 300, 2); // 400 - 100 fixed
    EXPECT_NEAR(i1->width(), 150, 2); // half of inner
}

TEST_F(FlexboxLayoutTest, ColumnWrap)
{
    loadHtml("<div id='c' style='display:flex; flex-direction:column; flex-wrap:wrap; width:300px; height:200px;'>"
             "<div id='a' style='width:100px; height:80px;'>1</div>"
             "<div id='b' style='width:100px; height:80px;'>2</div>"
             "<div id='d' style='width:100px; height:80px;'>3</div></div>");
    RenderObject* c = renderer("c");
    RenderObject* a = renderer("a");
    RenderObject* d = renderer("d");
    ASSERT_TRUE(c && a && d);
    // a,b fit in column 1 (80+80=160<=200); d wraps to column 2 at top
    EXPECT_NEAR(a->yPos() - c->yPos(), 0, 2);
    EXPECT_NEAR(d->yPos() - c->yPos(), 0, 2);
    EXPECT_GT(d->xPos(), a->xPos()); // d is in a later column
}

TEST_F(FlexboxLayoutTest, AbsoluteChildExcluded)
{
    loadHtml("<div style='display:flex; width:300px; position:relative;'>"
             "<div id='a' style='flex:1; height:30px;'>A</div>"
             "<div id='x' style='position:absolute; width:50px; height:50px;'>X</div>"
             "<div id='b' style='flex:1; height:30px;'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    // Absolute child does not consume main-axis space: a and b each get half.
    EXPECT_NEAR(a->width(), 150, 2);
    EXPECT_NEAR(b->width(), 150, 2);
}

#endif // ENABLE(MODERN_FLEXBOX)
