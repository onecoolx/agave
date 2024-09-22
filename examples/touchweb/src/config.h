/* config.h - Agave application
 *
 * Copyright (C) 2024 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#define APP_VERSION "105000"  /* version 1.2.8 */
#define VERSION_STRING "1.5.0"

#define THREAD_RENDER 1

#define SETUI(x)  (x)

#ifdef M8
#define DEFAULT_WIDTH	SETUI(480)
#define DEFAULT_HEIGHT	SETUI(688)
#else
#define DEFAULT_WIDTH	SETUI(240)
#define DEFAULT_HEIGHT	SETUI(320)
#endif

#if defined(WINCE) && !defined(M8)
extern int global_scale;
#define GLOBAL_SCALE  global_scale
#elif defined(M8)
#define GLOBAL_SCALE  2
#else
#define GLOBAL_SCALE  1
#endif

#define TOOLBAR_HEIGHT	(SETUI(30)*GLOBAL_SCALE)

#define TITLE_HEIGHT	(SETUI(22)*GLOBAL_SCALE)

#define DASH_TITLE_HEIGHT	(SETUI(30)*GLOBAL_SCALE)


#define BUTTON_HEIGHT (TITLE_HEIGHT*1.4)

#ifdef M8
#define MAX_TABS		12
#else
#define MAX_TABS		5
#endif

#define MAX_DRAG_STEP 	2

#define PAGE_ITEMS 20

#if (defined(WIN32) || defined(WINCE)) && defined(_MSC_VER)
#pragma warning (disable : 4786)
#define snprintf _snprintf
#define strncasecmp _strnicmp
#include <string>
typedef unsigned int  uint32_t;
#define PATHMAX		512
typedef wchar_t uchar_t;
typedef std::wstring ustring;
#define U(text) ((uchar_t*)(L##text))
#include "files.h"
#define DATA_PATH(path) CreatePath(ustring(U(path))).c_str()
#define FILENAME_UNICODE 1
#endif

#if defined(__linux)
#include <stdint.h>
#include <wctype.h>
#include <string>
#define PATHMAX		512
#define LINUX 1
typedef unsigned short uchar_t;
typedef std::basic_string<uchar_t> ustring;
#define U(text) ((uchar_t*)(L##text))
#define DATA_PATH(path) path
#define FILENAME_UNICODE 0
#endif

#define IS_PRINT(c) iswprint(c)

#define P16(utext) (reinterpret_cast<ps_uchar16*>(utext))

//utils macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x)	(((x) < 0) ? -(x) : (x))

#define ABS_DEC(x, d) \
	do {\
		int st = ((ABS(x)-d) > 0) ? (ABS(x)-d) : 0;\
		x=(x/ABS(x)*st);\
	}while(0)


#if 0
#include <windows.h>
//debug tools
		//FILE* pf=_wfopen(L"Disk\\new_tg.log", L"a+"); 
#define debug_str_val(s, v) \
	do { \
		FILE* pf=_wfopen(L"Storage Card\\tg.log", L"a+"); \
		fprintf(pf, "%s :%d\n", s, v); \
		fclose(pf); \
	} while(0)


#define debug_wstr_val(s, v) \
	do { \
		FILE* pf=_wfopen(L"Storage Card\\tg.log", L"a+"); \
		fwprintf(pf, L"%s :%d\n", s, v); \
		fclose(pf); \
	} while(0)


#define debug_str(s) \
    do { \
        OutputDebugStringA(s); \
    }while(0)

#endif
