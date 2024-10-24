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
#include "CSSFontFaceSource.h"

#include "CachedFont.h"
#include "CSSFontFace.h"
#include "CSSFontSelector.h"
#include "DocLoader.h"
#include "FontCache.h"
#include "FontData.h"
#include "FontDescription.h"
#include "GlyphPageTreeNode.h"

namespace WebCore {

CSSFontFaceSource::CSSFontFaceSource(const String& str, CachedFont* font)
: m_string(str)
, m_font(font)
, m_face(0)
{
    if (m_font)
        m_font->ref(this);
}

CSSFontFaceSource::~CSSFontFaceSource()
{
    if (m_font)
        m_font->deref(this);
    pruneTable();
}

void CSSFontFaceSource::pruneTable()
{
    // Make sure the glyph page tree prunes out all uses of these custom fonts.
    if (m_fontDataTable.isEmpty())
        return;
    HashMap<int, FontData*>::iterator end = m_fontDataTable.end();
    for (HashMap<int, FontData*>::iterator it = m_fontDataTable.begin(); it != end; ++it)
        GlyphPageTreeNode::pruneTreeCustomFontData(it->second);
    deleteAllValues(m_fontDataTable);
    m_fontDataTable.clear();
}

bool CSSFontFaceSource::isLoaded() const
{
    if (m_font)
        return m_font->isLoaded();
    return true;
}

bool CSSFontFaceSource::isValid() const
{
    if (m_font)
        return !m_font->errorOccurred();
    return true;
}

void CSSFontFaceSource::fontLoaded(CachedFont*)
{
    pruneTable();
    if (m_face)
        m_face->fontLoaded(this);
}

FontData* CSSFontFaceSource::getFontData(const FontDescription& fontDescription, bool syntheticBold, bool syntheticItalic, CSSFontSelector* fontSelector)
{
    // If the font hasn't loaded or an error occurred, then we've got nothing.
    if (!isValid())
        return 0;

    if (!m_font)
        // We're local.  Just return a FontData from the normal cache.
        return FontCache::getCachedFontData(FontCache::getCachedFontPlatformData(fontDescription, m_string));
    
    // See if we have a mapping in our FontData cache.
    FontData* cachedData = m_fontDataTable.get(fontDescription.computedPixelSize());
    if (cachedData)
        return cachedData;
    
    // If we are still loading, then we let the system pick a font.
    if (isLoaded()) {
        // Create new FontPlatformData from our CGFontRef, point size and ATSFontRef.
        if (!m_font->ensureCustomFontData())
            return 0;
        cachedData = new FontData(m_font->platformDataFromCustomData(fontDescription.computedPixelSize(), syntheticBold, syntheticItalic), true, false);
    } else {
        // Kick off the load now.
        m_font->beginLoadIfNeeded(fontSelector->docLoader());

        FontPlatformData* tempData = FontCache::getCachedFontPlatformData(fontDescription, m_string);
        if (!tempData)
            tempData = FontCache::getLastResortFallbackFont(fontDescription);
        cachedData = new FontData(*tempData, true, true);
    }

    m_fontDataTable.set(fontDescription.computedPixelSize(), cachedData);

    return cachedData;
}

}

