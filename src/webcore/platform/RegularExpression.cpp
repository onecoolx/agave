/*
 * Copyright (C) 2004 Apple Computer, Inc.  All rights reserved.
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
#include "RegularExpression.h"

#include "Logging.h"
#include "Shared.h"
#if HAVE(SYS_TYPES_H)
#include <sys/types.h>
#endif
#if ENABLE(KJS)
#include <pcre/pcre.h>
#endif

#if ENABLE(QJS)
#ifndef BOOL
#define BOOL int
#define FALSE 0
#define TRUE 1
#endif
#include <vector>
extern "C" {
#include <libregexp.h>
}
#endif

namespace WebCore {

const size_t maxSubstrings = 10;
const size_t maxOffsets = 3 * maxSubstrings;

class RegularExpression::Private : public Shared<RegularExpression::Private>
{
public:
    Private();
    Private(DeprecatedString pattern, bool caseSensitive, bool glob);
    ~Private();

    void compile(bool caseSensitive, bool glob);

    DeprecatedString pattern;
    DeprecatedString lastMatchString;
#if ENABLE(KJS)
    pcre *regex;
    int lastMatchOffsets[maxOffsets];
#elif ENABLE(QJS)
    WTF::Vector<uint8_t> regexBuf; // RegExp bytecode buffer.
#endif
    int lastMatchCount;
    int lastMatchPos;
    int lastMatchLength;
};

RegularExpression::Private::Private() : pattern("")
{
    compile(true, false);
}

RegularExpression::Private::Private(DeprecatedString p, bool caseSensitive, bool glob) : pattern(p), lastMatchPos(-1), lastMatchLength(-1)
{
    compile(caseSensitive, glob);
}

static DeprecatedString RegExpFromGlob(DeprecatedString glob)
{
    DeprecatedString result = glob;

    // escape regexp metacharacters which are NOT glob metacharacters

    result.replace(RegularExpression("\\\\"), "\\\\");
    result.replace(RegularExpression("\\."), "\\.");
    result.replace(RegularExpression("\\+"), "\\+");
    result.replace(RegularExpression("\\$"), "\\$");
    // FIXME: incorrect for ^ inside bracket group
    result.replace(RegularExpression("\\^"), "\\^");

    // translate glob metacharacters into regexp metacharacters
    result.replace(RegularExpression("\\*"), ".*");
    result.replace(RegularExpression("\\?"), ".");
   
    // Require the glob to match the whole string
    result = "^" + result + "$";

    return result;
}

void RegularExpression::Private::compile(bool caseSensitive, bool glob)
{
    DeprecatedString p;

    if (glob) {
        p = RegExpFromGlob(pattern);
    } else {
        p = pattern;
    }
    // Note we don't honor the Qt syntax for various character classes.  If we convert
    // to a different underlying engine, we may need to change client code that relies
    // on the regex syntax (see FrameMac.mm for a couple examples).
    
    const char *errorMessage;
#if ENABLE(KJS)
    int errorOffset;
    regex = pcre_compile(reinterpret_cast<const uint16_t *>(p.unicode()), p.length(), caseSensitive ? 0 : PCRE_CASELESS, &errorMessage, &errorOffset, NULL);
#endif

#if ENABLE(QJS)
    char errorMsg[64];
    int relen;
    int flags = LRE_FLAG_UTF16;
    if(!caseSensitive) {
        flags |= LRE_FLAG_IGNORECASE;
    }
    errorMessage = errorMsg;
    uint8_t * regex = lre_compile(&relen, errorMsg, sizeof(errorMsg), reinterpret_cast<const char *>(p.unicode()), p.length() * 2, flags, 0);
    if (regex) {
        regexBuf.resize(relen + 1);
        memcpy(regexBuf.data(), regex, relen);
        regexBuf[relen] = '\0';
        free(regex);
    }
#endif

    if (regex == NULL) {
        LOG_ERROR("RegularExpression: pcre_compile failed with '%s'", errorMessage);
    }
}

RegularExpression::Private::~Private()
{
#if ENABLE(KJS)
    pcre_free(regex);
#endif
}


RegularExpression::RegularExpression() : d(new RegularExpression::Private())
{
}

RegularExpression::RegularExpression(const DeprecatedString &pattern, bool caseSensitive, bool glob) : d(new RegularExpression::Private(pattern, caseSensitive, glob))
{
}

RegularExpression::RegularExpression(const char *cpattern) : d(new RegularExpression::Private(cpattern, true, false))
{
}


RegularExpression::RegularExpression(const RegularExpression &re) : d (re.d)
{
}

RegularExpression::~RegularExpression()
{
}

RegularExpression &RegularExpression::operator=(const RegularExpression &re)
{
    RegularExpression tmp(re);
    RefPtr<RegularExpression::Private> tmpD = tmp.d;
    
    tmp.d = d;
    d = tmpD;

    return *this;
}

DeprecatedString RegularExpression::pattern() const
{
    return d->pattern;
}

int RegularExpression::match(const DeprecatedString &str, int startFrom, int *matchLength) const
{
    d->lastMatchString = str;
#if ENABLE(KJS)
    // First 2 offsets are start and end offsets; 3rd entry is used internally by pcre
    d->lastMatchCount = pcre_exec(d->regex, NULL, reinterpret_cast<const uint16_t *>(d->lastMatchString.unicode()), d->lastMatchString.length(), startFrom, startFrom == 0 ? 0 : PCRE_NOTBOL, d->lastMatchOffsets, maxOffsets);
    if (d->lastMatchCount < 0) {
        if (d->lastMatchCount != PCRE_ERROR_NOMATCH)
            LOG_ERROR("RegularExpression: pcre_exec() failed with result %d", d->lastMatchCount);
        d->lastMatchPos = -1;
        d->lastMatchLength = -1;
        d->lastMatchString = DeprecatedString();
        return -1;
    }
    // 1 means 1 match; 0 means more than one match. First match is recorded in offsets.
    //ASSERT(d->lastMatchCount < 2);
    d->lastMatchPos = d->lastMatchOffsets[0];
    d->lastMatchLength = d->lastMatchOffsets[1] - d->lastMatchOffsets[0];
    if (matchLength != NULL) {
        *matchLength = d->lastMatchLength;
    }
    return d->lastMatchPos;
#endif

#if ENABLE(QJS)
    WTF::Vector<uint8_t*> capture;
    int capture_count = lre_get_capture_count(d->regexBuf.data());
    capture.resize(capture_count * 2);
    const uint8_t * str_buf = reinterpret_cast<const uint8_t*>(d->lastMatchString.unicode());
    int rc = lre_exec(capture.data(), d->regexBuf.data(), str_buf, startFrom, d->lastMatchString.length(), 1, 0);
    if (rc != 1) {
        d->lastMatchCount = -1;
        d->lastMatchPos = -1;
        d->lastMatchLength = -1;
        d->lastMatchString = DeprecatedString();
        return -1;
    }

    d->lastMatchCount = 1;
    d->lastMatchPos = (capture[0] - str_buf) >> 1;
    d->lastMatchLength = (capture[1] - capture[0]) >> 1;
    if (matchLength != NULL) {
        *matchLength = d->lastMatchLength;
    }
    return d->lastMatchPos;
#endif
}

int RegularExpression::search(const DeprecatedString &str, int startFrom) const
{
    if (startFrom < 0) {
        startFrom = str.length() - startFrom;
    }
    return match(str, startFrom, NULL);
}

int RegularExpression::searchRev(const DeprecatedString &str) const
{
    // FIXME: Total hack for now.  Search forward, return the last, greedy match
    int start = 0;
    int pos;
    int lastPos = -1;
    int lastMatchLength = -1;
    do {
        int matchLength;
        pos = match(str, start, &matchLength);
        if (pos >= 0) {
            if ((pos+matchLength) > (lastPos+lastMatchLength)) {
                // replace last match if this one is later and not a subset of the last match
                lastPos = pos;
                lastMatchLength = matchLength;
            }
            start = pos + 1;
        }
    } while (pos != -1);
    d->lastMatchPos = lastPos;
    d->lastMatchLength = lastMatchLength;
    return lastPos;
}

int RegularExpression::pos(int n)
{
    ASSERT(n == 0);
    return d->lastMatchPos;
}

int RegularExpression::matchedLength() const
{
    return d->lastMatchLength;
}

DeprecatedString RegularExpression::cap(int n) const
{
#if ENABLE(KJS)
    const pcre_char *substring = NULL;
    int substringLength = pcre_get_substring(reinterpret_cast<const UChar16 *>(d->lastMatchString.unicode()), d->lastMatchOffsets, d->lastMatchCount, n, &substring);
    if (substringLength > 0) {
       DeprecatedString capture(substring, substringLength);
       pcre_free_substring(substring);
       return capture;
    }
    return DeprecatedString();
#endif

#if ENABLE(QJS)
    WTF::Vector<uint8_t*> capture;
    int capture_count = lre_get_capture_count(d->regexBuf.data());
    capture.resize(capture_count * 2);
    const uint8_t * str_buf = reinterpret_cast<const uint8_t*>(d->lastMatchString.unicode());
    int sidx = 0;
    while (sidx < n) {
        int rc = lre_exec(capture.data(), d->regexBuf.data(), str_buf, 0, d->lastMatchString.length(), 1, 0);
        if (rc != 1) {
            return DeprecatedString();
        }
        sidx++;
    }

    int startPos = (capture[0] - str_buf) >> 1;
    int len = (capture[1] - capture[0]) >> 1;
    DeprecatedString subString(str_buf + startPos, len);
    return subString;
#endif
}

}
