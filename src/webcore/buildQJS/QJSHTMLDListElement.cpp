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

#include "QJSHTMLDListElement.h"

#include "HTMLDListElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLDListElementAttributesFunctions[2];
static bool JSHTMLDListElementAttributesFunctions_initialized = false;

static void init_JSHTMLDListElementAttributesFunctions()
{
    if (JSHTMLDListElementAttributesFunctions_initialized) return;
    JSHTMLDListElementAttributesFunctions_initialized = true;
    memset(JSHTMLDListElementAttributesFunctions, 0, sizeof(JSHTMLDListElementAttributesFunctions));
    JSHTMLDListElementAttributesFunctions[0].name = "compact";
    JSHTMLDListElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDListElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDListElementAttributesFunctions[0].magic = JSHTMLDListElement::CompactAttrNum;
    JSHTMLDListElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLDListElement::getValueProperty;
    JSHTMLDListElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLDListElement::putValueProperty;
    JSHTMLDListElementAttributesFunctions[1].name = "constructor";
    JSHTMLDListElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDListElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDListElementAttributesFunctions[1].magic = JSHTMLDListElement::ConstructorAttrNum;
    JSHTMLDListElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLDListElement::getValueProperty;
    JSHTMLDListElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLDListElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLDListElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDListElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDListElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDListElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDListElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLDListElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDListElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLDListElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDListElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDListElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLDListElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDListElementAttributesFunctions, countof(JSHTMLDListElementAttributesFunctions));
}

static JSClassDef JSHTMLDListElementClassDefine;
static bool JSHTMLDListElementClassDefine_initialized = false;

static void init_JSHTMLDListElementClassDefine()
{
    if (JSHTMLDListElementClassDefine_initialized) return;
    JSHTMLDListElementClassDefine_initialized = true;
    memset(&JSHTMLDListElementClassDefine, 0, sizeof(JSHTMLDListElementClassDefine));
    JSHTMLDListElementClassDefine.class_name = "HTMLDListElement";
    JSHTMLDListElementClassDefine.finalizer = JSHTMLDListElement::finalizer;
    JSHTMLDListElementClassDefine.gc_mark = JSHTMLDListElement::mark;
}

JSClassID JSHTMLDListElement::js_class_id = 0;

void JSHTMLDListElement::init(JSContext* ctx)
{
    if (JSHTMLDListElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLDListElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLDListElement::create(JSContext* ctx, HTMLDListElement* impl)
{
    JSHTMLDListElement::init(ctx);
    JSValue _proto = JSHTMLDListElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDListElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDListElement* impl = (HTMLDListElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDListElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLDListElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDListElement* imp = (HTMLDListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDListElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDListElement* imp = (HTMLDListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDListElement::getConstructor(JSContext *ctx)
{
    return JSHTMLDListElementConstructor::self(ctx);
}


}
