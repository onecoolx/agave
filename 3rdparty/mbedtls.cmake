# HTML5 runtime
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(MTLS_NAME "mbedtls")
set(MTLS_VERSION "3.6.0")
set(MTLS_PACKAGE "${PROJ_ROOT}/packages/${MTLS_NAME}-${MTLS_VERSION}.tar.bz2")
set(MTLS_HASH "3ecf94fcfdaacafb757786a01b7538a61750ebd85c4b024f56ff8ba1490fcd38")

ExternalProject_Add(
  ${MTLS_NAME}
  PREFIX "${PROJ_OUT}/${MTLS_NAME}"
  URL "${MTLS_PACKAGE}"
  URL_HASH SHA256=${MTLS_HASH}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${MTLS_NAME}-${MTLS_VERSION}/library/CMakeLists.txt" "${PROJ_OUT}/${MTLS_NAME}/src/${MTLS_NAME}/library/"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DENABLE_PROGRAMS=OFF -DENABLE_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)

set(LIB_DEPS ${LIB_DEPS} mbedtls-static mbedcrypto p256m mbedx509 everest)
