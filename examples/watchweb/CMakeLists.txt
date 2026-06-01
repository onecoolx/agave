# CMakeLists.txt - WatchWeb smartwatch browser
cmake_minimum_required(VERSION 3.16)
project(watchweb LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Paths
set(AGAVE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(TOUCHWEB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../touchweb/src)
set(VGCL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../touchweb/vgcl)

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

# Touchweb shared sources (core logic, reused as-is)
set(TOUCHWEB_SOURCES
    ${TOUCHWEB_SRC}/application.cpp
    ${TOUCHWEB_SRC}/mainwindow.cpp
    ${TOUCHWEB_SRC}/webview.cpp
    ${TOUCHWEB_SRC}/widget.cpp
    ${TOUCHWEB_SRC}/toolbar.cpp
    ${TOUCHWEB_SRC}/addressbar.cpp
    ${TOUCHWEB_SRC}/tabpage.cpp
    ${TOUCHWEB_SRC}/pagemgr.cpp
    ${TOUCHWEB_SRC}/ctrlpanel.cpp
    ${TOUCHWEB_SRC}/dialog.cpp
    ${TOUCHWEB_SRC}/contextmenu.cpp
    ${TOUCHWEB_SRC}/search.cpp
    ${TOUCHWEB_SRC}/setting.cpp
    ${TOUCHWEB_SRC}/history.cpp
    ${TOUCHWEB_SRC}/favorites.cpp
    ${TOUCHWEB_SRC}/autofill.cpp
    ${TOUCHWEB_SRC}/engine.cpp
    ${TOUCHWEB_SRC}/network.cpp
    ${TOUCHWEB_SRC}/netstat.cpp
    ${TOUCHWEB_SRC}/extracfg.cpp
    ${TOUCHWEB_SRC}/files.cpp
    ${TOUCHWEB_SRC}/scripts.cpp
    ${TOUCHWEB_SRC}/url.cpp
    ${TOUCHWEB_SRC}/unicode.cpp
    ${TOUCHWEB_SRC}/timer.cpp
    ${TOUCHWEB_SRC}/imclient.cpp
    ${TOUCHWEB_SRC}/renderthread.cpp
    ${TOUCHWEB_SRC}/about.cpp
    ${TOUCHWEB_SRC}/popmenu.cpp
    ${TOUCHWEB_SRC}/dashlayer.cpp
    ${TOUCHWEB_SRC}/hismgr.cpp
    ${TOUCHWEB_SRC}/favmgr.cpp
    ${TOUCHWEB_SRC}/fillmgr.cpp
    ${TOUCHWEB_SRC}/secmgr.cpp
    ${TOUCHWEB_SRC}/button.cpp
    ${TOUCHWEB_SRC}/shadebtn.cpp
    ${TOUCHWEB_SRC}/colorbtn.cpp
    ${TOUCHWEB_SRC}/floatbtn.cpp
    ${TOUCHWEB_SRC}/scrollview.cpp
    ${TOUCHWEB_SRC}/edit.cpp
    ${TOUCHWEB_SRC}/layer.cpp
    ${TOUCHWEB_SRC}/panel.cpp
    ${TOUCHWEB_SRC}/topmost.cpp
)

# LVGL platform backend (replaces qt5/)
set(LVGL_BACKEND_SOURCES
    src/lvgl/main.cpp
    src/lvgl/application_lvgl.cpp
    src/lvgl/mainwindow_lvgl.cpp
    src/lvgl/webview_lvgl.cpp
    src/lvgl/timer_lvgl.cpp
)

# SQLite
set(SQLITE_SOURCES
    ${TOUCHWEB_SRC}/sqlite/sqlite3.c
    ${TOUCHWEB_SRC}/ConvertUTF.c
)

# VGCL
set(VGCL_SOURCES
    ${VGCL_ROOT}/src/vthread.cpp
    ${VGCL_ROOT}/src/vapplication.cpp
    ${VGCL_ROOT}/src/vgraphic.cpp
    ${VGCL_ROOT}/src/posix/vplatform_thread.cpp
)

add_executable(watchweb
    ${LVGL_BACKEND_SOURCES}
    ${TOUCHWEB_SOURCES}
    ${SQLITE_SOURCES}
    ${VGCL_SOURCES}
)

target_include_directories(watchweb PRIVATE
    src             # watchweb config.h (overrides touchweb's)
    src/lvgl        # LVGL platform headers
    ${TOUCHWEB_SRC}
    ${TOUCHWEB_SRC}/sqlite
    ${VGCL_ROOT}/include
    ${VGCL_ROOT}/include/vgcl
    ${VGCL_ROOT}/src/posix
    ${AGAVE_ROOT}/include
    ${AGAVE_ROOT}/proj  # mconfig.h
)

target_compile_definitions(watchweb PRIVATE
    QT5=1
)

# Force-include our config.h before touchweb's (overrides sqlite3_key etc.)
target_compile_options(watchweb PRIVATE
    -include ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
)

# Link
find_library(AGAVE_LIB agave PATHS ${AGAVE_ROOT}/proj NO_DEFAULT_PATH)
set(AGAVE_DEPS_DIR ${AGAVE_ROOT}/proj)
target_link_libraries(watchweb PRIVATE
    lvgl
    ${SDL2_LIBRARIES}
    ${AGAVE_LIB}
    ${AGAVE_DEPS_DIR}/picasso/src/picasso-build/libpicasso2_sw.a
    ${AGAVE_DEPS_DIR}/zlib/src/zlib-build/libz.a
    ${AGAVE_DEPS_DIR}/libxml2/src/libxml2-build/libxml2.a
    ${AGAVE_DEPS_DIR}/libjpeg-turbo/src/libjpeg-turbo-build/libjpeg.a
    ${AGAVE_DEPS_DIR}/quickjs/src/quickjs-build/libqjslib.a
    pthread dl m freetype
)
