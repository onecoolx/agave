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

#include "QJSMutationEvent.h"

#include "MutationEvent.h"
#include "Node.h"
#include "QJSNode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSMutationEventAttributesFunctions[6];
static bool JSMutationEventAttributesFunctions_initialized = false;

static void init_JSMutationEventAttributesFunctions()
{
    if (JSMutationEventAttributesFunctions_initialized) return;
    JSMutationEventAttributesFunctions_initialized = true;
    memset(JSMutationEventAttributesFunctions, 0, sizeof(JSMutationEventAttributesFunctions));
    JSMutationEventAttributesFunctions[0].name = "prevValue";
    JSMutationEventAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[0].magic = JSMutationEvent::PrevValueAttrNum;
    JSMutationEventAttributesFunctions[0].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSMutationEventAttributesFunctions[1].name = "constructor";
    JSMutationEventAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[1].magic = JSMutationEvent::ConstructorAttrNum;
    JSMutationEventAttributesFunctions[1].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSMutationEventAttributesFunctions[2].name = "attrName";
    JSMutationEventAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[2].magic = JSMutationEvent::AttrNameAttrNum;
    JSMutationEventAttributesFunctions[2].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSMutationEventAttributesFunctions[3].name = "relatedNode";
    JSMutationEventAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[3].magic = JSMutationEvent::RelatedNodeAttrNum;
    JSMutationEventAttributesFunctions[3].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSMutationEventAttributesFunctions[4].name = "newValue";
    JSMutationEventAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[4].magic = JSMutationEvent::NewValueAttrNum;
    JSMutationEventAttributesFunctions[4].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSMutationEventAttributesFunctions[5].name = "attrChange";
    JSMutationEventAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventAttributesFunctions[5].magic = JSMutationEvent::AttrChangeAttrNum;
    JSMutationEventAttributesFunctions[5].u.getset.get.getter_magic = JSMutationEvent::getValueProperty;
    JSMutationEventAttributesFunctions[5].u.getset.set.setter_magic = NULL;
}

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

static JSCFunctionListEntry JSMutationEventConstructorFunctions[3];
static bool JSMutationEventConstructorFunctions_initialized = false;

static void init_JSMutationEventConstructorFunctions()
{
    if (JSMutationEventConstructorFunctions_initialized) return;
    JSMutationEventConstructorFunctions_initialized = true;
    memset(JSMutationEventConstructorFunctions, 0, sizeof(JSMutationEventConstructorFunctions));
    JSMutationEventConstructorFunctions[0].name = "MODIFICATION";
    JSMutationEventConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventConstructorFunctions[0].magic = MutationEvent::MODIFICATION;
    JSMutationEventConstructorFunctions[0].u.getset.get.getter_magic = JSMutationEventConstructor::getValueProperty;
    JSMutationEventConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSMutationEventConstructorFunctions[1].name = "ADDITION";
    JSMutationEventConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventConstructorFunctions[1].magic = MutationEvent::ADDITION;
    JSMutationEventConstructorFunctions[1].u.getset.get.getter_magic = JSMutationEventConstructor::getValueProperty;
    JSMutationEventConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSMutationEventConstructorFunctions[2].name = "REMOVAL";
    JSMutationEventConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventConstructorFunctions[2].magic = MutationEvent::REMOVAL;
    JSMutationEventConstructorFunctions[2].u.getset.get.getter_magic = JSMutationEventConstructor::getValueProperty;
    JSMutationEventConstructorFunctions[2].u.getset.set.setter_magic = NULL;
}

JSValue JSMutationEventConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[MutationEvent.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSMutationEventConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[MutationEvent.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSMutationEventConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSMutationEventConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventConstructorFunctions, countof(JSMutationEventConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSMutationEventPrototypeConstantsFunctions[3];
static bool JSMutationEventPrototypeConstantsFunctions_initialized = false;

static void init_JSMutationEventPrototypeConstantsFunctions()
{
    if (JSMutationEventPrototypeConstantsFunctions_initialized) return;
    JSMutationEventPrototypeConstantsFunctions_initialized = true;
    memset(JSMutationEventPrototypeConstantsFunctions, 0, sizeof(JSMutationEventPrototypeConstantsFunctions));
    JSMutationEventPrototypeConstantsFunctions[0].name = "MODIFICATION";
    JSMutationEventPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventPrototypeConstantsFunctions[0].magic = MutationEvent::MODIFICATION;
    JSMutationEventPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSMutationEventPrototype::getValueProperty;
    JSMutationEventPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSMutationEventPrototypeConstantsFunctions[1].name = "ADDITION";
    JSMutationEventPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventPrototypeConstantsFunctions[1].magic = MutationEvent::ADDITION;
    JSMutationEventPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSMutationEventPrototype::getValueProperty;
    JSMutationEventPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSMutationEventPrototypeConstantsFunctions[2].name = "REMOVAL";
    JSMutationEventPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSMutationEventPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSMutationEventPrototypeConstantsFunctions[2].magic = MutationEvent::REMOVAL;
    JSMutationEventPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSMutationEventPrototype::getValueProperty;
    JSMutationEventPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSMutationEventPrototypeFunctions[1];
static bool JSMutationEventPrototypeFunctions_initialized = false;

static void init_JSMutationEventPrototypeFunctions()
{
    if (JSMutationEventPrototypeFunctions_initialized) return;
    JSMutationEventPrototypeFunctions_initialized = true;
    memset(JSMutationEventPrototypeFunctions, 0, sizeof(JSMutationEventPrototypeFunctions));
    JSMutationEventPrototypeFunctions[0].name = "initMutationEvent";
    JSMutationEventPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSMutationEventPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSMutationEventPrototypeFunctions[0].magic = JSMutationEvent::InitMutationEventFuncNum;
    JSMutationEventPrototypeFunctions[0].u.func.length = 8;
    JSMutationEventPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSMutationEventPrototypeFunctions[0].u.func.cfunc.generic_magic = JSMutationEventPrototypeFunction::callAsFunction;
}

JSValue JSMutationEventPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSMutationEvent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventPrototype::self(ctx));
        JSMutationEventPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSMutationEvent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSMutationEventPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSMutationEventAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventAttributesFunctions, countof(JSMutationEventAttributesFunctions));
    init_JSMutationEventPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventPrototypeConstantsFunctions, countof(JSMutationEventPrototypeConstantsFunctions));
    init_JSMutationEventPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMutationEventPrototypeFunctions, countof(JSMutationEventPrototypeFunctions));
}

JSValue JSMutationEventPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSMutationEventClassDefine;
static bool JSMutationEventClassDefine_initialized = false;

static void init_JSMutationEventClassDefine()
{
    if (JSMutationEventClassDefine_initialized) return;
    JSMutationEventClassDefine_initialized = true;
    memset(&JSMutationEventClassDefine, 0, sizeof(JSMutationEventClassDefine));
    JSMutationEventClassDefine.class_name = "MutationEvent";
    JSMutationEventClassDefine.finalizer = JSMutationEvent::finalizer;
    JSMutationEventClassDefine.gc_mark = JSMutationEvent::mark;
}

JSClassID JSMutationEvent::js_class_id = 0;

void JSMutationEvent::init(JSContext* ctx)
{
    if (JSMutationEvent::js_class_id == 0) {
        init_JSMutationEventClassDefine();
        JS_NewClassID(&JSMutationEvent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSMutationEvent::js_class_id, &JSMutationEventClassDefine);
        JS_SetConstructor(ctx, JSMutationEventConstructor::self(ctx), JSMutationEventPrototype::self(ctx));
        JS_SetClassProto(ctx, JSMutationEvent::js_class_id, JSMutationEventPrototype::self(ctx));
    }
}

JSValue JSMutationEvent::create(JSContext* ctx, MutationEvent* impl)
{
    JSMutationEvent::init(ctx);
    JSValue _proto = JSMutationEventPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSMutationEvent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->relatedNode()));
        }
        case PrevValueAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->prevValue()).utf8().data());
        }
        case NewValueAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->newValue()).utf8().data());
        }
        case AttrNameAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->attrName()).utf8().data());
        }
        case AttrChangeAttrNum: {
            MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
            return JS_NewInt32(ctx, imp->attrChange());
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
    MutationEvent* imp = (MutationEvent*)JS_GetOpaque(this_val, JSMutationEvent::js_class_id);
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
