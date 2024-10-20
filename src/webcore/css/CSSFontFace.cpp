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

#include "config.h"
#include "CSSFontFace.h"

#include "CSSFontFaceSource.h"
#include "CSSFontSelector.h"
#include "FontData.h"
#include "FontDescription.h"

namespace WebCore {

CSSFontFace::CSSFontFace(CSSFontSelector* selector)
: m_fontSelector(selector)
{
}

CSSFontFace::~CSSFontFace()
{
    deleteAllValues(m_sources);
}

bool CSSFontFace::isLoaded() const
{
    unsigned size = m_sources.size();
    for (unsigned i = 0; i < size; i++) {
        if (!m_sources[i]->isLoaded())
            return false;
    }
    return true;
}

bool CSSFontFace::isValid() const
{
    unsigned size = m_sources.size();
    if (!size)
        return false;
    for (unsigned i = 0; i < size; i++) {
        if (m_sources[i]->isValid())
            return true;
    }
    return false;
}
    
void CSSFontFace::addSource(CSSFontFaceSource* source)
{
    m_sources.append(source);
    source->setFontFace(this);
}

void CSSFontFace::fontLoaded(CSSFontFaceSource*)
{
    if (isLoaded())
        return m_fontSelector->fontLoaded(this);
}

FontData* CSSFontFace::getFontData(const FontDescription& fontDescription, bool syntheticBold, bool syntheticItalic)
{
    if (!isValid())
        return 0;
        
    // If we hit a local font, we know it is valid, and can just return it.
    FontData* result = 0;
    unsigned size = m_sources.size();
    for (unsigned i = 0; i < size && !result; i++)
        result = m_sources[i]->getFontData(fontDescription, syntheticBold, syntheticItalic, m_fontSelector);
    return result;
}

}

