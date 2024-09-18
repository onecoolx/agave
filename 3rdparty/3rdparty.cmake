# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

if (NOT OPT_USE_INTERNAL_LIBS)
include (${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/png.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/jpeg.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/mbedtls.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/curl.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/xml2.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/xslt.cmake)
endif()

include (${CMAKE_CURRENT_LIST_DIR}/picasso.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/quickjs.cmake)
