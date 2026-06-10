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

TEST_F(GridLayoutTest, ExplicitColumnPlacement)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px 100px; grid-template-rows:50px; width:300px;'>"
             "<div id='x' style='grid-column-start:3;'>X</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* x = renderer("x");
    ASSERT_TRUE(g && x);
    EXPECT_NEAR(x->xPos() - g->xPos(), 200, 2); // third column
    EXPECT_NEAR(x->width(), 100, 2);
}

TEST_F(GridLayoutTest, ColumnSpan)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px 100px; grid-template-rows:50px; width:300px;'>"
             "<div id='s' style='grid-column-end:span 2;'>S</div><div id='b'>B</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* s = renderer("s");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(g && s && b);
    EXPECT_NEAR(s->width(), 200, 2); // spans 2 columns
    EXPECT_NEAR(b->xPos() - g->xPos(), 200, 2); // auto-placed after span
}

TEST_F(GridLayoutTest, GridColumnShorthandSpan)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px 100px; grid-template-rows:50px; width:300px;'>"
             "<div id='w' style='grid-column:1 / 3;'>W</div><div id='b'>B</div></div>");
    RenderObject* w = renderer("w");
    RenderObject* g = renderer("g");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(w && g && b);
    EXPECT_NEAR(w->width(), 200, 2);
    EXPECT_NEAR(b->xPos() - g->xPos(), 200, 2);
}

TEST_F(GridLayoutTest, AutoFlowAroundExplicit)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px; grid-template-rows:50px 50px; width:200px;'>"
             "<div id='f' style='grid-column:1; grid-row:2;'>F</div>"
             "<div id='a'>A</div><div id='b'>B</div><div id='c'>C</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* f = renderer("f");
    RenderObject* c = renderer("c");
    ASSERT_TRUE(g && f && c);
    EXPECT_NEAR(f->xPos() - g->xPos(), 0, 2);
    EXPECT_NEAR(f->yPos() - g->yPos(), 50, 2);
    // C is third auto item: row1col1(A), row1col2(B), row2col2(C)
    EXPECT_NEAR(c->xPos() - g->xPos(), 100, 2);
    EXPECT_NEAR(c->yPos() - g->yPos(), 50, 2);
}

TEST_F(GridLayoutTest, RowSpan)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px; grid-template-rows:50px 50px; width:200px;'>"
             "<div id='t' style='grid-row:1 / 3;'>T</div><div id='b'>B</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* t = renderer("t");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(g && t && b);
    EXPECT_NEAR(t->height(), 100, 2); // spans 2 rows
    EXPECT_NEAR(b->xPos() - g->xPos(), 100, 2); // auto-placed at col2
    EXPECT_NEAR(b->yPos() - g->yPos(), 0, 2);
}

TEST_F(GridLayoutTest, InlineGridShrinkToTracks)
{
    loadHtml("<span id='g' style='display:inline-grid; grid-template-columns:80px 120px; grid-template-rows:40px;'>"
             "<div id='a'>A</div><div id='b'>B</div></span>");
    RenderObject* g = renderer("g");
    ASSERT_TRUE(g);
    EXPECT_NEAR(g->width(), 200, 2); // 80 + 120
}

TEST_F(GridLayoutTest, InlineGridShrinkWithGap)
{
    loadHtml("<span id='g' style='display:inline-grid; grid-template-columns:50px 50px 50px; grid-template-rows:30px; column-gap:10px;'>"
             "<div>A</div><div>B</div><div>C</div></span>");
    RenderObject* g = renderer("g");
    ASSERT_TRUE(g);
    EXPECT_NEAR(g->width(), 170, 2); // 50*3 + 10*2
}

TEST_F(GridLayoutTest, NestedGrid)
{
    loadHtml("<div id='o' style='display:grid; grid-template-columns:200px 200px; grid-template-rows:100px; width:400px;'>"
             "<div id='inner' style='display:grid; grid-template-columns:1fr 1fr; grid-template-rows:50px;'>"
             "<div id='i1'>1</div><div id='i2'>2</div></div>"
             "<div id='o2'>O2</div></div>");
    RenderObject* inner = renderer("inner");
    RenderObject* i1 = renderer("i1");
    ASSERT_TRUE(inner && i1);
    EXPECT_NEAR(inner->width(), 200, 2);
    EXPECT_NEAR(i1->width(), 100, 2); // half of inner
}

TEST_F(GridLayoutTest, GridNestingFlex)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:1fr 1fr; grid-template-rows:50px; width:400px;'>"
             "<div id='f' style='display:flex;'><div id='fa'>A</div><div id='fb'>B</div></div>"
             "<div id='c'>C</div></div>");
    RenderObject* f = renderer("f");
    RenderObject* fa = renderer("fa");
    ASSERT_TRUE(f && fa);
    EXPECT_NEAR(f->width(), 200, 2); // grid column width
}

TEST_F(GridLayoutTest, MinMaxFixedClampsToMax)
{
    loadHtml("<div style='display:grid; grid-template-columns:minmax(100px, 200px) 200px; grid-template-rows:50px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 200, 2); // non-flexible minmax uses definite max
    EXPECT_NEAR(b->width(), 200, 2);
}

TEST_F(GridLayoutTest, MinMaxWithFrFlexible)
{
    loadHtml("<div style='display:grid; grid-template-columns:minmax(50px, 1fr) 1fr; grid-template-rows:50px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 200, 2); // both flexible, split evenly
    EXPECT_NEAR(b->width(), 200, 2);
}

TEST_F(GridLayoutTest, MinMaxFrPlusFixed)
{
    loadHtml("<div style='display:grid; grid-template-columns:minmax(100px, 1fr) 100px; grid-template-rows:50px; width:400px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 300, 2); // flexible takes 400-100
    EXPECT_NEAR(b->width(), 100, 2);
}

TEST_F(GridLayoutTest, MinMaxZeroFrEqualSplit)
{
    loadHtml("<div style='display:grid; grid-template-columns:minmax(0, 1fr) minmax(0, 1fr); grid-template-rows:50px; width:300px;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(a && b);
    EXPECT_NEAR(a->width(), 150, 2);
    EXPECT_NEAR(b->width(), 150, 2);
}

TEST_F(GridLayoutTest, JustifyItemsEnd)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px; grid-template-rows:50px; width:100px; justify-items:end;'>"
             "<div id='a' style='width:40px; height:20px;'>A</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && a);
    EXPECT_NEAR(a->width(), 40, 2); // not stretched
    EXPECT_NEAR(a->xPos() - g->xPos(), 60, 2); // right-aligned in 100px cell
}

TEST_F(GridLayoutTest, JustifyItemsCenter)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px; grid-template-rows:50px; width:100px; justify-items:center;'>"
             "<div id='a' style='width:40px; height:20px;'>A</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && a);
    EXPECT_NEAR(a->xPos() - g->xPos(), 30, 2); // (100-40)/2
}

TEST_F(GridLayoutTest, AlignItemsCenterGrid)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px; grid-template-rows:80px; width:100px; align-items:center;'>"
             "<div id='a' style='width:40px; height:20px;'>A</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && a);
    EXPECT_NEAR(a->yPos() - g->yPos(), 30, 2); // (80-20)/2
}

TEST_F(GridLayoutTest, JustifySelfOverridesItems)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px; grid-template-rows:50px; width:100px; justify-items:start;'>"
             "<div id='a' style='width:40px; height:20px; justify-self:end;'>A</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && a);
    EXPECT_NEAR(a->xPos() - g->xPos(), 60, 2); // self:end overrides items:start
}

TEST_F(GridLayoutTest, JustifyContentCenterGrid)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:50px 50px; grid-template-rows:40px; width:200px; justify-content:center;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && a);
    EXPECT_NEAR(a->xPos() - g->xPos(), 50, 2); // (200-100)/2 offset
}

TEST_F(GridLayoutTest, JustifyContentSpaceBetweenGrid)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:50px 50px; grid-template-rows:40px; width:200px; justify-content:space-between;'>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* a = renderer("a");
    RenderObject* b = renderer("b");
    ASSERT_TRUE(g && a && b);
    EXPECT_NEAR(a->xPos() - g->xPos(), 0, 2);
    EXPECT_NEAR(b->xPos() - g->xPos(), 150, 2);
}

TEST_F(GridLayoutTest, AutoRowsImplicitSize)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 100px; grid-auto-rows:60px; width:200px;'>"
             "<div id='a'>A</div><div id='b'>B</div><div id='c'>C</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* c = renderer("c");
    ASSERT_TRUE(g && c);
    EXPECT_NEAR(c->yPos() - g->yPos(), 60, 2); // second (implicit) row
    EXPECT_NEAR(c->height(), 60, 2); // grid-auto-rows size
}

TEST_F(GridLayoutTest, AutoRowsAfterExplicit)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:50px 50px; grid-template-rows:40px; grid-auto-rows:70px; width:100px;'>"
             "<div id='a'>A</div><div id='b'>B</div><div id='c'>C</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* c = renderer("c");
    ASSERT_TRUE(g && c);
    EXPECT_NEAR(c->yPos() - g->yPos(), 40, 2); // after explicit 40px row
    EXPECT_NEAR(c->height(), 70, 2); // implicit row uses auto-rows
}

TEST_F(GridLayoutTest, AutoFlowColumn)
{
    loadHtml("<div id='g' style='display:grid; grid-template-rows:50px 50px; grid-template-columns:100px 100px; grid-auto-flow:column; width:200px;'>"
             "<div id='a'>A</div><div id='b'>B</div><div id='c'>C</div><div id='d'>D</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* b = renderer("b");
    RenderObject* c = renderer("c");
    ASSERT_TRUE(g && b && c);
    // Column flow: A(0,0) B(row1,col0) C(row0,col1) D(row1,col1)
    EXPECT_NEAR(b->yPos() - g->yPos(), 50, 2); // B below A
    EXPECT_NEAR(b->xPos() - g->xPos(), 0, 2);
    EXPECT_NEAR(c->xPos() - g->xPos(), 100, 2); // C in second column
    EXPECT_NEAR(c->yPos() - g->yPos(), 0, 2);
}

TEST_F(GridLayoutTest, AutoFlowDense)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:50px 50px 50px; grid-template-rows:40px 40px; grid-auto-flow:dense; width:150px;'>"
             "<div class='wide' id='w' style='grid-column:1 / 3;'>W</div>"
             "<div id='a'>A</div><div id='b'>B</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* w = renderer("w");
    RenderObject* a = renderer("a");
    ASSERT_TRUE(g && w && a);
    EXPECT_NEAR(w->width(), 100, 2); // spans 2 columns
    EXPECT_NEAR(a->xPos() - g->xPos(), 100, 2); // A flows to column 3 of row 1
    EXPECT_NEAR(a->yPos() - g->yPos(), 0, 2);
}

TEST_F(GridLayoutTest, TemplateAreasLayout)
{
    loadHtml("<div id='g' style='display:grid; "
             "grid-template-columns:100px 200px; grid-template-rows:50px 100px 40px; "
             "grid-template-areas:\"header header\" \"sidebar main\" \"footer footer\"; width:300px;'>"
             "<div id='hd' style='grid-area:header;'>H</div>"
             "<div id='sb' style='grid-area:sidebar;'>S</div>"
             "<div id='mn' style='grid-area:main;'>M</div>"
             "<div id='ft' style='grid-area:footer;'>F</div></div>");
    RenderObject* g = renderer("g");
    RenderObject* hd = renderer("hd");
    RenderObject* sb = renderer("sb");
    RenderObject* mn = renderer("mn");
    RenderObject* ft = renderer("ft");
    ASSERT_TRUE(g && hd && sb && mn && ft);
    // header spans both columns of row 0
    EXPECT_NEAR(hd->width(), 300, 2);
    EXPECT_NEAR(hd->yPos() - g->yPos(), 0, 2);
    // sidebar: col0 row1
    EXPECT_NEAR(sb->xPos() - g->xPos(), 0, 2);
    EXPECT_NEAR(sb->yPos() - g->yPos(), 50, 2);
    EXPECT_NEAR(sb->width(), 100, 2);
    // main: col1 row1
    EXPECT_NEAR(mn->xPos() - g->xPos(), 100, 2);
    EXPECT_NEAR(mn->width(), 200, 2);
    // footer spans both columns of row 2
    EXPECT_NEAR(ft->width(), 300, 2);
    EXPECT_NEAR(ft->yPos() - g->yPos(), 150, 2);
}

#endif // ENABLE(MODERN_GRID)
