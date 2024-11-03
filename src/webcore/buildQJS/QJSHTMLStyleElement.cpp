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

#include "QJSHTMLStyleElement.h"

#include "HTMLStyleElement.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLStyleElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("sheet", JSHTMLStyleElement::getValueProperty, NULL, JSHTMLStyleElement::SheetAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLStyleElement::getValueProperty, NULL, JSHTMLStyleElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLStyleElement::getValueProperty, JSHTMLStyleElement::putValueProperty, JSHTMLStyleElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("media", JSHTMLStyleElement::getValueProperty, JSHTMLStyleElement::putValueProperty, JSHTMLStyleElement::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLStyleElement::getValueProperty, JSHTMLStyleElement::putValueProperty, JSHTMLStyleElement::TypeAttrNum)
};

class JSHTMLStyleElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLStyleElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLStyleElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLStyleElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLStyleElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLStyleElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLStyleElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLStyleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLStyleElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLStyleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLStyleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLStyleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLStyleElementAttributesFunctions, countof(JSHTMLStyleElementAttributesFunctions));
}

static JSClassDef JSHTMLStyleElementClassDefine = 
{
    "HTMLStyleElement",
    .finalizer = JSHTMLStyleElement::finalizer,
    .gc_mark = JSHTMLStyleElement::mark,
};

JSClassID JSHTMLStyleElement::js_class_id = 0;

void JSHTMLStyleElement::init(JSContext* ctx)
{
    if (JSHTMLStyleElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLStyleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLStyleElement::js_class_id, &JSHTMLStyleElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLStyleElementConstructor::self(ctx), JSHTMLStyleElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLStyleElement::js_class_id, JSHTMLStyleElementPrototype::self(ctx));
    }
}

JSValue JSHTMLStyleElement::create(JSContext* ctx, HTMLStyleElement* impl)
{
    JSHTMLStyleElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLStyleElementPrototype::self(ctx), JSHTMLStyleElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLStyleElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLStyleElement* impl = (HTMLStyleElement*)JS_GetOpaque(val, JSHTMLStyleElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLStyleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLStyleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MediaAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->media()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SheetAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->sheet()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLStyleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MediaAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            imp->setMedia(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLStyleElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLStyleElement::getConstructor(JSContext *ctx)
{
    return JSHTMLStyleElementConstructor::self(ctx);
}


}
