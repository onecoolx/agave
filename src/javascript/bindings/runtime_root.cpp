/*
 * Copyright (C) 2004 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */
#include "config.h"

#if USE(NPOBJECT)
#include "runtime_root.h"

#include "object.h"
#include "runtime.h"
#include "runtime_object.h"

#include <wtf/HashCountedSet.h>
#include <wtf/HashSet.h>

namespace KJS { namespace Bindings {

// This code attempts to solve two problems: (1) plug-ins leaking references to 
// JS and the DOM; (2) plug-ins holding stale references to JS and the DOM. Previous 
// comments in this file claimed that problem #1 was an issue in Java, in particular, 
// because Java, allegedly, didn't always call finalize when collecting an object.

typedef HashSet<RootObject*> RootObjectSet;

static RootObjectSet* rootObjectSet()
{
    static RootObjectSet staticRootObjectSet;
    return &staticRootObjectSet;
}

// FIXME:  These two functions are a potential performance problem.  We could 
// fix them by adding a JSObject to RootObject dictionary.

RootObject* findRootObject(JSObject* jsObject)
{
    RootObjectSet::const_iterator end = rootObjectSet()->end();
    for (RootObjectSet::const_iterator it = rootObjectSet()->begin(); it != end; ++it) {
        if ((*it)->gcIsProtected(jsObject))
            return *it;
    }
    return 0;
}

RootObject* findRootObject(Interpreter* interpreter)
{
    RootObjectSet::const_iterator end = rootObjectSet()->end();
    for (RootObjectSet::const_iterator it = rootObjectSet()->begin(); it != end; ++it) {
        if ((*it)->interpreter() == interpreter)
            return *it;
    }
    return 0;
}

PassRefPtr<RootObject> RootObject::create(const void* nativeHandle, PassRefPtr<Interpreter> interpreter)
{
    return new RootObject(nativeHandle, interpreter);
}

RootObject::RootObject(const void* nativeHandle, PassRefPtr<Interpreter> interpreter)
    : m_refCount(0)
    , m_isValid(true)
    , m_nativeHandle(nativeHandle)
    , m_interpreter(interpreter)
{
    ASSERT(m_interpreter);
    rootObjectSet()->add(this);
}

RootObject::~RootObject()
{
    if (m_isValid)
        invalidate();
}

void RootObject::invalidate()
{
    if (!m_isValid)
        return;

    {
        HashSet<RuntimeObjectImp*>::iterator end = m_runtimeObjects.end();
        for (HashSet<RuntimeObjectImp*>::iterator it = m_runtimeObjects.begin(); it != end; ++it)
            (*it)->invalidate();
        
        m_runtimeObjects.clear();
    }
    
    m_isValid = false;

    m_nativeHandle = 0;
    m_interpreter = 0;

    ProtectCountSet::iterator end = m_protectCountSet.end();
    for (ProtectCountSet::iterator it = m_protectCountSet.begin(); it != end; ++it) {
        JSLock lock;
        KJS::gcUnprotect(it->first);
    }
    m_protectCountSet.clear();

    rootObjectSet()->remove(this);
}

void RootObject::gcProtect(JSObject* jsObject)
{
    ASSERT(m_isValid);
    
    if (!m_protectCountSet.contains(jsObject)) {
        JSLock lock;
        KJS::gcProtect(jsObject);
    }
    m_protectCountSet.add(jsObject);
}

void RootObject::gcUnprotect(JSObject* jsObject)
{
    ASSERT(m_isValid);
    
    if (!jsObject)
        return;

    if (m_protectCountSet.count(jsObject) == 1) {
        JSLock lock;
        KJS::gcUnprotect(jsObject);
    }
    m_protectCountSet.remove(jsObject);
}

bool RootObject::gcIsProtected(JSObject* jsObject)
{
    ASSERT(m_isValid);
    return m_protectCountSet.contains(jsObject);
}

const void* RootObject::nativeHandle() const 
{ 
    ASSERT(m_isValid);
    return m_nativeHandle; 
}

Interpreter* RootObject::interpreter() const 
{ 
    ASSERT(m_isValid);
    return m_interpreter.get(); 
}

void RootObject::addRuntimeObject(RuntimeObjectImp* object)
{
    ASSERT(m_isValid);
    ASSERT(!m_runtimeObjects.contains(object));
    
    m_runtimeObjects.add(object);
}        
    
void RootObject::removeRuntimeObject(RuntimeObjectImp* object)
{
    ASSERT(m_isValid);
    ASSERT(m_runtimeObjects.contains(object));
    
    m_runtimeObjects.remove(object);
}

} } // namespace KJS::Bindings
#endif //USE(NPOBJECT)
