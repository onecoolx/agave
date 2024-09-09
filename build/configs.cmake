# HTML5 runtime
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

if (WIN32)
    set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /W3 /WX /Gm- /Gd /Zc:wchar_t- /MP")
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /W3 /WX /Gm- /Gd /Zc:wchar_t- /MP")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /W3 /WX /Ot /GF /Gm- /Gy /GL /Zc:wchar_t- /MP")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /WX /Ot /GF /Gm- /Gy /GL /Zc:wchar_t- /MP")
elseif ()
    set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG -O0 -Wall -g -fno-strict-aliasing -pipe")
    set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -Wall -g -fno-strict-aliasing -pipe -std=c++11 -fno-exceptions -fno-rtti")
    set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Os -Wall -fno-strict-aliasing -pipe")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Os -Wall -fno-strict-aliasing -pipe -std=c++11 -fno-exceptions -fno-rtti")
endif()
