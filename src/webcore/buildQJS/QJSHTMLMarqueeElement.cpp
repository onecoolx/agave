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

#include "QJSHTMLMarqueeElement.h"

#include "HTMLMarqueeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLMarqueeElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLMarqueeElement::getValueProperty, NULL, JSHTMLMarqueeElement::ConstructorAttrNum)
};

class JSHTMLMarqueeElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLMarqueeElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLMarqueeElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMarqueeElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMarqueeElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMarqueeElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMarqueeElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLMarqueeElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("start", 0, JSHTMLMarqueeElementPrototypeFunction::callAsFunction, JSHTMLMarqueeElement::StartFuncNum),
    JS_CFUNC_MAGIC_DEF("stop", 0, JSHTMLMarqueeElementPrototypeFunction::callAsFunction, JSHTMLMarqueeElement::StopFuncNum)
};

JSValue JSHTMLMarqueeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMarqueeElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMarqueeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMarqueeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMarqueeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMarqueeElementAttributesFunctions, countof(JSHTMLMarqueeElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMarqueeElementPrototypeFunctions, countof(JSHTMLMarqueeElementPrototypeFunctions));
}

static JSClassDef JSHTMLMarqueeElementClassDefine = 
{
    "HTMLMarqueeElement",
    .finalizer = JSHTMLMarqueeElement::finalizer,
    .gc_mark = JSHTMLMarqueeElement::mark,
};

JSClassID JSHTMLMarqueeElement::js_class_id = 0;

void JSHTMLMarqueeElement::init(JSContext* ctx)
{
    if (JSHTMLMarqueeElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLMarqueeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLMarqueeElement::js_class_id, &JSHTMLMarqueeElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLMarqueeElementConstructor::self(ctx), JSHTMLMarqueeElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLMarqueeElement::js_class_id, JSHTMLMarqueeElementPrototype::self(ctx));
    }
}

JSValue JSHTMLMarqueeElement::create(JSContext* ctx, HTMLMarqueeElement* impl)
{
    JSHTMLMarqueeElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLMarqueeElementPrototype::self(ctx), JSHTMLMarqueeElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMarqueeElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMarqueeElement* impl = (HTMLMarqueeElement*)JS_GetOpaque(val, JSHTMLMarqueeElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMarqueeElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMarqueeElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMarqueeElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMarqueeElementConstructor::self(ctx);
}

JSValue JSHTMLMarqueeElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLMarqueeElement* imp = (HTMLMarqueeElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMarqueeElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLMarqueeElement::StartFuncNum: {
            imp->start();
            return JS_UNDEFINED;
        }
        case JSHTMLMarqueeElement::StopFuncNum: {
            imp->stop();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
