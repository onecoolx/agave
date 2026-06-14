# agave web browser
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(PICASSO_NAME "picasso")
set(PICASSO_VERSION "2.9.0")
set(PICASSO_PACKAGE "${PROJ_ROOT}/packages/${PICASSO_NAME}-${PICASSO_VERSION}.tar.gz")
set(PICASSO_HASH "f7df38418135f7968df7e01dae5e4737e3f91113f90b133c7026654a37043ebd")

ExternalProject_Add(
  ${PICASSO_NAME}
  PREFIX "${PROJ_OUT}/${PICASSO_NAME}"
  URL "${PICASSO_PACKAGE}"
  URL_HASH SHA256=${PICASSO_HASH}
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
   -DOPT_EXTENSIONS=OFF 
   -DOPT_DEMOS=OFF 
   -DOPT_TESTS=OFF 
   -DOPT_FAST_COPY=OFF
   -DOPT_FORMAT_ABGR=OFF
   -DOPT_FORMAT_ARGB=OFF
   -DOPT_FORMAT_RGB555=OFF
   -DOPT_FREE_TYPE2=ON   # enable the FreeType2 font backend (otherwise picasso
                         # falls back to the dummy adapter and all font metrics
                         # are zero -- see docs/picasso-2.9.0-font-regression.md)
   -DOPT_FONT_CONFIG=ON  # use the system font database via fontconfig
   -DOPT_SYSTEM_MALLOC=ON # use system malloc
   -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
)

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)
