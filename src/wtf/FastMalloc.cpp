/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2005 Apple Computer, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */
#include "config.h"
#include "FastMalloc.h"
#include <stdlib.h>
#if PLATFORM(WIN32)
#include "windows.h"
#define strdup _strdup 
#endif
#include <string.h>

using namespace std;

namespace WTF {
    
void *fastMalloc(size_t n) 
{
    return malloc(n);
}

char *fastStrdup(const char *str)
{
    return strdup(str);
}

void *fastCalloc(size_t n_elements, size_t element_size)
{
    return calloc(n_elements, element_size);
}

void fastFree(void* p)
{
    free(p);
}

void *fastRealloc(void* p, size_t n)
{
    return realloc(p, n);
}

} // namespace WTF
