/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>

#include "QJSHTMLAnchorElement.h"

#include "HTMLAnchorElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLAnchorElementAttributesFunctions[21];
static bool JSHTMLAnchorElementAttributesFunctions_initialized = false;

static void init_JSHTMLAnchorElementAttributesFunctions()
{
    if (JSHTMLAnchorElementAttributesFunctions_initialized) return;
    JSHTMLAnchorElementAttributesFunctions_initialized = true;
    memset(JSHTMLAnchorElementAttributesFunctions, 0, sizeof(JSHTMLAnchorElementAttributesFunctions));
    JSHTMLAnchorElementAttributesFunctions[0].name = "accessKey";
    JSHTMLAnchorElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[0].magic = JSHTMLAnchorElement::AccessKeyAttrNum;
    JSHTMLAnchorElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[1].name = "charset";
    JSHTMLAnchorElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[1].magic = JSHTMLAnchorElement::CharsetAttrNum;
    JSHTMLAnchorElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[2].name = "coords";
    JSHTMLAnchorElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[2].magic = JSHTMLAnchorElement::CoordsAttrNum;
    JSHTMLAnchorElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[3].name = "href";
    JSHTMLAnchorElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[3].magic = JSHTMLAnchorElement::HrefAttrNum;
    JSHTMLAnchorElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[4].name = "hreflang";
    JSHTMLAnchorElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[4].magic = JSHTMLAnchorElement::HreflangAttrNum;
    JSHTMLAnchorElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[5].name = "name";
    JSHTMLAnchorElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[5].magic = JSHTMLAnchorElement::NameAttrNum;
    JSHTMLAnchorElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[6].name = "rel";
    JSHTMLAnchorElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[6].magic = JSHTMLAnchorElement::RelAttrNum;
    JSHTMLAnchorElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[7].name = "rev";
    JSHTMLAnchorElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[7].magic = JSHTMLAnchorElement::RevAttrNum;
    JSHTMLAnchorElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[8].name = "shape";
    JSHTMLAnchorElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[8].magic = JSHTMLAnchorElement::ShapeAttrNum;
    JSHTMLAnchorElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[9].name = "tabIndex";
    JSHTMLAnchorElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[9].magic = JSHTMLAnchorElement::TabIndexAttrNum;
    JSHTMLAnchorElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[10].name = "target";
    JSHTMLAnchorElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[10].magic = JSHTMLAnchorElement::TargetAttrNum;
    JSHTMLAnchorElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[11].name = "type";
    JSHTMLAnchorElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[11].magic = JSHTMLAnchorElement::TypeAttrNum;
    JSHTMLAnchorElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLAnchorElement::putValueProperty;
    JSHTMLAnchorElementAttributesFunctions[12].name = "hash";
    JSHTMLAnchorElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[12].magic = JSHTMLAnchorElement::HashAttrNum;
    JSHTMLAnchorElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[13].name = "host";
    JSHTMLAnchorElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[13].magic = JSHTMLAnchorElement::HostAttrNum;
    JSHTMLAnchorElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[14].name = "hostname";
    JSHTMLAnchorElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[14].magic = JSHTMLAnchorElement::HostnameAttrNum;
    JSHTMLAnchorElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[15].name = "pathname";
    JSHTMLAnchorElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[15].magic = JSHTMLAnchorElement::PathnameAttrNum;
    JSHTMLAnchorElementAttributesFunctions[15].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[16].name = "port";
    JSHTMLAnchorElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[16].magic = JSHTMLAnchorElement::PortAttrNum;
    JSHTMLAnchorElementAttributesFunctions[16].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[17].name = "protocol";
    JSHTMLAnchorElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[17].magic = JSHTMLAnchorElement::ProtocolAttrNum;
    JSHTMLAnchorElementAttributesFunctions[17].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[17].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[18].name = "search";
    JSHTMLAnchorElementAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[18].magic = JSHTMLAnchorElement::SearchAttrNum;
    JSHTMLAnchorElementAttributesFunctions[18].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[18].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[19].name = "text";
    JSHTMLAnchorElementAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[19].magic = JSHTMLAnchorElement::TextAttrNum;
    JSHTMLAnchorElementAttributesFunctions[19].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[19].u.getset.set.setter_magic = NULL;
    JSHTMLAnchorElementAttributesFunctions[20].name = "constructor";
    JSHTMLAnchorElementAttributesFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementAttributesFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAnchorElementAttributesFunctions[20].magic = JSHTMLAnchorElement::ConstructorAttrNum;
    JSHTMLAnchorElementAttributesFunctions[20].u.getset.get.getter_magic = JSHTMLAnchorElement::getValueProperty;
    JSHTMLAnchorElementAttributesFunctions[20].u.getset.set.setter_magic = NULL;
}

class JSHTMLAnchorElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLAnchorElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAnchorElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAnchorElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAnchorElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAnchorElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLAnchorElementPrototypeFunctions[3];
static bool JSHTMLAnchorElementPrototypeFunctions_initialized = false;

static void init_JSHTMLAnchorElementPrototypeFunctions()
{
    if (JSHTMLAnchorElementPrototypeFunctions_initialized) return;
    JSHTMLAnchorElementPrototypeFunctions_initialized = true;
    memset(JSHTMLAnchorElementPrototypeFunctions, 0, sizeof(JSHTMLAnchorElementPrototypeFunctions));
    JSHTMLAnchorElementPrototypeFunctions[0].name = "blur";
    JSHTMLAnchorElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLAnchorElementPrototypeFunctions[0].magic = JSHTMLAnchorElement::BlurFuncNum;
    JSHTMLAnchorElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLAnchorElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLAnchorElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLAnchorElementPrototypeFunction::callAsFunction;
    JSHTMLAnchorElementPrototypeFunctions[1].name = "focus";
    JSHTMLAnchorElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLAnchorElementPrototypeFunctions[1].magic = JSHTMLAnchorElement::FocusFuncNum;
    JSHTMLAnchorElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLAnchorElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLAnchorElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLAnchorElementPrototypeFunction::callAsFunction;
    JSHTMLAnchorElementPrototypeFunctions[2].name = "toString";
    JSHTMLAnchorElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLAnchorElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLAnchorElementPrototypeFunctions[2].magic = JSHTMLAnchorElement::ToStringFuncNum;
    JSHTMLAnchorElementPrototypeFunctions[2].u.func.length = 0;
    JSHTMLAnchorElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLAnchorElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLAnchorElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLAnchorElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAnchorElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAnchorElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAnchorElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAnchorElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLAnchorElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAnchorElementAttributesFunctions, countof(JSHTMLAnchorElementAttributesFunctions));
    init_JSHTMLAnchorElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAnchorElementPrototypeFunctions, countof(JSHTMLAnchorElementPrototypeFunctions));
}

JSClassID JSHTMLAnchorElement::js_class_id = 0;

void JSHTMLAnchorElement::init(JSContext* ctx)
{
    if (JSHTMLAnchorElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLAnchorElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLAnchorElement::create(JSContext* ctx, HTMLAnchorElement* impl)
{
    JSHTMLAnchorElement::init(ctx);
    JSValue _proto = JSHTMLAnchorElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAnchorElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAnchorElement* impl = (HTMLAnchorElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case CharsetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case CoordsAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->coords()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case HreflangAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hreflang()).utf8().data());
        }
        case NameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case RelAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rel()).utf8().data());
        }
        case RevAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rev()).utf8().data());
        }
        case ShapeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shape()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TargetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case HashAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hash()).utf8().data());
        }
        case HostAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->host()).utf8().data());
        }
        case HostnameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hostname()).utf8().data());
        }
        case PathnameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathname()).utf8().data());
        }
        case PortAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->port()).utf8().data());
        }
        case ProtocolAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->protocol()).utf8().data());
        }
        case SearchAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->search()).utf8().data());
        }
        case TextAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CoordsAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCoords(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HreflangAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHreflang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RelAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RevAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRev(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ShapeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setShape(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLAnchorElement* imp = (HTMLAnchorElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
