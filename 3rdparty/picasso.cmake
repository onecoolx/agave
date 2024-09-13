# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(PICASSO_NAME "picasso")
set(PICASSO_VERSION "2.8.0")
set(PICASSO_PACKAGE "${PROJ_ROOT}/packages/${PICASSO_NAME}-${PICASSO_VERSION}.tar.gz")
set(PICASSO_HASH "e569bc89a138a78e089bdc3d4a25914b859ce5ba28c4f08b0ec93e52144159af")

ExternalProject_Add(
  ${PICASSO_NAME}
  PREFIX "${PROJ_OUT}/${PICASSO_NAME}"
  URL "${PICASSO_PACKAGE}"
  URL_HASH SHA256=${PICASSO_HASH}
  PATCH_COMMAND
     ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${PICASSO_NAME}-${PICASSO_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${PICASSO_NAME}/src/${PICASSO_NAME}/CMakeLists.txt"
  && ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${PICASSO_NAME}-${PICASSO_VERSION}/src.cmake" "${PROJ_OUT}/${PICASSO_NAME}/src/${PICASSO_NAME}/src/src.cmake"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DOPT_EXTENSIONS=OFF -DOPT_DEMOS=OFF -DOPT_TESTS=OFF -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)
