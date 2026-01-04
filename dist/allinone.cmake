# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

file(GLOB_RECURSE SOURCES ${PROJ_ROOT}/dist/src/*.c)

include_directories(${PROJ_ROOT}/dist ${PROJ_ROOT}/dist/include)

set(LIB_DEPS dist)

add_library(${LIB_DEPS} STATIC ${SOURCES})

if (MSVC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267 /wd4244 /wd4311 /wd4312 /wd4996")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267 /wd4244 /wd4311 /wd4312 /wd4996")
else()
target_compile_options(${LIB_DEPS} PRIVATE -Wno-incompatible-function-pointer-types)
endif()

install(TARGETS ${LIB_DEPS} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

