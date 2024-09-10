/* history.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"

#include <sstream>
#include <string>

#include "history.h"
#include "sqlite3.h"

#if FILENAME_UNICODE
#define DB_OPEN sqlite3_open16
#else
#define DB_OPEN sqlite3_open
#endif

#define DB_NAME (DATA_PATH("history.dat"))
#define TABLE_NAME "history"

class HistoryImpl
{
public:
	HistoryImpl()
		: pdb(0), init(false)
	{
	}
	
	sqlite3* pdb;
	bool init;
};


History::History()
	: m_impl(new HistoryImpl)
{
	if (DB_OPEN(DB_NAME, &m_impl->pdb) == SQLITE_OK) {
		char **result = 0;
		char *err = 0;
		int nrow = 0, ncol = 0;
		std::stringstream sql;
		sql<<"select * from "<<TABLE_NAME<<" limit 1";
		if (sqlite3_get_table(m_impl->pdb, sql.str().c_str(), 
					&result, &nrow, &ncol, &err) != SQLITE_OK) 
		{
			std::stringstream csql;
			csql<<"create table "<<TABLE_NAME
			   <<"(id integer primary key not null,"
			   <<" title text not null,"
			   <<" url text not null,"
			   <<" count integer not null,"
			   <<" gtime time not null)";
			
			if (sqlite3_exec(m_impl->pdb, csql.str().c_str(), 0, 0, &err) == SQLITE_OK)
				m_impl->init = true;
		} else {
			m_impl->init = true;
		}
		sqlite3_free_table(result);
	}
}

History::~History()
{
	if (m_impl->pdb) {
		sqlite3_close(m_impl->pdb);	
		m_impl->pdb = 0;
		m_impl->init = false;
	}
	delete m_impl;
}

void History::addHistory(const std::string& title, const std::string& url)
{
	if (!m_impl->init)
		return;

	std::stringstream sql;
		sql<<"select id from "<<TABLE_NAME<<" where url=\'"<<url.c_str()<<"\' limit 1";

	char **result = 0;
	int nrow = 0, ncol = 0;
	int ret = sqlite3_get_table(m_impl->pdb, sql.str().c_str(), &result, &nrow, &ncol, 0);
	if ((ret == SQLITE_OK)) {
		if (nrow) {
			std::stringstream isql;
			isql<<"update "<<TABLE_NAME<<" set count=count+1 where url=\'"<<url.c_str()<<"\'";

			sqlite3_exec(m_impl->pdb, isql.str().c_str(), 0, 0, 0);
		} else {
			//insert the history.
			std::stringstream isql;
			isql<<"insert into "<<TABLE_NAME<<"(title, url, count, gtime)"
				<<" values(\'"<<title.c_str()<<"\',\'"<<url.c_str()
				<<"\',1,datetime(\'now\', \'localtime\'))";

			sqlite3_exec(m_impl->pdb, isql.str().c_str(), 0, 0, 0);
		}
	}
	sqlite3_free_table(result);
}

unsigned int History::getHistoryCount(void)
{
	if (!m_impl->init)
		return 0;

	unsigned int num = 0;
	std::stringstream sql;
	sql<<"select count(id) as num from "<<TABLE_NAME;

	sqlite3_stmt* stmt = 0;
	if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			num = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	return num;
}

void History::clearAllHistory(void)
{
	if (!m_impl->init)
		return;

	std::stringstream sql;
		sql<<"delete from "<<TABLE_NAME;
	sqlite3_exec(m_impl->pdb, sql.str().c_str(), 0, 0, 0);
}

std::vector<History::Item> History::getHistoryItems(unsigned int offset, unsigned int nums)
{
	std::vector<History::Item> items;
	if (!m_impl->init)
		return items;

	std::stringstream sql;
	sql<<"select * from "<<TABLE_NAME<<" order by count, gtime DESC limit "<<nums<<" offset "<<offset;

	sqlite3_stmt* stmt = 0;
	if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			History::Item item;
			item.id = sqlite3_column_int(stmt, 0);
			item.title = (char*)sqlite3_column_text(stmt, 1);
			item.url = (char*)sqlite3_column_text(stmt, 2);
			items.push_back(item);
		}
	}
	sqlite3_finalize(stmt);
	return items;
}

