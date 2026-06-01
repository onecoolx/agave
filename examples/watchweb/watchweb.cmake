# WatchWeb - smartwatch browser (LVGL + SDL backend)
#
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com

set(APP_WATCH_DIR ${PROJ_ROOT}/examples/watchweb)
set(APP_WATCH watchweb)

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
    ${APP_WATCH_DIR}   # lv_conf.h is here
    ${SDL2_INCLUDE_DIRS}
)
target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(lvgl PUBLIC ${SDL2_LIBRARIES})


set(APP_WATCH_SRCS
    ${APP_WATCH_DIR}/src/main.cpp
    ${APP_WATCH_DIR}/src/webview.cpp
    ${APP_WATCH_DIR}/src/ui.cpp
)

add_executable(${APP_WATCH} ${APP_WATCH_SRCS})
target_compile_definitions(${APP_WATCH} PUBLIC LV_CONF_INCLUDE_SIMPLE)

configure_file(${PROJ_ROOT}/packages/fonts/ZCOOLXiaoWei-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/ZCOOLXiaoWei-Regular.ttf COPYONLY)
configure_file(${PROJ_ROOT}/packages/fonts/font_config.cfg ${CMAKE_CURRENT_BINARY_DIR}/font_config.cfg COPYONLY)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")      # GCC
set_property(TARGET ${APP_WATCH} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
set_property(TARGET ${APP_WATCH} PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

install(TARGETS ${APP_WATCH} RUNTIME DESTINATION bin)

target_include_directories(${APP_WATCH} PRIVATE 
                    ${lvgl_SOURCE_DIR}
                    ${PROJ_ROOT}/include
                    ${PROJ_OUT}/
                    SYSTEM PUBLIC
                    ${SYSTEM_INCLUDE}
)

target_link_libraries(${APP_WATCH} PRIVATE
                                    lvgl
                                    ${SDL2_LIBRARIES}
                                    ${LIB_NAME}
                                    picasso2_sw
                                    ${SYSTEM_LIBS} 
                                    ${CMAKE_DL_LIBS} 
)



