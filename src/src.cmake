# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

file(GLOB_RECURSE SOURCES ${PROJ_ROOT}/src/webcore/css/*.cpp
                          ${PROJ_ROOT}/src/webcore/dom/*.cpp
                          ${PROJ_ROOT}/src/webcore/editing/*.cpp
                          ${PROJ_ROOT}/src/webcore/history/*.cpp
                          ${PROJ_ROOT}/src/webcore/html/*.cpp
                          ${PROJ_ROOT}/src/webcore/ksvg2/*.cpp
                          ${PROJ_ROOT}/src/webcore/loader/*.cpp
                          ${PROJ_ROOT}/src/webcore/page/*.cpp
                          ${PROJ_ROOT}/src/webcore/platform/*.cpp
                          ${PROJ_ROOT}/src/webcore/rendering/*.cpp
                          ${PROJ_ROOT}/src/webcore/xml/*.cpp
                          ${PROJ_ROOT}/src/webview/*.cpp
)

list(REMOVE_ITEM SOURCES 
    ${PROJ_ROOT}/src/webcore/platform/unicode/UnicodeIDNA_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/char_break_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/line_break_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/ubidi_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/ucase_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/uchar_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/unorm_props_data.cpp
    ${PROJ_ROOT}/src/webcore/platform/unicode/word_break_data.cpp
    ${PROJ_ROOT}/src/webcore/page/InspectorController.cpp
)

if (OPT_USE_KJS)
file(GLOB_RECURSE KJS_SOURCES ${PROJ_ROOT}/src/javascript/*.cpp
                              ${PROJ_ROOT}/src/javascript/*.c
                              ${PROJ_ROOT}/src/webcore/buildKJS/*.cpp
                              ${PROJ_ROOT}/src/webcore/buildKJS/*.c
                              ${PROJ_ROOT}/src/webcore/bindings/js/*.cpp
                              ${PROJ_ROOT}/src/wtf/*.cpp
)

list(REMOVE_ITEM KJS_SOURCES 
    ${PROJ_ROOT}/src/javascript/pcre/chartables.c
    ${PROJ_ROOT}/src/javascript/pcre/ucptable.c
    ${PROJ_ROOT}/src/javascript/pcre/dftables.c
    ${PROJ_ROOT}/src/webcore/buildKJS/CSSPropertyNames.c
    ${PROJ_ROOT}/src/webcore/buildKJS/CSSValueKeywords.c
    ${PROJ_ROOT}/src/webcore/buildKJS/DocTypeStrings.cpp
    ${PROJ_ROOT}/src/webcore/buildKJS/tokenizer.cpp
    ${PROJ_ROOT}/src/webcore/buildKJS/JSHTMLInputElementBaseTable.cpp
)

set(SOURCES ${SOURCES} ${KJS_SOURCES})
elseif (OPT_USE_QJS)
file(GLOB_RECURSE QJS_SOURCES ${PROJ_ROOT}/src/webcore/buildQJS/*.cpp
                              ${PROJ_ROOT}/src/webcore/buildQJS/*.c
                              ${PROJ_ROOT}/src/webcore/bindings/qjs/*.cpp
                              ${PROJ_ROOT}/src/wtf/*.cpp
)

list(REMOVE_ITEM QJS_SOURCES 
    ${PROJ_ROOT}/src/webcore/buildQJS/CSSPropertyNames.c
    ${PROJ_ROOT}/src/webcore/buildQJS/CSSValueKeywords.c
    ${PROJ_ROOT}/src/webcore/buildQJS/DocTypeStrings.cpp
    ${PROJ_ROOT}/src/webcore/buildQJS/tokenizer.cpp
)

set(SOURCES ${SOURCES} ${QJS_SOURCES})
endif()

set(LIB_NAME agave)

add_library(${LIB_NAME} ${SOURCES})

set(HEADERS ${PROJ_ROOT}/include
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
            ${PROJ_ROOT}/src/webview
            ${PROJ_OUT}/include
            ${PROJ_OUT}/
)

if (OPT_USE_KJS)
set(KJS_HEADERS ${PROJ_ROOT}/src/wtf
                ${PROJ_ROOT}/src/javascript
                ${PROJ_ROOT}/src/javascript/pcre
                ${PROJ_ROOT}/src/javascript/kjs
                ${PROJ_ROOT}/src/javascript/bindings
                ${PROJ_ROOT}/src/javascript/bindings/c
                ${PROJ_ROOT}/src/webcore/buildKJS
                ${PROJ_ROOT}/src/webcore/bindings/js
)

set(HEADERS ${HEADERS} ${KJS_HEADERS})
elseif (OPT_USE_QJS)
set(QJS_HEADERS ${PROJ_ROOT}/src/wtf
                ${PROJ_ROOT}/src/webcore/buildQJS
                ${PROJ_ROOT}/src/webcore/bindings/qjs
                ${PROJ_ROOT}/src/javascript # tmp include
)

set(HEADERS ${HEADERS} ${QJS_HEADERS})
endif()

target_include_directories(${LIB_NAME} PRIVATE ${HEADERS})

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


