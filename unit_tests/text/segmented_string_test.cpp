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

TEST(SegmentedStringTest, DefaultCtor)
{
    SegmentedString s;
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.length(), 0u);
    EXPECT_FALSE(s.escaped());
}
TEST(SegmentedStringTest, StringCtor)
{
    SegmentedString s("abc");
    EXPECT_FALSE(s.isEmpty());
    EXPECT_EQ(s.length(), 3u);
    EXPECT_EQ(*s, 'a');
}
TEST(SegmentedStringTest, CopyCtor)
{
    SegmentedString a("xy");
    SegmentedString b(a);
    EXPECT_EQ(b.toString(), "xy");
}
TEST(SegmentedStringTest, Assignment)
{
    SegmentedString a("123");
    SegmentedString b;
    b = a;
    EXPECT_EQ(b.toString(), "123");
}

TEST(SegmentedStringTest, PushAndEscaped)
{
    SegmentedString s("bc");
    s.push('a');
    EXPECT_TRUE(s.escaped());
    EXPECT_EQ(*s, 'a');
    s.advance();
    EXPECT_EQ(*s, 'b');
    s.push('z');
    EXPECT_EQ(*s, 'z');
    s.advance();
    EXPECT_EQ(*s, 'b');
}

TEST(SegmentedStringTest, AdvanceWithLineNumbers)
{
    SegmentedString s("a\nb\nc");
    int line = 0;
    while (!s.isEmpty()) {
        if (*s == '\n') {
            ++line;
        }
        s.advance();
    }
    EXPECT_EQ(line, 2);
}
TEST(SegmentedStringTest, AdvanceExcludeLineNumbers)
{
    SegmentedString s("a\nb");
    s.setExcludeLineNumbers();
    int line = 0;
    while (!s.isEmpty()) {
        s.advance(&line);
    }
    EXPECT_EQ(line, 0);
}

TEST(SegmentedStringTest, AppendSingle)
{
    SegmentedString s("ab");
    s.append(SegmentedString("cd"));
    EXPECT_EQ(s.toString(), "abcd");
}
TEST(SegmentedStringTest, PrependSingle)
{
    SegmentedString s("12");
    s.prepend(SegmentedString("01"));
    EXPECT_EQ(s.toString(), "0112");
}
TEST(SegmentedStringTest, AppendComposite)
{
    SegmentedString s("a");
    s.append(SegmentedString("b"));
    s.append(SegmentedString("c"));
    EXPECT_EQ(s.toString(), "abc");
    EXPECT_GT(s.length(), 2u);
}

TEST(SegmentedStringTest, ToStringWithPushed)
{
    SegmentedString s("bc");
    s.push('a');
    EXPECT_EQ(s.toString(), "abc");
}
TEST(SegmentedStringTest, LengthAccountsForPushed)
{
    SegmentedString s("x");
    s.push('y');
    s.push('z');
    EXPECT_EQ(s.length(), 3u);
}

TEST(SegmentedStringTest, Clear)
{
    SegmentedString s("abc");
    s.push('x');
    s.clear();
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.length(), 0u);
    EXPECT_FALSE(s.escaped());
}

TEST(SegmentedStringTest, Operators)
{
    SegmentedString s("A");
    EXPECT_EQ(*s, 'A');
    EXPECT_EQ(s.operator->(), &(*s));
}

TEST(SegmentedStringTest, AdvanceAcrossSegments)
{
    SegmentedString s("ab");
    s.append(SegmentedString("cd"));
    EXPECT_EQ(*s, 'a');
    s.advance();
    EXPECT_EQ(*s, 'b');
    s.advance();
    EXPECT_EQ(*s, 'c');
    s.advance();
    EXPECT_EQ(*s, 'd');
    s.advance();
    EXPECT_TRUE(s.isEmpty());
}

TEST(SegmentedStringTest, SetExcludeLineNumbersPropagates)
{
    SegmentedString s("a");
    s.append(SegmentedString("b"));
    s.setExcludeLineNumbers();
    int line = 0;
    SegmentedString nl("\n");
    s.append(nl);
    while (!s.isEmpty()) {
        s.advance(&line);
    }
    EXPECT_EQ(line, 1);
}
