/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#if ENABLE(WEB_STORAGE)

#include <string.h>

#include "QJSStorage.h"

#include "ExceptionCode.h"
#include "Storage.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSStorageAttributesFunctions[1];
static bool JSStorageAttributesFunctions_initialized = false;

static void init_JSStorageAttributesFunctions()
{
    if (JSStorageAttributesFunctions_initialized) return;
    JSStorageAttributesFunctions_initialized = true;
    memset(JSStorageAttributesFunctions, 0, sizeof(JSStorageAttributesFunctions));
    JSStorageAttributesFunctions[0].name = "length";
    JSStorageAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSStorageAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSStorageAttributesFunctions[0].magic = JSStorage::LengthAttrNum;
    JSStorageAttributesFunctions[0].u.getset.get.getter_magic = JSStorage::getValueProperty;
    JSStorageAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSStoragePrototypeFunctions[5];
static bool JSStoragePrototypeFunctions_initialized = false;

static void init_JSStoragePrototypeFunctions()
{
    if (JSStoragePrototypeFunctions_initialized) return;
    JSStoragePrototypeFunctions_initialized = true;
    memset(JSStoragePrototypeFunctions, 0, sizeof(JSStoragePrototypeFunctions));
    JSStoragePrototypeFunctions[0].name = "key";
    JSStoragePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStoragePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSStoragePrototypeFunctions[0].magic = JSStorage::KeyFuncNum;
    JSStoragePrototypeFunctions[0].u.func.length = 1;
    JSStoragePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSStoragePrototypeFunctions[0].u.func.cfunc.generic_magic = JSStoragePrototypeFunction::callAsFunction;
    JSStoragePrototypeFunctions[1].name = "getItem";
    JSStoragePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStoragePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSStoragePrototypeFunctions[1].magic = JSStorage::GetItemFuncNum;
    JSStoragePrototypeFunctions[1].u.func.length = 1;
    JSStoragePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSStoragePrototypeFunctions[1].u.func.cfunc.generic_magic = JSStoragePrototypeFunction::callAsFunction;
    JSStoragePrototypeFunctions[2].name = "setItem";
    JSStoragePrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStoragePrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSStoragePrototypeFunctions[2].magic = JSStorage::SetItemFuncNum;
    JSStoragePrototypeFunctions[2].u.func.length = 2;
    JSStoragePrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSStoragePrototypeFunctions[2].u.func.cfunc.generic_magic = JSStoragePrototypeFunction::callAsFunction;
    JSStoragePrototypeFunctions[3].name = "removeItem";
    JSStoragePrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStoragePrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSStoragePrototypeFunctions[3].magic = JSStorage::RemoveItemFuncNum;
    JSStoragePrototypeFunctions[3].u.func.length = 1;
    JSStoragePrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSStoragePrototypeFunctions[3].u.func.cfunc.generic_magic = JSStoragePrototypeFunction::callAsFunction;
    JSStoragePrototypeFunctions[4].name = "clear";
    JSStoragePrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStoragePrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSStoragePrototypeFunctions[4].magic = JSStorage::ClearFuncNum;
    JSStoragePrototypeFunctions[4].u.func.length = 0;
    JSStoragePrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSStoragePrototypeFunctions[4].u.func.cfunc.generic_magic = JSStoragePrototypeFunction::callAsFunction;
}

JSValue JSStoragePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSStorage.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSStoragePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSStorage.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSStoragePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSStorageAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSStorageAttributesFunctions, countof(JSStorageAttributesFunctions));
    init_JSStoragePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSStoragePrototypeFunctions, countof(JSStoragePrototypeFunctions));
}

static JSClassDef JSStorageClassDefine;
static bool JSStorageClassDefine_initialized = false;

static void init_JSStorageClassDefine()
{
    if (JSStorageClassDefine_initialized) return;
    JSStorageClassDefine_initialized = true;
    memset(&JSStorageClassDefine, 0, sizeof(JSStorageClassDefine));
    JSStorageClassDefine.class_name = "Storage";
    JSStorageClassDefine.finalizer = JSStorage::finalizer;
    JSStorageClassDefine.gc_mark = JSStorage::mark;
}

JSClassID JSStorage::js_class_id = 0;

void JSStorage::init(JSContext* ctx)
{
    if (JSStorage::js_class_id == 0) {
        init_JSStorageClassDefine();
        JS_NewClassID(&JSStorage::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSStorage::js_class_id, &JSStorageClassDefine);
        JS_SetClassProto(ctx, JSStorage::js_class_id, JSStoragePrototype::self(ctx));
    }
}

JSValue JSStorage::create(JSContext* ctx, Storage* impl)
{
    JSStorage::init(ctx);
    JSValue _proto = JSStoragePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSStorage::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSStorage::finalizer(JSRuntime* rt, JSValue val)
{
    Storage* impl = (Storage*)JS_GetOpaque(val, JSStorage::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSStorage::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSStorage::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            Storage* imp = (Storage*)JS_GetOpaque(this_val, JSStorage::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
    }
    return JS_NULL;
}

JSValue JSStoragePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Storage* imp = (Storage*)JS_GetOpaque(this_val, JSStorage::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSStorage::KeyFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = jsStringOrNull(ctx, imp->key(index));
            return result;
        }
        case JSStorage::GetItemFuncNum: {
            String key = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getItem(key));
            return result;
        }
        case JSStorage::SetItemFuncNum: {
            ExceptionCode ec = 0;
            String key = valueToString(ctx, argv[0]);
            String data = valueToString(ctx, argv[1]);
            imp->setItem(key, data, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSStorage::RemoveItemFuncNum: {
            String key = valueToString(ctx, argv[0]);
            imp->removeItem(key);
            return JS_UNDEFINED;
        }
        case JSStorage::ClearFuncNum: {
            imp->clear();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue JSStorage::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    Storage* imp = (Storage*)JS_GetOpaque2(ctx, this_obj, JSStorage::js_class_id);
    return jsStringOrNull(ctx, imp->key(idx));
}

JSValue toJS(JSContext *ctx, Storage* obj)
{
    return QJS::cacheDOMObject<Storage, JSStorage>(ctx, obj);
}

Storage* toStorage(JSValue val)
{
    if (JS_IsObject(val)) {
        Storage* impl = (Storage*)JS_GetOpaque(val, JSStorage::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(WEB_STORAGE)
