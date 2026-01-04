#!/bin/sh

astyle --options=code_style.ini --recursive "../examples/*.cpp,*.h"
#astyle --options=code_style.ini --recursive "../ext/*.cpp,*.h"
astyle --options=code_style.ini --recursive "../include/*.h"
