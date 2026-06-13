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

#ifndef StyleCustomPropertyData_h
#define StyleCustomPropertyData_h

#include "PlatformString.h"
#include "Shared.h"
#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

// Inherited holder for an element's custom properties (--name -> value). Stored
// on RenderStyle and copied during inheritFrom() so custom properties inherit
// by default, per the CSS Custom Properties spec. Refcounted and copy-on-write:
// when an element does not set any custom property it simply shares its parent's
// map (or none), keeping the common case cheap.
class StyleCustomPropertyData : public Shared<StyleCustomPropertyData> {
public:
    static PassRefPtr<StyleCustomPropertyData> create()
    {
        return RefPtr<StyleCustomPropertyData>(new StyleCustomPropertyData());
    }

    PassRefPtr<StyleCustomPropertyData> copy() const
    {
        return RefPtr<StyleCustomPropertyData>(new StyleCustomPropertyData(*this));
    }

    bool operator==(const StyleCustomPropertyData& other) const
    {
        return m_values == other.m_values;
    }
    bool operator!=(const StyleCustomPropertyData& other) const
    {
        return !(*this == other);
    }

    void setProperty(const String& name, const String& value)
    {
        m_values.set(name, value);
    }

    String getProperty(const String& name, bool& found) const
    {
        HashMap<String, String>::const_iterator it = m_values.find(name);
        if (it == m_values.end()) {
            found = false;
            return String();
        }
        found = true;
        return it->second;
    }

    bool isEmpty() const { return m_values.isEmpty(); }

private:
    StyleCustomPropertyData() { }
    StyleCustomPropertyData(const StyleCustomPropertyData& o) : Shared<StyleCustomPropertyData>(), m_values(o.m_values) { }

    HashMap<String, String> m_values;
};

} // namespace WebCore

#endif // StyleCustomPropertyData_h
