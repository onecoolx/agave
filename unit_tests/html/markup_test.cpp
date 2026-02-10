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

class MarkupTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        Test_Init();
    }

    static void TearDownTestSuite()
    {
        Test_Shutdown();
    }

    void SetUp() override
    {
        view = new TestWebView();
        loadTestData();
    }

    void TearDown() override
    {
        delete view;
    }

    Document* document() const
    {
        return view->mainframe()->document();
    }

    void loadTestData()
    {
        static const char* testHtml = R"(
        <!DOCTYPE html>
        <html>
        <head><meta charset="utf-8"><title>Markup Test</title></head>
        <body id="test-body">
          <div title="a&amp;b&lt;c&quot;d">Text with &amp; &lt; &gt;</div>
          <span style="color:red; font-weight:bold;">Styled span</span>
          <blockquote type="cite" class="gmail_quote">Mail blockquote</blockquote>
          <pre>Pre with
        newline</pre>
          <table><tr><td>Cell</td></tr></table>
          <ul><li>Item</li></ul>
          <p>Para1<br>Para2</p>
          <textarea>Tab\tSeparated</textarea>
          <!-- comment -->
        </body>
        </html>
        )";

        view->loadHtml(testHtml, "http://localhost/test");

        TestWebView::waitForDocumentComplete(view);
    }

    TestWebView* view;
};

TEST_F(MarkupTest, CreateFragmentFromMarkupEscapingAndBaseURL)
{
    String markup = "<div title=\"a&amp;b&lt;c&quot;d\">x</div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "http://base.test/");
    ASSERT_TRUE(frag);
    EXPECT_EQ(frag->childNodes()->length(), 1u);
    Element* div = (Element*)frag->firstChild();
    EXPECT_EQ(div->getAttribute("title"), "a&b<c\"d");
}

TEST_F(MarkupTest, CreateFragmentFromTextTabsAndNewlines)
{
    RefPtr<Range> range = new Range(document(), document()->body(), 0, document()->body(), 0);
    String text = "a\tb\nc";
    RefPtr<DocumentFragment> frag = createFragmentFromText(range.get(), text);
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0u);
}

TEST_F(MarkupTest, CreateFragmentFromNodesWrapInParagraphs)
{
    Vector<Node*> nodes;
    nodes.append(document()->createTextNode("Hello").releaseRef());
    RefPtr<DocumentFragment> frag = createFragmentFromNodes(document(), nodes);
    ASSERT_TRUE(frag);
    EXPECT_EQ(frag->childNodes()->length(), nodes.size());
    unrefAllValues(nodes);
}

TEST_F(MarkupTest, CreateMarkupNodeChildrenOnly)
{
    ExceptionCode e;
    RefPtr<Element> div = document()->createElement("div", e);
    div->setAttribute(HTMLNames::idAttr, "t");
    div->appendChild(document()->createTextNode("text"), e);
    String out = createMarkup(div.get(), ChildrenOnly);
    EXPECT_FALSE(out.contains("<div"));
    EXPECT_TRUE(out.contains("text"));
}

// Test creating markup with attributes
TEST_F(MarkupTest, CreateMarkupWithAttributes)
{
    ExceptionCode e = 0;
    RefPtr<Element> div = document()->createElement("div", e);
    div->setAttribute(HTMLNames::idAttr, "test-id");
    div->setAttribute(HTMLNames::classAttr, "test-class");
    div->appendChild(document()->createTextNode("content"), e);

    String markup = createMarkup(div.get());
    EXPECT_TRUE(markup.contains("id=\"test-id\"") || markup.contains("id='test-id'"));
    EXPECT_TRUE(markup.contains("class=\"test-class\"") || markup.contains("class='test-class'"));
    EXPECT_TRUE(markup.contains("content"));
}

// Test creating markup from nested elements
TEST_F(MarkupTest, CreateMarkupNestedElements)
{
    ExceptionCode e;
    RefPtr<Element> outer = document()->createElement("div", e);
    RefPtr<Element> inner = document()->createElement("span", e);
    inner->appendChild(document()->createTextNode("inner text"), e);
    outer->appendChild(inner, e);

    String markup = createMarkup(outer.get());
    EXPECT_TRUE(markup.contains("<div"));
    EXPECT_TRUE(markup.contains("<span"));
    EXPECT_TRUE(markup.contains("inner text"));
    EXPECT_TRUE(markup.contains("</span>"));
    EXPECT_TRUE(markup.contains("</div>"));
}

// Test creating fragment from complex markup
TEST_F(MarkupTest, CreateFragmentFromComplexMarkup)
{
    String markup = "<div><p>Paragraph 1</p><p>Paragraph 2</p></div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}

// Test creating fragment with special characters
TEST_F(MarkupTest, CreateFragmentWithSpecialCharacters)
{
    String markup = "<div>&lt;&gt;&amp;&quot;&#39;</div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);

    Element* div = (Element*)frag->firstChild();
    ASSERT_NE(div, nullptr);
    String text = div->textContent();
    EXPECT_TRUE(text.contains("<"));
    EXPECT_TRUE(text.contains(">"));
    EXPECT_TRUE(text.contains("&"));
}

// Test creating fragment from text with newlines
TEST_F(MarkupTest, CreateFragmentFromTextWithNewlines)
{
    RefPtr<Range> range = new Range(document(), document()->body(), 0, document()->body(), 0);
    String text = "Line 1\nLine 2\nLine 3";
    RefPtr<DocumentFragment> frag = createFragmentFromText(range.get(), text);
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}

// Test creating fragment from empty markup
TEST_F(MarkupTest, CreateFragmentFromEmptyMarkup)
{
    String markup = "";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_EQ(frag->childNodes()->length(), 0);
}

// Test creating fragment from text-only content
TEST_F(MarkupTest, CreateFragmentFromTextOnly)
{
    String markup = "Plain text without tags";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}

// Test creating markup with style attribute
TEST_F(MarkupTest, CreateMarkupWithStyle)
{
    ExceptionCode e = 0;
    RefPtr<Element> div = document()->createElement("div", e);
    div->setAttribute(HTMLNames::styleAttr, "color: red; font-size: 14px;");
    div->appendChild(document()->createTextNode("styled"), e);

    String markup = createMarkup(div.get());
    EXPECT_TRUE(markup.contains("style"));
    EXPECT_TRUE(markup.contains("styled"));
}

// Test creating fragment with self-closing tags
TEST_F(MarkupTest, CreateFragmentWithSelfClosingTags)
{
    String markup = "<div>Text<br/>More text<hr/></div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}

// Test creating fragment with comments
TEST_F(MarkupTest, CreateFragmentWithComments)
{
    String markup = "<div><!-- This is a comment -->Text</div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}

// Test creating markup from list elements
TEST_F(MarkupTest, CreateMarkupFromList)
{
    ExceptionCode e;
    RefPtr<Element> ul = document()->createElement("ul", e);
    RefPtr<Element> li1 = document()->createElement("li", e);
    li1->appendChild(document()->createTextNode("Item 1"), e);
    RefPtr<Element> li2 = document()->createElement("li", e);
    li2->appendChild(document()->createTextNode("Item 2"), e);
    ul->appendChild(li1, e);
    ul->appendChild(li2, e);

    String markup = createMarkup(ul.get());
    EXPECT_TRUE(markup.contains("<ul"));
    EXPECT_TRUE(markup.contains("<li"));
    EXPECT_TRUE(markup.contains("Item 1"));
    EXPECT_TRUE(markup.contains("Item 2"));
}

// Test creating markup from table elements
TEST_F(MarkupTest, CreateMarkupFromTable)
{
    ExceptionCode e;
    RefPtr<Element> table = document()->createElement("table", e);
    RefPtr<Element> tr = document()->createElement("tr", e);
    RefPtr<Element> td = document()->createElement("td", e);
    td->appendChild(document()->createTextNode("Cell"), e);
    tr->appendChild(td, e);
    table->appendChild(tr, e);

    String markup = createMarkup(table.get());
    EXPECT_TRUE(markup.contains("<table"));
    EXPECT_TRUE(markup.contains("<tr"));
    EXPECT_TRUE(markup.contains("<td"));
    EXPECT_TRUE(markup.contains("Cell"));
}

// Test creating fragment with inline styles
TEST_F(MarkupTest, CreateFragmentWithInlineStyles)
{
    String markup = "<span style=\"font-weight: bold;\">Bold text</span>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);

    Element* span = (Element*)frag->firstChild();
    ASSERT_NE(span, nullptr);
    String styleAttr = span->getAttribute(HTMLNames::styleAttr);
    EXPECT_FALSE(styleAttr.isEmpty());
}

// Test creating markup preserves whitespace in pre
TEST_F(MarkupTest, CreateMarkupPreservesWhitespace)
{
    ExceptionCode e;
    RefPtr<Element> pre = document()->createElement("pre", e);
    pre->appendChild(document()->createTextNode("  Indented\n  Text  "), e);

    String markup = createMarkup(pre.get());
    EXPECT_TRUE(markup.contains("<pre"));
    // Whitespace should be preserved
    EXPECT_TRUE(markup.contains("Indented"));
}

// Test creating fragment from multiple root elements
TEST_F(MarkupTest, CreateFragmentMultipleRoots)
{
    String markup = "<div>First</div><div>Second</div><div>Third</div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GE(frag->childNodes()->length(), 3);
}

// Test creating markup with data attributes
TEST_F(MarkupTest, CreateMarkupWithDataAttributes)
{
    ExceptionCode e = 0;
    RefPtr<Element> div = document()->createElement("div", e);
    div->setAttribute(String("data-id"), String("123"), e);
    div->setAttribute(String("data-name"), String("test"), e);
    div->appendChild(document()->createTextNode("content"), e);

    String markup = createMarkup(div.get());
    EXPECT_TRUE(markup.contains("data-id"));
    EXPECT_TRUE(markup.contains("data-name"));
}

// Test creating fragment with boolean attributes
TEST_F(MarkupTest, CreateFragmentWithBooleanAttributes)
{
    String markup = "<input type=\"checkbox\" checked disabled readonly>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "");
    ASSERT_TRUE(frag);
    EXPECT_GT(frag->childNodes()->length(), 0);
}
