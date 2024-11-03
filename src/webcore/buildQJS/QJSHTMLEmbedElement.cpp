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

#include "QJSHTMLEmbedElement.h"

#include "HTMLEmbedElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLEmbedElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("width", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLEmbedElement::getValueProperty, JSHTMLEmbedElement::putValueProperty, JSHTMLEmbedElement::SrcAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLEmbedElement::getValueProperty, NULL, JSHTMLEmbedElement::ConstructorAttrNum)
};

class JSHTMLEmbedElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLEmbedElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLEmbedElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLEmbedElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLEmbedElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLEmbedElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLEmbedElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLEmbedElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLEmbedElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLEmbedElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLEmbedElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLEmbedElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLEmbedElementAttributesFunctions, countof(JSHTMLEmbedElementAttributesFunctions));
}

static JSClassDef JSHTMLEmbedElementClassDefine = 
{
    "HTMLEmbedElement",
    .finalizer = JSHTMLEmbedElement::finalizer,
    .gc_mark = JSHTMLEmbedElement::mark,
};

JSClassID JSHTMLEmbedElement::js_class_id = 0;

void JSHTMLEmbedElement::init(JSContext* ctx)
{
    if (JSHTMLEmbedElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLEmbedElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLEmbedElement::js_class_id, &JSHTMLEmbedElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLEmbedElementConstructor::self(ctx), JSHTMLEmbedElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLEmbedElement::js_class_id, JSHTMLEmbedElementPrototype::self(ctx));
    }
}

JSValue JSHTMLEmbedElement::create(JSContext* ctx, HTMLEmbedElement* impl)
{
    JSHTMLEmbedElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLEmbedElementPrototype::self(ctx), JSHTMLEmbedElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLEmbedElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLEmbedElement* impl = (HTMLEmbedElement*)JS_GetOpaque(val, JSHTMLEmbedElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLEmbedElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLEmbedElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case NameAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLEmbedElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque2(ctx, this_val, JSHTMLEmbedElement::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLEmbedElement::getConstructor(JSContext *ctx)
{
    return JSHTMLEmbedElementConstructor::self(ctx);
}


}
