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

#include "QJSHTMLFontElement.h"

#include "HTMLFontElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLFontElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("size", JSHTMLFontElement::getValueProperty, JSHTMLFontElement::putValueProperty, JSHTMLFontElement::SizeAttrNum),
    JS_CGETSET_MAGIC_DEF("color", JSHTMLFontElement::getValueProperty, JSHTMLFontElement::putValueProperty, JSHTMLFontElement::ColorAttrNum),
    JS_CGETSET_MAGIC_DEF("face", JSHTMLFontElement::getValueProperty, JSHTMLFontElement::putValueProperty, JSHTMLFontElement::FaceAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLFontElement::getValueProperty, NULL, JSHTMLFontElement::ConstructorAttrNum)
};

class JSHTMLFontElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLFontElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLFontElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFontElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFontElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFontElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFontElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFontElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFontElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFontElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFontElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFontElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFontElementAttributesFunctions, countof(JSHTMLFontElementAttributesFunctions));
}

static JSClassDef JSHTMLFontElementClassDefine = 
{
    "HTMLFontElement",
    .finalizer = JSHTMLFontElement::finalizer,
    .gc_mark = JSHTMLFontElement::mark,
};

JSClassID JSHTMLFontElement::js_class_id = 0;

void JSHTMLFontElement::init(JSContext* ctx)
{
    if (JSHTMLFontElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLFontElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLFontElement::js_class_id, &JSHTMLFontElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLFontElementConstructor::self(ctx), JSHTMLFontElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLFontElement::js_class_id, JSHTMLFontElementPrototype::self(ctx));
    }
}

JSValue JSHTMLFontElement::create(JSContext* ctx, HTMLFontElement* impl)
{
    JSHTMLFontElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLFontElementPrototype::self(ctx), JSHTMLFontElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFontElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFontElement* impl = (HTMLFontElement*)JS_GetOpaque(val, JSHTMLFontElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLFontElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFontElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->color()).utf8().data());
        }
        case FaceAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->face()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->size()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFontElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            imp->setColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FaceAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            imp->setFace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFontElement::js_class_id);
            imp->setSize(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLFontElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFontElementConstructor::self(ctx);
}


}
