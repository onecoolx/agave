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

#include "MutationObserverRegistry.h"

#include "MutationObserver.h"
#include "MutationRecord.h"
#include "Node.h"
#include "StaticNodeList.h"
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace WebCore {

namespace MutationObserverRegistry {

struct Registration {
    MutationObserver* observer;
    unsigned options;
};

typedef Vector<Registration> RegistrationList;
typedef HashMap<Node*, RegistrationList*> NodeRegistrationMap;

// All node->registrations. Pointer values; cleaned up on unregister/destroy.
static NodeRegistrationMap& registrations()
{
    static NodeRegistrationMap map;
    return map;
}

// Observers with pending delivery (delivered by the scheduled microtask).
static Vector<RefPtr<MutationObserver> >& pendingObservers()
{
    static Vector<RefPtr<MutationObserver> > observers;
    return observers;
}

static DeliveryScheduler s_scheduler = 0;
static unsigned s_observerCount = 0; // number of nodes with >=1 registration

bool hasObservers()
{
    return s_observerCount > 0;
}

void setDeliveryScheduler(DeliveryScheduler scheduler)
{
    s_scheduler = scheduler;
}

void registerObserver(MutationObserver* observer, Node* target, unsigned options)
{
    RegistrationList* list = registrations().get(target);
    if (!list) {
        list = new RegistrationList();
        registrations().set(target, list);
    }
    // Replace an existing registration of the same observer on this node
    // (observe() called again with new options), otherwise append.
    for (size_t i = 0; i < list->size(); ++i) {
        if (list->at(i).observer == observer) {
            list->at(i).options = options;
            return;
        }
    }
    Registration reg;
    reg.observer = observer;
    reg.options = options;
    list->append(reg);
    ++s_observerCount;
}

// Removes every registration belonging to |observer| across all nodes.
static void removeObserverEverywhere(MutationObserver* observer)
{
    Vector<Node*> emptied;
    NodeRegistrationMap::iterator end = registrations().end();
    for (NodeRegistrationMap::iterator it = registrations().begin(); it != end; ++it) {
        RegistrationList* list = it->second;
        for (size_t i = 0; i < list->size(); ) {
            if (list->at(i).observer == observer) {
                list->remove(i);
                if (s_observerCount)
                    --s_observerCount;
            } else
                ++i;
        }
        if (list->isEmpty())
            emptied.append(it->first);
    }
    for (size_t i = 0; i < emptied.size(); ++i) {
        RegistrationList* list = registrations().get(emptied[i]);
        registrations().remove(emptied[i]);
        delete list;
    }
}

void unregisterObserver(MutationObserver* observer)
{
    removeObserverEverywhere(observer);
}

void nodeDestroyed(Node* node)
{
    RegistrationList* list = registrations().get(node);
    if (!list)
        return;
    if (s_observerCount >= list->size())
        s_observerCount -= list->size();
    registrations().remove(node);
    delete list;
}

void observerDestroyed(MutationObserver* observer)
{
    removeObserverEverywhere(observer);
    // Drop any pending-delivery reference to the dying observer.
    Vector<RefPtr<MutationObserver> >& pending = pendingObservers();
    for (size_t i = 0; i < pending.size(); ) {
        if (pending[i].get() == observer)
            pending.remove(i);
        else
            ++i;
    }
}

void scheduleDelivery(MutationObserver* observer)
{
    if (!observer->hasPendingDelivery()) {
        observer->setPendingDelivery(true);
        pendingObservers().append(observer);
    }
    if (s_scheduler)
        s_scheduler();
}

void deliverAllPending()
{
    // Swap out the pending list so observers re-queued during delivery are
    // scheduled for the next round rather than mutated mid-iteration.
    Vector<RefPtr<MutationObserver> > observers;
    observers.swap(pendingObservers());
    for (size_t i = 0; i < observers.size(); ++i)
        observers[i]->deliverRecords();
}

// Collects matching observers for a change on |target|: the target's own
// registrations always apply for the matching type; ancestor registrations
// apply only when subtree:true.
static void enqueueToMatchingObservers(Node* target, unsigned typeFlag,
    PassRefPtr<MutationRecord> prototypeRecord)
{
    RefPtr<MutationRecord> record = prototypeRecord;
    for (Node* node = target; node; node = node->parentNode()) {
        RegistrationList* list = registrations().get(node);
        if (!list)
            continue;
        bool isTarget = (node == target);
        for (size_t i = 0; i < list->size(); ++i) {
            const Registration& reg = list->at(i);
            if (!(reg.options & typeFlag))
                continue;
            if (!isTarget && !(reg.options & MutationObserveSubtree))
                continue;
            reg.observer->enqueueRecord(record);
        }
    }
}

void notifyChildListChanged(Node* target, Node* added, Node* removed,
                            Node* previousSibling, Node* nextSibling)
{
    if (!hasObservers() || !target)
        return;

    RefPtr<NodeList> addedList;
    RefPtr<NodeList> removedList;
    if (added) {
        Vector<RefPtr<Node> > nodes;
        nodes.append(added);
        addedList = StaticNodeList::adopt(target, nodes);
    }
    if (removed) {
        Vector<RefPtr<Node> > nodes;
        nodes.append(removed);
        removedList = StaticNodeList::adopt(target, nodes);
    }
    enqueueToMatchingObservers(target, MutationObserveChildList,
        MutationRecord::createChildList(target, addedList, removedList, previousSibling, nextSibling));
}

void notifyAttributeChanged(Node* target, const String& attributeName, const String& oldValue)
{
    if (!hasObservers() || !target)
        return;
    // Each observer needs oldValue only when it asked for it; we build the
    // record per the spec's "interested observers" but keep it simple by
    // including oldValue and letting the binding gate exposure. To honor
    // attributeOldValue precisely we create per-observer records below.
    for (Node* node = target; node; node = node->parentNode()) {
        RegistrationList* list = registrations().get(node);
        if (!list)
            continue;
        bool isTarget = (node == target);
        for (size_t i = 0; i < list->size(); ++i) {
            const Registration& reg = list->at(i);
            if (!(reg.options & MutationObserveAttributes))
                continue;
            if (!isTarget && !(reg.options & MutationObserveSubtree))
                continue;
            bool wantOld = (reg.options & MutationAttributeOldValue) != 0;
            reg.observer->enqueueRecord(
                MutationRecord::createAttributes(target, attributeName, wantOld ? oldValue : String(), wantOld));
        }
    }
}

void notifyCharacterDataChanged(Node* target, const String& oldValue)
{
    if (!hasObservers() || !target)
        return;
    for (Node* node = target; node; node = node->parentNode()) {
        RegistrationList* list = registrations().get(node);
        if (!list)
            continue;
        bool isTarget = (node == target);
        for (size_t i = 0; i < list->size(); ++i) {
            const Registration& reg = list->at(i);
            if (!(reg.options & MutationObserveCharacterData))
                continue;
            if (!isTarget && !(reg.options & MutationObserveSubtree))
                continue;
            bool wantOld = (reg.options & MutationCharacterDataOldValue) != 0;
            reg.observer->enqueueRecord(
                MutationRecord::createCharacterData(target, wantOld ? oldValue : String(), wantOld));
        }
    }
}

} // namespace MutationObserverRegistry

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)
