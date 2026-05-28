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

#include "QJSHTMLQuoteElement.h"

#include "HTMLQuoteElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLQuoteElementAttributesFunctions[2];
static bool JSHTMLQuoteElementAttributesFunctions_initialized = false;

static void init_JSHTMLQuoteElementAttributesFunctions()
{
    if (JSHTMLQuoteElementAttributesFunctions_initialized) return;
    JSHTMLQuoteElementAttributesFunctions_initialized = true;
    memset(JSHTMLQuoteElementAttributesFunctions, 0, sizeof(JSHTMLQuoteElementAttributesFunctions));
    JSHTMLQuoteElementAttributesFunctions[0].name = "cite";
    JSHTMLQuoteElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLQuoteElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLQuoteElementAttributesFunctions[0].magic = JSHTMLQuoteElement::CiteAttrNum;
    JSHTMLQuoteElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLQuoteElement::getValueProperty;
    JSHTMLQuoteElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLQuoteElement::putValueProperty;
    JSHTMLQuoteElementAttributesFunctions[1].name = "constructor";
    JSHTMLQuoteElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLQuoteElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLQuoteElementAttributesFunctions[1].magic = JSHTMLQuoteElement::ConstructorAttrNum;
    JSHTMLQuoteElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLQuoteElement::getValueProperty;
    JSHTMLQuoteElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLQuoteElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLQuoteElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLQuoteElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLQuoteElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLQuoteElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLQuoteElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLQuoteElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLQuoteElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLQuoteElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLQuoteElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLQuoteElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLQuoteElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLQuoteElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLQuoteElementAttributesFunctions, countof(JSHTMLQuoteElementAttributesFunctions));
}

static JSClassDef JSHTMLQuoteElementClassDefine;
static bool JSHTMLQuoteElementClassDefine_initialized = false;

static void init_JSHTMLQuoteElementClassDefine()
{
    if (JSHTMLQuoteElementClassDefine_initialized) return;
    JSHTMLQuoteElementClassDefine_initialized = true;
    memset(&JSHTMLQuoteElementClassDefine, 0, sizeof(JSHTMLQuoteElementClassDefine));
    JSHTMLQuoteElementClassDefine.class_name = "HTMLQuoteElement";
    JSHTMLQuoteElementClassDefine.finalizer = JSHTMLQuoteElement::finalizer;
    JSHTMLQuoteElementClassDefine.gc_mark = JSHTMLQuoteElement::mark;
}

JSClassID JSHTMLQuoteElement::js_class_id = 0;

void JSHTMLQuoteElement::init(JSContext* ctx)
{
    if (JSHTMLQuoteElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLQuoteElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLQuoteElement::create(JSContext* ctx, HTMLQuoteElement* impl)
{
    JSHTMLQuoteElement::init(ctx);
    JSValue _proto = JSHTMLQuoteElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLQuoteElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLQuoteElement* impl = (HTMLQuoteElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLQuoteElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLQuoteElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLQuoteElement* imp = (HTMLQuoteElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cite()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLQuoteElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLQuoteElement* imp = (HTMLQuoteElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCite(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLQuoteElement::getConstructor(JSContext *ctx)
{
    return JSHTMLQuoteElementConstructor::self(ctx);
}


}
