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

#include "QJSHTMLPreElement.h"

#include "HTMLPreElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLPreElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("wrap", JSHTMLPreElement::getValueProperty, JSHTMLPreElement::putValueProperty, JSHTMLPreElement::WrapAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLPreElement::getValueProperty, JSHTMLPreElement::putValueProperty, JSHTMLPreElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLPreElement::getValueProperty, NULL, JSHTMLPreElement::ConstructorAttrNum)
};

class JSHTMLPreElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLPreElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLPreElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLPreElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLPreElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLPreElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLPreElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLPreElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLPreElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLPreElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLPreElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLPreElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLPreElementAttributesFunctions, countof(JSHTMLPreElementAttributesFunctions));
}

static JSClassDef JSHTMLPreElementClassDefine = 
{
    "HTMLPreElement",
    .finalizer = JSHTMLPreElement::finalizer,
    .gc_mark = JSHTMLPreElement::mark,
};

JSClassID JSHTMLPreElement::js_class_id = 0;

void JSHTMLPreElement::init(JSContext* ctx)
{
    if (JSHTMLPreElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLPreElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLPreElement::js_class_id, &JSHTMLPreElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLPreElementConstructor::self(ctx), JSHTMLPreElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLPreElement::js_class_id, JSHTMLPreElementPrototype::self(ctx));
    }
}

JSValue JSHTMLPreElement::create(JSContext* ctx, HTMLPreElement* impl)
{
    JSHTMLPreElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLPreElementPrototype::self(ctx), JSHTMLPreElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLPreElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLPreElement* impl = (HTMLPreElement*)JS_GetOpaque(val, JSHTMLPreElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLPreElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLPreElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque2(ctx, this_val, JSHTMLPreElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case WrapAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque2(ctx, this_val, JSHTMLPreElement::js_class_id);
            return JS_NewBool(ctx, imp->wrap() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLPreElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque2(ctx, this_val, JSHTMLPreElement::js_class_id);
            imp->setWidth(valueToInt32(ctx, value));
            break;
        }
        case WrapAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque2(ctx, this_val, JSHTMLPreElement::js_class_id);
            imp->setWrap(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLPreElement::getConstructor(JSContext *ctx)
{
    return JSHTMLPreElementConstructor::self(ctx);
}


}
