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

#include "QJSKeyboardEvent.h"

#include "ExceptionCode.h"
#include "KeyboardEvent.h"
#include "QJSDOMWindow.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSKeyboardEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("shiftKey", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::ShiftKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("altGraphKey", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::AltGraphKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("keyIdentifier", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::KeyIdentifierAttrNum),
    JS_CGETSET_MAGIC_DEF("altKey", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::AltKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("keyLocation", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::KeyLocationAttrNum),
    JS_CGETSET_MAGIC_DEF("ctrlKey", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::CtrlKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("metaKey", JSKeyboardEvent::getValueProperty, NULL, JSKeyboardEvent::MetaKeyAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSKeyboardEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initKeyboardEvent", 11, JSKeyboardEventPrototypeFunction::callAsFunction, JSKeyboardEvent::InitKeyboardEventFuncNum)
};

JSValue JSKeyboardEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSKeyboardEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSKeyboardEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSKeyboardEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSKeyboardEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSKeyboardEventAttributesFunctions, countof(JSKeyboardEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSKeyboardEventPrototypeFunctions, countof(JSKeyboardEventPrototypeFunctions));
}

static JSClassDef JSKeyboardEventClassDefine = 
{
    "KeyboardEvent",
    .finalizer = JSKeyboardEvent::finalizer,
    .gc_mark = JSKeyboardEvent::mark,
};

JSClassID JSKeyboardEvent::js_class_id = 0;

void JSKeyboardEvent::init(JSContext* ctx)
{
    if (JSKeyboardEvent::js_class_id == 0) {
        JS_NewClassID(&JSKeyboardEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSKeyboardEvent::js_class_id, &JSKeyboardEventClassDefine);
        JS_SetClassProto(ctx, JSKeyboardEvent::js_class_id, JSKeyboardEventPrototype::self(ctx));
    }
}

JSValue JSKeyboardEvent::create(JSContext* ctx, KeyboardEvent* impl)
{
    JSKeyboardEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSKeyboardEventPrototype::self(ctx), JSKeyboardEvent::js_class_id);
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
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->keyIdentifier()).utf8().data());
        }
        case KeyLocationAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->keyLocation());
        }
        case CtrlKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case AltGraphKeyAttrNum: {
            KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
            return JS_NewBool(ctx, imp->altGraphKey() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSKeyboardEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    KeyboardEvent* imp = (KeyboardEvent*)JS_GetOpaque2(ctx, this_val, JSKeyboardEvent::js_class_id);
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
