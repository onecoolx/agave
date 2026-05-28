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

#include "QJSHTMLMarqueeElement.h"

#include "HTMLMarqueeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLMarqueeElementAttributesFunctions[1];
static bool JSHTMLMarqueeElementAttributesFunctions_initialized = false;

static void init_JSHTMLMarqueeElementAttributesFunctions()
{
    if (JSHTMLMarqueeElementAttributesFunctions_initialized) return;
    JSHTMLMarqueeElementAttributesFunctions_initialized = true;
    memset(JSHTMLMarqueeElementAttributesFunctions, 0, sizeof(JSHTMLMarqueeElementAttributesFunctions));
    JSHTMLMarqueeElementAttributesFunctions[0].name = "constructor";
    JSHTMLMarqueeElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMarqueeElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMarqueeElementAttributesFunctions[0].magic = JSHTMLMarqueeElement::ConstructorAttrNum;
    JSHTMLMarqueeElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLMarqueeElement::getValueProperty;
    JSHTMLMarqueeElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSHTMLMarqueeElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLMarqueeElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMarqueeElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMarqueeElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMarqueeElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMarqueeElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLMarqueeElementPrototypeFunctions[2];
static bool JSHTMLMarqueeElementPrototypeFunctions_initialized = false;

static void init_JSHTMLMarqueeElementPrototypeFunctions()
{
    if (JSHTMLMarqueeElementPrototypeFunctions_initialized) return;
    JSHTMLMarqueeElementPrototypeFunctions_initialized = true;
    memset(JSHTMLMarqueeElementPrototypeFunctions, 0, sizeof(JSHTMLMarqueeElementPrototypeFunctions));
    JSHTMLMarqueeElementPrototypeFunctions[0].name = "start";
    JSHTMLMarqueeElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLMarqueeElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLMarqueeElementPrototypeFunctions[0].magic = JSHTMLMarqueeElement::StartFuncNum;
    JSHTMLMarqueeElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLMarqueeElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLMarqueeElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLMarqueeElementPrototypeFunction::callAsFunction;
    JSHTMLMarqueeElementPrototypeFunctions[1].name = "stop";
    JSHTMLMarqueeElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLMarqueeElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLMarqueeElementPrototypeFunctions[1].magic = JSHTMLMarqueeElement::StopFuncNum;
    JSHTMLMarqueeElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLMarqueeElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLMarqueeElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLMarqueeElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLMarqueeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMarqueeElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMarqueeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMarqueeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMarqueeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLMarqueeElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMarqueeElementAttributesFunctions, countof(JSHTMLMarqueeElementAttributesFunctions));
    init_JSHTMLMarqueeElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMarqueeElementPrototypeFunctions, countof(JSHTMLMarqueeElementPrototypeFunctions));
}

JSClassID JSHTMLMarqueeElement::js_class_id = 0;

void JSHTMLMarqueeElement::init(JSContext* ctx)
{
    if (JSHTMLMarqueeElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLMarqueeElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLMarqueeElement::create(JSContext* ctx, HTMLMarqueeElement* impl)
{
    JSHTMLMarqueeElement::init(ctx);
    JSValue _proto = JSHTMLMarqueeElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMarqueeElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMarqueeElement* impl = (HTMLMarqueeElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
    HTMLMarqueeElement* imp = (HTMLMarqueeElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
