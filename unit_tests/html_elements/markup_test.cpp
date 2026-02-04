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
