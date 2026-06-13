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

using namespace WebCore;

// Stage B: custom properties (--name) are stored on the computed style and
// inherit by default to descendants.
class CustomPropertyStyleTest : public ::testing::Test
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

TEST_F(CustomPropertyStyleTest, StoredOnDeclaringElement)
{
    loadHtml("<div id='a' style='--brand: blue; --gap: 10px;'>A</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s);
    bool found = false;
    EXPECT_EQ(s->customProperty("--brand", found), String("blue"));
    EXPECT_TRUE(found);
    found = false;
    EXPECT_EQ(s->customProperty("--gap", found), String("10px"));
    EXPECT_TRUE(found);
}

TEST_F(CustomPropertyStyleTest, InheritsToDescendant)
{
    loadHtml("<div id='p' style='--brand: green;'><div id='c'>C</div></div>");
    RenderStyle* c = styleById("c");
    ASSERT_TRUE(c);
    bool found = false;
    // Inherited by default from the parent.
    EXPECT_EQ(c->customProperty("--brand", found), String("green"));
    EXPECT_TRUE(found);
}

TEST_F(CustomPropertyStyleTest, UnknownPropertyNotFound)
{
    loadHtml("<div id='a' style='--x: 1;'>A</div>");
    RenderStyle* s = styleById("a");
    ASSERT_TRUE(s);
    bool found = true;
    s->customProperty("--missing", found);
    EXPECT_FALSE(found);
}

TEST_F(CustomPropertyStyleTest, ChildOverridesInherited)
{
    loadHtml("<div id='p' style='--c: red;'><div id='c' style='--c: blue;'>C</div></div>");
    RenderStyle* c = styleById("c");
    ASSERT_TRUE(c);
    bool found = false;
    // The child's own declaration wins over the inherited value.
    EXPECT_EQ(c->customProperty("--c", found), String("blue"));
    EXPECT_TRUE(found);
}
