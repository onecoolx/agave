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

#include "QJSHTMLBlockquoteElement.h"

#include "HTMLBlockquoteElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLBlockquoteElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cite", JSHTMLBlockquoteElement::getValueProperty, JSHTMLBlockquoteElement::putValueProperty, JSHTMLBlockquoteElement::CiteAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLBlockquoteElement::getValueProperty, NULL, JSHTMLBlockquoteElement::ConstructorAttrNum)
};

class JSHTMLBlockquoteElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLBlockquoteElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLBlockquoteElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBlockquoteElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBlockquoteElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBlockquoteElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBlockquoteElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBlockquoteElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBlockquoteElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBlockquoteElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBlockquoteElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBlockquoteElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBlockquoteElementAttributesFunctions, countof(JSHTMLBlockquoteElementAttributesFunctions));
}

static JSClassDef JSHTMLBlockquoteElementClassDefine = 
{
    "HTMLBlockquoteElement",
    .finalizer = JSHTMLBlockquoteElement::finalizer,
    .gc_mark = JSHTMLBlockquoteElement::mark,
};

JSClassID JSHTMLBlockquoteElement::js_class_id = 0;

void JSHTMLBlockquoteElement::init(JSContext* ctx)
{
    if (JSHTMLBlockquoteElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLBlockquoteElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLBlockquoteElement::js_class_id, &JSHTMLBlockquoteElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLBlockquoteElementConstructor::self(ctx), JSHTMLBlockquoteElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLBlockquoteElement::js_class_id, JSHTMLBlockquoteElementPrototype::self(ctx));
    }
}

JSValue JSHTMLBlockquoteElement::create(JSContext* ctx, HTMLBlockquoteElement* impl)
{
    JSHTMLBlockquoteElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLBlockquoteElementPrototype::self(ctx), JSHTMLBlockquoteElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBlockquoteElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBlockquoteElement* impl = (HTMLBlockquoteElement*)JS_GetOpaque(val, JSHTMLBlockquoteElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBlockquoteElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBlockquoteElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLBlockquoteElement* imp = (HTMLBlockquoteElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBlockquoteElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cite()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBlockquoteElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLBlockquoteElement* imp = (HTMLBlockquoteElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBlockquoteElement::js_class_id);
            imp->setCite(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBlockquoteElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBlockquoteElementConstructor::self(ctx);
}


}
