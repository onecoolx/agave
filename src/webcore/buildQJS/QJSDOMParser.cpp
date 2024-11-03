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

#include "QJSDOMParser.h"

#include "DOMParser.h"
#include "Document.h"
#include "QJSDocument.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSDOMParserAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSDOMParser::getValueProperty, NULL, JSDOMParser::ConstructorAttrNum)
};

class JSDOMParserConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
    static JSValue construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
};

JSValue JSDOMParserConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSDOMParserConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[DOMParser.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewCFunction2(ctx, JSDOMParserConstructor::construct, "DOMParser", 0, JS_CFUNC_constructor, 0);
        JSDOMParserConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DOMParser.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDOMParserConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSDOMParserConstructor::construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)
{
    return toJS(ctx, adoptRef(new DOMParser).get());
}

/* Prototype functions table */

static const JSCFunctionListEntry JSDOMParserPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("parseFromString", 2, JSDOMParserPrototypeFunction::callAsFunction, JSDOMParser::ParseFromStringFuncNum)
};

JSValue JSDOMParserPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMParser.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMParserPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMParser.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDOMParserPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMParserAttributesFunctions, countof(JSDOMParserAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMParserPrototypeFunctions, countof(JSDOMParserPrototypeFunctions));
}

static JSClassDef JSDOMParserClassDefine = 
{
    "DOMParser",
    .finalizer = JSDOMParser::finalizer,
    .gc_mark = JSDOMParser::mark,
};

JSClassID JSDOMParser::js_class_id = 0;

void JSDOMParser::init(JSContext* ctx)
{
    if (JSDOMParser::js_class_id == 0) {
        JS_NewClassID(&JSDOMParser::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMParser::js_class_id, &JSDOMParserClassDefine);
        JS_SetConstructor(ctx, JSDOMParserConstructor::self(ctx), JSDOMParserPrototype::self(ctx));
        JS_SetClassProto(ctx, JSDOMParser::js_class_id, JSDOMParserPrototype::self(ctx));
    }
}

JSValue JSDOMParser::create(JSContext* ctx, DOMParser* impl)
{
    JSDOMParser::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDOMParserPrototype::self(ctx), JSDOMParser::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDOMParser::finalizer(JSRuntime* rt, JSValue val)
{
    DOMParser* impl = (DOMParser*)JS_GetOpaque(val, JSDOMParser::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDOMParser::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSDOMParser::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSDOMParser::getConstructor(JSContext *ctx)
{
    return JSDOMParserConstructor::self(ctx);
}

JSValue JSDOMParserPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMParser* imp = (DOMParser*)JS_GetOpaque2(ctx, this_val, JSDOMParser::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDOMParser::ParseFromStringFuncNum: {
            String str = valueToString(ctx, argv[0]);
            String contentType = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->parseFromString(str, contentType)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, DOMParser* obj)
{
    return QJS::cacheDOMObject<DOMParser, JSDOMParser>(ctx, obj);
}

DOMParser* toDOMParser(JSValue val)
{
    if (JS_IsObject(val)) {
        DOMParser* impl = (DOMParser*)JS_GetOpaque(val, JSDOMParser::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
