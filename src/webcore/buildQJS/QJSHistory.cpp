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

#include "QJSHistory.h"

#include "ExceptionCode.h"
#include "History.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHistoryAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSHistory::getValueProperty, NULL, JSHistory::LengthAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSHistoryPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("forward", 0, JSHistoryPrototypeFunction::callAsFunction, JSHistory::ForwardFuncNum),
    JS_CFUNC_MAGIC_DEF("back", 0, JSHistoryPrototypeFunction::callAsFunction, JSHistory::BackFuncNum),
    JS_CFUNC_MAGIC_DEF("go", 1, JSHistoryPrototypeFunction::callAsFunction, JSHistory::GoFuncNum)
};

JSValue JSHistoryPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHistory.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHistoryPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHistory.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHistoryPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHistoryAttributesFunctions, countof(JSHistoryAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHistoryPrototypeFunctions, countof(JSHistoryPrototypeFunctions));
}

static JSClassDef JSHistoryClassDefine = 
{
    "History",
    .finalizer = JSHistory::finalizer,
    .gc_mark = JSHistory::mark,
};

JSClassID JSHistory::js_class_id = 0;

void JSHistory::init(JSContext* ctx)
{
    if (JSHistory::js_class_id == 0) {
        JS_NewClassID(&JSHistory::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHistory::js_class_id, &JSHistoryClassDefine);
        JS_SetClassProto(ctx, JSHistory::js_class_id, JSHistoryPrototype::self(ctx));
    }
}

JSValue JSHistory::create(JSContext* ctx, History* impl)
{
    JSHistory::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHistoryPrototype::self(ctx), JSHistory::js_class_id);
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
            History* imp = (History*)JS_GetOpaque2(ctx, this_val, JSHistory::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
    }
    return JS_NULL;
}

JSValue JSHistoryPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    History* imp = (History*)JS_GetOpaque2(ctx, this_val, JSHistory::js_class_id);
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
