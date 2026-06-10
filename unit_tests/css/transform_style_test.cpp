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
#include "AffineTransform.h"

#if ENABLE(MODERN_CSS3)

class TransformStyleTest : public ::testing::Test
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

TEST_F(TransformStyleTest, TranslateParsed)
{
    loadHtml("<div id='t' style='transform:translate(40px, 25px);'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    ASSERT_TRUE(s->hasTransform());
    ASSERT_EQ(s->transformOperations().size(), 1u);
    const TransformOperation& op = s->transformOperations()[0];
    EXPECT_EQ(op.type, TransformOperation::TranslateOp);
    EXPECT_FLOAT_EQ(op.x, 40.0f);
    EXPECT_FLOAT_EQ(op.y, 25.0f);
}

TEST_F(TransformStyleTest, ScaleParsed)
{
    loadHtml("<div id='t' style='transform:scale(2, 3);'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->transformOperations().size(), 1u);
    const TransformOperation& op = s->transformOperations()[0];
    EXPECT_EQ(op.type, TransformOperation::ScaleOp);
    EXPECT_FLOAT_EQ(op.x, 2.0f);
    EXPECT_FLOAT_EQ(op.y, 3.0f);
}

TEST_F(TransformStyleTest, RotateParsed)
{
    loadHtml("<div id='t' style='transform:rotate(90deg);'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->transformOperations().size(), 1u);
    EXPECT_EQ(s->transformOperations()[0].type, TransformOperation::RotateOp);
    EXPECT_FLOAT_EQ(s->transformOperations()[0].angleX, 90.0f);
}

TEST_F(TransformStyleTest, MultipleOperationsOrdered)
{
    loadHtml("<div id='t' style='transform:translate(10px,10px) rotate(45deg) scale(2);'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    ASSERT_EQ(s->transformOperations().size(), 3u);
    EXPECT_EQ(s->transformOperations()[0].type, TransformOperation::TranslateOp);
    EXPECT_EQ(s->transformOperations()[1].type, TransformOperation::RotateOp);
    EXPECT_EQ(s->transformOperations()[2].type, TransformOperation::ScaleOp);
    // scale(2) is uniform.
    EXPECT_FLOAT_EQ(s->transformOperations()[2].x, 2.0f);
    EXPECT_FLOAT_EQ(s->transformOperations()[2].y, 2.0f);
}

TEST_F(TransformStyleTest, NoneClearsTransform)
{
    loadHtml("<div id='t' style='transform:none;'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    EXPECT_FALSE(s->hasTransform());
}

// Geometry: a translate maps a point by the given offset.
TEST_F(TransformStyleTest, ApplyTransformTranslateGeometry)
{
    loadHtml("<div id='t' style='transform:translate(30px, 20px); transform-origin:0 0;'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    AffineTransform m;
    s->applyTransform(m, 100, 100);
    IntPoint p = m.mapPoint(IntPoint(0, 0));
    EXPECT_EQ(p.x(), 30);
    EXPECT_EQ(p.y(), 20);
}

// Geometry: scale about origin (0,0) doubles coordinates.
TEST_F(TransformStyleTest, ApplyTransformScaleGeometry)
{
    loadHtml("<div id='t' style='transform:scale(2); transform-origin:0 0;'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    AffineTransform m;
    s->applyTransform(m, 100, 100);
    IntPoint p = m.mapPoint(IntPoint(10, 10));
    EXPECT_EQ(p.x(), 20);
    EXPECT_EQ(p.y(), 20);
}

// Geometry: scale about the center leaves the center fixed.
TEST_F(TransformStyleTest, ApplyTransformScaleAboutCenter)
{
    loadHtml("<div id='t' style='transform:scale(2); transform-origin:50px 50px;'>X</div>");
    RenderStyle* s = styleById("t");
    ASSERT_TRUE(s);
    AffineTransform m;
    s->applyTransform(m, 100, 100);
    IntPoint c = m.mapPoint(IntPoint(50, 50));
    EXPECT_EQ(c.x(), 50);
    EXPECT_EQ(c.y(), 50);
    // A corner moves outward from the center.
    IntPoint corner = m.mapPoint(IntPoint(0, 0));
    EXPECT_EQ(corner.x(), -50);
    EXPECT_EQ(corner.y(), -50);
}

#endif // ENABLE(MODERN_CSS3)
