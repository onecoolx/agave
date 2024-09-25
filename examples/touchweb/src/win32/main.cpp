/* main.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include <windows.h>
#include "config.h"
#include "application.h"

#ifdef WINCE
    int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
    int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    Application app;
    if (!app.init(hInstance, lpCmdLine)) {
        return 0;
    }
    return app.run();
}
