/* scripts.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "scripts.h"

#include <sstream>
#include <string>

char js_func_get_obj[] = \
                         "function get_obj(name){"
                         "var obj = document.getElementById(name);"
                         "if (obj == null) {"
                         "obj = document.getElementsByName(name)[0];"
                         "}"
                         "return obj;"
                         "} ";

char js_func_url_strip_query[] = \
                                 "function url_strip_query(url){"
                                 " var pos = url.indexOf('?', 0); "
                                 " if (pos == -1)"
                                 " return url; "
                                 " else "
                                 " return url.substr(0, pos); "
                                 "} ";

static std::string script_tools;

void Scripts_init(void)
{
    script_tools += js_func_get_obj;
    script_tools += js_func_url_strip_query;
}

std::string createFormFillScript(const AutoFill::FormData& data)
{
    std::stringstream fill_script;

    fill_script << "var form = get_obj(\'"
                << data.formid.c_str() << "\'); "
                << " if (form == null) {"
                << " for (i=0; i<document.forms.length;i++){"
                << " if (url_strip_query(document.forms[i].action)==\'" << data.action.c_str() << "\'){"
                << " form = document.forms[i]; break;}"
                << " }"
                << " }"
                << " if (form != null) { var item; ";

    for (std::map<std::string, std::string>::const_iterator itr = data.values.begin();
         itr != data.values.end(); ++itr) {
        // fill form values
        fill_script << " item = get_obj(\'"
                    << itr->first.c_str() << "\'); "
                    << " if (item != null) {"
                    << "item.value=\'"
                    << itr->second.c_str()
                    << "\';" << "}";
    }
    fill_script << "}";

    return script_tools + fill_script.str();
}
