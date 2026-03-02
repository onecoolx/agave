/*
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
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

#ifndef KURL_h
#define KURL_h

#include "PlatformString.h"
#include <wtf/Platform.h>

namespace WebCore {

    class KURL;
    class TextEncoding;

    bool operator==(const KURL&, const KURL&);
    inline bool operator!=(const KURL &a, const KURL &b) { return !(a == b); }

    bool equalIgnoringRef(const KURL&, const KURL&);

class KURL {
public:
    KURL();
    KURL(const char*);
    KURL(const KURL&, const String&);
    KURL(const KURL&, const String&, const TextEncoding&);
    KURL(const String&);

    bool isEmpty() const { return urlString.isEmpty(); } 
    bool isMalformed() const { return !m_isValid; }
    bool isValid() const { return m_isValid; }
    bool hasPath() const;

	unsigned urlHash() const;
    String url() const { return urlString; }

    String protocol() const;
    String host() const;
    unsigned short int port() const;
    String user() const;
    String pass() const;
    String path() const;
    String lastPathComponent() const;
    String query() const;
    String ref() const;
    String localFile() const;
    String ignoreRefURL() const;
    bool hasRef() const;

    String encodedHtmlRef() const { return ref(); }

    void setProtocol(const String &);
    void setHost(const String &);
    void setPort(unsigned short int);
    void setHostAndPort(const String&);
    void setUser(const String &);
    void setPass(const String &);
    void setPath(const String &);
    void setQuery(const String &);
    void setRef(const String &);

    String prettyURL() const;

    bool isLocalFile() const;
    bool isHttp() const;
    bool isFtp() const;

    static String decode_string(const String&);
    static String decode_string(const String&, const TextEncoding&);
    static String encode_string(const String&);
    
    friend bool operator==(const KURL &, const KURL &);

private:
    bool isHierarchical() const;
    void init(const KURL&, const String&, const TextEncoding&);
    void parse(const char *url, const String *originalString);

    String urlString;
	mutable unsigned m_hash;
    bool m_isValid;
    int schemeEndPos;
    int userStartPos;
    int userEndPos;
    int passwordEndPos;
    int hostEndPos;
    int portEndPos;
    int pathEndPos;
    int queryEndPos;
    int fragmentEndPos;
    
    friend bool equalIgnoringRef(const KURL& a, const KURL& b);
};

bool isSchemeFirstChar(unsigned char c);
bool isSchemeChar(unsigned char c); 

}

#endif // KURL_h
