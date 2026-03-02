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

class DocumentURLTest : public ::testing::Test
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
};

// ============================================================
// Document URL set/get
// ============================================================

TEST_F(DocumentURLTest, SetAndGetURL)
{
    TestWebView view;
    Frame* frame = view.mainframe();
    ASSERT_NE(frame, (Frame*)0);

    Document* doc = frame->document();
    ASSERT_NE(doc, (Document*)0);

    doc->setURL(String("http://example.com/test"));
    EXPECT_EQ(doc->URL(), String("http://example.com/test"));
}

TEST_F(DocumentURLTest, DefaultURLIsAboutBlank)
{
    TestWebView view;
    Frame* frame = view.mainframe();
    ASSERT_NE(frame, (Frame*)0);

    Document* doc = frame->document();
    ASSERT_NE(doc, (Document*)0);

    // When URL is empty, should return "about:blank"
    doc->setURL(String(""));
    EXPECT_EQ(doc->URL(), String("about:blank"));
}

// ============================================================
// Document baseURL set/get
// ============================================================

TEST_F(DocumentURLTest, SetAndGetBaseURL)
{
    TestWebView view;
    Frame* frame = view.mainframe();
    ASSERT_NE(frame, (Frame*)0);

    Document* doc = frame->document();
    ASSERT_NE(doc, (Document*)0);

    doc->setBaseURL(String("http://cdn.example.com/assets/"));
    EXPECT_EQ(doc->baseURL(), String("http://cdn.example.com/assets/"));
}

TEST_F(DocumentURLTest, BaseURLFallsBackToURL)
{
    TestWebView view;
    Frame* frame = view.mainframe();
    ASSERT_NE(frame, (Frame*)0);

    Document* doc = frame->document();
    ASSERT_NE(doc, (Document*)0);

    doc->setURL(String("http://example.com/page"));
    doc->setBaseURL(String(""));
    // When baseURL is empty, should fall back to URL()
    EXPECT_EQ(doc->baseURL(), doc->URL());
}

// ============================================================
// URL after loading HTML
// ============================================================

TEST_F(DocumentURLTest, URLAfterLoadHtml)
{
    TestWebView view;
    const char* html = "<html><body><p>Hello</p></body></html>";
    const char* url = "http://test.example.com/page.html";

    view.loadHtml(html, url);
    TestWebView::waitForDocumentComplete(&view);

    Frame* frame = view.mainframe();
    ASSERT_NE(frame, (Frame*)0);

    Document* doc = frame->document();
    ASSERT_NE(doc, (Document*)0);

    // After loading, the document URL should match the provided URL
    String docURL = doc->URL();
    EXPECT_FALSE(docURL.isEmpty());
    EXPECT_TRUE(docURL.contains("test.example.com"));
}

// **Validates: Requirements 5.1, 5.2, 5.3, 9.5**
