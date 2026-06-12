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

#ifndef Storage_h
#define Storage_h

#include "StorageArea.h"
#include "Shared.h"
#include <wtf/RefPtr.h>

namespace WebCore {

typedef int ExceptionCode;

// The Web Storage DOM object (window.localStorage / window.sessionStorage).
// Thin facade over a StorageArea backend.
class Storage : public Shared<Storage> {
public:
    static PassRefPtr<Storage> create(PassRefPtr<StorageArea> area)
    {
        return PassRefPtr<Storage>(new Storage(area));
    }

    unsigned length() const { return m_area->length(); }
    String key(unsigned index) const { return m_area->key(index); }
    String getItem(const String& key) const { return m_area->getItem(key); }
    void setItem(const String& key, const String& value, ExceptionCode&);
    void removeItem(const String& key) { m_area->removeItem(key); }
    void clear() { m_area->clear(); }
    bool contains(const String& key) const { return m_area->contains(key); }

private:
    Storage(PassRefPtr<StorageArea> area) : m_area(area) { }

    RefPtr<StorageArea> m_area;
};

} // namespace WebCore

#endif // Storage_h
