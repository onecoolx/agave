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

#include "QJSHTMLAreaElement.h"

#include "HTMLAreaElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLAreaElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("hostname", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::HostnameAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("hash", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::HashAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("alt", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::AltAttrNum),
    JS_CGETSET_MAGIC_DEF("port", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::PortAttrNum),
    JS_CGETSET_MAGIC_DEF("shape", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::ShapeAttrNum),
    JS_CGETSET_MAGIC_DEF("coords", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::CoordsAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("noHref", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::NoHrefAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLAreaElement::getValueProperty, JSHTMLAreaElement::putValueProperty, JSHTMLAreaElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("host", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::HostAttrNum),
    JS_CGETSET_MAGIC_DEF("pathname", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::PathnameAttrNum),
    JS_CGETSET_MAGIC_DEF("protocol", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::ProtocolAttrNum),
    JS_CGETSET_MAGIC_DEF("search", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::SearchAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLAreaElement::getValueProperty, NULL, JSHTMLAreaElement::ConstructorAttrNum)
};

class JSHTMLAreaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLAreaElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLAreaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAreaElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAreaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAreaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAreaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLAreaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAreaElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAreaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAreaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAreaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAreaElementAttributesFunctions, countof(JSHTMLAreaElementAttributesFunctions));
}

static JSClassDef JSHTMLAreaElementClassDefine = 
{
    "HTMLAreaElement",
    .finalizer = JSHTMLAreaElement::finalizer,
    .gc_mark = JSHTMLAreaElement::mark,
};

JSClassID JSHTMLAreaElement::js_class_id = 0;

void JSHTMLAreaElement::init(JSContext* ctx)
{
    if (JSHTMLAreaElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLAreaElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLAreaElement::js_class_id, &JSHTMLAreaElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLAreaElementConstructor::self(ctx), JSHTMLAreaElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLAreaElement::js_class_id, JSHTMLAreaElementPrototype::self(ctx));
    }
}

JSValue JSHTMLAreaElement::create(JSContext* ctx, HTMLAreaElement* impl)
{
    JSHTMLAreaElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLAreaElementPrototype::self(ctx), JSHTMLAreaElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAreaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAreaElement* impl = (HTMLAreaElement*)JS_GetOpaque(val, JSHTMLAreaElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLAreaElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLAreaElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AltAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case CoordsAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->coords()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case NoHrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewBool(ctx, imp->noHref() ? 1 : 0);
        }
        case ShapeAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shape()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TargetAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case HashAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hash()).utf8().data());
        }
        case HostAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->host()).utf8().data());
        }
        case HostnameAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hostname()).utf8().data());
        }
        case PathnameAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathname()).utf8().data());
        }
        case PortAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->port()).utf8().data());
        }
        case ProtocolAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->protocol()).utf8().data());
        }
        case SearchAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->search()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLAreaElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CoordsAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setCoords(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoHrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setNoHref(valueToBoolean(ctx, value));
            break;
        }
        case ShapeAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setShape(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAreaElement::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLAreaElement::getConstructor(JSContext *ctx)
{
    return JSHTMLAreaElementConstructor::self(ctx);
}


}
