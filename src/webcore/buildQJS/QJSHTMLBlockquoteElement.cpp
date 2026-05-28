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

#include "QJSHTMLBlockquoteElement.h"

#include "HTMLBlockquoteElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLBlockquoteElementAttributesFunctions[2];
static bool JSHTMLBlockquoteElementAttributesFunctions_initialized = false;

static void init_JSHTMLBlockquoteElementAttributesFunctions()
{
    if (JSHTMLBlockquoteElementAttributesFunctions_initialized) return;
    JSHTMLBlockquoteElementAttributesFunctions_initialized = true;
    memset(JSHTMLBlockquoteElementAttributesFunctions, 0, sizeof(JSHTMLBlockquoteElementAttributesFunctions));
    JSHTMLBlockquoteElementAttributesFunctions[0].name = "cite";
    JSHTMLBlockquoteElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBlockquoteElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBlockquoteElementAttributesFunctions[0].magic = JSHTMLBlockquoteElement::CiteAttrNum;
    JSHTMLBlockquoteElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLBlockquoteElement::getValueProperty;
    JSHTMLBlockquoteElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLBlockquoteElement::putValueProperty;
    JSHTMLBlockquoteElementAttributesFunctions[1].name = "constructor";
    JSHTMLBlockquoteElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBlockquoteElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBlockquoteElementAttributesFunctions[1].magic = JSHTMLBlockquoteElement::ConstructorAttrNum;
    JSHTMLBlockquoteElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLBlockquoteElement::getValueProperty;
    JSHTMLBlockquoteElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLBlockquoteElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLBlockquoteElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLBlockquoteElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBlockquoteElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBlockquoteElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBlockquoteElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBlockquoteElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBlockquoteElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBlockquoteElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBlockquoteElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBlockquoteElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBlockquoteElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLBlockquoteElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBlockquoteElementAttributesFunctions, countof(JSHTMLBlockquoteElementAttributesFunctions));
}

static JSClassDef JSHTMLBlockquoteElementClassDefine;
static bool JSHTMLBlockquoteElementClassDefine_initialized = false;

static void init_JSHTMLBlockquoteElementClassDefine()
{
    if (JSHTMLBlockquoteElementClassDefine_initialized) return;
    JSHTMLBlockquoteElementClassDefine_initialized = true;
    memset(&JSHTMLBlockquoteElementClassDefine, 0, sizeof(JSHTMLBlockquoteElementClassDefine));
    JSHTMLBlockquoteElementClassDefine.class_name = "HTMLBlockquoteElement";
    JSHTMLBlockquoteElementClassDefine.finalizer = JSHTMLBlockquoteElement::finalizer;
    JSHTMLBlockquoteElementClassDefine.gc_mark = JSHTMLBlockquoteElement::mark;
}

JSClassID JSHTMLBlockquoteElement::js_class_id = 0;

void JSHTMLBlockquoteElement::init(JSContext* ctx)
{
    if (JSHTMLBlockquoteElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLBlockquoteElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLBlockquoteElement::create(JSContext* ctx, HTMLBlockquoteElement* impl)
{
    JSHTMLBlockquoteElement::init(ctx);
    JSValue _proto = JSHTMLBlockquoteElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBlockquoteElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBlockquoteElement* impl = (HTMLBlockquoteElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBlockquoteElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBlockquoteElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLBlockquoteElement* imp = (HTMLBlockquoteElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cite()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBlockquoteElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLBlockquoteElement* imp = (HTMLBlockquoteElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCite(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBlockquoteElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBlockquoteElementConstructor::self(ctx);
}


}
