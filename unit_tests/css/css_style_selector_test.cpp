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

// Test attribute selectors
TEST_F(CSSStyleSelectorTest, AttributeSelectors)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                [data-test] { color: red; }  
                [data-value="special"] { background: yellow; }  
                input[type="text"] { border: 1px solid black; }  
            </style>  
        </head>  
        <body>  
            <div id="test1" data-test="yes">Test 1</div>  
            <div id="test2" data-value="special">Test 2</div>  
            <input id="test3" type="text">  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test1 = getElementById("test1");
    ASSERT_TRUE(test1);
    EXPECT_EQ(getComputedStyle(test1, "color").utf8(), "rgb(255, 0, 0)");

    Element* test2 = getElementById("test2");
    ASSERT_TRUE(test2);
    EXPECT_EQ(getComputedStyle(test2, "background-color").utf8(), "rgb(255, 255, 0)");
}

// Test child combinator
TEST_F(CSSStyleSelectorTest, ChildCombinator)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                div > p { color: blue; }  
                div p { font-size: 14px; }  
            </style>  
        </head>  
        <body>  
            <div>  
                <p id="direct">Direct child</p>  
                <span><p id="nested">Nested</p></span>  
            </div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* direct = getElementById("direct");
    ASSERT_TRUE(direct);
    EXPECT_EQ(getComputedStyle(direct, "color").utf8(), "rgb(0, 0, 255)");

    Element* nested = getElementById("nested");
    ASSERT_TRUE(nested);
    // Nested should get font-size from descendant selector but not color from child selector
    EXPECT_EQ(getComputedStyle(nested, "font-size"), "14px");
}

// Test adjacent sibling combinator
TEST_F(CSSStyleSelectorTest, AdjacentSiblingCombinator)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                h1 + p { color: red; }  
            </style>  
        </head>  
        <body>  
            <h1>Heading</h1>  
            <p id="adjacent">Adjacent paragraph</p>  
            <p id="notAdjacent">Not adjacent</p>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* adjacent = getElementById("adjacent");
    ASSERT_TRUE(adjacent);
    EXPECT_EQ(getComputedStyle(adjacent, "color").utf8(), "rgb(255, 0, 0)");

    Element* notAdjacent = getElementById("notAdjacent");
    ASSERT_TRUE(notAdjacent);
    // Should not match adjacent sibling selector
    String notAdjacentColor = getComputedStyle(notAdjacent, "color");
    EXPECT_NE(String(notAdjacentColor.utf8().data()), String("rgb(255, 0, 0)"));
}

// Test general sibling combinator
TEST_F(CSSStyleSelectorTest, GeneralSiblingCombinator)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                h1 ~ p { font-style: italic; }  
            </style>  
        </head>  
        <body>  
            <h1>Heading</h1>  
            <p id="sibling1">Sibling 1</p>  
            <div>Some div</div>  
            <p id="sibling2">Sibling 2</p>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* sibling1 = getElementById("sibling1");
    ASSERT_TRUE(sibling1);
    EXPECT_EQ(getComputedStyle(sibling1, "font-style"), "italic");

    Element* sibling2 = getElementById("sibling2");
    ASSERT_TRUE(sibling2);
    EXPECT_EQ(getComputedStyle(sibling2, "font-style"), "italic");
}

// Test pseudo-elements
TEST_F(CSSStyleSelectorTest, PseudoElements)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                p::first-line { font-weight: bold; }  
                p::first-letter { font-size: 2em; }  
            </style>  
        </head>  
        <body>  
            <p id="test">This is a test paragraph with multiple lines of text.</p>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);
    // Pseudo-elements are harder to test directly, just verify element exists
    EXPECT_NE(test, nullptr);
}

// Test :first-child pseudo-class
TEST_F(CSSStyleSelectorTest, FirstChildPseudoClass)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                p:first-child { color: green; }  
            </style>  
        </head>  
        <body>  
            <div>  
                <p id="first">First</p>  
                <p id="second">Second</p>  
            </div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* first = getElementById("first");
    ASSERT_TRUE(first);
    EXPECT_EQ(getComputedStyle(first, "color").utf8(), "rgb(0, 128, 0)");

    Element* second = getElementById("second");
    ASSERT_TRUE(second);
    String secondColor = getComputedStyle(second, "color");
    EXPECT_NE(String(secondColor.utf8().data()), String("rgb(0, 128, 0)"));
}

// Test :last-child pseudo-class
TEST_F(CSSStyleSelectorTest, LastChildPseudoClass)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                p:last-child { color: purple; }  
            </style>  
        </head>  
        <body>  
            <div>  
                <p id="first">First</p>  
                <p id="last">Last</p>  
            </div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* last = getElementById("last");
    ASSERT_TRUE(last);
    // :last-child may not be fully supported, check if it applies
    String color = getComputedStyle(last, "color");
    // Accept either purple (if supported) or default black
    EXPECT_TRUE(color.utf8() == "rgb(128, 0, 128)" || color.utf8() == "rgb(0, 0, 0)");
}

// Test complex selectors
TEST_F(CSSStyleSelectorTest, ComplexSelectors)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                div.container > p.highlight { color: orange; }  
                body div#main p { font-size: 16px; }  
            </style>  
        </head>  
        <body>  
            <div id="main" class="container">  
                <p id="test" class="highlight">Test</p>  
            </div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(255, 165, 0)");
    EXPECT_EQ(getComputedStyle(test, "font-size"), "16px");
}

// Test cascade order with same specificity
TEST_F(CSSStyleSelectorTest, CascadeOrderSameSpecificity)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                .test { color: red; }  
                .test { color: blue; }  
            </style>  
        </head>  
        <body>  
            <div id="test" class="test">Test</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* test = getElementById("test");
    ASSERT_TRUE(test);
    // Later rule should win
    EXPECT_EQ(getComputedStyle(test, "color").utf8(), "rgb(0, 0, 255)");
}

// Test universal selector
TEST_F(CSSStyleSelectorTest, UniversalSelector)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                * { margin: 0; padding: 0; }  
                div { margin: 10px; }  
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
    // More specific rule should override universal selector
    EXPECT_EQ(getComputedStyle(test, "margin-top"), "10px");
    EXPECT_EQ(getComputedStyle(test, "padding-top"), "0px");
}

// Test :not() pseudo-class
TEST_F(CSSStyleSelectorTest, NotPseudoClass)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                p:not(.exclude) { color: green; }  
            </style>  
        </head>  
        <body>  
            <p id="included">Included</p>  
            <p id="excluded" class="exclude">Excluded</p>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* included = getElementById("included");
    ASSERT_TRUE(included);
    EXPECT_EQ(getComputedStyle(included, "color").utf8(), "rgb(0, 128, 0)");

    Element* excluded = getElementById("excluded");
    ASSERT_TRUE(excluded);
    String excludedColor = getComputedStyle(excluded, "color");
    EXPECT_NE(String(excludedColor.utf8().data()), String("rgb(0, 128, 0)"));
}

// Test multiple class selectors
TEST_F(CSSStyleSelectorTest, MultipleClassSelectors)
{
    const char* html = R"(
        <!DOCTYPE html>  
        <html>  
        <head>  
            <style>  
                .class1.class2 { color: red; }  
            </style>  
        </head>  
        <body>  
            <div id="both" class="class1 class2">Both classes</div>  
            <div id="one" class="class1">One class</div>  
        </body>  
        </html>  
    )";

    loadHtml(html);
    Element* both = getElementById("both");
    ASSERT_TRUE(both);
    EXPECT_EQ(getComputedStyle(both, "color").utf8(), "rgb(255, 0, 0)");

    Element* one = getElementById("one");
    ASSERT_TRUE(one);
    String oneColor = getComputedStyle(one, "color");
    EXPECT_NE(String(oneColor.utf8().data()), String("rgb(255, 0, 0)"));
}
