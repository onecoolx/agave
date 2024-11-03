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

static const JSCFunctionListEntry JSMouseEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("metaKey", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::MetaKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("toElement", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ToElementAttrNum),
    JS_CGETSET_MAGIC_DEF("relatedTarget", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::RelatedTargetAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::XAttrNum),
    JS_CGETSET_MAGIC_DEF("clientY", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ClientYAttrNum),
    JS_CGETSET_MAGIC_DEF("screenX", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ScreenXAttrNum),
    JS_CGETSET_MAGIC_DEF("screenY", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ScreenYAttrNum),
    JS_CGETSET_MAGIC_DEF("dataTransfer", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::DataTransferAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetY", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::OffsetYAttrNum),
    JS_CGETSET_MAGIC_DEF("clientX", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ClientXAttrNum),
    JS_CGETSET_MAGIC_DEF("ctrlKey", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::CtrlKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("shiftKey", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ShiftKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("altKey", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::AltKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("button", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::ButtonAttrNum),
    JS_CGETSET_MAGIC_DEF("offsetX", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::OffsetXAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::YAttrNum),
    JS_CGETSET_MAGIC_DEF("fromElement", JSMouseEvent::getValueProperty, NULL, JSMouseEvent::FromElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSMouseEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initMouseEvent", 15, JSMouseEventPrototypeFunction::callAsFunction, JSMouseEvent::InitMouseEventFuncNum)
};

JSValue JSMouseEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSMouseEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSUIEventPrototype::self(ctx));
        JSMouseEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSMouseEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSMouseEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSMouseEventAttributesFunctions, countof(JSMouseEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSMouseEventPrototypeFunctions, countof(JSMouseEventPrototypeFunctions));
}

static JSClassDef JSMouseEventClassDefine = 
{
    "MouseEvent",
    .finalizer = JSMouseEvent::finalizer,
    .gc_mark = JSMouseEvent::mark,
};

JSClassID JSMouseEvent::js_class_id = 0;

void JSMouseEvent::init(JSContext* ctx)
{
    if (JSMouseEvent::js_class_id == 0) {
        JS_NewClassID(&JSMouseEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSMouseEvent::js_class_id, &JSMouseEventClassDefine);
        JS_SetClassProto(ctx, JSMouseEvent::js_class_id, JSMouseEventPrototype::self(ctx));
    }
}

JSValue JSMouseEvent::create(JSContext* ctx, MouseEvent* impl)
{
    JSMouseEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSMouseEventPrototype::self(ctx), JSMouseEvent::js_class_id);
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
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenX());
        }
        case ScreenYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenY());
        }
        case ClientXAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientX());
        }
        case ClientYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->clientY());
        }
        case CtrlKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->ctrlKey() ? 1 : 0);
        }
        case ShiftKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->shiftKey() ? 1 : 0);
        }
        case AltKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->altKey() ? 1 : 0);
        }
        case MetaKeyAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBool(ctx, imp->metaKey() ? 1 : 0);
        }
        case ButtonAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->button());
        }
        case RelatedTargetAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->relatedTarget()));
        }
        case OffsetXAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetX());
        }
        case OffsetYAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->offsetY());
        }
        case XAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case FromElementAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->fromElement()));
        }
        case ToElementAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->toElement()));
        }
        case DataTransferAttrNum: {
            MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->dataTransfer()));
        }
    }
    return JS_NULL;
}

JSValue JSMouseEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    MouseEvent* imp = (MouseEvent*)JS_GetOpaque2(ctx, this_val, JSMouseEvent::js_class_id);
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
