/* unicode.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <string.h>
#include "config.h"
#include "unicode.h"
#include "ConvertUTF.h"

ustring Unicode::ConvertUTF8ToUTF16(const std::string& str)
{
    if (str.empty()) {
        return ustring();
    }

    size_t len = str.length();
    UTF16* buffer = new UTF16[len + 1];
    while (true) {
        memset(buffer, 0, sizeof(UTF16) * (len + 1));
        const UTF8* start = (const UTF8*)str.c_str();
        const UTF8* end = (const UTF8*)str.c_str() + str.length();
        UTF16* target = buffer;
        UTF16* tend = buffer + len;
        ConversionResult ret = ConvertUTF8toUTF16(&start, end, &target, tend, lenientConversion);
        if (ret == conversionOK) {
            break;
        } else if (ret == sourceIllegal) {
            delete [] buffer;
            return ustring();
        } else if (ret == sourceExhausted) {
            delete [] buffer;
            return ustring();
        } else if (ret == targetExhausted) {
            len = len << 1;
            delete [] buffer;
            buffer = new UTF16[len + 1];
            continue;
        }
    }
    ustring result((uchar_t*)buffer);
    delete [] buffer;
    return result;
}

std::string Unicode::ConvertUTF16ToUTF8(const ustring& str)
{
    if (str.empty()) {
        return std::string();
    }

    size_t len = str.length();
    UTF8* buffer = new UTF8[len + 1];
    while (true) {
        memset(buffer, 0, sizeof(UTF8) * (len + 1));
        const UTF16* start = (const UTF16*)str.c_str();
        const UTF16* end = (const UTF16*)str.c_str() + str.length();
        UTF8* target = buffer;
        UTF8* tend = buffer + len;
        ConversionResult ret = ConvertUTF16toUTF8(&start, end, &target, tend, lenientConversion);
        if (ret == conversionOK) {
            break;
        } else if (ret == sourceIllegal) {
            delete [] buffer;
            return std::string();
        } else if (ret == sourceExhausted) {
            delete [] buffer;
            return std::string();
        } else if (ret == targetExhausted) {
            len = len << 1;
            delete [] buffer;
            buffer = new UTF8[len + 1];
            continue;
        }
    }
    std::string result((char*)buffer);
    delete [] buffer;
    return result;
}
