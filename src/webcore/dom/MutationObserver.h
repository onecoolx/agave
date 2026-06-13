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

#ifndef MutationObserver_h
#define MutationObserver_h

#include "config.h"

#if ENABLE(MUTATION_OBSERVERS)

#include "MutationRecord.h"
#include "Shared.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class Node;
class MutationObserver;

// Bitmask of what a registration observes.
enum MutationObserverOption {
    MutationObserveChildList      = 1 << 0,
    MutationObserveAttributes     = 1 << 1,
    MutationObserveCharacterData  = 1 << 2,
    MutationObserveSubtree        = 1 << 3,
    MutationAttributeOldValue     = 1 << 4,
    MutationCharacterDataOldValue = 1 << 5
};

// Implemented by the binding layer to invoke the JS callback with the records.
class MutationCallback {
public:
    virtual ~MutationCallback() { }
    virtual void deliver(const Vector<RefPtr<MutationRecord> >& records, MutationObserver*) = 0;
    // Called when the observer is destroyed so the binding can release its
    // hold on the JS object/callback.
    virtual void observerDestroyed() = 0;
};

class MutationObserver : public Shared<MutationObserver> {
public:
    static PassRefPtr<MutationObserver> create(MutationCallback* callback)
    {
        return RefPtr<MutationObserver>(new MutationObserver(callback));
    }
    ~MutationObserver();

    void observe(Node* target, unsigned options);
    void disconnect();
    void takeRecords(Vector<RefPtr<MutationRecord> >& outRecords);

    // Queue a record for later microtask delivery. Schedules delivery on first
    // enqueue.
    void enqueueRecord(PassRefPtr<MutationRecord>);

    // Invoked by the delivery microtask; flushes the queue to the callback.
    void deliverRecords();

    bool hasPendingDelivery() const { return m_pendingDelivery; }
    void setPendingDelivery(bool b) { m_pendingDelivery = b; }

private:
    explicit MutationObserver(MutationCallback*);

    MutationCallback* m_callback; // owned by the binding wrapper
    Vector<RefPtr<MutationRecord> > m_records;
    bool m_pendingDelivery;
};

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)

#endif // MutationObserver_h
