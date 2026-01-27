cmake_minimum_required(VERSION 3.16)

project(Agave LANGUAGES C CXX ASM HOMEPAGE_URL https://github.com/onecoolx/agave)

set (CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS ON) # build shared library, OFF for static

set(PROJ_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(PROJ_OUT ${CMAKE_CURRENT_BINARY_DIR})

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_MICRO 0)
set(VERSION_INFO ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO})

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if(POLICY CMP0114)
  cmake_policy(SET CMP0114 NEW)
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" FORCE)
endif()


include (${CMAKE_CURRENT_LIST_DIR}/build/configs.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/build/defines.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/3rdparty/3rdparty.cmake)

OPTION_MACRO_DEFINE(OPT_PNG_SUPPORT PNG_SUPPORT "Build support PNG image format." ON)
OPTION_MACRO_DEFINE(OPT_JPEG_SUPPORT JPEG_SUPPORT "Build support JPEG image format." ON)
OPTION_MACRO_DEFINE(OPT_GIF_SUPPORT GIF_SUPPORT "Build support GIF image format." ON)
OPTION_MACRO_DEFINE(OPT_FILESYSTEM ENABLE_FILESYSTEM "Build support file system." ON)
OPTION_MACRO_DEFINE(OPT_FTPDIR ENABLE_FTPDIR "Build support list ftp dir." ON)
OPTION_MACRO_DEFINE(OPT_COOKIES ENABLE_COOKIES "Build cookie support." ON)
OPTION_MACRO_DEFINE(OPT_FILECOOKIE ENABLE_FILECOOKIE "Build file support with cookies." ON)
OPTION_MACRO_DEFINE(OPT_HTTPCACHE ENABLE_HTTPCACHE "Build http disk cache support." ON)
OPTION_MACRO_DEFINE(OPT_SSL ENABLE_SSL "Build with SSL support." ON)
OPTION_MACRO_DEFINE(OPT_AJAX ENABLE_AJAX "Build support ajax." ON)
OPTION_MACRO_DEFINE(OPT_SSLFILE ENABLE_SSLFILE "Build with file support with SSL." ON)
OPTION_MACRO_DEFINE(OPT_XML ENABLE_XML "Build XML support." ON)
OPTION_MACRO_DEFINE(OPT_XSLT ENABLE_XSLT "Build XSLT support." ON)
OPTION_MACRO_DEFINE(OPT_XPATH ENABLE_XPATH "Build XPATH support." ON)
OPTION_MACRO_DEFINE(OPT_FOCUSRING FOCUSRING_DRAWING "Build support focus ring of owner drawing style." OFF)
OPTION_MACRO_DEFINE(OPT_BIG5_SUPPORT ENABLE_BIG5_SUPPORT "Build support charset BIG5." ON)
OPTION_MACRO_DEFINE(OPT_GBK_SUPPORT ENABLE_GBK_SUPPORT "Build support charset GBK." ON)
OPTION_MACRO_DEFINE(OPT_GB2312_SUPPORT ENABLE_GB2312_SUPPORT "Build support charset GB2312." ON)
OPTION_MACRO_DEFINE(OPT_EUCJP_SUPPORT ENABLE_EUCJP_SUPPORT "Build support charset EUCJP." ON)
OPTION_MACRO_DEFINE(OPT_EUCKR_SUPPORT ENABLE_EUCKR_SUPPORT "Build support charset EUCKR." ON)
OPTION_MACRO_DEFINE(OPT_SHIFTJIS_SUPPORT ENABLE_SHIFTJIS_SUPPORT "Build support charset SHIFTJIS." ON)

OPTION_MACRO_DEFINE(OPT_USE_KJS ENABLE_KJS "Build with KJS JavaScript engine." ON)
OPTION_MACRO_DEFINE(OPT_USE_QJS ENABLE_QJS "Build with QuickJS JavaScript engine." OFF)

include (${CMAKE_CURRENT_LIST_DIR}/src/src.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/test/test.cmake)

option(OPT_UNITTEST "Build unit tests" OFF)
if (OPT_UNITTEST)
enable_testing()
include (${CMAKE_CURRENT_LIST_DIR}/unit_tests/unit_tests.cmake)
endif()

option(OPT_EXAMPLES "Build examples" OFF)
if (OPT_EXAMPLES)
include (${CMAKE_CURRENT_LIST_DIR}/examples/examples.cmake)
endif()

include(CheckIncludeFile)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stddef HAVE_STDDEF_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(errno.h HAVE_ERRNO_H)
check_include_file(float.h HAVE_FLOAT_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)

include(CheckFunctionExists)
check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strncasecmp HAVE_STRNCASECMP)
check_function_exists(ftime HAVE_FTIME)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(munmap HAVE_MUNMAP)

configure_file(
    "${PROJ_ROOT}/build/mconfig.h.in"
    "${PROJ_OUT}/mconfig.h"
)
