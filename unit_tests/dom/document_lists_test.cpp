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

class DocumentListsTest : public ::testing::Test
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

TEST_F(DocumentListsTest, ChangedDocumentsVectorFIFO)
{
    Vector<int> order;
    Vector<Document*>* changed = new Vector<Document*>;

    // Use dummy pointers (never dereferenced) to validate FIFO behavior.
    Document* d1 = reinterpret_cast<Document*>(0x1);
    Document* d2 = reinterpret_cast<Document*>(0x2);
    Document* d3 = reinterpret_cast<Document*>(0x3);

    changed->append(d1);
    changed->append(d2);
    changed->append(d3);

    while (!changed->isEmpty()) {
        Document* d = changed->at(0);
        changed->remove(0);
        order.append(static_cast<int>(reinterpret_cast<uintptr_t>(d)));
    }

    EXPECT_EQ(3u, order.size());
    EXPECT_EQ(1, order[0]);
    EXPECT_EQ(2, order[1]);
    EXPECT_EQ(3, order[2]);

    delete changed;
}

TEST_F(DocumentListsTest, ImageLoadDispatchVectorRemoveAllInstances)
{
    Vector<void*> soon;
    Vector<void*> dispatching;

    void* img = reinterpret_cast<void*>(0x1234);
    void* other = reinterpret_cast<void*>(0x5678);

    // Allow duplicates like the old DeprecatedPtrList code.
    soon.append(img);
    soon.append(other);
    soon.append(img);
    dispatching.append(img);
    dispatching.append(img);

    auto removeAll = [](Vector<void*>& v, void* target) {
        for (unsigned i = 0; i < v.size();) {
            if (v[i] == target) {
                v.remove(i);
            } else {
                ++i;
            }
        }
    };

    removeAll(soon, img);
    removeAll(dispatching, img);

    EXPECT_EQ(1u, soon.size());
    EXPECT_EQ(other, soon[0]);
    EXPECT_TRUE(dispatching.isEmpty());
}

TEST_F(DocumentListsTest, MicroBenchmarkVectorFrontPopLikeQueue)
{
    // Document::updateDocumentsRendering uses Vector + remove(0) as a FIFO.
    // This test is a loose guard to catch accidental quadratic blowups beyond expectation.
    constexpr unsigned N = 20000;
    Vector<Document*> q;
    q.reserveCapacity(N);

    for (unsigned i = 0; i < N; ++i) {
        q.append(reinterpret_cast<Document*>(static_cast<uintptr_t>(i + 1)));
    }

    auto start = std::chrono::steady_clock::now();
    while (!q.isEmpty()) {
        q.remove(0);
    }
    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    EXPECT_TRUE(q.isEmpty());
    EXPECT_LT(ms, 2000);
}
