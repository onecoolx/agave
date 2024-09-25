/* autofill.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"

#include <sstream>
#include <string>

#include "autofill.h"
#include "sqlite3.h"

#if FILENAME_UNICODE
    #define DB_OPEN sqlite3_open16
#else
    #define DB_OPEN sqlite3_open
#endif

#define DB_NAME (DATA_PATH("authinfo.dat"))
#define INDEX_TABLE_NAME "UrlIndex"
#define VALUE_TABLE_NAME "UpInfo"

using namespace std;

class AutoFillImpl
{
public:
    AutoFillImpl()
        : pdb(0), init(false), lastsid(0)
    {
    }

    sqlite3* pdb;
    bool init;
    unsigned int lastsid;
};

AutoFill::AutoFill()
    : m_impl(new AutoFillImpl)
{
    if (DB_OPEN(DB_NAME, &m_impl->pdb) == SQLITE_OK) {
        sqlite3_key(m_impl->pdb, "Qw1236#a5t15", 12);

        char** result = 0;
        char* err = 0;
        int nrow = 0, ncol = 0;
        std::stringstream sql;
        sql << "select * from " << INDEX_TABLE_NAME << " limit 1";
        if (sqlite3_get_table(m_impl->pdb, sql.str().c_str(),
                              &result, &nrow, &ncol, &err) != SQLITE_OK) {
            bool ib = false, vb = false;
            std::stringstream csql;
            csql << "create table " << INDEX_TABLE_NAME
                 << "(id integer primary key not null,"
                 << " url text not null,"
                 << " formid text not null,"
                 << " action text not null,"
                 << " sid integer not null,"
                 << " dnts integer not null)";

            if (sqlite3_exec(m_impl->pdb, csql.str().c_str(), 0, 0, &err) == SQLITE_OK) {
                ib = true;
            }

            std::stringstream vsql;
            vsql << "create table " << VALUE_TABLE_NAME
                 << "(id integer primary key not null,"
                 << " sid integer not null,"
                 << " name text not null,"
                 << " value text not null)";

            if (sqlite3_exec(m_impl->pdb, vsql.str().c_str(), 0, 0, &err) == SQLITE_OK) {
                vb = true;
            }

            if (ib && vb) {
                m_impl->init = true;
            }
            m_impl->lastsid += 1;
        } else {
            m_impl->init = true;
            // get lastsid for value table.

            unsigned int num = 0;
            stringstream sql;
            sql << "select max(sid) as num from " << INDEX_TABLE_NAME;

            sqlite3_stmt* stmt = 0;
            if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    num = sqlite3_column_int(stmt, 0);
                }
            }
            sqlite3_finalize(stmt);

            m_impl->lastsid = num + 1;
        }
        sqlite3_free_table(result);
    }
}

AutoFill::~AutoFill()
{
    if (m_impl->pdb) {
        sqlite3_close(m_impl->pdb);
        m_impl->pdb = 0;
        m_impl->init = false;
    }
    delete m_impl;
}

bool AutoFill::updateFormData(const FormData& data, int presid)
{
    char** result = 0;
    char* err = 0;
    int nrow = 0, ncol = 0;

    int sid = presid;
    if (sid < 0) {
        std::stringstream sql;
        sql << "select * from " << INDEX_TABLE_NAME << " where id=" << data.id << " limit 1";

        sqlite3_stmt* stmt = 0;
        if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sid = sqlite3_column_int(stmt, 4);
            }
        }
        sqlite3_finalize(stmt);
    }

    if (sid == -1) {
        return false;
    }

    // update form data

    std::stringstream isql;
    isql << "update "INDEX_TABLE_NAME << " "
         << "set url=\'" << data.url.c_str() << "\'"
         << ", formid=\'" << data.formid.c_str() << "\'"
         << ", action=\'" << data.action.c_str() << "\'"
         << ", dnts=" << data.notsave << ""
         << " where id=" << data.id;

    if (sqlite3_exec(m_impl->pdb, isql.str().c_str(), 0, 0, 0) != SQLITE_OK) {
        return false;
    }

    std::stringstream dsql;
    dsql << "delete from " << VALUE_TABLE_NAME << " where sid=" << sid;

    if (sqlite3_exec(m_impl->pdb, dsql.str().c_str(), 0, 0, 0) != SQLITE_OK) {
        return false;
    }

    for (std::map<std::string, std::string>::const_iterator itr = data.values.begin();
         itr != data.values.end(); ++itr) {
        // insert for fields
        stringstream vsql;
        vsql << "insert into " << VALUE_TABLE_NAME << " (sid, name, value) ";
        vsql << "values (" << sid << ", \'" << itr->first.c_str() << "\', \'"
             << itr->second.c_str() << "\')";

        if (sqlite3_exec(m_impl->pdb, vsql.str().c_str(), 0, 0, 0) != SQLITE_OK) {
            return false;
        }
    }

    return true;
}

bool AutoFill::addFormData(const FormData& data)
{
    int sid = m_impl->lastsid;
    m_impl->lastsid++;

    stringstream isql;
    isql << "insert into " << INDEX_TABLE_NAME << " (url, formid, action, sid, dnts) ";
    isql << "values (\'" << data.url.c_str() << "\', \'" << data.formid.c_str() << "\', \'"
         << data.action.c_str() << "\', " << sid << ", " << data.notsave << ")";

    if (sqlite3_exec(m_impl->pdb, isql.str().c_str(), 0, 0, 0) != SQLITE_OK) {
        return false;
    }

    for (std::map<std::string, std::string>::const_iterator itr = data.values.begin();
         itr != data.values.end(); ++itr) {
        // insert for fields
        stringstream vsql;
        vsql << "insert into " << VALUE_TABLE_NAME << " (sid, name, value) ";
        vsql << "values (" << sid << ", \'" << itr->first.c_str() << "\', \'"
             << itr->second.c_str() << "\')";

        if (sqlite3_exec(m_impl->pdb, vsql.str().c_str(), 0, 0, 0) != SQLITE_OK) {
            return false;
        }
    }

    return true;
}

bool AutoFill::findFormData(const std::string& baseurl, FormData& data, int* psid)
{
    stringstream sql;
    sql << "select * from " << INDEX_TABLE_NAME << " where url=\'" << baseurl.c_str() << "\' limit 1";

    sqlite3_stmt* stmt = 0;
    bool ret = false;
    int sid = -1;
    if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {

            data.id = sqlite3_column_int(stmt, 0);
            data.url = (char*)sqlite3_column_text(stmt, 1);
            data.formid = (char*)sqlite3_column_text(stmt, 2);
            data.action = (char*)sqlite3_column_text(stmt, 3);
            sid = sqlite3_column_int(stmt, 4);
            data.notsave = sqlite3_column_int(stmt, 5);

            ret = true;
        }

        if (psid) {
            *psid = sid;
        }
    }
    sqlite3_finalize(stmt);

    if (!ret || sid < 0) {
        return false;
    }

    if (data.notsave) {
        return true;
    }

    stringstream ssql;
    ssql << "select * from " << VALUE_TABLE_NAME << " where sid=" << sid;

    ret = false;
    if (sqlite3_prepare(m_impl->pdb, ssql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {

            std::string name = (char*)sqlite3_column_text(stmt, 2);
            std::string value = (char*)sqlite3_column_text(stmt, 3);
            data.values[name] = value;
            ret = true;
        }
    }
    sqlite3_finalize(stmt);

    if (!ret) {
        return false;
    }

    return true;
}

void AutoFill::clearAllFormData(void)
{
    std::stringstream dsql;
    dsql << "delete from " << VALUE_TABLE_NAME;

    std::stringstream dsql2;
    dsql2 << "delete from " << INDEX_TABLE_NAME;

    sqlite3_exec(m_impl->pdb, dsql.str().c_str(), 0, 0, 0);
    sqlite3_exec(m_impl->pdb, dsql2.str().c_str(), 0, 0, 0);
}
