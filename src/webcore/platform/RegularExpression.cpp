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
#include "PlatformString.h"
#include "Shared.h"
#if HAVE(SYS_TYPES_H)
#include <sys/types.h>
#endif

#if ENABLE(QJS)
#ifndef BOOL
#define BOOL int
#define FALSE 0
#define TRUE 1
#endif
#include <vector>
#include <quickjs.h>
#include "global.h"
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
    Private(String pattern, bool caseSensitive, bool glob);
    ~Private();

    void compile(bool caseSensitive, bool glob);

    String pattern;
    String lastMatchString;
#if ENABLE(QJS)
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

RegularExpression::Private::Private(String p, bool caseSensitive, bool glob) : pattern(p), lastMatchPos(-1), lastMatchLength(-1)
{
    compile(caseSensitive, glob);
}

static String RegExpFromGlob(String glob)
{
    String result = glob;

    // escape regexp metacharacters which are NOT glob metacharacters
    result.replace("\\", "\\\\");
    result.replace(".", "\\.");
    result.replace("+", "\\+");
    result.replace("$", "\\$");
    // FIXME: incorrect for ^ inside bracket group
    result.replace("^", "\\^");

    // translate glob metacharacters into regexp metacharacters
    result.replace("*", ".*");
    result.replace("?", ".");
   
    // Require the glob to match the whole string
    result = "^" + result + "$";

    return result;
}

void RegularExpression::Private::compile(bool caseSensitive, bool glob)
{
    String p;

    if (glob) {
        p = RegExpFromGlob(pattern);
    } else {
        p = pattern;
    }
    // Note we don't honor the Qt syntax for various character classes.  If we convert
    // to a different underlying engine, we may need to change client code that relies
    // on the regex syntax (see FrameMac.mm for a couple examples).
    
    const char *errorMessage;
#if ENABLE(QJS)
    char errorMsg[64];
    int relen;
    int flags = LRE_FLAG_UTF16;
    if(!caseSensitive) {
        flags |= LRE_FLAG_IGNORECASE;
    }
    errorMessage = errorMsg;
    CString utf8Pattern = p.utf8();
    JSContext* ctx = GLOBAL()->utilContext;
    if (!ctx) {
        mescal::_global_initialize();
        ctx = GLOBAL()->utilContext;
    }
    uint8_t * regex = lre_compile(&relen, errorMsg, sizeof(errorMsg), utf8Pattern.data(), utf8Pattern.length(), flags, ctx);
    if (regex) {
        regexBuf.resize(relen + 1);
        memcpy(regexBuf.data(), regex, relen);
        regexBuf[relen] = '\0';
        js_free(ctx, regex);
    }
#endif

    if (regex == NULL) {
        LOG_ERROR("RegularExpression: pcre_compile failed with '%s'", errorMessage);
    }
}

RegularExpression::Private::~Private()
{
}


RegularExpression::RegularExpression() : d(new RegularExpression::Private())
{
}

RegularExpression::RegularExpression(const String &pattern, bool caseSensitive, bool glob) : d(new RegularExpression::Private(pattern, caseSensitive, glob))
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

String RegularExpression::pattern() const
{
    return d->pattern;
}

int RegularExpression::match(const String &str, int startFrom, int *matchLength) const
{
    d->lastMatchString = str;
#if ENABLE(QJS)
    if (d->regexBuf.isEmpty()) {
        d->lastMatchPos = -1;
        d->lastMatchLength = -1;
        return -1;
    }
    WTF::Vector<uint8_t*> capture;
    int capture_count = lre_get_capture_count(d->regexBuf.data());
    capture.resize(capture_count * 2);
    const uint8_t * str_buf = reinterpret_cast<const uint8_t*>(d->lastMatchString.characters());
    int rc = lre_exec(capture.data(), d->regexBuf.data(), str_buf, startFrom, d->lastMatchString.length(), 1, GLOBAL()->utilContext);
    if (rc != 1) {
        d->lastMatchCount = -1;
        d->lastMatchPos = -1;
        d->lastMatchLength = -1;
        d->lastMatchString = String();
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

int RegularExpression::search(const String &str, int startFrom) const
{
    if (startFrom < 0) {
        startFrom = str.length() - startFrom;
    }
    return match(str, startFrom, NULL);
}

int RegularExpression::searchRev(const String &str) const
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

String RegularExpression::cap(int n) const
{
#if ENABLE(QJS)
    if (d->regexBuf.isEmpty())
        return String();
    WTF::Vector<uint8_t*> capture;
    int capture_count = lre_get_capture_count(d->regexBuf.data());
    capture.resize(capture_count * 2);
    const uint8_t * str_buf = reinterpret_cast<const uint8_t*>(d->lastMatchString.characters());
    int rc = lre_exec(capture.data(), d->regexBuf.data(), str_buf, 0, d->lastMatchString.length(), 1, GLOBAL()->utilContext);
    if (rc != 1)
        return String();

    // n=0 is first capture group (capture[2]/[3]), matching pcre semantics
    // but if no explicit group, n=0 is the whole match (capture[0]/[1])
    int idx = (n + 1) * 2;
    if (idx + 1 >= capture_count * 2)
        idx = 0; // fallback to whole match
    if (!capture[idx] || !capture[idx + 1])
        return String();
    int startPos = (capture[idx] - str_buf) >> 1;
    int len = (capture[idx + 1] - capture[idx]) >> 1;
    return String(d->lastMatchString.characters() + startPos, len);
#endif
}

}
