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

class StyleSheetListTest : public ::testing::Test
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
        doc = new Document(0, false, false);
        doc->ref();
        list = new StyleSheetList(doc);
        list->ref();
    }

    void TearDown() override
    {
        list->deref();
        doc->deref();
    }

    Document* doc;
    StyleSheetList* list;
};

// Test: Empty list initialization
TEST_F(StyleSheetListTest, EmptyListInitialization)
{
    EXPECT_EQ(0u, list->length());
    EXPECT_EQ(nullptr, list->item(0));
}

// Test: Add single stylesheet
TEST_F(StyleSheetListTest, AddSingleStyleSheet)
{
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    list->add(sheet);

    EXPECT_EQ(1u, list->length());
    EXPECT_EQ(sheet, list->item(0));

    sheet->deref();
}

// Test: Add multiple stylesheets
TEST_F(StyleSheetListTest, AddMultipleStyleSheets)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet3 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();
    sheet3->ref();

    list->add(sheet1);
    list->add(sheet2);
    list->add(sheet3);

    EXPECT_EQ(3u, list->length());
    EXPECT_EQ(sheet1, list->item(0));
    EXPECT_EQ(sheet2, list->item(1));
    EXPECT_EQ(sheet3, list->item(2));

    sheet1->deref();
    sheet2->deref();
    sheet3->deref();
}

// Test: Add duplicate stylesheet (should not add)
TEST_F(StyleSheetListTest, AddDuplicateStyleSheet)
{
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    list->add(sheet);
    list->add(sheet); // Try to add again

    EXPECT_EQ(1u, list->length());
    EXPECT_EQ(sheet, list->item(0));

    sheet->deref();
}

// Test: Remove stylesheet
TEST_F(StyleSheetListTest, RemoveStyleSheet)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet3 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();
    sheet3->ref();

    list->add(sheet1);
    list->add(sheet2);
    list->add(sheet3);

    list->remove(sheet2);

    EXPECT_EQ(2u, list->length());
    EXPECT_EQ(sheet1, list->item(0));
    EXPECT_EQ(sheet3, list->item(1));

    sheet1->deref();
    sheet2->deref();
    sheet3->deref();
}

// Test: Remove first stylesheet
TEST_F(StyleSheetListTest, RemoveFirstStyleSheet)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();

    list->add(sheet1);
    list->add(sheet2);

    list->remove(sheet1);

    EXPECT_EQ(1u, list->length());
    EXPECT_EQ(sheet2, list->item(0));

    sheet1->deref();
    sheet2->deref();
}

// Test: Remove last stylesheet
TEST_F(StyleSheetListTest, RemoveLastStyleSheet)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();

    list->add(sheet1);
    list->add(sheet2);

    list->remove(sheet2);

    EXPECT_EQ(1u, list->length());
    EXPECT_EQ(sheet1, list->item(0));

    sheet1->deref();
    sheet2->deref();
}

// Test: Remove non-existent stylesheet
TEST_F(StyleSheetListTest, RemoveNonExistentStyleSheet)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();

    list->add(sheet1);

    // Try to remove a sheet that's not in the list
    list->remove(sheet2);

    EXPECT_EQ(1u, list->length());
    EXPECT_EQ(sheet1, list->item(0));

    sheet1->deref();
    sheet2->deref();
}

// Test: Item with invalid index
TEST_F(StyleSheetListTest, ItemInvalidIndex)
{
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    list->add(sheet);

    EXPECT_EQ(nullptr, list->item(10));
    EXPECT_EQ(nullptr, list->item(1));

    sheet->deref();
}

// Test: Reference counting on add
TEST_F(StyleSheetListTest, ReferenceCountingOnAdd)
{
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    int initialRefCount = sheet->refCount();

    list->add(sheet);

    // RefPtr should have increased ref count
    EXPECT_GT(sheet->refCount(), initialRefCount);

    sheet->deref();
}

// Test: Reference counting on remove
TEST_F(StyleSheetListTest, ReferenceCountingOnRemove)
{
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    list->add(sheet);
    int refCountAfterAdd = sheet->refCount();

    list->remove(sheet);

    // RefPtr should have decreased ref count
    EXPECT_LT(sheet->refCount(), refCountAfterAdd);

    sheet->deref();
}

// Test: Destructor properly cleans up
TEST_F(StyleSheetListTest, DestructorCleansUp)
{
    StyleSheetList* tempList = new StyleSheetList(doc);
    tempList->ref();

    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    tempList->add(sheet);
    int refCountAfterAdd = sheet->refCount();

    tempList->deref(); // Should trigger destructor

    // Sheet's ref count should have decreased
    EXPECT_LT(sheet->refCount(), refCountAfterAdd);

    sheet->deref();
}

// Test: Multiple add and remove operations
TEST_F(StyleSheetListTest, MultipleOperationsSequence)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet3 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet4 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();
    sheet3->ref();
    sheet4->ref();

    list->add(sheet1);
    list->add(sheet2);
    list->add(sheet3);
    list->remove(sheet2);
    list->add(sheet4);

    EXPECT_EQ(3u, list->length());
    EXPECT_EQ(sheet1, list->item(0));
    EXPECT_EQ(sheet3, list->item(1));
    EXPECT_EQ(sheet4, list->item(2));

    sheet1->deref();
    sheet2->deref();
    sheet3->deref();
    sheet4->deref();
}

// Test: Clear all stylesheets
TEST_F(StyleSheetListTest, ClearAllStyleSheets)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet3 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();
    sheet3->ref();

    list->add(sheet1);
    list->add(sheet2);
    list->add(sheet3);

    list->remove(sheet1);
    list->remove(sheet2);
    list->remove(sheet3);

    EXPECT_EQ(0u, list->length());

    sheet1->deref();
    sheet2->deref();
    sheet3->deref();
}

// Test: Large number of stylesheets
TEST_F(StyleSheetListTest, LargeNumberOfStyleSheets)
{
    const int numSheets = 100;
    CSSStyleSheet* sheets[numSheets];

    for (int i = 0; i < numSheets; i++) {
        sheets[i] = new CSSStyleSheet(doc);
        sheets[i]->ref();
        list->add(sheets[i]);
    }

    EXPECT_EQ(static_cast<unsigned>(numSheets), list->length());

    for (int i = 0; i < numSheets; i++) {
        EXPECT_EQ(sheets[i], list->item(i));
    }

    for (int i = 0; i < numSheets; i++) {
        sheets[i]->deref();
    }
}

// Test: Document destroyed notification
TEST_F(StyleSheetListTest, DocumentDestroyed)
{
    list->documentDestroyed();

    // List should still be functional but document pointer is null
    CSSStyleSheet* sheet = new CSSStyleSheet(doc);
    sheet->ref();

    list->add(sheet);
    EXPECT_EQ(1u, list->length());

    sheet->deref();
}

// Test: No memory leaks with RefPtr
TEST_F(StyleSheetListTest, NoMemoryLeaksWithRefPtr)
{
    CSSStyleSheet* sheet1 = new CSSStyleSheet(doc);
    CSSStyleSheet* sheet2 = new CSSStyleSheet(doc);
    sheet1->ref();
    sheet2->ref();

    int initialRefCount1 = sheet1->refCount();
    int initialRefCount2 = sheet2->refCount();

    list->add(sheet1);
    list->add(sheet2);
    list->remove(sheet1);
    list->remove(sheet2);

    // After add and remove, ref counts should be back to initial
    EXPECT_EQ(initialRefCount1, sheet1->refCount());
    EXPECT_EQ(initialRefCount2, sheet2->refCount());

    sheet1->deref();
    sheet2->deref();
}
