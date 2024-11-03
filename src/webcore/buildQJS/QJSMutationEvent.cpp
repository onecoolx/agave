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

#include "QJSMutationEvent.h"

#include "MutationEvent.h"
#include "Node.h"
#include "QJSNode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSMutationEventAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("prevValue", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::PrevValueAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("attrName", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::AttrNameAttrNum),
    JS_CGETSET_MAGIC_DEF("relatedNode", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::RelatedNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("newValue", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::NewValueAttrNum),
    JS_CGETSET_MAGIC_DEF("attrChange", JSMutationEvent::getValueProperty, NULL, JSMutationEvent::AttrChangeAttrNum)
};

class JSMutationEventConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSMutationEventConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSMutationEventConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MODIFICATION", JSMutationEventConstructor::getValueProperty, NULL, MutationEvent::MODIFICATION),
    JS_CGETSET_MAGIC_DEF("ADDITION", JSMutationEventConstructor::getValueProperty, NULL, MutationEvent::ADDITION),
    JS_CGETSET_MAGIC_DEF("REMOVAL", JSMutationEventConstructor::getValueProperty, NULL, MutationEvent::REMOVAL)
};

JSValue JSMutationEventConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[MutationEvent.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSMutationEventConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[MutationEvent.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSMutationEventConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventConstructorFunctions, countof(JSMutationEventConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSMutationEventPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MODIFICATION", JSMutationEventPrototype::getValueProperty, NULL, MutationEvent::MODIFICATION),
    JS_CGETSET_MAGIC_DEF("ADDITION", JSMutationEventPrototype::getValueProperty, NULL, MutationEvent::ADDITION),
    JS_CGETSET_MAGIC_DEF("REMOVAL", JSMutationEventPrototype::getValueProperty, NULL, MutationEvent::REMOVAL)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSMutationEventPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("initMutationEvent", 8, JSMutationEventPrototypeFunction::callAsFunction, JSMutationEvent::InitMutationEventFuncNum)
};

JSValue JSMutationEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSMutationEvent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSMutationEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSMutationEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSMutationEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventAttributesFunctions, countof(JSMutationEventAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventPrototypeConstantsFunctions, countof(JSMutationEventPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventPrototypeFunctions, countof(JSMutationEventPrototypeFunctions));
}

JSValue JSMutationEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSMutationEventClassDefine = 
{
    "MutationEvent",
    .finalizer = JSMutationEvent::finalizer,
    .gc_mark = JSMutationEvent::mark,
};

JSClassID JSMutationEvent::js_class_id = 0;

void JSMutationEvent::init(JSContext* ctx)
{
    if (JSMutationEvent::js_class_id == 0) {
        JS_NewClassID(&JSMutationEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSMutationEvent::js_class_id, &JSMutationEventClassDefine);
        JS_SetConstructor(ctx, JSMutationEventConstructor::self(ctx), JSMutationEventPrototype::self(ctx));
        JS_SetClassProto(ctx, JSMutationEvent::js_class_id, JSMutationEventPrototype::self(ctx));
    }
}

JSValue JSMutationEvent::create(JSContext* ctx, MutationEvent* impl)
{
    JSMutationEvent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSMutationEventPrototype::self(ctx), JSMutationEvent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSMutationEvent::finalizer(JSRuntime* rt, JSValue val)
{
    MutationEvent* impl = (MutationEvent*)JS_GetOpaque(val, JSMutationEvent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSMutationEvent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEvent::mark(rt, val, mark_func);
}

JSValue JSMutationEvent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RelatedNodeAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->relatedNode()));
        }
        case PrevValueAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->prevValue()).utf8().data());
        }
        case NewValueAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->newValue()).utf8().data());
        }
        case AttrNameAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->attrName()).utf8().data());
        }
        case AttrChangeAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
            return JS_NewBigUint64(ctx, imp->attrChange());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSMutationEvent::getConstructor(JSContext *ctx)
{
    return JSMutationEventConstructor::self(ctx);
}

JSValue JSMutationEventPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    MutationEvent* imp = (MutationEvent*)JS_GetOpaque2(ctx, this_val, JSMutationEvent::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSMutationEvent::InitMutationEventFuncNum: {
            AtomicString type = valueToString(ctx, argv[0]);
            bool canBubble = valueToBoolean(ctx, argv[1]);
            bool cancelable = valueToBoolean(ctx, argv[2]);
            Node* relatedNode = toNode(argv[3]);
            String prevValue = valueToString(ctx, argv[4]);
            String newValue = valueToString(ctx, argv[5]);
            String attrName = valueToString(ctx, argv[6]);
            unsigned short attrChange = valueToInt32(ctx, argv[7]);
            imp->initMutationEvent(type, canBubble, cancelable, relatedNode, prevValue, newValue, attrName, attrChange);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
