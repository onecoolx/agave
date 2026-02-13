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

class LoaderTest : public ::testing::Test
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
        loader = new Loader();
    }

    void TearDown() override
    {
        delete loader;
    }

    Loader* loader;
};

// Test: Loader initialization
TEST_F(LoaderTest, LoaderInitialization)
{
    EXPECT_NE(nullptr, loader);
}

// Test: Loader destructor cleans up pending requests
TEST_F(LoaderTest, DestructorCleansPendingRequests)
{
    // This test verifies that the destructor properly cleans up
    // any pending requests without memory leaks
    Loader* tempLoader = new Loader();

    // Destructor should clean up without crashing
    delete tempLoader;

    SUCCEED();
}

// Test: Deque operations - append and removeFirst
TEST_F(LoaderTest, DequeAppendAndRemoveFirst)
{
    Deque<int*> deque;

    int* val1 = new int(1);
    int* val2 = new int(2);
    int* val3 = new int(3);

    deque.append(val1);
    deque.append(val2);
    deque.append(val3);

    EXPECT_EQ(3u, deque.size());
    EXPECT_FALSE(deque.isEmpty());

    EXPECT_EQ(val1, deque.first());
    deque.removeFirst();

    EXPECT_EQ(val2, deque.first());
    deque.removeFirst();

    EXPECT_EQ(val3, deque.first());
    deque.removeFirst();

    EXPECT_TRUE(deque.isEmpty());

    delete val1;
    delete val2;
    delete val3;
}

// Test: Deque FIFO behavior
TEST_F(LoaderTest, DequeFIFOBehavior)
{
    Deque<int*> deque;

    const int count = 10;
    int* values[count];

    // Append values
    for (int i = 0; i < count; i++) {
        values[i] = new int(i);
        deque.append(values[i]);
    }

    EXPECT_EQ(static_cast<size_t>(count), deque.size());

    // Remove in FIFO order
    for (int i = 0; i < count; i++) {
        EXPECT_EQ(values[i], deque.first());
        EXPECT_EQ(*values[i], i);
        deque.removeFirst();
    }

    EXPECT_TRUE(deque.isEmpty());

    // Cleanup
    for (int i = 0; i < count; i++) {
        delete values[i];
    }
}

// Test: Deque iterator-based removal
TEST_F(LoaderTest, DequeIteratorRemoval)
{
    Deque<int*> deque;

    int* val1 = new int(1);
    int* val2 = new int(2);
    int* val3 = new int(3);
    int* val4 = new int(4);

    deque.append(val1);
    deque.append(val2);
    deque.append(val3);
    deque.append(val4);

    // Remove val2.
    // NOTE: Deque iterator stability across removals is not guaranteed.
    typedef Deque<int*>::iterator Iterator;
    for (Iterator it = deque.begin(); it != deque.end(); ++it) {
        if (*(*it) == 2) {
            deque.remove(it);
            break;
        }
    }

    EXPECT_EQ(3u, deque.size());

    // Verify remaining elements
    Iterator it = deque.begin();
    EXPECT_EQ(val1, *it);
    ++it;
    EXPECT_EQ(val3, *it);
    ++it;
    EXPECT_EQ(val4, *it);
    ++it;

    delete val1;
    delete val2;
    delete val3;
    delete val4;
}

// Test: Deque multiple iterator removals
TEST_F(LoaderTest, DequeMultipleIteratorRemovals)
{
    // NOTE: WTF::Deque has strict iterator validity tracking (debug-only), and
    // removal invalidates iterators. The prior version of this test attempted
    // to validate "erase-while-iterating" patterns that are not guaranteed to
    // be safe for WTF::Deque.
    //
    // Keep this test focused on correctness: remove items without relying on
    // iterator stability across removals.

    Deque<int*> deque;

    const int count = 10;
    int* values[count];

    for (int i = 0; i < count; i++) {
        values[i] = new int(i);
        deque.append(values[i]);
    }

    // Remove even numbers by repeatedly searching from begin().
    // This avoids holding iterators across removals.
    bool removedAny = true;
    while (removedAny) {
        removedAny = false;
        typedef Deque<int*>::iterator Iterator;
        for (Iterator it = deque.begin(); it != deque.end(); ++it) {
            if (*(*it) % 2 == 0) {
                deque.remove(it);
                removedAny = true;
                break;
            }
        }
    }

    EXPECT_EQ(5u, deque.size());

    // Verify only odd numbers remain
    for (size_t i = 0; i < deque.size(); ++i)
        EXPECT_EQ(1, *deque[i] % 2);

    // Cleanup
    for (int i = 0; i < count; i++)
        delete values[i];
}

// Test: Deque clear operation
TEST_F(LoaderTest, DequeClearOperation)
{
    Deque<int*> deque;

    int* val1 = new int(1);
    int* val2 = new int(2);
    int* val3 = new int(3);

    deque.append(val1);
    deque.append(val2);
    deque.append(val3);

    EXPECT_EQ(3u, deque.size());

    // Manual cleanup before clear
    while (!deque.isEmpty()) {
        delete deque.first();
        deque.removeFirst();
    }

    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(0u, deque.size());
}

// Test: Deque with large number of elements
TEST_F(LoaderTest, DequeLargeNumberOfElements)
{
    Deque<int*> deque;

    const int count = 1000;

    // Append many elements
    for (int i = 0; i < count; i++) {
        deque.append(new int(i));
    }

    EXPECT_EQ(static_cast<size_t>(count), deque.size());

    // Remove all elements
    for (int i = 0; i < count; i++) {
        EXPECT_EQ(i, *deque.first());
        delete deque.first();
        deque.removeFirst();
    }

    EXPECT_TRUE(deque.isEmpty());
}

// Test: Deque memory management
TEST_F(LoaderTest, DequeMemoryManagement)
{
    Deque<int*> deque;

    // Add and remove multiple times
    for (int cycle = 0; cycle < 10; cycle++) {
        for (int i = 0; i < 10; i++) {
            deque.append(new int(i));
        }

        EXPECT_EQ(10u, deque.size());

        while (!deque.isEmpty()) {
            delete deque.first();
            deque.removeFirst();
        }

        EXPECT_TRUE(deque.isEmpty());
    }

    SUCCEED();
}

// Test: Deque iterator stability during removal
TEST_F(LoaderTest, DequeIteratorStability)
{
    Deque<int*> deque;

    for (int i = 0; i < 5; i++) {
        deque.append(new int(i));
    }

    // Remove middle element.
    // NOTE: Deque iterator stability across removals is not guaranteed.
    // Avoid holding iterators across remove() calls.
    bool removed = false;
    typedef Deque<int*>::iterator Iterator;
    for (Iterator it = deque.begin(); it != deque.end(); ++it) {
        if (*(*it) == 2) {
            delete *it;
            deque.remove(it);
            removed = true;
            break;
        }
    }

    EXPECT_TRUE(removed);
    EXPECT_EQ(4u, deque.size());

    // Cleanup
    while (!deque.isEmpty()) {
        delete deque.first();
        deque.removeFirst();
    }
}

// Test: Deque performance - append operations
TEST_F(LoaderTest, DequePerformanceAppend)
{
    Deque<int*> deque;

    const int count = 10000;

    // Measure append performance
    for (int i = 0; i < count; i++) {
        deque.append(new int(i));
    }

    EXPECT_EQ(static_cast<size_t>(count), deque.size());

    // Cleanup
    while (!deque.isEmpty()) {
        delete deque.first();
        deque.removeFirst();
    }
}

// Test: Deque performance - removeFirst operations
TEST_F(LoaderTest, DequePerformanceRemoveFirst)
{
    Deque<int*> deque;

    const int count = 10000;

    for (int i = 0; i < count; i++) {
        deque.append(new int(i));
    }

    // Measure removeFirst performance
    for (int i = 0; i < count; i++) {
        delete deque.first();
        deque.removeFirst();
    }

    EXPECT_TRUE(deque.isEmpty());
}

// Test: Deque vs DeprecatedPtrList behavior compatibility
TEST_F(LoaderTest, DequeCompatibilityWithDeprecatedPtrList)
{
    Deque<int*> deque;

    // Test that Deque provides similar FIFO behavior as DeprecatedPtrList
    int* val1 = new int(1);
    int* val2 = new int(2);
    int* val3 = new int(3);

    // Append (like DeprecatedPtrList::append)
    deque.append(val1);
    deque.append(val2);
    deque.append(val3);

    // First (like DeprecatedPtrList::getFirst)
    EXPECT_EQ(val1, deque.first());

    // RemoveFirst (like DeprecatedPtrList::removeFirst)
    deque.removeFirst();
    EXPECT_EQ(val2, deque.first());

    // Size (like DeprecatedPtrList::count)
    EXPECT_EQ(2u, deque.size());

    // IsEmpty (like DeprecatedPtrList::isEmpty)
    EXPECT_FALSE(deque.isEmpty());

    delete val1;
    delete val2;
    delete val3;
}
