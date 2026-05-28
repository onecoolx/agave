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


#if ENABLE(XPATH)

#include "QJSXPathEvaluator.h"

#include "QJSCustomXPathNSResolver.h"
#include "QJSNode.h"
#include "QJSXPathExpression.h"
#include "QJSXPathNSResolver.h"
#include "QJSXPathResult.h"
#include "XPathEvaluator.h"
#include "XPathExpression.h"
#include "XPathNSResolver.h"
#include "XPathResult.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSXPathEvaluatorAttributesFunctions[1];
static bool JSXPathEvaluatorAttributesFunctions_initialized = false;

static void init_JSXPathEvaluatorAttributesFunctions()
{
    if (JSXPathEvaluatorAttributesFunctions_initialized) return;
    JSXPathEvaluatorAttributesFunctions_initialized = true;
    memset(JSXPathEvaluatorAttributesFunctions, 0, sizeof(JSXPathEvaluatorAttributesFunctions));
    JSXPathEvaluatorAttributesFunctions[0].name = "constructor";
    JSXPathEvaluatorAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathEvaluatorAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathEvaluatorAttributesFunctions[0].magic = JSXPathEvaluator::ConstructorAttrNum;
    JSXPathEvaluatorAttributesFunctions[0].u.getset.get.getter_magic = JSXPathEvaluator::getValueProperty;
    JSXPathEvaluatorAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSXPathEvaluatorConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv);
};

JSValue JSXPathEvaluatorConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[XPathEvaluator.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewCFunction2(ctx, JSXPathEvaluatorConstructor::construct, "XPathEvaluator", 0, JS_CFUNC_constructor, 0);
        JSXPathEvaluatorConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[XPathEvaluator.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSXPathEvaluatorConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSXPathEvaluatorConstructor::construct(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv)
{
    return toJS(ctx, adoptRef(new XPathEvaluator).get());
}

/* Prototype functions table */

static JSCFunctionListEntry JSXPathEvaluatorPrototypeFunctions[3];
static bool JSXPathEvaluatorPrototypeFunctions_initialized = false;

static void init_JSXPathEvaluatorPrototypeFunctions()
{
    if (JSXPathEvaluatorPrototypeFunctions_initialized) return;
    JSXPathEvaluatorPrototypeFunctions_initialized = true;
    memset(JSXPathEvaluatorPrototypeFunctions, 0, sizeof(JSXPathEvaluatorPrototypeFunctions));
    JSXPathEvaluatorPrototypeFunctions[0].name = "createExpression";
    JSXPathEvaluatorPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathEvaluatorPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSXPathEvaluatorPrototypeFunctions[0].magic = JSXPathEvaluator::CreateExpressionFuncNum;
    JSXPathEvaluatorPrototypeFunctions[0].u.func.length = 2;
    JSXPathEvaluatorPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathEvaluatorPrototypeFunctions[0].u.func.cfunc.generic_magic = JSXPathEvaluatorPrototypeFunction::callAsFunction;
    JSXPathEvaluatorPrototypeFunctions[1].name = "createNSResolver";
    JSXPathEvaluatorPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathEvaluatorPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSXPathEvaluatorPrototypeFunctions[1].magic = JSXPathEvaluator::CreateNSResolverFuncNum;
    JSXPathEvaluatorPrototypeFunctions[1].u.func.length = 1;
    JSXPathEvaluatorPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathEvaluatorPrototypeFunctions[1].u.func.cfunc.generic_magic = JSXPathEvaluatorPrototypeFunction::callAsFunction;
    JSXPathEvaluatorPrototypeFunctions[2].name = "evaluate";
    JSXPathEvaluatorPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathEvaluatorPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSXPathEvaluatorPrototypeFunctions[2].magic = JSXPathEvaluator::EvaluateFuncNum;
    JSXPathEvaluatorPrototypeFunctions[2].u.func.length = 5;
    JSXPathEvaluatorPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathEvaluatorPrototypeFunctions[2].u.func.cfunc.generic_magic = JSXPathEvaluatorPrototypeFunction::callAsFunction;
}

JSValue JSXPathEvaluatorPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXPathEvaluator.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathEvaluatorPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXPathEvaluator.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSXPathEvaluatorPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSXPathEvaluatorAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathEvaluatorAttributesFunctions, countof(JSXPathEvaluatorAttributesFunctions));
    init_JSXPathEvaluatorPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathEvaluatorPrototypeFunctions, countof(JSXPathEvaluatorPrototypeFunctions));
}

static JSClassDef JSXPathEvaluatorClassDefine;
static bool JSXPathEvaluatorClassDefine_initialized = false;

static void init_JSXPathEvaluatorClassDefine()
{
    if (JSXPathEvaluatorClassDefine_initialized) return;
    JSXPathEvaluatorClassDefine_initialized = true;
    memset(&JSXPathEvaluatorClassDefine, 0, sizeof(JSXPathEvaluatorClassDefine));
    JSXPathEvaluatorClassDefine.class_name = "XPathEvaluator";
    JSXPathEvaluatorClassDefine.finalizer = JSXPathEvaluator::finalizer;
    JSXPathEvaluatorClassDefine.gc_mark = JSXPathEvaluator::mark;
}

JSClassID JSXPathEvaluator::js_class_id = 0;

void JSXPathEvaluator::init(JSContext* ctx)
{
    if (JSXPathEvaluator::js_class_id == 0) {
        init_JSXPathEvaluatorClassDefine();
        JS_NewClassID(&JSXPathEvaluator::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXPathEvaluator::js_class_id, &JSXPathEvaluatorClassDefine);
        JS_SetConstructor(ctx, JSXPathEvaluatorConstructor::self(ctx), JSXPathEvaluatorPrototype::self(ctx));
        JS_SetClassProto(ctx, JSXPathEvaluator::js_class_id, JSXPathEvaluatorPrototype::self(ctx));
    }
}

JSValue JSXPathEvaluator::create(JSContext* ctx, XPathEvaluator* impl)
{
    JSXPathEvaluator::init(ctx);
    JSValue _proto = JSXPathEvaluatorPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSXPathEvaluator::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSXPathEvaluator::finalizer(JSRuntime* rt, JSValue val)
{
    XPathEvaluator* impl = (XPathEvaluator*)JS_GetOpaque(val, JSXPathEvaluator::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSXPathEvaluator::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSXPathEvaluator::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSXPathEvaluator::getConstructor(JSContext *ctx)
{
    return JSXPathEvaluatorConstructor::self(ctx);
}

JSValue JSXPathEvaluatorPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XPathEvaluator* imp = (XPathEvaluator*)JS_GetOpaque(this_val, JSXPathEvaluator::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXPathEvaluator::CreateExpressionFuncNum: {
            ExceptionCode ec = 0;
            String expression = valueToString(ctx, argv[0]);
            RefPtr<XPathNSResolver> customResolver;
            XPathNSResolver* resolver = toXPathNSResolver(argv[1]);
            if (!resolver) {
                customResolver = JSCustomXPathNSResolver::create(ctx, argv[1]);
                if (JS_HasException(ctx))
                    return JS_UNDEFINED;
                resolver = customResolver.get();
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->createExpression(expression, resolver, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSXPathEvaluator::CreateNSResolverFuncNum: {
            Node* nodeResolver = toNode(argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createNSResolver(nodeResolver)));
            return result;
        }
        case JSXPathEvaluator::EvaluateFuncNum: {
            ExceptionCode ec = 0;
            String expression = valueToString(ctx, argv[0]);
            Node* contextNode = toNode(argv[1]);
            RefPtr<XPathNSResolver> customResolver;
            XPathNSResolver* resolver = toXPathNSResolver(argv[2]);
            if (!resolver) {
                customResolver = JSCustomXPathNSResolver::create(ctx, argv[2]);
                if (JS_HasException(ctx))
                    return JS_UNDEFINED;
                resolver = customResolver.get();
            }
            unsigned short type = valueToInt32(ctx, argv[3]);
            XPathResult* inResult = toXPathResult(argv[4]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->evaluate(expression, contextNode, resolver, type, inResult, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, XPathEvaluator* obj)
{
    return QJS::cacheDOMObject<XPathEvaluator, JSXPathEvaluator>(ctx, obj);
}

XPathEvaluator* toXPathEvaluator(JSValue val)
{
    if (JS_IsObject(val)) {
        XPathEvaluator* impl = (XPathEvaluator*)JS_GetOpaque(val, JSXPathEvaluator::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(XPATH)
