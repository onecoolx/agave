# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

set(APP_SRCS 
    ${PROJ_ROOT}/test/callback.cpp
    ${PROJ_ROOT}/test/interface.cpp
    ${PROJ_ROOT}/test/gtk2/functions.cpp
    ${PROJ_ROOT}/test/gtk2/main.cpp
    ${PROJ_ROOT}/test/gtk2/popmenu.cpp
)

set(APP_NAME agave_browser)

include(FindPkgConfig)
pkg_search_module(GTK2 REQUIRED gtk+-2.0)

add_executable(${APP_NAME} ${APP_SRCS})

target_include_directories(${APP_NAME} PRIVATE 
                    ${PROJ_ROOT}/include
                    ${PROJ_ROOT}/test
                    SYSTEM PUBLIC
                    ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME} ${LIB_DEPS}
                                  PUBLIC
                                  ${GTK2_LIBRARIES} 
                                  picasso2_sw pthread m z stdc++
)


