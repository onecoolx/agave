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

#include "QJSXPathExpression.h"

#include "QJSNode.h"
#include "QJSXPathResult.h"
#include "XPathExpression.h"
#include "XPathResult.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Prototype functions table */

static JSCFunctionListEntry JSXPathExpressionPrototypeFunctions[1];
static bool JSXPathExpressionPrototypeFunctions_initialized = false;

static void init_JSXPathExpressionPrototypeFunctions()
{
    if (JSXPathExpressionPrototypeFunctions_initialized) return;
    JSXPathExpressionPrototypeFunctions_initialized = true;
    memset(JSXPathExpressionPrototypeFunctions, 0, sizeof(JSXPathExpressionPrototypeFunctions));
    JSXPathExpressionPrototypeFunctions[0].name = "evaluate";
    JSXPathExpressionPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathExpressionPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSXPathExpressionPrototypeFunctions[0].magic = JSXPathExpression::EvaluateFuncNum;
    JSXPathExpressionPrototypeFunctions[0].u.func.length = 3;
    JSXPathExpressionPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathExpressionPrototypeFunctions[0].u.func.cfunc.generic_magic = JSXPathExpressionPrototypeFunction::callAsFunction;
}

JSValue JSXPathExpressionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXPathExpression.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathExpressionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXPathExpression.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSXPathExpressionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSXPathExpressionPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathExpressionPrototypeFunctions, countof(JSXPathExpressionPrototypeFunctions));
}

static JSClassDef JSXPathExpressionClassDefine;
static bool JSXPathExpressionClassDefine_initialized = false;

static void init_JSXPathExpressionClassDefine()
{
    if (JSXPathExpressionClassDefine_initialized) return;
    JSXPathExpressionClassDefine_initialized = true;
    memset(&JSXPathExpressionClassDefine, 0, sizeof(JSXPathExpressionClassDefine));
    JSXPathExpressionClassDefine.class_name = "XPathExpression";
    JSXPathExpressionClassDefine.finalizer = JSXPathExpression::finalizer;
    JSXPathExpressionClassDefine.gc_mark = JSXPathExpression::mark;
}

JSClassID JSXPathExpression::js_class_id = 0;

void JSXPathExpression::init(JSContext* ctx)
{
    if (JSXPathExpression::js_class_id == 0) {
        init_JSXPathExpressionClassDefine();
        JS_NewClassID(&JSXPathExpression::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXPathExpression::js_class_id, &JSXPathExpressionClassDefine);
        JS_SetClassProto(ctx, JSXPathExpression::js_class_id, JSXPathExpressionPrototype::self(ctx));
    }
}

JSValue JSXPathExpression::create(JSContext* ctx, XPathExpression* impl)
{
    JSXPathExpression::init(ctx);
    JSValue _proto = JSXPathExpressionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSXPathExpression::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSXPathExpression::finalizer(JSRuntime* rt, JSValue val)
{
    XPathExpression* impl = (XPathExpression*)JS_GetOpaque(val, JSXPathExpression::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSXPathExpression::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSXPathExpressionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XPathExpression* imp = (XPathExpression*)JS_GetOpaque(this_val, JSXPathExpression::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXPathExpression::EvaluateFuncNum: {
            ExceptionCode ec = 0;
            Node* contextNode = toNode(argv[0]);
            unsigned short type = valueToInt32(ctx, argv[1]);
            XPathResult* inResult = toXPathResult(argv[2]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->evaluate(contextNode, type, inResult, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, XPathExpression* obj)
{
    return QJS::cacheDOMObject<XPathExpression, JSXPathExpression>(ctx, obj);
}

XPathExpression* toXPathExpression(JSValue val)
{
    if (JS_IsObject(val)) {
        XPathExpression* impl = (XPathExpression*)JS_GetOpaque(val, JSXPathExpression::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(XPATH)
