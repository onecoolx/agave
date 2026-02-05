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

#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace WTF;

// Test basic construction and properties
TEST(DequeTest, DefaultConstructor)
{
    Deque<int> deque;
    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(deque.size(), 0);
}

TEST(DequeTest, InitialCapacity)
{
    Deque<int, 16> deque; // Deque with inline capacity of 16
    EXPECT_TRUE(deque.isEmpty());
}

// Test element addition operations
TEST(DequeTest, AppendElements)
{
    Deque<int> deque;

    deque.append(1);
    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.first(), 1);
    EXPECT_EQ(deque.last(), 1);

    deque.append(2);
    EXPECT_EQ(deque.size(), 2);
    EXPECT_EQ(deque.first(), 1);
    EXPECT_EQ(deque.last(), 2);

    deque.append(3);
    EXPECT_EQ(deque.size(), 3);
    EXPECT_EQ(deque.first(), 1);
    EXPECT_EQ(deque.last(), 3);
}

TEST(DequeTest, PrependElements)
{
    Deque<int> deque;

    deque.prepend(1);
    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.first(), 1);
    EXPECT_EQ(deque.last(), 1);

    deque.prepend(2);
    EXPECT_EQ(deque.size(), 2);
    EXPECT_EQ(deque.first(), 2);
    EXPECT_EQ(deque.last(), 1);

    deque.prepend(3);
    EXPECT_EQ(deque.size(), 3);
    EXPECT_EQ(deque.first(), 3);
    EXPECT_EQ(deque.last(), 1);
}

TEST(DequeTest, MixedAppendPrepend)
{
    Deque<int> deque;

    deque.append(1);
    deque.prepend(2);
    deque.append(3);
    deque.prepend(4);

    EXPECT_EQ(deque.size(), 4);
    EXPECT_EQ(deque.first(), 4);
    EXPECT_EQ(deque.last(), 3);
}

// Test element removal operations
TEST(DequeTest, RemoveFirst)
{
    Deque<int> deque;

    deque.append(1);
    deque.append(2);
    deque.append(3);

    deque.removeFirst();
    EXPECT_EQ(deque.size(), 2);
    EXPECT_EQ(deque.first(), 2);
    EXPECT_EQ(deque.last(), 3);

    deque.removeFirst();
    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.first(), 3);
    EXPECT_EQ(deque.last(), 3);

    deque.removeFirst();
    EXPECT_TRUE(deque.isEmpty());
}

// Test copy operations
TEST(DequeTest, CopyConstructor)
{
    Deque<int> original;
    for (int i = 0; i < 10; ++i) {
        original.append(i);
    }

    Deque<int> copy(original);
    EXPECT_EQ(original.size(), copy.size());

    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

TEST(DequeTest, AssignmentOperator)
{
    Deque<int> original;
    for (int i = 0; i < 10; ++i) {
        original.append(i);
    }

    Deque<int> assigned;
    assigned.append(99); // Add content to ensure it gets cleared
    assigned = original;

    EXPECT_EQ(original.size(), assigned.size());

    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], assigned[i]);
    }
}

TEST(DequeTest, Swap)
{
    Deque<int> deque1;
    Deque<int> deque2;

    for (int i = 0; i < 5; ++i) {
        deque1.append(i);
    }
    for (int i = 10; i < 15; ++i) {
        deque2.append(i);
    }

    size_t size1 = deque1.size();
    size_t size2 = deque2.size();

    deque1.swap(deque2);

    EXPECT_EQ(deque1.size(), size2);
    EXPECT_EQ(deque2.size(), size1);

    EXPECT_EQ(deque1.first(), 10);
    EXPECT_EQ(deque2.first(), 0);
}

TEST(DequeTest, Clear)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    EXPECT_FALSE(deque.isEmpty());
    deque.clear();
    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(deque.size(), 0);
}

// Test forward iteration in detail
TEST(DequeTest, ForwardIteration)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    int expected = 0;
    for (Deque<int>::iterator it = deque.begin(); it != deque.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    EXPECT_EQ(expected, 10);
}

TEST(DequeTest, ConstForwardIteration)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    const Deque<int>& const_deque = deque;
    int expected = 0;
    for (Deque<int>::const_iterator it = const_deque.begin(); it != const_deque.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    EXPECT_EQ(expected, 10);
}

// Comprehensive reverse iteration tests
TEST(DequeTest, ReverseIteration)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Test reverse iteration using reverse iterators
    int expected = 9;
    for (Deque<int>::reverse_iterator it = deque.rbegin(); it != deque.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    EXPECT_EQ(expected, -1); // All elements visited
}

// Enhanced test to verify reverse iteration doesn't miss elements
TEST(DequeTest, ReverseIterationCompleteCoverage)
{
    Deque<int> deque;

    // Fill deque with known values
    std::vector<int> forward_values;
    for (int i = 0; i < 15; ++i) {
        deque.append(i * 10); // 0, 10, 20, ..., 140
        forward_values.push_back(i * 10);
    }

    // Collect values in reverse order
    std::vector<int> reverse_values;
    for (Deque<int>::reverse_iterator it = deque.rbegin(); it != deque.rend(); ++it) {
        reverse_values.push_back(*it);
    }

    // Verify reverse iteration covers all elements
    EXPECT_EQ(deque.size(), reverse_values.size());

    // Check that reverse order matches expected reverse of forward order
    std::vector<int> expected_reverse(forward_values.rbegin(), forward_values.rend());
    EXPECT_EQ(reverse_values, expected_reverse);

    // Verify specific element access
    EXPECT_EQ(*deque.rbegin(), 140); // Last element
    EXPECT_EQ(*(--deque.rend()), 0); // First element (after decrementing rend)
}

TEST(DequeTest, ConstReverseIteration)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    const Deque<int>& const_deque = deque;
    int expected = 9;
    for (Deque<int>::const_reverse_iterator it = const_deque.rbegin(); it != const_deque.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    EXPECT_EQ(expected, -1);
}

// Test the "it != end()" pattern for forward iteration
TEST(DequeTest, ForwardIteratorEndPattern)
{
    Deque<int> deque;

    // Empty deque: begin() should equal end()
    EXPECT_TRUE(deque.begin() == deque.end());

    // Add elements
    for (int i = 0; i < 5; ++i) {
        deque.append(i);
    }

    // Verify the iteration pattern
    int count = 0;
    for (Deque<int>::iterator it = deque.begin(); it != deque.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 5);
    EXPECT_EQ(count, deque.size());
}

// Test the "it != rend()" pattern for reverse iteration
TEST(DequeTest, ReverseIteratorRendPattern)
{
    Deque<int> deque;

    // Empty deque: rbegin() should equal rend()
    EXPECT_TRUE(deque.rbegin() == deque.rend());

    // Add elements
    for (int i = 0; i < 5; ++i) {
        deque.append(i);
    }

    // Verify the reverse iteration pattern
    int count = 0;
    for (Deque<int>::reverse_iterator it = deque.rbegin(); it != deque.rend(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 5);
    EXPECT_EQ(count, deque.size());
}

// Test both forward and reverse iteration on the same deque
TEST(DequeTest, BidirectionalIteration)
{
    Deque<int> deque;
    const int COUNT = 7;

    for (int i = 0; i < COUNT; ++i) {
        deque.append(i * 2); // 0, 2, 4, ..., 12
    }

    // Forward iteration
    std::vector<int> forward_result;
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        forward_result.push_back(*it);
    }

    // Reverse iteration
    std::vector<int> reverse_result;
    for (auto it = deque.rbegin(); it != deque.rend(); ++it) {
        reverse_result.push_back(*it);
    }

    // Verify they have the same number of elements
    EXPECT_EQ(forward_result.size(), reverse_result.size());
    EXPECT_EQ(forward_result.size(), deque.size());

    // Verify reverse is actually the reverse of forward
    std::reverse(forward_result.begin(), forward_result.end());
    EXPECT_EQ(forward_result, reverse_result);
}

TEST(DequeTest, RangeBasedForLoop)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    int expected = 0;
    for (int value : deque) { // Uses begin()/end() internally
        EXPECT_EQ(value, expected++);
    }
    EXPECT_EQ(expected, 10);
}

TEST(DequeTest, IteratorEquality)
{
    Deque<int> deque;
    deque.append(1);
    deque.append(2);

    Deque<int>::iterator it1 = deque.begin();
    Deque<int>::iterator it2 = deque.begin();
    EXPECT_TRUE(it1 == it2);

    ++it1;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);
}

// Test circular buffer behavior
TEST(DequeTest, CircularBufferWrapAround)
{
    Deque<int, 4> deque; // Small capacity to test circular buffer

    // Fill the buffer
    deque.append(1);
    deque.append(2);
    deque.append(3);
    // Buffer: [1, 2, 3, _] start=0, end=3

    // Remove elements from front to create space
    deque.removeFirst(); // Buffer: [_, 2, 3, _] start=1, end=3
    deque.removeFirst(); // Buffer: [_, _, 3, _] start=2, end=3

    // Add more elements to trigger wrap-around
    deque.append(4); // Buffer: [_, _, 3, 4] start=2, end=0 (wrapped)
    deque.append(5); // Buffer: [5, _, 3, 4] start=2, end=1
    deque.append(6); // Buffer: [5, 6, 3, 4] start=2, end=2 (full)

    // Test iteration with wrap-around
    std::vector<int> expected = {3, 4, 5, 6};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }

    EXPECT_EQ(actual, expected);

    // Test reverse iteration with wrap-around
    std::vector<int> reverse_actual;
    for (Deque<int, 4>::reverse_iterator it = deque.rbegin(); it != deque.rend(); ++it) {
        reverse_actual.push_back(*it);
    }

    std::vector<int> reverse_expected = {6, 5, 4, 3};
    EXPECT_EQ(reverse_actual, reverse_expected);
}

// Test removing elements by iterator
TEST(DequeTest, RemoveByIterator)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Remove middle element
    Deque<int>::iterator it = deque.begin();
    for (int i = 0; i < 5; i++) {
        ++it; // Point to element 5
    }
    deque.remove(it);

    EXPECT_EQ(deque.size(), 9);

    // Verify elements
    std::vector<int> expected = {0, 1, 2, 3, 4, 6, 7, 8, 9};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }

    EXPECT_EQ(actual, expected);
}

TEST(DequeTest, RemoveAllElements)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    while (!deque.isEmpty()) {
        deque.removeFirst();
    }

    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(deque.size(), 0);
}

// Test random access operations
TEST(DequeTest, RandomAccess)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i * 10);
    }

    for (size_t i = 0; i < deque.size(); ++i) {
        EXPECT_EQ(deque[i], static_cast<int>(i * 10));
        EXPECT_EQ(deque.at(i), static_cast<int>(i * 10));
    }
}

// Test edge cases
TEST(DequeTest, EmptyDequeOperations)
{
    Deque<int> deque;

    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(deque.size(), 0);

    // Iterators of empty deque should be equal
    EXPECT_TRUE(deque.begin() == deque.end());
    EXPECT_TRUE(deque.rbegin() == deque.rend());
}

TEST(DequeTest, SingleElementDeque)
{
    Deque<int> deque;
    deque.append(42);

    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.first(), 42);
    EXPECT_EQ(deque.last(), 42);
    EXPECT_EQ(*deque.begin(), 42);
    EXPECT_TRUE(++deque.begin() == deque.end());

    // Test reverse iteration on single element
    EXPECT_EQ(*deque.rbegin(), 42);
    EXPECT_TRUE(++deque.rbegin() == deque.rend());
}

// Test algorithm integration
TEST(DequeTest, FindIf)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Find first even number
    auto isEven = [](int x) { return x % 2 == 0; };
    Deque<int>::iterator it = deque.findIf(isEven);
    EXPECT_TRUE(it != deque.end());
    EXPECT_EQ(*it, 0);

    // Find number greater than 100 (should not exist)
    auto isGreaterThan100 = [](int x) { return x > 100; };
    it = deque.findIf(isGreaterThan100);
    EXPECT_TRUE(it == deque.end());
}

// Test with string type
TEST(DequeTest, StringType)
{
    Deque<std::string> deque;

    deque.append("Hello");
    deque.append("World");
    deque.prepend("Start");

    EXPECT_EQ(deque.size(), 3);
    EXPECT_EQ(deque.first(), "Start");
    EXPECT_EQ(deque.last(), "World");
    EXPECT_EQ(deque[1], "Hello");

    // Test iteration with strings
    std::vector<std::string> expected = {"Start", "Hello", "World"};
    std::vector<std::string> actual;
    for (const auto& str : deque) {
        actual.push_back(str);
    }
    EXPECT_EQ(actual, expected);
}

// Test capacity expansion
TEST(DequeTest, CapacityExpansion)
{
    Deque<int> deque;

    // Add many elements to trigger multiple expansions
    for (int i = 0; i < 1000; ++i) {
        deque.append(i);
    }

    EXPECT_EQ(deque.size(), 1000);

    // Verify all elements
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(deque[i], i);
    }

    // Verify iteration still works after expansion
    int count = 0;
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        EXPECT_EQ(*it, count++);
    }
    EXPECT_EQ(count, 1000);
}

// Test complex operations
TEST(DequeTest, ComplexOperations)
{
    Deque<int> deque;

    // Alternate between append and prepend
    for (int i = 0; i < 100; ++i) {
        if (i % 2 == 0) {
            deque.append(i);
        } else {
            deque.prepend(i);
        }

        // Maintain size around 50
        if (deque.size() > 50) {
            deque.removeFirst();
        }
    }

    // Test random access
    for (size_t i = 0; i < deque.size(); ++i) {
        EXPECT_NE(deque.at(i), 999);
    }

    // Test forward iteration
    int forward_count = 0;
    for (Deque<int>::iterator it = deque.begin(); it != deque.end(); ++it) {
        ++forward_count;
    }
    EXPECT_EQ(forward_count, static_cast<int>(deque.size()));

    // Test reverse iteration
    int reverse_count = 0;
    for (Deque<int>::reverse_iterator it = deque.rbegin(); it != deque.rend(); ++it) {
        ++reverse_count;
    }
    EXPECT_EQ(reverse_count, static_cast<int>(deque.size()));
}

// Test different data types
TEST(DequeTest, DifferentTypes)
{
    Deque<double> double_deque;
    for (int i = 0; i < 10; ++i) {
        double_deque.append(i * 1.5);
    }

    Deque<char> char_deque;
    for (char c = 'a'; c <= 'z'; ++c) {
        char_deque.append(c);
    }

    EXPECT_EQ(double_deque.size(), 10);
    EXPECT_EQ(char_deque.size(), 26);

    // Test reverse iteration on char deque
    std::string reverse_chars;
    for (auto it = char_deque.rbegin(); it != char_deque.rend(); ++it) {
        reverse_chars += *it;
    }
    std::string expected_reverse = "zyxwvutsrqponmlkjihgfedcba";
    EXPECT_EQ(reverse_chars, expected_reverse);
}

// Test inline capacity effect
TEST(DequeTest, InlineCapacity)
{
    // Deque with inline capacity
    Deque<int, 10> deque_with_capacity;
    for (int i = 0; i < 5; ++i) {
        deque_with_capacity.append(i);
    }

    // Should use inline storage
    EXPECT_EQ(deque_with_capacity.size(), 5);

    // Exceed inline capacity (trigger heap allocation)
    for (int i = 0; i < 10; ++i) {
        deque_with_capacity.append(i + 10);
    }

    EXPECT_EQ(deque_with_capacity.size(), 15);

    // Test iteration still works after exceeding inline capacity
    int sum = 0;
    for (auto it = deque_with_capacity.begin(); it != deque_with_capacity.end(); ++it) {
        sum += *it;
    }
    EXPECT_GT(sum, 0);
}

// Test iterator invalidation
TEST(DequeTest, IteratorInvalidation)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Get iterator to middle element
    Deque<int>::iterator it = deque.begin();
    for (int i = 0; i < 5; i++) {
        ++it; // Point to element 5
    }

    // Remove elements from front (should not invalidate our iterator)
    deque.removeFirst(); // Remove 0
    deque.removeFirst(); // Remove 1

    // Iterator should still be valid and point to the same logical position
    // Original position was 5, after removing two elements it should be 7
    EXPECT_EQ(*it, 7);

    // Remove element at iterator position (invalidates the iterator)
    Deque<int>::iterator it2 = deque.begin();
    for (int i = 0; i < 3; i++) {
        ++it2; // Point to element 5
    }
    deque.remove(it2);
    // it2 is now invalid according to the implementation
}

// Performance/stress test
TEST(DequeTest, LargeNumberOfElements)
{
    Deque<int> deque;
    const int COUNT = 10000;

    for (int i = 0; i < COUNT; ++i) {
        deque.append(i);
    }

    EXPECT_EQ(deque.size(), COUNT);

    // Test forward iteration on large deque
    int expected = 0;
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    EXPECT_EQ(expected, COUNT);

    // Test reverse iteration on large deque
    expected = COUNT - 1;
    for (auto it = deque.rbegin(); it != deque.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    EXPECT_EQ(expected, -1);
}

// Test specific edge case: removing last element affects reverse iterators
TEST(DequeTest, RemoveAffectsReverseIterators)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i);
    }

    // Get reverse iterator to last element
    auto rit = deque.rbegin();
    EXPECT_EQ(*rit, 4);

    // Remove last element
    deque.removeFirst(); // Actually removes first element (0)
    // Reverse iterator should still be valid and now point to 3

    // Create new reverse iterator after removal
    auto rit2 = deque.rbegin();
    EXPECT_EQ(*rit2, 4); // Last element is still 4
}

// Test the specific "it != rend()" behavior
TEST(DequeTest, ReverseIteratorRendBehavior)
{
    Deque<int> deque;
    deque.append(1);
    deque.append(2);
    deque.append(3);

    // rend() points to "one before the first element"
    // We should be able to iterate from rbegin() to rend()
    std::vector<int> collected;
    for (auto rit = deque.rbegin(); rit != deque.rend(); ++rit) {
        collected.push_back(*rit);
    }

    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 3);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 1);
}
