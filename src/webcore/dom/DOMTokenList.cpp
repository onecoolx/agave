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

#include "config.h"
#include "DOMTokenList.h"

#include "Element.h"
#include "ExceptionCode.h"
#include "HTMLNames.h"

namespace WebCore {

using namespace HTMLNames;

// CSS/HTML ASCII whitespace for class-token splitting.
static inline bool isClassWhitespace(UChar c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f';
}

// Splits the class attribute on ASCII whitespace.
Vector<String> DOMTokenList::tokens() const
{
    Vector<String> result;
    String value = m_element->getAttribute(classAttr);
    unsigned len = value.length();
    unsigned start = 0;
    while (start < len) {
        while (start < len && isClassWhitespace(value[start]))
            start++;
        unsigned end = start;
        while (end < len && !isClassWhitespace(value[end]))
            end++;
        if (end > start)
            result.append(value.substring(start, end - start));
        start = end;
    }
    return result;
}

void DOMTokenList::writeTokens(const Vector<String>& tokens)
{
    String value;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (i)
            value += " ";
        value += tokens[i];
    }
    ExceptionCode ec = 0;
    m_element->setAttribute(classAttr, value.impl(), ec);
}

bool DOMTokenList::isValidToken(const String& token, ExceptionCode& ec)
{
    if (token.isEmpty()) {
        ec = SYNTAX_ERR;
        return false;
    }
    for (unsigned i = 0; i < token.length(); i++) {
        if (isClassWhitespace(token[i])) {
            ec = INVALID_CHARACTER_ERR;
            return false;
        }
    }
    return true;
}

unsigned DOMTokenList::length() const
{
    return tokens().size();
}

String DOMTokenList::item(unsigned index) const
{
    Vector<String> list = tokens();
    return index < list.size() ? list[index] : String();
}

bool DOMTokenList::contains(const String& token) const
{
    Vector<String> list = tokens();
    for (size_t i = 0; i < list.size(); i++)
        if (list[i] == token)
            return true;
    return false;
}

void DOMTokenList::add(const String& token, ExceptionCode& ec)
{
    if (!isValidToken(token, ec))
        return;
    Vector<String> list = tokens();
    for (size_t i = 0; i < list.size(); i++)
        if (list[i] == token)
            return; // already present
    list.append(token);
    writeTokens(list);
}

void DOMTokenList::remove(const String& token, ExceptionCode& ec)
{
    if (!isValidToken(token, ec))
        return;
    Vector<String> list = tokens();
    Vector<String> out;
    for (size_t i = 0; i < list.size(); i++)
        if (list[i] != token)
            out.append(list[i]);
    writeTokens(out);
}

bool DOMTokenList::toggle(const String& token, ExceptionCode& ec)
{
    if (!isValidToken(token, ec))
        return false;
    if (contains(token)) {
        remove(token, ec);
        return false;
    }
    add(token, ec);
    return true;
}

String DOMTokenList::toString() const
{
    return m_element->getAttribute(classAttr);
}

} // namespace WebCore
