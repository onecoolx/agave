# HTML5 runtime
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

include(ExternalProject)

set(CURL_NAME "curl")
set(CURL_LIB "lib${CURL_NAME}")
set(CURL_VERSION "8.8.0")
set(CURL_PACKAGE "${PROJ_ROOT}/packages/${CURL_NAME}-${CURL_VERSION}.tar.gz")
set(CURL_HASH "77c0e1cd35ab5b45b659645a93b46d660224d0024f1185e8a95cdb27ae3d787d")

if(WIN32)
    set(TLS_ARGS
        -DCURL_USE_SCHANNEL=ON
        -DCURL_USE_OPENSSL=OFF
        -DCURL_USE_MBEDTLS=OFF
    )
else()
    set(TLS_ARGS
        -DCURL_USE_MBEDTLS=ON
        -DMBEDTLS_USE_STATIC_LIBS=ON
        -DMBEDTLS_INCLUDE_DIRS=${PROJ_OUT}/include
    )
endif()

ExternalProject_Add(
  ${CURL_LIB}
  PREFIX "${PROJ_OUT}/${CURL_NAME}"
  URL "${CURL_PACKAGE}"
  URL_HASH SHA256=${CURL_HASH}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
    "${PROJ_ROOT}/packages/patchs/${CURL_NAME}-${CURL_VERSION}/CMakeLists.txt" "${PROJ_OUT}/${CURL_NAME}/src/${CURL_LIB}/"
  BUILD_IN_SOURCE
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    -DBUILD_CURL_EXE=OFF
    -DCURL_DISABLE_LDAP=ON
    -DCURL_DISABLE_LDAPS=ON
    -DCURL_DISABLE_DICT=ON
    -DCURL_DISABLE_MQTT=ON
    -DCURL_DISABLE_GOPHER=ON
    -DCURL_DISABLE_IMAP=ON
    -DCURL_DISABLE_POP3=ON
    -DCURL_DISABLE_RTSP=ON
    -DCURL_DISABLE_SMB=ON
    -DCURL_DISABLE_SMTP=ON
    -DCURL_DISABLE_TELNET=ON
    -DCURL_DISABLE_TFTP=ON
    -DENABLE_WEBSOCKETS=ON
    -DCMAKE_INSTALL_PREFIX=${PROJ_OUT}
    -DBUILD_SHARED_LIBS=${LIB_BUILD_SHARED_VAL}
    -DBUILD_STATIC_LIBS=${LIB_BUILD_STATIC_VAL}
    ${TLS_ARGS}
    ${LIBS_EXTRA_ARGS}
)

include_directories(${PROJ_OUT}/include)
link_directories(${PROJ_OUT}/lib)

add_dependencies(${CURL_LIB} ${ZLIB_NAME} ${MTLS_NAME})

if (OPT_EXT_LIBS_SHARED)
add_library(curl SHARED IMPORTED)
else()
add_library(curl STATIC IMPORTED)
endif()

if (OPT_EXT_LIBS_SHARED)
if(WIN32)
    set_target_properties(curl PROPERTIES
        IMPORTED_LOCATION ${PROJ_OUT}/bin/libcurl${CMAKE_SHARED_LIBRARY_SUFFIX}
        IMPORTED_IMPLIB   ${PROJ_OUT}/lib/libcurl_imp${CMAKE_IMPORT_LIBRARY_SUFFIX}
        INTERFACE_LINK_LIBRARIES "ws2_32;bcrypt;crypt32"
    )
else()
    set_target_properties(curl PROPERTIES
        IMPORTED_LOCATION ${PROJ_OUT}/lib/libcurl${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
endif()
else()
if(WIN32)
    set_target_properties(curl PROPERTIES
        IMPORTED_LOCATION ${PROJ_OUT}/lib/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX}
        INTERFACE_LINK_LIBRARIES "ws2_32;bcrypt;crypt32"
    )
else()
    set_target_properties(curl PROPERTIES
        IMPORTED_LOCATION ${PROJ_OUT}/lib/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
endif()
endif()

set(LIB_DEPS curl ${LIB_DEPS})
