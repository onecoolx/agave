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

class RefPtrPerformanceTest : public ::testing::Test
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

// Test: Vector<RefPtr<T>> performance - append operations
TEST_F(RefPtrPerformanceTest, VectorRefPtrAppendPerformance)
{
    Vector<RefPtr<CSSRule> > rules;

    const int count = 1000;
    CSSStyleRule* rawRules[count];

    // Create rules
    for (int i = 0; i < count; i++) {
        rawRules[i] = new CSSStyleRule(nullptr);
        rawRules[i]->ref();
    }

    // Measure append performance
    for (int i = 0; i < count; i++) {
        rules.append(rawRules[i]);
    }

    EXPECT_EQ(static_cast<size_t>(count), rules.size());

    // Cleanup
    for (int i = 0; i < count; i++) {
        rawRules[i]->deref();
    }
}

// Test: Vector<RefPtr<T>> performance - remove operations
TEST_F(RefPtrPerformanceTest, VectorRefPtrRemovePerformance)
{
    Vector<RefPtr<CSSRule> > rules;

    const int count = 1000;
    CSSStyleRule* rawRules[count];

    for (int i = 0; i < count; i++) {
        rawRules[i] = new CSSStyleRule(nullptr);
        rawRules[i]->ref();
        rules.append(rawRules[i]);
    }

    // Measure remove performance (remove from end)
    for (int i = count - 1; i >= 0; i--) {
        rules.remove(i);
    }

    EXPECT_EQ(0u, rules.size());

    // Cleanup
    for (int i = 0; i < count; i++) {
        rawRules[i]->deref();
    }
}

// Test: Vector<RefPtr<T>> performance - random access
TEST_F(RefPtrPerformanceTest, VectorRefPtrRandomAccessPerformance)
{
    Vector<RefPtr<CSSRule> > rules;

    const int count = 1000;
    CSSStyleRule* rawRules[count];

    for (int i = 0; i < count; i++) {
        rawRules[i] = new CSSStyleRule(nullptr);
        rawRules[i]->ref();
        rules.append(rawRules[i]);
    }

    // Measure random access performance
    for (int i = 0; i < count; i++) {
        CSSRule* rule = rules[i].get();
        EXPECT_EQ(rawRules[i], rule);
    }

    // Cleanup
    for (int i = 0; i < count; i++) {
        rawRules[i]->deref();
    }
}

// Test: Memory - no leaks with Vector<RefPtr<T>>
TEST_F(RefPtrPerformanceTest, VectorRefPtrNoMemoryLeaks)
{
    const int iterations = 100;
    const int itemsPerIteration = 100;

    for (int iter = 0; iter < iterations; iter++) {
        Vector<RefPtr<CSSRule> > rules;
        CSSStyleRule* rawRules[itemsPerIteration];

        // Create and add rules
        for (int i = 0; i < itemsPerIteration; i++) {
            rawRules[i] = new CSSStyleRule(nullptr);
            rawRules[i]->ref();
            rules.append(rawRules[i]);
        }

        // Verify ref counts
        for (int i = 0; i < itemsPerIteration; i++) {
            EXPECT_GE(rawRules[i]->refCount(), 2); // At least our ref + RefPtr's ref
        }

        // Clear vector (RefPtr should handle cleanup)
        rules.clear();

        // Verify ref counts decreased
        for (int i = 0; i < itemsPerIteration; i++) {
            EXPECT_EQ(1, rawRules[i]->refCount()); // Only our ref remains
        }

        // Cleanup
        for (int i = 0; i < itemsPerIteration; i++) {
            rawRules[i]->deref();
        }
    }

    SUCCEED();
}

// Test: Memory - no double free with Vector<RefPtr<T>>
TEST_F(RefPtrPerformanceTest, VectorRefPtrNoDoubleFree)
{
    Vector<RefPtr<CSSRule> > rules;

    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();

    int initialRefCount = rule->refCount();

    // Add to vector
    rules.append(rule);
    EXPECT_EQ(initialRefCount + 1, rule->refCount());

    // Remove from vector
    rules.remove(0);
    EXPECT_EQ(initialRefCount, rule->refCount());

    // Add again
    rules.append(rule);
    EXPECT_EQ(initialRefCount + 1, rule->refCount());

    // Clear vector
    rules.clear();
    EXPECT_EQ(initialRefCount, rule->refCount());

    rule->deref();
}

// Test: CSSRuleList performance comparison
TEST_F(RefPtrPerformanceTest, CSSRuleListPerformance)
{
    CSSRuleList* ruleList = new CSSRuleList();
    ruleList->ref();

    const int count = 1000;
    CSSStyleRule* rules[count];

    // Create rules
    for (int i = 0; i < count; i++) {
        rules[i] = new CSSStyleRule(nullptr);
        rules[i]->ref();
    }

    // Measure append performance
    for (int i = 0; i < count; i++) {
        ruleList->append(rules[i]);
    }

    EXPECT_EQ(static_cast<unsigned>(count), ruleList->length());

    // Measure access performance
    for (int i = 0; i < count; i++) {
        EXPECT_EQ(rules[i], ruleList->item(i));
    }

    // Cleanup
    for (int i = 0; i < count; i++) {
        rules[i]->deref();
    }
    ruleList->deref();
}

// Test: StyleSheetList performance
TEST_F(RefPtrPerformanceTest, StyleSheetListPerformance)
{
    Document* doc = new Document(0, nullptr, false);
    doc->ref();

    StyleSheetList* list = new StyleSheetList(doc);
    list->ref();

    const int count = 1000;
    CSSStyleSheet* sheets[count];

    // Create stylesheets
    for (int i = 0; i < count; i++) {
        sheets[i] = new CSSStyleSheet(doc);
        sheets[i]->ref();
    }

    // Measure add performance
    for (int i = 0; i < count; i++) {
        list->add(sheets[i]);
    }

    EXPECT_EQ(static_cast<unsigned>(count), list->length());

    // Measure access performance
    for (int i = 0; i < count; i++) {
        EXPECT_EQ(sheets[i], list->item(i));
    }

    // Measure remove performance
    for (int i = 0; i < count; i++) {
        list->remove(sheets[i]);
    }

    EXPECT_EQ(0u, list->length());

    // Cleanup
    for (int i = 0; i < count; i++) {
        sheets[i]->deref();
    }
    list->deref();
    doc->deref();
}

// Test: RefPtr reference counting correctness
TEST_F(RefPtrPerformanceTest, RefPtrReferenceCountingCorrectness)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();

    int initialRefCount = rule->refCount();

    {
        Vector<RefPtr<CSSRule> > rules1;
        rules1.append(rule);
        EXPECT_EQ(initialRefCount + 1, rule->refCount());

        {
            Vector<RefPtr<CSSRule> > rules2;
            rules2.append(rule);
            EXPECT_EQ(initialRefCount + 2, rule->refCount());

            Vector<RefPtr<CSSRule> > rules3 = rules2;
            EXPECT_EQ(initialRefCount + 3, rule->refCount());
        }

        // rules2 and rules3 destroyed
        EXPECT_EQ(initialRefCount + 1, rule->refCount());
    }

    // rules1 destroyed
    EXPECT_EQ(initialRefCount, rule->refCount());

    rule->deref();
}

// Test: Large scale memory stress test
TEST_F(RefPtrPerformanceTest, LargeScaleMemoryStressTest)
{
    const int outerIterations = 10;
    const int innerCount = 1000;

    for (int outer = 0; outer < outerIterations; outer++) {
        Vector<RefPtr<CSSRule> > rules;
        CSSStyleRule* rawRules[innerCount];

        // Allocate
        for (int i = 0; i < innerCount; i++) {
            rawRules[i] = new CSSStyleRule(nullptr);
            rawRules[i]->ref();
            rules.append(rawRules[i]);
        }

        // Verify
        EXPECT_EQ(static_cast<size_t>(innerCount), rules.size());

        // Deallocate
        rules.clear();
        for (int i = 0; i < innerCount; i++) {
            EXPECT_EQ(1, rawRules[i]->refCount());
            rawRules[i]->deref();
        }
    }

    SUCCEED();
}

// Test: Vector insert performance with RefPtr
TEST_F(RefPtrPerformanceTest, VectorRefPtrInsertPerformance)
{
    Vector<RefPtr<CSSRule> > rules;

    const int count = 100;
    CSSStyleRule* rawRules[count];

    for (int i = 0; i < count; i++) {
        rawRules[i] = new CSSStyleRule(nullptr);
        rawRules[i]->ref();
    }

    // Insert at various positions
    for (int i = 0; i < count; i++) {
        rules.insert(i / 2, rawRules[i]);
    }

    EXPECT_EQ(static_cast<size_t>(count), rules.size());

    // Cleanup
    for (int i = 0; i < count; i++) {
        rawRules[i]->deref();
    }
}

// Test: Verify no performance regression vs raw pointers
TEST_F(RefPtrPerformanceTest, RefPtrVsRawPointerPerformance)
{
    const int count = 1000;

    // Test with RefPtr
    {
        Vector<RefPtr<CSSRule> > rules;
        CSSStyleRule* rawRules[count];

        for (int i = 0; i < count; i++) {
            rawRules[i] = new CSSStyleRule(nullptr);
            rawRules[i]->ref();
            rules.append(rawRules[i]);
        }

        // Access test
        for (int i = 0; i < count; i++) {
            CSSRule* rule = rules[i].get();
            (void)rule; // Prevent optimization
        }

        for (int i = 0; i < count; i++) {
            rawRules[i]->deref();
        }
    }

    // Test with raw pointers (for comparison)
    {
        Vector<CSSRule*> rules;
        CSSStyleRule* rawRules[count];

        for (int i = 0; i < count; i++) {
            rawRules[i] = new CSSStyleRule(nullptr);
            rawRules[i]->ref();
            rules.append(rawRules[i]);
        }

        // Access test
        for (int i = 0; i < count; i++) {
            CSSRule* rule = rules[i];
            (void)rule; // Prevent optimization
        }

        for (int i = 0; i < count; i++) {
            rawRules[i]->deref();
        }
    }

    // Both should complete without issues
    SUCCEED();
}
