# HTML5 runtime
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(XSLT_NAME "libxslt")
set(XSLT_VERSION "1.1.42")
set(XSLT_PACKAGE "${PROJ_ROOT}/packages/${XSLT_NAME}-${XSLT_VERSION}.tar.gz")
set(XSLT_HASH "b950e8c873069eb570dbc5828eed5a522fc2486edc0cc1dc01d3a360b63a8a62")

ExternalProject_Add(
  ${XSLT_NAME}
  PREFIX "${PROJ_OUT}/${XSLT_NAME}"
  URL "${XSLT_PACKAGE}"
  URL_HASH SHA256=${XSLT_HASH}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${XSLT_NAME}-${XSLT_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${XSLT_NAME}/src/${XSLT_NAME}/"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_SHARED_LIBS=OFF -DLIBXSLT_WITH_DEBUGGER=OFF -DLIBXSLT_WITH_PYTHON=OFF -DLIBXSLT_WITH_PROGRAMS=OFF -DLIBXSLT_WITH_TESTS=OFF -DLIBXSLT_WITH_XSLT_DEBUG=OFF -DCMAKE_INSTALL_PREFIX=${PROJ_OUT} -DLIBXML2_INCLUDE_DIRS=${PROJ_OUT}/include/libxml2
)
include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)

add_dependencies(${XSLT_NAME} ${XML2_NAME})

set(LIB_DEPS ${LIB_DEPS} xslt exslt)
