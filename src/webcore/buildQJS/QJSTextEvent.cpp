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

#include "QJSTextEvent.h"

#include "QJSDOMWindow.h"
#include "TextEvent.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSTextEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("data", JSTextEvent::getValueProperty, NULL, JSTextEvent::DataAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSTextEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initTextEvent", 5, JSTextEventPrototypeFunction::callAsFunction, JSTextEvent::InitTextEventFuncNum)
};

JSValue JSTextEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSTextEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSTextEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSTextEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSTextEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSTextEventAttributesFunctions, countof(JSTextEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSTextEventPrototypeFunctions, countof(JSTextEventPrototypeFunctions));
}

static JSClassDef JSTextEventClassDefine = 
{
    "TextEvent",
    .finalizer = JSTextEvent::finalizer,
    .gc_mark = JSTextEvent::mark,
};

JSClassID JSTextEvent::js_class_id = 0;

void JSTextEvent::init(JSContext* ctx)
{
    if (JSTextEvent::js_class_id == 0) {
        JS_NewClassID(&JSTextEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSTextEvent::js_class_id, &JSTextEventClassDefine);
        JS_SetClassProto(ctx, JSTextEvent::js_class_id, JSTextEventPrototype::self(ctx));
    }
}

JSValue JSTextEvent::create(JSContext* ctx, TextEvent* impl)
{
    JSTextEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSTextEventPrototype::self(ctx), JSTextEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSTextEvent::finalizer(JSRuntime* rt, JSValue val)
{
    TextEvent* impl = (TextEvent*)JS_GetOpaque(val, JSTextEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSTextEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSUIEvent::mark(rt, val, mark_func);
}

JSValue JSTextEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DataAttrNum: {
            TextEvent* imp = (TextEvent*)JS_GetOpaque2(ctx, this_val, JSTextEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->data()).utf8().data());
        }
    }
    return JS_NULL;
}

JSValue JSTextEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    TextEvent* imp = (TextEvent*)JS_GetOpaque2(ctx, this_val, JSTextEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSTextEvent::InitTextEventFuncNum: {
            String typeArg = valueToString(ctx, argv[0]);
            bool canBubbleArg = valueToBoolean(ctx, argv[1]);
            bool cancelableArg = valueToBoolean(ctx, argv[2]);
            DOMWindow* viewArg = toDOMWindow(argv[3]);
            String dataArg = valueToString(ctx, argv[4]);
            imp->initTextEvent(typeArg, canBubbleArg, cancelableArg, viewArg, dataArg);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
