/* url.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _URL_CHECK_H_
#define _URL_CHECK_H_

#include <string>

bool urlIsValid(const std::string& url);
std::string url_strip_query(const char* urlstr);

#endif/*_URL_CHECK_H_*/

