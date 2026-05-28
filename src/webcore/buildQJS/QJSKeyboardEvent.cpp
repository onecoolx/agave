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

#include "QJSKeyboardEvent.h"

#include "ExceptionCode.h"
#include "KeyboardEvent.h"
#include "QJSDOMWindow.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSKeyboardEventAttributesFunctions[7];
static bool JSKeyboardEventAttributesFunctions_initialized = false;

static void init_JSKeyboardEventAttributesFunctions()
{
    if (JSKeyboardEventAttributesFunctions_initialized) return;
    JSKeyboardEventAttributesFunctions_initialized = true;
    memset(JSKeyboardEventAttributesFunctions, 0, sizeof(JSKeyboardEventAttributesFunctions));
    JSKeyboardEventAttributesFunctions[0].name = "shiftKey";
    JSKeyboardEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[0].magic = JSKeyboardEvent::ShiftKeyAttrNum;
    JSKeyboardEventAttributesFunctions[0].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[1].name = "altGraphKey";
    JSKeyboardEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[1].magic = JSKeyboardEvent::AltGraphKeyAttrNum;
    JSKeyboardEventAttributesFunctions[1].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[2].name = "keyIdentifier";
    JSKeyboardEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[2].magic = JSKeyboardEvent::KeyIdentifierAttrNum;
    JSKeyboardEventAttributesFunctions[2].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[3].name = "altKey";
    JSKeyboardEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[3].magic = JSKeyboardEvent::AltKeyAttrNum;
    JSKeyboardEventAttributesFunctions[3].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[4].name = "keyLocation";
    JSKeyboardEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[4].magic = JSKeyboardEvent::KeyLocationAttrNum;
    JSKeyboardEventAttributesFunctions[4].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[5].name = "ctrlKey";
    JSKeyboardEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[5].magic = JSKeyboardEvent::CtrlKeyAttrNum;
    JSKeyboardEventAttributesFunctions[5].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSKeyboardEventAttributesFunctions[6].name = "metaKey";
    JSKeyboardEventAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSKeyboardEventAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSKeyboardEventAttributesFunctions[6].magic = JSKeyboardEvent::MetaKeyAttrNum;
    JSKeyboardEventAttributesFunctions[6].u.getset.get.getter_magic = JSKeyboardEvent::getValueProperty;
    JSKeyboardEventAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSKeyboardEventPrototypeFunctions[1];
static bool JSKeyboardEventPrototypeFunctions_initialized = false;

static void init_JSKeyboardEventPrototypeFunctions()
{
    if (JSKeyboardEventPrototypeFunctions_initialized) return;
    JSKeyboardEventPrototypeFunctions_initialized = true;
    memset(JSKeyboardEventPrototypeFunctions, 0, sizeof(JSKeyboardEventPrototypeFunctions));
    JSKeyboardEventPrototypeFunctions[0].name = "initKeyboardEvent";
    JSKeyboardEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSKeyboardEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSKeyboardEventPrototypeFunctions[0].magic = JSKeyboardEvent::InitKeyboardEventFuncNum;
    JSKeyboardEventPrototypeFunctions[0].u.func.length = 11;
    JSKeyboardEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSKeyboardEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSKeyboardEventPrototypeFunction::callAsFunction;
}

JSValue JSKeyboardEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSKeyboardEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSKeyboardEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSKeyboardEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSKeyboardEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSKeyboardEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSKeyboardEventAttributesFunctions, countof(JSKeyboardEventAttributesFunctions));
    init_JSKeyboardEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSKeyboardEventPrototypeFunctions, countof(JSKeyboardEventPrototypeFunctions));
}

static JSClassDef JSKeyboardEventClassDefine;
static bool JSKeyboardEventClassDefine_initialized = false;

static void init_JSKeyboardEventClassDefine()
{
    if (JSKeyboardEventClassDefine_initialized) return;
    JSKeyboardEventClassDefine_initialized = true;
    memset(&JSKeyboardEventClassDefine, 0, sizeof(JSKeyboardEventClassDefine));
    JSKeyboardEventClassDefine.class_name = "KeyboardEvent";
    JSKeyboardEventClassDefine.finalizer = JSKeyboardEvent::finalizer;
    JSKeyboardEventClassDefine.gc_mark = JSKeyboardEvent::mark;
}

JSClassID JSKeyboardEvent::js_class_id = 0;

void JSKeyboardEvent::init(JSContext* ctx)
{
    if (JSKeyboardEvent::js_class_id == 0) {
        init_JSKeyboardEventClassDefine();
        JS_NewClassID(&JSKeyboardEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSKeyboardEvent::js_class_id, &JSKeyboardEventClassDefine);
        JS_SetClassProto(ctx, JSKeyboardEvent::js_class_id, JSKeyboardEventPrototype::self(ctx));
    }
}

JSValue JSKeyboardEvent::create(JSContext* ctx, KeyboardEvent* impl)
{
    JSKeyboardEvent::init(ctx);
    JSValue _proto = JSKeyboardEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSKeyboardEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSKeyboardEvent::finalizer(JSRuntime* rt, JSValue val)
{
    KeyboardEvent* impl = (KeyboardEvent*)JS_GetOpaque(val, JSKeyboardEvent::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSKeyboardEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSUIEvent::mark(rt, val, mark_func);
}

JSValue JSKeyboardEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case KeyIdentifierAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->keyIdentifier()).utf8().data());
        }
        case KeyLocationAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewInt32(ctx, imp->keyLocation());
        }
        case CtrlKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case AltGraphKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->altGraphKey() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSKeyboardEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque(this_val, JSKeyboardEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSKeyboardEvent::InitKeyboardEventFuncNum: {
            AtomicString type = valueToString(ctx, argv[0]);
            bool canBubble = valueToBoolean(ctx, argv[1]);
            bool cancelable = valueToBoolean(ctx, argv[2]);
            DOMWindow* view = toDOMWindow(argv[3]);
            String keyIdentifier = valueToString(ctx, argv[4]);
            bool keyLocationOk;
            unsigned keyLocation = valueToInt32(ctx, argv[5], keyLocationOk);
            if (!keyLocationOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool ctrlKey = valueToBoolean(ctx, argv[6]);
            bool altKey = valueToBoolean(ctx, argv[7]);
            bool shiftKey = valueToBoolean(ctx, argv[8]);
            bool metaKey = valueToBoolean(ctx, argv[9]);
            bool altGraphKey = valueToBoolean(ctx, argv[10]);
            imp->initKeyboardEvent(type, canBubble, cancelable, view, keyIdentifier, keyLocation, ctrlKey, altKey, shiftKey, metaKey, altGraphKey);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
