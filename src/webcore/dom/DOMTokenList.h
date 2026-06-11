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

#ifndef DOMTokenList_h
#define DOMTokenList_h

#include "PlatformString.h"
#include "Shared.h"
#include <wtf/Vector.h>

namespace WebCore {

class Element;
typedef int ExceptionCode;

// Implements the DOMTokenList interface backing Element.classList. Operates on
// the element's class attribute: tokens are whitespace-separated, deduplicated
// on write, and changes are written straight back to the attribute (so
// className and classList stay in sync).
class DOMTokenList : public Shared<DOMTokenList> {
public:
    DOMTokenList(Element* element) : m_element(element) { }

    unsigned length() const;
    String item(unsigned index) const;
    bool contains(const String& token) const;
    void add(const String& token, ExceptionCode&);
    void remove(const String& token, ExceptionCode&);
    bool toggle(const String& token, ExceptionCode&);
    String toString() const;

    void ref() { Shared<DOMTokenList>::ref(); }
    void deref() { Shared<DOMTokenList>::deref(); }

private:
    Vector<String> tokens() const;
    void writeTokens(const Vector<String>&);
    static bool isValidToken(const String&, ExceptionCode&);

    Element* m_element; // not owned; the element owns this list
};

} // namespace WebCore

#endif // DOMTokenList_h
