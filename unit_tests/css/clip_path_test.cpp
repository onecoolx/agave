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

// Verifies CSS clip-path parsing and storage in RenderStyle.

class ClipPathTest : public ::testing::Test
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

    const ClipPathOperation* clipById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        if (!el || !el->renderer()) {
            return 0;
        }
        return &el->renderer()->style()->clipPath();
    }

    TestWebView* view;
};

TEST_F(ClipPathTest, InsetShape)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:inset(10px 20px 30px 40px);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::InsetShape);
    EXPECT_FLOAT_EQ(c->inset[0].value, 10); // top
    EXPECT_FLOAT_EQ(c->inset[1].value, 20); // right
    EXPECT_FLOAT_EQ(c->inset[2].value, 30); // bottom
    EXPECT_FLOAT_EQ(c->inset[3].value, 40); // left
}

TEST_F(ClipPathTest, InsetShorthandExpansion)
{
    // inset(10px 20px) -> top=bottom=10, right=left=20
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:inset(10px 20px);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_FLOAT_EQ(c->inset[0].value, 10);
    EXPECT_FLOAT_EQ(c->inset[1].value, 20);
    EXPECT_FLOAT_EQ(c->inset[2].value, 10);
    EXPECT_FLOAT_EQ(c->inset[3].value, 20);
}

TEST_F(ClipPathTest, CircleShape)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:circle(40px at 30% 60%);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::CircleShape);
    EXPECT_FLOAT_EQ(c->rx.value, 40);
    EXPECT_FLOAT_EQ(c->ry.value, 40); // circle: rx == ry
    EXPECT_TRUE(c->cx.isPercent);
    EXPECT_FLOAT_EQ(c->cx.value, 30);
    EXPECT_FLOAT_EQ(c->cy.value, 60);
}

TEST_F(ClipPathTest, CircleDefaultCenter)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:circle(25px);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::CircleShape);
    // Default center is 50% 50%.
    EXPECT_TRUE(c->cx.isPercent);
    EXPECT_FLOAT_EQ(c->cx.value, 50);
    EXPECT_FLOAT_EQ(c->cy.value, 50);
}

TEST_F(ClipPathTest, EllipseShape)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:ellipse(40px 30px);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::EllipseShape);
    EXPECT_FLOAT_EQ(c->rx.value, 40);
    EXPECT_FLOAT_EQ(c->ry.value, 30);
}

TEST_F(ClipPathTest, PolygonShape)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:polygon(50% 0%, 100% 100%, 0% 100%);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::PolygonShape);
    ASSERT_EQ(c->polygon.size(), (size_t)6); // 3 points
    EXPECT_FLOAT_EQ(c->polygon[0].value, 50);
    EXPECT_TRUE(c->polygon[0].isPercent);
    EXPECT_FLOAT_EQ(c->polygon[1].value, 0);
}

TEST_F(ClipPathTest, PolygonEvenOdd)
{
    loadHtml("<div id='a' style='width:100px;height:100px;"
             "clip-path:polygon(evenodd, 0% 0%, 100% 0%, 100% 100%);'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::PolygonShape);
    EXPECT_TRUE(c->windEvenOdd);
}

TEST_F(ClipPathTest, NoneClears)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:none;'>a</div>");
    const ClipPathOperation* c = clipById("a");
    ASSERT_TRUE(c != 0);
    EXPECT_EQ(c->type, ClipPathOperation::NoClip);
}

TEST_F(ClipPathTest, RequiresLayer)
{
    loadHtml("<div id='a' style='width:100px;height:100px;clip-path:circle(40px);'>a</div>");
    Element* el = view->mainframe()->document()->getElementById(String("a"));
    ASSERT_TRUE(el != 0);
    ASSERT_TRUE(el->renderer() != 0);
    EXPECT_TRUE(el->renderer()->requiresLayer());
}
