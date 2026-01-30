#!/bin/sh

astyle --options=code_style.ini --recursive "../examples/*.cpp,*.h"
astyle --options=code_style.ini --recursive "../unit_tests/*.cpp,*.h"
astyle --options=code_style.ini --recursive "../include/*.h"
