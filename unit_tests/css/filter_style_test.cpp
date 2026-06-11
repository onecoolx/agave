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

class FilterStyleTest : public ::testing::Test
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

TEST_F(FilterStyleTest, BlurParsed)
{
    loadHtml("<div id='f' style='filter:blur(5px);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_TRUE(s->hasFilter());
    ASSERT_EQ(s->filterOperations().size(), 1u);
    EXPECT_EQ(s->filterOperations()[0].type, FilterOperation::BlurOp);
    EXPECT_FLOAT_EQ(s->filterOperations()[0].stdDeviation, 5.0f);
}

TEST_F(FilterStyleTest, OpacityParsed)
{
    loadHtml("<div id='f' style='filter:opacity(0.5);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->filterOperations().size(), 1u);
    EXPECT_EQ(s->filterOperations()[0].type, FilterOperation::OpacityOp);
    EXPECT_FLOAT_EQ(s->filterOperations()[0].amount, 0.5f);
}

TEST_F(FilterStyleTest, DropShadowParsed)
{
    loadHtml("<div id='f' style='filter:drop-shadow(4px 6px 2px black);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->filterOperations().size(), 1u);
    const FilterOperation& op = s->filterOperations()[0];
    EXPECT_EQ(op.type, FilterOperation::DropShadowOp);
    EXPECT_EQ(op.shadowX, 4);
    EXPECT_EQ(op.shadowY, 6);
    EXPECT_EQ(op.shadowBlur, 2);
}

TEST_F(FilterStyleTest, MultipleFiltersOrdered)
{
    loadHtml("<div id='f' style='filter:blur(2px) opacity(0.8);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->filterOperations().size(), 2u);
    EXPECT_EQ(s->filterOperations()[0].type, FilterOperation::BlurOp);
    EXPECT_EQ(s->filterOperations()[1].type, FilterOperation::OpacityOp);
}

TEST_F(FilterStyleTest, ColorMatrixParsedButTyped)
{
    // grayscale() is parsed (as ColorMatrixOp) even though not yet rendered.
    loadHtml("<div id='f' style='filter:grayscale(1);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->filterOperations().size(), 1u);
    EXPECT_EQ(s->filterOperations()[0].type, FilterOperation::ColorMatrixOp);
}

TEST_F(FilterStyleTest, NoneClearsFilter)
{
    loadHtml("<div id='f' style='filter:none;'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    EXPECT_FALSE(s->hasFilter());
}

TEST_F(FilterStyleTest, PrefixedFilterAlias)
{
    loadHtml("<div id='f' style='-webkit-filter:blur(3px);'>X</div>");
    RenderStyle* s = styleById("f");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->filterOperations().size(), 1u);
    EXPECT_EQ(s->filterOperations()[0].type, FilterOperation::BlurOp);
}

#endif // ENABLE(MODERN_CSS3)
