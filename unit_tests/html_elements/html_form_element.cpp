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

    HTMLInputElement* password = (HTMLInputElement*)getElementById("password");
    password->setValue("testpass");

    HTMLInputElement* email = (HTMLInputElement*)getElementById("email");
    email->setValue("test@example.com");

    HTMLInputElement* remember = (HTMLInputElement*)getElementById("remember");
    remember->setChecked(true);

    HTMLInputElement* male = (HTMLInputElement*)getElementById("male");
    male->setChecked(true);

    HTMLSelectElement* country = (HTMLSelectElement*)getElementById("country");
    country->setValue("us");

    HTMLTextAreaElement* comments = (HTMLTextAreaElement*)getElementById("comments");
    comments->setValue("This is a test comment");
}
