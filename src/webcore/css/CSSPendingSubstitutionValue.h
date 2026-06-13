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

#ifndef CSSPendingSubstitutionValue_h
#define CSSPendingSubstitutionValue_h

#include "CSSValue.h"
#include "PlatformString.h"

namespace WebCore {

// A property value that contains var() references and therefore cannot be
// resolved at parse time (the referenced custom properties are only known
// after the cascade). It carries the target property id and the original value
// text; the style selector substitutes var() at apply time and re-parses.
class CSSPendingSubstitutionValue : public CSSValue {
public:
    CSSPendingSubstitutionValue(int propertyId, const String& text)
        : m_propertyId(propertyId), m_text(text) { }
    virtual ~CSSPendingSubstitutionValue() { }

    virtual String cssText() const { return m_text; }

    virtual bool isPendingSubstitutionValue() const { return true; }

    int propertyId() const { return m_propertyId; }
    const String& text() const { return m_text; }

private:
    int m_propertyId;
    String m_text;
};

} // namespace WebCore

#endif // CSSPendingSubstitutionValue_h
