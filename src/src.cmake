# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

file(GLOB_RECURSE SOURCES ${PROJ_ROOT}/src/*.cpp ${PROJ_ROOT}/src/*.c)

list(REMOVE_ITEM SOURCES 
    ${PROJ_ROOT}/src/javascript/pcre/chartables.c
    ${PROJ_ROOT}/src/javascript/pcre/ucptable.c
    ${PROJ_ROOT}/src/javascript/pcre/dftables.c
    ${PROJ_ROOT}/src/webcore/buildsrc/CSSPropertyNames.c
    ${PROJ_ROOT}/src/webcore/buildsrc/CSSValueKeywords.c
    ${PROJ_ROOT}/src/webcore/buildsrc/DocTypeStrings.cpp
    ${PROJ_ROOT}/src/webcore/buildsrc/tokenizer.cpp
    ${PROJ_ROOT}/src/webcore/buildsrc/JSHTMLInputElementBaseTable.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/UnicodeIDNA_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/char_break_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/line_break_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/ubidi_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/ucase_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/uchar_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/unorm_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/word_break_data.cpp
)

set(LIB_NAME agave)

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME} PRIVATE
                    ${PROJ_ROOT}/include
                    ${PROJ_ROOT}/src
                    ${PROJ_ROOT}/src/wtf
                    ${PROJ_ROOT}/src/javascript
                    ${PROJ_ROOT}/src/javascript/pcre
                    ${PROJ_ROOT}/src/javascript/kjs
                    ${PROJ_ROOT}/src/javascript/bindings
                    ${PROJ_ROOT}/src/javascript/bindings/c
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
                    ${PROJ_ROOT}/src/webcore/buildsrc
                    ${PROJ_ROOT}/src/webcore/bindings/js
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
                    ${PROJ_ROOT}/src/webview
                    ${PROJ_OUT}/include
                    ${PROJ_OUT}/
)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4838 /wd4305 /wd4291 /wd4065 /wd4146 /wd4756")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4838 /wd4305 /wd4291 /wd4065 /wd4146 /wd4756")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-misleading-indentation -Wno-narrowing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-misleading-indentation -Wno-narrowing")
endif()

target_link_libraries(${LIB_NAME} PRIVATE ${LIB_DEPS})

target_compile_definitions(${LIB_NAME} PRIVATE CURL_STATICLIB)

if (MSVC)
    target_compile_definitions(${LIB_NAME} PRIVATE _EXPORT_)
    target_link_libraries(${LIB_NAME} PRIVATE picasso2_sw)
endif()

set_target_properties(${LIB_NAME} PROPERTIES VERSION ${VERSION_INFO} SOVERSION 1)

add_dependencies(${LIB_NAME} ${LIB_DEPS} ${PICASSO_NAME})

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)


