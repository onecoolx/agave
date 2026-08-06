# agave web browser
# 
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com

if(WIN32)
set(LIBS_EXTRA_ARGS
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${PROJ_OUT}
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${PROJ_OUT}/lib
)
else()
set(LIBS_EXTRA_ARGS "")
endif()

if (OPT_EXT_LIBS_SHARED)
    set(LIB_BUILD_SHARED_VAL ON)
    set(LIB_BUILD_STATIC_VAL OFF)
else()
    set(LIB_BUILD_SHARED_VAL OFF)
    set(LIB_BUILD_STATIC_VAL ON)
endif()

include (${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/png.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/jpeg.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/mbedtls.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/curl.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/xml2.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/xslt.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/picasso.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/quickjs.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/sqlite.cmake)
