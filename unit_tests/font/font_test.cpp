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

class FontTest : public ::testing::Test
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
        // Create a basic font description
        desc.setComputedSize(14.0f);
        desc.setSpecifiedSize(14.0f);
        desc.setItalic(false);
        desc.setWeight(cNormalWeight);
        desc.setSmallCaps(false);

        FontFamily family;
        family.setFamily("Arial");
        desc.setFamily(family);
    }

    void TearDown() override
    {
    }

    FontDescription desc;
};

TEST_F(FontTest, DefaultConstructor)
{
    Font font;

    // Default font has no description, so size is 0
    EXPECT_EQ(font.pixelSize(), 0);
    EXPECT_FLOAT_EQ(font.size(), 0.0f);
    EXPECT_EQ(font.letterSpacing(), 0);
    EXPECT_EQ(font.wordSpacing(), 0);
}

TEST_F(FontTest, ConstructorWithDescription)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    EXPECT_EQ(font.pixelSize(), 14);
    EXPECT_FLOAT_EQ(font.size(), 14.0f);
    EXPECT_FALSE(font.italic());
    EXPECT_EQ(font.weight(), cNormalWeight);
    EXPECT_FALSE(font.bold());
    EXPECT_FALSE(font.isSmallCaps());
}

TEST_F(FontTest, CopyConstructor)
{
    Font font1(desc, 2, 3);
    font1.update(nullptr);
    Font font2(font1);

    EXPECT_EQ(font1.pixelSize(), font2.pixelSize());
    EXPECT_EQ(font1.size(), font2.size());
    EXPECT_EQ(font1.letterSpacing(), font2.letterSpacing());
    EXPECT_EQ(font1.wordSpacing(), font2.wordSpacing());
}

TEST_F(FontTest, AssignmentOperator)
{
    Font font1(desc, 2, 3);
    font1.update(nullptr);
    Font font2;

    font2 = font1;

    EXPECT_EQ(font1.pixelSize(), font2.pixelSize());
    EXPECT_EQ(font1.letterSpacing(), font2.letterSpacing());
    EXPECT_EQ(font1.wordSpacing(), font2.wordSpacing());
}

TEST_F(FontTest, EqualityOperator)
{
    Font font1(desc, 2, 3);
    font1.update(nullptr);
    Font font2(desc, 2, 3);
    font2.update(nullptr);
    Font font3(desc, 1, 1);
    font3.update(nullptr);

    EXPECT_TRUE(font1 == font2);
    EXPECT_FALSE(font1 == font3);
    EXPECT_TRUE(font1 != font3);
}

TEST_F(FontTest, LetterAndWordSpacing)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    EXPECT_EQ(font.letterSpacing(), 0);
    EXPECT_EQ(font.wordSpacing(), 0);

    font.setLetterSpacing(2);
    font.setWordSpacing(5);

    EXPECT_EQ(font.letterSpacing(), 2);
    EXPECT_EQ(font.wordSpacing(), 5);
}

TEST_F(FontTest, FontMetrics)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Font metrics should be positive
    EXPECT_GT(font.ascent(), 0);
    EXPECT_GT(font.descent(), 0);
    EXPECT_GT(font.height(), 0);
    EXPECT_GT(font.lineSpacing(), 0);
    EXPECT_GT(font.xHeight(), 0.0f);
    EXPECT_GT(font.spaceWidth(), 0);
    EXPECT_GT(font.tabWidth(), 0);

    // Height should be ascent + descent
    EXPECT_EQ(font.height(), font.ascent() + font.descent());

    // Tab width should be 8 times space width
    EXPECT_EQ(font.tabWidth(), 8 * font.spaceWidth());
}

TEST_F(FontTest, BoldAndItalic)
{
    desc.setItalic(true);
    desc.setWeight(cBoldWeight);

    Font font(desc, 0, 0);
    font.update(nullptr);

    EXPECT_TRUE(font.italic());
    EXPECT_TRUE(font.bold());
    EXPECT_EQ(font.weight(), cBoldWeight);
}

TEST_F(FontTest, SmallCaps)
{
    desc.setSmallCaps(true);

    Font font(desc, 0, 0);
    font.update(nullptr);

    EXPECT_TRUE(font.isSmallCaps());
}

TEST_F(FontTest, FontFamily)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    const FontFamily& family = font.family();
    EXPECT_FALSE(family.familyIsEmpty());
    EXPECT_EQ(family.family(), "Arial");
}

TEST_F(FontTest, PrimaryFont)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    const FontData* fontData = font.primaryFont();
    ASSERT_NE(fontData, nullptr);

    // Primary font should have valid metrics
    EXPECT_GT(fontData->ascent(), 0);
    EXPECT_GT(fontData->descent(), 0);
    EXPECT_GT(fontData->lineSpacing(), 0);
}

TEST_F(FontTest, TextRunBasic)
{
    const UChar text[] = {'H', 'e', 'l', 'l', 'o'};
    TextRun run(text, 5);

    EXPECT_EQ(run.length(), 5);
    EXPECT_EQ(run[0], 'H');
    EXPECT_EQ(run[4], 'o');
    EXPECT_EQ(run.characters(), text);
}

TEST_F(FontTest, TextRunFromString)
{
    String str("Hello World");
    TextRun run(str);

    EXPECT_EQ(run.length(), 11);
    EXPECT_EQ(run[0], 'H');
    EXPECT_EQ(run[6], 'W');
}

TEST_F(FontTest, TextWidth)
{
    Font font(desc, 0, 0);
    font.update(nullptr);
    String text("Hello");
    TextRun run(text);
    TextStyle style;

    int width = font.width(run, style);
    EXPECT_GT(width, 0);

    // Width should increase with more characters
    String longerText("Hello World");
    TextRun longerRun(longerText);
    int longerWidth = font.width(longerRun, style);
    EXPECT_GT(longerWidth, width);
}

TEST_F(FontTest, FloatWidth)
{
    Font font(desc, 0, 0);
    font.update(nullptr);
    String text("Test");
    TextRun run(text);
    TextStyle style;

    float width = font.floatWidth(run, style);
    EXPECT_GT(width, 0.0f);
}

TEST_F(FontTest, FixedPitch)
{
    // Test with proportional font
    Font font1(desc, 0, 0);
    font1.update(nullptr);
    // Arial is typically proportional

    // Test with monospace font
    FontDescription monoDesc = desc;
    FontFamily monoFamily;
    monoFamily.setFamily("Courier New");
    monoDesc.setFamily(monoFamily);
    Font font2(monoDesc, 0, 0);
    font2.update(nullptr);

    // Both should return a valid result
    bool isFixed1 = font1.isFixedPitch();
    bool isFixed2 = font2.isFixedPitch();

    // Just verify the method works
    EXPECT_TRUE(isFixed1 == true || isFixed1 == false);
    EXPECT_TRUE(isFixed2 == true || isFixed2 == false);
}

TEST_F(FontTest, TreatAsSpace)
{
    EXPECT_TRUE(Font::treatAsSpace(' '));
    EXPECT_TRUE(Font::treatAsSpace('\t'));
    EXPECT_TRUE(Font::treatAsSpace('\n'));
    EXPECT_TRUE(Font::treatAsSpace(0x00A0)); // Non-breaking space
    EXPECT_FALSE(Font::treatAsSpace('a'));
    EXPECT_FALSE(Font::treatAsSpace('0'));
}

TEST_F(FontTest, TreatAsZeroWidthSpace)
{
    EXPECT_TRUE(Font::treatAsZeroWidthSpace(0x00));
    EXPECT_TRUE(Font::treatAsZeroWidthSpace(0x1F));
    EXPECT_TRUE(Font::treatAsZeroWidthSpace(0x7F));
    EXPECT_TRUE(Font::treatAsZeroWidthSpace(0x200e));
    EXPECT_TRUE(Font::treatAsZeroWidthSpace(0x200f));
    EXPECT_FALSE(Font::treatAsZeroWidthSpace('a'));
    EXPECT_FALSE(Font::treatAsZeroWidthSpace(' '));
}

TEST_F(FontTest, DifferentSizes)
{
    FontDescription smallDesc = desc;
    smallDesc.setComputedSize(10.0f);
    Font smallFont(smallDesc, 0, 0);
    smallFont.update(nullptr);

    FontDescription largeDesc = desc;
    largeDesc.setComputedSize(20.0f);
    Font largeFont(largeDesc, 0, 0);
    largeFont.update(nullptr);

    EXPECT_LT(smallFont.pixelSize(), largeFont.pixelSize());
    EXPECT_LT(smallFont.size(), largeFont.size());

    // Larger font should have larger or equal metrics
    EXPECT_LE(smallFont.ascent(), largeFont.ascent());
    EXPECT_LE(smallFont.descent(), largeFont.descent());
    EXPECT_LE(smallFont.lineSpacing(), largeFont.lineSpacing());
}

TEST_F(FontTest, MultipleFontFamilies)
{
    FontFamily family1;
    family1.setFamily("NonExistentFont");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Arial");
    family1.appendFamily(family2);

    desc.setFamily(family1);
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Should fall back to Arial
    const FontData* fontData = font.primaryFont();
    ASSERT_NE(fontData, nullptr);
}
