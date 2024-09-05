#include <wtf/Platform.h>

#define WTF_USE_PTHREADS 1
#if !COMPILER(MSVC) // can't get this to compile on Visual C++ yet
#define AVOID_STATIC_CONSTRUCTORS 1
#endif

/* types define */
#if PLATFORM(WIN32)
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;
typedef signed char     int8_t;
typedef short int       int16_t;
typedef int             int32_t;
typedef long long int   int64_t;
typedef int			        intptr_t;
typedef unsigned int        uintptr_t;
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
#undef new
#undef delete
#include <wtf/FastMalloc.h>
#endif

