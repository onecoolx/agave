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
        ${PROJ_ROOT}/test/qt5/functions.cpp
        ${PROJ_ROOT}/test/qt5/main.cpp
        ${PROJ_ROOT}/test/qt5/popmenu.cpp
    )

    include(FindPkgConfig)
    pkg_search_module(QTCore REQUIRED Qt5Core)
    pkg_search_module(QTGui REQUIRED Qt5Gui)
    pkg_search_module(QTWidgets REQUIRED Qt5Widgets)
    set(SYSTEM_INCLUDE ${QTCore_INCLUDE_DIRS} ${QTGui_INCLUDE_DIRS} ${QTWidgets_INCLUDE_DIRS})
    set(SYSTEM_LIBS ${QTCore_LIBRARIES} ${QTGui_LIBRARIES} ${QTWidgets_LIBRARIES} freetype fontconfig pthread m z stdc++)
endif()

add_executable(${APP_NAME} ${APP_TYPE} ${APP_SRCS})

set_property(TARGET ${APP_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

if (NOT WIN32)
    target_compile_options(${APP_NAME} PRIVATE -std=c++11)
endif()

install(TARGETS ${APP_NAME} RUNTIME DESTINATION bin)

target_include_directories(${APP_NAME} PRIVATE 
                    ${PROJ_ROOT}/include
                    ${PROJ_ROOT}/test
                    ${PROJ_OUT}/
                    SYSTEM PUBLIC
                    ${SYSTEM_INCLUDE}
)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME} ${LIB_DEPS} picasso2_sw PUBLIC ${SYSTEM_LIBS}
)

