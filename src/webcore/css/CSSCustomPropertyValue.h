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

#ifndef CSSCustomPropertyValue_h
#define CSSCustomPropertyValue_h

#include "CSSValue.h"
#include "PlatformString.h"

namespace WebCore {

// Sentinel property id for custom properties, chosen above the generated
// CSSPropertyNames range (max 243) so it never collides. The applyProperty()
// switch ignores unknown ids by default; the cascade handles custom properties
// explicitly (Stage B).
const int CSS_PROP_CUSTOM_PROPERTY = 0x4000;

// Holds a CSS custom property declaration ("--name: value"). The name retains
// its leading "--" and the value is kept as its serialized text so it can be
// substituted into var() references later (Stage C). This is the parse-and-store
// representation produced by the parser (Stage A); the cascade reads it into the
// computed custom-property map (Stage B).
class CSSCustomPropertyValue : public CSSValue {
public:
    CSSCustomPropertyValue(const String& name, const String& value)
        : m_name(name), m_value(value) { }

    const String& name() const { return m_name; }
    const String& value() const { return m_value; }

    virtual String cssText() const { return m_value; }
    virtual bool isCustomPropertyValue() const { return true; }

private:
    String m_name;
    String m_value;
};

} // namespace WebCore

#endif // CSSCustomPropertyValue_h
