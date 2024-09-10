/* url.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <vector>
#include <string.h>
#include "unicode.h"
#include "url.h"

bool urlIsValid(const std::string& url)
{
	// this is very simple rules. 
	// need be rewrite feature.
	const char * purl = url.c_str();

	if ((strncmp(purl, "http://", 7) == 0) ||
		(strncmp(purl, "https://", 8) == 0) ||
		(strncmp(purl, "ftp://", 6) == 0) ||
		(strncmp(purl, "ftps://", 7) == 0) ||
		(strncmp(purl, "file://", 7) == 0))
	{
		// valid scheme
		return true;
	}

	if (strncmp(purl, "www.", 4) == 0) {
		// www header
		return true;
	}
			
	size_t len = url.length();

	if (strncmp(purl+len-4, ".", 1) == 0 ||
		strncmp(purl+len-3, ".", 1) == 0)
	{
		// valid tailer ".com .cn and so on"
		return true;
	}

	return false;
}

std::string url_strip_query(const char* urlstr)
{
	const char* pend = strchr(urlstr, '?');
	if (pend) {
		return std::string(urlstr, pend-urlstr);
	} else {
		return std::string(urlstr);
	}
}

