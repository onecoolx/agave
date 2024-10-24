/*
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef CSSFontSelector_h
#define CSSFontSelector_h

#include "FontSelector.h"

#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class AtomicString;
class CSSFontFace;
class CSSFontFaceRule;
class Document;
class DocLoader;
class FontDescription;
class String;

class CSSFontSelector : public FontSelector {
public:
    CSSFontSelector(Document* doc);
    virtual ~CSSFontSelector();

    virtual FontData* getFontData(const FontDescription& fontDescription, const AtomicString& familyName);
    
    void addFontFaceRule(const CSSFontFaceRule*);

    void fontLoaded(CSSFontFace*);

    bool isEmpty() const;

    DocLoader* docLoader() const;

protected:
    Document* m_document; // No need to ref, since we will always get destroyed before the document does.
    HashMap<String, RefPtr<CSSFontFace> > m_fonts;
};

} // namespace WebCore

#endif // CSSFontSelector_h
