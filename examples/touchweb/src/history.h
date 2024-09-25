/* history.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <vector>

class HistoryImpl;

class History
{
public:
    typedef struct {
        int id;
        std::string title;
        std::string url;
    } Item;

    History();
    ~History();

    std::vector<Item> getHistoryItems(unsigned int offset, unsigned int nums);
    unsigned int getHistoryCount(void);
    void clearAllHistory(void);
    void addHistory(const std::string& utf8_title, const std::string& utf8_url);
private:
    HistoryImpl* m_impl;
};

#endif/*_HISTORY_H_*/
