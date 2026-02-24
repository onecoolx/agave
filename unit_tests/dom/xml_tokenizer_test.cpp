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

#if ENABLE(XML)

#include "XMLTokenizer.h"

class XMLTokenizerTest : public ::testing::Test
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
    }

    void TearDown() override
    {
        delete view;
    }

    Document* document() const
    {
        return view->mainframe()->document();
    }

    void loadAndWait(const char* html)
    {
        view->loadHtml(html, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    TestWebView* view;
};

// Verify the helper used by XMLTokenizer to parse a loose attribute string.
TEST_F(XMLTokenizerTest, ParseAttributesBasic)
{
    bool ok = false;
    HashMap<String, String> attrs = WebCore::parseAttributes("a=\"1\" b=\"two\"", ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(String("1"), attrs.get("a"));
    EXPECT_EQ(String("two"), attrs.get("b"));
}

// NOTE: libxml2 provides namespace declarations separately (namespaces array),
// so they don't show up in the attributes map returned by parseAttributes().
TEST_F(XMLTokenizerTest, ParseAttributesWithPrefix)
{
    bool ok = false;
    HashMap<String, String> attrs = WebCore::parseAttributes("xmlns:x=\"urn:example\" x:id=\"123\"", ok);
    EXPECT_TRUE(ok);
    EXPECT_TRUE(attrs.get("xmlns:x").isEmpty());
    EXPECT_EQ(String("123"), attrs.get("x:id"));
}

TEST_F(XMLTokenizerTest, ParseAttributesRejectsInvalid)
{
    bool ok = true;
    HashMap<String, String> attrs = WebCore::parseAttributes("a=\"1", ok); // missing closing quote
    EXPECT_FALSE(ok);
    EXPECT_TRUE(attrs.isEmpty());
}

// End-to-end: load XHTML via TestWebView and verify the DOM is produced.
// This exercises the XMLTokenizer path inside the normal browser pipeline.
TEST_F(XMLTokenizerTest, LoadXHTMLDocument)
{
    const char* html = R"(
        <?xml version="1.0" encoding="utf-8"?>
        <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
          "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head><title>XML Test</title></head>
        <body><p id="p">Hello</p></body>
        </html>
    )";

    loadAndWait(html);

    ASSERT_TRUE(document());
    EXPECT_FALSE(document()->parsing());

    // Basic sanity checks: title and an element with id.
    EXPECT_EQ(String("XML Test"), document()->title());
    Element* p = document()->getElementById("p");
    ASSERT_TRUE(p);
    // In XHTML, tagName() may be returned uppercased depending on the DOM/HTML
    // integration layer. Only assert it's a <p> element in a case-insensitive way.
    String tagName = p->tagName();
    EXPECT_TRUE(tagName == "p" || tagName == "P");
}

#endif // ENABLE(XML)
