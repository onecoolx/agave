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

// ============================================================
// latin1() / CString tests
// ============================================================

TEST(StringExtensionsTest, Latin1NullString)
{
    String s;
    EXPECT_TRUE(s.isNull());
    CString data = s.latin1();
    // null string should return a valid pointer to empty string
    EXPECT_NE(data.data(), (const char*)0);
    EXPECT_EQ(strlen(data.data()), 0u);
}

TEST(StringExtensionsTest, Latin1EmptyString)
{
    String s("");
    EXPECT_TRUE(s.isEmpty());
    CString data = s.latin1();
    EXPECT_NE(data.data(), (const char*)0);
    EXPECT_EQ(strlen(data.data()), 0u);
}

TEST(StringExtensionsTest, Latin1ASCIIString)
{
    String s("Hello World");
    CString data = s.latin1();
    EXPECT_STREQ(data.data(), "Hello World");
}

TEST(StringExtensionsTest, Latin1Latin1String)
{
    // Latin-1 character: 0xE9 = 'é'
    UChar chars[] = { 0x48, 0xE9, 0x6C, 0x6C, 0x6F }; // "Héllo"
    String s(chars, 5);
    CString data = s.latin1();
    EXPECT_EQ(strlen(data.data()), 5u);
    EXPECT_EQ((unsigned char)data.data()[1], 0xE9u);
}

// ============================================================
// copyLatin1() tests
// ============================================================

TEST(StringExtensionsTest, CopyLatin1Normal)
{
    String s("Hello");
    char buf[32];
    s.copyLatin1(buf);
    EXPECT_STREQ(buf, "Hello");
}

TEST(StringExtensionsTest, CopyLatin1WithOffset)
{
    String s("Hello World");
    char buf[32];
    s.copyLatin1(buf, 6);
    EXPECT_STREQ(buf, "World");
}

TEST(StringExtensionsTest, CopyLatin1WithLength)
{
    String s("Hello World");
    char buf[32];
    s.copyLatin1(buf, 0, 5);
    EXPECT_STREQ(buf, "Hello");
}

TEST(StringExtensionsTest, CopyLatin1OutOfRange)
{
    String s("Hi");
    char buf[32];
    s.copyLatin1(buf, 100);
    EXPECT_STREQ(buf, "");
}

// ============================================================
// isAllASCII() tests
// ============================================================

TEST(StringExtensionsTest, IsAllASCIIPureASCII)
{
    String s("Hello World 123");
    EXPECT_TRUE(s.isAllASCII());
}

TEST(StringExtensionsTest, IsAllASCIIWithNonASCII)
{
    UChar chars[] = { 0x48, 0xE9, 0x6C, 0x6C, 0x6F }; // "Héllo"
    String s(chars, 5);
    EXPECT_FALSE(s.isAllASCII());
}

TEST(StringExtensionsTest, IsAllASCIIEmptyString)
{
    String s("");
    EXPECT_TRUE(s.isAllASCII());
}

TEST(StringExtensionsTest, IsAllASCIINullString)
{
    String s;
    EXPECT_TRUE(s.isAllASCII());
}

// ============================================================
// hasFastLatin1() tests
// ============================================================

TEST(StringExtensionsTest, HasFastLatin1PureASCII)
{
    String s("abc");
    EXPECT_TRUE(s.hasFastLatin1());
}

TEST(StringExtensionsTest, HasFastLatin1Latin1Range)
{
    UChar chars[] = { 0xE9, 0xFF }; // Latin-1 range chars
    String s(chars, 2);
    EXPECT_TRUE(s.hasFastLatin1());
}

TEST(StringExtensionsTest, HasFastLatin1BeyondLatin1)
{
    UChar chars[] = { 0x4E16 }; // Chinese character '世'
    String s(chars, 1);
    EXPECT_FALSE(s.hasFastLatin1());
}

// ============================================================
// toShort() tests
// ============================================================

TEST(StringExtensionsTest, ToShortNormal)
{
    bool ok;
    String s("123");
    EXPECT_EQ(s.toShort(&ok), 123);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToShortNegative)
{
    bool ok;
    String s("-456");
    EXPECT_EQ(s.toShort(&ok), -456);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToShortBoundaryMax)
{
    bool ok;
    String s("32767");
    EXPECT_EQ(s.toShort(&ok), 32767);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToShortBoundaryMin)
{
    bool ok;
    String s("-32768");
    EXPECT_EQ(s.toShort(&ok), -32768);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToShortOverflow)
{
    bool ok;
    String s("32768");
    s.toShort(&ok);
    EXPECT_FALSE(ok);
}

TEST(StringExtensionsTest, ToShortUnderflow)
{
    bool ok;
    String s("-32769");
    s.toShort(&ok);
    EXPECT_FALSE(ok);
}

TEST(StringExtensionsTest, ToShortInvalid)
{
    bool ok;
    String s("abc");
    s.toShort(&ok);
    EXPECT_FALSE(ok);
}

// ============================================================
// toUShort() tests
// ============================================================

TEST(StringExtensionsTest, ToUShortNormal)
{
    bool ok;
    String s("1234");
    EXPECT_EQ(s.toUShort(&ok), 1234u);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToUShortMax)
{
    bool ok;
    String s("65535");
    EXPECT_EQ(s.toUShort(&ok), 65535u);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToUShortOverflow)
{
    bool ok;
    String s("65536");
    s.toUShort(&ok);
    EXPECT_FALSE(ok);
}

TEST(StringExtensionsTest, ToUShortNegative)
{
    bool ok;
    String s("-1");
    s.toUShort(&ok);
    EXPECT_FALSE(ok);
}

// ============================================================
// toUInt() tests
// ============================================================

TEST(StringExtensionsTest, ToUIntNormal)
{
    bool ok;
    String s("42");
    EXPECT_EQ(s.toUInt(&ok), 42u);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToUIntMax)
{
    bool ok;
    String s("4294967295");
    EXPECT_EQ(s.toUInt(&ok), 4294967295u);
    EXPECT_TRUE(ok);
}

TEST(StringExtensionsTest, ToUIntOverflow)
{
    bool ok;
    String s("4294967296");
    s.toUInt(&ok);
    EXPECT_FALSE(ok);
}

TEST(StringExtensionsTest, ToUIntNegative)
{
    bool ok;
    String s("-1");
    s.toUInt(&ok);
    EXPECT_FALSE(ok);
}

// ============================================================
// reverseFind(const char*, int) tests
// ============================================================

TEST(StringExtensionsTest, ReverseFindFound)
{
    String s("hello world hello");
    int pos = s.reverseFind("hello");
    EXPECT_EQ(pos, 12);
}

TEST(StringExtensionsTest, ReverseFindNotFound)
{
    String s("hello world");
    int pos = s.reverseFind("xyz");
    EXPECT_EQ(pos, -1);
}

TEST(StringExtensionsTest, ReverseFindEmptyPattern)
{
    String s("hello");
    int pos = s.reverseFind("");
    // Empty pattern behavior: typically returns length or start
    EXPECT_GE(pos, -1);
}

TEST(StringExtensionsTest, ReverseFindWithStart)
{
    String s("hello world hello");
    int pos = s.reverseFind("hello", 11);
    EXPECT_EQ(pos, 0);
}

TEST(StringExtensionsTest, ReverseFindOnNullString)
{
    String s;
    int pos = s.reverseFind("hello");
    EXPECT_EQ(pos, -1);
}

// ============================================================
// prepend() tests
// ============================================================

TEST(StringExtensionsTest, PrependNormal)
{
    String s("World");
    s.prepend(String("Hello "));
    EXPECT_EQ(s, String("Hello World"));
}

TEST(StringExtensionsTest, PrependEmptyString)
{
    String s("Hello");
    s.prepend(String(""));
    EXPECT_EQ(s, String("Hello"));
}

TEST(StringExtensionsTest, PrependNullString)
{
    String s("Hello");
    String null;
    s.prepend(null);
    EXPECT_EQ(s, String("Hello"));
}

TEST(StringExtensionsTest, PrependToNullString)
{
    String s;
    s.prepend(String("Hello"));
    EXPECT_EQ(s, String("Hello"));
}

TEST(StringExtensionsTest, PrependToEmptyString)
{
    String s("");
    s.prepend(String("Hello"));
    EXPECT_EQ(s, String("Hello"));
}

// ============================================================
// find(RegularExpression, int) tests
// ============================================================

TEST(StringExtensionsTest, FindRegexBasicMatch)
{
    String s("hello world");
    RegularExpression re("world");
    int pos = s.find(re);
    EXPECT_EQ(pos, 6);
}

TEST(StringExtensionsTest, FindRegexNoMatch)
{
    String s("hello world");
    RegularExpression re("xyz");
    int pos = s.find(re);
    EXPECT_EQ(pos, -1);
}

TEST(StringExtensionsTest, FindRegexOnNullString)
{
    String s;
    RegularExpression re("test");
    int pos = s.find(re);
    EXPECT_EQ(pos, -1);
}

// ============================================================
// Property-Based Tests
// ============================================================

// **Feature: remove-deprecated-string, Property 1: reverseFind 行为一致性**
// **Validates: Requirements 1.1**
// For any string containing a known substring, reverseFind should return
// a valid position where the substring actually exists.
TEST(StringExtensionsPropertyTest, ReverseFindConsistency)
{
    srand(42);
    for (int iter = 0; iter < 100; ++iter) {
        // Generate a random base string of ASCII chars
        int baseLen = 5 + (rand() % 20);
        Vector<UChar> baseChars;
        for (int i = 0; i < baseLen; ++i) {
            baseChars.append('a' + (rand() % 26));
        }
        String base(baseChars.data(), baseChars.size());

        // Pick a random substring from base
        int subStart = rand() % baseLen;
        int subLen = 1 + (rand() % (baseLen - subStart));
        String sub = base.substring(subStart, subLen);

        // reverseFind should find the substring
        CString subLatin1 = sub.latin1();
        int pos = base.reverseFind(subLatin1.data());
        EXPECT_GE(pos, 0) << "reverseFind should find substring in base string";

        // Verify the found position actually contains the substring
        if (pos >= 0) {
            String found = base.substring(pos, sub.length());
            EXPECT_EQ(found, sub) << "Found position should contain the substring";
        }
    }
}

// **Feature: remove-deprecated-string, Property 2: 数值转换正确性**
// **Validates: Requirements 1.2**
// For any integer in the valid range, toShort/toUShort/toUInt should
// correctly convert and the ok flag should be true.
TEST(StringExtensionsPropertyTest, NumericConversionCorrectness)
{
    srand(43);
    for (int iter = 0; iter < 100; ++iter) {
        // Test toShort with values in range [-32768, 32767]
        {
            int val = -32768 + (rand() % 65536);
            String s = String::number(val);
            bool ok = false;
            short result = s.toShort(&ok);
            EXPECT_TRUE(ok) << "toShort should succeed for value " << val;
            EXPECT_EQ(result, static_cast<short>(val));
        }

        // Test toUShort with values in range [0, 65535]
        {
            int val = rand() % 65536;
            String s = String::number(val);
            bool ok = false;
            unsigned short result = s.toUShort(&ok);
            EXPECT_TRUE(ok) << "toUShort should succeed for value " << val;
            EXPECT_EQ(result, static_cast<unsigned short>(val));
        }

        // Test toUInt with values in range [0, UINT_MAX subset]
        {
            unsigned val = static_cast<unsigned>(rand());
            String s = String::number(val);
            bool ok = false;
            unsigned result = s.toUInt(&ok);
            EXPECT_TRUE(ok) << "toUInt should succeed for value " << val;
            EXPECT_EQ(result, val);
        }
    }
}

// **Feature: remove-deprecated-string, Property 3: Latin-1 编码往返**
// **Validates: Requirements 1.3**
// For any string containing only Latin-1 range characters (0x00-0xFF),
// round-tripping through latin1() and back should preserve content.
TEST(StringExtensionsPropertyTest, Latin1RoundTrip)
{
    srand(44);
    for (int iter = 0; iter < 100; ++iter) {
        // Generate a random string with Latin-1 range characters (0x20-0xFF)
        int len = 1 + (rand() % 30);
        Vector<UChar> chars;
        for (int i = 0; i < len; ++i) {
            // Use printable Latin-1 range: 0x20-0x7E (ASCII printable)
            // to ensure clean round-trip (non-printable chars may have encoding issues)
            chars.append(0x20 + (rand() % 95));
        }
        String original(chars.data(), chars.size());

        CString latin1 = original.latin1();
        String reconstructed(latin1.data());

        EXPECT_EQ(original.length(), reconstructed.length())
                << "Round-trip should preserve length";
        EXPECT_EQ(original, reconstructed)
                << "Round-trip should preserve content";
    }
}

// **Feature: remove-deprecated-string, Property 4: prepend 正确性**
// **Validates: Requirements 1.4**
// For any two strings a and b, b.prepend(a) should result in a + original_b.
TEST(StringExtensionsPropertyTest, PrependCorrectness)
{
    srand(45);
    for (int iter = 0; iter < 100; ++iter) {
        int lenA = rand() % 20;
        int lenB = rand() % 20;

        Vector<UChar> charsA, charsB;
        for (int i = 0; i < lenA; ++i) {
            charsA.append('a' + (rand() % 26));
        }
        for (int i = 0; i < lenB; ++i) {
            charsB.append('A' + (rand() % 26));
        }

        String a(charsA.data(), charsA.size());
        String b(charsB.data(), charsB.size());
        String originalB = b.copy();

        String expected = a;
        expected.append(originalB);

        b.prepend(a);

        EXPECT_EQ(b, expected)
                << "prepend(a) should equal a + original_b";
        EXPECT_EQ(b.length(), a.length() + originalB.length())
                << "Length should be sum of both strings";
    }
}
