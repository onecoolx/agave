# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

set(APP_MOBILE_DIR ${PROJ_ROOT}/examples/touchweb)

set(APP_MOBILE_SRCS
    ${APP_MOBILE_DIR}/src/ConvertUTF.c
    ${APP_MOBILE_DIR}/src/about.cpp
    ${APP_MOBILE_DIR}/src/addressbar.cpp
    ${APP_MOBILE_DIR}/src/application.cpp
    ${APP_MOBILE_DIR}/src/autofill.cpp
    ${APP_MOBILE_DIR}/src/button.cpp
    ${APP_MOBILE_DIR}/src/colorbtn.cpp
    ${APP_MOBILE_DIR}/src/contextmenu.cpp
    ${APP_MOBILE_DIR}/src/ctrlpanel.cpp
    ${APP_MOBILE_DIR}/src/dashlayer.cpp
    ${APP_MOBILE_DIR}/src/dialog.cpp
    ${APP_MOBILE_DIR}/src/edit.cpp
    ${APP_MOBILE_DIR}/src/engine.cpp
    ${APP_MOBILE_DIR}/src/extracfg.cpp
    ${APP_MOBILE_DIR}/src/favmgr.cpp
    ${APP_MOBILE_DIR}/src/favorites.cpp
    ${APP_MOBILE_DIR}/src/files.cpp
    ${APP_MOBILE_DIR}/src/fillmgr.cpp
    ${APP_MOBILE_DIR}/src/floatbtn.cpp
    ${APP_MOBILE_DIR}/src/hismgr.cpp
    ${APP_MOBILE_DIR}/src/history.cpp
    ${APP_MOBILE_DIR}/src/imclient.cpp
    ${APP_MOBILE_DIR}/src/layer.cpp
    ${APP_MOBILE_DIR}/src/mainwindow.cpp
    ${APP_MOBILE_DIR}/src/netstat.cpp
    ${APP_MOBILE_DIR}/src/network.cpp
    ${APP_MOBILE_DIR}/src/pagemgr.cpp
    ${APP_MOBILE_DIR}/src/panel.cpp
    ${APP_MOBILE_DIR}/src/popmenu.cpp
    ${APP_MOBILE_DIR}/src/renderthread.cpp
    ${APP_MOBILE_DIR}/src/scripts.cpp
    ${APP_MOBILE_DIR}/src/scrollview.cpp
    ${APP_MOBILE_DIR}/src/search.cpp
    ${APP_MOBILE_DIR}/src/secmgr.cpp
    ${APP_MOBILE_DIR}/src/setting.cpp
    ${APP_MOBILE_DIR}/src/shadebtn.cpp
    ${APP_MOBILE_DIR}/src/tabpage.cpp
    ${APP_MOBILE_DIR}/src/timer.cpp
    ${APP_MOBILE_DIR}/src/toolbar.cpp
    ${APP_MOBILE_DIR}/src/topmost.cpp
    ${APP_MOBILE_DIR}/src/unicode.cpp
    ${APP_MOBILE_DIR}/src/url.cpp
    ${APP_MOBILE_DIR}/src/webview.cpp
    ${APP_MOBILE_DIR}/src/widget.cpp
)

set(APP_MOBILE touchweb)

if (WIN32)
    set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
        ${PROJ_ROOT}/test/win32/functions.cpp
        ${PROJ_ROOT}/test/win32/main.cpp
        ${PROJ_ROOT}/test/win32/popmenu.cpp
    )
    set(APP_TYPE WIN32)
else()
    set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
        ${APP_MOBILE_DIR}/src/gtk2/mainwindow_gtk2.cpp
        ${APP_MOBILE_DIR}/src/gtk2/application_gtk2.cpp
        ${APP_MOBILE_DIR}/src/gtk2/main.cpp
        ${APP_MOBILE_DIR}/src/gtk2/timer_gtk2.cpp
        ${APP_MOBILE_DIR}/src/gtk2/network_gtk2.cpp
        ${APP_MOBILE_DIR}/src/gtk2/dialog_gtk2.cpp
        ${APP_MOBILE_DIR}/src/gtk2/webview_gtk2.cpp
    )

    include_directories(${APP_MOBILE_DIR}/src/gtk2
    )

    include(FindPkgConfig)
    pkg_search_module(GTK2 REQUIRED gtk+-2.0)
    set(SYSTEM_INCLUDE ${GTK2_INCLUDE_DIRS})
    set(SYSTEM_LIBS ${GTK2_LIBRARIES} pthread m z stdc++)
endif()

include_directories(${APP_MOBILE_DIR}/src
                    ${APP_MOBILE_DIR}/src/sqlite
)

set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/src/sqlite/sqlite3secure.c
)

# VGCL library
include_directories(${APP_MOBILE_DIR}/vgcl/include
                    ${APP_MOBILE_DIR}/vgcl/include/vgcl
                    ${APP_MOBILE_DIR}/vgcl/src/ps
)

if (WIN32)
else()
include_directories(${APP_MOBILE_DIR}/vgcl/src/posix
)
set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/vgcl/src/posix/vplatform_thread.cpp
)
endif()

set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/vgcl/src/vapplication.cpp
    ${APP_MOBILE_DIR}/vgcl/src/vgraphic.cpp
    ${APP_MOBILE_DIR}/vgcl/src/vthread.cpp
)

add_executable(${APP_MOBILE} ${APP_TYPE} ${APP_MOBILE_SRCS})

target_compile_definitions(${APP_MOBILE} PRIVATE SQLITE_HAS_CODEC)

if (WIN32)
else()
target_compile_definitions(${APP_MOBILE} PRIVATE GTK2)
endif()

set_property(TARGET ${APP_MOBILE} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS ${APP_MOBILE} RUNTIME DESTINATION bin)

target_include_directories(${APP_MOBILE} PRIVATE 
                    ${PROJ_ROOT}/include
                    SYSTEM PUBLIC
                    ${SYSTEM_INCLUDE}
)

target_link_libraries(${APP_MOBILE} PRIVATE
                                    ${LIB_NAME}
                                    ${LIB_DEPS}
                                    picasso2_sw
                                    ${SYSTEM_LIBS} 
                                    ${CMAKE_DL_LIBS} 
)

