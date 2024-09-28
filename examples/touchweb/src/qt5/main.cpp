/* main.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include "application.h"

int __argc;
char** __argv;

int main(int argc, char* argv[])
{
    __argc = argc;
    __argv == argv;

    Application app;
    if (!app.init(0, argv[1])) {
        return 0;
    }
    return app.run();
}
