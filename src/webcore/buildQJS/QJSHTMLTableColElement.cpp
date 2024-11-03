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

#include "QJSHTMLTableColElement.h"

#include "HTMLTableColElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTableColElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("width", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("chOff", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::ChOffAttrNum),
    JS_CGETSET_MAGIC_DEF("ch", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::ChAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("span", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::SpanAttrNum),
    JS_CGETSET_MAGIC_DEF("vAlign", JSHTMLTableColElement::getValueProperty, JSHTMLTableColElement::putValueProperty, JSHTMLTableColElement::VAlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableColElement::getValueProperty, NULL, JSHTMLTableColElement::ConstructorAttrNum)
};

class JSHTMLTableColElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableColElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableColElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableColElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableColElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableColElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableColElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableColElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableColElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableColElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableColElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableColElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableColElementAttributesFunctions, countof(JSHTMLTableColElementAttributesFunctions));
}

static JSClassDef JSHTMLTableColElementClassDefine = 
{
    "HTMLTableColElement",
    .finalizer = JSHTMLTableColElement::finalizer,
    .gc_mark = JSHTMLTableColElement::mark,
};

JSClassID JSHTMLTableColElement::js_class_id = 0;

void JSHTMLTableColElement::init(JSContext* ctx)
{
    if (JSHTMLTableColElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableColElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableColElement::js_class_id, &JSHTMLTableColElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableColElementConstructor::self(ctx), JSHTMLTableColElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableColElement::js_class_id, JSHTMLTableColElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableColElement::create(JSContext* ctx, HTMLTableColElement* impl)
{
    JSHTMLTableColElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableColElementPrototype::self(ctx), JSHTMLTableColElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableColElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableColElement* impl = (HTMLTableColElement*)JS_GetOpaque(val, JSHTMLTableColElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableColElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableColElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case SpanAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->span());
        }
        case VAlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableColElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SpanAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setSpan(valueToInt32(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableColElement::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableColElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableColElementConstructor::self(ctx);
}


}
