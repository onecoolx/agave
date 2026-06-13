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

#ifndef MutationRecord_h
#define MutationRecord_h

#include "config.h"

#if ENABLE(MUTATION_OBSERVERS)

#include "NodeList.h"
#include "PlatformString.h"
#include "Shared.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class Node;

// A single observed DOM change, delivered to a MutationObserver callback.
class MutationRecord : public Shared<MutationRecord> {
public:
    static PassRefPtr<MutationRecord> createChildList(Node* target, PassRefPtr<NodeList> added,
        PassRefPtr<NodeList> removed, Node* previousSibling, Node* nextSibling);
    static PassRefPtr<MutationRecord> createAttributes(Node* target, const String& attributeName,
        const String& oldValue, bool hasOldValue);
    static PassRefPtr<MutationRecord> createCharacterData(Node* target, const String& oldValue,
        bool hasOldValue);

    ~MutationRecord();

    const String& type() const { return m_type; }
    Node* target() const { return m_target.get(); }
    NodeList* addedNodes() const { return m_addedNodes.get(); }
    NodeList* removedNodes() const { return m_removedNodes.get(); }
    Node* previousSibling() const { return m_previousSibling.get(); }
    Node* nextSibling() const { return m_nextSibling.get(); }
    const String& attributeName() const { return m_attributeName; }
    const String& oldValue() const { return m_oldValue; }
    bool hasOldValue() const { return m_hasOldValue; }

private:
    MutationRecord(const String& type, Node* target);

    String m_type;
    RefPtr<Node> m_target;
    RefPtr<NodeList> m_addedNodes;
    RefPtr<NodeList> m_removedNodes;
    RefPtr<Node> m_previousSibling;
    RefPtr<Node> m_nextSibling;
    String m_attributeName;
    String m_oldValue;
    bool m_hasOldValue;
};

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)

#endif // MutationRecord_h
