# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(QJS_NAME "quickjs")
set(QJS_VERSION "2024-01-13")
set(QJS_PACKAGE "${PROJ_ROOT}/packages/${QJS_NAME}-${QJS_VERSION}.tar.xz")
set(QJS_HASH "3c4bf8f895bfa54beb486c8d1218112771ecfc5ac3be1036851ef41568212e03")

if(WIN32)
ExternalProject_Add(
  ${QJS_NAME}
  PREFIX "${PROJ_OUT}/${QJS_NAME}"
  URL "${QJS_PACKAGE}"
  URL_HASH SHA256=${QJS_HASH}
  PATCH_COMMAND
     ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/CMakeLists.txt"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/cutils.h" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/cutils.h"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/dirent.h" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/dirent.h"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/getopt.h" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/getopt.h"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/cutils.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/cutils.c"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/libregexp.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/libregexp.c"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/qjs.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/qjs.c"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/qjsc.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/qjsc.c"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/quickjs.h" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/quickjs.h"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/quickjs.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/quickjs.c"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/quickjs-libc.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/quickjs-libc.c"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)
else()
ExternalProject_Add(
  ${QJS_NAME}
  PREFIX "${PROJ_OUT}/${QJS_NAME}"
  URL "${QJS_PACKAGE}"
  URL_HASH SHA256=${QJS_HASH}
  PATCH_COMMAND
     ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/CMakeLists.txt"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/quickjs.h" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/quickjs.h"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${QJS_NAME}-${QJS_VERSION}/quickjs_posix.c" "${PROJ_OUT}/${QJS_NAME}/src/${QJS_NAME}/quickjs.c"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)
endif()

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)
