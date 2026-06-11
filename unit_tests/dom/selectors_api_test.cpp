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
#include "Document.h"
#include "Element.h"
#include "NodeList.h"
#include "DOMTokenList.h"
#include "ExceptionCode.h"

using namespace WebCore;

class SelectorsApiTest : public ::testing::Test
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

    Document* doc() { return view->mainframe()->document(); }
    Element* byId(const char* id) { return doc()->getElementById(String(id)); }

    TestWebView* view;
};

TEST_F(SelectorsApiTest, QuerySelectorById)
{
    loadHtml("<div id='a' class='x'>A</div><div id='b' class='y'>B</div>");
    ExceptionCode ec = 0;
    Element* e = doc()->querySelector("#b", ec);
    ASSERT_TRUE(e);
    EXPECT_EQ(e->getAttribute("id"), "b");
    EXPECT_EQ(ec, 0);
}

TEST_F(SelectorsApiTest, QuerySelectorByClass)
{
    loadHtml("<div class='card'>1</div><div class='card special'>2</div>");
    ExceptionCode ec = 0;
    Element* e = doc()->querySelector(".special", ec);
    ASSERT_TRUE(e);
    EXPECT_EQ(e->getAttribute("class"), "card special");
}

TEST_F(SelectorsApiTest, QuerySelectorAllCount)
{
    loadHtml("<ul><li class='item'>1</li><li class='item'>2</li><li>3</li><li class='item'>4</li></ul>");
    ExceptionCode ec = 0;
    RefPtr<NodeList> list = doc()->querySelectorAll(".item", ec);
    ASSERT_TRUE(list);
    EXPECT_EQ(list->length(), 3u);
}

TEST_F(SelectorsApiTest, QuerySelectorScopedToElement)
{
    loadHtml("<div id='box'><p class='t'>in</p></div><p class='t'>out</p>");
    Element* box = byId("box");
    ASSERT_TRUE(box);
    ExceptionCode ec = 0;
    RefPtr<NodeList> list = box->querySelectorAll(".t", ec);
    ASSERT_TRUE(list);
    EXPECT_EQ(list->length(), 1u); // only the descendant of #box
}

TEST_F(SelectorsApiTest, QuerySelectorNoMatch)
{
    loadHtml("<div>A</div>");
    ExceptionCode ec = 0;
    Element* e = doc()->querySelector(".nope", ec);
    EXPECT_FALSE(e);
}

TEST_F(SelectorsApiTest, ClassListContainsAddRemoveToggle)
{
    loadHtml("<div id='a' class='one two'>A</div>");
    Element* a = byId("a");
    ASSERT_TRUE(a);
    DOMTokenList* cl = a->classList();
    ASSERT_TRUE(cl);
    EXPECT_EQ(cl->length(), 2u);
    EXPECT_TRUE(cl->contains("one"));
    EXPECT_TRUE(cl->contains("two"));
    EXPECT_FALSE(cl->contains("three"));

    ExceptionCode ec = 0;
    cl->add("three", ec);
    EXPECT_TRUE(cl->contains("three"));
    EXPECT_EQ(cl->length(), 3u);
    // className stays in sync with classList.
    EXPECT_TRUE(a->getAttribute("class").contains("three"));

    cl->remove("one", ec);
    EXPECT_FALSE(cl->contains("one"));
    EXPECT_EQ(cl->length(), 2u);

    bool present = cl->toggle("two", ec); // removes
    EXPECT_FALSE(present);
    EXPECT_FALSE(cl->contains("two"));
    present = cl->toggle("two", ec); // adds back
    EXPECT_TRUE(present);
    EXPECT_TRUE(cl->contains("two"));
}

TEST_F(SelectorsApiTest, ClassListAddDuplicateNoop)
{
    loadHtml("<div id='a' class='one'>A</div>");
    Element* a = byId("a");
    DOMTokenList* cl = a->classList();
    ExceptionCode ec = 0;
    cl->add("one", ec); // already present
    EXPECT_EQ(cl->length(), 1u);
}

TEST_F(SelectorsApiTest, ClassListEmptyTokenThrows)
{
    loadHtml("<div id='a' class='one'>A</div>");
    Element* a = byId("a");
    DOMTokenList* cl = a->classList();
    ExceptionCode ec = 0;
    cl->add("", ec);
    EXPECT_EQ(ec, SYNTAX_ERR);
}

