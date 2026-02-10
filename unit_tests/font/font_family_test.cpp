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

class FontFamilyTest : public ::testing::Test
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
    }

    void TearDown() override
    {
    }
};

TEST_F(FontFamilyTest, DefaultConstructor)
{
    FontFamily family;

    EXPECT_TRUE(family.familyIsEmpty());
    EXPECT_TRUE(family.family().isEmpty());
    EXPECT_EQ(family.next(), nullptr);
}

TEST_F(FontFamilyTest, SetFamily)
{
    FontFamily family;

    AtomicString arial("Arial");
    family.setFamily(arial);

    EXPECT_FALSE(family.familyIsEmpty());
    EXPECT_EQ(family.family(), arial);
    EXPECT_EQ(family.family(), "Arial");
}

TEST_F(FontFamilyTest, CopyConstructor)
{
    FontFamily family1;
    family1.setFamily("Arial");

    FontFamily family2(family1);

    EXPECT_EQ(family1.family(), family2.family());
    EXPECT_FALSE(family2.familyIsEmpty());
}

TEST_F(FontFamilyTest, AssignmentOperator)
{
    FontFamily family1;
    family1.setFamily("Arial");

    FontFamily family2;
    family2 = family1;

    EXPECT_EQ(family1.family(), family2.family());
    EXPECT_FALSE(family2.familyIsEmpty());
}

TEST_F(FontFamilyTest, EqualityOperator)
{
    FontFamily family1;
    family1.setFamily("Arial");

    FontFamily family2;
    family2.setFamily("Arial");

    FontFamily family3;
    family3.setFamily("Times New Roman");

    EXPECT_TRUE(family1 == family2);
    EXPECT_FALSE(family1 == family3);
    EXPECT_TRUE(family1 != family3);
}

TEST_F(FontFamilyTest, AppendFamily)
{
    FontFamily family1;
    family1.setFamily("Arial");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Times New Roman");

    family1.appendFamily(family2);

    EXPECT_EQ(family1.family(), "Arial");
    ASSERT_NE(family1.next(), nullptr);
    EXPECT_EQ(family1.next()->family(), "Times New Roman");
}

TEST_F(FontFamilyTest, MultipleAppends)
{
    FontFamily family1;
    family1.setFamily("Arial");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Helvetica");
    family1.appendFamily(family2);

    RefPtr<FontFamily> family3 = new FontFamily();
    family3->setFamily("Times New Roman");
    family2->appendFamily(family3);

    RefPtr<FontFamily> family4 = new FontFamily();
    family4->setFamily("Courier New");
    family3->appendFamily(family4);

    // Verify the chain
    EXPECT_EQ(family1.family(), "Arial");
    ASSERT_NE(family1.next(), nullptr);
    EXPECT_EQ(family1.next()->family(), "Helvetica");
    ASSERT_NE(family1.next()->next(), nullptr);
    EXPECT_EQ(family1.next()->next()->family(), "Times New Roman");
    ASSERT_NE(family1.next()->next()->next(), nullptr);
    EXPECT_EQ(family1.next()->next()->next()->family(), "Courier New");
    EXPECT_EQ(family1.next()->next()->next()->next(), nullptr);
}

TEST_F(FontFamilyTest, NextPointer)
{
    FontFamily family1;
    family1.setFamily("Arial");

    // Initially no next
    EXPECT_EQ(family1.next(), nullptr);

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Times New Roman");
    family1.appendFamily(family2);

    // Now has next
    ASSERT_NE(family1.next(), nullptr);
    EXPECT_EQ(family1.next(), family2.get());
}

TEST_F(FontFamilyTest, ConstNext)
{
    FontFamily family1;
    family1.setFamily("Arial");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Times New Roman");
    family1.appendFamily(family2);

    const FontFamily& constFamily = family1;
    const FontFamily* nextFamily = constFamily.next();

    ASSERT_NE(nextFamily, nullptr);
    EXPECT_EQ(nextFamily->family(), "Times New Roman");
}

TEST_F(FontFamilyTest, EmptyFamilyName)
{
    FontFamily family;

    AtomicString empty("");
    family.setFamily(empty);

    EXPECT_TRUE(family.familyIsEmpty());
    EXPECT_TRUE(family.family().isEmpty());
}

TEST_F(FontFamilyTest, OverwriteFamily)
{
    FontFamily family;

    family.setFamily("Arial");
    EXPECT_EQ(family.family(), "Arial");

    family.setFamily("Times New Roman");
    EXPECT_EQ(family.family(), "Times New Roman");
}

TEST_F(FontFamilyTest, CopyWithNext)
{
    FontFamily family1;
    family1.setFamily("Arial");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Times New Roman");
    family1.appendFamily(family2);

    FontFamily family3(family1);

    EXPECT_EQ(family3.family(), "Arial");
    ASSERT_NE(family3.next(), nullptr);
    EXPECT_EQ(family3.next()->family(), "Times New Roman");
}

TEST_F(FontFamilyTest, AssignmentWithNext)
{
    FontFamily family1;
    family1.setFamily("Arial");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Times New Roman");
    family1.appendFamily(family2);

    FontFamily family3;
    family3 = family1;

    EXPECT_EQ(family3.family(), "Arial");
    ASSERT_NE(family3.next(), nullptr);
    EXPECT_EQ(family3.next()->family(), "Times New Roman");
}

TEST_F(FontFamilyTest, EqualityWithNext)
{
    FontFamily family1;
    family1.setFamily("Arial");
    RefPtr<FontFamily> family1Next = new FontFamily();
    family1Next->setFamily("Times New Roman");
    family1.appendFamily(family1Next);

    FontFamily family2;
    family2.setFamily("Arial");
    RefPtr<FontFamily> family2Next = new FontFamily();
    family2Next->setFamily("Times New Roman");
    family2.appendFamily(family2Next);

    FontFamily family3;
    family3.setFamily("Arial");
    RefPtr<FontFamily> family3Next = new FontFamily();
    family3Next->setFamily("Helvetica");
    family3.appendFamily(family3Next);

    EXPECT_TRUE(family1 == family2);
    EXPECT_FALSE(family1 == family3);
}

TEST_F(FontFamilyTest, SelfAssignment)
{
    FontFamily family;
    family.setFamily("Arial");

    family = family;

    EXPECT_EQ(family.family(), "Arial");
}

TEST_F(FontFamilyTest, ChainTraversal)
{
    FontFamily family1;
    family1.setFamily("Font1");

    RefPtr<FontFamily> family2 = new FontFamily();
    family2->setFamily("Font2");
    family1.appendFamily(family2);

    RefPtr<FontFamily> family3 = new FontFamily();
    family3->setFamily("Font3");
    family2->appendFamily(family3);

    // Traverse the chain
    int count = 0;
    const FontFamily* current = &family1;
    while (current) {
        count++;
        EXPECT_FALSE(current->familyIsEmpty());
        current = current->next();
    }

    EXPECT_EQ(count, 3);
}

TEST_F(FontFamilyTest, SpecialCharactersInName)
{
    FontFamily family;

    AtomicString specialName("Arial-Bold-Italic");
    family.setFamily(specialName);

    EXPECT_EQ(family.family(), specialName);
    EXPECT_FALSE(family.familyIsEmpty());
}

TEST_F(FontFamilyTest, LongFamilyName)
{
    FontFamily family;

    AtomicString longName("This Is A Very Long Font Family Name That Should Still Work Correctly");
    family.setFamily(longName);

    EXPECT_EQ(family.family(), longName);
    EXPECT_FALSE(family.familyIsEmpty());
}

TEST_F(FontFamilyTest, GenericFamilyNames)
{
    FontFamily serif;
    serif.setFamily("serif");
    EXPECT_EQ(serif.family(), "serif");

    FontFamily sansSerif;
    sansSerif.setFamily("sans-serif");
    EXPECT_EQ(sansSerif.family(), "sans-serif");

    FontFamily monospace;
    monospace.setFamily("monospace");
    EXPECT_EQ(monospace.family(), "monospace");

    FontFamily cursive;
    cursive.setFamily("cursive");
    EXPECT_EQ(cursive.family(), "cursive");

    FontFamily fantasy;
    fantasy.setFamily("fantasy");
    EXPECT_EQ(fantasy.family(), "fantasy");
}
