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
        ${APP_MOBILE_DIR}/src/win32/mainwindow_win32.cpp
        ${APP_MOBILE_DIR}/src/win32/application_win32.cpp
        ${APP_MOBILE_DIR}/src/win32/main.cpp
        ${APP_MOBILE_DIR}/src/win32/timer_win32.cpp
        ${APP_MOBILE_DIR}/src/win32/network_win32.cpp
        ${APP_MOBILE_DIR}/src/win32/dialog_win32.cpp
        ${APP_MOBILE_DIR}/src/win32/webview_win32.cpp
    )
    include_directories(${APP_MOBILE_DIR}/src/win32)
    set(APP_TYPE WIN32)
else()
    set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
        ${APP_MOBILE_DIR}/src/qt5/mainwindow_qt5.cpp
        ${APP_MOBILE_DIR}/src/qt5/application_qt5.cpp
        ${APP_MOBILE_DIR}/src/qt5/main.cpp
        ${APP_MOBILE_DIR}/src/qt5/timer_qt5.cpp
        ${APP_MOBILE_DIR}/src/qt5/network_qt5.cpp
        ${APP_MOBILE_DIR}/src/qt5/dialog_qt5.cpp
        ${APP_MOBILE_DIR}/src/qt5/webview_qt5.cpp
    )

    include_directories(${APP_MOBILE_DIR}/src/qt5)

    include(FindPkgConfig)
    pkg_search_module(QTCore REQUIRED Qt5Core)
    pkg_search_module(QTGui REQUIRED Qt5Gui)
    pkg_search_module(QTWidgets REQUIRED Qt5Widgets)
    set(SYSTEM_INCLUDE ${QTCore_INCLUDE_DIRS} ${QTGui_INCLUDE_DIRS} ${QTWidgets_INCLUDE_DIRS})
    set(SYSTEM_LIBS ${QTCore_LIBRARIES} ${QTGui_LIBRARIES} ${QTWidgets_LIBRARIES} freetype fontconfig pthread m z stdc++)
endif()

include_directories(${APP_MOBILE_DIR}/src
                    ${APP_MOBILE_DIR}/src/sqlite
)

set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/src/sqlite/sqlite3secure.c
)

if (WIN32)
include_directories(${APP_MOBILE_DIR}/vgcl/src/win32)
set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/vgcl/src/win32/vplatform_thread.cpp
)
else()
include_directories(${APP_MOBILE_DIR}/vgcl/src/posix)
set(APP_MOBILE_SRCS ${APP_MOBILE_SRCS}
    ${APP_MOBILE_DIR}/vgcl/src/posix/vplatform_thread.cpp
)
endif()

set(LIB_VGCL vgcl)

set(VGCL_SRCS 
    ${APP_MOBILE_DIR}/vgcl/src/vapplication.cpp
    ${APP_MOBILE_DIR}/vgcl/src/vgraphic.cpp
    ${APP_MOBILE_DIR}/vgcl/src/vthread.cpp
)

# VGCL library
include_directories(${APP_MOBILE_DIR}/vgcl/include
                    ${APP_MOBILE_DIR}/vgcl/include/vgcl
                    ${APP_MOBILE_DIR}/vgcl/src/ps
)

add_library(${LIB_VGCL} STATIC ${VGCL_SRCS})

add_executable(${APP_MOBILE} ${APP_TYPE} ${APP_MOBILE_SRCS})

target_compile_definitions(${APP_MOBILE} PRIVATE SQLITE_HAS_CODEC)

if (NOT WIN32)
    target_compile_definitions(${APP_MOBILE} PRIVATE QT5)
    target_compile_options(${APP_MOBILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
    configure_file(${PROJ_ROOT}/packages/fonts/ZCOOLXiaoWei-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/ZCOOLXiaoWei-Regular.ttf COPYONLY)
    configure_file(${PROJ_ROOT}/packages/fonts/font_config.cfg ${CMAKE_CURRENT_BINARY_DIR}/font_config.cfg COPYONLY)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")      # GCC
set_property(TARGET ${APP_MOBILE} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
set_property(TARGET ${APP_MOBILE} PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

install(TARGETS ${APP_MOBILE} RUNTIME DESTINATION bin)

target_include_directories(${APP_MOBILE} PRIVATE 
                    ${PROJ_ROOT}/include
                    ${PROJ_ROOT}/include/kjs
                    ${PROJ_ROOT}/include/npapi
                    ${PROJ_ROOT}/include
                    ${PROJ_OUT}/
                    SYSTEM PUBLIC
                    ${SYSTEM_INCLUDE}
)

target_link_libraries(${APP_MOBILE} PRIVATE
                                    ${LIB_VGCL}
                                    ${LIB_NAME}
                                    ${LIB_DEPS}
                                    picasso2_sw
                                    ${SYSTEM_LIBS} 
                                    ${CMAKE_DL_LIBS} 
)

