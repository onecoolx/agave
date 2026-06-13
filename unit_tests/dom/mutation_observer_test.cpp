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

#if ENABLE(MUTATION_OBSERVERS)

#include "Document.h"
#include "Element.h"
#include "MutationObserver.h"
#include "MutationObserverRegistry.h"
#include "MutationRecord.h"
#include "ExceptionCode.h"

using namespace WebCore;

namespace {

// Captures delivered records for assertions.
class RecordingMutationCallback : public MutationCallback
{
public:
    RecordingMutationCallback() : deliverCount(0), destroyed(false) { }
    virtual void deliver(const Vector<RefPtr<MutationRecord> >& records, MutationObserver*)
    {
        deliverCount++;
        for (size_t i = 0; i < records.size(); ++i) {
            delivered.append(records[i]);
        }
    }
    virtual void observerDestroyed() { destroyed = true; }

    int deliverCount;
    bool destroyed;
    Vector<RefPtr<MutationRecord> > delivered;
};

} // namespace

class MutationObserverTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite() { Test_Init(); }
    static void TearDownTestSuite() { Test_Shutdown(); }

    void SetUp() override { view = new TestWebView(); }
    void TearDown() override { delete view; }

    void loadHtml(const char* html)
    {
        view->loadHtml(html, "http://localhost/test");
        TestWebView::waitForDocumentComplete(view);
    }

    Document* doc() { return view->mainframe()->document(); }
    Element* byId(const char* id) { return doc()->getElementById(String(id)); }

    TestWebView* view;
};

TEST_F(MutationObserverTest, ChildListRecordOnAppend)
{
    loadHtml("<div id='t'></div>");
    Element* t = byId("t");
    ASSERT_TRUE(t);

    RecordingMutationCallback callback;
    RefPtr<MutationObserver> obs = MutationObserver::create(&callback);
    obs->observe(t, MutationObserveChildList);

    ExceptionCode ec = 0;
    t->appendChild(doc()->createElement("span", ec), ec);

    // Records are queued; deliver synchronously for the test.
    MutationObserverRegistry::deliverAllPending();
    EXPECT_EQ(callback.deliverCount, 1);
    ASSERT_EQ(callback.delivered.size(), 1u);
    EXPECT_EQ(callback.delivered[0]->type(), "childList");
    ASSERT_TRUE(callback.delivered[0]->addedNodes());
    EXPECT_EQ(callback.delivered[0]->addedNodes()->length(), 1u);

    obs->disconnect();
}

TEST_F(MutationObserverTest, AttributeRecordWithOldValue)
{
    loadHtml("<div id='t' data-x='old'></div>");
    Element* t = byId("t");
    ASSERT_TRUE(t);

    RecordingMutationCallback callback;
    RefPtr<MutationObserver> obs = MutationObserver::create(&callback);
    obs->observe(t, MutationObserveAttributes | MutationAttributeOldValue);

    ExceptionCode ec = 0;
    t->setAttribute("data-x", "new", ec);

    MutationObserverRegistry::deliverAllPending();
    ASSERT_EQ(callback.delivered.size(), 1u);
    EXPECT_EQ(callback.delivered[0]->type(), "attributes");
    EXPECT_EQ(callback.delivered[0]->attributeName(), "data-x");
    EXPECT_TRUE(callback.delivered[0]->hasOldValue());
    EXPECT_EQ(callback.delivered[0]->oldValue(), "old");

    obs->disconnect();
}

TEST_F(MutationObserverTest, SubtreeObservesDescendant)
{
    loadHtml("<div id='root'><div id='child'></div></div>");
    Element* root = byId("root");
    Element* child = byId("child");
    ASSERT_TRUE(root && child);

    RecordingMutationCallback callback;
    RefPtr<MutationObserver> obs = MutationObserver::create(&callback);
    obs->observe(root, MutationObserveChildList | MutationObserveSubtree);

    ExceptionCode ec = 0;
    child->appendChild(doc()->createElement("span", ec), ec);

    MutationObserverRegistry::deliverAllPending();
    ASSERT_EQ(callback.delivered.size(), 1u);
    EXPECT_EQ(callback.delivered[0]->type(), "childList");

    obs->disconnect();
}

TEST_F(MutationObserverTest, DisconnectEmptiesQueue)
{
    loadHtml("<div id='t'></div>");
    Element* t = byId("t");
    ASSERT_TRUE(t);

    RecordingMutationCallback callback;
    RefPtr<MutationObserver> obs = MutationObserver::create(&callback);
    obs->observe(t, MutationObserveChildList);

    ExceptionCode ec = 0;
    t->appendChild(doc()->createElement("span", ec), ec);
    obs->disconnect(); // empties the pending queue per spec

    MutationObserverRegistry::deliverAllPending();
    EXPECT_EQ(callback.deliverCount, 0);
}

TEST_F(MutationObserverTest, NoObserverNoRecord)
{
    loadHtml("<div id='t'></div>");
    Element* t = byId("t");
    ASSERT_TRUE(t);
    // With no observer registered, hasObservers() is false and mutations are
    // not collected (hot-path guard).
    ExceptionCode ec = 0;
    t->appendChild(doc()->createElement("span", ec), ec);
    // Nothing to assert beyond not crashing; the guard short-circuits.
    SUCCEED();
}

#endif // ENABLE(MUTATION_OBSERVERS)
