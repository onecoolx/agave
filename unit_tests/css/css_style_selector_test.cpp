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

namespace WebCore {
extern int getPropertyID(const char* tagStr, int len);
}

class CSSStyleSelectorTest : public ::testing::Test
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

    void loadHtml(const char* html)
    {
        view->loadHtml(html, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    Element* getElementById(const char* id)
    {
        Document* doc = view->mainframe()->document();
        return doc->getElementById(String(id));
    }

    int propertyID(const String& s)
    {
        char buffer[maxCSSPropertyNameLength];

        unsigned len = s.length();
        if (len > maxCSSPropertyNameLength) {
            return 0;
        }

        for (unsigned i = 0; i != len; ++i) {
            UChar c = s[i];
            if (c == 0 || c >= 0x7F) {
                return 0; // illegal character
            }
            buffer[i] = (char)toASCIILower(c);
        }

        return getPropertyID(buffer, len);
    }

    String getComputedStyle(Element* element, const char* property)
    {
        RefPtr<CSSComputedStyleDeclaration> style = new CSSComputedStyleDeclaration(element);
        int propID = propertyID(String(property));
        return style->getPropertyValue(propID);
    }

    TestWebView* view;
};

// Test basic CSS property application
TEST_F(CSSStyleSelectorTest, BasicPropertyApplication)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { color: red; font-size: 14px; }  
            </style>  
        </head>  
        <body>  
            <div id="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    EXPECT_STREQ(getComputedStyle(test, "color").utf8(), "rgb(255, 0, 0)");
    EXPECT_EQ(getComputedStyle(test, "font-size"), "14px");
}

// Test CSS selector specificity
TEST_F(CSSStyleSelectorTest, SelectorSpecificity)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                div { color: blue; }  
                #test { color: red; }  
                .class { color: green; }  
            </style>  
        </head>  
        <body>  
            <div id="test" class="class">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // ID selector has highest specificity
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(255, 0, 0)");
}

// Test CSS inheritance
TEST_F(CSSStyleSelectorTest, StyleInheritance)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                body { font-family: Arial; color: blue; }  
                #child { font-size: 16px; }  
            </style>  
        </head>  
        <body>  
            <div id="child">Child</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* child = getElementById("child");
    ASSERT_TRUE(child);

    // Inherits font-family from body
    EXPECT_EQ(getComputedStyle(child, "font-family"), "Arial");
    // Inherits color from body
    EXPECT_EQ(getComputedStyle(child, "color").utf8(), "rgb(0, 0, 255)");
    // Own property overrides
    EXPECT_EQ(getComputedStyle(child, "font-size"), "16px");
}

// Test !important declaration
TEST_F(CSSStyleSelectorTest, ImportantDeclaration)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { color: red !important; }  
                div { color: blue; }  
            </style>  
        </head>  
        <body>  
            <div id="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // !important overrides normal specificity
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(255, 0, 0)");
}

// Test inline styles
TEST_F(CSSStyleSelectorTest, InlineStyles)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { color: red; }  
            </style>  
        </head>  
        <body>  
            <div id="test" style="color: blue;">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // Inline style has highest specificity
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(0, 0, 255)");
}

// Test class selectors
TEST_F(CSSStyleSelectorTest, ClassSelectors)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                .test { background-color: yellow; }  
                .another { color: green; }  
            </style>  
        </head>  
        <body>  
            <div id="test" class="test another">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    EXPECT_EQ(getComputedStyle(test, "background-color").utf8(), "rgb(255, 255, 0)");
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(0, 128, 0)");
}

// Test pseudo-classes
TEST_F(CSSStyleSelectorTest, PseudoClasses)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                a:link { color: blue; }  
                a:visited { color: purple; }  
                a:hover { color: red; }  
            </style>  
        </head>  
        <body>  
            <a id="test" href="#">Test Link</a>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // Default link state
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(0, 0, 255)");
}

// Test shorthand properties
TEST_F(CSSStyleSelectorTest, ShorthandProperties)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { margin: 10px 20px; padding: 5px; border: 1px solid black; }  
            </style>  
        </head>  
        <body>  
            <div id="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // Check individual properties from shorthand
    EXPECT_EQ(getComputedStyle(test, "margin-top"), "10px");
    EXPECT_EQ(getComputedStyle(test, "margin-right"), "20px");
    EXPECT_EQ(getComputedStyle(test, "margin-bottom"), "10px");
    EXPECT_EQ(getComputedStyle(test, "margin-left"), "20px");
    EXPECT_EQ(getComputedStyle(test, "padding-top"), "5px");
    EXPECT_EQ(getComputedStyle(test, "border-top-width"), "1px");
}

// Test multiple stylesheets
TEST_F(CSSStyleSelectorTest, MultipleStylesheets)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { color: red; }  
            </style>  
            <link rel="stylesheet" href="data:text/css,#test { font-size: 20px; }">  
            <style>  
                #test { background: yellow; }  
            </style>  
        </head>  
        <body>  
            <div id="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // Properties from all stylesheets should be applied
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(255, 0, 0)");
    EXPECT_EQ(getComputedStyle(test, "font-size"), "20px");
    EXPECT_EQ(getComputedStyle(test, "background-color").utf8(), "rgb(255, 255, 0)");
}

// Test media queries
TEST_F(CSSStyleSelectorTest, MediaQueries)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                #test { color: red; }  
                @media screen {  
                    #test { color: blue; }  
                }  
                @media print {  
                    #test { color: black; }  
                }  
            </style>  
        </head>  
        <body>  
            <div id="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);

    // Screen media should apply
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(0, 0, 255)");
}

// Test CSS inheritance depth
TEST_F(CSSStyleSelectorTest, InheritanceDepth)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                body { font-size: 12px; line-height: 1.5; }  
                .nested { font-weight: bold; }  
            </style>  
        </head>  
        <body>  
            <div class="nested">  
                <div class="nested">  
                    <div id="deep">Deep nested</div>  
                </div>  
            </div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* deep = getElementById("deep");
    ASSERT_TRUE(deep);

    // Should inherit from body
    EXPECT_EQ(getComputedStyle(deep, "font-size"), "12px");
    EXPECT_EQ(getComputedStyle(deep, "line-height").utf8(), "18px");
    // Should inherit from parent
    EXPECT_EQ(getComputedStyle(deep, "font-weight"), "bold");
}
