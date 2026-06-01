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
FetchContent_Populate(lvgl)

# Remove ARM assembly files (not needed on x86)
file(GLOB_RECURSE _asm_files ${lvgl_SOURCE_DIR}/src/*.S)
if(_asm_files)
    file(REMOVE ${_asm_files})
endif()

add_subdirectory(${lvgl_SOURCE_DIR} ${lvgl_BINARY_DIR})

# LVGL config: use our lv_conf.h (placed next to lvgl source via include path)
target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}   # lv_conf.h is here
    ${SDL2_INCLUDE_DIRS}
)
target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)
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
