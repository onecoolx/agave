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

#include "QJSNodeFilter.h"

#include "NodeFilter.h"
#include "QJSNode.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSNodeFilterAttributesFunctions[1];
static bool JSNodeFilterAttributesFunctions_initialized = false;

static void init_JSNodeFilterAttributesFunctions()
{
    if (JSNodeFilterAttributesFunctions_initialized) return;
    JSNodeFilterAttributesFunctions_initialized = true;
    memset(JSNodeFilterAttributesFunctions, 0, sizeof(JSNodeFilterAttributesFunctions));
    JSNodeFilterAttributesFunctions[0].name = "constructor";
    JSNodeFilterAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterAttributesFunctions[0].magic = JSNodeFilter::ConstructorAttrNum;
    JSNodeFilterAttributesFunctions[0].u.getset.get.getter_magic = JSNodeFilter::getValueProperty;
    JSNodeFilterAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSNodeFilterConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSNodeFilterConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSNodeFilterConstructorFunctions[16];
static bool JSNodeFilterConstructorFunctions_initialized = false;

static void init_JSNodeFilterConstructorFunctions()
{
    if (JSNodeFilterConstructorFunctions_initialized) return;
    JSNodeFilterConstructorFunctions_initialized = true;
    memset(JSNodeFilterConstructorFunctions, 0, sizeof(JSNodeFilterConstructorFunctions));
    JSNodeFilterConstructorFunctions[0].name = "SHOW_CDATA_SECTION";
    JSNodeFilterConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[0].magic = NodeFilter::SHOW_CDATA_SECTION;
    JSNodeFilterConstructorFunctions[0].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[1].name = "FILTER_ACCEPT";
    JSNodeFilterConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[1].magic = NodeFilter::FILTER_ACCEPT;
    JSNodeFilterConstructorFunctions[1].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[2].name = "FILTER_REJECT";
    JSNodeFilterConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[2].magic = NodeFilter::FILTER_REJECT;
    JSNodeFilterConstructorFunctions[2].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[3].name = "SHOW_TEXT";
    JSNodeFilterConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[3].magic = NodeFilter::SHOW_TEXT;
    JSNodeFilterConstructorFunctions[3].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[4].name = "SHOW_COMMENT";
    JSNodeFilterConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[4].magic = NodeFilter::SHOW_COMMENT;
    JSNodeFilterConstructorFunctions[4].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[5].name = "SHOW_ALL";
    JSNodeFilterConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[5].magic = NodeFilter::SHOW_ALL;
    JSNodeFilterConstructorFunctions[5].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[5].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[6].name = "SHOW_ENTITY_REFERENCE";
    JSNodeFilterConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[6].magic = NodeFilter::SHOW_ENTITY_REFERENCE;
    JSNodeFilterConstructorFunctions[6].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[6].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[7].name = "SHOW_NOTATION";
    JSNodeFilterConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[7].magic = NodeFilter::SHOW_NOTATION;
    JSNodeFilterConstructorFunctions[7].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[7].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[8].name = "SHOW_PROCESSING_INSTRUCTION";
    JSNodeFilterConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[8].magic = NodeFilter::SHOW_PROCESSING_INSTRUCTION;
    JSNodeFilterConstructorFunctions[8].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[8].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[9].name = "SHOW_DOCUMENT";
    JSNodeFilterConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[9].magic = NodeFilter::SHOW_DOCUMENT;
    JSNodeFilterConstructorFunctions[9].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[9].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[10].name = "FILTER_SKIP";
    JSNodeFilterConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[10].magic = NodeFilter::FILTER_SKIP;
    JSNodeFilterConstructorFunctions[10].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[10].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[11].name = "SHOW_ELEMENT";
    JSNodeFilterConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[11].magic = NodeFilter::SHOW_ELEMENT;
    JSNodeFilterConstructorFunctions[11].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[11].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[12].name = "SHOW_ATTRIBUTE";
    JSNodeFilterConstructorFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[12].magic = NodeFilter::SHOW_ATTRIBUTE;
    JSNodeFilterConstructorFunctions[12].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[12].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[13].name = "SHOW_ENTITY";
    JSNodeFilterConstructorFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[13].magic = NodeFilter::SHOW_ENTITY;
    JSNodeFilterConstructorFunctions[13].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[13].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[14].name = "SHOW_DOCUMENT_TYPE";
    JSNodeFilterConstructorFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[14].magic = NodeFilter::SHOW_DOCUMENT_TYPE;
    JSNodeFilterConstructorFunctions[14].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[14].u.getset.set.setter_magic = NULL;
    JSNodeFilterConstructorFunctions[15].name = "SHOW_DOCUMENT_FRAGMENT";
    JSNodeFilterConstructorFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterConstructorFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterConstructorFunctions[15].magic = NodeFilter::SHOW_DOCUMENT_FRAGMENT;
    JSNodeFilterConstructorFunctions[15].u.getset.get.getter_magic = JSNodeFilterConstructor::getValueProperty;
    JSNodeFilterConstructorFunctions[15].u.getset.set.setter_magic = NULL;
}

JSValue JSNodeFilterConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[NodeFilter.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeFilterConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[NodeFilter.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeFilterConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeFilterConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterConstructorFunctions, countof(JSNodeFilterConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSNodeFilterPrototypeConstantsFunctions[16];
static bool JSNodeFilterPrototypeConstantsFunctions_initialized = false;

static void init_JSNodeFilterPrototypeConstantsFunctions()
{
    if (JSNodeFilterPrototypeConstantsFunctions_initialized) return;
    JSNodeFilterPrototypeConstantsFunctions_initialized = true;
    memset(JSNodeFilterPrototypeConstantsFunctions, 0, sizeof(JSNodeFilterPrototypeConstantsFunctions));
    JSNodeFilterPrototypeConstantsFunctions[0].name = "SHOW_CDATA_SECTION";
    JSNodeFilterPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[0].magic = NodeFilter::SHOW_CDATA_SECTION;
    JSNodeFilterPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[1].name = "FILTER_ACCEPT";
    JSNodeFilterPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[1].magic = NodeFilter::FILTER_ACCEPT;
    JSNodeFilterPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[2].name = "FILTER_REJECT";
    JSNodeFilterPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[2].magic = NodeFilter::FILTER_REJECT;
    JSNodeFilterPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[3].name = "SHOW_TEXT";
    JSNodeFilterPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[3].magic = NodeFilter::SHOW_TEXT;
    JSNodeFilterPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[4].name = "SHOW_COMMENT";
    JSNodeFilterPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[4].magic = NodeFilter::SHOW_COMMENT;
    JSNodeFilterPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[5].name = "SHOW_ALL";
    JSNodeFilterPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[5].magic = NodeFilter::SHOW_ALL;
    JSNodeFilterPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[6].name = "SHOW_ENTITY_REFERENCE";
    JSNodeFilterPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[6].magic = NodeFilter::SHOW_ENTITY_REFERENCE;
    JSNodeFilterPrototypeConstantsFunctions[6].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[6].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[7].name = "SHOW_NOTATION";
    JSNodeFilterPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[7].magic = NodeFilter::SHOW_NOTATION;
    JSNodeFilterPrototypeConstantsFunctions[7].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[7].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[8].name = "SHOW_PROCESSING_INSTRUCTION";
    JSNodeFilterPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[8].magic = NodeFilter::SHOW_PROCESSING_INSTRUCTION;
    JSNodeFilterPrototypeConstantsFunctions[8].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[8].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[9].name = "SHOW_DOCUMENT";
    JSNodeFilterPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[9].magic = NodeFilter::SHOW_DOCUMENT;
    JSNodeFilterPrototypeConstantsFunctions[9].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[9].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[10].name = "FILTER_SKIP";
    JSNodeFilterPrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[10].magic = NodeFilter::FILTER_SKIP;
    JSNodeFilterPrototypeConstantsFunctions[10].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[10].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[11].name = "SHOW_ELEMENT";
    JSNodeFilterPrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[11].magic = NodeFilter::SHOW_ELEMENT;
    JSNodeFilterPrototypeConstantsFunctions[11].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[11].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[12].name = "SHOW_ATTRIBUTE";
    JSNodeFilterPrototypeConstantsFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[12].magic = NodeFilter::SHOW_ATTRIBUTE;
    JSNodeFilterPrototypeConstantsFunctions[12].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[12].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[13].name = "SHOW_ENTITY";
    JSNodeFilterPrototypeConstantsFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[13].magic = NodeFilter::SHOW_ENTITY;
    JSNodeFilterPrototypeConstantsFunctions[13].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[13].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[14].name = "SHOW_DOCUMENT_TYPE";
    JSNodeFilterPrototypeConstantsFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[14].magic = NodeFilter::SHOW_DOCUMENT_TYPE;
    JSNodeFilterPrototypeConstantsFunctions[14].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[14].u.getset.set.setter_magic = NULL;
    JSNodeFilterPrototypeConstantsFunctions[15].name = "SHOW_DOCUMENT_FRAGMENT";
    JSNodeFilterPrototypeConstantsFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeConstantsFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeFilterPrototypeConstantsFunctions[15].magic = NodeFilter::SHOW_DOCUMENT_FRAGMENT;
    JSNodeFilterPrototypeConstantsFunctions[15].u.getset.get.getter_magic = JSNodeFilterPrototype::getValueProperty;
    JSNodeFilterPrototypeConstantsFunctions[15].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSNodeFilterPrototypeFunctions[1];
static bool JSNodeFilterPrototypeFunctions_initialized = false;

static void init_JSNodeFilterPrototypeFunctions()
{
    if (JSNodeFilterPrototypeFunctions_initialized) return;
    JSNodeFilterPrototypeFunctions_initialized = true;
    memset(JSNodeFilterPrototypeFunctions, 0, sizeof(JSNodeFilterPrototypeFunctions));
    JSNodeFilterPrototypeFunctions[0].name = "acceptNode";
    JSNodeFilterPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodeFilterPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSNodeFilterPrototypeFunctions[0].magic = JSNodeFilter::AcceptNodeFuncNum;
    JSNodeFilterPrototypeFunctions[0].u.func.length = 1;
    JSNodeFilterPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodeFilterPrototypeFunctions[0].u.func.cfunc.generic_magic = JSNodeFilterPrototypeFunction::callAsFunction;
}

JSValue JSNodeFilterPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeFilter.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeFilterPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeFilter.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeFilterPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeFilterAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterAttributesFunctions, countof(JSNodeFilterAttributesFunctions));
    init_JSNodeFilterPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterPrototypeConstantsFunctions, countof(JSNodeFilterPrototypeConstantsFunctions));
    init_JSNodeFilterPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterPrototypeFunctions, countof(JSNodeFilterPrototypeFunctions));
}

JSValue JSNodeFilterPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSNodeFilterClassDefine;
static bool JSNodeFilterClassDefine_initialized = false;

static void init_JSNodeFilterClassDefine()
{
    if (JSNodeFilterClassDefine_initialized) return;
    JSNodeFilterClassDefine_initialized = true;
    memset(&JSNodeFilterClassDefine, 0, sizeof(JSNodeFilterClassDefine));
    JSNodeFilterClassDefine.class_name = "NodeFilter";
    JSNodeFilterClassDefine.finalizer = JSNodeFilter::finalizer;
    JSNodeFilterClassDefine.gc_mark = JSNodeFilter::mark;
}

JSClassID JSNodeFilter::js_class_id = 0;

void JSNodeFilter::init(JSContext* ctx)
{
    if (JSNodeFilter::js_class_id == 0) {
        init_JSNodeFilterClassDefine();
        JS_NewClassID(&JSNodeFilter::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeFilter::js_class_id, &JSNodeFilterClassDefine);
        JS_SetConstructor(ctx, JSNodeFilterConstructor::self(ctx), JSNodeFilterPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNodeFilter::js_class_id, JSNodeFilterPrototype::self(ctx));
    }
}

JSValue JSNodeFilter::create(JSContext* ctx, NodeFilter* impl)
{
    JSNodeFilter::init(ctx);
    JSValue _proto = JSNodeFilterPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNodeFilter::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNodeFilter::finalizer(JSRuntime* rt, JSValue val)
{
    NodeFilter* impl = (NodeFilter*)JS_GetOpaque(val, JSNodeFilter::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSNodeFilter::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSNodeFilter::getConstructor(JSContext *ctx)
{
    return JSNodeFilterConstructor::self(ctx);
}

JSValue JSNodeFilterPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    NodeFilter* imp = (NodeFilter*)JS_GetOpaque(this_val, JSNodeFilter::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNodeFilter::AcceptNodeFuncNum: {
            Node* n = toNode(argv[0]);

            JSValue result = JS_NewInt32(ctx, imp->acceptNode(n));
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, NodeFilter* obj)
{
    return QJS::cacheDOMObject<NodeFilter, JSNodeFilter>(ctx, obj);
}

}
