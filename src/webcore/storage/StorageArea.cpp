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
#include "StorageArea.h"

namespace WebCore {

String MemoryStorageArea::key(unsigned index) const
{
    if (index >= m_order.size())
        return String(); // null
    return m_order[index];
}

String MemoryStorageArea::getItem(const String& key) const
{
    HashMap<String, String>::const_iterator it = m_map.find(key);
    if (it == m_map.end())
        return String(); // null
    return it->second;
}

bool MemoryStorageArea::setItem(const String& key, const String& value)
{
    if (value.length() > kMaxStorageItemLength)
        return false;
    if (!m_map.contains(key)) {
        if (m_order.size() >= kMaxStorageEntries)
            return false; // quota exceeded
        m_order.append(key);
    }
    m_map.set(key, value);
    return true;
}

void MemoryStorageArea::removeItem(const String& key)
{
    if (!m_map.contains(key))
        return;
    m_map.remove(key);
    for (size_t i = 0; i < m_order.size(); i++) {
        if (m_order[i] == key) {
            m_order.remove(i);
            break;
        }
    }
}

void MemoryStorageArea::clear()
{
    m_map.clear();
    m_order.clear();
}

} // namespace WebCore
