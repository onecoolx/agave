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

#include "QJSHTMLAreaElement.h"

#include "HTMLAreaElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLAreaElementAttributesFunctions[16];
static bool JSHTMLAreaElementAttributesFunctions_initialized = false;

static void init_JSHTMLAreaElementAttributesFunctions()
{
    if (JSHTMLAreaElementAttributesFunctions_initialized) return;
    JSHTMLAreaElementAttributesFunctions_initialized = true;
    memset(JSHTMLAreaElementAttributesFunctions, 0, sizeof(JSHTMLAreaElementAttributesFunctions));
    JSHTMLAreaElementAttributesFunctions[0].name = "accessKey";
    JSHTMLAreaElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[0].magic = JSHTMLAreaElement::AccessKeyAttrNum;
    JSHTMLAreaElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[1].name = "alt";
    JSHTMLAreaElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[1].magic = JSHTMLAreaElement::AltAttrNum;
    JSHTMLAreaElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[2].name = "coords";
    JSHTMLAreaElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[2].magic = JSHTMLAreaElement::CoordsAttrNum;
    JSHTMLAreaElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[3].name = "href";
    JSHTMLAreaElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[3].magic = JSHTMLAreaElement::HrefAttrNum;
    JSHTMLAreaElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[4].name = "noHref";
    JSHTMLAreaElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[4].magic = JSHTMLAreaElement::NoHrefAttrNum;
    JSHTMLAreaElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[5].name = "shape";
    JSHTMLAreaElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[5].magic = JSHTMLAreaElement::ShapeAttrNum;
    JSHTMLAreaElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[6].name = "tabIndex";
    JSHTMLAreaElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[6].magic = JSHTMLAreaElement::TabIndexAttrNum;
    JSHTMLAreaElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[7].name = "target";
    JSHTMLAreaElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[7].magic = JSHTMLAreaElement::TargetAttrNum;
    JSHTMLAreaElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLAreaElement::putValueProperty;
    JSHTMLAreaElementAttributesFunctions[8].name = "hash";
    JSHTMLAreaElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[8].magic = JSHTMLAreaElement::HashAttrNum;
    JSHTMLAreaElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[9].name = "host";
    JSHTMLAreaElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[9].magic = JSHTMLAreaElement::HostAttrNum;
    JSHTMLAreaElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[10].name = "hostname";
    JSHTMLAreaElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[10].magic = JSHTMLAreaElement::HostnameAttrNum;
    JSHTMLAreaElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[11].name = "pathname";
    JSHTMLAreaElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[11].magic = JSHTMLAreaElement::PathnameAttrNum;
    JSHTMLAreaElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[12].name = "port";
    JSHTMLAreaElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[12].magic = JSHTMLAreaElement::PortAttrNum;
    JSHTMLAreaElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[13].name = "protocol";
    JSHTMLAreaElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[13].magic = JSHTMLAreaElement::ProtocolAttrNum;
    JSHTMLAreaElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[14].name = "search";
    JSHTMLAreaElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[14].magic = JSHTMLAreaElement::SearchAttrNum;
    JSHTMLAreaElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSHTMLAreaElementAttributesFunctions[15].name = "constructor";
    JSHTMLAreaElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAreaElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAreaElementAttributesFunctions[15].magic = JSHTMLAreaElement::ConstructorAttrNum;
    JSHTMLAreaElementAttributesFunctions[15].u.getset.get.getter_magic = JSHTMLAreaElement::getValueProperty;
    JSHTMLAreaElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
}

class JSHTMLAreaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLAreaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAreaElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAreaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAreaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAreaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLAreaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAreaElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAreaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAreaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAreaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLAreaElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAreaElementAttributesFunctions, countof(JSHTMLAreaElementAttributesFunctions));
}

static JSClassDef JSHTMLAreaElementClassDefine;
static bool JSHTMLAreaElementClassDefine_initialized = false;

static void init_JSHTMLAreaElementClassDefine()
{
    if (JSHTMLAreaElementClassDefine_initialized) return;
    JSHTMLAreaElementClassDefine_initialized = true;
    memset(&JSHTMLAreaElementClassDefine, 0, sizeof(JSHTMLAreaElementClassDefine));
    JSHTMLAreaElementClassDefine.class_name = "HTMLAreaElement";
    JSHTMLAreaElementClassDefine.finalizer = JSHTMLAreaElement::finalizer;
    JSHTMLAreaElementClassDefine.gc_mark = JSHTMLAreaElement::mark;
}

JSClassID JSHTMLAreaElement::js_class_id = 0;

void JSHTMLAreaElement::init(JSContext* ctx)
{
    if (JSHTMLAreaElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLAreaElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLAreaElement::create(JSContext* ctx, HTMLAreaElement* impl)
{
    JSHTMLAreaElement::init(ctx);
    JSValue _proto = JSHTMLAreaElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAreaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAreaElement* impl = (HTMLAreaElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AltAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case CoordsAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->coords()).utf8().data());
        }
        case HrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case NoHrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->noHref() ? 1 : 0);
        }
        case ShapeAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shape()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TargetAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case HashAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hash()).utf8().data());
        }
        case HostAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->host()).utf8().data());
        }
        case HostnameAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hostname()).utf8().data());
        }
        case PathnameAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathname()).utf8().data());
        }
        case PortAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->port()).utf8().data());
        }
        case ProtocolAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->protocol()).utf8().data());
        }
        case SearchAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CoordsAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCoords(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoHrefAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setNoHref(valueToBoolean(ctx, value));
            break;
        }
        case ShapeAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setShape(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLAreaElement* imp = (HTMLAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
