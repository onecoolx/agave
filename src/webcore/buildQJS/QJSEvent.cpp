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

#include "QJSEvent.h"

#include "Event.h"
#include "EventTargetNode.h"
#include "QJSEventTargetNode.h"
#include "qjs_dom.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("srcElement", JSEvent::getValueProperty, NULL, JSEvent::SrcElementAttrNum),
    JS_CGETSET_MAGIC_DEF("bubbles", JSEvent::getValueProperty, NULL, JSEvent::BubblesAttrNum),
    JS_CGETSET_MAGIC_DEF("currentTarget", JSEvent::getValueProperty, NULL, JSEvent::CurrentTargetAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSEvent::getValueProperty, NULL, JSEvent::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("returnValue", JSEvent::getValueProperty, JSEvent::putValueProperty, JSEvent::ReturnValueAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSEvent::getValueProperty, NULL, JSEvent::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSEvent::getValueProperty, NULL, JSEvent::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("eventPhase", JSEvent::getValueProperty, NULL, JSEvent::EventPhaseAttrNum),
    JS_CGETSET_MAGIC_DEF("cancelable", JSEvent::getValueProperty, NULL, JSEvent::CancelableAttrNum),
    JS_CGETSET_MAGIC_DEF("timeStamp", JSEvent::getValueProperty, NULL, JSEvent::TimeStampAttrNum),
    JS_CGETSET_MAGIC_DEF("cancelBubble", JSEvent::getValueProperty, JSEvent::putValueProperty, JSEvent::CancelBubbleAttrNum),
    JS_CGETSET_MAGIC_DEF("clipboardData", JSEvent::getValueProperty, NULL, JSEvent::ClipboardDataAttrNum)
};

class JSEventConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSEventConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSEventConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MOUSEOUT", JSEventConstructor::getValueProperty, NULL, Event::MOUSEOUT),
    JS_CGETSET_MAGIC_DEF("MOUSEOVER", JSEventConstructor::getValueProperty, NULL, Event::MOUSEOVER),
    JS_CGETSET_MAGIC_DEF("CAPTURING_PHASE", JSEventConstructor::getValueProperty, NULL, Event::CAPTURING_PHASE),
    JS_CGETSET_MAGIC_DEF("MOUSEDOWN", JSEventConstructor::getValueProperty, NULL, Event::MOUSEDOWN),
    JS_CGETSET_MAGIC_DEF("AT_TARGET", JSEventConstructor::getValueProperty, NULL, Event::AT_TARGET),
    JS_CGETSET_MAGIC_DEF("DRAGDROP", JSEventConstructor::getValueProperty, NULL, Event::DRAGDROP),
    JS_CGETSET_MAGIC_DEF("KEYUP", JSEventConstructor::getValueProperty, NULL, Event::KEYUP),
    JS_CGETSET_MAGIC_DEF("SELECT", JSEventConstructor::getValueProperty, NULL, Event::SELECT),
    JS_CGETSET_MAGIC_DEF("BLUR", JSEventConstructor::getValueProperty, NULL, Event::BLUR),
    JS_CGETSET_MAGIC_DEF("MOUSEMOVE", JSEventConstructor::getValueProperty, NULL, Event::MOUSEMOVE),
    JS_CGETSET_MAGIC_DEF("CLICK", JSEventConstructor::getValueProperty, NULL, Event::CLICK),
    JS_CGETSET_MAGIC_DEF("MOUSEUP", JSEventConstructor::getValueProperty, NULL, Event::MOUSEUP),
    JS_CGETSET_MAGIC_DEF("BUBBLING_PHASE", JSEventConstructor::getValueProperty, NULL, Event::BUBBLING_PHASE),
    JS_CGETSET_MAGIC_DEF("MOUSEDRAG", JSEventConstructor::getValueProperty, NULL, Event::MOUSEDRAG),
    JS_CGETSET_MAGIC_DEF("DBLCLICK", JSEventConstructor::getValueProperty, NULL, Event::DBLCLICK),
    JS_CGETSET_MAGIC_DEF("KEYDOWN", JSEventConstructor::getValueProperty, NULL, Event::KEYDOWN),
    JS_CGETSET_MAGIC_DEF("KEYPRESS", JSEventConstructor::getValueProperty, NULL, Event::KEYPRESS),
    JS_CGETSET_MAGIC_DEF("FOCUS", JSEventConstructor::getValueProperty, NULL, Event::FOCUS),
    JS_CGETSET_MAGIC_DEF("CHANGE", JSEventConstructor::getValueProperty, NULL, Event::CHANGE)
};

JSValue JSEventConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Event.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSEventConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Event.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSEventConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventConstructorFunctions, countof(JSEventConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSEventPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MOUSEOUT", JSEventPrototype::getValueProperty, NULL, Event::MOUSEOUT),
    JS_CGETSET_MAGIC_DEF("MOUSEOVER", JSEventPrototype::getValueProperty, NULL, Event::MOUSEOVER),
    JS_CGETSET_MAGIC_DEF("CAPTURING_PHASE", JSEventPrototype::getValueProperty, NULL, Event::CAPTURING_PHASE),
    JS_CGETSET_MAGIC_DEF("MOUSEDOWN", JSEventPrototype::getValueProperty, NULL, Event::MOUSEDOWN),
    JS_CGETSET_MAGIC_DEF("AT_TARGET", JSEventPrototype::getValueProperty, NULL, Event::AT_TARGET),
    JS_CGETSET_MAGIC_DEF("DRAGDROP", JSEventPrototype::getValueProperty, NULL, Event::DRAGDROP),
    JS_CGETSET_MAGIC_DEF("KEYUP", JSEventPrototype::getValueProperty, NULL, Event::KEYUP),
    JS_CGETSET_MAGIC_DEF("SELECT", JSEventPrototype::getValueProperty, NULL, Event::SELECT),
    JS_CGETSET_MAGIC_DEF("BLUR", JSEventPrototype::getValueProperty, NULL, Event::BLUR),
    JS_CGETSET_MAGIC_DEF("MOUSEMOVE", JSEventPrototype::getValueProperty, NULL, Event::MOUSEMOVE),
    JS_CGETSET_MAGIC_DEF("CLICK", JSEventPrototype::getValueProperty, NULL, Event::CLICK),
    JS_CGETSET_MAGIC_DEF("MOUSEUP", JSEventPrototype::getValueProperty, NULL, Event::MOUSEUP),
    JS_CGETSET_MAGIC_DEF("BUBBLING_PHASE", JSEventPrototype::getValueProperty, NULL, Event::BUBBLING_PHASE),
    JS_CGETSET_MAGIC_DEF("MOUSEDRAG", JSEventPrototype::getValueProperty, NULL, Event::MOUSEDRAG),
    JS_CGETSET_MAGIC_DEF("DBLCLICK", JSEventPrototype::getValueProperty, NULL, Event::DBLCLICK),
    JS_CGETSET_MAGIC_DEF("KEYDOWN", JSEventPrototype::getValueProperty, NULL, Event::KEYDOWN),
    JS_CGETSET_MAGIC_DEF("KEYPRESS", JSEventPrototype::getValueProperty, NULL, Event::KEYPRESS),
    JS_CGETSET_MAGIC_DEF("FOCUS", JSEventPrototype::getValueProperty, NULL, Event::FOCUS),
    JS_CGETSET_MAGIC_DEF("CHANGE", JSEventPrototype::getValueProperty, NULL, Event::CHANGE)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("stopPropagation", 0, JSEventPrototypeFunction::callAsFunction, JSEvent::StopPropagationFuncNum),
    JS_CFUNC_MAGIC_DEF("preventDefault", 0, JSEventPrototypeFunction::callAsFunction, JSEvent::PreventDefaultFuncNum),
    JS_CFUNC_MAGIC_DEF("initEvent", 3, JSEventPrototypeFunction::callAsFunction, JSEvent::InitEventFuncNum)
};

JSValue JSEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventAttributesFunctions, countof(JSEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventPrototypeConstantsFunctions, countof(JSEventPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSEventPrototypeFunctions, countof(JSEventPrototypeFunctions));
}

JSValue JSEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSEventClassDefine = 
{
    "Event",
    .finalizer = JSEvent::finalizer,
    .gc_mark = JSEvent::mark,
};

JSClassID JSEvent::js_class_id = 0;

void JSEvent::init(JSContext* ctx)
{
    if (JSEvent::js_class_id == 0) {
        JS_NewClassID(&JSEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSEvent::js_class_id, &JSEventClassDefine);
        JS_SetConstructor(ctx, JSEventConstructor::self(ctx), JSEventPrototype::self(ctx));
        JS_SetClassProto(ctx, JSEvent::js_class_id, JSEventPrototype::self(ctx));
    }
}

JSValue JSEvent::create(JSContext* ctx, Event* impl)
{
    JSEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSEventPrototype::self(ctx), JSEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSEvent::finalizer(JSRuntime* rt, JSValue val)
{
    Event* impl = (Event*)JS_GetOpaque(val, JSEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case TargetAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->target()));
        }
        case CurrentTargetAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->currentTarget()));
        }
        case EventPhaseAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->eventPhase());
        }
        case BubblesAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->bubbles() ? 1 : 0);
        }
        case CancelableAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->cancelable() ? 1 : 0);
        }
        case TimeStampAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->timeStamp());
        }
        case SrcElementAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->srcElement()));
        }
        case ReturnValueAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->returnValue() ? 1 : 0);
        }
        case CancelBubbleAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JS_NewBool(ctx, imp->cancelBubble() ? 1 : 0);
        }
        case ClipboardDataAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            return JSEvent::clipboardData(ctx, imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSEvent::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ReturnValueAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            imp->setReturnValue(valueToBoolean(ctx, value));
            break;
        }
        case CancelBubbleAttrNum: {
            Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
            imp->setCancelBubble(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSEvent::getConstructor(JSContext *ctx)
{
    return JSEventConstructor::self(ctx);
}

JSValue JSEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Event* imp = (Event*)JS_GetOpaque2(ctx, this_val, JSEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSEvent::StopPropagationFuncNum: {
            imp->stopPropagation();
            return JS_UNDEFINED;
        }
        case JSEvent::PreventDefaultFuncNum: {
            imp->preventDefault();
            return JS_UNDEFINED;
        }
        case JSEvent::InitEventFuncNum: {
            String eventTypeArg = valueToString(ctx, argv[0]);
            bool canBubbleArg = valueToBoolean(ctx, argv[1]);
            bool cancelableArg = valueToBoolean(ctx, argv[2]);
            imp->initEvent(eventTypeArg, canBubbleArg, cancelableArg);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


Event* toEvent(JSValue val)
{
    if (JS_IsObject(val)) {
        Event* impl = (Event*)JS_GetOpaque(val, JSEvent::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
