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

#ifndef MutationObserverRegistry_h
#define MutationObserverRegistry_h

#include "config.h"

#if ENABLE(MUTATION_OBSERVERS)

#include "PlatformString.h"

namespace WebCore {

class Node;
class MutationObserver;

// Global registry of MutationObserver registrations, plus the DOM-side change
// notification entry points. Hooks in ContainerNode / StyledElement /
// CharacterData call the notify* functions; when no observers exist these
// return immediately (gObserverCount guard) to keep mutation hot paths cheap.
namespace MutationObserverRegistry {

    // True when at least one observer is registered (hot-path guard for hooks).
    bool hasObservers();

    void registerObserver(MutationObserver*, Node* target, unsigned options);
    void unregisterObserver(MutationObserver*);   // disconnect()
    void observerDestroyed(MutationObserver*);     // ~MutationObserver
    // Called from ~Node so the registry never retains a dangling node key.
    void nodeDestroyed(Node*);

    // Delivery scheduling: the binding layer installs a hook that schedules a
    // microtask; scheduleDelivery() invokes it (once) when records are queued.
    typedef void (*DeliveryScheduler)();
    void setDeliveryScheduler(DeliveryScheduler);
    void scheduleDelivery(MutationObserver*);
    // Runs by the scheduled microtask: delivers all observers with pending records.
    void deliverAllPending();

    // DOM change notification entry points (called from mutation hot paths).
    void notifyChildListChanged(Node* target, Node* added, Node* removed,
                                Node* previousSibling, Node* nextSibling);
    void notifyAttributeChanged(Node* target, const String& attributeName, const String& oldValue);
    void notifyCharacterDataChanged(Node* target, const String& oldValue);

} // namespace MutationObserverRegistry

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)

#endif // MutationObserverRegistry_h
