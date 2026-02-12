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
#include "CSSRuleList.h"
#include "CSSStyleRule.h"

class CSSRuleListTest : public ::testing::Test
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
        ruleList = new CSSRuleList();
    }

    void TearDown() override
    {
        ruleList->deref();
    }

    CSSRuleList* ruleList;
};

// Test: Empty list initialization
TEST_F(CSSRuleListTest, EmptyListInitialization)
{
    EXPECT_EQ(0u, ruleList->length());
    EXPECT_EQ(nullptr, ruleList->item(0));
}

// Test: Append single rule
TEST_F(CSSRuleListTest, AppendSingleRule)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    ruleList->append(rule);
    
    EXPECT_EQ(1u, ruleList->length());
    EXPECT_EQ(rule, ruleList->item(0));
    
    rule->deref();
}

// Test: Append multiple rules
TEST_F(CSSRuleListTest, AppendMultipleRules)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule3 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    rule3->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    ruleList->append(rule3);
    
    EXPECT_EQ(3u, ruleList->length());
    EXPECT_EQ(rule1, ruleList->item(0));
    EXPECT_EQ(rule2, ruleList->item(1));
    EXPECT_EQ(rule3, ruleList->item(2));
    
    rule1->deref();
    rule2->deref();
    rule3->deref();
}

// Test: Insert rule at beginning
TEST_F(CSSRuleListTest, InsertRuleAtBeginning)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    
    ruleList->append(rule1);
    unsigned index = ruleList->insertRule(rule2, 0);
    
    EXPECT_EQ(0u, index);
    EXPECT_EQ(2u, ruleList->length());
    EXPECT_EQ(rule2, ruleList->item(0));
    EXPECT_EQ(rule1, ruleList->item(1));
    
    rule1->deref();
    rule2->deref();
}

// Test: Insert rule in middle
TEST_F(CSSRuleListTest, InsertRuleInMiddle)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule3 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    rule3->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule3);
    unsigned index = ruleList->insertRule(rule2, 1);
    
    EXPECT_EQ(1u, index);
    EXPECT_EQ(3u, ruleList->length());
    EXPECT_EQ(rule1, ruleList->item(0));
    EXPECT_EQ(rule2, ruleList->item(1));
    EXPECT_EQ(rule3, ruleList->item(2));
    
    rule1->deref();
    rule2->deref();
    rule3->deref();
}

// Test: Insert rule at end
TEST_F(CSSRuleListTest, InsertRuleAtEnd)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    
    ruleList->append(rule1);
    unsigned index = ruleList->insertRule(rule2, 1);
    
    EXPECT_EQ(1u, index);
    EXPECT_EQ(2u, ruleList->length());
    EXPECT_EQ(rule2, ruleList->item(1));
    
    rule1->deref();
    rule2->deref();
}

// Test: Delete rule at beginning
TEST_F(CSSRuleListTest, DeleteRuleAtBeginning)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    
    ruleList->deleteRule(0);
    
    EXPECT_EQ(1u, ruleList->length());
    EXPECT_EQ(rule2, ruleList->item(0));
    
    rule1->deref();
    rule2->deref();
}

// Test: Delete rule in middle
TEST_F(CSSRuleListTest, DeleteRuleInMiddle)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule3 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    rule3->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    ruleList->append(rule3);
    
    ruleList->deleteRule(1);
    
    EXPECT_EQ(2u, ruleList->length());
    EXPECT_EQ(rule1, ruleList->item(0));
    EXPECT_EQ(rule3, ruleList->item(1));
    
    rule1->deref();
    rule2->deref();
    rule3->deref();
}

// Test: Delete rule at end
TEST_F(CSSRuleListTest, DeleteRuleAtEnd)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    
    ruleList->deleteRule(1);
    
    EXPECT_EQ(1u, ruleList->length());
    EXPECT_EQ(rule1, ruleList->item(0));
    
    rule1->deref();
    rule2->deref();
}

// Test: Delete rule with invalid index
TEST_F(CSSRuleListTest, DeleteRuleInvalidIndex)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    ruleList->append(rule);
    
    // Delete with out-of-bounds index should not crash
    ruleList->deleteRule(10);
    
    // List should remain unchanged
    EXPECT_EQ(1u, ruleList->length());
    
    rule->deref();
}

// Test: Item with invalid index
TEST_F(CSSRuleListTest, ItemInvalidIndex)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    ruleList->append(rule);
    
    EXPECT_EQ(nullptr, ruleList->item(10));
    EXPECT_EQ(nullptr, ruleList->item(1));
    
    rule->deref();
}

// Test: Insert with invalid index
TEST_F(CSSRuleListTest, InsertRuleInvalidIndex)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    // Insert with out-of-bounds index should fail
    unsigned index = ruleList->insertRule(rule, 10);
    
    EXPECT_EQ(0u, index);  // Should return 0 on failure
    EXPECT_EQ(0u, ruleList->length());
    
    rule->deref();
}

// Test: Insert null rule
TEST_F(CSSRuleListTest, InsertNullRule)
{
    unsigned index = ruleList->insertRule(nullptr, 0);
    
    EXPECT_EQ(0u, index);  // Should return 0 on failure
    EXPECT_EQ(0u, ruleList->length());
}

// Test: Reference counting - rules are properly managed
TEST_F(CSSRuleListTest, ReferenceCountingOnAppend)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    int initialRefCount = rule->refCount();
    
    ruleList->append(rule);
    
    // RefPtr should have increased ref count
    EXPECT_GT(rule->refCount(), initialRefCount);
    
    rule->deref();
}

// Test: Reference counting on delete
TEST_F(CSSRuleListTest, ReferenceCountingOnDelete)
{
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    ruleList->append(rule);
    int refCountAfterAppend = rule->refCount();
    
    ruleList->deleteRule(0);
    
    // RefPtr should have decreased ref count
    EXPECT_LT(rule->refCount(), refCountAfterAppend);
    
    rule->deref();
}

// Test: Multiple operations sequence
TEST_F(CSSRuleListTest, MultipleOperationsSequence)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule3 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule4 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    rule3->ref();
    rule4->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    ruleList->insertRule(rule3, 1);
    ruleList->append(rule4);
    ruleList->deleteRule(2);
    
    EXPECT_EQ(3u, ruleList->length());
    EXPECT_EQ(rule1, ruleList->item(0));
    EXPECT_EQ(rule3, ruleList->item(1));
    EXPECT_EQ(rule4, ruleList->item(2));
    
    rule1->deref();
    rule2->deref();
    rule3->deref();
    rule4->deref();
}

// Test: Clear all rules by deleting
TEST_F(CSSRuleListTest, ClearAllRules)
{
    CSSStyleRule* rule1 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule2 = new CSSStyleRule(nullptr);
    CSSStyleRule* rule3 = new CSSStyleRule(nullptr);
    rule1->ref();
    rule2->ref();
    rule3->ref();
    
    ruleList->append(rule1);
    ruleList->append(rule2);
    ruleList->append(rule3);
    
    ruleList->deleteRule(0);
    ruleList->deleteRule(0);
    ruleList->deleteRule(0);
    
    EXPECT_EQ(0u, ruleList->length());
    
    rule1->deref();
    rule2->deref();
    rule3->deref();
}

// Test: Large number of rules
TEST_F(CSSRuleListTest, LargeNumberOfRules)
{
    const int numRules = 100;
    CSSStyleRule* rules[numRules];
    
    for (int i = 0; i < numRules; i++) {
        rules[i] = new CSSStyleRule(nullptr);
        rules[i]->ref();
        ruleList->append(rules[i]);
    }
    
    EXPECT_EQ(static_cast<unsigned>(numRules), ruleList->length());
    
    for (int i = 0; i < numRules; i++) {
        EXPECT_EQ(rules[i], ruleList->item(i));
    }
    
    for (int i = 0; i < numRules; i++) {
        rules[i]->deref();
    }
}

// Test: Destructor properly cleans up
TEST_F(CSSRuleListTest, DestructorCleansUp)
{
    CSSRuleList* tempList = new CSSRuleList();
    tempList->ref();
    
    CSSStyleRule* rule = new CSSStyleRule(nullptr);
    rule->ref();
    
    tempList->append(rule);
    int refCountAfterAppend = rule->refCount();
    
    tempList->deref();  // Should trigger destructor
    
    // Rule's ref count should have decreased
    EXPECT_LT(rule->refCount(), refCountAfterAppend);
    
    rule->deref();
}
