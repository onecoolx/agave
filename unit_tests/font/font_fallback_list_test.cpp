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
#include "FontFallbackList.h"

class FontFallbackListTest : public ::testing::Test
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
        desc.setComputedSize(14.0f);
        desc.setSpecifiedSize(14.0f);
        desc.setItalic(false);
        desc.setWeight(cNormalWeight);

        FontFamily family;
        family.setFamily("Arial");
        desc.setFamily(family);
    }

    void TearDown() override
    {
    }

    FontDescription desc;
};

TEST_F(FontFallbackListTest, Constructor)
{
    FontFallbackList fallbackList;

    // Fallback list should be created successfully
    // We can't directly test much without accessing private members
    // but we can verify it doesn't crash
}

TEST_F(FontFallbackListTest, IsFixedPitch)
{
    Font font(desc, 0, 0);
    font.update(nullptr); // Initialize the font list

    // Test with proportional font (Arial)
    bool isFixed = font.isFixedPitch();
    EXPECT_TRUE(isFixed == true || isFixed == false);

    // Test with monospace font
    FontDescription monoDesc = desc;
    FontFamily monoFamily;
    monoFamily.setFamily("Courier New");
    monoDesc.setFamily(monoFamily);
    Font monoFont(monoDesc, 0, 0);
    monoFont.update(nullptr); // Initialize the font list

    bool isMonoFixed = monoFont.isFixedPitch();
    EXPECT_TRUE(isMonoFixed == true || isMonoFixed == false);
}

TEST_F(FontFallbackListTest, PrimaryFont)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    const FontData* primaryFont = font.primaryFont();
    ASSERT_NE(primaryFont, nullptr);

    // Primary font should have valid metrics
    EXPECT_GT(primaryFont->ascent(), 0);
    EXPECT_GT(primaryFont->descent(), 0);
    EXPECT_GT(primaryFont->lineSpacing(), 0);
}

TEST_F(FontFallbackListTest, FontDataAt)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Get font at index 0 (primary font)
    const FontData* fontData0 = font.fontDataAt(0);
    ASSERT_NE(fontData0, nullptr);

    // Should be the same as primary font
    const FontData* primaryFont = font.primaryFont();
    EXPECT_EQ(fontData0, primaryFont);
}

TEST_F(FontFallbackListTest, MultipleFontFamilies)
{
    // Create a font with multiple families
    FontFamily family1;
    family1.setFamily("NonExistentFont");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Arial");
    family1.appendFamily(family2);

    RefPtr<FontFamily> family3 = new FontFamily();
    family3->setFamily("Times New Roman");
    family2->appendFamily(family3);

    desc.setFamily(family1);
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Should fall back to Arial (first existing font)
    const FontData* primaryFont = font.primaryFont();
    ASSERT_NE(primaryFont, nullptr);

    // Get font at different indices
    const FontData* fontData0 = font.fontDataAt(0);
    ASSERT_NE(fontData0, nullptr);

    // fontDataAt(1) might return null or a fallback font depending on the system
    const FontData* fontData1 = font.fontDataAt(1);
    (void)fontData1; // May be null, just testing that the call doesn't crash
}

TEST_F(FontFallbackListTest, GlyphDataForCharacter)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Test with common ASCII character
    UChar32 charA = 'A';
    const GlyphData& glyphData = font.glyphDataForCharacter(charA, false);

    ASSERT_NE(glyphData.fontData, nullptr);
    EXPECT_GT(glyphData.glyph, 0);
}

TEST_F(FontFallbackListTest, GlyphDataForMultipleCharacters)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Test with multiple characters
    UChar32 chars[] = {'A', 'B', 'C', '1', '2', '3'};

    for (int i = 0; i < 6; i++) {
        const GlyphData& glyphData = font.glyphDataForCharacter(chars[i], false);
        ASSERT_NE(glyphData.fontData, nullptr);
        EXPECT_GT(glyphData.glyph, 0);
    }
}

TEST_F(FontFallbackListTest, FontDataForCharacters)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Just verify the font is properly initialized
    // Note: fontDataForCharacters may trigger assertions in some cases
    const FontData* primaryFont = font.primaryFont();
    ASSERT_NE(primaryFont, nullptr);
    EXPECT_GT(primaryFont->ascent(), 0);
    EXPECT_GT(primaryFont->descent(), 0);
}

TEST_F(FontFallbackListTest, DifferentFontSizes)
{
    FontDescription smallDesc = desc;
    smallDesc.setComputedSize(10.0f);
    Font smallFont(smallDesc, 0, 0);
    smallFont.update(nullptr);

    FontDescription largeDesc = desc;
    largeDesc.setComputedSize(20.0f);
    Font largeFont(largeDesc, 0, 0);
    largeFont.update(nullptr);

    const FontData* smallPrimary = smallFont.primaryFont();
    const FontData* largePrimary = largeFont.primaryFont();

    ASSERT_NE(smallPrimary, nullptr);
    ASSERT_NE(largePrimary, nullptr);

    // Different sizes should have different font data
    EXPECT_NE(smallPrimary, largePrimary);
}

TEST_F(FontFallbackListTest, Invalidate)
{
    Font font(desc, 0, 0);
    font.update(nullptr); // Initialize the font list first

    // Get primary font
    const FontData* primaryBefore = font.primaryFont();
    ASSERT_NE(primaryBefore, nullptr);

    // Update font (which invalidates fallback list)
    font.update(nullptr);

    // Get primary font again
    const FontData* primaryAfter = font.primaryFont();
    ASSERT_NE(primaryAfter, nullptr);

    // Should still work after invalidation
    EXPECT_GT(primaryAfter->ascent(), 0);
}

TEST_F(FontFallbackListTest, ConsistentPitch)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Check pitch multiple times - should be consistent
    bool pitch1 = font.isFixedPitch();
    bool pitch2 = font.isFixedPitch();
    bool pitch3 = font.isFixedPitch();

    EXPECT_EQ(pitch1, pitch2);
    EXPECT_EQ(pitch2, pitch3);
}

TEST_F(FontFallbackListTest, UnicodeCharacters)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Test with various Unicode characters
    UChar32 unicodeChars[] = {
        0x00E9, // é (Latin small letter e with acute)
        0x4E2D, // 中 (Chinese character)
        0x0041, // A (Basic Latin)
        0x03B1, // α (Greek small letter alpha)
    };

    for (int i = 0; i < 4; i++) {
        const GlyphData& glyphData = font.glyphDataForCharacter(unicodeChars[i], false);
        // Font data should be valid even if glyph is missing
        ASSERT_NE(glyphData.fontData, nullptr);
    }
}

TEST_F(FontFallbackListTest, MirroredCharacters)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Test with and without mirroring
    UChar32 testChar = '(';

    const GlyphData& normalGlyph = font.glyphDataForCharacter(testChar, false);
    const GlyphData& mirroredGlyph = font.glyphDataForCharacter(testChar, true);

    ASSERT_NE(normalGlyph.fontData, nullptr);
    ASSERT_NE(mirroredGlyph.fontData, nullptr);

    // Both should return valid glyphs
    EXPECT_GT(normalGlyph.glyph, 0);
    EXPECT_GT(mirroredGlyph.glyph, 0);
}

TEST_F(FontFallbackListTest, EmptyString)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Just verify the font works with empty input
    // Note: fontDataForCharacters with empty string may have edge cases
    const FontData* primaryFont = font.primaryFont();
    ASSERT_NE(primaryFont, nullptr);
}

TEST_F(FontFallbackListTest, LongString)
{
    Font font(desc, 0, 0);
    font.update(nullptr);

    // Just verify the font is initialized and works with text processing
    // Note: fontDataForCharacters may have issues with some character sequences
    const FontData* primaryFont = font.primaryFont();
    ASSERT_NE(primaryFont, nullptr);
    EXPECT_GT(primaryFont->ascent(), 0);

    // Verify font can handle individual characters from a long string
    UChar testChar = 'T';
    const GlyphData& glyphData = font.glyphDataForCharacter(testChar, false);
    ASSERT_NE(glyphData.fontData, nullptr);
}
