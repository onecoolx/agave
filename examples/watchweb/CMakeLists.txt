# CMakeLists.txt - WatchWeb smartwatch browser
# Copyright (C) 2026 Zhang Ji Peng <onecoolx@gmail.com>
cmake_minimum_required(VERSION 3.16)
project(watchweb LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Agave engine
set(AGAVE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Fetch LVGL
include(FetchContent)
FetchContent_Declare(
    lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG v9.2.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lvgl)

# LVGL config
target_compile_definitions(lvgl PUBLIC
    LV_USE_SDL=1
    LV_USE_OS=LV_OS_NONE
    LV_COLOR_DEPTH=32
    LV_FONT_MONTSERRAT_14=1
    LV_USE_LOG=0
)
target_include_directories(lvgl PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(lvgl PUBLIC ${SDL2_LIBRARIES})

# WatchWeb sources
add_executable(watchweb
    src/main.cpp
    src/webview.cpp
    src/ui.cpp
)

target_include_directories(watchweb PRIVATE
    src
    ${AGAVE_ROOT}/include
    ${AGAVE_ROOT}/proj
)

# Link
find_library(AGAVE_LIB agave PATHS ${AGAVE_ROOT}/proj NO_DEFAULT_PATH)
target_link_libraries(watchweb PRIVATE
    lvgl
    ${SDL2_LIBRARIES}
    ${AGAVE_LIB}
    ${AGAVE_ROOT}/proj/picasso/src/picasso-build/libpicasso2_sw.a
    ${AGAVE_ROOT}/proj/zlib/src/zlib-build/libz.a
    ${AGAVE_ROOT}/proj/libxml2/src/libxml2-build/libxml2.a
    ${AGAVE_ROOT}/proj/libjpeg-turbo/src/libjpeg-turbo-build/libjpeg.a
    ${AGAVE_ROOT}/proj/quickjs/src/quickjs-build/libqjslib.a
    freetype pthread dl m
)
