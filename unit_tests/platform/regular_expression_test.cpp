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
#include "RegularExpression.h"

// ============================================================
// Basic matching
// ============================================================

TEST(RegularExpressionTest, SimpleMatch)
{
    RegularExpression re("hello");
    int matchLen = 0;
    int pos = re.match(String("say hello world"), 0, &matchLen);
    EXPECT_EQ(pos, 4);
    EXPECT_EQ(matchLen, 5);
}

TEST(RegularExpressionTest, CharacterClass)
{
    RegularExpression re("[0-9]+");
    int matchLen = 0;
    int pos = re.match(String("abc123def"), 0, &matchLen);
    EXPECT_EQ(pos, 3);
    EXPECT_EQ(matchLen, 3);
}

TEST(RegularExpressionTest, Quantifiers)
{
    RegularExpression re("a+b");
    int matchLen = 0;
    int pos = re.match(String("xaaaby"), 0, &matchLen);
    EXPECT_EQ(pos, 1);
    EXPECT_EQ(matchLen, 4); // "aaab"
}

TEST(RegularExpressionTest, DotWildcard)
{
    RegularExpression re("a.c");
    int matchLen = 0;
    int pos = re.match(String("xabcy"), 0, &matchLen);
    EXPECT_EQ(pos, 1);
    EXPECT_EQ(matchLen, 3);
}

// ============================================================
// Search and reverse search
// ============================================================

TEST(RegularExpressionTest, SearchFromStart)
{
    RegularExpression re("world");
    int pos = re.search(String("hello world"), 0);
    EXPECT_EQ(pos, 6);
}

TEST(RegularExpressionTest, SearchFromOffset)
{
    RegularExpression re("[a-z]+");
    int pos = re.search(String("123abc456def"), 6);
    EXPECT_EQ(pos, 9); // "def"
}

TEST(RegularExpressionTest, SearchRev)
{
    RegularExpression re("ab");
    int pos = re.searchRev(String("ab cd ab ef"));
    EXPECT_EQ(pos, 6); // last "ab"
}

// ============================================================
// Case sensitivity
// ============================================================

TEST(RegularExpressionTest, CaseSensitiveMatch)
{
    RegularExpression re("Hello", true); // caseSensitive = true
    int pos = re.match(String("hello Hello"), 0, 0);
    EXPECT_EQ(pos, 6);
}

TEST(RegularExpressionTest, CaseInsensitiveMatch)
{
    RegularExpression re("hello", false); // caseSensitive = false
    int pos = re.match(String("HELLO world"), 0, 0);
    EXPECT_EQ(pos, 0);
}

// ============================================================
// Capture groups
// ============================================================

TEST(RegularExpressionTest, CaptureGroup)
{
    RegularExpression re("(hello)");
    int matchLen = 0;
    int pos = re.match(String("say hello"), 0, &matchLen);
    EXPECT_EQ(pos, 4);
    EXPECT_EQ(matchLen, 5);
    String captured = re.cap(0);
    EXPECT_EQ(captured, String("hello"));
}

// ============================================================
// No match
// ============================================================

TEST(RegularExpressionTest, NoMatch)
{
    RegularExpression re("xyz");
    int matchLen = 0;
    int pos = re.match(String("hello world"), 0, &matchLen);
    EXPECT_EQ(pos, -1);
}

TEST(RegularExpressionTest, NoMatchEmptyString)
{
    RegularExpression re("abc");
    int pos = re.match(String(""), 0, 0);
    EXPECT_EQ(pos, -1);
}

// ============================================================
// Pattern and matchedLength
// ============================================================

TEST(RegularExpressionTest, PatternRetrieval)
{
    RegularExpression re("test.*pattern");
    EXPECT_EQ(re.pattern(), String("test.*pattern"));
}

TEST(RegularExpressionTest, MatchedLength)
{
    RegularExpression re("abc");
    re.match(String("xabcx"), 0, 0);
    EXPECT_EQ(re.matchedLength(), 3);
}

// ============================================================
// Glob patterns
// ============================================================

TEST(RegularExpressionTest, GlobStar)
{
    RegularExpression re("*.txt", false, true); // glob mode
    int matchLen = 0;
    int pos = re.match(String("readme.txt"), 0, &matchLen);
    EXPECT_EQ(pos, 0);
    EXPECT_EQ(matchLen, 10);
}

TEST(RegularExpressionTest, GlobQuestion)
{
    RegularExpression re("a?c", false, true); // glob mode
    int matchLen = 0;
    int pos = re.match(String("abc"), 0, &matchLen);
    EXPECT_EQ(pos, 0);
    EXPECT_EQ(matchLen, 3);
}

TEST(RegularExpressionTest, GlobNoMatch)
{
    RegularExpression re("*.cpp", false, true);
    int pos = re.match(String("readme.txt"), 0, 0);
    EXPECT_EQ(pos, -1);
}

// ============================================================
// Copy and assignment
// ============================================================

TEST(RegularExpressionTest, CopyConstructor)
{
    RegularExpression re1("hello");
    RegularExpression re2(re1);
    EXPECT_EQ(re2.pattern(), String("hello"));
    int pos = re2.match(String("say hello"), 0, 0);
    EXPECT_EQ(pos, 4);
}

TEST(RegularExpressionTest, Assignment)
{
    RegularExpression re1("world");
    RegularExpression re2;
    re2 = re1;
    EXPECT_EQ(re2.pattern(), String("world"));
    int pos = re2.match(String("hello world"), 0, 0);
    EXPECT_EQ(pos, 6);
}

// **Validates: Requirements 3.4, 9.3**
