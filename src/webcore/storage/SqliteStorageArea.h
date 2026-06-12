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

#ifndef SqliteStorageArea_h
#define SqliteStorageArea_h

#include "StorageArea.h"

struct sqlite3;

namespace WebCore {

// Persistent localStorage backend. Entries are keyed by (origin, key) in a
// single SQLite database. The origin's rows are loaded into an in-memory
// ordered map on construction for fast reads; mutations write through to both
// the cache and the database. Falls back to memory-only if the DB can't open.
class SqliteStorageArea : public StorageArea {
public:
    // origin: the page origin key (e.g. host); dbPath: the storage DB file.
    static PassRefPtr<SqliteStorageArea> create(const String& origin, const String& dbPath)
    {
        return PassRefPtr<SqliteStorageArea>(new SqliteStorageArea(origin, dbPath));
    }
    virtual ~SqliteStorageArea();

    virtual unsigned length() const { return m_order.size(); }
    virtual String key(unsigned index) const;
    virtual String getItem(const String& key) const;
    virtual bool setItem(const String& key, const String& value);
    virtual void removeItem(const String& key);
    virtual void clear();
    virtual bool contains(const String& key) const { return m_map.contains(key); }

private:
    SqliteStorageArea(const String& origin, const String& dbPath);
    bool openDatabase(const String& dbPath);
    void loadFromDatabase();
    bool execKeyValue(const char* sql, const String& key, const String* value);

    String m_origin;
    sqlite3* m_db;
    HashMap<String, String> m_map;
    Vector<String> m_order;
};

} // namespace WebCore

#endif // SqliteStorageArea_h
