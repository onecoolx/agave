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
#include "ClientRect.h"
#include "HTMLCollection.h"
#include "ExceptionCode.h"

using namespace WebCore;

class DomApiTest : public ::testing::Test
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

TEST_F(DomApiTest, BoundingClientRectGeometry)
{
    loadHtml("<div id='d' style='position:absolute;left:10px;top:20px;width:100px;height:50px;'>x</div>");
    Element* d = byId("d");
    ASSERT_TRUE(d);
    RefPtr<ClientRect> r = d->getBoundingClientRect();
    ASSERT_TRUE(r);
    EXPECT_EQ(r->left(), 10);
    EXPECT_EQ(r->top(), 20);
    EXPECT_EQ(r->width(), 100);
    EXPECT_EQ(r->height(), 50);
    EXPECT_EQ(r->right(), 110);
    EXPECT_EQ(r->bottom(), 70);
}

TEST_F(DomApiTest, BoundingClientRectReturnsObject)
{
    loadHtml("<div id='d' style='width:30px;height:40px;'>x</div>");
    Element* d = byId("d");
    ASSERT_TRUE(d);
    RefPtr<ClientRect> r = d->getBoundingClientRect();
    ASSERT_TRUE(r);
    EXPECT_EQ(r->width(), 30);
    EXPECT_EQ(r->height(), 40);
}

TEST_F(DomApiTest, ElementTraversalSkipsTextNodes)
{
    loadHtml("<ul id='l'>\n  <li id='a'>A</li>\n  <li id='b'>B</li>\n  <li id='c'>C</li>\n</ul>");
    Element* l = byId("l");
    ASSERT_TRUE(l);
    EXPECT_EQ(l->childElementCount(), 3u);
    ASSERT_TRUE(l->firstElementChild());
    EXPECT_EQ(l->firstElementChild()->getAttribute("id"), "a");
    ASSERT_TRUE(l->lastElementChild());
    EXPECT_EQ(l->lastElementChild()->getAttribute("id"), "c");

    Element* b = byId("b");
    ASSERT_TRUE(b);
    ASSERT_TRUE(b->nextElementSibling());
    EXPECT_EQ(b->nextElementSibling()->getAttribute("id"), "c");
    ASSERT_TRUE(b->previousElementSibling());
    EXPECT_EQ(b->previousElementSibling()->getAttribute("id"), "a");
}

TEST_F(DomApiTest, ChildrenCollectionElementOnly)
{
    loadHtml("<div id='p'>text<span>1</span>more<span>2</span></div>");
    Element* p = byId("p");
    ASSERT_TRUE(p);
    RefPtr<HTMLCollection> kids = p->children();
    ASSERT_TRUE(kids);
    EXPECT_EQ(kids->length(), 2u); // two spans, text nodes excluded
}

TEST_F(DomApiTest, MatchesSelf)
{
    loadHtml("<button id='btn' class='primary'>x</button>");
    Element* btn = byId("btn");
    ASSERT_TRUE(btn);
    ExceptionCode ec = 0;
    EXPECT_TRUE(btn->matches(".primary", ec));
    EXPECT_EQ(ec, 0);
    EXPECT_TRUE(btn->matches("button", ec));
    EXPECT_FALSE(btn->matches(".secondary", ec));
}

TEST_F(DomApiTest, MatchesEmptySelectorThrows)
{
    loadHtml("<div id='d'>x</div>");
    Element* d = byId("d");
    ASSERT_TRUE(d);
    ExceptionCode ec = 0;
    d->matches("", ec);
    EXPECT_NE(ec, 0);
}

TEST_F(DomApiTest, ClosestWalksAncestors)
{
    loadHtml("<div id='outer' class='container'><section id='mid'><button id='btn'>x</button></section></div>");
    Element* btn = byId("btn");
    ASSERT_TRUE(btn);
    ExceptionCode ec = 0;
    ASSERT_TRUE(btn->closest("section", ec));
    EXPECT_EQ(btn->closest("section", ec)->getAttribute("id"), "mid");
    ASSERT_TRUE(btn->closest(".container", ec));
    EXPECT_EQ(btn->closest(".container", ec)->getAttribute("id"), "outer");
    // closest matches the element itself.
    ASSERT_TRUE(btn->closest("button", ec));
    EXPECT_EQ(btn->closest("button", ec)->getAttribute("id"), "btn");
    // no ancestor matches.
    EXPECT_FALSE(btn->closest("table", ec));
}

TEST_F(DomApiTest, DatasetBackedByDataAttributes)
{
    // The dataset DOMStringMap is a JS-level exotic object; at the C++ layer we
    // verify the underlying data-* attribute storage it maps onto.
    loadHtml("<div id='d' data-role='panel' data-foo-bar='hello'></div>");
    Element* d = byId("d");
    ASSERT_TRUE(d);
    EXPECT_EQ(d->getAttribute("data-role"), "panel");
    EXPECT_EQ(d->getAttribute("data-foo-bar"), "hello");
    ExceptionCode ec = 0;
    d->setAttribute("data-new-prop", "v", ec);
    EXPECT_EQ(d->getAttribute("data-new-prop"), "v");
}
