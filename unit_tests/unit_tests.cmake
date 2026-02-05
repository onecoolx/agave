# agave web browser
# 
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_library(GMock::GMock INTERFACE IMPORTED)
target_link_libraries(GMock::GMock INTERFACE gmock_main)

file(GLOB_RECURSE TESTS_SOURCES ${PROJ_ROOT}/unit_tests/*.cpp)

#list(REMOVE_ITEM TESTS_SOURCES
#    ${PROJ_ROOT}/unit_tests/main.cpp
#)

set(UNIT_TESTS unit_tests)

include_directories(
    ${PROJ_ROOT}/unit_tests
    ${PROJ_ROOT}/src
    ${PROJ_ROOT}/src/webcore
    ${PROJ_ROOT}/src/webcore/css
    ${PROJ_ROOT}/src/webcore/dom
    ${PROJ_ROOT}/src/webcore/bridge
    ${PROJ_ROOT}/src/webcore/editing
    ${PROJ_ROOT}/src/webcore/html
    ${PROJ_ROOT}/src/webcore/history
    ${PROJ_ROOT}/src/webcore/loader
    ${PROJ_ROOT}/src/webcore/page
    ${PROJ_ROOT}/src/webcore/rendering
    ${PROJ_ROOT}/src/webcore/platform
    ${PROJ_ROOT}/src/webcore/platform/graphics
    ${PROJ_ROOT}/src/webcore/platform/picasso
    ${PROJ_ROOT}/src/webcore/platform/unicode
    ${PROJ_ROOT}/src/webcore/platform/network
    ${PROJ_ROOT}/src/webcore/platform/network/curl
    ${PROJ_ROOT}/src/webcore/platform/image-decoders
    ${PROJ_ROOT}/src/webcore/platform/image-decoders/jpeg
    ${PROJ_ROOT}/src/webcore/platform/image-decoders/gif
    ${PROJ_ROOT}/src/webcore/platform/image-decoders/png
    ${PROJ_ROOT}/src/webcore/platform/image-decoders/bmp
    ${PROJ_ROOT}/src/webcore/platform/image-decoders/ico
    ${PROJ_ROOT}/src/webcore/xml
if (OPT_USE_KJS)
    ${PROJ_ROOT}/src/webcore/buildKJS/
    ${PROJ_ROOT}/src/javascript
endif()
    ${PROJ_ROOT}/src/webview
    ${PROJ_ROOT}/include
    ${PROJ_OUT}/include
    ${PROJ_OUT}/
)
add_executable(${UNIT_TESTS} ${TESTS_SOURCES})
target_link_libraries(${UNIT_TESTS} PRIVATE GTest::GTest GMock::GMock ${LIB_NAME} ${LIB_DEPS} picasso2_sw PUBLIC ${SYSTEM_LIBS})

if (WIN32)
    add_custom_command(
        TARGET ${UNIT_TESTS} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:unit_tests> $<TARGET_FILE_DIR:unit_tests>
        COMMAND_EXPAND_LISTS
    )
else()
    target_compile_options(${UNIT_TESTS} PRIVATE -std=gnu++17)
    configure_file(${PROJ_ROOT}/packages/fonts/ZCOOLXiaoWei-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/ZCOOLXiaoWei-Regular.ttf COPYONLY)
    configure_file(${PROJ_ROOT}/packages/fonts/font_config.cfg ${CMAKE_CURRENT_BINARY_DIR}/font_config.cfg COPYONLY)
endif()

add_test(NAME unittest COMMAND ${UNIT_TESTS})
