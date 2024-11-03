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

#include "QJSHTMLParagraphElement.h"

#include "HTMLParagraphElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLParagraphElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("align", JSHTMLParagraphElement::getValueProperty, JSHTMLParagraphElement::putValueProperty, JSHTMLParagraphElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLParagraphElement::getValueProperty, NULL, JSHTMLParagraphElement::ConstructorAttrNum)
};

class JSHTMLParagraphElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLParagraphElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLParagraphElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLParagraphElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLParagraphElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLParagraphElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLParagraphElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLParagraphElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLParagraphElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLParagraphElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLParagraphElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLParagraphElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLParagraphElementAttributesFunctions, countof(JSHTMLParagraphElementAttributesFunctions));
}

static JSClassDef JSHTMLParagraphElementClassDefine = 
{
    "HTMLParagraphElement",
    .finalizer = JSHTMLParagraphElement::finalizer,
    .gc_mark = JSHTMLParagraphElement::mark,
};

JSClassID JSHTMLParagraphElement::js_class_id = 0;

void JSHTMLParagraphElement::init(JSContext* ctx)
{
    if (JSHTMLParagraphElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLParagraphElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLParagraphElement::js_class_id, &JSHTMLParagraphElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLParagraphElementConstructor::self(ctx), JSHTMLParagraphElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLParagraphElement::js_class_id, JSHTMLParagraphElementPrototype::self(ctx));
    }
}

JSValue JSHTMLParagraphElement::create(JSContext* ctx, HTMLParagraphElement* impl)
{
    JSHTMLParagraphElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLParagraphElementPrototype::self(ctx), JSHTMLParagraphElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLParagraphElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLParagraphElement* impl = (HTMLParagraphElement*)JS_GetOpaque(val, JSHTMLParagraphElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLParagraphElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLParagraphElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLParagraphElement* imp = (HTMLParagraphElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParagraphElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLParagraphElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLParagraphElement* imp = (HTMLParagraphElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParagraphElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLParagraphElement::getConstructor(JSContext *ctx)
{
    return JSHTMLParagraphElementConstructor::self(ctx);
}


}
