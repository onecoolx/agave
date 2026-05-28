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

#include "QJSHTMLMenuElement.h"

#include "HTMLMenuElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLMenuElementAttributesFunctions[2];
static bool JSHTMLMenuElementAttributesFunctions_initialized = false;

static void init_JSHTMLMenuElementAttributesFunctions()
{
    if (JSHTMLMenuElementAttributesFunctions_initialized) return;
    JSHTMLMenuElementAttributesFunctions_initialized = true;
    memset(JSHTMLMenuElementAttributesFunctions, 0, sizeof(JSHTMLMenuElementAttributesFunctions));
    JSHTMLMenuElementAttributesFunctions[0].name = "compact";
    JSHTMLMenuElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMenuElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMenuElementAttributesFunctions[0].magic = JSHTMLMenuElement::CompactAttrNum;
    JSHTMLMenuElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLMenuElement::getValueProperty;
    JSHTMLMenuElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLMenuElement::putValueProperty;
    JSHTMLMenuElementAttributesFunctions[1].name = "constructor";
    JSHTMLMenuElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMenuElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMenuElementAttributesFunctions[1].magic = JSHTMLMenuElement::ConstructorAttrNum;
    JSHTMLMenuElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLMenuElement::getValueProperty;
    JSHTMLMenuElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLMenuElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLMenuElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMenuElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMenuElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMenuElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMenuElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLMenuElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMenuElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMenuElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMenuElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMenuElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLMenuElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMenuElementAttributesFunctions, countof(JSHTMLMenuElementAttributesFunctions));
}

static JSClassDef JSHTMLMenuElementClassDefine;
static bool JSHTMLMenuElementClassDefine_initialized = false;

static void init_JSHTMLMenuElementClassDefine()
{
    if (JSHTMLMenuElementClassDefine_initialized) return;
    JSHTMLMenuElementClassDefine_initialized = true;
    memset(&JSHTMLMenuElementClassDefine, 0, sizeof(JSHTMLMenuElementClassDefine));
    JSHTMLMenuElementClassDefine.class_name = "HTMLMenuElement";
    JSHTMLMenuElementClassDefine.finalizer = JSHTMLMenuElement::finalizer;
    JSHTMLMenuElementClassDefine.gc_mark = JSHTMLMenuElement::mark;
}

JSClassID JSHTMLMenuElement::js_class_id = 0;

void JSHTMLMenuElement::init(JSContext* ctx)
{
    if (JSHTMLMenuElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLMenuElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLMenuElement::create(JSContext* ctx, HTMLMenuElement* impl)
{
    JSHTMLMenuElement::init(ctx);
    JSValue _proto = JSHTMLMenuElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMenuElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMenuElement* impl = (HTMLMenuElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMenuElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMenuElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLMenuElement* imp = (HTMLMenuElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMenuElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLMenuElement* imp = (HTMLMenuElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLMenuElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMenuElementConstructor::self(ctx);
}


}
