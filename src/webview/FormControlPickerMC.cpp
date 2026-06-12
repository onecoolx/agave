/*
** FormControlPickerMC.cpp: host-delegated date/color picker implementation.
**
** Copyright(C) 2026 Zhang Ji Peng (onecoolx@gmail.com)
**
*/
#include "config.h"

#include "FormControlPicker.h"

#include "CString.h"
#include "WebCallback.h"

using namespace MaCross;

namespace WebCore {

// Largest value we accept back from a host picker. Date ("YYYY-MM-DD") and
// color ("#rrggbb") values are tiny; this is a safe upper bound.
static const unsigned kPickerValueMax = 64;

bool chooseDateValue(const String& initialValue, String& result)
{
    if (!cb_choose_date)
        return false;

    char buffer[kPickerValueMax + 1];
    memset(buffer, 0, sizeof(buffer));
    CString initial = initialValue.utf8();
    if (cb_choose_date(buffer, kPickerValueMax, initial.data())) {
        result = String::fromUTF8(buffer);
        return true;
    }
    return false;
}

bool chooseColorValue(const String& initialValue, String& result)
{
    if (!cb_choose_color)
        return false;

    char buffer[kPickerValueMax + 1];
    memset(buffer, 0, sizeof(buffer));
    CString initial = initialValue.utf8();
    if (cb_choose_color(buffer, kPickerValueMax, initial.data())) {
        result = String::fromUTF8(buffer);
        return true;
    }
    return false;
}

} // namespace WebCore

