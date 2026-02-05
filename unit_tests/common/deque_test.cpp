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

// Test the new at(int) method with positive indices
TEST(DequeTest, AtWithIntIndexPositive)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i * 10);
    }

    // Test positive indices
    EXPECT_EQ(deque.at(0), 0);
    EXPECT_EQ(deque.at(1), 10);
    EXPECT_EQ(deque.at(5), 50);
    EXPECT_EQ(deque.at(9), 90);

    // Test out of bounds (should ASSERT in debug mode)
    // EXPECT_DEATH(deque.at(10), "");  // This would fail in debug mode
}

// Test operator[] with int indices
TEST(DequeTest, OperatorBracketWithInt)
{
    Deque<std::string> deque;
    deque.append("zero");
    deque.append("one");
    deque.append("two");
    deque.append("three");
    deque.append("four");

    // Test positive indices
    EXPECT_EQ(deque[0], "zero");
    EXPECT_EQ(deque[1], "one");
    EXPECT_EQ(deque[4], "four");
}

// Test mixed usage of size_t and int indices
TEST(DequeTest, MixedIndexTypes)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i + 100);
    }

    size_t size_t_index = 2;
    int int_index = 2;

    EXPECT_EQ(deque.at(size_t_index), deque.at(int_index));
    EXPECT_EQ(deque[size_t_index], deque[int_index]);
}

// Test at() method after wrap-around in circular buffer
TEST(DequeTest, AtMethodWithCircularBuffer)
{
    Deque<int, 4> deque; // Small capacity to force wrap-around

    // Fill and then partially empty to create wrap-around situation
    deque.append(10);
    deque.append(20);
    deque.append(30);
    // Buffer: [10, 20, 30, _] start=0, end=3

    deque.removeFirst();
    deque.removeFirst();
    // Buffer: [_, _, 30, _] start=2, end=3

    deque.append(40);
    deque.append(50);
    deque.append(60);
    // Buffer after wrap-around: [50, 60, 30, 40] start=2, end=2 (wrapped)
    // Logical order: 30, 40, 50, 60

    // Test at() with various indices
    EXPECT_EQ(deque.at(0), 30);
    EXPECT_EQ(deque.at(1), 40);
    EXPECT_EQ(deque.at(2), 50);
    EXPECT_EQ(deque.at(3), 60);

    // Test operator[]
    EXPECT_EQ(deque[0], 30);
}

// Test at() method with const Deque
TEST(DequeTest, AtMethodConst)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i * 2);
    }

    const Deque<int>& const_deque = deque;

    // Test const at() with size_t
    EXPECT_EQ(const_deque.at(0), 0);
    EXPECT_EQ(const_deque.at(2), 4);
    EXPECT_EQ(const_deque.at(4), 8);

    // Test const operator[]
    EXPECT_EQ(const_deque[0], 0);
}

// Test edge cases for at() method
TEST(DequeTest, AtMethodEdgeCases)
{
    // Test with single element
    Deque<int> single;
    single.append(42);

    EXPECT_EQ(single.at(0), 42);
    EXPECT_EQ(single[0], 42);

    // Test with empty deque (should ASSERT on access)
    Deque<int> empty;
    // EXPECT_DEATH(empty.at(0), "");  // Would ASSERT in debug mode
    // EXPECT_DEATH(empty.at(-1), ""); // Would ASSERT in debug mode

    // Test with large indices
    Deque<int> large;
    for (int i = 0; i < 1000; ++i) {
        large.append(i);
    }

    EXPECT_EQ(large.at(0), 0);
    EXPECT_EQ(large.at(500), 500);
    EXPECT_EQ(large.at(999), 999);
}

// Test that at() and operator[] work correctly with modifications
TEST(DequeTest, AtMethodWithModifications)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i);
    }

    // Modify using at() with int index
    deque.at(2) = 100;
    EXPECT_EQ(deque[2], 100);

    // Modify using operator[] with int index
    deque[1] = 300;
    EXPECT_EQ(deque.at(1), 300);
}

static bool inline cmp_int(int val) { return val == 25; }
// Test compatibility with STL algorithms using at() and operator[]
TEST(DequeTest, AtMethodWithSTLAlgorithms)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i * i); // Squares: 0, 1, 4, 9, 16, 25, 36, 49, 64, 81
    }

    // Use std::find with operator[]
    auto it = deque.findIf(cmp_int);
    EXPECT_NE(it, deque.end());

    // Find index of element 25
    int index = -1;
    for (int i = 0; i < static_cast<int>(deque.size()); ++i) {
        if (deque.at(i) == 25) {
            index = i;
            break;
        }
    }
    EXPECT_EQ(index, 5);
    EXPECT_EQ(deque[index], 25);
    EXPECT_EQ(deque.at(index), 25);

    // Use std::accumulate with at()
    int sum = 0;
    for (int i = 0; i < static_cast<int>(deque.size()); ++i) {
        sum += deque.at(i);
    }
    EXPECT_EQ(sum, 0 + 1 + 4 + 9 + 16 + 25 + 36 + 49 + 64 + 81);
}

// Test that at() works correctly with the fixed buffer indexing
TEST(DequeTest, AtMethodBufferIndexing)
{
    // This test specifically verifies that at() correctly handles
    // the circular buffer indexing

    Deque<int, 8> deque;

    // Create a scenario where m_start is not at 0
    deque.append(1);
    deque.append(2);
    deque.append(3);
    deque.append(4);
    deque.append(5);

    deque.removeFirst();
    deque.removeFirst();
    // Now m_start is at 2

    // Add more elements
    deque.append(6);
    deque.append(7);
    deque.append(8);
    deque.append(9);
    deque.append(10); // This should cause wrap-around

    // Verify at() returns correct values
    // Expected order: 3, 4, 5, 6, 7, 8, 9, 10
    EXPECT_EQ(deque.at(0), 3);
    EXPECT_EQ(deque.at(1), 4);
    EXPECT_EQ(deque.at(2), 5);
    EXPECT_EQ(deque.at(3), 6);
    EXPECT_EQ(deque.at(4), 7);
    EXPECT_EQ(deque.at(5), 8);
    EXPECT_EQ(deque.at(6), 9);
    EXPECT_EQ(deque.at(7), 10);

    // Verify operator[]
    EXPECT_EQ(deque[0], 3);
}

// Test performance aspect of at() method
TEST(DequeTest, AtMethodPerformance)
{
    Deque<int> deque;
    const int COUNT = 10000;

    // Fill deque with values
    for (int i = 0; i < COUNT; ++i) {
        deque.append(i);
    }

    // Access all elements using at() with positive indices
    for (int i = 0; i < COUNT; ++i) {
        EXPECT_EQ(deque.at(i), i);
    }

    // Access all elements using operator[] with positive indices
    for (int i = 0; i < COUNT; ++i) {
        EXPECT_EQ(deque[i], i);
    }
}

template <typename Itr>
static void _advance(Itr& it, int cn)
{
    for (int i = 0; i < cn; i++) {
        ++it;
    }
}

// Test removing elements using reverse iterator
TEST(DequeTest, RemoveByReverseIterator)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i * 10); // 0, 10, 20, ..., 90
    }

    // Get reverse iterator to last element
    auto rit = deque.rbegin();
    EXPECT_EQ(*rit, 90);

    // Remove last element using reverse iterator
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 9);
    EXPECT_EQ(deque.last(), 80);
    EXPECT_EQ(*deque.rbegin(), 80);

    // Verify the element was actually removed
    std::vector<int> expected = {0, 10, 20, 30, 40, 50, 60, 70, 80};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }
    EXPECT_EQ(actual, expected);
}

// Test removing middle element using reverse iterator
TEST(DequeTest, RemoveMiddleByReverseIterator)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Get reverse iterator to middle element (5th from end, which is element 5)
    auto rit = deque.rbegin();
    _advance(rit, 4); // Points to element 5 (0,1,2,3,4,5,6,7,8,9 - rbegin starts at 9)
    EXPECT_EQ(*rit, 5);

    // Remove element 5
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 9);

    // Verify elements
    std::vector<int> expected = {0, 1, 2, 3, 4, 6, 7, 8, 9};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }
    EXPECT_EQ(actual, expected);
}

// Test removing first element using reverse iterator (rend() - 1)
TEST(DequeTest, RemoveFirstByReverseIterator)
{
    Deque<std::string> deque;
    deque.append("first");
    deque.append("second");
    deque.append("third");
    deque.append("fourth");

    // Get reverse iterator to first element (rend() - 1)
    auto rit = deque.rend();
    --rit; // Points to "first"
    EXPECT_EQ(*rit, "first");

    // Remove first element
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 3);
    EXPECT_EQ(deque.first(), "second");
    EXPECT_EQ(deque.last(), "fourth");

    // Verify using forward iteration
    std::vector<std::string> expected = {"second", "third", "fourth"};
    std::vector<std::string> actual;
    for (const auto& str : deque) {
        actual.push_back(str);
    }
    EXPECT_EQ(actual, expected);
}

// Test removing with const_reverse_iterator
TEST(DequeTest, RemoveByConstReverseIterator)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i * 2); // 0, 2, 4, 6, 8
    }

    const Deque<int>& const_deque = deque;

    // Get const_reverse_iterator to last element
    auto crit = const_deque.rbegin();
    EXPECT_EQ(*crit, 8);

    // Remove last element (need to cast away const for removal)
    Deque<int>::const_reverse_iterator non_const_crit = crit;
    deque.remove(non_const_crit);

    EXPECT_EQ(deque.size(), 4);
    EXPECT_EQ(deque.last(), 6);
}

// Test removing elements in reverse order using reverse iterators
TEST(DequeTest, RemoveAllInReverseOrder)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i + 1); // 1, 2, 3, 4, 5
    }

    // Remove all elements in reverse order
    while (!deque.isEmpty()) {
        auto rit = deque.rbegin();
        int value = *rit;
        deque.remove(rit);

        // Verify we're removing in reverse order
        if (!deque.isEmpty()) {
            EXPECT_EQ(deque.last(), value - 1);
        }
    }

    EXPECT_TRUE(deque.isEmpty());
}

// Test that reverse iterator removal works correctly with circular buffer wrap-around
TEST(DequeTest, ReverseIteratorRemoveWithWrapAround)
{
    Deque<int, 4> deque; // Small capacity to force wrap-around

    // Create wrap-around situation
    deque.append(10);
    deque.append(20);
    deque.append(30);
    // Buffer: [10, 20, 30, _] start=0, end=3

    deque.removeFirst(); // Remove 10
    deque.removeFirst(); // Remove 20
    // Buffer: [_, _, 30, _] start=2, end=3

    deque.append(40);
    deque.append(50);
    deque.append(60);
    // Buffer after wrap-around: [50, 60, 30, 40] start=2, end=2 (full)
    // Logical order: 30, 40, 50, 60

    // Get reverse iterator to element 50 (which is at physical index 0)
    auto rit = deque.rbegin();
    ++rit; // Skip 60, now points to 50
    EXPECT_EQ(*rit, 50);

    // Remove element 50
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 3);

    // Verify remaining elements
    std::vector<int> expected = {30, 40, 60};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }
    EXPECT_EQ(actual, expected);

    // Verify reverse iteration after removal
    std::vector<int> reverse_actual;
    for (auto rit2 = deque.rbegin(); rit2 != deque.rend(); ++rit2) {
        reverse_actual.push_back(*rit2);
    }
    std::vector<int> reverse_expected = {60, 40, 30};
    EXPECT_EQ(reverse_actual, reverse_expected);
}

// Test mixed forward and reverse iterator removal
TEST(DequeTest, MixedForwardAndReverseIteratorRemoval)
{
    Deque<int> deque;
    for (int i = 0; i < 10; ++i) {
        deque.append(i);
    }

    // Remove using forward iterator (element 3)
    auto fit = deque.begin();
    _advance(fit, 3);
    EXPECT_EQ(*fit, 3);
    deque.remove(fit);
    EXPECT_EQ(deque.size(), 9);

    // Remove using reverse iterator (element 7, which is now at position 6)
    auto rit = deque.rbegin();
    _advance(rit, 2); // Skip 9 and 8, points to 7
    EXPECT_EQ(*rit, 7);
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 8);

    // Remove using forward iterator again (element 1)
    fit = deque.begin();
    ++fit; // Points to 1 (after removing 3, elements are: 0,1,2,4,5,6,8,9)
    EXPECT_EQ(*fit, 1);
    deque.remove(fit);
    EXPECT_EQ(deque.size(), 7);

    // Verify final elements
    std::vector<int> expected = {0, 2, 4, 5, 6, 8, 9};
    std::vector<int> actual;
    for (int value : deque) {
        actual.push_back(value);
    }
    EXPECT_EQ(actual, expected);
}

// Test iterator invalidation after removal
TEST(DequeTest, ReverseIteratorInvalidationAfterRemoval)
{
    Deque<int> deque;
    for (int i = 0; i < 5; ++i) {
        deque.append(i * 10); // 0, 10, 20, 30, 40
    }

    // Get two reverse iterators
    auto rit1 = deque.rbegin(); // Points to 40
    auto rit2 = deque.rbegin();
    ++rit2; // Points to 30

    // Remove element 40 (pointed by rit1)
    deque.remove(rit1);

    // rit1 is now invalidated
    // rit2 should still be valid and now point to 20 (elements shifted)

    // Get new iterator to verify
    auto rit3 = deque.rbegin();
    EXPECT_EQ(*rit3, 30); // After removing 40, 30 is now last
}

// Test removing with reverse iterator at boundary conditions
TEST(DequeTest, ReverseIteratorRemoveBoundaryConditions)
{
    // Test with single element
    Deque<int> single;
    single.append(42);

    auto rit = single.rbegin();
    EXPECT_EQ(*rit, 42);
    single.remove(rit);
    EXPECT_TRUE(single.isEmpty());

    // Test with two elements
    Deque<int> two;
    two.append(1);
    two.append(2);

    // Remove last element
    rit = two.rbegin();
    EXPECT_EQ(*rit, 2);
    two.remove(rit);
    EXPECT_EQ(two.size(), 1);
    EXPECT_EQ(two.first(), 1);
    EXPECT_EQ(two.last(), 1);

    // Remove first element (now the only element)
    rit = two.rbegin();
    EXPECT_EQ(*rit, 1);
    two.remove(rit);
    EXPECT_TRUE(two.isEmpty());
}

// Test removing element pointed by reverse iterator when it's at rend() (should ASSERT)
TEST(DequeTest, RemoveReverseIteratorAtRend)
{
    Deque<int> deque;
    deque.append(1);
    deque.append(2);

    // Get iterator to rend() (one before first element)
    auto rit = deque.rend();

    // Trying to remove rend() should ASSERT in debug mode
    // EXPECT_DEATH(deque.remove(rit), "");  // Would ASSERT in debug mode

    // Instead, test with valid iterator just before rend()
    rit = deque.rend();
    --rit; // Points to first element (1)
    EXPECT_EQ(*rit, 1);
    deque.remove(rit);
    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.first(), 2);
}
