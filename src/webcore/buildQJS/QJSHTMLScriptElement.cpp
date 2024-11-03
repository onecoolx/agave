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

#include "QJSHTMLScriptElement.h"

#include "HTMLScriptElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLScriptElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("event", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::EventAttrNum),
    JS_CGETSET_MAGIC_DEF("htmlFor", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::HtmlForAttrNum),
    JS_CGETSET_MAGIC_DEF("charset", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::CharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::SrcAttrNum),
    JS_CGETSET_MAGIC_DEF("defer", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::DeferAttrNum),
    JS_CGETSET_MAGIC_DEF("text", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::TextAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLScriptElement::getValueProperty, JSHTMLScriptElement::putValueProperty, JSHTMLScriptElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLScriptElement::getValueProperty, NULL, JSHTMLScriptElement::ConstructorAttrNum)
};

class JSHTMLScriptElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLScriptElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLScriptElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLScriptElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLScriptElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLScriptElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLScriptElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLScriptElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLScriptElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLScriptElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLScriptElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLScriptElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLScriptElementAttributesFunctions, countof(JSHTMLScriptElementAttributesFunctions));
}

static JSClassDef JSHTMLScriptElementClassDefine = 
{
    "HTMLScriptElement",
    .finalizer = JSHTMLScriptElement::finalizer,
    .gc_mark = JSHTMLScriptElement::mark,
};

JSClassID JSHTMLScriptElement::js_class_id = 0;

void JSHTMLScriptElement::init(JSContext* ctx)
{
    if (JSHTMLScriptElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLScriptElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLScriptElement::js_class_id, &JSHTMLScriptElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLScriptElementConstructor::self(ctx), JSHTMLScriptElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLScriptElement::js_class_id, JSHTMLScriptElementPrototype::self(ctx));
    }
}

JSValue JSHTMLScriptElement::create(JSContext* ctx, HTMLScriptElement* impl)
{
    JSHTMLScriptElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLScriptElementPrototype::self(ctx), JSHTMLScriptElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLScriptElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLScriptElement* impl = (HTMLScriptElement*)JS_GetOpaque(val, JSHTMLScriptElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLScriptElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLScriptElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case HtmlForAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->htmlFor()).utf8().data());
        }
        case EventAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->event()).utf8().data());
        }
        case CharsetAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case DeferAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewBool(ctx, imp->defer() ? 1 : 0);
        }
        case SrcAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLScriptElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HtmlForAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setHtmlFor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EventAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setEvent(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DeferAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setDefer(valueToBoolean(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque2(ctx, this_val, JSHTMLScriptElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLScriptElement::getConstructor(JSContext *ctx)
{
    return JSHTMLScriptElementConstructor::self(ctx);
}


}
