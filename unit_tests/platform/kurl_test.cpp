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

#include "test.h"

// ============================================================
// Basic URL parsing
// ============================================================

TEST(KURLTest, ParseHTTP)
{
    KURL url("http://www.example.com/path?query=1#frag");
    EXPECT_TRUE(url.isValid());
    EXPECT_EQ(url.protocol(), String("http"));
    EXPECT_EQ(url.host(), String("www.example.com"));
    EXPECT_EQ(url.path(), String("/path"));
    EXPECT_EQ(url.query(), String("?query=1"));
    EXPECT_EQ(url.ref(), String("frag"));
}

TEST(KURLTest, ParseHTTPS)
{
    KURL url("https://secure.example.com:443/");
    EXPECT_TRUE(url.isValid());
    EXPECT_EQ(url.protocol(), String("https"));
    EXPECT_EQ(url.host(), String("secure.example.com"));
    EXPECT_EQ(url.port(), 443);
}

TEST(KURLTest, ParseFTP)
{
    KURL url("ftp://files.example.com/pub/file.txt");
    EXPECT_TRUE(url.isValid());
    EXPECT_EQ(url.protocol(), String("ftp"));
    EXPECT_EQ(url.host(), String("files.example.com"));
    EXPECT_EQ(url.path(), String("/pub/file.txt"));
}

TEST(KURLTest, ParseFileURL)
{
    KURL url("file:///home/user/doc.html");
    EXPECT_TRUE(url.isValid());
    EXPECT_EQ(url.protocol(), String("file"));
    EXPECT_TRUE(url.isLocalFile());
}

// ============================================================
// URL component extraction
// ============================================================

TEST(KURLTest, ComponentExtraction)
{
    KURL url("http://user:pass@host.com:8080/path/page?q=1&r=2#section");
    EXPECT_EQ(url.protocol(), String("http"));
    EXPECT_EQ(url.user(), String("user"));
    EXPECT_EQ(url.pass(), String("pass"));
    EXPECT_EQ(url.host(), String("host.com"));
    EXPECT_EQ(url.port(), 8080);
    EXPECT_EQ(url.path(), String("/path/page"));
    EXPECT_EQ(url.query(), String("?q=1&r=2"));
    EXPECT_EQ(url.ref(), String("section"));
}

TEST(KURLTest, LastPathComponent)
{
    KURL url("http://example.com/a/b/c.html");
    EXPECT_EQ(url.lastPathComponent(), String("c.html"));
}

TEST(KURLTest, PortDefault)
{
    KURL url("http://example.com/");
    EXPECT_EQ(url.port(), 0); // no explicit port
}

// ============================================================
// URL modification
// ============================================================

TEST(KURLTest, SetProtocol)
{
    KURL url("http://example.com/path");
    url.setProtocol(String("https"));
    EXPECT_EQ(url.protocol(), String("https"));
}

TEST(KURLTest, SetHost)
{
    KURL url("http://old.com/path");
    url.setHost(String("new.com"));
    EXPECT_EQ(url.host(), String("new.com"));
}

TEST(KURLTest, SetPort)
{
    KURL url("http://example.com/path");
    url.setPort(9090);
    EXPECT_EQ(url.port(), 9090);
}

TEST(KURLTest, SetPath)
{
    KURL url("http://example.com/old");
    url.setPath(String("/new/path"));
    EXPECT_EQ(url.path(), String("/new/path"));
}

TEST(KURLTest, SetQuery)
{
    KURL url("http://example.com/path");
    url.setQuery(String("key=value"));
    EXPECT_TRUE(url.query().contains("key=value"));
}

TEST(KURLTest, SetRef)
{
    KURL url("http://example.com/path");
    url.setRef(String("anchor"));
    EXPECT_EQ(url.ref(), String("anchor"));
    EXPECT_TRUE(url.hasRef());
}

// ============================================================
// Relative URL resolution
// ============================================================

TEST(KURLTest, RelativeURL)
{
    KURL base("http://example.com/a/b/c.html");
    KURL relative(base, String("../d.html"));
    EXPECT_TRUE(relative.isValid());
    EXPECT_EQ(relative.path(), String("/a/d.html"));
}

TEST(KURLTest, RelativeURLAbsolute)
{
    KURL base("http://example.com/a/b/");
    KURL absolute(base, String("http://other.com/x"));
    EXPECT_EQ(absolute.host(), String("other.com"));
    EXPECT_EQ(absolute.path(), String("/x"));
}

// ============================================================
// URL encode/decode
// ============================================================

TEST(KURLTest, EncodeString)
{
    String encoded = KURL::encode_string(String("hello world"));
    EXPECT_TRUE(encoded.contains("%20") || encoded.contains("+"));
}

TEST(KURLTest, DecodeString)
{
    String decoded = KURL::decode_string(String("hello%20world"));
    EXPECT_EQ(decoded, String("hello world"));
}

// ============================================================
// Edge cases
// ============================================================

TEST(KURLTest, EmptyURL)
{
    KURL url("");
    EXPECT_TRUE(url.isEmpty());
}

TEST(KURLTest, InvalidURL)
{
    KURL url("not a url");
    EXPECT_TRUE(url.isMalformed());
}

TEST(KURLTest, FragmentOnlyURL)
{
    KURL base("http://example.com/page");
    KURL fragOnly(base, String("#section"));
    EXPECT_TRUE(fragOnly.isValid());
    EXPECT_EQ(fragOnly.ref(), String("section"));
}

TEST(KURLTest, URLWithUserInfo)
{
    KURL url("http://admin:secret@example.com/");
    EXPECT_EQ(url.user(), String("admin"));
    EXPECT_EQ(url.pass(), String("secret"));
}

// ============================================================
// prettyURL
// ============================================================

TEST(KURLTest, PrettyURL)
{
    KURL url("http://example.com/path?q=1#ref");
    String pretty = url.prettyURL();
    EXPECT_FALSE(pretty.isEmpty());
    EXPECT_TRUE(pretty.contains("example.com"));
}

// ============================================================
// Protocol checks
// ============================================================

TEST(KURLTest, IsHttp)
{
    KURL http("http://example.com/");
    KURL https("https://example.com/");
    KURL ftp("ftp://example.com/");
    EXPECT_TRUE(http.isHttp());
    EXPECT_TRUE(https.isHttp()); // isHttp() returns true for both http and https
    EXPECT_FALSE(ftp.isHttp());
    EXPECT_TRUE(ftp.isFtp());
}

// ============================================================
// Property-Based Tests
// ============================================================

// **Feature: remove-deprecated-string, Property 5: KURL URL 解析组件提取一致性**
// **Validates: Requirements 2.7, 9.2**
// For any well-formed URL, parsing and extracting components should be consistent,
// and prettyURL() should produce a valid URL that re-parses to the same components.
TEST(KURLPropertyTest, ParseComponentConsistency)
{
    const char* hosts[] = {
        "example.com", "test.org", "sub.domain.net", "localhost",
        "a.b.c.d.com", "my-site.io"
    };
    const char* paths[] = {
        "/", "/path", "/a/b/c", "/index.html", "/path/to/resource"
    };
    const char* queries[] = {
        "", "q=1", "a=1&b=2", "key=value"
    };
    const char* fragments[] = {
        "", "top", "section1", "ref"
    };

    srand(46);
    for (int iter = 0; iter < 100; ++iter) {
        const char* host = hosts[rand() % 6];
        const char* path = paths[rand() % 5];
        const char* query = queries[rand() % 4];
        const char* frag = fragments[rand() % 4];
        int port = (rand() % 2) ? (1024 + rand() % 64000) : 0;

        // Build URL string
        String urlStr("http://");
        urlStr.append(host);
        if (port > 0) {
            urlStr.append(":");
            urlStr.append(String::number(port));
        }
        urlStr.append(path);
        if (strlen(query) > 0) {
            urlStr.append("?");
            urlStr.append(query);
        }
        if (strlen(frag) > 0) {
            urlStr.append("#");
            urlStr.append(frag);
        }

        KURL url(urlStr);
        EXPECT_TRUE(url.isValid()) << "URL should be valid: " << urlStr.latin1().data();
        EXPECT_EQ(url.protocol(), String("http"));
        EXPECT_EQ(url.host(), String(host));
        if (port > 0) {
            EXPECT_EQ(url.port(), static_cast<unsigned short>(port));
        }
        EXPECT_EQ(url.path(), String(path));

        // prettyURL should produce a re-parseable URL
        String pretty = url.prettyURL();
        KURL reparsed(pretty);
        EXPECT_TRUE(reparsed.isValid());
        EXPECT_EQ(reparsed.protocol(), url.protocol());
        EXPECT_EQ(reparsed.host(), url.host());
        EXPECT_EQ(reparsed.path(), url.path());
    }
}
