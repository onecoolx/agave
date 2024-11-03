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

#include "QJSHTMLBodyElement.h"

#include "HTMLBodyElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLBodyElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("aLink", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::ALinkAttrNum),
    JS_CGETSET_MAGIC_DEF("text", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::TextAttrNum),
    JS_CGETSET_MAGIC_DEF("link", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::LinkAttrNum),
    JS_CGETSET_MAGIC_DEF("background", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::BackgroundAttrNum),
    JS_CGETSET_MAGIC_DEF("vLink", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::VLinkAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollLeft", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::ScrollLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("bgColor", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::BgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollTop", JSHTMLBodyElement::getValueProperty, JSHTMLBodyElement::putValueProperty, JSHTMLBodyElement::ScrollTopAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollWidth", JSHTMLBodyElement::getValueProperty, NULL, JSHTMLBodyElement::ScrollWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("scrollHeight", JSHTMLBodyElement::getValueProperty, NULL, JSHTMLBodyElement::ScrollHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLBodyElement::getValueProperty, NULL, JSHTMLBodyElement::ConstructorAttrNum)
};

class JSHTMLBodyElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLBodyElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLBodyElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBodyElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBodyElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBodyElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBodyElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBodyElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBodyElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBodyElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBodyElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBodyElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBodyElementAttributesFunctions, countof(JSHTMLBodyElementAttributesFunctions));
}

static JSClassDef JSHTMLBodyElementClassDefine = 
{
    "HTMLBodyElement",
    .finalizer = JSHTMLBodyElement::finalizer,
    .gc_mark = JSHTMLBodyElement::mark,
};

JSClassID JSHTMLBodyElement::js_class_id = 0;

void JSHTMLBodyElement::init(JSContext* ctx)
{
    if (JSHTMLBodyElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLBodyElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLBodyElement::js_class_id, &JSHTMLBodyElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLBodyElementConstructor::self(ctx), JSHTMLBodyElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLBodyElement::js_class_id, JSHTMLBodyElementPrototype::self(ctx));
    }
}

JSValue JSHTMLBodyElement::create(JSContext* ctx, HTMLBodyElement* impl)
{
    JSHTMLBodyElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLBodyElementPrototype::self(ctx), JSHTMLBodyElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBodyElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBodyElement* impl = (HTMLBodyElement*)JS_GetOpaque(val, JSHTMLBodyElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBodyElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBodyElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ALinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->aLink()).utf8().data());
        }
        case BackgroundAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->background()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case LinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->link()).utf8().data());
        }
        case TextAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case VLinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vLink()).utf8().data());
        }
        case ScrollLeftAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollLeft());
        }
        case ScrollTopAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollTop());
        }
        case ScrollWidthAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollWidth());
        }
        case ScrollHeightAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->scrollHeight());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBodyElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ALinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setALink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BackgroundAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setBackground(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setLink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TextAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VLinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setVLink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ScrollLeftAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setScrollLeft(valueToInt32(ctx, value));
            break;
        }
        case ScrollTopAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBodyElement::js_class_id);
            imp->setScrollTop(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBodyElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBodyElementConstructor::self(ctx);
}


}
