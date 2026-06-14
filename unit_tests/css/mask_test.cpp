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

// Verifies CSS mask-image (gradient) parsing and storage in RenderStyle.

class MaskTest : public ::testing::Test
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

TEST_F(MaskTest, LinearGradientMaskStored)
{
    loadHtml("<div id='a' style='width:100px;height:100px;"
             "mask-image:linear-gradient(to right, black, transparent);'>a</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    ASSERT_TRUE(s->hasMask());
    StyleGradient* g = s->maskGradient();
    ASSERT_TRUE(g != 0);
    EXPECT_EQ(g->type, StyleGradient::Linear);
    EXPECT_GE(g->stops.size(), (size_t)2);
}

TEST_F(MaskTest, MaskGradientStops)
{
    loadHtml("<div id='a' style='width:100px;height:100px;"
             "mask-image:linear-gradient(to bottom, black 0%, transparent 100%);'>a</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    StyleGradient* g = s->maskGradient();
    ASSERT_TRUE(g != 0);
    ASSERT_EQ(g->stops.size(), (size_t)2);
}

TEST_F(MaskTest, NoneClearsMask)
{
    loadHtml("<div id='a' style='width:100px;height:100px;mask-image:none;'>a</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    EXPECT_FALSE(s->hasMask());
}

TEST_F(MaskTest, NoMaskByDefault)
{
    loadHtml("<div id='a' style='width:100px;height:100px;'>a</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s != 0);
    EXPECT_FALSE(s->hasMask());
}

TEST_F(MaskTest, MaskRequiresLayer)
{
    loadHtml("<div id='a' style='width:100px;height:100px;"
             "mask-image:linear-gradient(black, transparent);'>a</div>");
    Element* el = view->mainframe()->document()->getElementById(String("a"));
    ASSERT_TRUE(el != 0);
    ASSERT_TRUE(el->renderer() != 0);
    EXPECT_TRUE(el->renderer()->requiresLayer());
}
