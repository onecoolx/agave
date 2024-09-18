cmake_minimum_required(VERSION 3.16.0)

project(quickjs LANGUAGES C)

if(MSVC)
    add_compile_options("/utf-8")
    add_compile_options("/MP")
endif(MSVC)

option(BUILD_EXAMPLES "Build examples" OFF)

set(QJS_DIR ${CMAKE_CURRENT_LIST_DIR}/)

set(LIB_QUICKJS quickjs)

set(QJS_SOURCES
    ${QJS_DIR}/quickjs.c
    ${QJS_DIR}/libregexp.c
    ${QJS_DIR}/libunicode.c
    ${QJS_DIR}/cutils.c
    ${QJS_DIR}/quickjs-libc.c
    ${QJS_DIR}/libbf.c
)

set(QJS_HEADERS
    ${QJS_DIR}/quickjs.h
)

add_library(${LIB_QUICKJS} STATIC ${QJS_SOURCES})

set(LIB_VERSION 2024-01-13)

target_compile_definitions(${LIB_QUICKJS} PRIVATE
    _GNU_SOURCE
    CONFIG_VERSION="${LIB_VERSION}"
    CONFIG_BIGNUM
)

set_property(TARGET ${LIB_QUICKJS} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

install(TARGETS ${LIB_QUICKJS} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

install(FILES ${QJS_HEADERS} DESTINATION include/)

include_directories(${QJS_DIR})

find_package(Threads)

set(QUICKJS_COMPILER qjsc)
add_executable(${QUICKJS_COMPILER} qjsc.c)
target_compile_definitions(${QUICKJS_COMPILER} PRIVATE
    _GNU_SOURCE
    CONFIG_VERSION="${LIB_VERSION}"
    CONFIG_BIGNUM
)
target_link_libraries(${QUICKJS_COMPILER} PRIVATE ${LIB_QUICKJS} PUBLIC m Threads::Threads ${CMAKE_DL_LIBS})
install(TARGETS ${QUICKJS_COMPILER} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

set(REPLSRCS repl.c)

add_custom_command(
    OUTPUT ${REPLSRCS}
    COMMAND ${QUICKJS_COMPILER} -c -o ${REPLSRCS} -m ${QJS_DIR}/repl.js
    DEPENDS ${QUICKJS_COMPILER}
)

set(SCALESRCS qjscalc.c)
add_custom_command(
    OUTPUT ${SCALESRCS}
    COMMAND ${QUICKJS_COMPILER} -fbignum -c -o ${SCALESRCS} ${QJS_DIR}/qjscalc.js
    DEPENDS ${QUICKJS_COMPILER}
)

set(QUICKJS_EXE qjs)
add_executable(${QUICKJS_EXE} qjs.c ${REPLSRCS} ${SCALESRCS})
target_compile_definitions(${QUICKJS_EXE} PRIVATE
    _GNU_SOURCE
    CONFIG_VERSION="${LIB_VERSION}"
    CONFIG_BIGNUM
)
target_link_libraries(${QUICKJS_EXE} PRIVATE ${LIB_QUICKJS} PUBLIC m Threads::Threads ${CMAKE_DL_LIBS})
install(TARGETS ${QUICKJS_EXE} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

if(BUILD_EXAMPLES)
add_library(point SHARED examples/point.c)
target_link_libraries(point PRIVATE ${LIB_QUICKJS})
set_target_properties(point PROPERTIES PREFIX "")
endif()
