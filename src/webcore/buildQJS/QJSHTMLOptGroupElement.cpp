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

#include "QJSHTMLOptGroupElement.h"

#include "HTMLOptGroupElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLOptGroupElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLOptGroupElement::getValueProperty, NULL, JSHTMLOptGroupElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLOptGroupElement::getValueProperty, JSHTMLOptGroupElement::putValueProperty, JSHTMLOptGroupElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("label", JSHTMLOptGroupElement::getValueProperty, JSHTMLOptGroupElement::putValueProperty, JSHTMLOptGroupElement::LabelAttrNum)
};

class JSHTMLOptGroupElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLOptGroupElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLOptGroupElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLOptGroupElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLOptGroupElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLOptGroupElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLOptGroupElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLOptGroupElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOptGroupElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLOptGroupElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOptGroupElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLOptGroupElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptGroupElementAttributesFunctions, countof(JSHTMLOptGroupElementAttributesFunctions));
}

static JSClassDef JSHTMLOptGroupElementClassDefine = 
{
    "HTMLOptGroupElement",
    .finalizer = JSHTMLOptGroupElement::finalizer,
    .gc_mark = JSHTMLOptGroupElement::mark,
};

JSClassID JSHTMLOptGroupElement::js_class_id = 0;

void JSHTMLOptGroupElement::init(JSContext* ctx)
{
    if (JSHTMLOptGroupElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLOptGroupElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLOptGroupElement::js_class_id, &JSHTMLOptGroupElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLOptGroupElementConstructor::self(ctx), JSHTMLOptGroupElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLOptGroupElement::js_class_id, JSHTMLOptGroupElementPrototype::self(ctx));
    }
}

JSValue JSHTMLOptGroupElement::create(JSContext* ctx, HTMLOptGroupElement* impl)
{
    JSHTMLOptGroupElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLOptGroupElementPrototype::self(ctx), JSHTMLOptGroupElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOptGroupElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOptGroupElement* impl = (HTMLOptGroupElement*)JS_GetOpaque(val, JSHTMLOptGroupElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLOptGroupElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLOptGroupElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLOptGroupElement* imp = (HTMLOptGroupElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptGroupElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case LabelAttrNum: {
            HTMLOptGroupElement* imp = (HTMLOptGroupElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptGroupElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->label()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLOptGroupElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLOptGroupElement* imp = (HTMLOptGroupElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptGroupElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case LabelAttrNum: {
            HTMLOptGroupElement* imp = (HTMLOptGroupElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptGroupElement::js_class_id);
            imp->setLabel(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLOptGroupElement::getConstructor(JSContext *ctx)
{
    return JSHTMLOptGroupElementConstructor::self(ctx);
}


}
