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

#include "QJSHTMLMetaElement.h"

#include "HTMLMetaElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLMetaElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("name", JSHTMLMetaElement::getValueProperty, JSHTMLMetaElement::putValueProperty, JSHTMLMetaElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("httpEquiv", JSHTMLMetaElement::getValueProperty, JSHTMLMetaElement::putValueProperty, JSHTMLMetaElement::HttpEquivAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLMetaElement::getValueProperty, NULL, JSHTMLMetaElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("content", JSHTMLMetaElement::getValueProperty, JSHTMLMetaElement::putValueProperty, JSHTMLMetaElement::ContentAttrNum),
    JS_CGETSET_MAGIC_DEF("scheme", JSHTMLMetaElement::getValueProperty, JSHTMLMetaElement::putValueProperty, JSHTMLMetaElement::SchemeAttrNum)
};

class JSHTMLMetaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLMetaElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLMetaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMetaElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMetaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMetaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMetaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLMetaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMetaElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMetaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMetaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMetaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMetaElementAttributesFunctions, countof(JSHTMLMetaElementAttributesFunctions));
}

static JSClassDef JSHTMLMetaElementClassDefine = 
{
    "HTMLMetaElement",
    .finalizer = JSHTMLMetaElement::finalizer,
    .gc_mark = JSHTMLMetaElement::mark,
};

JSClassID JSHTMLMetaElement::js_class_id = 0;

void JSHTMLMetaElement::init(JSContext* ctx)
{
    if (JSHTMLMetaElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLMetaElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLMetaElement::js_class_id, &JSHTMLMetaElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLMetaElementConstructor::self(ctx), JSHTMLMetaElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLMetaElement::js_class_id, JSHTMLMetaElementPrototype::self(ctx));
    }
}

JSValue JSHTMLMetaElement::create(JSContext* ctx, HTMLMetaElement* impl)
{
    JSHTMLMetaElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLMetaElementPrototype::self(ctx), JSHTMLMetaElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMetaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMetaElement* impl = (HTMLMetaElement*)JS_GetOpaque(val, JSHTMLMetaElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMetaElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMetaElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ContentAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->content()).utf8().data());
        }
        case HttpEquivAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->httpEquiv()).utf8().data());
        }
        case NameAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SchemeAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scheme()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMetaElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ContentAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            imp->setContent(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HttpEquivAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            imp->setHttpEquiv(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SchemeAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMetaElement::js_class_id);
            imp->setScheme(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLMetaElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMetaElementConstructor::self(ctx);
}


}
