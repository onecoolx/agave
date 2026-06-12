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

#ifndef StorageArea_h
#define StorageArea_h

#include "PlatformString.h"
#include "StringHash.h"
#include "Shared.h"
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace WebCore {

// Web Storage limits, applied per area to bound memory/disk from untrusted
// pages. Mirrors typical browser quotas conservatively for an embedded engine.
const unsigned kMaxStorageEntries = 5000;
const unsigned kMaxStorageItemLength = 1024 * 1024; // 1MB per value

// Backend for a Storage object: an ordered string key/value map. Subclasses
// provide in-memory (sessionStorage) or persistent (localStorage) storage.
class StorageArea : public Shared<StorageArea> {
public:
    virtual ~StorageArea() { }

    virtual unsigned length() const = 0;
    virtual String key(unsigned index) const = 0;
    virtual String getItem(const String& key) const = 0;
    // Returns false (and does nothing) when a quota would be exceeded.
    virtual bool setItem(const String& key, const String& value) = 0;
    virtual void removeItem(const String& key) = 0;
    virtual void clear() = 0;
    virtual bool contains(const String& key) const = 0;
};

// In-memory backend used by sessionStorage. Insertion order is preserved so
// key(index) is stable across reads.
class MemoryStorageArea : public StorageArea {
public:
    static PassRefPtr<MemoryStorageArea> create() { return PassRefPtr<MemoryStorageArea>(new MemoryStorageArea()); }

    virtual unsigned length() const { return m_order.size(); }
    virtual String key(unsigned index) const;
    virtual String getItem(const String& key) const;
    virtual bool setItem(const String& key, const String& value);
    virtual void removeItem(const String& key);
    virtual void clear();
    virtual bool contains(const String& key) const { return m_map.contains(key); }

private:
    MemoryStorageArea() { }

    HashMap<String, String> m_map;
    Vector<String> m_order; // keys in insertion order
};

} // namespace WebCore

#endif // StorageArea_h
