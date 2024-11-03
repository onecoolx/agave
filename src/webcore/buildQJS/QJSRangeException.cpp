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

#include "QJSRangeException.h"

#include "RangeException.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSRangeExceptionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSRangeException::getValueProperty, NULL, JSRangeException::ConstructorAttrNum)
};

class JSRangeExceptionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSRangeExceptionConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSRangeExceptionConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("BAD_BOUNDARYPOINTS_ERR", JSRangeExceptionConstructor::getValueProperty, NULL, RangeException::BAD_BOUNDARYPOINTS_ERR),
    JS_CGETSET_MAGIC_DEF("INVALID_NODE_TYPE_ERR", JSRangeExceptionConstructor::getValueProperty, NULL, RangeException::INVALID_NODE_TYPE_ERR)
};

JSValue JSRangeExceptionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[RangeException.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRangeExceptionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[RangeException.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRangeExceptionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeExceptionConstructorFunctions, countof(JSRangeExceptionConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSRangeExceptionPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("BAD_BOUNDARYPOINTS_ERR", JSRangeExceptionPrototype::getValueProperty, NULL, RangeException::BAD_BOUNDARYPOINTS_ERR),
    JS_CGETSET_MAGIC_DEF("INVALID_NODE_TYPE_ERR", JSRangeExceptionPrototype::getValueProperty, NULL, RangeException::INVALID_NODE_TYPE_ERR)
};

JSValue JSRangeExceptionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSRangeException.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRangeExceptionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSRangeException.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRangeExceptionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeExceptionAttributesFunctions, countof(JSRangeExceptionAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSRangeExceptionPrototypeConstantsFunctions, countof(JSRangeExceptionPrototypeConstantsFunctions));
}

JSValue JSRangeExceptionPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSRangeExceptionClassDefine = 
{
    "RangeException",
    .finalizer = JSRangeException::finalizer,
    .gc_mark = JSRangeException::mark,
};

JSClassID JSRangeException::js_class_id = 0;

void JSRangeException::init(JSContext* ctx)
{
    if (JSRangeException::js_class_id == 0) {
        JS_NewClassID(&JSRangeException::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRangeException::js_class_id, &JSRangeExceptionClassDefine);
        JS_SetConstructor(ctx, JSRangeExceptionConstructor::self(ctx), JSRangeExceptionPrototype::self(ctx));
        JS_SetClassProto(ctx, JSRangeException::js_class_id, JSRangeExceptionPrototype::self(ctx));
    }
}

JSValue JSRangeException::create(JSContext* ctx, RangeException* impl)
{
    JSRangeException::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSRangeExceptionPrototype::self(ctx), JSRangeException::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSRangeException::finalizer(JSRuntime* rt, JSValue val)
{
    RangeException* impl = (RangeException*)JS_GetOpaque(val, JSRangeException::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSRangeException::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSRangeException::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSRangeException::getConstructor(JSContext *ctx)
{
    return JSRangeExceptionConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, RangeException* obj)
{
    return QJS::cacheDOMObject<RangeException, JSRangeException>(ctx, obj);
}

RangeException* toRangeException(JSValue val)
{
    if (JS_IsObject(val)) {
        RangeException* impl = (RangeException*)JS_GetOpaque(val, JSRangeException::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
