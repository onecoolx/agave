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


#if ENABLE(XPATH)

#include "QJSXPathNSResolver.h"

#include "QJSCustomXPathNSResolver.h"
#include "QJSXPathNSResolver.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Prototype functions table */

static const JSCFunctionListEntry JSXPathNSResolverPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("lookupNamespaceURI", 1, JSXPathNSResolverPrototypeFunction::callAsFunction, JSXPathNSResolver::LookupNamespaceURIFuncNum)
};

JSValue JSXPathNSResolverPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXPathNSResolver.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathNSResolverPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXPathNSResolver.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSXPathNSResolverPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathNSResolverPrototypeFunctions, countof(JSXPathNSResolverPrototypeFunctions));
}

static JSClassDef JSXPathNSResolverClassDefine = 
{
    "XPathNSResolver",
    .finalizer = JSXPathNSResolver::finalizer,
    .gc_mark = JSXPathNSResolver::mark,
};

JSClassID JSXPathNSResolver::js_class_id = 0;

void JSXPathNSResolver::init(JSContext* ctx)
{
    if (JSXPathNSResolver::js_class_id == 0) {
        JS_NewClassID(&JSXPathNSResolver::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXPathNSResolver::js_class_id, &JSXPathNSResolverClassDefine);
        JS_SetClassProto(ctx, JSXPathNSResolver::js_class_id, JSXPathNSResolverPrototype::self(ctx));
    }
}

JSValue JSXPathNSResolver::create(JSContext* ctx, XPathNSResolver* impl)
{
    JSXPathNSResolver::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSXPathNSResolverPrototype::self(ctx), JSXPathNSResolver::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSXPathNSResolver::finalizer(JSRuntime* rt, JSValue val)
{
    XPathNSResolver* impl = (XPathNSResolver*)JS_GetOpaque(val, JSXPathNSResolver::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSXPathNSResolver::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSXPathNSResolverPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XPathNSResolver* imp = (XPathNSResolver*)JS_GetOpaque2(ctx, this_val, JSXPathNSResolver::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXPathNSResolver::LookupNamespaceURIFuncNum: {
            String prefix = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->lookupNamespaceURI(prefix));
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, XPathNSResolver* obj)
{
    return QJS::cacheDOMObject<XPathNSResolver, JSXPathNSResolver>(ctx, obj);
}

XPathNSResolver* toXPathNSResolver(JSValue val)
{
    if (JS_IsObject(val)) {
        XPathNSResolver* impl = (XPathNSResolver*)JS_GetOpaque(val, JSXPathNSResolver::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(XPATH)
