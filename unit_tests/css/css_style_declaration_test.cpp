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

class CSSStyleDeclarationTest : public ::testing::Test
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
        decl = new CSSMutableStyleDeclaration();
    }

    void TearDown() override
    {
        decl->deref();
    }

    CSSMutableStyleDeclaration* decl;
};

TEST_F(CSSStyleDeclarationTest, CssTextRoundTrip)
{
    ExceptionCode ec = 0;
    decl->setCssText("color: red; font-size: 14px;", ec);
    EXPECT_EQ(decl->cssText(), "color: red; font-size: 14px; ");
}

TEST_F(CSSStyleDeclarationTest, LengthAndItem)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    decl->setProperty(CSS_PROP_FONT_SIZE, "14px", false, ec);

    EXPECT_EQ(decl->length(), 2u);
    EXPECT_EQ(decl->item(0), "color");
    EXPECT_EQ(decl->item(1), "font-size");
    EXPECT_TRUE(decl->item(2).isNull());
}

TEST_F(CSSStyleDeclarationTest, GetPropertyCSSValue)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    RefPtr<CSSValue> value = decl->getPropertyCSSValue(CSS_PROP_COLOR);
    ASSERT_TRUE(value);
    EXPECT_EQ(value->cssText(), "red");
}

TEST_F(CSSStyleDeclarationTest, GetPropertyValue)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_COLOR), "red");
    EXPECT_TRUE(decl->getPropertyValue(CSS_PROP_BACKGROUND).isEmpty());
}

TEST_F(CSSStyleDeclarationTest, GetPropertyPriority)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    EXPECT_FALSE(decl->getPropertyPriority(CSS_PROP_COLOR));

    decl->setProperty(CSS_PROP_COLOR, "blue", true, ec);
    EXPECT_TRUE(decl->getPropertyPriority(CSS_PROP_COLOR));
}

TEST_F(CSSStyleDeclarationTest, RemoveProperty)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    String removed = decl->removeProperty(CSS_PROP_COLOR, ec);
    EXPECT_EQ(removed, "red");
    EXPECT_TRUE(decl->getPropertyValue(CSS_PROP_COLOR).isEmpty());
}

TEST_F(CSSStyleDeclarationTest, CopyAndMakeMutable)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    RefPtr<CSSMutableStyleDeclaration> copy = decl->copy();
    ASSERT_TRUE(copy);
    EXPECT_EQ(copy->getPropertyValue(CSS_PROP_COLOR), "red");
}

// Constructor tests
TEST_F(CSSStyleDeclarationTest, Constructors)
{
    CSSMutableStyleDeclaration decl1;
    EXPECT_EQ(0u, decl1.length());

    CSSMutableStyleDeclaration decl2(nullptr);
    EXPECT_EQ(0u, decl2.length());

    Deque<CSSProperty> props;
    props.append(CSSProperty(CSS_PROP_COLOR, new CSSPrimitiveValue(String("red"), CSSPrimitiveValue::CSS_STRING), false));
    CSSMutableStyleDeclaration decl3(nullptr, props);
    EXPECT_EQ(1u, decl3.length());

    CSSProperty* propArray[] = { new CSSProperty(CSS_PROP_COLOR, new CSSPrimitiveValue(String("blue"), CSSPrimitiveValue::CSS_STRING), false) };
    CSSMutableStyleDeclaration decl4(nullptr, propArray, 1);
    EXPECT_EQ(1u, decl4.length());
}

// Assignment operator test
TEST_F(CSSStyleDeclarationTest, AssignmentOperator)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    CSSMutableStyleDeclaration other;
    other = *decl;
    EXPECT_EQ(other.getPropertyValue(CSS_PROP_COLOR), "red");
}

// Property setting test
TEST_F(CSSStyleDeclarationTest, SetPropertyVariants)
{
    ExceptionCode ec = 0;
    // String value setting
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_COLOR), "red");

    // With notification control
    EXPECT_TRUE(decl->setProperty(CSS_PROP_FONT_SIZE, "14px", false, false, ec));

    // Empty value removes property
    decl->setProperty(CSS_PROP_COLOR, "", false, ec);
    EXPECT_TRUE(decl->getPropertyValue(CSS_PROP_COLOR).isEmpty());
}

// Special property setter methods
TEST_F(CSSStyleDeclarationTest, SpecialPropertySetters)
{
    decl->setStringProperty(CSS_PROP_FONT_FAMILY, "Arial", CSSPrimitiveValue::CSS_STRING, false);
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_FONT_FAMILY), "Arial");

    decl->setImageProperty(CSS_PROP_BACKGROUND_IMAGE, "url(test.png)", false);
    EXPECT_FALSE(decl->getPropertyValue(CSS_PROP_BACKGROUND_IMAGE).isEmpty());

    decl->setLengthProperty(CSS_PROP_WIDTH, "100px", false);
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_WIDTH), "100px");
}

// Shorthand property test - margin
TEST_F(CSSStyleDeclarationTest, ShorthandMargin)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_MARGIN_TOP, "10px", false, ec);
    decl->setProperty(CSS_PROP_MARGIN_RIGHT, "20px", false, ec);
    decl->setProperty(CSS_PROP_MARGIN_BOTTOM, "10px", false, ec);
    decl->setProperty(CSS_PROP_MARGIN_LEFT, "20px", false, ec);

    String margin = decl->getPropertyValue(CSS_PROP_MARGIN);
    EXPECT_FALSE(margin.isEmpty());
    EXPECT_TRUE(margin.contains("10px"));
    EXPECT_TRUE(margin.contains("20px"));
}

// Shorthand property test - padding
TEST_F(CSSStyleDeclarationTest, ShorthandPadding)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_PADDING_TOP, "5px", false, ec);
    decl->setProperty(CSS_PROP_PADDING_RIGHT, "10px", false, ec);
    decl->setProperty(CSS_PROP_PADDING_BOTTOM, "5px", false, ec);
    decl->setProperty(CSS_PROP_PADDING_LEFT, "10px", false, ec);

    String padding = decl->getPropertyValue(CSS_PROP_PADDING);
    EXPECT_FALSE(padding.isEmpty());
}

// Shorthand property test - border
TEST_F(CSSStyleDeclarationTest, ShorthandBorder)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_BORDER_TOP_WIDTH, "1px", false, ec);
    decl->setProperty(CSS_PROP_BORDER_TOP_STYLE, "solid", false, ec);
    decl->setProperty(CSS_PROP_BORDER_TOP_COLOR, "red", false, ec);

    String borderTop = decl->getPropertyValue(CSS_PROP_BORDER_TOP);
    EXPECT_FALSE(borderTop.isEmpty());
    EXPECT_TRUE(borderTop.contains("1px"));
    EXPECT_TRUE(borderTop.contains("solid"));
    EXPECT_TRUE(borderTop.contains("red"));
}

// Shorthand property test - background
TEST_F(CSSStyleDeclarationTest, ShorthandBackground)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_BACKGROUND_COLOR, "red", false, ec);
    decl->setProperty(CSS_PROP_BACKGROUND_IMAGE, "url(test.png)", false, ec);
    decl->setProperty(CSS_PROP_BACKGROUND_REPEAT, "repeat", false, ec);

    String background = decl->getPropertyValue(CSS_PROP_BACKGROUND);
    EXPECT_FALSE(background.isEmpty());
}

// Parse declaration test
TEST_F(CSSStyleDeclarationTest, ParseDeclaration)
{
    decl->parseDeclaration("color: red; font-size: 14px; background: url(test.png)");

    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_COLOR), "red");
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_FONT_SIZE), "14px");
    EXPECT_FALSE(decl->getPropertyValue(CSS_PROP_BACKGROUND_IMAGE).isEmpty());
}

// Clear test
TEST_F(CSSStyleDeclarationTest, Clear)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    EXPECT_EQ(decl->length(), 1u);

    decl->clear();
    EXPECT_EQ(decl->length(), 0u);
}

// Block property copy test
TEST_F(CSSStyleDeclarationTest, CopyBlockProperties)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec); // Non-block property
    decl->setProperty(CSS_PROP_TEXT_ALIGN, "center", false, ec); // Block property
    decl->setProperty(CSS_PROP_PAGE_BREAK_AFTER, "always", false, ec); // Block property

    RefPtr<CSSMutableStyleDeclaration> blockProps = decl->copyBlockProperties();
    ASSERT_TRUE(blockProps);

    EXPECT_TRUE(blockProps->getPropertyValue(CSS_PROP_COLOR).isEmpty());
    EXPECT_EQ(blockProps->getPropertyValue(CSS_PROP_TEXT_ALIGN), "center");
    EXPECT_EQ(blockProps->getPropertyValue(CSS_PROP_PAGE_BREAK_AFTER), "always");
}

// Remove block properties test
TEST_F(CSSStyleDeclarationTest, RemoveBlockProperties)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec); // Non-block property
    decl->setProperty(CSS_PROP_TEXT_ALIGN, "center", false, ec); // Block property

    decl->removeBlockProperties();

    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_COLOR), "red");
    EXPECT_TRUE(decl->getPropertyValue(CSS_PROP_TEXT_ALIGN).isEmpty());
}

// Merge test
TEST_F(CSSStyleDeclarationTest, Merge)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);

    CSSMutableStyleDeclaration other;
    other.setProperty(CSS_PROP_FONT_SIZE, "14px", false, ec);
    other.setProperty(CSS_PROP_COLOR, "blue", false, ec);

    decl->merge(&other, true); // Parameter overrides conflicts
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_COLOR), "blue");
    EXPECT_EQ(decl->getPropertyValue(CSS_PROP_FONT_SIZE), "14px");
}

// Add parsed properties test
TEST_F(CSSStyleDeclarationTest, AddParsedProperties)
{
    CSSProperty* props[] = {
        new CSSProperty(CSS_PROP_COLOR, new CSSPrimitiveValue(String("red"), CSSPrimitiveValue::CSS_STRING), false),
        new CSSProperty(CSS_PROP_FONT_SIZE, new CSSPrimitiveValue(String("14px"), CSSPrimitiveValue::CSS_STRING), false)
    };

    decl->addParsedProperties(props, 2);
    EXPECT_EQ(decl->length(), 2u);
}

// Property priority and shorthand ID test
TEST_F(CSSStyleDeclarationTest, PropertyMetadata)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", true, ec);

    EXPECT_TRUE(decl->getPropertyPriority(CSS_PROP_COLOR));
    EXPECT_EQ(decl->getPropertyShorthand(CSS_PROP_COLOR), 0);
    EXPECT_FALSE(decl->isPropertyImplicit(CSS_PROP_COLOR));
}

// Remove property variants test
TEST_F(CSSStyleDeclarationTest, RemovePropertyVariants)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);

    // With return text
    String removed = decl->removeProperty(CSS_PROP_COLOR, true, true, ec);
    EXPECT_EQ(removed, "red");

    decl->setProperty(CSS_PROP_COLOR, "blue", false, ec);
    // Without return text
    decl->removeProperty(CSS_PROP_COLOR, true, false, ec);
    EXPECT_TRUE(decl->getPropertyValue(CSS_PROP_COLOR).isEmpty());
}

// cssText generation test
TEST_F(CSSStyleDeclarationTest, CssTextGeneration)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_COLOR, "red", false, ec);
    decl->setProperty(CSS_PROP_FONT_SIZE, "14px", true, ec);

    String cssText = decl->cssText();
    EXPECT_TRUE(cssText.contains("color: red"));
    EXPECT_TRUE(cssText.contains("font-size: 14px"));
    EXPECT_TRUE(cssText.contains("!important"));
}

// Background position special handling test
TEST_F(CSSStyleDeclarationTest, BackgroundPositionHandling)
{
    ExceptionCode ec = 0;
    decl->setProperty(CSS_PROP_BACKGROUND_POSITION_X, "10px", false, ec);
    decl->setProperty(CSS_PROP_BACKGROUND_POSITION_Y, "20px", false, ec);

    String cssText = decl->cssText();
    EXPECT_TRUE(cssText.contains("background-position"));
}
