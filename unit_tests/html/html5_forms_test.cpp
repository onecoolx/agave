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

#if ENABLE(HTML5_FORMS)

class HTML5FormsTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite() { Test_Init(); }
    static void TearDownTestSuite() { Test_Shutdown(); }

    void SetUp() override
    {
        view = new TestWebView();
        loadTestData();
    }

    void TearDown() override { delete view; }

    HTMLInputElement* input(const String& id)
    {
        Document* doc = view->mainframe()->document();
        Element* e = doc->getElementById(id);
        return e ? (HTMLInputElement*)e : nullptr;
    }

    void loadTestData()
    {
        static const char* testHtml = R"(
        <!DOCTYPE html><html><body>
        <input type="email" id="email" value="">
        <input type="url" id="url" value="">
        <input type="number" id="number" value="">
        <input type="date" id="date" value="">
        <input type="color" id="color" value="">
        <input type="text" id="req" required value="">
        <input type="text" id="pat" pattern="[0-9]+" value="">
        <input type="number" id="ranged" min="10" max="20" step="5" value="">
        </body></html>
        )";
        view->loadHtml(testHtml, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    TestWebView* view;
};

TEST_F(HTML5FormsTest, TypeRecognition)
{
    EXPECT_EQ(input("email")->type(), "email");
    EXPECT_EQ(input("url")->type(), "url");
    EXPECT_EQ(input("number")->type(), "number");
    EXPECT_EQ(input("date")->type(), "date");
    EXPECT_EQ(input("color")->type(), "color");
    // HTML5 text-like types behave as text fields.
    EXPECT_TRUE(input("email")->isTextField());
    EXPECT_TRUE(input("number")->isTextField());
}

TEST_F(HTML5FormsTest, EmailTypeMismatch)
{
    HTMLInputElement* e = input("email");
    e->setValue("not-an-email");
    EXPECT_TRUE(e->typeMismatch());
    e->setValue("user@example.com");
    EXPECT_FALSE(e->typeMismatch());
    // Empty value is never a type mismatch (that is valueMissing's job).
    e->setValue("");
    EXPECT_FALSE(e->typeMismatch());
}

TEST_F(HTML5FormsTest, UrlAndNumberTypeMismatch)
{
    HTMLInputElement* u = input("url");
    u->setValue("notaurl");
    EXPECT_TRUE(u->typeMismatch());
    u->setValue("https://example.com/path");
    EXPECT_FALSE(u->typeMismatch());

    HTMLInputElement* n = input("number");
    n->setValue("abc");
    EXPECT_TRUE(n->typeMismatch());
    n->setValue("3.14");
    EXPECT_FALSE(n->typeMismatch());
}

TEST_F(HTML5FormsTest, DateAndColorTypeMismatch)
{
    HTMLInputElement* d = input("date");
    d->setValue("2026-13-40"); // invalid month/day
    EXPECT_TRUE(d->typeMismatch());
    d->setValue("2026-02-29"); // 2026 is not a leap year
    EXPECT_TRUE(d->typeMismatch());
    d->setValue("2024-02-29"); // 2024 is a leap year
    EXPECT_FALSE(d->typeMismatch());

    HTMLInputElement* c = input("color");
    c->setValue("red");
    EXPECT_TRUE(c->typeMismatch());
    c->setValue("#aabbcc");
    EXPECT_FALSE(c->typeMismatch());
}

TEST_F(HTML5FormsTest, RequiredValueMissing)
{
    HTMLInputElement* r = input("req");
    EXPECT_TRUE(r->required());
    EXPECT_TRUE(r->valueMissing());
    EXPECT_FALSE(r->valid());
    r->setValue("filled");
    EXPECT_FALSE(r->valueMissing());
    EXPECT_TRUE(r->valid());
}

TEST_F(HTML5FormsTest, PatternMismatch)
{
    HTMLInputElement* p = input("pat");
    p->setValue("abc");
    EXPECT_TRUE(p->patternMismatch());
    p->setValue("12345");
    EXPECT_FALSE(p->patternMismatch());
    // A partial match must not count as a full match.
    p->setValue("12a34");
    EXPECT_TRUE(p->patternMismatch());
}

TEST_F(HTML5FormsTest, RangeAndStep)
{
    HTMLInputElement* n = input("ranged"); // min=10 max=20 step=5
    n->setValue("5");
    EXPECT_TRUE(n->rangeUnderflow());
    n->setValue("25");
    EXPECT_TRUE(n->rangeOverflow());
    n->setValue("15"); // 10 + 5
    EXPECT_FALSE(n->rangeUnderflow());
    EXPECT_FALSE(n->rangeOverflow());
    EXPECT_FALSE(n->stepMismatch());
    n->setValue("12"); // not on the step grid (10,15,20)
    EXPECT_TRUE(n->stepMismatch());
}

TEST_F(HTML5FormsTest, CustomValidity)
{
    HTMLInputElement* e = input("email");
    e->setValue("user@example.com");
    EXPECT_TRUE(e->valid());
    e->setCustomValidity("forced error");
    EXPECT_TRUE(e->customError());
    EXPECT_FALSE(e->valid());
    EXPECT_EQ(e->validationMessage(), "forced error");
    e->setCustomValidity("");
    EXPECT_FALSE(e->customError());
    EXPECT_TRUE(e->valid());
}

TEST_F(HTML5FormsTest, CheckValidityReturnsValidity)
{
    HTMLInputElement* r = input("req");
    EXPECT_FALSE(r->checkValidity()); // empty required
    r->setValue("x");
    EXPECT_TRUE(r->checkValidity());
}

#endif // ENABLE(HTML5_FORMS)
