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
#include "Length.h"

using namespace WebCore;

// Length-level calc() evaluation (independent of layout).
TEST(CalcLengthTest, LinearEvaluation)
{
    // 100% - 40px against a 400px reference => 360.
    Length a = Length::makeCalculated(CalcExpression(100.0f, -40.0f));
    EXPECT_TRUE(a.isCalculated());
    EXPECT_EQ(a.calcValue(400), 360);

    // 50% + 10px against 400 => 210.
    Length b = Length::makeCalculated(CalcExpression(50.0f, 10.0f));
    EXPECT_EQ(b.calcValue(400), 210);

    // Pure pixels (0%) is reference-independent.
    Length c = Length::makeCalculated(CalcExpression(0.0f, 150.0f));
    EXPECT_EQ(c.calcValue(400), 150);
    EXPECT_EQ(c.calcValue(800), 150);
}

TEST(CalcLengthTest, SideTableDeduplicates)
{
    Length a = Length::makeCalculated(CalcExpression(25.0f, 5.0f));
    Length b = Length::makeCalculated(CalcExpression(25.0f, 5.0f));
    // Identical expressions share the same side-table index.
    EXPECT_EQ(a, b);
}

class CalcLayoutTest : public ::testing::Test
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

    int widthById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        return (el && el->renderer()) ? el->renderer()->width() : -1;
    }

    TestWebView* view;
};

TEST_F(CalcLayoutTest, PercentMinusPixels)
{
    loadHtml("<div style='width:400px;'><div id='a' style='width:calc(100% - 40px);'>A</div></div>");
    EXPECT_EQ(widthById("a"), 360);
}

TEST_F(CalcLayoutTest, PercentPlusPixels)
{
    loadHtml("<div style='width:400px;'><div id='a' style='width:calc(50% + 10px);'>A</div></div>");
    EXPECT_EQ(widthById("a"), 210);
}

TEST_F(CalcLayoutTest, DivisionAndAddition)
{
    loadHtml("<div style='width:400px;'><div id='a' style='width:calc(200px / 2);'>A</div></div>");
    EXPECT_EQ(widthById("a"), 100);
}

TEST_F(CalcLayoutTest, PixelOnly)
{
    loadHtml("<div style='width:400px;'><div id='a' style='width:calc(100px + 50px);'>A</div></div>");
    EXPECT_EQ(widthById("a"), 150);
}

TEST_F(CalcLayoutTest, WebkitPrefixedCalc)
{
    loadHtml("<div style='width:400px;'><div id='a' style='width:-webkit-calc(100% - 100px);'>A</div></div>");
    EXPECT_EQ(widthById("a"), 300);
}

TEST_F(CalcLayoutTest, MalformedCalcIgnored)
{
    // Non-linear (percent * percent) is rejected; the width falls back to auto
    // (fills the 400px parent), and the page must not crash.
    loadHtml("<div style='width:400px;'><div id='a' style='width:calc(50% * 50%);'>A</div></div>");
    EXPECT_GE(widthById("a"), 0);
}
