/* autofill.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _AUTO_FILL_H_
#define _AUTO_FILL_H_

#include <map>

class AutoFillImpl;

class AutoFill
{
public:
    struct FormData {
        int id;
        std::string url;
        std::string formid;
        std::string action;
        std::map<std::string, std::string> values;
        int notsave;
        FormData(): id(0), notsave(0) {}
    };

    AutoFill();
    ~AutoFill();

    bool findFormData(const std::string& baseurl, FormData& data, int* presid = 0);

    bool addFormData(const FormData& data);
    bool updateFormData(const FormData& data, int presid = -1);

    void clearAllFormData(void);

private:
    AutoFillImpl* m_impl;
};

inline bool operator == (const AutoFill::FormData& a, const AutoFill::FormData& b)
{
    return (a.url == b.url) &&
           (a.formid == b.formid) &&
           (a.action == b.action) &&
           (a.values == b.values);
}

#endif /*_AUTO_FILL_H_*/
