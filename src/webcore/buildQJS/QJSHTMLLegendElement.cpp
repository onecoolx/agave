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

#include "QJSHTMLLegendElement.h"

#include "HTMLFormElement.h"
#include "HTMLLegendElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLLegendElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("form", JSHTMLLegendElement::getValueProperty, NULL, JSHTMLLegendElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLLegendElement::getValueProperty, JSHTMLLegendElement::putValueProperty, JSHTMLLegendElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLLegendElement::getValueProperty, JSHTMLLegendElement::putValueProperty, JSHTMLLegendElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLLegendElement::getValueProperty, NULL, JSHTMLLegendElement::ConstructorAttrNum)
};

class JSHTMLLegendElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLLegendElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLLegendElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLegendElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLegendElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLegendElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLegendElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLLegendElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLLegendElementPrototypeFunction::callAsFunction, JSHTMLLegendElement::FocusFuncNum)
};

JSValue JSHTMLLegendElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLegendElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLegendElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLegendElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLegendElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLegendElementAttributesFunctions, countof(JSHTMLLegendElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLegendElementPrototypeFunctions, countof(JSHTMLLegendElementPrototypeFunctions));
}

static JSClassDef JSHTMLLegendElementClassDefine = 
{
    "HTMLLegendElement",
    .finalizer = JSHTMLLegendElement::finalizer,
    .gc_mark = JSHTMLLegendElement::mark,
};

JSClassID JSHTMLLegendElement::js_class_id = 0;

void JSHTMLLegendElement::init(JSContext* ctx)
{
    if (JSHTMLLegendElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLLegendElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLLegendElement::js_class_id, &JSHTMLLegendElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLLegendElementConstructor::self(ctx), JSHTMLLegendElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLLegendElement::js_class_id, JSHTMLLegendElementPrototype::self(ctx));
    }
}

JSValue JSHTMLLegendElement::create(JSContext* ctx, HTMLLegendElement* impl)
{
    JSHTMLLegendElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLLegendElementPrototype::self(ctx), JSHTMLLegendElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLegendElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLegendElement* impl = (HTMLLegendElement*)JS_GetOpaque(val, JSHTMLLegendElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLegendElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLegendElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLegendElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLegendElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLegendElementConstructor::self(ctx);
}

JSValue JSHTMLLegendElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLegendElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLLegendElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
