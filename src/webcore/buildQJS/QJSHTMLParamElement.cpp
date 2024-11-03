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

#include "QJSHTMLParamElement.h"

#include "HTMLParamElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLParamElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("name", JSHTMLParamElement::getValueProperty, JSHTMLParamElement::putValueProperty, JSHTMLParamElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("valueType", JSHTMLParamElement::getValueProperty, JSHTMLParamElement::putValueProperty, JSHTMLParamElement::ValueTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLParamElement::getValueProperty, JSHTMLParamElement::putValueProperty, JSHTMLParamElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLParamElement::getValueProperty, JSHTMLParamElement::putValueProperty, JSHTMLParamElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLParamElement::getValueProperty, NULL, JSHTMLParamElement::ConstructorAttrNum)
};

class JSHTMLParamElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLParamElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLParamElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLParamElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLParamElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLParamElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLParamElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLParamElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLParamElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLParamElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLParamElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLParamElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLParamElementAttributesFunctions, countof(JSHTMLParamElementAttributesFunctions));
}

static JSClassDef JSHTMLParamElementClassDefine = 
{
    "HTMLParamElement",
    .finalizer = JSHTMLParamElement::finalizer,
    .gc_mark = JSHTMLParamElement::mark,
};

JSClassID JSHTMLParamElement::js_class_id = 0;

void JSHTMLParamElement::init(JSContext* ctx)
{
    if (JSHTMLParamElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLParamElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLParamElement::js_class_id, &JSHTMLParamElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLParamElementConstructor::self(ctx), JSHTMLParamElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLParamElement::js_class_id, JSHTMLParamElementPrototype::self(ctx));
    }
}

JSValue JSHTMLParamElement::create(JSContext* ctx, HTMLParamElement* impl)
{
    JSHTMLParamElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLParamElementPrototype::self(ctx), JSHTMLParamElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLParamElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLParamElement* impl = (HTMLParamElement*)JS_GetOpaque(val, JSHTMLParamElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLParamElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLParamElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case ValueTypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->valueType()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLParamElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueTypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque2(ctx, this_val, JSHTMLParamElement::js_class_id);
            imp->setValueType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLParamElement::getConstructor(JSContext *ctx)
{
    return JSHTMLParamElementConstructor::self(ctx);
}


}
