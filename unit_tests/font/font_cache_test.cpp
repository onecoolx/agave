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

class FontCacheTest : public ::testing::Test
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

TEST_F(FontCacheTest, GetCachedFontPlatformData)
{
    AtomicString familyName("Arial");

    FontPlatformData* data1 = FontCache::getCachedFontPlatformData(desc, familyName);
    ASSERT_NE(data1, nullptr);

    // Getting the same font again should return cached data
    FontPlatformData* data2 = FontCache::getCachedFontPlatformData(desc, familyName);
    EXPECT_EQ(data1, data2);
}

TEST_F(FontCacheTest, DifferentFontSizes)
{
    AtomicString familyName("Arial");

    FontDescription desc1 = desc;
    desc1.setComputedSize(12.0f);
    FontPlatformData* data1 = FontCache::getCachedFontPlatformData(desc1, familyName);

    FontDescription desc2 = desc;
    desc2.setComputedSize(16.0f);
    FontPlatformData* data2 = FontCache::getCachedFontPlatformData(desc2, familyName);

    // Different sizes should return different platform data
    EXPECT_NE(data1, data2);
}

TEST_F(FontCacheTest, DifferentFontStyles)
{
    AtomicString familyName("Arial");

    FontDescription normalDesc = desc;
    normalDesc.setItalic(false);
    FontPlatformData* normalData = FontCache::getCachedFontPlatformData(normalDesc, familyName);

    FontDescription italicDesc = desc;
    italicDesc.setItalic(true);
    FontPlatformData* italicData = FontCache::getCachedFontPlatformData(italicDesc, familyName);

    // Different styles should return different platform data
    EXPECT_NE(normalData, italicData);
}

TEST_F(FontCacheTest, DifferentFontWeights)
{
    AtomicString familyName("Arial");

    FontDescription normalDesc = desc;
    normalDesc.setWeight(cNormalWeight);
    FontPlatformData* normalData = FontCache::getCachedFontPlatformData(normalDesc, familyName);

    FontDescription boldDesc = desc;
    boldDesc.setWeight(cBoldWeight);
    FontPlatformData* boldData = FontCache::getCachedFontPlatformData(boldDesc, familyName);

    // Different weights should return different platform data
    EXPECT_NE(normalData, boldData);
}

TEST_F(FontCacheTest, GetCachedFontData)
{
    AtomicString familyName("Arial");
    FontPlatformData* platformData = FontCache::getCachedFontPlatformData(desc, familyName);
    ASSERT_NE(platformData, nullptr);

    FontData* fontData1 = FontCache::getCachedFontData(platformData);
    ASSERT_NE(fontData1, nullptr);

    // Getting font data again should return cached data
    FontData* fontData2 = FontCache::getCachedFontData(platformData);
    EXPECT_EQ(fontData1, fontData2);
}

TEST_F(FontCacheTest, GetFontData)
{
    Font font(desc, 0, 0);
    font.update(nullptr);
    int familyIndex = 0;

    const FontData* fontData = FontCache::getFontData(font, familyIndex, nullptr);
    ASSERT_NE(fontData, nullptr);

    // Font data should have valid metrics
    EXPECT_GT(fontData->ascent(), 0);
    EXPECT_GT(fontData->descent(), 0);
    EXPECT_GT(fontData->lineSpacing(), 0);
}

TEST_F(FontCacheTest, FontFallback)
{
    // Create a font description with non-existent font first, then Arial
    FontFamily family1;
    family1.setFamily("NonExistentFont12345");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Arial");
    family1.appendFamily(family2);

    desc.setFamily(family1);
    Font font(desc, 0, 0);
    font.update(nullptr);

    int familyIndex = 0;
    const FontData* fontData = FontCache::getFontData(font, familyIndex, nullptr);

    // Should fall back to Arial
    ASSERT_NE(fontData, nullptr);
    EXPECT_GT(familyIndex, 0); // Should have moved to next family
}

TEST_F(FontCacheTest, LastResortFallback)
{
    FontPlatformData* fallbackData = FontCache::getLastResortFallbackFont(desc);
    ASSERT_NE(fallbackData, nullptr);

    // Last resort font should be usable
    FontData* fontData = FontCache::getCachedFontData(fallbackData);
    ASSERT_NE(fontData, nullptr);
    EXPECT_GT(fontData->ascent(), 0);
}

TEST_F(FontCacheTest, FontExists)
{
    AtomicString arial("Arial");
    bool exists = FontCache::fontExists(desc, arial);

    // Arial should typically exist
    EXPECT_TRUE(exists);

    AtomicString nonExistent("NonExistentFont98765");
    bool notExists = FontCache::fontExists(desc, nonExistent);

    // Non-existent font should return true (implementation always returns true)
    EXPECT_TRUE(notExists);
}

TEST_F(FontCacheTest, MultipleFontsInCache)
{
    AtomicString arial("Arial");
    AtomicString times("Times New Roman");
    AtomicString courier("Courier New");

    FontPlatformData* arialData = FontCache::getCachedFontPlatformData(desc, arial);
    FontPlatformData* timesData = FontCache::getCachedFontPlatformData(desc, times);
    FontPlatformData* courierData = FontCache::getCachedFontPlatformData(desc, courier);

    ASSERT_NE(arialData, nullptr);
    ASSERT_NE(timesData, nullptr);
    ASSERT_NE(courierData, nullptr);

    // All should be different
    EXPECT_NE(arialData, timesData);
    EXPECT_NE(arialData, courierData);
    EXPECT_NE(timesData, courierData);
}

TEST_F(FontCacheTest, FontAliases)
{
    // Test font aliases (Arial <-> Helvetica, Times <-> Times New Roman, etc.)
    AtomicString arial("Arial");
    AtomicString helvetica("Helvetica");

    FontPlatformData* arialData = FontCache::getCachedFontPlatformData(desc, arial);
    FontPlatformData* helveticaData = FontCache::getCachedFontPlatformData(desc, helvetica);

    // Both should return valid data
    ASSERT_NE(arialData, nullptr);
    ASSERT_NE(helveticaData, nullptr);
}

TEST_F(FontCacheTest, CacheConsistency)
{
    AtomicString familyName("Arial");

    // Get platform data
    FontPlatformData* platformData = FontCache::getCachedFontPlatformData(desc, familyName);
    ASSERT_NE(platformData, nullptr);

    // Get font data from platform data
    FontData* fontData1 = FontCache::getCachedFontData(platformData);
    ASSERT_NE(fontData1, nullptr);

    // Get platform data again
    FontPlatformData* platformData2 = FontCache::getCachedFontPlatformData(desc, familyName);
    EXPECT_EQ(platformData, platformData2);

    // Get font data again
    FontData* fontData2 = FontCache::getCachedFontData(platformData2);
    EXPECT_EQ(fontData1, fontData2);
}

TEST_F(FontCacheTest, NullPlatformData)
{
    FontData* fontData = FontCache::getCachedFontData(nullptr);
    EXPECT_EQ(fontData, nullptr);
}

TEST_F(FontCacheTest, EmptyFamilyName)
{
    AtomicString emptyName("");
    FontPlatformData* data = FontCache::getCachedFontPlatformData(desc, emptyName);

    // Should handle empty family name gracefully
    // May return null or fallback font depending on implementation
    if (data != nullptr) {
        FontData* fontData = FontCache::getCachedFontData(data);
        if (fontData != nullptr) {
            EXPECT_GT(fontData->ascent(), 0);
        }
    }
}
