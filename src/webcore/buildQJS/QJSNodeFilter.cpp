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

#include "QJSNodeFilter.h"

#include "NodeFilter.h"
#include "QJSNode.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSNodeFilterAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSNodeFilter::getValueProperty, NULL, JSNodeFilter::ConstructorAttrNum)
};

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

static const JSCFunctionListEntry JSNodeFilterConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SHOW_CDATA_SECTION", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_CDATA_SECTION),
    JS_CGETSET_MAGIC_DEF("FILTER_ACCEPT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::FILTER_ACCEPT),
    JS_CGETSET_MAGIC_DEF("FILTER_REJECT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::FILTER_REJECT),
    JS_CGETSET_MAGIC_DEF("SHOW_TEXT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_TEXT),
    JS_CGETSET_MAGIC_DEF("SHOW_COMMENT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_COMMENT),
    JS_CGETSET_MAGIC_DEF("SHOW_ALL", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_ALL),
    JS_CGETSET_MAGIC_DEF("SHOW_ENTITY_REFERENCE", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_ENTITY_REFERENCE),
    JS_CGETSET_MAGIC_DEF("SHOW_NOTATION", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_NOTATION),
    JS_CGETSET_MAGIC_DEF("SHOW_PROCESSING_INSTRUCTION", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_PROCESSING_INSTRUCTION),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT),
    JS_CGETSET_MAGIC_DEF("FILTER_SKIP", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::FILTER_SKIP),
    JS_CGETSET_MAGIC_DEF("SHOW_ELEMENT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_ELEMENT),
    JS_CGETSET_MAGIC_DEF("SHOW_ATTRIBUTE", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_ATTRIBUTE),
    JS_CGETSET_MAGIC_DEF("SHOW_ENTITY", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_ENTITY),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT_TYPE", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT_TYPE),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT_FRAGMENT", JSNodeFilterConstructor::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT_FRAGMENT)
};

JSValue JSNodeFilterConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[NodeFilter.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeFilterConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[NodeFilter.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeFilterConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterConstructorFunctions, countof(JSNodeFilterConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSNodeFilterPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SHOW_CDATA_SECTION", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_CDATA_SECTION),
    JS_CGETSET_MAGIC_DEF("FILTER_ACCEPT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::FILTER_ACCEPT),
    JS_CGETSET_MAGIC_DEF("FILTER_REJECT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::FILTER_REJECT),
    JS_CGETSET_MAGIC_DEF("SHOW_TEXT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_TEXT),
    JS_CGETSET_MAGIC_DEF("SHOW_COMMENT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_COMMENT),
    JS_CGETSET_MAGIC_DEF("SHOW_ALL", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_ALL),
    JS_CGETSET_MAGIC_DEF("SHOW_ENTITY_REFERENCE", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_ENTITY_REFERENCE),
    JS_CGETSET_MAGIC_DEF("SHOW_NOTATION", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_NOTATION),
    JS_CGETSET_MAGIC_DEF("SHOW_PROCESSING_INSTRUCTION", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_PROCESSING_INSTRUCTION),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT),
    JS_CGETSET_MAGIC_DEF("FILTER_SKIP", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::FILTER_SKIP),
    JS_CGETSET_MAGIC_DEF("SHOW_ELEMENT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_ELEMENT),
    JS_CGETSET_MAGIC_DEF("SHOW_ATTRIBUTE", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_ATTRIBUTE),
    JS_CGETSET_MAGIC_DEF("SHOW_ENTITY", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_ENTITY),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT_TYPE", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT_TYPE),
    JS_CGETSET_MAGIC_DEF("SHOW_DOCUMENT_FRAGMENT", JSNodeFilterPrototype::getValueProperty, NULL, NodeFilter::SHOW_DOCUMENT_FRAGMENT)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSNodeFilterPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("acceptNode", 1, JSNodeFilterPrototypeFunction::callAsFunction, JSNodeFilter::AcceptNodeFuncNum)
};

JSValue JSNodeFilterPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeFilter.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeFilterPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeFilter.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeFilterPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterAttributesFunctions, countof(JSNodeFilterAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterPrototypeConstantsFunctions, countof(JSNodeFilterPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeFilterPrototypeFunctions, countof(JSNodeFilterPrototypeFunctions));
}

JSValue JSNodeFilterPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSNodeFilterClassDefine = 
{
    "NodeFilter",
    .finalizer = JSNodeFilter::finalizer,
    .gc_mark = JSNodeFilter::mark,
};

JSClassID JSNodeFilter::js_class_id = 0;

void JSNodeFilter::init(JSContext* ctx)
{
    if (JSNodeFilter::js_class_id == 0) {
        JS_NewClassID(&JSNodeFilter::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeFilter::js_class_id, &JSNodeFilterClassDefine);
        JS_SetConstructor(ctx, JSNodeFilterConstructor::self(ctx), JSNodeFilterPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNodeFilter::js_class_id, JSNodeFilterPrototype::self(ctx));
    }
}

JSValue JSNodeFilter::create(JSContext* ctx, NodeFilter* impl)
{
    JSNodeFilter::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNodeFilterPrototype::self(ctx), JSNodeFilter::js_class_id);
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
    NodeFilter* imp = (NodeFilter*)JS_GetOpaque2(ctx, this_val, JSNodeFilter::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNodeFilter::AcceptNodeFuncNum: {
            Node* n = toNode(argv[0]);

            JSValue result = JS_NewBigUint64(ctx, imp->acceptNode(n));
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
