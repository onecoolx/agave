cmake_minimum_required(VERSION 3.16.0)

project(sqlite LANGUAGES C)

if(MSVC)
    add_compile_options("/utf-8")
    add_compile_options("/MP")
endif(MSVC)

set(SQLITE_DIR ${CMAKE_CURRENT_LIST_DIR}/)

set(LIB_SQLITE sqlite3)

add_library(${LIB_SQLITE} STATIC ${SQLITE_DIR}/sqlite3.c)
set_property(TARGET ${LIB_SQLITE} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${LIB_SQLITE} PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)

# Compile-time options: a thread-safe, embeddable build with commonly used
# features and the deprecated/unused surface trimmed to keep the footprint small.
target_compile_definitions(${LIB_SQLITE} PRIVATE
    SQLITE_THREADSAFE=1
    SQLITE_ENABLE_FTS5
    SQLITE_ENABLE_JSON1
    SQLITE_OMIT_LOAD_EXTENSION
    SQLITE_DEFAULT_FOREIGN_KEYS=1
)

if(NOT WIN32)
    find_package(Threads)
    target_link_libraries(${LIB_SQLITE} PUBLIC Threads::Threads ${CMAKE_DL_LIBS})
endif()

set(SQLITE_HEADERS
    ${SQLITE_DIR}/sqlite3.h
    ${SQLITE_DIR}/sqlite3ext.h
)

install(TARGETS ${LIB_SQLITE} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES ${SQLITE_HEADERS} DESTINATION include/)
