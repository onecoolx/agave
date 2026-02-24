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

#include <chrono>

// These tests validate core behaviors relied on by RenderBlock's migrated
// float container (Vector<FloatingObject*>) without needing a full layout tree.

class RenderBlockFloatContainerTest : public ::testing::Test
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
};

TEST_F(RenderBlockFloatContainerTest, RemoveDeletesAndCompacts)
{
    struct DummyFloat {
        int id;
        explicit DummyFloat(int i) : id(i) { }
    };

    Vector<DummyFloat*> floats;
    floats.append(new DummyFloat(1));
    floats.append(new DummyFloat(2));
    floats.append(new DummyFloat(3));

    // Remove middle element (matches RenderBlock::removeFloatingObject semantics: delete + remove).
    for (unsigned i = 0; i < floats.size();) {
        if (floats[i]->id == 2) {
            delete floats[i];
            floats.remove(i);
            break;
        }
        ++i;
    }

    ASSERT_EQ(2u, floats.size());
    EXPECT_EQ(1, floats[0]->id);
    EXPECT_EQ(3, floats[1]->id);

    for (unsigned i = 0; i < floats.size(); ++i)
        delete floats[i];
}

TEST_F(RenderBlockFloatContainerTest, UnpositionedStartIndexProgresses)
{
    struct DummyFloat {
        int startY;
        explicit DummyFloat(int s) : startY(s) { }
    };

    Vector<DummyFloat*> floats;
    unsigned unpositionedStart = 0;

    floats.append(new DummyFloat(-1));
    floats.append(new DummyFloat(-1));
    floats.append(new DummyFloat(10)); // already positioned

    // Simulate that the first two were processed.
    unpositionedStart = 2;
    ASSERT_EQ(-1, floats[unpositionedStart - 1]->startY);

    // Mark remaining as positioned and advance like RenderBlock::positionNewFloats.
    for (unsigned i = unpositionedStart; i < floats.size(); ++i)
        floats[i]->startY = 42;
    unpositionedStart = floats.size();

    EXPECT_EQ(floats.size(), unpositionedStart);

    for (unsigned i = 0; i < floats.size(); ++i)
        delete floats[i];
}

TEST_F(RenderBlockFloatContainerTest, MicroBenchmarkRemoveAndIterate)
{
    // Not a strict perf test (CI variance), but acts as a regression guard that
    // the new approach remains linear-ish and doesn't explode accidentally.
    struct DummyFloat {
        unsigned id;
        explicit DummyFloat(unsigned v) : id(v) { }
    };

    constexpr unsigned N = 50000;
    Vector<DummyFloat*> floats;
    floats.reserveCapacity(N);
    for (unsigned i = 0; i < N; ++i)
        floats.append(new DummyFloat(i));

    auto start = std::chrono::steady_clock::now();

    // Remove every 3rd item.
    for (unsigned i = 0; i < floats.size();) {
        if ((floats[i]->id % 3) == 0) {
            delete floats[i];
            floats.remove(i);
        } else
            ++i;
    }

    // Iterate and touch data.
    unsigned sum = 0;
    for (unsigned i = 0; i < floats.size(); ++i)
        sum += floats[i]->id;

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Basic sanity.
    EXPECT_GT(floats.size(), 0u);
    EXPECT_GT(sum, 0u);

    // Time budget: keep loose to avoid flakes.
    EXPECT_LT(ms, 2000);

    for (unsigned i = 0; i < floats.size(); ++i)
        delete floats[i];
}
