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

#include "QJSOverflowEvent.h"

#include "OverflowEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSOverflowEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("orient", JSOverflowEvent::getValueProperty, NULL, JSOverflowEvent::OrientAttrNum),
    JS_CGETSET_MAGIC_DEF("verticalOverflow", JSOverflowEvent::getValueProperty, NULL, JSOverflowEvent::VerticalOverflowAttrNum),
    JS_CGETSET_MAGIC_DEF("horizontalOverflow", JSOverflowEvent::getValueProperty, NULL, JSOverflowEvent::HorizontalOverflowAttrNum)
};

/* Functions table */

static const JSCFunctionListEntry JSOverflowEventPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("BOTH", JSOverflowEventPrototype::getValueProperty, NULL, OverflowEvent::BOTH),
    JS_CGETSET_MAGIC_DEF("HORIZONTAL", JSOverflowEventPrototype::getValueProperty, NULL, OverflowEvent::HORIZONTAL),
    JS_CGETSET_MAGIC_DEF("VERTICAL", JSOverflowEventPrototype::getValueProperty, NULL, OverflowEvent::VERTICAL)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSOverflowEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initOverflowEvent", 3, JSOverflowEventPrototypeFunction::callAsFunction, JSOverflowEvent::InitOverflowEventFuncNum)
};

JSValue JSOverflowEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSOverflowEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSOverflowEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSOverflowEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSOverflowEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventAttributesFunctions, countof(JSOverflowEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventPrototypeConstantsFunctions, countof(JSOverflowEventPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventPrototypeFunctions, countof(JSOverflowEventPrototypeFunctions));
}

JSValue JSOverflowEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSOverflowEventClassDefine = 
{
    "OverflowEvent",
    .finalizer = JSOverflowEvent::finalizer,
    .gc_mark = JSOverflowEvent::mark,
};

JSClassID JSOverflowEvent::js_class_id = 0;

void JSOverflowEvent::init(JSContext* ctx)
{
    if (JSOverflowEvent::js_class_id == 0) {
        JS_NewClassID(&JSOverflowEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSOverflowEvent::js_class_id, &JSOverflowEventClassDefine);
        JS_SetClassProto(ctx, JSOverflowEvent::js_class_id, JSOverflowEventPrototype::self(ctx));
    }
}

JSValue JSOverflowEvent::create(JSContext* ctx, OverflowEvent* impl)
{
    JSOverflowEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSOverflowEventPrototype::self(ctx), JSOverflowEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSOverflowEvent::finalizer(JSRuntime* rt, JSValue val)
{
    OverflowEvent* impl = (OverflowEvent*)JS_GetOpaque(val, JSOverflowEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSOverflowEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEvent::mark(rt, val, mark_func);
}

JSValue JSOverflowEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case OrientAttrNum: {
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque2(ctx, this_val, JSOverflowEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->orient());
        }
        case HorizontalOverflowAttrNum: {
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque2(ctx, this_val, JSOverflowEvent::js_class_id);
            return JS_NewBool(ctx, imp->horizontalOverflow() ? 1 : 0);
        }
        case VerticalOverflowAttrNum: {
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque2(ctx, this_val, JSOverflowEvent::js_class_id);
            return JS_NewBool(ctx, imp->verticalOverflow() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSOverflowEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque2(ctx, this_val, JSOverflowEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSOverflowEvent::InitOverflowEventFuncNum: {
            unsigned short orient = valueToInt32(ctx, argv[0]);
            bool horizontalOverflow = valueToBoolean(ctx, argv[1]);
            bool verticalOverflow = valueToBoolean(ctx, argv[2]);
            imp->initOverflowEvent(orient, horizontalOverflow, verticalOverflow);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
