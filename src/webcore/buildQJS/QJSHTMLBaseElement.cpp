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

#include "QJSHTMLBaseElement.h"

#include "HTMLBaseElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLBaseElementAttributesFunctions[3];
static bool JSHTMLBaseElementAttributesFunctions_initialized = false;

static void init_JSHTMLBaseElementAttributesFunctions()
{
    if (JSHTMLBaseElementAttributesFunctions_initialized) return;
    JSHTMLBaseElementAttributesFunctions_initialized = true;
    memset(JSHTMLBaseElementAttributesFunctions, 0, sizeof(JSHTMLBaseElementAttributesFunctions));
    JSHTMLBaseElementAttributesFunctions[0].name = "href";
    JSHTMLBaseElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseElementAttributesFunctions[0].magic = JSHTMLBaseElement::HrefAttrNum;
    JSHTMLBaseElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLBaseElement::getValueProperty;
    JSHTMLBaseElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLBaseElement::putValueProperty;
    JSHTMLBaseElementAttributesFunctions[1].name = "target";
    JSHTMLBaseElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseElementAttributesFunctions[1].magic = JSHTMLBaseElement::TargetAttrNum;
    JSHTMLBaseElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLBaseElement::getValueProperty;
    JSHTMLBaseElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLBaseElement::putValueProperty;
    JSHTMLBaseElementAttributesFunctions[2].name = "constructor";
    JSHTMLBaseElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseElementAttributesFunctions[2].magic = JSHTMLBaseElement::ConstructorAttrNum;
    JSHTMLBaseElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLBaseElement::getValueProperty;
    JSHTMLBaseElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSHTMLBaseElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLBaseElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBaseElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBaseElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBaseElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBaseElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBaseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBaseElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBaseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBaseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBaseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLBaseElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBaseElementAttributesFunctions, countof(JSHTMLBaseElementAttributesFunctions));
}

static JSClassDef JSHTMLBaseElementClassDefine;
static bool JSHTMLBaseElementClassDefine_initialized = false;

static void init_JSHTMLBaseElementClassDefine()
{
    if (JSHTMLBaseElementClassDefine_initialized) return;
    JSHTMLBaseElementClassDefine_initialized = true;
    memset(&JSHTMLBaseElementClassDefine, 0, sizeof(JSHTMLBaseElementClassDefine));
    JSHTMLBaseElementClassDefine.class_name = "HTMLBaseElement";
    JSHTMLBaseElementClassDefine.finalizer = JSHTMLBaseElement::finalizer;
    JSHTMLBaseElementClassDefine.gc_mark = JSHTMLBaseElement::mark;
}

JSClassID JSHTMLBaseElement::js_class_id = 0;

void JSHTMLBaseElement::init(JSContext* ctx)
{
    if (JSHTMLBaseElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLBaseElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLBaseElement::create(JSContext* ctx, HTMLBaseElement* impl)
{
    JSHTMLBaseElement::init(ctx);
    JSValue _proto = JSHTMLBaseElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBaseElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBaseElement* impl = (HTMLBaseElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBaseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBaseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBaseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case HrefAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBaseElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBaseElementConstructor::self(ctx);
}


}
