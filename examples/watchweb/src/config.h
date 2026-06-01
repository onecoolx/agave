/* config.h - WatchWeb (circular smartwatch browser)
 *
 * Copyright (C) 2026 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#define APP_VERSION "100000"
#define VERSION_STRING "1.0.0"

#define MOBILE_PHONE 0

#define THREAD_RENDER 0

#define SETUI(x)  (x)

/* 480x480 circular display */
#define DEFAULT_WIDTH   480
#define DEFAULT_HEIGHT  480

#define GLOBAL_SCALE  1

#define TOOLBAR_HEIGHT  (SETUI(44)*GLOBAL_SCALE)
#define TITLE_HEIGHT    (SETUI(32)*GLOBAL_SCALE)
#define DASH_TITLE_HEIGHT   (SETUI(44)*GLOBAL_SCALE)
#define BUTTON_HEIGHT (TITLE_HEIGHT*1.4)

#define MAX_TABS        3
#define MAX_DRAG_STEP   2
#define PAGE_ITEMS      10

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#ifdef __cplusplus
#include <string>
#endif

#define PATHMAX     512
#define LINUX 1
typedef unsigned short uchar_t;
#ifdef __cplusplus
typedef std::basic_string<uchar_t> ustring;
#endif
#define U(text) ((uchar_t*)(u##text))
#define DATA_PATH(path) path
#define FILENAME_UNICODE 0

#define IS_PRINT(c) iswprint(c)
#define P16(utext) (reinterpret_cast<ps_uchar16*>(utext))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x)  (((x) < 0) ? -(x) : (x))

#include <stdio.h>

/* Disable sqlite encryption (not needed on watch) */
#define sqlite3_key(db, key, len) 0
#define sqlite3_rekey(db, key, len) 0
