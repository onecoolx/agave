#include <wtf/Platform.h>

#define WTF_USE_PTHREADS 1
#if !COMPILER(MSVC) // can't get this to compile on Visual C++ yet
#define AVOID_STATIC_CONSTRUCTORS 1
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <wtf/FastMalloc.h>
#endif

