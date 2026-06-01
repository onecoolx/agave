# agave web browser - examples
#
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com

# TouchWeb - mobile browser (Qt5/Win32)
if (OPT_EXAMPLES_TOUCH)
include(${PROJ_ROOT}/examples/touchweb/touchweb.cmake)
endif()

# WatchWeb - smartwatch browser (LVGL/SDL) - standalone build
if (OPT_EXAMPLES_WATCH)
include(${PROJ_ROOT}/examples/watchweb/watchweb.cmake)
endif()
