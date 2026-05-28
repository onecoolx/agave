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

#include "QJSWheelEvent.h"

#include "WheelEvent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSWheelEventAttributesFunctions[15];
static bool JSWheelEventAttributesFunctions_initialized = false;

static void init_JSWheelEventAttributesFunctions()
{
    if (JSWheelEventAttributesFunctions_initialized) return;
    JSWheelEventAttributesFunctions_initialized = true;
    memset(JSWheelEventAttributesFunctions, 0, sizeof(JSWheelEventAttributesFunctions));
    JSWheelEventAttributesFunctions[0].name = "screenX";
    JSWheelEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[0].magic = JSWheelEvent::ScreenXAttrNum;
    JSWheelEventAttributesFunctions[0].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[1].name = "screenY";
    JSWheelEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[1].magic = JSWheelEvent::ScreenYAttrNum;
    JSWheelEventAttributesFunctions[1].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[2].name = "clientX";
    JSWheelEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[2].magic = JSWheelEvent::ClientXAttrNum;
    JSWheelEventAttributesFunctions[2].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[3].name = "clientY";
    JSWheelEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[3].magic = JSWheelEvent::ClientYAttrNum;
    JSWheelEventAttributesFunctions[3].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[4].name = "ctrlKey";
    JSWheelEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[4].magic = JSWheelEvent::CtrlKeyAttrNum;
    JSWheelEventAttributesFunctions[4].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[5].name = "shiftKey";
    JSWheelEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[5].magic = JSWheelEvent::ShiftKeyAttrNum;
    JSWheelEventAttributesFunctions[5].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[6].name = "altKey";
    JSWheelEventAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[6].magic = JSWheelEvent::AltKeyAttrNum;
    JSWheelEventAttributesFunctions[6].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[7].name = "metaKey";
    JSWheelEventAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[7].magic = JSWheelEvent::MetaKeyAttrNum;
    JSWheelEventAttributesFunctions[7].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[8].name = "wheelDelta";
    JSWheelEventAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[8].magic = JSWheelEvent::WheelDeltaAttrNum;
    JSWheelEventAttributesFunctions[8].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[9].name = "wheelDeltaX";
    JSWheelEventAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[9].magic = JSWheelEvent::WheelDeltaXAttrNum;
    JSWheelEventAttributesFunctions[9].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[10].name = "wheelDeltaY";
    JSWheelEventAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[10].magic = JSWheelEvent::WheelDeltaYAttrNum;
    JSWheelEventAttributesFunctions[10].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[11].name = "offsetX";
    JSWheelEventAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[11].magic = JSWheelEvent::OffsetXAttrNum;
    JSWheelEventAttributesFunctions[11].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[12].name = "offsetY";
    JSWheelEventAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[12].magic = JSWheelEvent::OffsetYAttrNum;
    JSWheelEventAttributesFunctions[12].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[13].name = "x";
    JSWheelEventAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[13].magic = JSWheelEvent::XAttrNum;
    JSWheelEventAttributesFunctions[13].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSWheelEventAttributesFunctions[14].name = "y";
    JSWheelEventAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSWheelEventAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSWheelEventAttributesFunctions[14].magic = JSWheelEvent::YAttrNum;
    JSWheelEventAttributesFunctions[14].u.getset.get.getter_magic = JSWheelEvent::getValueProperty;
    JSWheelEventAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

JSValue JSWheelEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSWheelEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSWheelEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSWheelEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSWheelEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSWheelEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSWheelEventAttributesFunctions, countof(JSWheelEventAttributesFunctions));
}

static JSClassDef JSWheelEventClassDefine;
static bool JSWheelEventClassDefine_initialized = false;

static void init_JSWheelEventClassDefine()
{
    if (JSWheelEventClassDefine_initialized) return;
    JSWheelEventClassDefine_initialized = true;
    memset(&JSWheelEventClassDefine, 0, sizeof(JSWheelEventClassDefine));
    JSWheelEventClassDefine.class_name = "WheelEvent";
    JSWheelEventClassDefine.finalizer = JSWheelEvent::finalizer;
    JSWheelEventClassDefine.gc_mark = JSWheelEvent::mark;
}

JSClassID JSWheelEvent::js_class_id = 0;

void JSWheelEvent::init(JSContext* ctx)
{
    if (JSWheelEvent::js_class_id == 0) {
        init_JSWheelEventClassDefine();
        JS_NewClassID(&JSWheelEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSWheelEvent::js_class_id, &JSWheelEventClassDefine);
        JS_SetClassProto(ctx, JSWheelEvent::js_class_id, JSWheelEventPrototype::self(ctx));
    }
}

JSValue JSWheelEvent::create(JSContext* ctx, WheelEvent* impl)
{
    JSWheelEvent::init(ctx);
    JSValue _proto = JSWheelEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSWheelEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->screenY());
        }
        case ClientXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->clientX());
        }
        case ClientYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->clientY());
        }
        case CtrlKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case WheelDeltaAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->wheelDelta());
        }
        case WheelDeltaXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->wheelDeltaX());
        }
        case WheelDeltaYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->wheelDeltaY());
        }
        case OffsetXAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->offsetX());
        }
        case OffsetYAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->offsetY());
        }
        case XAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            WheelEvent* imp = (WheelEvent*)JS_GetOpaque(this_val, JSWheelEvent::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}


}
