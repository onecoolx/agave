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

#include "QJSHTMLLIElement.h"

#include "HTMLLIElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLLIElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLLIElement::getValueProperty, NULL, JSHTMLLIElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLLIElement::getValueProperty, JSHTMLLIElement::putValueProperty, JSHTMLLIElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLLIElement::getValueProperty, JSHTMLLIElement::putValueProperty, JSHTMLLIElement::ValueAttrNum)
};

class JSHTMLLIElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLLIElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLLIElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLIElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLIElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLIElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLIElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLLIElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLIElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLIElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLIElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLIElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLIElementAttributesFunctions, countof(JSHTMLLIElementAttributesFunctions));
}

static JSClassDef JSHTMLLIElementClassDefine = 
{
    "HTMLLIElement",
    .finalizer = JSHTMLLIElement::finalizer,
    .gc_mark = JSHTMLLIElement::mark,
};

JSClassID JSHTMLLIElement::js_class_id = 0;

void JSHTMLLIElement::init(JSContext* ctx)
{
    if (JSHTMLLIElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLLIElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLLIElement::js_class_id, &JSHTMLLIElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLLIElementConstructor::self(ctx), JSHTMLLIElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLLIElement::js_class_id, JSHTMLLIElementPrototype::self(ctx));
    }
}

JSValue JSHTMLLIElement::create(JSContext* ctx, HTMLLIElement* impl)
{
    JSHTMLLIElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLLIElementPrototype::self(ctx), JSHTMLLIElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLIElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLIElement* impl = (HTMLLIElement*)JS_GetOpaque(val, JSHTMLLIElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLIElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLIElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            HTMLLIElement* imp = (HTMLLIElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLIElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLLIElement* imp = (HTMLLIElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLIElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->value());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLIElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TypeAttrNum: {
            HTMLLIElement* imp = (HTMLLIElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLIElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLLIElement* imp = (HTMLLIElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLIElement::js_class_id);
            imp->setValue(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLIElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLIElementConstructor::self(ctx);
}


}
