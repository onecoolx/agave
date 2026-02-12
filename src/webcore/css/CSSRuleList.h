/*
 * This file is part of the DOM implementation for KDE.
 *
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef CSSRuleList_h
#define CSSRuleList_h

#include "Shared.h"
#include <wtf/Vector.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class CSSRule;
class StyleList;

class CSSRuleList : public Shared<CSSRuleList> {
public:
    CSSRuleList();
    CSSRuleList(StyleList*, bool omitCharsetRules = false);
    ~CSSRuleList();

    unsigned length() const { return m_lstCSSRules.size(); }
    CSSRule* item(unsigned index) { return index < m_lstCSSRules.size() ? m_lstCSSRules[index].get() : 0; }

    /* not part of the DOM */
    unsigned insertRule(CSSRule*, unsigned index);
    void deleteRule(unsigned index);
    void append(CSSRule*);

protected:
    Vector<RefPtr<CSSRule> > m_lstCSSRules;
};

} // namespace WebCore

#endif // CSSRuleList_h
