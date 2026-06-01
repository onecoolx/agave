/* main.cpp - WatchWeb entry point
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "application.h"

int __argc;
char** __argv;

int main(int argc, char** argv)
{
    __argc = argc;
    __argv = argv;

    Application app;
    if (!app.init(nullptr, argc > 1 ? argv[1] : (char*)"about:blank"))
        return -1;
    return app.run();
}
