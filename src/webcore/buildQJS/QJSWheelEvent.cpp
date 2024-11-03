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

#include "QJSWheelEvent.h"

#include "WheelEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSWheelEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("wheelDeltaY", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::WheelDeltaYAttrNum),
    JS_CGETSET_MAGIC_DEF("clientY", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::ClientYAttrNum),
    JS_CGETSET_MAGIC_DEF("wheelDelta", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::WheelDeltaAttrNum),
    JS_CGETSET_MAGIC_DEF("shiftKey", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::ShiftKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("screenX", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::ScreenXAttrNum),
    JS_CGETSET_MAGIC_DEF("wheelDeltaX", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::WheelDeltaXAttrNum),
    JS_CGETSET_MAGIC_DEF("altKey", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::AltKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("ctrlKey", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::CtrlKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("screenY", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::ScreenYAttrNum),
    JS_CGETSET_MAGIC_DEF("clientX", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::ClientXAttrNum),
    JS_CGETSET_MAGIC_DEF("metaKey", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::MetaKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetX", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::OffsetXAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetY", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::OffsetYAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSWheelEvent::getValueProperty, NULL, JSWheelEvent::YAttrNum)
};

JSValue JSWheelEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSWheelEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSWheelEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSWheelEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSWheelEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSWheelEventAttributesFunctions, countof(JSWheelEventAttributesFunctions));
}

static JSClassDef JSWheelEventClassDefine = 
{
    "WheelEvent",
    .finalizer = JSWheelEvent::finalizer,
    .gc_mark = JSWheelEvent::mark,
};

JSClassID JSWheelEvent::js_class_id = 0;

void JSWheelEvent::init(JSContext* ctx)
{
    if (JSWheelEvent::js_class_id == 0) {
        JS_NewClassID(&JSWheelEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSWheelEvent::js_class_id, &JSWheelEventClassDefine);
        JS_SetClassProto(ctx, JSWheelEvent::js_class_id, JSWheelEventPrototype::self(ctx));
    }
}

JSValue JSWheelEvent::create(JSContext* ctx, WheelEvent* impl)
{
    JSWheelEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSWheelEventPrototype::self(ctx), JSWheelEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSWheelEvent::finalizer(JSRuntime* rt, JSValue val)
{
    WheelEvent* impl = (WheelEvent*)JS_GetOpaque(val, JSWheelEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSWheelEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSUIEvent::mark(rt, val, mark_func);
}

JSValue JSWheelEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ScreenXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenY());
        }
        case ClientXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientX());
        }
        case ClientYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientY());
        }
        case CtrlKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case WheelDeltaAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->wheelDelta());
        }
        case WheelDeltaXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->wheelDeltaX());
        }
        case WheelDeltaYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->wheelDeltaY());
        }
        case OffsetXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetX());
        }
        case OffsetYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetY());
        }
        case XAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque2(ctx, this_val, JSWheelEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
    }
    return JS_NULL;
}


}
