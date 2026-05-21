/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "global.h"

#include <wtf/FastMalloc.h>

#include <assert.h>

#if defined(__linux__) || defined(__GLIBC__)
#include <malloc.h>
#define HAVE_MALLOC_USABLE_SIZE 1
#elif defined(__NuttX__)
#include <malloc.h>
#define HAVE_MALLOC_USABLE_SIZE 1
#endif

#if !defined(__APPLE__) && !defined(_WIN32) && !defined(HAVE_MALLOC_USABLE_SIZE)
#error "No malloc_usable_size available - QuickJS memory tracking will be inaccurate"
#endif

namespace mescal {

/*
 * Custom memory allocator for QuickJS using WTF::fastMalloc.
 * Follows the same pattern as QuickJS's trace_malloc in qjs.c.
 */

#if defined(__APPLE__)
#define MALLOC_OVERHEAD  0
#else
#define MALLOC_OVERHEAD  8
#endif

static inline size_t qjs_malloc_usable_size(const void *ptr)
{
#if defined(__APPLE__)
    return malloc_size(ptr);
#elif defined(_WIN32)
    return _msize((void *)ptr);
#elif defined(HAVE_MALLOC_USABLE_SIZE)
    return malloc_usable_size(const_cast<void*>(ptr));
#else
    return 0;
#endif
}

static void* qjs_malloc(JSMallocState *s, size_t size)
{
    void *ptr;

    assert(size != 0);

    if (s->malloc_size + size > s->malloc_limit)
        return NULL;

    ptr = fastMalloc(size);
    if (ptr) {
        s->malloc_count++;
        s->malloc_size += qjs_malloc_usable_size(ptr) + MALLOC_OVERHEAD;
    }
    return ptr;
}

static void qjs_free(JSMallocState *s, void *ptr)
{
    if (!ptr)
        return;

    s->malloc_count--;
    s->malloc_size -= qjs_malloc_usable_size(ptr) + MALLOC_OVERHEAD;
    fastFree(ptr);
}

static void* qjs_realloc(JSMallocState *s, void *ptr, size_t size)
{
    size_t old_size;

    if (!ptr) {
        if (size == 0)
            return NULL;
        return qjs_malloc(s, size);
    }

    old_size = qjs_malloc_usable_size(ptr);

    if (size == 0) {
        s->malloc_count--;
        s->malloc_size -= old_size + MALLOC_OVERHEAD;
        fastFree(ptr);
        return NULL;
    }

    if (s->malloc_size + size - old_size > s->malloc_limit)
        return NULL;

    ptr = fastRealloc(ptr, size);
    if (ptr) {
        s->malloc_size += qjs_malloc_usable_size(ptr) - old_size;
    }
    return ptr;
}

static const JSMallocFunctions qjs_malloc_funcs = {
    qjs_malloc,
    qjs_free,
    qjs_realloc,
    qjs_malloc_usable_size,
};

static GlobalData _globalData;

const GlobalData* const _global(void)
{
    return &_globalData;
}

bool _global_initialize(void)
{
    _globalData.runtime = JS_NewRuntime2(&qjs_malloc_funcs, NULL);
    _globalData.utilContext = JS_NewContext(_globalData.runtime);
    _globalData.domObjects = new DOMObjectMap;
    _globalData.domNodesPerDoc = new NodePerDocMap;
    _globalData.jsValWindows = new JSValueWindowMap;
    return true;
}

void _global_shutdown(void)
{
    JSRuntime* rt = _globalData.runtime;

    // Release all cached JSValues before freeing runtime
    if (_globalData.domObjects) {
        DOMObjectMap::iterator it = _globalData.domObjects->begin();
        DOMObjectMap::iterator end = _globalData.domObjects->end();
        for (; it != end; ++it) {
            if (JS_VALUE_GET_TAG(it->second) == JS_TAG_OBJECT)
                JS_FreeValueRT(rt, it->second);
        }
        delete _globalData.domObjects;
        _globalData.domObjects = 0;
    }

    if (_globalData.domNodesPerDoc) {
        NodePerDocMap::iterator it = _globalData.domNodesPerDoc->begin();
        NodePerDocMap::iterator end = _globalData.domNodesPerDoc->end();
        for (; it != end; ++it) {
            NodeMap* nodeMap = it->second;
            if (nodeMap) {
                NodeMap::iterator nit = nodeMap->begin();
                NodeMap::iterator nend = nodeMap->end();
                for (; nit != nend; ++nit) {
                    if (JS_VALUE_GET_TAG(nit->second) == JS_TAG_OBJECT)
                        JS_FreeValueRT(rt, nit->second);
                }
                delete nodeMap;
            }
        }
        delete _globalData.domNodesPerDoc;
        _globalData.domNodesPerDoc = 0;
    }

    if (_globalData.jsValWindows) {
        delete _globalData.jsValWindows;
        _globalData.jsValWindows = 0;
    }

    JS_FreeContext(_globalData.utilContext);
    // Skip JS_FreeRuntime to avoid GC assert on exit.
    // OS will reclaim all memory. This matches browser behavior
    // where process exit is the cleanup mechanism.
}

}
