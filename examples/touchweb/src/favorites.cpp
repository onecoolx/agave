/* favorites.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"

#include <sstream>
#include <string>

#include "favorites.h"
#include "sqlite3.h"

#if FILENAME_UNICODE
#define DB_OPEN sqlite3_open16
#else
#define DB_OPEN sqlite3_open
#endif

#define DB_NAME (DATA_PATH("favorites.dat"))
#define TABLE_NAME "favorites"

using namespace std;

class FavoritesImpl
{
public:
	FavoritesImpl()
		: pdb(0), init(false)
	{
	}

	sqlite3* pdb;
	bool init;
};


Favorites::Favorites()
	: m_impl(new FavoritesImpl)
{
	if (DB_OPEN(DB_NAME, &m_impl->pdb) == SQLITE_OK) {
		char **result = 0;
		char *err = 0;
		int nrow = 0, ncol = 0;
		stringstream sql;
		sql<<"select * from "<<TABLE_NAME<<" limit 1";
		if (sqlite3_get_table(m_impl->pdb, sql.str().c_str(), 
					&result, &nrow, &ncol, &err) != SQLITE_OK) 
		{
			stringstream csql;
			csql<<"create table "<<TABLE_NAME
			   <<"(id integer primary key not null,"
			   <<" title text not null,"
			   <<" url text not null,"
			   <<" tags text,"
			   <<" notes text,"
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

Favorites::~Favorites()
{
	if (m_impl->pdb) {
		sqlite3_close(m_impl->pdb);	
		m_impl->pdb = 0;
		m_impl->init = false;
	}
	delete m_impl;
}

void Favorites::addFavorites(const string& title, const string& url, 
														const string& tags, const string& note)
{
	if (!m_impl->init)
		return;

	stringstream sql;
		sql<<"select id from "<<TABLE_NAME<<" where url=\'"<<url.c_str()<<"\' limit 1";

	char **result = 0;
	int nrow = 0, ncol = 0;
	int ret = sqlite3_get_table(m_impl->pdb, sql.str().c_str(), &result, &nrow, &ncol, 0);
	if ((ret == SQLITE_OK)) {
		if (!nrow) {
			//insert the favorites.
			stringstream isql;
			isql<<"insert into "<<TABLE_NAME<<"(title, url, count, gtime";

				if (!tags.empty()) 
					isql<<", tags";
				if (!note.empty())
					isql<<", notes";

				isql<<") values(\'"<<title.c_str()<<"\',\'"<<url.c_str()
				<<"\',1,datetime(\'now\', \'localtime\')";
				
				if (!tags.empty()) 
					isql<<",\'"<<tags.c_str()<<"\'";
				if (!note.empty())
					isql<<",\'"<<note.c_str()<<"\'";

				isql<<")";

			sqlite3_exec(m_impl->pdb, isql.str().c_str(), 0, 0, 0);
		}
	}
	sqlite3_free_table(result);
}

void Favorites::increaseCount(int id)
{
	if (!m_impl->init)
		return;

	stringstream sql;
	sql<<"update "<<TABLE_NAME<<" set count=count+1 where id="<<id;
	sqlite3_exec(m_impl->pdb, sql.str().c_str(), 0, 0, 0);
}

void Favorites::deleteFavortesByID(int id)
{
	if (!m_impl->init)
		return;

	stringstream sql;
	sql<<"delete from "<<TABLE_NAME<<" where id="<<id;
	sqlite3_exec(m_impl->pdb, sql.str().c_str(), 0, 0, 0);
}

unsigned int Favorites::getFavoritesCount(void)
{
	if (!m_impl->init)
		return 0;

	unsigned int num = 0;
	stringstream sql;
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

std::vector<Favorites::Item> Favorites::getFavoritesItems(unsigned int offset, unsigned int nums)
{
	std::vector<Favorites::Item> items;
	if (!m_impl->init)
		return items;

	stringstream sql;
	sql<<"select * from "<<TABLE_NAME<<" order by count DESC limit "<<nums<<" offset "<<offset;

	sqlite3_stmt* stmt = 0;
	if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Favorites::Item item;
			item.id = sqlite3_column_int(stmt, 0);
			item.title = (char*)sqlite3_column_text(stmt, 1);
			item.url = (char*)sqlite3_column_text(stmt, 2);
			if ((char*)sqlite3_column_text(stmt, 3))
				item.tags = (char*)sqlite3_column_text(stmt, 3);
			items.push_back(item);
		}
	}
	sqlite3_finalize(stmt);
	return items;
}

std::vector<Favorites::Item> Favorites::searchFavoritesItems(const string& key, unsigned int offset, unsigned int nums)
{
	std::vector<Favorites::Item> items;
	if (!m_impl->init)
		return items;

	stringstream sql;
	sql<<"select * from "<<TABLE_NAME<<" where title like \"%"<<key.c_str()<<"%\" or "
		<<"tags like \"%"<<key.c_str()<<"%\" or url like \"%"<<key.c_str()<<"%\""
		<<" order by count DESC limit "<<nums<<" offset "<<offset;

	sqlite3_stmt* stmt = 0;
	if (sqlite3_prepare(m_impl->pdb, sql.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Favorites::Item item;
			item.id = sqlite3_column_int(stmt, 0);
			item.title = (char*)sqlite3_column_text(stmt, 1);
			item.url = (char*)sqlite3_column_text(stmt, 2);
			if ((char*)sqlite3_column_text(stmt, 3))
				item.tags = (char*)sqlite3_column_text(stmt, 3);
			items.push_back(item);
		}
	}
	sqlite3_finalize(stmt);
	return items;
}
