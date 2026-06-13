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

// CSS parsing/tokenizer robustness suite. Drives the full HTML -> stylesheet
// tokenizer -> parser path and checks the applied computed style. Serves as the
// regression safety net for tokenizer changes (e.g. custom-property support).

#include "test.h"
#include "Document.h"
#include "Element.h"
#include "CSSComputedStyleDeclaration.h"
#include "CSSPropertyNames.h"
#include <wtf/unicode/Unicode.h>

using namespace WebCore;

namespace WebCore {
extern int getPropertyID(const char* tagStr, int len);
}

class CSSParsingRobustnessTest : public ::testing::Test
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

    Element* byId(const char* id)
    {
        return view->mainframe()->document()->getElementById(String(id));
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
                return 0;
            }
            buffer[i] = (char)toASCIILower(c);
        }
        return getPropertyID(buffer, len);
    }

    String computed(const char* id, const char* property)
    {
        Element* e = byId(id);
        if (!e) {
            return String();
        }
        RefPtr<CSSComputedStyleDeclaration> style = new CSSComputedStyleDeclaration(e);
        return style->getPropertyValue(propertyID(String(property)));
    }

    TestWebView* view;
};

// Hyphenated property names and identifier values must tokenize correctly.
// These are the cases most sensitive to a tokenizer ident-rule change.
TEST_F(CSSParsingRobustnessTest, HyphenatedPropertiesAndValues)
{
    loadHtml("<style>#a{background-color:rgb(1,2,3);text-align:center;font-weight:bold;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "background-color"), "rgb(1, 2, 3)");
    EXPECT_EQ(computed("a", "text-align"), "center");
    EXPECT_EQ(computed("a", "font-weight"), "bold");
}

// Vendor-prefixed (single leading hyphen) identifiers must still tokenize.
TEST_F(CSSParsingRobustnessTest, VendorPrefixIdentifiers)
{
    loadHtml("<style>#a{display:-webkit-box;}</style><div id='a'>x</div>");
    // Whether or not -webkit-box is supported, parsing must not corrupt the
    // following declarations; display should be a non-empty value.
    EXPECT_FALSE(computed("a", "display").isEmpty());
}

// Negative numbers and units.
TEST_F(CSSParsingRobustnessTest, NegativeNumbers)
{
    loadHtml("<style>#a{margin-left:-10px;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "margin-left"), "-10px");
}

// Functional notation values (rgb/url-like) parse and round-trip.
TEST_F(CSSParsingRobustnessTest, FunctionalValues)
{
    loadHtml("<style>#a{color:rgb(10,20,30);width:calc(100px - 10px);}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "color"), "rgb(10, 20, 30)");
    // calc resolves to a length; just ensure it produced a value.
    EXPECT_FALSE(computed("a", "width").isEmpty());
}

// Comments inside and between declarations must be stripped without breaking
// the surrounding tokens. (color:red serializes as rgb(255, 0, 0).)
TEST_F(CSSParsingRobustnessTest, Comments)
{
    loadHtml("<style>#a{/* c1 */color:red;/* c2 */font-size:/* mid */14px;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "color"), "rgb(255, 0, 0)");
    EXPECT_EQ(computed("a", "font-size"), "14px");
}

// A malformed declaration must be skipped without dropping valid neighbors.
TEST_F(CSSParsingRobustnessTest, MalformedDeclarationRecovery)
{
    loadHtml("<style>#a{color:red; bogus!!!; font-size:14px;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "color"), "rgb(255, 0, 0)");
    EXPECT_EQ(computed("a", "font-size"), "14px");
}

// An unknown property is dropped but valid neighbors survive.
TEST_F(CSSParsingRobustnessTest, UnknownPropertyDropped)
{
    loadHtml("<style>#a{notaproperty:5;color:blue;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "color"), "rgb(0, 0, 255)");
}

// Shorthand expansion.
TEST_F(CSSParsingRobustnessTest, ShorthandMargin)
{
    loadHtml("<style>#a{margin:1px 2px 3px 4px;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "margin-top"), "1px");
    EXPECT_EQ(computed("a", "margin-right"), "2px");
    EXPECT_EQ(computed("a", "margin-bottom"), "3px");
    EXPECT_EQ(computed("a", "margin-left"), "4px");
}

// !important priority is parsed.
TEST_F(CSSParsingRobustnessTest, ImportantPriority)
{
    loadHtml("<style>#a{color:red;} #a{color:green !important;}</style><div id='a'>x</div>");
    EXPECT_EQ(computed("a", "color"), "rgb(0, 128, 0)");
}

// Multiple selectors and class/id combinations through the tokenizer.
TEST_F(CSSParsingRobustnessTest, SelectorVariety)
{
    loadHtml("<style>.c1.c2{color:red;} div>span{font-size:9px;} #x:first-child{font-weight:bold;}</style>"
             "<div><span id='x' class='c1 c2'>y</span></div>");
    EXPECT_EQ(computed("x", "color"), "rgb(255, 0, 0)");
    EXPECT_EQ(computed("x", "font-size"), "9px");
    EXPECT_EQ(computed("x", "font-weight"), "bold");
}

// Empty rule and trailing whitespace must not break parsing.
TEST_F(CSSParsingRobustnessTest, EmptyRuleAndWhitespace)
{
    loadHtml("<style>#a{}  #b{color:red;}   </style><div id='a'>x</div><div id='b'>y</div>");
    EXPECT_EQ(computed("b", "color"), "rgb(255, 0, 0)");
}
