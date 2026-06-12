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
#include "StorageArea.h"
#include "SqliteStorageArea.h"
#include "Storage.h"
#include "ExceptionCode.h"
#include <wtf/Vector.h>
#include <stdio.h>

using namespace WebCore;

TEST(StorageAreaTest, MemoryBasicOps)
{
    RefPtr<MemoryStorageArea> area = MemoryStorageArea::create();
    EXPECT_EQ(area->length(), 0u);
    EXPECT_TRUE(area->setItem("a", "1"));
    EXPECT_TRUE(area->setItem("b", "2"));
    EXPECT_EQ(area->length(), 2u);
    EXPECT_EQ(area->getItem("a"), "1");
    EXPECT_EQ(area->getItem("b"), "2");
    EXPECT_TRUE(area->contains("a"));
    EXPECT_FALSE(area->contains("z"));
    // null for missing key
    EXPECT_TRUE(area->getItem("z").isNull());
}

TEST(StorageAreaTest, MemoryInsertionOrder)
{
    RefPtr<MemoryStorageArea> area = MemoryStorageArea::create();
    area->setItem("first", "1");
    area->setItem("second", "2");
    area->setItem("third", "3");
    EXPECT_EQ(area->key(0), "first");
    EXPECT_EQ(area->key(1), "second");
    EXPECT_EQ(area->key(2), "third");
    EXPECT_TRUE(area->key(3).isNull()); // out of range
}

TEST(StorageAreaTest, MemoryOverwriteKeepsOrder)
{
    RefPtr<MemoryStorageArea> area = MemoryStorageArea::create();
    area->setItem("a", "1");
    area->setItem("b", "2");
    area->setItem("a", "updated"); // overwrite, not a new entry
    EXPECT_EQ(area->length(), 2u);
    EXPECT_EQ(area->getItem("a"), "updated");
    EXPECT_EQ(area->key(0), "a");
}

TEST(StorageAreaTest, MemoryRemoveAndClear)
{
    RefPtr<MemoryStorageArea> area = MemoryStorageArea::create();
    area->setItem("a", "1");
    area->setItem("b", "2");
    area->removeItem("a");
    EXPECT_EQ(area->length(), 1u);
    EXPECT_TRUE(area->getItem("a").isNull());
    EXPECT_EQ(area->key(0), "b");
    area->clear();
    EXPECT_EQ(area->length(), 0u);
}

TEST(StorageAreaTest, MemoryItemLengthQuota)
{
    RefPtr<MemoryStorageArea> area = MemoryStorageArea::create();
    // A value over the per-item limit is rejected.
    Vector<UChar> buf(kMaxStorageItemLength + 1);
    for (size_t i = 0; i < buf.size(); i++) {
        buf[i] = 'x';
    }
    String huge(buf.data(), buf.size());
    EXPECT_FALSE(area->setItem("big", huge));
    EXPECT_EQ(area->length(), 0u);
}

TEST(StorageTest, QuotaExceededThrows)
{
    RefPtr<Storage> storage = Storage::create(MemoryStorageArea::create());
    ExceptionCode ec = 0;
    storage->setItem("k", "v", ec);
    EXPECT_EQ(ec, 0);
    Vector<UChar> buf(kMaxStorageItemLength + 1);
    for (size_t i = 0; i < buf.size(); i++) {
        buf[i] = 'x';
    }
    String huge(buf.data(), buf.size());
    storage->setItem("big", huge, ec);
    EXPECT_EQ(ec, QUOTA_EXCEEDED_ERR);
}

TEST(SqliteStorageAreaTest, PersistsAcrossInstances)
{
    const char* dbPath = "/tmp/agave_storage_test.db";
    remove(dbPath);
    {
        RefPtr<SqliteStorageArea> area = SqliteStorageArea::create("example.com", dbPath);
        area->setItem("token", "abc123");
        area->setItem("user", "alice");
        EXPECT_EQ(area->length(), 2u);
    }
    // New instance over the same DB + origin sees the persisted data.
    {
        RefPtr<SqliteStorageArea> area = SqliteStorageArea::create("example.com", dbPath);
        EXPECT_EQ(area->length(), 2u);
        EXPECT_EQ(area->getItem("token"), "abc123");
        EXPECT_EQ(area->getItem("user"), "alice");
    }
    remove(dbPath);
}

TEST(SqliteStorageAreaTest, OriginIsolation)
{
    const char* dbPath = "/tmp/agave_storage_origin_test.db";
    remove(dbPath);
    {
        RefPtr<SqliteStorageArea> a = SqliteStorageArea::create("site-a.com", dbPath);
        a->setItem("k", "from-a");
    }
    {
        RefPtr<SqliteStorageArea> b = SqliteStorageArea::create("site-b.com", dbPath);
        // site-b does not see site-a's data.
        EXPECT_EQ(b->length(), 0u);
        EXPECT_TRUE(b->getItem("k").isNull());
    }
    remove(dbPath);
}

// SEC-002 regression: non-ASCII (multi-byte UTF-8) values must round-trip
// through the SQLite backend unchanged. Earlier the QJS string conversion (and
// any UTF-8/Latin-1 confusion) corrupted such values.
TEST(SqliteStorageAreaTest, UnicodeRoundTrip)
{
    const char* dbPath = "/tmp/agave_storage_unicode_test.db";
    remove(dbPath);
    // "ni hao" in CJK + an emoji-like multi-byte sequence, as UTF-16.
    const UChar chars[] = { 0x4F60, 0x597D, 0x4E16, 0x754C, 0x00E9, 0x00F1 };
    String value(chars, 6);
    {
        RefPtr<SqliteStorageArea> a = SqliteStorageArea::create("u.com", dbPath);
        EXPECT_TRUE(a->setItem("k", value));
        EXPECT_EQ(a->getItem("k"), value);
    }
    // Persisted and reloaded identically.
    {
        RefPtr<SqliteStorageArea> a = SqliteStorageArea::create("u.com", dbPath);
        EXPECT_EQ(a->getItem("k"), value);
        EXPECT_EQ(a->getItem("k").length(), 6u);
    }
    remove(dbPath);
}
