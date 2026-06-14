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

#ifndef CSSClipPathValue_h
#define CSSClipPathValue_h

#include "config.h"

#if ENABLE(MODERN_CSS3)

#include "CSSValue.h"
#include "RenderStyle.h" // for ClipPathOperation

namespace WebCore {

// A CSSValue wrapper carrying a parsed clip-path basic-shape, so the style
// selector can copy it into RenderStyle's transform data.
class CSSClipPathValue : public CSSValue {
public:
    CSSClipPathValue(const ClipPathOperation& op)
        : m_clipPath(op)
    {
    }

    virtual bool isClipPathValue() const { return true; }
    virtual String cssText() const { return String(); }

    const ClipPathOperation& clipPath() const { return m_clipPath; }

private:
    ClipPathOperation m_clipPath;
};

} // namespace WebCore

#endif // ENABLE(MODERN_CSS3)

#endif // CSSClipPathValue_h
