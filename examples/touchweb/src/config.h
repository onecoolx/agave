/* config.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#define APP_VERSION "105000"  /* version 1.5.0 */
#define VERSION_STRING "1.5.0"

#define MOBILE_PHONE 0

#define THREAD_RENDER 1

#define SETUI(x)  (x)

#ifdef MOBILE_PHONE
    #define DEFAULT_WIDTH   SETUI(480)
    #define DEFAULT_HEIGHT  SETUI(688)
#else
    #define DEFAULT_WIDTH   SETUI(480)
    #define DEFAULT_HEIGHT  SETUI(480)
#endif

#if defined(MOBILE_PHONE)
    #define GLOBAL_SCALE  1
#else
    #define GLOBAL_SCALE  1
#endif

#define TOOLBAR_HEIGHT  (SETUI(48)*GLOBAL_SCALE)

#define TITLE_HEIGHT    (SETUI(36)*GLOBAL_SCALE)

#define DASH_TITLE_HEIGHT   (SETUI(48)*GLOBAL_SCALE)

#define BUTTON_HEIGHT (TITLE_HEIGHT*1.4)

#ifdef MOBILE_PHONE
    #define MAX_TABS        12
#else
    #define MAX_TABS        5
#endif

#define MAX_DRAG_STEP   2

#define PAGE_ITEMS 20

#if defined(WIN32) && defined(_MSC_VER)
    #pragma warning (disable : 4786)
    #define strncasecmp _strnicmp
    #include <string>
    typedef unsigned int  uint32_t;
    #define PATHMAX     512
    typedef wchar_t uchar_t;
    typedef std::basic_string<uchar_t> ustring;
    #define U(text) ((uchar_t*)(L##text))
    #include "files.h"
    #define DATA_PATH(path) CreatePath(ustring(U(path))).c_str()
    #define FILENAME_UNICODE 1
#endif

#if defined(__linux)
    #include <stdint.h>
    #include <wctype.h>
    #include <string>
    #define PATHMAX     512
    #define LINUX 1
    typedef unsigned short uchar_t;
    typedef std::basic_string<uchar_t> ustring;
    #define U(text) ((uchar_t*)(u##text))
    #define DATA_PATH(path) path
    #define FILENAME_UNICODE 0
#endif

#define IS_PRINT(c) iswprint(c)

#define P16(utext) (reinterpret_cast<ps_uchar16*>(utext))

//utils macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x)  (((x) < 0) ? -(x) : (x))

#define ABS_DEC(x, d) \
    do {\
        int st = ((ABS(x)-d) > 0) ? (ABS(x)-d) : 0;\
        x=(x/ABS(x)*st);\
    }while(0)

#include <stdio.h>
