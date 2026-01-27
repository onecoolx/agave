# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(PNG_NAME "libpng")
set(PNG_VERSION "1.6.43")
set(PNG_PACKAGE "${PROJ_ROOT}/packages/${PNG_NAME}-${PNG_VERSION}.tar.gz")
set(PNG_HASH "e804e465d4b109b5ad285a8fb71f0dd3f74f0068f91ce3cdfde618180c174925")

ExternalProject_Add(
  ${PNG_NAME}
  PREFIX "${PROJ_OUT}/${PNG_NAME}"
  URL "${PNG_PACKAGE}"
  URL_HASH SHA256=${PNG_HASH}
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DPNG_SHARED=OFF -DPNG_STATIC=ON -DPNG_TESTS=OFF -DPNG_TOOLS=OFF -DZLIB_ROOT=${PROJ_OUT}/${ZLIB_NAME}/src/${ZLIB_NAME}-build -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)
include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)

add_dependencies(${PNG_NAME} ${ZLIB_NAME})

add_library(png STATIC IMPORTED)
set_target_properties(png PROPERTIES
  IMPORTED_LOCATION ${PROJ_OUT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}png${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(LIB_DEPS ${LIB_DEPS} png)
