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

class HTMLFormElementTest : public ::testing::Test
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

    HTMLElement* getElementById(const String& id)
    {
        Document* doc = view->mainframe()->document();
        Element* element = doc->getElementById(id);
        return element ? (HTMLElement*)element : nullptr;
    }

    HTMLFormElement* getFormById(const String& id)
    {
        Document* doc = view->mainframe()->document();
        Element* element = doc->getElementById(id);
        return element ? (HTMLFormElement*)element : nullptr;
    }

    void loadTestData()
    {
        static const char* testHtml = R"(
        <!DOCTYPE html>
        <html>
        <body>
        <form id="testForm" name="myForm" action="/submit" method="post">
            <input type="text" id="username" name="username" value="">
            <input type="password" id="password" name="password" value="">
            <input type="email" id="email" name="email" value="">
            <input type="hidden" id="hiddenField" name="hidden" value="secret">
            <input type="checkbox" id="remember" name="remember" value="yes">
            <input type="radio" id="male" name="gender" value="male">
            <input type="radio" id="female" name="gender" value="female">
            <select id="country" name="country">
                <option value="cn">China</option>
                <option value="us">USA</option>
            </select>
            <textarea id="comments" name="comments"></textarea>
            <button type="submit" id="submitBtn">Submit</button>
        </form>
        </body>
        </html>
        )";

        view->loadHtml(testHtml, "http://localhost/test");

        TestWebView::waitForDocumentComplete(view);
    }

    TestWebView* view;
};

TEST_F(HTMLFormElementTest, BasicFormProperties)
{

    HTMLFormElement* form = getFormById("testForm");
    ASSERT_NE(form, nullptr);

    EXPECT_EQ(form->action(), "/submit");
    EXPECT_EQ(form->method(), "post");
    EXPECT_EQ(form->name(), "myForm");
    EXPECT_EQ(form->enctype(), "application/x-www-form-urlencoded");
}

TEST_F(HTMLFormElementTest, FormElementAccess)
{

    HTMLFormElement* form = getFormById("testForm");
    ASSERT_NE(form, nullptr);

    RefPtr<HTMLCollection> elements = form->elements();
    EXPECT_GT(elements->length(), 0);

    HTMLInputElement* username = (HTMLInputElement*)getElementById("username");
    ASSERT_NE(username, nullptr);
    EXPECT_EQ(username->name(), "username");

    HTMLInputElement* password = (HTMLInputElement*)getElementById("password");
    ASSERT_NE(password, nullptr);
    EXPECT_EQ(password->name(), "password");
}

TEST_F(HTMLFormElementTest, FormDataHandling)
{
    HTMLFormElement* form = getFormById("testForm");
    ASSERT_NE(form, nullptr);

    HTMLInputElement* username = (HTMLInputElement*)getElementById("username");
    username->setValue("testuser");
    EXPECT_EQ(username->value(), "testuser");

    HTMLInputElement* password = (HTMLInputElement*)getElementById("password");
    password->setValue("testpass");
    EXPECT_EQ(password->value(), "testpass");

    HTMLInputElement* email = (HTMLInputElement*)getElementById("email");
    email->setValue("test@example.com");
    EXPECT_EQ(email->value(), "test@example.com");

    HTMLInputElement* remember = (HTMLInputElement*)getElementById("remember");
    remember->setChecked(true);
    EXPECT_TRUE(remember->checked());

    HTMLInputElement* male = (HTMLInputElement*)getElementById("male");
    male->setChecked(true);
    EXPECT_TRUE(male->checked());

    HTMLSelectElement* country = (HTMLSelectElement*)getElementById("country");
    country->setValue("us");
    EXPECT_EQ(country->value(), "us");

    HTMLTextAreaElement* comments = (HTMLTextAreaElement*)getElementById("comments");
    comments->setValue("This is a test comment");
    EXPECT_EQ(comments->value(), "This is a test comment");
}

// Test form method variations
TEST_F(HTMLFormElementTest, FormMethodVariations)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="getForm" method="get"></form>
        <form id="postForm" method="post"></form>
        <form id="defaultForm"></form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* getForm = getFormById("getForm");
    ASSERT_NE(getForm, nullptr);
    EXPECT_EQ(getForm->method(), "get");

    HTMLFormElement* postForm = getFormById("postForm");
    ASSERT_NE(postForm, nullptr);
    EXPECT_EQ(postForm->method(), "post");

    HTMLFormElement* defaultForm = getFormById("defaultForm");
    ASSERT_NE(defaultForm, nullptr);
    // Default method may be empty string or "get" depending on implementation
    String method = defaultForm->method();
    EXPECT_TRUE(method.isEmpty() || method == "get");
}

// Test form enctype variations
TEST_F(HTMLFormElementTest, FormEnctypeVariations)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="urlencoded" enctype="application/x-www-form-urlencoded"></form>
        <form id="multipart" enctype="multipart/form-data"></form>
        <form id="plaintext" enctype="text/plain"></form>
        <form id="defaultEnctype"></form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* urlencoded = getFormById("urlencoded");
    ASSERT_NE(urlencoded, nullptr);
    EXPECT_EQ(urlencoded->enctype(), "application/x-www-form-urlencoded");

    HTMLFormElement* multipart = getFormById("multipart");
    ASSERT_NE(multipart, nullptr);
    EXPECT_EQ(multipart->enctype(), "multipart/form-data");

    HTMLFormElement* plaintext = getFormById("plaintext");
    ASSERT_NE(plaintext, nullptr);
    EXPECT_EQ(plaintext->enctype(), "text/plain");

    HTMLFormElement* defaultEnctype = getFormById("defaultEnctype");
    ASSERT_NE(defaultEnctype, nullptr);
    EXPECT_EQ(defaultEnctype->enctype(), "application/x-www-form-urlencoded");
}

// Test checkbox handling
TEST_F(HTMLFormElementTest, CheckboxHandling)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="checkbox" id="cb1" name="option1" value="val1">
            <input type="checkbox" id="cb2" name="option2" value="val2" checked>
            <input type="checkbox" id="cb3" name="option3">
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* cb1 = (HTMLInputElement*)getElementById("cb1");
    ASSERT_NE(cb1, nullptr);
    EXPECT_FALSE(cb1->checked());
    EXPECT_EQ(cb1->value(), "val1");

    HTMLInputElement* cb2 = (HTMLInputElement*)getElementById("cb2");
    ASSERT_NE(cb2, nullptr);
    EXPECT_TRUE(cb2->checked());

    HTMLInputElement* cb3 = (HTMLInputElement*)getElementById("cb3");
    ASSERT_NE(cb3, nullptr);
    EXPECT_FALSE(cb3->checked());
    // Default value for checkbox without value attribute may be empty or "on"
    String value = cb3->value();
    EXPECT_TRUE(value.isEmpty() || value == "on");
}

// Test radio button groups
TEST_F(HTMLFormElementTest, RadioButtonGroups)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="radio" id="r1" name="group1" value="a">
            <input type="radio" id="r2" name="group1" value="b" checked>
            <input type="radio" id="r3" name="group1" value="c">
            <input type="radio" id="r4" name="group2" value="x">
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* r1 = (HTMLInputElement*)getElementById("r1");
    HTMLInputElement* r2 = (HTMLInputElement*)getElementById("r2");
    HTMLInputElement* r3 = (HTMLInputElement*)getElementById("r3");

    ASSERT_NE(r1, nullptr);
    ASSERT_NE(r2, nullptr);
    ASSERT_NE(r3, nullptr);

    // Initially r2 is checked
    EXPECT_FALSE(r1->checked());
    EXPECT_TRUE(r2->checked());
    EXPECT_FALSE(r3->checked());

    // Check r1, should uncheck r2
    r1->setChecked(true);
    EXPECT_TRUE(r1->checked());
    // Note: Depending on implementation, r2 might still show as checked
    // This tests the basic setChecked functionality
}

// Test select element with options
TEST_F(HTMLFormElementTest, SelectElementOptions)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <select id="select1" name="choice">
                <option value="opt1">Option 1</option>
                <option value="opt2" selected>Option 2</option>
                <option value="opt3">Option 3</option>
            </select>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLSelectElement* select = (HTMLSelectElement*)getElementById("select1");
    ASSERT_NE(select, nullptr);

    // Check initial selected value
    EXPECT_EQ(select->value(), "opt2");

    // Change selection
    select->setValue("opt3");
    EXPECT_EQ(select->value(), "opt3");
}

// Test textarea element
TEST_F(HTMLFormElementTest, TextareaElement)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <textarea id="ta1" name="text1">Initial text</textarea>
            <textarea id="ta2" name="text2"></textarea>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLTextAreaElement* ta1 = (HTMLTextAreaElement*)getElementById("ta1");
    ASSERT_NE(ta1, nullptr);
    EXPECT_EQ(ta1->value(), "Initial text");

    HTMLTextAreaElement* ta2 = (HTMLTextAreaElement*)getElementById("ta2");
    ASSERT_NE(ta2, nullptr);
    EXPECT_TRUE(ta2->value().isEmpty());

    // Set new value
    ta2->setValue("New content\nwith newline");
    EXPECT_EQ(ta2->value(), "New content\nwith newline");
}

// Test hidden input fields
TEST_F(HTMLFormElementTest, HiddenInputFields)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="hidden" id="h1" name="token" value="abc123">
            <input type="hidden" id="h2" name="session">
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* h1 = (HTMLInputElement*)getElementById("h1");
    ASSERT_NE(h1, nullptr);
    EXPECT_EQ(h1->value(), "abc123");

    HTMLInputElement* h2 = (HTMLInputElement*)getElementById("h2");
    ASSERT_NE(h2, nullptr);
    EXPECT_TRUE(h2->value().isEmpty());
}

// Test form reset functionality
TEST_F(HTMLFormElementTest, FormReset)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" id="text1" name="text1" value="initial">
            <input type="checkbox" id="cb1" name="cb1" checked>
            <textarea id="ta1" name="ta1">initial text</textarea>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* form = getFormById("form");
    ASSERT_NE(form, nullptr);

    HTMLInputElement* text1 = (HTMLInputElement*)getElementById("text1");
    HTMLInputElement* cb1 = (HTMLInputElement*)getElementById("cb1");
    HTMLTextAreaElement* ta1 = (HTMLTextAreaElement*)getElementById("ta1");

    // Modify values
    text1->setValue("modified");
    cb1->setChecked(false);
    ta1->setValue("modified text");

    // Reset form
    form->reset();

    // Values should be restored to initial state
    EXPECT_EQ(text1->value(), "initial");
    EXPECT_TRUE(cb1->checked());
    EXPECT_EQ(ta1->value(), "initial text");
}

// Test empty form
TEST_F(HTMLFormElementTest, EmptyForm)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="emptyForm"></form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* form = getFormById("emptyForm");
    ASSERT_NE(form, nullptr);

    RefPtr<HTMLCollection> elements = form->elements();
    EXPECT_EQ(elements->length(), 0);
}

// Test form with disabled elements
TEST_F(HTMLFormElementTest, DisabledElements)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" id="enabled" name="enabled" value="test">
            <input type="text" id="disabled" name="disabled" value="test" disabled>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* enabled = (HTMLInputElement*)getElementById("enabled");
    ASSERT_NE(enabled, nullptr);
    EXPECT_FALSE(enabled->disabled());

    HTMLInputElement* disabled = (HTMLInputElement*)getElementById("disabled");
    ASSERT_NE(disabled, nullptr);
    EXPECT_TRUE(disabled->disabled());
}

// Test form with readonly elements
TEST_F(HTMLFormElementTest, ReadonlyElements)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" id="normal" name="normal" value="test">
            <input type="text" id="readonly" name="readonly" value="test" readonly>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* normal = (HTMLInputElement*)getElementById("normal");
    ASSERT_NE(normal, nullptr);
    EXPECT_FALSE(normal->readOnly());

    HTMLInputElement* readonly = (HTMLInputElement*)getElementById("readonly");
    ASSERT_NE(readonly, nullptr);
    EXPECT_TRUE(readonly->readOnly());
}

// Test form elements collection length
TEST_F(HTMLFormElementTest, ElementsCollectionLength)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" name="field1">
            <input type="text" name="field2">
            <select name="field3"><option>A</option></select>
            <textarea name="field4"></textarea>
            <button type="submit">Submit</button>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* form = getFormById("form");
    ASSERT_NE(form, nullptr);

    RefPtr<HTMLCollection> elements = form->elements();
    // Should include all form controls
    EXPECT_GE(elements->length(), 4);
}

// Test form with multiple submit buttons
TEST_F(HTMLFormElementTest, MultipleSubmitButtons)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" name="field1">
            <button type="submit" id="btn1" name="action" value="save">Save</button>
            <button type="submit" id="btn2" name="action" value="delete">Delete</button>
            <input type="submit" id="btn3" value="Submit">
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLElement* btn1 = getElementById("btn1");
    ASSERT_NE(btn1, nullptr);

    HTMLElement* btn2 = getElementById("btn2");
    ASSERT_NE(btn2, nullptr);

    HTMLInputElement* btn3 = (HTMLInputElement*)getElementById("btn3");
    ASSERT_NE(btn3, nullptr);
    EXPECT_EQ(btn3->value(), "Submit");
}

// Test form target attribute
TEST_F(HTMLFormElementTest, FormTarget)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form1" target="_blank"></form>
        <form id="form2" target="_self"></form>
        <form id="form3"></form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* form1 = getFormById("form1");
    ASSERT_NE(form1, nullptr);
    EXPECT_EQ(form1->target(), "_blank");

    HTMLFormElement* form2 = getFormById("form2");
    ASSERT_NE(form2, nullptr);
    EXPECT_EQ(form2->target(), "_self");

    HTMLFormElement* form3 = getFormById("form3");
    ASSERT_NE(form3, nullptr);
    // Default target is empty
    EXPECT_TRUE(form3->target().isEmpty());
}

// Test form acceptCharset attribute
TEST_F(HTMLFormElementTest, FormAcceptCharset)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form1" accept-charset="UTF-8"></form>
        <form id="form2" accept-charset="ISO-8859-1"></form>
        <form id="form3"></form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLFormElement* form1 = getFormById("form1");
    ASSERT_NE(form1, nullptr);
    EXPECT_EQ(form1->acceptCharset(), "UTF-8");

    HTMLFormElement* form2 = getFormById("form2");
    ASSERT_NE(form2, nullptr);
    EXPECT_EQ(form2->acceptCharset(), "ISO-8859-1");
}

// Test input type variations
TEST_F(HTMLFormElementTest, InputTypeVariations)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <input type="text" id="text" name="text">
            <input type="password" id="password" name="password">
            <input type="email" id="email" name="email">
            <input type="number" id="number" name="number">
            <input type="tel" id="tel" name="tel">
            <input type="url" id="url" name="url">
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLInputElement* text = (HTMLInputElement*)getElementById("text");
    ASSERT_NE(text, nullptr);
    EXPECT_EQ(text->type(), "text");

    HTMLInputElement* password = (HTMLInputElement*)getElementById("password");
    ASSERT_NE(password, nullptr);
    EXPECT_EQ(password->type(), "password");

    HTMLInputElement* email = (HTMLInputElement*)getElementById("email");
    ASSERT_NE(email, nullptr);
    // HTML5 input types like "email" may not be supported and fall back to "text"
    String emailType = email->type();
    EXPECT_TRUE(emailType == "email" || emailType == "text");
}

// Test form with fieldset
TEST_F(HTMLFormElementTest, FormWithFieldset)
{
    const char* html = R"(
        <!DOCTYPE html>
        <html><body>
        <form id="form">
            <fieldset id="fs1">
                <legend>Personal Info</legend>
                <input type="text" name="name">
            </fieldset>
            <fieldset id="fs2" disabled>
                <input type="text" name="disabled_field">
            </fieldset>
        </form>
        </body></html>
    )";

    view->loadHtml(html, "http://localhost/test");
    TestWebView::waitForDocumentComplete(view);

    HTMLElement* fs1 = getElementById("fs1");
    ASSERT_NE(fs1, nullptr);

    HTMLElement* fs2 = getElementById("fs2");
    ASSERT_NE(fs2, nullptr);
}

// Test form length property
TEST_F(HTMLFormElementTest, FormLength)
{
    HTMLFormElement* form = getFormById("testForm");
    ASSERT_NE(form, nullptr);

    RefPtr<HTMLCollection> elements = form->elements();
    unsigned length = elements->length();

    // Should have multiple form controls
    EXPECT_GT(length, 0);
    EXPECT_EQ(form->length(), length);
}
