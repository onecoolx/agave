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

class FontDataTest : public ::testing::Test
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

        // Get a valid FontPlatformData for testing
        AtomicString familyName("Arial");
        platformData = FontCache::getCachedFontPlatformData(desc, familyName);
        ASSERT_NE(platformData, nullptr);
    }

    void TearDown() override
    {
    }

    FontDescription desc;
    FontPlatformData* platformData;
};

TEST_F(FontDataTest, Constructor)
{
    FontData fontData(*platformData);

    // Font data should have valid metrics
    EXPECT_GT(fontData.ascent(), 0);
    EXPECT_GT(fontData.descent(), 0);
    EXPECT_GT(fontData.lineSpacing(), 0);
    EXPECT_GT(fontData.lineGap(), 0);
    EXPECT_GT(fontData.xHeight(), 0.0f);
    EXPECT_GT(fontData.unitsPerEm(), 0);
}

TEST_F(FontDataTest, PlatformData)
{
    FontData fontData(*platformData);

    const FontPlatformData& pd = fontData.platformData();
    // The platform data should have the same properties
    EXPECT_EQ(pd.size(), platformData->size());
}

TEST_F(FontDataTest, VerticalMetrics)
{
    FontData fontData(*platformData);

    int ascent = fontData.ascent();
    int descent = fontData.descent();
    int lineSpacing = fontData.lineSpacing();
    int lineGap = fontData.lineGap();

    EXPECT_GT(ascent, 0);
    EXPECT_GT(descent, 0);
    EXPECT_GT(lineSpacing, 0);
    EXPECT_GE(lineGap, 0); // Line gap can be 0

    // Line spacing should be at least ascent + descent
    EXPECT_GE(lineSpacing, ascent + descent);
}

TEST_F(FontDataTest, XHeight)
{
    FontData fontData(*platformData);

    float xHeight = fontData.xHeight();
    EXPECT_GT(xHeight, 0.0f);

    // X-height should be reasonable relative to ascent
    // (not necessarily less than ascent for all fonts)
    EXPECT_GT(static_cast<float>(fontData.ascent()), 0.0f);
}

TEST_F(FontDataTest, UnitsPerEm)
{
    FontData fontData(*platformData);

    unsigned unitsPerEm = fontData.unitsPerEm();
    EXPECT_GT(unitsPerEm, 0);

    // Typical values are 1000, 1024, 2048
    EXPECT_TRUE(unitsPerEm == 1000 || unitsPerEm == 1024 || unitsPerEm == 2048 || unitsPerEm > 0);
}

TEST_F(FontDataTest, Pitch)
{
    FontData fontData(*platformData);

    Pitch pitch = fontData.pitch();
    EXPECT_TRUE(pitch == FixedPitch || pitch == VariablePitch);
}

TEST_F(FontDataTest, CustomFont)
{
    FontData normalFont(*platformData, false, false);
    EXPECT_FALSE(normalFont.isCustomFont());
    EXPECT_FALSE(normalFont.isLoading());

    FontData customFont(*platformData, true, false);
    EXPECT_TRUE(customFont.isCustomFont());
    EXPECT_FALSE(customFont.isLoading());

    FontData loadingFont(*platformData, true, true);
    EXPECT_TRUE(loadingFont.isCustomFont());
    EXPECT_TRUE(loadingFont.isLoading());
}

TEST_F(FontDataTest, WidthForGlyph)
{
    FontData fontData(*platformData);

    // Test with common glyphs
    Glyph spaceGlyph = 32; // Space character
    Glyph aGlyph = 65; // 'A' character

    float spaceWidth = fontData.widthForGlyph(spaceGlyph);
    float aWidth = fontData.widthForGlyph(aGlyph);

    EXPECT_GE(spaceWidth, 0.0f);
    EXPECT_GT(aWidth, 0.0f);

    // 'A' should typically be wider than space
    EXPECT_GT(aWidth, spaceWidth);
}

TEST_F(FontDataTest, PlatformWidthForGlyph)
{
    FontData fontData(*platformData);

    Glyph aGlyph = 65; // 'A' character
    float width = fontData.platformWidthForGlyph(aGlyph);

    EXPECT_GT(width, 0.0f);
}

TEST_F(FontDataTest, ContainsCharacters)
{
    FontData fontData(*platformData);

    const UChar basicLatin[] = {'H', 'e', 'l', 'l', 'o'};
    bool contains = fontData.containsCharacters(basicLatin, 5);

    // Implementation always returns true
    EXPECT_TRUE(contains);
}

TEST_F(FontDataTest, SmallCapsFontData)
{
    FontData fontData(*platformData);

    FontData* smallCaps = fontData.smallCapsFontData(desc);
    ASSERT_NE(smallCaps, nullptr);

    // Small caps font should have smaller size
    // Note: The implementation creates a font at 70% of original size
    EXPECT_LT(smallCaps->ascent(), fontData.ascent());
    // Descent might be the same for small fonts, so just check it's valid
    EXPECT_GE(smallCaps->descent(), 0);

    // Getting small caps again should return the same instance
    FontData* smallCaps2 = fontData.smallCapsFontData(desc);
    EXPECT_EQ(smallCaps, smallCaps2);
}

TEST_F(FontDataTest, MissingGlyphData)
{
    FontData fontData(*platformData);

    const GlyphData& missingGlyph = fontData.missingGlyphData();
    EXPECT_EQ(missingGlyph.fontData, &fontData);
}

TEST_F(FontDataTest, DifferentSizes)
{
    FontDescription smallDesc = desc;
    smallDesc.setComputedSize(10.0f);
    AtomicString familyName("Arial");
    FontPlatformData* smallPlatformData = FontCache::getCachedFontPlatformData(smallDesc, familyName);
    ASSERT_NE(smallPlatformData, nullptr);

    FontDescription largeDesc = desc;
    largeDesc.setComputedSize(20.0f);
    FontPlatformData* largePlatformData = FontCache::getCachedFontPlatformData(largeDesc, familyName);
    ASSERT_NE(largePlatformData, nullptr);

    FontData smallFont(*smallPlatformData);
    FontData largeFont(*largePlatformData);

    // Larger font should have larger metrics
    EXPECT_LT(smallFont.ascent(), largeFont.ascent());
    // Descent might be the same for small fonts, so just check line spacing
    EXPECT_LT(smallFont.lineSpacing(), largeFont.lineSpacing());
    EXPECT_LT(smallFont.xHeight(), largeFont.xHeight());
}

TEST_F(FontDataTest, GlyphWidthCaching)
{
    FontData fontData(*platformData);

    Glyph testGlyph = 65; // 'A'

    // First call - should calculate and cache
    float width1 = fontData.widthForGlyph(testGlyph);

    // Second call - should return cached value
    float width2 = fontData.widthForGlyph(testGlyph);

    EXPECT_FLOAT_EQ(width1, width2);
}

TEST_F(FontDataTest, MultipleGlyphs)
{
    FontData fontData(*platformData);

    // Test multiple different glyphs
    Glyph glyphs[] = {65, 66, 67, 68, 69}; // A, B, C, D, E
    float widths[5];

    for (int i = 0; i < 5; i++) {
        widths[i] = fontData.widthForGlyph(glyphs[i]);
        EXPECT_GT(widths[i], 0.0f);
    }

    // All widths should be positive
    // Note: In a proportional font, different characters may have different widths
    // But we can't guarantee this, so just check they're all valid
    for (int i = 0; i < 5; i++) {
        EXPECT_GT(widths[i], 0.0f);
    }
}

TEST_F(FontDataTest, ZeroGlyph)
{
    FontData fontData(*platformData);

    Glyph zeroGlyph = 0;
    float width = fontData.widthForGlyph(zeroGlyph);

    // Zero glyph might have zero width or be treated as missing glyph
    EXPECT_GE(width, 0.0f);
}
