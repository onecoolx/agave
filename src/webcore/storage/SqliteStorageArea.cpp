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
#include "SqliteStorageArea.h"

#include "CString.h"
#include <sqlite3.h>

namespace WebCore {

SqliteStorageArea::SqliteStorageArea(const String& origin, const String& dbPath)
    : m_origin(origin)
    , m_db(0)
{
    if (openDatabase(dbPath))
        loadFromDatabase();
}

SqliteStorageArea::~SqliteStorageArea()
{
    if (m_db)
        sqlite3_close(m_db);
}

bool SqliteStorageArea::openDatabase(const String& dbPath)
{
    CString path = dbPath.utf8();
    if (sqlite3_open(path.data(), &m_db) != SQLITE_OK) {
        if (m_db) {
            sqlite3_close(m_db);
            m_db = 0;
        }
        return false; // memory-only fallback
    }
    // Schema: one row per (origin, key). Parameterized statements below prevent
    // SQL injection from untrusted keys/values.
    const char* schema =
        "CREATE TABLE IF NOT EXISTS ItemTable ("
        "origin TEXT NOT NULL, key TEXT NOT NULL, value TEXT NOT NULL, "
        "seq INTEGER, PRIMARY KEY(origin, key));";
    char* err = 0;
    if (sqlite3_exec(m_db, schema, 0, 0, &err) != SQLITE_OK) {
        if (err)
            sqlite3_free(err);
        sqlite3_close(m_db);
        m_db = 0;
        return false;
    }
    return true;
}

void SqliteStorageArea::loadFromDatabase()
{
    if (!m_db)
        return;
    sqlite3_stmt* stmt = 0;
    const char* sql = "SELECT key, value FROM ItemTable WHERE origin = ? ORDER BY seq;";
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, 0) != SQLITE_OK)
        return;
    CString origin = m_origin.utf8();
    sqlite3_bind_text(stmt, 1, origin.data(), origin.length(), SQLITE_TRANSIENT);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* k = (const char*)sqlite3_column_text(stmt, 0);
        const char* v = (const char*)sqlite3_column_text(stmt, 1);
        String key = String::fromUTF8(k ? k : "");
        String value = String::fromUTF8(v ? v : "");
        if (!m_map.contains(key))
            m_order.append(key);
        m_map.set(key, value);
    }
    sqlite3_finalize(stmt);
}

bool SqliteStorageArea::execKeyValue(const char* sql, const String& key, const String* value)
{
    if (!m_db)
        return true; // memory-only: treat as success
    sqlite3_stmt* stmt = 0;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, 0) != SQLITE_OK)
        return false;
    CString origin = m_origin.utf8();
    CString keyUtf = key.utf8();
    int col = 1;
    sqlite3_bind_text(stmt, col++, origin.data(), origin.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, col++, keyUtf.data(), keyUtf.length(), SQLITE_TRANSIENT);
    CString valUtf;
    if (value) {
        valUtf = value->utf8();
        sqlite3_bind_text(stmt, col++, valUtf.data(), valUtf.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, col++, (int)m_order.size());
    }
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

String SqliteStorageArea::key(unsigned index) const
{
    if (index >= m_order.size())
        return String();
    return m_order[index];
}

String SqliteStorageArea::getItem(const String& key) const
{
    HashMap<String, String>::const_iterator it = m_map.find(key);
    if (it == m_map.end())
        return String();
    return it->second;
}

bool SqliteStorageArea::setItem(const String& key, const String& value)
{
    if (value.length() > kMaxStorageItemLength)
        return false;
    bool isNew = !m_map.contains(key);
    if (isNew && m_order.size() >= kMaxStorageEntries)
        return false; // quota exceeded

    // Write through to the database (INSERT OR REPLACE keeps seq stable for
    // existing keys by reusing the cached order index).
    execKeyValue("INSERT OR REPLACE INTO ItemTable (origin, key, value, seq) VALUES (?, ?, ?, ?);", key, &value);

    if (isNew)
        m_order.append(key);
    m_map.set(key, value);
    return true;
}

void SqliteStorageArea::removeItem(const String& key)
{
    if (!m_map.contains(key))
        return;
    execKeyValue("DELETE FROM ItemTable WHERE origin = ? AND key = ?;", key, 0);
    m_map.remove(key);
    for (size_t i = 0; i < m_order.size(); i++) {
        if (m_order[i] == key) {
            m_order.remove(i);
            break;
        }
    }
}

void SqliteStorageArea::clear()
{
    if (m_db) {
        sqlite3_stmt* stmt = 0;
        if (sqlite3_prepare_v2(m_db, "DELETE FROM ItemTable WHERE origin = ?;", -1, &stmt, 0) == SQLITE_OK) {
            CString origin = m_origin.utf8();
            sqlite3_bind_text(stmt, 1, origin.data(), origin.length(), SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    m_map.clear();
    m_order.clear();
}

} // namespace WebCore
