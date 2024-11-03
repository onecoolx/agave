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

#include "QJSHTMLBaseFontElement.h"

#include "HTMLBaseFontElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLBaseFontElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("size", JSHTMLBaseFontElement::getValueProperty, JSHTMLBaseFontElement::putValueProperty, JSHTMLBaseFontElement::SizeAttrNum),
    JS_CGETSET_MAGIC_DEF("color", JSHTMLBaseFontElement::getValueProperty, JSHTMLBaseFontElement::putValueProperty, JSHTMLBaseFontElement::ColorAttrNum),
    JS_CGETSET_MAGIC_DEF("face", JSHTMLBaseFontElement::getValueProperty, JSHTMLBaseFontElement::putValueProperty, JSHTMLBaseFontElement::FaceAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLBaseFontElement::getValueProperty, NULL, JSHTMLBaseFontElement::ConstructorAttrNum)
};

class JSHTMLBaseFontElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLBaseFontElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLBaseFontElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBaseFontElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBaseFontElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBaseFontElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBaseFontElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBaseFontElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBaseFontElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBaseFontElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBaseFontElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBaseFontElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBaseFontElementAttributesFunctions, countof(JSHTMLBaseFontElementAttributesFunctions));
}

static JSClassDef JSHTMLBaseFontElementClassDefine = 
{
    "HTMLBaseFontElement",
    .finalizer = JSHTMLBaseFontElement::finalizer,
    .gc_mark = JSHTMLBaseFontElement::mark,
};

JSClassID JSHTMLBaseFontElement::js_class_id = 0;

void JSHTMLBaseFontElement::init(JSContext* ctx)
{
    if (JSHTMLBaseFontElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLBaseFontElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLBaseFontElement::js_class_id, &JSHTMLBaseFontElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLBaseFontElementConstructor::self(ctx), JSHTMLBaseFontElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLBaseFontElement::js_class_id, JSHTMLBaseFontElementPrototype::self(ctx));
    }
}

JSValue JSHTMLBaseFontElement::create(JSContext* ctx, HTMLBaseFontElement* impl)
{
    JSHTMLBaseFontElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLBaseFontElementPrototype::self(ctx), JSHTMLBaseFontElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBaseFontElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBaseFontElement* impl = (HTMLBaseFontElement*)JS_GetOpaque(val, JSHTMLBaseFontElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBaseFontElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBaseFontElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->color()).utf8().data());
        }
        case FaceAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->face()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->size());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBaseFontElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            imp->setColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FaceAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            imp->setFace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseFontElement::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBaseFontElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBaseFontElementConstructor::self(ctx);
}


}
