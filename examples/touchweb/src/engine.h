/* engine.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _SEARCH_ENGINE_H_
#define _SEARCH_ENGINE_H_

#include <vector>

class EngineImpl;

class Engine
{
public:
	typedef struct {
		int id;
		ustring name;
		unsigned char* img;
		int w;
		int h;
		int p;
		ps_color c;
		bool page_fold;
	}Item;

	Engine();
	~Engine();

	size_t getCount(void) const;
	int getDefaultId(void) const;
	ustring getDefaultName(void) const;
	std::string searchByDefault(const std::string& keyword, bool* need_fold=0);

	std::vector<Item> getEngines(void);

	void setDefaultById(int id);

private:
	EngineImpl* m_impl;
	int current;
};

#endif/*_SEARCH_ENGINE_H_*/
