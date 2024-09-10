# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

file(GLOB_RECURSE SOURCES ${PROJ_ROOT}/dist/src/*.c)

include_directories(${PROJ_ROOT}/dist ${PROJ_ROOT}/dist/include)

set(LIB_NAME dist)

add_library(${LIB_NAME} ${SOURCES})

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

include_directories(${PROJ_ROOT}/dist/include)

