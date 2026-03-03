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
// Character output
// ============================================================

TEST(TextStreamTest, OutputChar)
{
    String result;
    TextStream ts(&result);
    ts << 'A';
    EXPECT_EQ(result, String("A"));
}

// ============================================================
// Integer types output
// ============================================================

TEST(TextStreamTest, OutputShort)
{
    String result;
    TextStream ts(&result);
    ts << static_cast<short>(42);
    EXPECT_EQ(result, String("42"));
}

TEST(TextStreamTest, OutputShortNegative)
{
    String result;
    TextStream ts(&result);
    ts << static_cast<short>(-100);
    EXPECT_EQ(result, String("-100"));
}

TEST(TextStreamTest, OutputUnsignedShort)
{
    String result;
    TextStream ts(&result);
    ts << static_cast<unsigned short>(65535);
    EXPECT_EQ(result, String("65535"));
}

TEST(TextStreamTest, OutputInt)
{
    String result;
    TextStream ts(&result);
    ts << 12345;
    EXPECT_EQ(result, String("12345"));
}

TEST(TextStreamTest, OutputIntNegative)
{
    String result;
    TextStream ts(&result);
    ts << -9876;
    EXPECT_EQ(result, String("-9876"));
}

TEST(TextStreamTest, OutputUnsigned)
{
    String result;
    TextStream ts(&result);
    ts << 4000000000u;
    EXPECT_EQ(result, String("4000000000"));
}

TEST(TextStreamTest, OutputLong)
{
    String result;
    TextStream ts(&result);
    ts << 123456L;
    EXPECT_EQ(result, String("123456"));
}

TEST(TextStreamTest, OutputUnsignedLong)
{
    String result;
    TextStream ts(&result);
    ts << 999999UL;
    EXPECT_EQ(result, String("999999"));
}

// ============================================================
// Floating point output
// ============================================================

TEST(TextStreamTest, OutputFloat)
{
    String result;
    TextStream ts(&result);
    ts << 3.14f;
    // Default precision is 6
    EXPECT_TRUE(result.contains("3.14"));
}

TEST(TextStreamTest, OutputDouble)
{
    String result;
    TextStream ts(&result);
    ts << 2.71828;
    EXPECT_TRUE(result.contains("2.71"));
}

// ============================================================
// String output
// ============================================================

TEST(TextStreamTest, OutputCString)
{
    String result;
    TextStream ts(&result);
    ts << "hello world";
    EXPECT_EQ(result, String("hello world"));
}

TEST(TextStreamTest, OutputString)
{
    String result;
    TextStream ts(&result);
    String input("WebCore::String");
    ts << input;
    EXPECT_EQ(result, String("WebCore::String"));
}

// ============================================================
// Pointer output
// ============================================================

TEST(TextStreamTest, OutputPointer)
{
    String result;
    TextStream ts(&result);
    void* ptr = reinterpret_cast<void*>(0x1234);
    ts << ptr;
    EXPECT_FALSE(result.isEmpty());
    // Pointer format is platform-dependent, just check it's not empty
}

// ============================================================
// Precision control
// ============================================================

TEST(TextStreamTest, PrecisionControl)
{
    String result;
    TextStream ts(&result);
    int oldPrec = ts.precision(2);
    EXPECT_EQ(oldPrec, 6); // default precision
    ts << 3.14159;
    // With precision 2, should output "3.14"
    EXPECT_EQ(result, String("3.14"));
}

TEST(TextStreamTest, PrecisionZero)
{
    String result;
    TextStream ts(&result);
    ts.precision(0);
    ts << 3.7;
    EXPECT_EQ(result, String("4")); // rounded
}

// ============================================================
// endl manipulator
// ============================================================

TEST(TextStreamTest, EndlManipulator)
{
    String result;
    TextStream ts(&result);
    ts << "line1" << endl << "line2";
    EXPECT_TRUE(result.contains("\n"));
    EXPECT_EQ(result, String("line1\nline2"));
}

// ============================================================
// Chained output
// ============================================================

TEST(TextStreamTest, ChainedOutput)
{
    String result;
    TextStream ts(&result);
    ts << "x=" << 42 << " y=" << 3.14f;
    EXPECT_TRUE(result.startsWith("x=42 y=3.14"));
}

// ============================================================
// Property-Based Tests
// ============================================================

// **Feature: remove-deprecated-string, Property 6: TextStream 格式化输出正确性**
// **Validates: Requirements 4.2, 4.3, 9.4**
// For any integer n, TextStream << n should produce a String containing
// the decimal representation of n.
TEST(TextStreamPropertyTest, IntegerFormatCorrectness)
{
    srand(47);
    for (int iter = 0; iter < 100; ++iter) {
        int value = (rand() % 200000) - 100000;

        String result;
        TextStream ts(&result);
        ts << value;

        // Verify by converting back
        char expected[32];
        sprintf(expected, "%d", value);
        EXPECT_EQ(result, String(expected))
                << "TextStream << " << value << " should produce \"" << expected << "\"";
    }
}
