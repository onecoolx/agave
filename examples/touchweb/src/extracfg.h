/* extracfg.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _EXTRACFG_H_
#define _EXTRACFG_H_

struct config_data;

class ExtraConfig
{
public:
    ExtraConfig();
    ~ExtraConfig();

    void load(void);
    void save(void);

    bool needReport(void) const;
    void setNeedReport(bool b);

    bool getEquipId(std::string&);
    void setEquipId(const std::string&);

private:
    config_data* m_data;
};

#endif /*_EXTRACFG_H_*/
