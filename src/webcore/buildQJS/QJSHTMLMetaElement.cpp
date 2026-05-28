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

#include "QJSHTMLMetaElement.h"

#include "HTMLMetaElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLMetaElementAttributesFunctions[5];
static bool JSHTMLMetaElementAttributesFunctions_initialized = false;

static void init_JSHTMLMetaElementAttributesFunctions()
{
    if (JSHTMLMetaElementAttributesFunctions_initialized) return;
    JSHTMLMetaElementAttributesFunctions_initialized = true;
    memset(JSHTMLMetaElementAttributesFunctions, 0, sizeof(JSHTMLMetaElementAttributesFunctions));
    JSHTMLMetaElementAttributesFunctions[0].name = "content";
    JSHTMLMetaElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMetaElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMetaElementAttributesFunctions[0].magic = JSHTMLMetaElement::ContentAttrNum;
    JSHTMLMetaElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLMetaElement::getValueProperty;
    JSHTMLMetaElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLMetaElement::putValueProperty;
    JSHTMLMetaElementAttributesFunctions[1].name = "httpEquiv";
    JSHTMLMetaElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMetaElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMetaElementAttributesFunctions[1].magic = JSHTMLMetaElement::HttpEquivAttrNum;
    JSHTMLMetaElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLMetaElement::getValueProperty;
    JSHTMLMetaElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLMetaElement::putValueProperty;
    JSHTMLMetaElementAttributesFunctions[2].name = "name";
    JSHTMLMetaElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMetaElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMetaElementAttributesFunctions[2].magic = JSHTMLMetaElement::NameAttrNum;
    JSHTMLMetaElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLMetaElement::getValueProperty;
    JSHTMLMetaElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLMetaElement::putValueProperty;
    JSHTMLMetaElementAttributesFunctions[3].name = "scheme";
    JSHTMLMetaElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMetaElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMetaElementAttributesFunctions[3].magic = JSHTMLMetaElement::SchemeAttrNum;
    JSHTMLMetaElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLMetaElement::getValueProperty;
    JSHTMLMetaElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLMetaElement::putValueProperty;
    JSHTMLMetaElementAttributesFunctions[4].name = "constructor";
    JSHTMLMetaElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMetaElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMetaElementAttributesFunctions[4].magic = JSHTMLMetaElement::ConstructorAttrNum;
    JSHTMLMetaElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLMetaElement::getValueProperty;
    JSHTMLMetaElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSHTMLMetaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLMetaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMetaElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMetaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMetaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMetaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLMetaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMetaElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMetaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMetaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMetaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLMetaElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMetaElementAttributesFunctions, countof(JSHTMLMetaElementAttributesFunctions));
}

static JSClassDef JSHTMLMetaElementClassDefine;
static bool JSHTMLMetaElementClassDefine_initialized = false;

static void init_JSHTMLMetaElementClassDefine()
{
    if (JSHTMLMetaElementClassDefine_initialized) return;
    JSHTMLMetaElementClassDefine_initialized = true;
    memset(&JSHTMLMetaElementClassDefine, 0, sizeof(JSHTMLMetaElementClassDefine));
    JSHTMLMetaElementClassDefine.class_name = "HTMLMetaElement";
    JSHTMLMetaElementClassDefine.finalizer = JSHTMLMetaElement::finalizer;
    JSHTMLMetaElementClassDefine.gc_mark = JSHTMLMetaElement::mark;
}

JSClassID JSHTMLMetaElement::js_class_id = 0;

void JSHTMLMetaElement::init(JSContext* ctx)
{
    if (JSHTMLMetaElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLMetaElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLMetaElement::create(JSContext* ctx, HTMLMetaElement* impl)
{
    JSHTMLMetaElement::init(ctx);
    JSValue _proto = JSHTMLMetaElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMetaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMetaElement* impl = (HTMLMetaElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMetaElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMetaElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ContentAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->content()).utf8().data());
        }
        case HttpEquivAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->httpEquiv()).utf8().data());
        }
        case NameAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SchemeAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scheme()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMetaElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ContentAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setContent(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HttpEquivAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHttpEquiv(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SchemeAttrNum: {
            HTMLMetaElement* imp = (HTMLMetaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScheme(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLMetaElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMetaElementConstructor::self(ctx);
}


}
