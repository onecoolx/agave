# agave web browser
# 
# Copyright (C) 2026 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(SQLITE_NAME "sqlite")
set(SQLITE_VERSION "3.46.0")
set(SQLITE_PACKAGE "${PROJ_ROOT}/packages/${SQLITE_NAME}-${SQLITE_VERSION}.tar.gz")
set(SQLITE_HASH "0bd4a66fa7fb9a730ec79e3430660e2bb2cc6c7f675d4a328720a9e20812c38f")

# The SQLite amalgamation ships only sqlite3.c/.h with no build system, so we
# drop in our own CMakeLists.txt during the patch step (same approach as quickjs).
ExternalProject_Add(
  ${SQLITE_NAME}
  PREFIX "${PROJ_OUT}/${SQLITE_NAME}"
  URL "${SQLITE_PACKAGE}"
  URL_HASH SHA256=${SQLITE_HASH}
  PATCH_COMMAND
     ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${SQLITE_NAME}-${SQLITE_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${SQLITE_NAME}/src/${SQLITE_NAME}/CMakeLists.txt"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=${PROJ_OUT} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
)

add_library(sqlite3 STATIC IMPORTED)
set_target_properties(sqlite3 PROPERTIES
  IMPORTED_LOCATION ${PROJ_OUT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqlite3${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(LIB_DEPS ${LIB_DEPS} sqlite3)
