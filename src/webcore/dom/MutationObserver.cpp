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

#include "config.h"

#if ENABLE(MUTATION_OBSERVERS)

#include "MutationObserver.h"

#include "MutationObserverRegistry.h"
#include "Node.h"

namespace WebCore {

MutationObserver::MutationObserver(MutationCallback* callback)
    : m_callback(callback)
    , m_pendingDelivery(false)
{
}

MutationObserver::~MutationObserver()
{
    // Drop all registrations referencing this observer so the registry holds no
    // dangling pointers.
    MutationObserverRegistry::observerDestroyed(this);
    if (m_callback)
        m_callback->observerDestroyed();
}

void MutationObserver::observe(Node* target, unsigned options)
{
    if (!target)
        return;
    MutationObserverRegistry::registerObserver(this, target, options);
}

void MutationObserver::disconnect()
{
    MutationObserverRegistry::unregisterObserver(this);
    m_records.clear();
    m_pendingDelivery = false;
}

void MutationObserver::takeRecords(Vector<RefPtr<MutationRecord> >& outRecords)
{
    outRecords.swap(m_records);
    m_records.clear();
}

void MutationObserver::enqueueRecord(PassRefPtr<MutationRecord> record)
{
    m_records.append(record);
    MutationObserverRegistry::scheduleDelivery(this);
}

void MutationObserver::deliverRecords()
{
    m_pendingDelivery = false;
    if (m_records.isEmpty() || !m_callback)
        return;
    Vector<RefPtr<MutationRecord> > records;
    records.swap(m_records);
    m_callback->deliver(records, this);
}

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)
