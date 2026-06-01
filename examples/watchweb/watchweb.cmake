# WatchWeb - smartwatch browser (LVGL + SDL backend)
#
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com
#
# NOTE: WatchWeb uses FetchContent for LVGL, best built standalone:
#   cd examples/watchweb/build && cmake .. && cmake --build .
#
# To include in main build, LVGL must be pre-installed or fetched at top level.

set(APP_WATCH_DIR ${PROJ_ROOT}/examples/watchweb)
set(APP_WATCH watchweb)

message(STATUS "WatchWeb: use standalone build (cd examples/watchweb/build && cmake .. && make)")
