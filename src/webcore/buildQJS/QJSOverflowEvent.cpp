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

#include "QJSOverflowEvent.h"

#include "OverflowEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSOverflowEventAttributesFunctions[3];
static bool JSOverflowEventAttributesFunctions_initialized = false;

static void init_JSOverflowEventAttributesFunctions()
{
    if (JSOverflowEventAttributesFunctions_initialized) return;
    JSOverflowEventAttributesFunctions_initialized = true;
    memset(JSOverflowEventAttributesFunctions, 0, sizeof(JSOverflowEventAttributesFunctions));
    JSOverflowEventAttributesFunctions[0].name = "orient";
    JSOverflowEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventAttributesFunctions[0].magic = JSOverflowEvent::OrientAttrNum;
    JSOverflowEventAttributesFunctions[0].u.getset.get.getter_magic = JSOverflowEvent::getValueProperty;
    JSOverflowEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSOverflowEventAttributesFunctions[1].name = "verticalOverflow";
    JSOverflowEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventAttributesFunctions[1].magic = JSOverflowEvent::VerticalOverflowAttrNum;
    JSOverflowEventAttributesFunctions[1].u.getset.get.getter_magic = JSOverflowEvent::getValueProperty;
    JSOverflowEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSOverflowEventAttributesFunctions[2].name = "horizontalOverflow";
    JSOverflowEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventAttributesFunctions[2].magic = JSOverflowEvent::HorizontalOverflowAttrNum;
    JSOverflowEventAttributesFunctions[2].u.getset.get.getter_magic = JSOverflowEvent::getValueProperty;
    JSOverflowEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Functions table */

static JSCFunctionListEntry JSOverflowEventPrototypeConstantsFunctions[3];
static bool JSOverflowEventPrototypeConstantsFunctions_initialized = false;

static void init_JSOverflowEventPrototypeConstantsFunctions()
{
    if (JSOverflowEventPrototypeConstantsFunctions_initialized) return;
    JSOverflowEventPrototypeConstantsFunctions_initialized = true;
    memset(JSOverflowEventPrototypeConstantsFunctions, 0, sizeof(JSOverflowEventPrototypeConstantsFunctions));
    JSOverflowEventPrototypeConstantsFunctions[0].name = "BOTH";
    JSOverflowEventPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventPrototypeConstantsFunctions[0].magic = OverflowEvent::BOTH;
    JSOverflowEventPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSOverflowEventPrototype::getValueProperty;
    JSOverflowEventPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSOverflowEventPrototypeConstantsFunctions[1].name = "HORIZONTAL";
    JSOverflowEventPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventPrototypeConstantsFunctions[1].magic = OverflowEvent::HORIZONTAL;
    JSOverflowEventPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSOverflowEventPrototype::getValueProperty;
    JSOverflowEventPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSOverflowEventPrototypeConstantsFunctions[2].name = "VERTICAL";
    JSOverflowEventPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSOverflowEventPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSOverflowEventPrototypeConstantsFunctions[2].magic = OverflowEvent::VERTICAL;
    JSOverflowEventPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSOverflowEventPrototype::getValueProperty;
    JSOverflowEventPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSOverflowEventPrototypeFunctions[1];
static bool JSOverflowEventPrototypeFunctions_initialized = false;

static void init_JSOverflowEventPrototypeFunctions()
{
    if (JSOverflowEventPrototypeFunctions_initialized) return;
    JSOverflowEventPrototypeFunctions_initialized = true;
    memset(JSOverflowEventPrototypeFunctions, 0, sizeof(JSOverflowEventPrototypeFunctions));
    JSOverflowEventPrototypeFunctions[0].name = "initOverflowEvent";
    JSOverflowEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSOverflowEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSOverflowEventPrototypeFunctions[0].magic = JSOverflowEvent::InitOverflowEventFuncNum;
    JSOverflowEventPrototypeFunctions[0].u.func.length = 3;
    JSOverflowEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSOverflowEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSOverflowEventPrototypeFunction::callAsFunction;
}

JSValue JSOverflowEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSOverflowEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSOverflowEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSOverflowEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSOverflowEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSOverflowEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventAttributesFunctions, countof(JSOverflowEventAttributesFunctions));
    init_JSOverflowEventPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventPrototypeConstantsFunctions, countof(JSOverflowEventPrototypeConstantsFunctions));
    init_JSOverflowEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSOverflowEventPrototypeFunctions, countof(JSOverflowEventPrototypeFunctions));
}

JSValue JSOverflowEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSOverflowEventClassDefine;
static bool JSOverflowEventClassDefine_initialized = false;

static void init_JSOverflowEventClassDefine()
{
    if (JSOverflowEventClassDefine_initialized) return;
    JSOverflowEventClassDefine_initialized = true;
    memset(&JSOverflowEventClassDefine, 0, sizeof(JSOverflowEventClassDefine));
    JSOverflowEventClassDefine.class_name = "OverflowEvent";
    JSOverflowEventClassDefine.finalizer = JSOverflowEvent::finalizer;
    JSOverflowEventClassDefine.gc_mark = JSOverflowEvent::mark;
}

JSClassID JSOverflowEvent::js_class_id = 0;

void JSOverflowEvent::init(JSContext* ctx)
{
    if (JSOverflowEvent::js_class_id == 0) {
        init_JSOverflowEventClassDefine();
        JS_NewClassID(&JSOverflowEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSOverflowEvent::js_class_id, &JSOverflowEventClassDefine);
        JS_SetClassProto(ctx, JSOverflowEvent::js_class_id, JSOverflowEventPrototype::self(ctx));
    }
}

JSValue JSOverflowEvent::create(JSContext* ctx, OverflowEvent* impl)
{
    JSOverflowEvent::init(ctx);
    JSValue _proto = JSOverflowEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSOverflowEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque(this_val, JSOverflowEvent::js_class_id);
            return JS_NewInt32(ctx, imp->orient());
        }
        case HorizontalOverflowAttrNum: {
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque(this_val, JSOverflowEvent::js_class_id);
            return JS_NewBool(ctx, imp->horizontalOverflow() ? 1 : 0);
        }
        case VerticalOverflowAttrNum: {
            OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque(this_val, JSOverflowEvent::js_class_id);
            return JS_NewBool(ctx, imp->verticalOverflow() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSOverflowEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    OverflowEvent* imp = (OverflowEvent*)JS_GetOpaque(this_val, JSOverflowEvent::js_class_id);
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
