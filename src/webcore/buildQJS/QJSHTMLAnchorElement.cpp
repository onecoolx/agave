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

#include "QJSHTMLAnchorElement.h"

#include "HTMLAnchorElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLAnchorElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("protocol", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::ProtocolAttrNum),
    JS_CGETSET_MAGIC_DEF("hash", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::HashAttrNum),
    JS_CGETSET_MAGIC_DEF("charset", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::CharsetAttrNum),
    JS_CGETSET_MAGIC_DEF("hreflang", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::HreflangAttrNum),
    JS_CGETSET_MAGIC_DEF("host", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::HostAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("rev", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::RevAttrNum),
    JS_CGETSET_MAGIC_DEF("coords", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::CoordsAttrNum),
    JS_CGETSET_MAGIC_DEF("port", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::PortAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("search", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::SearchAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("shape", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::ShapeAttrNum),
    JS_CGETSET_MAGIC_DEF("hostname", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::HostnameAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("rel", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::RelAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLAnchorElement::getValueProperty, JSHTMLAnchorElement::putValueProperty, JSHTMLAnchorElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("pathname", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::PathnameAttrNum),
    JS_CGETSET_MAGIC_DEF("text", JSHTMLAnchorElement::getValueProperty, NULL, JSHTMLAnchorElement::TextAttrNum)
};

class JSHTMLAnchorElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLAnchorElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLAnchorElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAnchorElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAnchorElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAnchorElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAnchorElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLAnchorElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("blur", 0, JSHTMLAnchorElementPrototypeFunction::callAsFunction, JSHTMLAnchorElement::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLAnchorElementPrototypeFunction::callAsFunction, JSHTMLAnchorElement::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("toString", 0, JSHTMLAnchorElementPrototypeFunction::callAsFunction, JSHTMLAnchorElement::ToStringFuncNum)
};

JSValue JSHTMLAnchorElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAnchorElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAnchorElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAnchorElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAnchorElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAnchorElementAttributesFunctions, countof(JSHTMLAnchorElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAnchorElementPrototypeFunctions, countof(JSHTMLAnchorElementPrototypeFunctions));
}

static JSClassDef JSHTMLAnchorElementClassDefine = 
{
    "HTMLAnchorElement",
    .finalizer = JSHTMLAnchorElement::finalizer,
    .gc_mark = JSHTMLAnchorElement::mark,
};

JSClassID JSHTMLAnchorElement::js_class_id = 0;

void JSHTMLAnchorElement::init(JSContext* ctx)
{
    if (JSHTMLAnchorElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLAnchorElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLAnchorElement::js_class_id, &JSHTMLAnchorElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLAnchorElementConstructor::self(ctx), JSHTMLAnchorElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLAnchorElement::js_class_id, JSHTMLAnchorElementPrototype::self(ctx));
    }
}

JSValue JSHTMLAnchorElement::create(JSContext* ctx, HTMLAnchorElement* impl)
{
    JSHTMLAnchorElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLAnchorElementPrototype::self(ctx), JSHTMLAnchorElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAnchorElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAnchorElement* impl = (HTMLAnchorElement*)JS_GetOpaque(val, JSHTMLAnchorElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLAnchorElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLAnchorElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case CharsetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case CoordsAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->coords()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case HreflangAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hreflang()).utf8().data());
        }
        case NameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case RelAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rel()).utf8().data());
        }
        case RevAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rev()).utf8().data());
        }
        case ShapeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shape()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TargetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case HashAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hash()).utf8().data());
        }
        case HostAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->host()).utf8().data());
        }
        case HostnameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hostname()).utf8().data());
        }
        case PathnameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathname()).utf8().data());
        }
        case PortAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->port()).utf8().data());
        }
        case ProtocolAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->protocol()).utf8().data());
        }
        case SearchAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->search()).utf8().data());
        }
        case TextAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLAnchorElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CoordsAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setCoords(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HreflangAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setHreflang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RelAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setRel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RevAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setRev(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ShapeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setShape(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLAnchorElement::getConstructor(JSContext *ctx)
{
    return JSHTMLAnchorElementConstructor::self(ctx);
}

JSValue JSHTMLAnchorElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAnchorElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLAnchorElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSHTMLAnchorElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSHTMLAnchorElement::ToStringFuncNum: {

            JSValue result = JS_NewString(ctx, ((const String&)imp->toString()).utf8().data());
            return result;
        }
    }
    return JS_NULL;
}


}
