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

#include "QJSHTMLLabelElement.h"

#include "HTMLFormElement.h"
#include "HTMLLabelElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLLabelElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("form", JSHTMLLabelElement::getValueProperty, NULL, JSHTMLLabelElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLLabelElement::getValueProperty, JSHTMLLabelElement::putValueProperty, JSHTMLLabelElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("htmlFor", JSHTMLLabelElement::getValueProperty, JSHTMLLabelElement::putValueProperty, JSHTMLLabelElement::HtmlForAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLLabelElement::getValueProperty, NULL, JSHTMLLabelElement::ConstructorAttrNum)
};

class JSHTMLLabelElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLLabelElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLLabelElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLabelElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLabelElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLabelElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLabelElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLLabelElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLLabelElementPrototypeFunction::callAsFunction, JSHTMLLabelElement::FocusFuncNum)
};

JSValue JSHTMLLabelElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLabelElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLabelElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLabelElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLabelElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLabelElementAttributesFunctions, countof(JSHTMLLabelElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLabelElementPrototypeFunctions, countof(JSHTMLLabelElementPrototypeFunctions));
}

static JSClassDef JSHTMLLabelElementClassDefine = 
{
    "HTMLLabelElement",
    .finalizer = JSHTMLLabelElement::finalizer,
    .gc_mark = JSHTMLLabelElement::mark,
};

JSClassID JSHTMLLabelElement::js_class_id = 0;

void JSHTMLLabelElement::init(JSContext* ctx)
{
    if (JSHTMLLabelElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLLabelElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLLabelElement::js_class_id, &JSHTMLLabelElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLLabelElementConstructor::self(ctx), JSHTMLLabelElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLLabelElement::js_class_id, JSHTMLLabelElementPrototype::self(ctx));
    }
}

JSValue JSHTMLLabelElement::create(JSContext* ctx, HTMLLabelElement* impl)
{
    JSHTMLLabelElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLLabelElementPrototype::self(ctx), JSHTMLLabelElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLabelElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLabelElement* impl = (HTMLLabelElement*)JS_GetOpaque(val, JSHTMLLabelElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLabelElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLabelElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case HtmlForAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->htmlFor()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLabelElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HtmlForAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
            imp->setHtmlFor(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLabelElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLabelElementConstructor::self(ctx);
}

JSValue JSHTMLLabelElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLabelElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLLabelElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
