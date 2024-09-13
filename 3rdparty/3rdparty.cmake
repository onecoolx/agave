# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

#include (${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)
#include (${CMAKE_CURRENT_LIST_DIR}/png.cmake)
#include (${CMAKE_CURRENT_LIST_DIR}/jpeg.cmake)
#include (${CMAKE_CURRENT_LIST_DIR}/mbedtls.cmake)
#include (${CMAKE_CURRENT_LIST_DIR}/curl.cmake)

if (WIN32)
include (${CMAKE_CURRENT_LIST_DIR}/picasso.cmake)
endif()
