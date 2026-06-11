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

// Verifies structural pseudo-class selectors by observing the width each
// matched rule applies. Base width is 10px; matched rules override it.

class StructuralSelectorTest : public ::testing::Test
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
        if (!el || !el->renderer()) {
            return -1;
        }
        return el->renderer()->width();
    }

    TestWebView* view;
};

TEST_F(StructuralSelectorTest, LastChild)
{
    loadHtml("<style>.b{width:10px;} .b:last-child{width:100px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div><div class='b' id='c'>C</div></div>");
    EXPECT_EQ(widthById("a"), 10);
    EXPECT_EQ(widthById("b"), 10);
    EXPECT_EQ(widthById("c"), 100);
}

TEST_F(StructuralSelectorTest, OnlyChild)
{
    loadHtml("<style>.b{width:10px;} .b:only-child{width:100px;}</style>"
             "<div><div class='b' id='solo'>S</div></div>"
             "<div><div class='b' id='m1'>1</div><div class='b' id='m2'>2</div></div>");
    EXPECT_EQ(widthById("solo"), 100);
    EXPECT_EQ(widthById("m1"), 10);
    EXPECT_EQ(widthById("m2"), 10);
}

TEST_F(StructuralSelectorTest, NthChildOddEven)
{
    loadHtml("<style>.b{width:10px;} .b:nth-child(odd){width:50px;} .b:nth-child(even){width:60px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div>"
             "<div class='b' id='c'>C</div><div class='b' id='d'>D</div></div>");
    EXPECT_EQ(widthById("a"), 50); // 1 odd
    EXPECT_EQ(widthById("b"), 60); // 2 even
    EXPECT_EQ(widthById("c"), 50); // 3 odd
    EXPECT_EQ(widthById("d"), 60); // 4 even
}

TEST_F(StructuralSelectorTest, NthChildInteger)
{
    loadHtml("<style>.b{width:10px;} .b:nth-child(2){width:200px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div><div class='b' id='c'>C</div></div>");
    EXPECT_EQ(widthById("a"), 10);
    EXPECT_EQ(widthById("b"), 200);
    EXPECT_EQ(widthById("c"), 10);
}

TEST_F(StructuralSelectorTest, NthChildAnPlusB)
{
    // 2n+1 selects positions 1, 3, 5, ...
    loadHtml("<style>.b{width:10px;} .b:nth-child(2n+1){width:70px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div>"
             "<div class='b' id='c'>C</div><div class='b' id='d'>D</div></div>");
    EXPECT_EQ(widthById("a"), 70); // 1
    EXPECT_EQ(widthById("b"), 10); // 2
    EXPECT_EQ(widthById("c"), 70); // 3
    EXPECT_EQ(widthById("d"), 10); // 4
}

TEST_F(StructuralSelectorTest, NthChildPlainN)
{
    // 3n selects positions 3, 6, ...
    loadHtml("<style>.b{width:10px;} .b:nth-child(3n){width:80px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div>"
             "<div class='b' id='c'>C</div><div class='b' id='d'>D</div>"
             "<div class='b' id='e'>E</div><div class='b' id='f'>F</div></div>");
    EXPECT_EQ(widthById("c"), 80); // 3
    EXPECT_EQ(widthById("f"), 80); // 6
    EXPECT_EQ(widthById("a"), 10);
    EXPECT_EQ(widthById("d"), 10);
}

TEST_F(StructuralSelectorTest, FirstChildStillWorks)
{
    // Regression: the pre-existing :first-child must keep working.
    loadHtml("<style>.b{width:10px;} .b:first-child{width:90px;}</style>"
             "<div><div class='b' id='a'>A</div><div class='b' id='b'>B</div></div>");
    EXPECT_EQ(widthById("a"), 90);
    EXPECT_EQ(widthById("b"), 10);
}
