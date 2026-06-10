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

class GridStyleTest : public ::testing::Test
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
        return el ? el->renderStyle() : 0;
    }

    TestWebView* view;
};

TEST_F(GridStyleTest, DisplayGridParsed)
{
    loadHtml("<div id='g' style='display:grid;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->display(), GRID);
}

TEST_F(GridStyleTest, TemplateColumnsFixed)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 200px 50px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 3u);
    EXPECT_EQ(cols[0].kind, GridTrackSize::FixedTrack);
    EXPECT_EQ(cols[0].length, 100);
    EXPECT_EQ(cols[1].length, 200);
    EXPECT_EQ(cols[2].length, 50);
}

TEST_F(GridStyleTest, TemplateColumnsFr)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:1fr 2fr;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 2u);
    EXPECT_EQ(cols[0].kind, GridTrackSize::FrTrack);
    EXPECT_FLOAT_EQ(cols[0].fr, 1.0f);
    EXPECT_EQ(cols[1].kind, GridTrackSize::FrTrack);
    EXPECT_FLOAT_EQ(cols[1].fr, 2.0f);
}

TEST_F(GridStyleTest, TemplateColumnsMixed)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:100px 1fr auto;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 3u);
    EXPECT_EQ(cols[0].kind, GridTrackSize::FixedTrack);
    EXPECT_EQ(cols[1].kind, GridTrackSize::FrTrack);
    EXPECT_EQ(cols[2].kind, GridTrackSize::AutoTrack);
}

TEST_F(GridStyleTest, TemplateColumnsRepeat)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:repeat(3, 100px);'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 3u);
    EXPECT_EQ(cols[0].length, 100);
    EXPECT_EQ(cols[1].length, 100);
    EXPECT_EQ(cols[2].length, 100);
}

TEST_F(GridStyleTest, TemplatePercentTracks)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:25% 75%;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 2u);
    EXPECT_EQ(cols[0].kind, GridTrackSize::PercentTrack);
    EXPECT_EQ(cols[0].length, 25);
    EXPECT_EQ(cols[1].length, 75);
}

TEST_F(GridStyleTest, TemplateRows)
{
    loadHtml("<div id='g' style='display:grid; grid-template-rows:50px 1fr;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& rows = s->gridTemplateRows();
    ASSERT_EQ(rows.size(), 2u);
    EXPECT_EQ(rows[0].kind, GridTrackSize::FixedTrack);
    EXPECT_EQ(rows[1].kind, GridTrackSize::FrTrack);
}

TEST_F(GridStyleTest, GridColumnLine)
{
    loadHtml("<div style='display:grid;'><div id='i' style='grid-column-start:2; grid-column-end:4;'>A</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_FALSE(s->gridColumnStart().isAuto);
    EXPECT_EQ(s->gridColumnStart().line, 2);
    EXPECT_FALSE(s->gridColumnEnd().isAuto);
    EXPECT_EQ(s->gridColumnEnd().line, 4);
}

TEST_F(GridStyleTest, GridColumnSpan)
{
    loadHtml("<div style='display:grid;'><div id='i' style='grid-column-end:span 2;'>A</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_TRUE(s->gridColumnEnd().isSpan);
    EXPECT_EQ(s->gridColumnEnd().line, 2);
}

TEST_F(GridStyleTest, GridColumnShorthand)
{
    loadHtml("<div style='display:grid;'><div id='i' style='grid-column:1 / 3;'>A</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridColumnStart().line, 1);
    EXPECT_EQ(s->gridColumnEnd().line, 3);
}

TEST_F(GridStyleTest, GridRowShorthand)
{
    loadHtml("<div style='display:grid;'><div id='i' style='grid-row:2 / 4;'>A</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridRowStart().line, 2);
    EXPECT_EQ(s->gridRowEnd().line, 4);
}

TEST_F(GridStyleTest, GapShorthandSingle)
{
    loadHtml("<div id='g' style='display:grid; gap:10px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridRowGap(), 10);
    EXPECT_EQ(s->gridColumnGap(), 10);
}

TEST_F(GridStyleTest, GapShorthandTwo)
{
    loadHtml("<div id='g' style='display:grid; gap:10px 20px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridRowGap(), 10);
    EXPECT_EQ(s->gridColumnGap(), 20);
}

TEST_F(GridStyleTest, RowColumnGapSeparate)
{
    loadHtml("<div id='g' style='display:grid; row-gap:5px; column-gap:15px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridRowGap(), 5);
    EXPECT_EQ(s->gridColumnGap(), 15);
}

TEST_F(GridStyleTest, MinMaxTrackParsed)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:minmax(100px, 1fr) 200px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 2u);
    EXPECT_TRUE(cols[0].isMinMax);
    EXPECT_EQ(cols[0].kind, GridTrackSize::FixedTrack); // min component
    EXPECT_EQ(cols[0].length, 100);
    EXPECT_EQ(cols[0].maxKind, GridTrackSize::FrTrack); // max component
    EXPECT_FLOAT_EQ(cols[0].maxFr, 1.0f);
    EXPECT_FALSE(cols[1].isMinMax);
    EXPECT_EQ(cols[1].kind, GridTrackSize::FixedTrack);
}

TEST_F(GridStyleTest, MinContentMaxContentParsed)
{
    loadHtml("<div id='g' style='display:grid; grid-template-columns:min-content max-content;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridTrackSize>& cols = s->gridTemplateColumns();
    ASSERT_EQ(cols.size(), 2u);
    EXPECT_EQ(cols[0].kind, GridTrackSize::MinContentTrack);
    EXPECT_EQ(cols[1].kind, GridTrackSize::MaxContentTrack);
}

TEST_F(GridStyleTest, JustifyItemsParsed)
{
    loadHtml("<div id='g' style='display:grid; justify-items:center;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridJustifyItems(), GridAlignCenter);
}

TEST_F(GridStyleTest, JustifySelfParsed)
{
    loadHtml("<div style='display:grid;'><div id='i' style='justify-self:end;'>A</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridJustifySelf(), (int)GridAlignEnd);
}

TEST_F(GridStyleTest, AlignItemsGridParsed)
{
    loadHtml("<div id='g' style='display:grid; align-items:start;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridAlignItems(), GridAlignStart);
}

TEST_F(GridStyleTest, AutoRowsParsed)
{
    loadHtml("<div id='g' style='display:grid; grid-auto-rows:80px;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridAutoRows().kind, GridTrackSize::FixedTrack);
    EXPECT_EQ(s->gridAutoRows().length, 80);
}

TEST_F(GridStyleTest, AutoFlowColumnParsed)
{
    loadHtml("<div id='g' style='display:grid; grid-auto-flow:column;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridAutoFlow(), GridAutoFlowColumn);
    EXPECT_FALSE(s->gridAutoFlowDense());
}

TEST_F(GridStyleTest, AutoFlowColumnDenseParsed)
{
    loadHtml("<div id='g' style='display:grid; grid-auto-flow:column dense;'><div>A</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridAutoFlow(), GridAutoFlowColumn);
    EXPECT_TRUE(s->gridAutoFlowDense());
}

TEST_F(GridStyleTest, TemplateAreasParsed)
{
    loadHtml("<div id='g' style='display:grid; "
             "grid-template-areas:\"a a b\" \"a a c\";'><div>X</div></div>");
    RenderStyle* s = styleById("g");
    ASSERT_TRUE(s);
    const Vector<GridNamedArea>& areas = s->gridTemplateAreas();
    ASSERT_EQ(areas.size(), 3u); // a, b, c
    // 'a' spans cols 0-2, rows 0-2
    for (size_t i = 0; i < areas.size(); i++) {
        if (areas[i].name == "a") {
            EXPECT_EQ(areas[i].colStart, 0);
            EXPECT_EQ(areas[i].colEnd, 2);
            EXPECT_EQ(areas[i].rowStart, 0);
            EXPECT_EQ(areas[i].rowEnd, 2);
        }
    }
}

TEST_F(GridStyleTest, GridAreaNameParsed)
{
    loadHtml("<div style='display:grid;'><div id='i' style='grid-area:main;'>M</div></div>");
    RenderStyle* s = styleById("i");
    ASSERT_TRUE(s);
    EXPECT_EQ(s->gridArea(), String("main"));
}

#endif // ENABLE(MODERN_GRID)
