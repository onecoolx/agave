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

#ifndef _NP_RUNTIME_IMPL_H_
#define _NP_RUNTIME_IMPL_H_

#include "npruntime.h"

void _NPN_ReleaseVariantValue(NPVariant *variant);
NPIdentifier _NPN_GetStringIdentifier(const NPUTF8 *name);
void _NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers);
NPIdentifier _NPN_GetIntIdentifier(int32_t intid);
bool _NPN_IdentifierIsString(NPIdentifier identifier);
NPUTF8 *_NPN_UTF8FromIdentifier(NPIdentifier identifier);
int32_t _NPN_IntFromIdentifier(NPIdentifier identifier);    
NPObject *_NPN_CreateObject(NPP npp, NPClass *aClass);
NPObject *_NPN_RetainObject(NPObject *obj);
void _NPN_ReleaseObject(NPObject *obj);
bool _NPN_Invoke(NPP npp, NPObject *npobj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool _NPN_InvokeDefault(NPP npp, NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool _NPN_Evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result);
bool _NPN_GetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, NPVariant *result);
bool _NPN_SetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, const NPVariant *value);
bool _NPN_RemoveProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool _NPN_HasProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool _NPN_HasMethod(NPP npp, NPObject *npobj, NPIdentifier methodName);
void _NPN_SetException(NPObject *obj, const NPUTF8 *message);
bool _NPN_Enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier, uint32_t *count);

void _NPN_DeallocateObject(NPObject *obj);
#endif
