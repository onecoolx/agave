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

#ifndef CSSTransitionsValue_h
#define CSSTransitionsValue_h

#include "config.h"

#if ENABLE(CSS_TRANSITIONS)

#include "CSSValue.h"
#include "Animation.h"

namespace WebCore {

// A CSSValue wrapper carrying a fully-parsed CSS transition list. The parser
// produces this for the `transition` shorthand and the individual longhands so
// the style selector can copy it straight into RenderStyle.
class CSSTransitionsValue : public CSSValue {
public:
    CSSTransitionsValue(const TransitionList& list)
        : m_transitions(list)
    {
    }

    virtual bool isTransitionsValue() const { return true; }
    virtual String cssText() const { return String(); }

    const TransitionList& transitions() const { return m_transitions; }

private:
    TransitionList m_transitions;
};

} // namespace WebCore

#endif // ENABLE(CSS_TRANSITIONS)

#endif // CSSTransitionsValue_h
