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

// Verifies CSS mix-blend-mode parsing and storage in RenderStyle.

class MixBlendModeTest : public ::testing::Test
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

    EBlendMode blendModeById(const char* id)
    {
        Element* el = view->mainframe()->document()->getElementById(String(id));
        if (!el || !el->renderer()) {
            return BM_NORMAL;
        }
        return el->renderer()->style()->blendMode();
    }

    TestWebView* view;
};

TEST_F(MixBlendModeTest, ParsesKeywords)
{
    loadHtml("<div id='a' style='mix-blend-mode:multiply;'>a</div>"
             "<div id='b' style='mix-blend-mode:screen;'>b</div>"
             "<div id='c' style='mix-blend-mode:overlay;'>c</div>"
             "<div id='d' style='mix-blend-mode:darken;'>d</div>"
             "<div id='e' style='mix-blend-mode:color-dodge;'>e</div>"
             "<div id='f' style='mix-blend-mode:luminosity;'>f</div>"
             "<div id='g' style='mix-blend-mode:normal;'>g</div>"
             "<div id='h'>h</div>");
    EXPECT_EQ(blendModeById("a"), BM_MULTIPLY);
    EXPECT_EQ(blendModeById("b"), BM_SCREEN);
    EXPECT_EQ(blendModeById("c"), BM_OVERLAY);
    EXPECT_EQ(blendModeById("d"), BM_DARKEN);
    EXPECT_EQ(blendModeById("e"), BM_COLOR_DODGE);
    EXPECT_EQ(blendModeById("f"), BM_LUMINOSITY);
    EXPECT_EQ(blendModeById("g"), BM_NORMAL);
    // Default when unspecified is normal.
    EXPECT_EQ(blendModeById("h"), BM_NORMAL);
}

TEST_F(MixBlendModeTest, RejectsInvalidKeyword)
{
    loadHtml("<div id='a' style='mix-blend-mode:wobble;'>a</div>");
    // Invalid keyword leaves the initial value (normal).
    EXPECT_EQ(blendModeById("a"), BM_NORMAL);
}

TEST_F(MixBlendModeTest, AllSeparableAndNonSeparableModes)
{
    loadHtml("<div id='a' style='mix-blend-mode:color-burn;'>a</div>"
             "<div id='b' style='mix-blend-mode:hard-light;'>b</div>"
             "<div id='c' style='mix-blend-mode:soft-light;'>c</div>"
             "<div id='d' style='mix-blend-mode:difference;'>d</div>"
             "<div id='e' style='mix-blend-mode:exclusion;'>e</div>"
             "<div id='f' style='mix-blend-mode:hue;'>f</div>"
             "<div id='g' style='mix-blend-mode:saturation;'>g</div>"
             "<div id='h' style='mix-blend-mode:color;'>h</div>"
             "<div id='i' style='mix-blend-mode:lighten;'>i</div>");
    EXPECT_EQ(blendModeById("a"), BM_COLOR_BURN);
    EXPECT_EQ(blendModeById("b"), BM_HARD_LIGHT);
    EXPECT_EQ(blendModeById("c"), BM_SOFT_LIGHT);
    EXPECT_EQ(blendModeById("d"), BM_DIFFERENCE);
    EXPECT_EQ(blendModeById("e"), BM_EXCLUSION);
    EXPECT_EQ(blendModeById("f"), BM_HUE);
    EXPECT_EQ(blendModeById("g"), BM_SATURATION);
    EXPECT_EQ(blendModeById("h"), BM_COLOR);
    EXPECT_EQ(blendModeById("i"), BM_LIGHTEN);
}

TEST_F(MixBlendModeTest, BlendModeRequiresLayer)
{
    loadHtml("<div id='a' style='mix-blend-mode:multiply; width:50px; height:50px;'>a</div>");
    Element* el = view->mainframe()->document()->getElementById(String("a"));
    ASSERT_TRUE(el != 0);
    ASSERT_TRUE(el->renderer() != 0);
    // A blend mode forces a render layer so the compositing path runs.
    EXPECT_TRUE(el->renderer()->requiresLayer());
}
