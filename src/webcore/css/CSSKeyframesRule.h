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

#ifndef CSSKeyframesRule_h
#define CSSKeyframesRule_h

#include "config.h"

#if ENABLE(CSS_TRANSITIONS)

#include "CSSRule.h"
#include "PlatformString.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class CSSKeyframeRule;

// A CSS "@keyframes name { ... }" rule: a named, ordered list of keyframes.
class CSSKeyframesRule : public CSSRule {
public:
    CSSKeyframesRule(StyleBase* parent);
    virtual ~CSSKeyframesRule();

    virtual bool isKeyframesRule() { return true; }
    virtual unsigned short type() const { return WEBKIT_KEYFRAMES_RULE; }
    virtual String cssText() const { return String(); }

    const String& name() const { return m_name; }
    void setName(const String& name) { m_name = name; }

    void append(PassRefPtr<CSSKeyframeRule>);
    unsigned length() const { return m_keyframes.size(); }
    CSSKeyframeRule* item(unsigned index) const
    {
        return index < m_keyframes.size() ? m_keyframes[index].get() : 0;
    }

private:
    String m_name;
    Vector<RefPtr<CSSKeyframeRule> > m_keyframes;
};

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)

#endif // CSSKeyframesRule_h
