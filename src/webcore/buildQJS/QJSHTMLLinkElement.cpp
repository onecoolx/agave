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

#include "QJSHTMLLinkElement.h"

#include "HTMLLinkElement.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLLinkElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLLinkElement::getValueProperty, NULL, JSHTMLLinkElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("charset", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::CharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("media", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("hreflang", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::HreflangAttrNum),
    JS_CGETSET_MAGIC_DEF("rel", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::RelAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("rev", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::RevAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLLinkElement::getValueProperty, JSHTMLLinkElement::putValueProperty, JSHTMLLinkElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("sheet", JSHTMLLinkElement::getValueProperty, NULL, JSHTMLLinkElement::SheetAttrNum)
};

class JSHTMLLinkElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLLinkElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLLinkElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLinkElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLinkElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLinkElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLinkElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLLinkElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLinkElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLinkElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLinkElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLLinkElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLinkElementAttributesFunctions, countof(JSHTMLLinkElementAttributesFunctions));
}

static JSClassDef JSHTMLLinkElementClassDefine = 
{
    "HTMLLinkElement",
    .finalizer = JSHTMLLinkElement::finalizer,
    .gc_mark = JSHTMLLinkElement::mark,
};

JSClassID JSHTMLLinkElement::js_class_id = 0;

void JSHTMLLinkElement::init(JSContext* ctx)
{
    if (JSHTMLLinkElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLLinkElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLLinkElement::js_class_id, &JSHTMLLinkElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLLinkElementConstructor::self(ctx), JSHTMLLinkElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLLinkElement::js_class_id, JSHTMLLinkElementPrototype::self(ctx));
    }
}

JSValue JSHTMLLinkElement::create(JSContext* ctx, HTMLLinkElement* impl)
{
    JSHTMLLinkElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLLinkElementPrototype::self(ctx), JSHTMLLinkElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLinkElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLinkElement* impl = (HTMLLinkElement*)JS_GetOpaque(val, JSHTMLLinkElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLinkElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLinkElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case CharsetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case HreflangAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hreflang()).utf8().data());
        }
        case MediaAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->media()).utf8().data());
        }
        case RelAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rel()).utf8().data());
        }
        case RevAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rev()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SheetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->sheet()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLinkElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HreflangAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setHreflang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MediaAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setMedia(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RelAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setRel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RevAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setRev(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLLinkElement* imp = (HTMLLinkElement*)JS_GetOpaque2(ctx, this_val, JSHTMLLinkElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLinkElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLinkElementConstructor::self(ctx);
}


}
