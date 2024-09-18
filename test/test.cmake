# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

set(APP_SRCS 
    ${PROJ_ROOT}/test/callback.cpp
    ${PROJ_ROOT}/test/interface.cpp
)

set(APP_NAME agave_test)

if (WIN32)
    set(APP_SRCS ${APP_SRCS}
        ${PROJ_ROOT}/test/win32/functions.cpp
        ${PROJ_ROOT}/test/win32/main.cpp
        ${PROJ_ROOT}/test/win32/popmenu.cpp
    )
    set(APP_TYPE WIN32)
else()
    set(APP_SRCS ${APP_SRCS}
        ${PROJ_ROOT}/test/gtk2/functions.cpp
        ${PROJ_ROOT}/test/gtk2/main.cpp
        ${PROJ_ROOT}/test/gtk2/popmenu.cpp
    )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdeprecated-declarations")

    include(FindPkgConfig)
    pkg_search_module(GTK2 REQUIRED gtk+-2.0)
    set(SYSTEM_INCLUDE ${GTK2_INCLUDE_DIRS})
    set(SYSTEM_LIBS ${GTK2_LIBRARIES} pthread m z stdc++)
endif()

add_executable(${APP_NAME} ${APP_TYPE} ${APP_SRCS})

set_property(TARGET ${APP_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS ${APP_NAME} RUNTIME DESTINATION bin)

target_include_directories(${APP_NAME} PRIVATE 
                    ${PROJ_ROOT}/include
                    ${PROJ_ROOT}/test
                    SYSTEM PUBLIC
                    ${SYSTEM_INCLUDE}
)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME} ${LIB_DEPS} picasso2_sw ${SYSTEM_LIBS} 
)

