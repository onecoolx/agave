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

#include "QJSHTMLHeadElement.h"

#include "HTMLHeadElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLHeadElementAttributesFunctions[2];
static bool JSHTMLHeadElementAttributesFunctions_initialized = false;

static void init_JSHTMLHeadElementAttributesFunctions()
{
    if (JSHTMLHeadElementAttributesFunctions_initialized) return;
    JSHTMLHeadElementAttributesFunctions_initialized = true;
    memset(JSHTMLHeadElementAttributesFunctions, 0, sizeof(JSHTMLHeadElementAttributesFunctions));
    JSHTMLHeadElementAttributesFunctions[0].name = "profile";
    JSHTMLHeadElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHeadElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHeadElementAttributesFunctions[0].magic = JSHTMLHeadElement::ProfileAttrNum;
    JSHTMLHeadElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLHeadElement::getValueProperty;
    JSHTMLHeadElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLHeadElement::putValueProperty;
    JSHTMLHeadElementAttributesFunctions[1].name = "constructor";
    JSHTMLHeadElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHeadElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHeadElementAttributesFunctions[1].magic = JSHTMLHeadElement::ConstructorAttrNum;
    JSHTMLHeadElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLHeadElement::getValueProperty;
    JSHTMLHeadElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLHeadElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLHeadElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLHeadElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLHeadElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLHeadElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLHeadElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHeadElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLHeadElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLHeadElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLHeadElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLHeadElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHeadElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLHeadElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLHeadElementAttributesFunctions, countof(JSHTMLHeadElementAttributesFunctions));
}

static JSClassDef JSHTMLHeadElementClassDefine;
static bool JSHTMLHeadElementClassDefine_initialized = false;

static void init_JSHTMLHeadElementClassDefine()
{
    if (JSHTMLHeadElementClassDefine_initialized) return;
    JSHTMLHeadElementClassDefine_initialized = true;
    memset(&JSHTMLHeadElementClassDefine, 0, sizeof(JSHTMLHeadElementClassDefine));
    JSHTMLHeadElementClassDefine.class_name = "HTMLHeadElement";
    JSHTMLHeadElementClassDefine.finalizer = JSHTMLHeadElement::finalizer;
    JSHTMLHeadElementClassDefine.gc_mark = JSHTMLHeadElement::mark;
}

JSClassID JSHTMLHeadElement::js_class_id = 0;

void JSHTMLHeadElement::init(JSContext* ctx)
{
    if (JSHTMLHeadElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLHeadElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLHeadElement::create(JSContext* ctx, HTMLHeadElement* impl)
{
    JSHTMLHeadElement::init(ctx);
    JSValue _proto = JSHTMLHeadElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLHeadElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLHeadElement* impl = (HTMLHeadElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLHeadElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLHeadElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ProfileAttrNum: {
            HTMLHeadElement* imp = (HTMLHeadElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->profile()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLHeadElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ProfileAttrNum: {
            HTMLHeadElement* imp = (HTMLHeadElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setProfile(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLHeadElement::getConstructor(JSContext *ctx)
{
    return JSHTMLHeadElementConstructor::self(ctx);
}


}
