/* main.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <windows.h>
#include "config.h"
#include "application.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Application app;
    if (!app.init(hInstance, lpCmdLine)) {
        return 0;
    }
    return app.run();
}
