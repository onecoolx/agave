/* unicode.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _UNICODE_H_
#define _UNICODE_H_

class Unicode
{
public:
    static ustring ConvertUTF8ToUTF16(const std::string& str);
    static std::string ConvertUTF16ToUTF8(const ustring& str);
};

#endif/*_UNICODE_H_*/
