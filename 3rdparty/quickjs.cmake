# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(QJS_NAME "quickjs")
set(QJS_VERSION "2024-01-13")
set(QJS_PACKAGE "${PROJ_ROOT}/packages/${QJS_NAME}-${QJS_VERSION}.tar.xz")
set(QJS_HASH "3c4bf8f895bfa54beb486c8d1218112771ecfc5ac3be1036851ef41568212e03")

ExternalProject_Add(
  ${QJS_NAME}
  PREFIX "${PROJ_OUT}/${QJS_NAME}"
  URL "${QJS_PACKAGE}"
  URL_HASH SHA256=${QJS_HASH}
  PATCH_COMMAND
     ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/CMakeLists.txt"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)
