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

#include "QJSHTMLStyleElement.h"

#include "HTMLStyleElement.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLStyleElementAttributesFunctions[5];
static bool JSHTMLStyleElementAttributesFunctions_initialized = false;

static void init_JSHTMLStyleElementAttributesFunctions()
{
    if (JSHTMLStyleElementAttributesFunctions_initialized) return;
    JSHTMLStyleElementAttributesFunctions_initialized = true;
    memset(JSHTMLStyleElementAttributesFunctions, 0, sizeof(JSHTMLStyleElementAttributesFunctions));
    JSHTMLStyleElementAttributesFunctions[0].name = "sheet";
    JSHTMLStyleElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLStyleElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLStyleElementAttributesFunctions[0].magic = JSHTMLStyleElement::SheetAttrNum;
    JSHTMLStyleElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLStyleElement::getValueProperty;
    JSHTMLStyleElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLStyleElementAttributesFunctions[1].name = "constructor";
    JSHTMLStyleElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLStyleElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLStyleElementAttributesFunctions[1].magic = JSHTMLStyleElement::ConstructorAttrNum;
    JSHTMLStyleElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLStyleElement::getValueProperty;
    JSHTMLStyleElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSHTMLStyleElementAttributesFunctions[2].name = "disabled";
    JSHTMLStyleElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLStyleElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLStyleElementAttributesFunctions[2].magic = JSHTMLStyleElement::DisabledAttrNum;
    JSHTMLStyleElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLStyleElement::getValueProperty;
    JSHTMLStyleElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLStyleElement::putValueProperty;
    JSHTMLStyleElementAttributesFunctions[3].name = "media";
    JSHTMLStyleElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLStyleElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLStyleElementAttributesFunctions[3].magic = JSHTMLStyleElement::MediaAttrNum;
    JSHTMLStyleElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLStyleElement::getValueProperty;
    JSHTMLStyleElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLStyleElement::putValueProperty;
    JSHTMLStyleElementAttributesFunctions[4].name = "type";
    JSHTMLStyleElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLStyleElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLStyleElementAttributesFunctions[4].magic = JSHTMLStyleElement::TypeAttrNum;
    JSHTMLStyleElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLStyleElement::getValueProperty;
    JSHTMLStyleElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLStyleElement::putValueProperty;
}

class JSHTMLStyleElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLStyleElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLStyleElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLStyleElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLStyleElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLStyleElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLStyleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLStyleElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLStyleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLStyleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLStyleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLStyleElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLStyleElementAttributesFunctions, countof(JSHTMLStyleElementAttributesFunctions));
}

static JSClassDef JSHTMLStyleElementClassDefine;
static bool JSHTMLStyleElementClassDefine_initialized = false;

static void init_JSHTMLStyleElementClassDefine()
{
    if (JSHTMLStyleElementClassDefine_initialized) return;
    JSHTMLStyleElementClassDefine_initialized = true;
    memset(&JSHTMLStyleElementClassDefine, 0, sizeof(JSHTMLStyleElementClassDefine));
    JSHTMLStyleElementClassDefine.class_name = "HTMLStyleElement";
    JSHTMLStyleElementClassDefine.finalizer = JSHTMLStyleElement::finalizer;
    JSHTMLStyleElementClassDefine.gc_mark = JSHTMLStyleElement::mark;
}

JSClassID JSHTMLStyleElement::js_class_id = 0;

void JSHTMLStyleElement::init(JSContext* ctx)
{
    if (JSHTMLStyleElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLStyleElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLStyleElement::create(JSContext* ctx, HTMLStyleElement* impl)
{
    JSHTMLStyleElement::init(ctx);
    JSValue _proto = JSHTMLStyleElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLStyleElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLStyleElement* impl = (HTMLStyleElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MediaAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->media()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SheetAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MediaAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMedia(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLStyleElement* imp = (HTMLStyleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
