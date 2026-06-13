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

#include "MutationRecord.h"

#include "Node.h"

namespace WebCore {

MutationRecord::MutationRecord(const String& type, Node* target)
    : m_type(type)
    , m_target(target)
    , m_hasOldValue(false)
{
}

MutationRecord::~MutationRecord()
{
}

PassRefPtr<MutationRecord> MutationRecord::createChildList(Node* target, PassRefPtr<NodeList> added,
    PassRefPtr<NodeList> removed, Node* previousSibling, Node* nextSibling)
{
    RefPtr<MutationRecord> record = new MutationRecord("childList", target);
    record->m_addedNodes = added;
    record->m_removedNodes = removed;
    record->m_previousSibling = previousSibling;
    record->m_nextSibling = nextSibling;
    return record.release();
}

PassRefPtr<MutationRecord> MutationRecord::createAttributes(Node* target, const String& attributeName,
    const String& oldValue, bool hasOldValue)
{
    RefPtr<MutationRecord> record = new MutationRecord("attributes", target);
    record->m_attributeName = attributeName;
    record->m_oldValue = oldValue;
    record->m_hasOldValue = hasOldValue;
    return record.release();
}

PassRefPtr<MutationRecord> MutationRecord::createCharacterData(Node* target, const String& oldValue,
    bool hasOldValue)
{
    RefPtr<MutationRecord> record = new MutationRecord("characterData", target);
    record->m_oldValue = oldValue;
    record->m_hasOldValue = hasOldValue;
    return record.release();
}

} // namespace WebCore

#endif // ENABLE(MUTATION_OBSERVERS)
