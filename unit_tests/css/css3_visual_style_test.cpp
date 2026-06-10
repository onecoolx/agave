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

// Verifies that the standard unprefixed CSS3 visual properties (border-radius,
// box-shadow, transform-origin) are recognised and applied via the existing
// -webkit- implementations.

class Css3VisualStyleTest : public ::testing::Test
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

TEST_F(Css3VisualStyleTest, UnprefixedBorderRadiusParsed)
{
    loadHtml("<div id='b' style='border-radius:10px;'>X</div>");
    RenderStyle* s = styleById("b");
    ASSERT_TRUE(s);
    EXPECT_TRUE(s->hasBorderRadius());
    EXPECT_EQ(s->borderTopLeftRadius().width(), 10);
    EXPECT_EQ(s->borderBottomRightRadius().width(), 10);
}

TEST_F(Css3VisualStyleTest, UnprefixedBorderCornerRadiusParsed)
{
    loadHtml("<div id='b' style='border-top-left-radius:8px;'>X</div>");
    RenderStyle* s = styleById("b");
    ASSERT_TRUE(s);
    EXPECT_TRUE(s->hasBorderRadius());
    EXPECT_EQ(s->borderTopLeftRadius().width(), 8);
    // Other corners remain square.
    EXPECT_EQ(s->borderBottomRightRadius().width(), 0);
}

TEST_F(Css3VisualStyleTest, UnprefixedBoxShadowParsed)
{
    loadHtml("<div id='b' style='box-shadow:2px 4px 6px black;'>X</div>");
    RenderStyle* s = styleById("b");
    ASSERT_TRUE(s);
    ShadowData* shadow = s->boxShadow();
    ASSERT_TRUE(shadow);
    EXPECT_EQ(shadow->x, 2);
    EXPECT_EQ(shadow->y, 4);
    EXPECT_EQ(shadow->blur, 6);
}

TEST_F(Css3VisualStyleTest, PrefixedBorderRadiusStillWorks)
{
    loadHtml("<div id='b' style='-webkit-border-radius:5px;'>X</div>");
    RenderStyle* s = styleById("b");
    ASSERT_TRUE(s);
    EXPECT_TRUE(s->hasBorderRadius());
    EXPECT_EQ(s->borderTopLeftRadius().width(), 5);
}

TEST_F(Css3VisualStyleTest, UnprefixedTransformOriginParsed)
{
    loadHtml("<div id='b' style='transform-origin:30px 40px;'>X</div>");
    RenderStyle* s = styleById("b");
    ASSERT_TRUE(s);
    // transform-origin maps to the -webkit- storage.
    EXPECT_EQ(s->transformOriginX().value(), 30);
    EXPECT_EQ(s->transformOriginY().value(), 40);
}
