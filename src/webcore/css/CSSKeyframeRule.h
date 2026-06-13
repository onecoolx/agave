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

#ifndef CSSKeyframeRule_h
#define CSSKeyframeRule_h

#include "config.h"

#if ENABLE(CSS_TRANSITIONS)

#include "CSSRule.h"
#include "PlatformString.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

class CSSMutableStyleDeclaration;

// One keyframe inside an @keyframes rule, e.g. "0%, 50% { opacity: 1; }". The
// keys are the offsets in [0,1] (from = 0, to = 1) at which the declaration
// block applies.
class CSSKeyframeRule : public CSSRule {
public:
    CSSKeyframeRule(StyleBase* parent);
    virtual ~CSSKeyframeRule();

    virtual bool isKeyframeRule() { return true; }
    virtual unsigned short type() const { return WEBKIT_KEYFRAME_RULE; }
    virtual String cssText() const { return String(); }

    const Vector<float>& keys() const { return m_keys; }
    void setKeys(const Vector<float>& keys) { m_keys = keys; }

    CSSMutableStyleDeclaration* declaration() const { return m_style.get(); }
    void setDeclaration(PassRefPtr<CSSMutableStyleDeclaration>);

private:
    Vector<float> m_keys;
    RefPtr<CSSMutableStyleDeclaration> m_style;
};

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)

#endif // CSSKeyframeRule_h
