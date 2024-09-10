/* extracfg.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include <string.h>
#include "config.h"
#include "extracfg.h"
#include "files.h"
#include "application.h"

#define EQUIPID_LEN 64

struct config_data
{
	int reported;
	int lastdate;
	char equid[EQUIPID_LEN];
};

ExtraConfig::ExtraConfig()
	: m_data(new config_data)
{
	//default value
	m_data->reported = 0;
	m_data->lastdate = Application::getInstance()->today();
	memset(m_data->equid, 0, EQUIPID_LEN);
}

ExtraConfig::~ExtraConfig()
{
	delete m_data;
}

void ExtraConfig::load(void)
{
	File* f = File::OpenFile(DATA_PATH("ext_config.ini"), File::READ_ONLY, File::BINARY);
	if (f) {
		f->ReadData((unsigned char*)m_data, sizeof(config_data));
		File::CloseFile(f);

		int today = Application::getInstance()->today();
		if (m_data->lastdate != today) { //need report for different date.
			m_data->reported = 0;
			m_data->lastdate = today;
		}
	} 
}

void ExtraConfig::save(void)
{
	File* f = File::OpenFile(DATA_PATH("ext_config.ini"), File::READ_WRITE, File::BINARY);
	if (f) {
		f->WriteData((unsigned char*)m_data, sizeof(config_data));
		File::CloseFile(f);
	}
}

bool ExtraConfig::needReport(void) const
{
	return m_data->reported ? false : true;
}


void ExtraConfig::setNeedReport(bool b)
{
	m_data->reported = b ? 0 : 1;
}

bool ExtraConfig::getEquipId(std::string& id)
{
	id = std::string(m_data->equid);	
	if (!id.empty())
		return true;
	else
		return false;
}

void ExtraConfig::setEquipId(const std::string& id)
{
	strncpy(m_data->equid, id.c_str(), EQUIPID_LEN);
}

