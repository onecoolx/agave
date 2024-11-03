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

#include "QJSCanvasGradient.h"

#include "CanvasGradient.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Prototype functions table */

static const JSCFunctionListEntry JSCanvasGradientPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("addColorStop", 2, JSCanvasGradientPrototypeFunction::callAsFunction, JSCanvasGradient::AddColorStopFuncNum)
};

JSValue JSCanvasGradientPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCanvasGradient.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCanvasGradientPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCanvasGradient.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCanvasGradientPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCanvasGradientPrototypeFunctions, countof(JSCanvasGradientPrototypeFunctions));
}

static JSClassDef JSCanvasGradientClassDefine = 
{
    "CanvasGradient",
    .finalizer = JSCanvasGradient::finalizer,
    .gc_mark = JSCanvasGradient::mark,
};

JSClassID JSCanvasGradient::js_class_id = 0;

void JSCanvasGradient::init(JSContext* ctx)
{
    if (JSCanvasGradient::js_class_id == 0) {
        JS_NewClassID(&JSCanvasGradient::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCanvasGradient::js_class_id, &JSCanvasGradientClassDefine);
        JS_SetClassProto(ctx, JSCanvasGradient::js_class_id, JSCanvasGradientPrototype::self(ctx));
    }
}

JSValue JSCanvasGradient::create(JSContext* ctx, CanvasGradient* impl)
{
    JSCanvasGradient::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCanvasGradientPrototype::self(ctx), JSCanvasGradient::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCanvasGradient::finalizer(JSRuntime* rt, JSValue val)
{
    CanvasGradient* impl = (CanvasGradient*)JS_GetOpaque(val, JSCanvasGradient::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCanvasGradient::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCanvasGradientPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CanvasGradient* imp = (CanvasGradient*)JS_GetOpaque2(ctx, this_val, JSCanvasGradient::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCanvasGradient::AddColorStopFuncNum: {
            float offset = valueToFloat(ctx, argv[0]);
            String color = valueToString(ctx, argv[1]);
            imp->addColorStop(offset, color);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, CanvasGradient* obj)
{
    return QJS::cacheDOMObject<CanvasGradient, JSCanvasGradient>(ctx, obj);
}

CanvasGradient* toCanvasGradient(JSValue val)
{
    if (JS_IsObject(val)) {
        CanvasGradient* impl = (CanvasGradient*)JS_GetOpaque(val, JSCanvasGradient::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
