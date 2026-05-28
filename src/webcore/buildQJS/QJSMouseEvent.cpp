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

#include "QJSMouseEvent.h"

#include "Clipboard.h"
#include "EventTargetNode.h"
#include "ExceptionCode.h"
#include "MouseEvent.h"
#include "Node.h"
#include "QJSDOMWindow.h"
#include "QJSEventTargetNode.h"
#include "QJSNode.h"
#include "qjs_dom.h"
#include "qjs_events.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSMouseEventAttributesFunctions[17];
static bool JSMouseEventAttributesFunctions_initialized = false;

static void init_JSMouseEventAttributesFunctions()
{
    if (JSMouseEventAttributesFunctions_initialized) return;
    JSMouseEventAttributesFunctions_initialized = true;
    memset(JSMouseEventAttributesFunctions, 0, sizeof(JSMouseEventAttributesFunctions));
    JSMouseEventAttributesFunctions[0].name = "screenX";
    JSMouseEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[0].magic = JSMouseEvent::ScreenXAttrNum;
    JSMouseEventAttributesFunctions[0].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[1].name = "screenY";
    JSMouseEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[1].magic = JSMouseEvent::ScreenYAttrNum;
    JSMouseEventAttributesFunctions[1].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[2].name = "clientX";
    JSMouseEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[2].magic = JSMouseEvent::ClientXAttrNum;
    JSMouseEventAttributesFunctions[2].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[3].name = "clientY";
    JSMouseEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[3].magic = JSMouseEvent::ClientYAttrNum;
    JSMouseEventAttributesFunctions[3].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[4].name = "ctrlKey";
    JSMouseEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[4].magic = JSMouseEvent::CtrlKeyAttrNum;
    JSMouseEventAttributesFunctions[4].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[5].name = "shiftKey";
    JSMouseEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[5].magic = JSMouseEvent::ShiftKeyAttrNum;
    JSMouseEventAttributesFunctions[5].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[6].name = "altKey";
    JSMouseEventAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[6].magic = JSMouseEvent::AltKeyAttrNum;
    JSMouseEventAttributesFunctions[6].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[7].name = "metaKey";
    JSMouseEventAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[7].magic = JSMouseEvent::MetaKeyAttrNum;
    JSMouseEventAttributesFunctions[7].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[8].name = "button";
    JSMouseEventAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[8].magic = JSMouseEvent::ButtonAttrNum;
    JSMouseEventAttributesFunctions[8].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[9].name = "relatedTarget";
    JSMouseEventAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[9].magic = JSMouseEvent::RelatedTargetAttrNum;
    JSMouseEventAttributesFunctions[9].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[10].name = "offsetX";
    JSMouseEventAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[10].magic = JSMouseEvent::OffsetXAttrNum;
    JSMouseEventAttributesFunctions[10].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[11].name = "offsetY";
    JSMouseEventAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[11].magic = JSMouseEvent::OffsetYAttrNum;
    JSMouseEventAttributesFunctions[11].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[12].name = "x";
    JSMouseEventAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[12].magic = JSMouseEvent::XAttrNum;
    JSMouseEventAttributesFunctions[12].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[13].name = "y";
    JSMouseEventAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[13].magic = JSMouseEvent::YAttrNum;
    JSMouseEventAttributesFunctions[13].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[14].name = "fromElement";
    JSMouseEventAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[14].magic = JSMouseEvent::FromElementAttrNum;
    JSMouseEventAttributesFunctions[14].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[15].name = "toElement";
    JSMouseEventAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[15].magic = JSMouseEvent::ToElementAttrNum;
    JSMouseEventAttributesFunctions[15].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSMouseEventAttributesFunctions[16].name = "dataTransfer";
    JSMouseEventAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSMouseEventAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSMouseEventAttributesFunctions[16].magic = JSMouseEvent::DataTransferAttrNum;
    JSMouseEventAttributesFunctions[16].u.getset.get.getter_magic = JSMouseEvent::getValueProperty;
    JSMouseEventAttributesFunctions[16].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSMouseEventPrototypeFunctions[1];
static bool JSMouseEventPrototypeFunctions_initialized = false;

static void init_JSMouseEventPrototypeFunctions()
{
    if (JSMouseEventPrototypeFunctions_initialized) return;
    JSMouseEventPrototypeFunctions_initialized = true;
    memset(JSMouseEventPrototypeFunctions, 0, sizeof(JSMouseEventPrototypeFunctions));
    JSMouseEventPrototypeFunctions[0].name = "initMouseEvent";
    JSMouseEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSMouseEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSMouseEventPrototypeFunctions[0].magic = JSMouseEvent::InitMouseEventFuncNum;
    JSMouseEventPrototypeFunctions[0].u.func.length = 15;
    JSMouseEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSMouseEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSMouseEventPrototypeFunction::callAsFunction;
}

JSValue JSMouseEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSMouseEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSMouseEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSMouseEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSMouseEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSMouseEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMouseEventAttributesFunctions, countof(JSMouseEventAttributesFunctions));
    init_JSMouseEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMouseEventPrototypeFunctions, countof(JSMouseEventPrototypeFunctions));
}

static JSClassDef JSMouseEventClassDefine;
static bool JSMouseEventClassDefine_initialized = false;

static void init_JSMouseEventClassDefine()
{
    if (JSMouseEventClassDefine_initialized) return;
    JSMouseEventClassDefine_initialized = true;
    memset(&JSMouseEventClassDefine, 0, sizeof(JSMouseEventClassDefine));
    JSMouseEventClassDefine.class_name = "MouseEvent";
    JSMouseEventClassDefine.finalizer = JSMouseEvent::finalizer;
    JSMouseEventClassDefine.gc_mark = JSMouseEvent::mark;
}

JSClassID JSMouseEvent::js_class_id = 0;

void JSMouseEvent::init(JSContext* ctx)
{
    if (JSMouseEvent::js_class_id == 0) {
        init_JSMouseEventClassDefine();
        JS_NewClassID(&JSMouseEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSMouseEvent::js_class_id, &JSMouseEventClassDefine);
        JS_SetClassProto(ctx, JSMouseEvent::js_class_id, JSMouseEventPrototype::self(ctx));
    }
}

JSValue JSMouseEvent::create(JSContext* ctx, MouseEvent* impl)
{
    JSMouseEvent::init(ctx);
    JSValue _proto = JSMouseEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSMouseEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSMouseEvent::finalizer(JSRuntime* rt, JSValue val)
{
    MouseEvent* impl = (MouseEvent*)JS_GetOpaque(val, JSMouseEvent::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSMouseEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSUIEvent::mark(rt, val, mark_func);
}

JSValue JSMouseEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ScreenXAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->screenY());
        }
        case ClientXAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->clientX());
        }
        case ClientYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->clientY());
        }
        case CtrlKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case ButtonAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->button());
        }
        case RelatedTargetAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->relatedTarget()));
        }
        case OffsetXAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->offsetX());
        }
        case OffsetYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->offsetY());
        }
        case XAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
        case FromElementAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->fromElement()));
        }
        case ToElementAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->toElement()));
        }
        case DataTransferAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->dataTransfer()));
        }
    }
    return JS_NULL;
}

JSValue JSMouseEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    MouseEvent* imp = (MouseEvent*)JS_GetOpaque(this_val, JSMouseEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSMouseEvent::InitMouseEventFuncNum: {
            AtomicString type = valueToString(ctx, argv[0]);
            bool canBubble = valueToBoolean(ctx, argv[1]);
            bool cancelable = valueToBoolean(ctx, argv[2]);
            DOMWindow* view = toDOMWindow(argv[3]);
            bool detailOk;
            int detail = valueToInt32(ctx, argv[4], detailOk);
            if (!detailOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool screenXOk;
            int screenX = valueToInt32(ctx, argv[5], screenXOk);
            if (!screenXOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool screenYOk;
            int screenY = valueToInt32(ctx, argv[6], screenYOk);
            if (!screenYOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool clientXOk;
            int clientX = valueToInt32(ctx, argv[7], clientXOk);
            if (!clientXOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool clientYOk;
            int clientY = valueToInt32(ctx, argv[8], clientYOk);
            if (!clientYOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool ctrlKey = valueToBoolean(ctx, argv[9]);
            bool altKey = valueToBoolean(ctx, argv[10]);
            bool shiftKey = valueToBoolean(ctx, argv[11]);
            bool metaKey = valueToBoolean(ctx, argv[12]);
            unsigned short button = valueToInt32(ctx, argv[13]);
            EventTargetNode* relatedTarget = toEventTargetNode(argv[14]);
            imp->initMouseEvent(type, canBubble, cancelable, view, detail, screenX, screenY, clientX, clientY, ctrlKey, altKey, shiftKey, metaKey, button, relatedTarget);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
