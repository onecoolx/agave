/*
 * Summary: Windows configuration header
 * Description: Windows configuration header
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Igor Zlatkovic
 */
#ifndef __LIBEXSLT_WIN32_CONFIG__
#define __LIBEXSLT_WIN32_CONFIG__

#include "build.h"

#define HAVE_CTYPE_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#define HAVE_MALLOC_H 1
#define HAVE_TIME_H 1
#define HAVE_LOCALTIME 1
#define HAVE_GMTIME 1
#define HAVE_TIME 1
#define HAVE_MATH_H 1
#define HAVE_FCNTL_H 1

#ifndef _WIN32_WCE
#include <io.h>
#endif

#define HAVE_ISINF
#define HAVE_ISNAN

#include <math.h>
#if defined _MSC_VER || defined __MINGW32__
/* MS C-runtime has functions which can be used in order to determine if
   a given floating-point variable contains NaN, (+-)INF. These are 
   preferred, because floating-point technology is considered propriatary
   by MS and we can assume that their functions know more about their 
   oddities than we do. */
#include <float.h>
/* Bjorn Reese figured a quite nice construct for isinf() using the 
   _fpclass() function. */
#ifndef isinf
#define isinf(d) ((_fpclass(d) == _FPCLASS_PINF) ? 1 \
	: ((_fpclass(d) == _FPCLASS_NINF) ? -1 : 0))
#endif
/* _isnan(x) returns nonzero if (x == NaN) and zero otherwise. */
#ifndef isnan
#define isnan(d) (_isnan(d))
#endif
#else /* _MSC_VER */
static int isinf (double d) {
    int expon = 0;
    double val = frexp (d, &expon);
    if (expon == 1025) {
        if (val == 0.5) {
            return 1;
        } else if (val == -0.5) {
            return -1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
static int isnan (double d) {
    int expon = 0;
    double val = frexp (d, &expon);
    if (expon == 1025) {
        if (val == 0.5) {
            return 0;
        } else if (val == -0.5) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
#endif /* _MSC_VER */

#ifndef _WIN32_WCE
#include <direct.h>
#endif
#if defined(_MSC_VER) || defined(__MINGW32__)
#define mkdir(p,m) _mkdir(p)
#endif

#ifndef _WIN32_WCE
#define HAVE_SYS_STAT_H
#define HAVE__STAT
#endif
#define HAVE_STRING_H

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED
#endif

#endif /* __LIBXSLT_WIN32_CONFIG__ */

