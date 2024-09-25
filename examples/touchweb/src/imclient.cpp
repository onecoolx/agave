/* imclient.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "imclient.h"
#include "application.h"

ImClient::ImClient(Widget* parent)
    : DashLayer(parent)
{
#ifdef WINCE
    DashLayer::setCancelText(U("ƴ^"));
#else
    DashLayer::setCancel(false);
#endif
}

ImClient::~ImClient()
{
}

void ImClient::Cancel(void)
{
    if (ImeIsShow()) {
        SetImeStatus(false);
    } else {
        SetImeStatus(true);
    }
}
