# HTML5 runtime
# 
# Copyright (C) 2024 Zhang Ji Peng
# Contact: onecoolx@gmail.com

if (WIN32)
    set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /W3 /WX /Gm- /Gd /Zc:wchar_t- /MP")
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /W3 /WX /Gm- /Gd /Zc:wchar_t- /MP")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_RELEASE} /W3 /WX /Ot /GF /Gm- /Gy /GL /Zc:wchar_t- /MP")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} /W3 /WX /Ot /GF /Gm- /Gy /GL /Zc:wchar_t- /MP")
    set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_MINSIZEREL})
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_MINSIZEREL})
    set(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_MINSIZEREL})
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_MINSIZEREL})
else ()
    set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG -O0 -Wall -fPIC -g -fno-strict-aliasing -pipe")
    set(CMAKE_CXX_FLAGS_DEBUG
        "-D_DEBUG -O0 -Wall -fPIC -g -fno-strict-aliasing -pipe -std=c++98 -fno-exceptions -fno-rtti")
    set(CMAKE_C_FLAGS_MINSIZEREL "-DNDEBUG -Os -Wall -fPIC -fno-strict-aliasing -pipe")
    set(CMAKE_CXX_FLAGS_MINSIZEREL
        "-DNDEBUG -Os -Wall -fPIC -fno-strict-aliasing -pipe -std=c++98 -fno-exceptions -fno-rtti")
    set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_MINSIZEREL})
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_MINSIZEREL})
    set(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_MINSIZEREL})
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_MINSIZEREL})
endif()
