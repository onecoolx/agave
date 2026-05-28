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

#include <string.h>

#include "QJSHistory.h"

#include "ExceptionCode.h"
#include "History.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHistoryAttributesFunctions[1];
static bool JSHistoryAttributesFunctions_initialized = false;

static void init_JSHistoryAttributesFunctions()
{
    if (JSHistoryAttributesFunctions_initialized) return;
    JSHistoryAttributesFunctions_initialized = true;
    memset(JSHistoryAttributesFunctions, 0, sizeof(JSHistoryAttributesFunctions));
    JSHistoryAttributesFunctions[0].name = "length";
    JSHistoryAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHistoryAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHistoryAttributesFunctions[0].magic = JSHistory::LengthAttrNum;
    JSHistoryAttributesFunctions[0].u.getset.get.getter_magic = JSHistory::getValueProperty;
    JSHistoryAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSHistoryPrototypeFunctions[3];
static bool JSHistoryPrototypeFunctions_initialized = false;

static void init_JSHistoryPrototypeFunctions()
{
    if (JSHistoryPrototypeFunctions_initialized) return;
    JSHistoryPrototypeFunctions_initialized = true;
    memset(JSHistoryPrototypeFunctions, 0, sizeof(JSHistoryPrototypeFunctions));
    JSHistoryPrototypeFunctions[0].name = "forward";
    JSHistoryPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHistoryPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHistoryPrototypeFunctions[0].magic = JSHistory::ForwardFuncNum;
    JSHistoryPrototypeFunctions[0].u.func.length = 0;
    JSHistoryPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHistoryPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHistoryPrototypeFunction::callAsFunction;
    JSHistoryPrototypeFunctions[1].name = "back";
    JSHistoryPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHistoryPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHistoryPrototypeFunctions[1].magic = JSHistory::BackFuncNum;
    JSHistoryPrototypeFunctions[1].u.func.length = 0;
    JSHistoryPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHistoryPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHistoryPrototypeFunction::callAsFunction;
    JSHistoryPrototypeFunctions[2].name = "go";
    JSHistoryPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHistoryPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHistoryPrototypeFunctions[2].magic = JSHistory::GoFuncNum;
    JSHistoryPrototypeFunctions[2].u.func.length = 1;
    JSHistoryPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHistoryPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHistoryPrototypeFunction::callAsFunction;
}

JSValue JSHistoryPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHistory.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHistoryPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHistory.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHistoryPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHistoryAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHistoryAttributesFunctions, countof(JSHistoryAttributesFunctions));
    init_JSHistoryPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHistoryPrototypeFunctions, countof(JSHistoryPrototypeFunctions));
}

static JSClassDef JSHistoryClassDefine;
static bool JSHistoryClassDefine_initialized = false;

static void init_JSHistoryClassDefine()
{
    if (JSHistoryClassDefine_initialized) return;
    JSHistoryClassDefine_initialized = true;
    memset(&JSHistoryClassDefine, 0, sizeof(JSHistoryClassDefine));
    JSHistoryClassDefine.class_name = "History";
    JSHistoryClassDefine.finalizer = JSHistory::finalizer;
    JSHistoryClassDefine.gc_mark = JSHistory::mark;
}

JSClassID JSHistory::js_class_id = 0;

void JSHistory::init(JSContext* ctx)
{
    if (JSHistory::js_class_id == 0) {
        init_JSHistoryClassDefine();
        JS_NewClassID(&JSHistory::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHistory::js_class_id, &JSHistoryClassDefine);
        JS_SetClassProto(ctx, JSHistory::js_class_id, JSHistoryPrototype::self(ctx));
    }
}

JSValue JSHistory::create(JSContext* ctx, History* impl)
{
    JSHistory::init(ctx);
    JSValue _proto = JSHistoryPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSHistory::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHistory::finalizer(JSRuntime* rt, JSValue val)
{
    History* impl = (History*)JS_GetOpaque(val, JSHistory::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHistory::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSHistory::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            History* imp = (History*)JS_GetOpaque(this_val, JSHistory::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
    }
    return JS_NULL;
}

JSValue JSHistoryPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    History* imp = (History*)JS_GetOpaque(this_val, JSHistory::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHistory::BackFuncNum: {
            imp->back();
            return JS_UNDEFINED;
        }
        case JSHistory::ForwardFuncNum: {
            imp->forward();
            return JS_UNDEFINED;
        }
        case JSHistory::GoFuncNum: {
            bool distanceOk;
            int distance = valueToInt32(ctx, argv[0], distanceOk);
            if (!distanceOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->go(distance);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, History* obj)
{
    return QJS::cacheDOMObject<History, JSHistory>(ctx, obj);
}

History* toHistory(JSValue val)
{
    if (JS_IsObject(val)) {
        History* impl = (History*)JS_GetOpaque(val, JSHistory::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
