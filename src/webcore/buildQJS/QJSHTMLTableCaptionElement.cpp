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

#include "QJSHTMLTableCaptionElement.h"

#include "HTMLTableCaptionElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTableCaptionElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableCaptionElement::getValueProperty, JSHTMLTableCaptionElement::putValueProperty, JSHTMLTableCaptionElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableCaptionElement::getValueProperty, NULL, JSHTMLTableCaptionElement::ConstructorAttrNum)
};

class JSHTMLTableCaptionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableCaptionElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableCaptionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableCaptionElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableCaptionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableCaptionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableCaptionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableCaptionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableCaptionElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableCaptionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableCaptionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableCaptionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableCaptionElementAttributesFunctions, countof(JSHTMLTableCaptionElementAttributesFunctions));
}

static JSClassDef JSHTMLTableCaptionElementClassDefine = 
{
    "HTMLTableCaptionElement",
    .finalizer = JSHTMLTableCaptionElement::finalizer,
    .gc_mark = JSHTMLTableCaptionElement::mark,
};

JSClassID JSHTMLTableCaptionElement::js_class_id = 0;

void JSHTMLTableCaptionElement::init(JSContext* ctx)
{
    if (JSHTMLTableCaptionElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableCaptionElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableCaptionElement::js_class_id, &JSHTMLTableCaptionElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableCaptionElementConstructor::self(ctx), JSHTMLTableCaptionElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableCaptionElement::js_class_id, JSHTMLTableCaptionElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableCaptionElement::create(JSContext* ctx, HTMLTableCaptionElement* impl)
{
    JSHTMLTableCaptionElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableCaptionElementPrototype::self(ctx), JSHTMLTableCaptionElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableCaptionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableCaptionElement* impl = (HTMLTableCaptionElement*)JS_GetOpaque(val, JSHTMLTableCaptionElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableCaptionElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableCaptionElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableCaptionElement* imp = (HTMLTableCaptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCaptionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableCaptionElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableCaptionElement* imp = (HTMLTableCaptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCaptionElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableCaptionElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableCaptionElementConstructor::self(ctx);
}


HTMLTableCaptionElement* toHTMLTableCaptionElement(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLTableCaptionElement* impl = (HTMLTableCaptionElement*)JS_GetOpaque(val, JSHTMLTableCaptionElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
