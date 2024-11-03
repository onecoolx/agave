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

#include "QJSXMLSerializer.h"

#include "QJSNode.h"
#include "XMLSerializer.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSXMLSerializerAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSXMLSerializer::getValueProperty, NULL, JSXMLSerializer::ConstructorAttrNum)
};

class JSXMLSerializerConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
    static JSValue construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
};

JSValue JSXMLSerializerConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSXMLSerializerConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[XMLSerializer.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewCFunction2(ctx, JSXMLSerializerConstructor::construct, "XMLSerializer", 0, JS_CFUNC_constructor, 0);
        JSXMLSerializerConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[XMLSerializer.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSXMLSerializerConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSXMLSerializerConstructor::construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)
{
    return toJS(ctx, adoptRef(new XMLSerializer).get());
}

/* Prototype functions table */

static const JSCFunctionListEntry JSXMLSerializerPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("serializeToString", 1, JSXMLSerializerPrototypeFunction::callAsFunction, JSXMLSerializer::SerializeToStringFuncNum)
};

JSValue JSXMLSerializerPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXMLSerializer.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSXMLSerializerPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXMLSerializer.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSXMLSerializerPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSXMLSerializerAttributesFunctions, countof(JSXMLSerializerAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSXMLSerializerPrototypeFunctions, countof(JSXMLSerializerPrototypeFunctions));
}

static JSClassDef JSXMLSerializerClassDefine = 
{
    "XMLSerializer",
    .finalizer = JSXMLSerializer::finalizer,
    .gc_mark = JSXMLSerializer::mark,
};

JSClassID JSXMLSerializer::js_class_id = 0;

void JSXMLSerializer::init(JSContext* ctx)
{
    if (JSXMLSerializer::js_class_id == 0) {
        JS_NewClassID(&JSXMLSerializer::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXMLSerializer::js_class_id, &JSXMLSerializerClassDefine);
        JS_SetConstructor(ctx, JSXMLSerializerConstructor::self(ctx), JSXMLSerializerPrototype::self(ctx));
        JS_SetClassProto(ctx, JSXMLSerializer::js_class_id, JSXMLSerializerPrototype::self(ctx));
    }
}

JSValue JSXMLSerializer::create(JSContext* ctx, XMLSerializer* impl)
{
    JSXMLSerializer::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSXMLSerializerPrototype::self(ctx), JSXMLSerializer::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSXMLSerializer::finalizer(JSRuntime* rt, JSValue val)
{
    XMLSerializer* impl = (XMLSerializer*)JS_GetOpaque(val, JSXMLSerializer::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSXMLSerializer::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSXMLSerializer::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSXMLSerializer::getConstructor(JSContext *ctx)
{
    return JSXMLSerializerConstructor::self(ctx);
}

JSValue JSXMLSerializerPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XMLSerializer* imp = (XMLSerializer*)JS_GetOpaque2(ctx, this_val, JSXMLSerializer::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXMLSerializer::SerializeToStringFuncNum: {
            ExceptionCode ec = 0;
            Node* node = toNode(argv[0]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->serializeToString(node, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, XMLSerializer* obj)
{
    return QJS::cacheDOMObject<XMLSerializer, JSXMLSerializer>(ctx, obj);
}

XMLSerializer* toXMLSerializer(JSValue val)
{
    if (JS_IsObject(val)) {
        XMLSerializer* impl = (XMLSerializer*)JS_GetOpaque(val, JSXMLSerializer::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
