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

#include "QJSHTMLDivElement.h"

#include "HTMLDivElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLDivElementAttributesFunctions[2];
static bool JSHTMLDivElementAttributesFunctions_initialized = false;

static void init_JSHTMLDivElementAttributesFunctions()
{
    if (JSHTMLDivElementAttributesFunctions_initialized) return;
    JSHTMLDivElementAttributesFunctions_initialized = true;
    memset(JSHTMLDivElementAttributesFunctions, 0, sizeof(JSHTMLDivElementAttributesFunctions));
    JSHTMLDivElementAttributesFunctions[0].name = "align";
    JSHTMLDivElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDivElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDivElementAttributesFunctions[0].magic = JSHTMLDivElement::AlignAttrNum;
    JSHTMLDivElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLDivElement::getValueProperty;
    JSHTMLDivElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLDivElement::putValueProperty;
    JSHTMLDivElementAttributesFunctions[1].name = "constructor";
    JSHTMLDivElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDivElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDivElementAttributesFunctions[1].magic = JSHTMLDivElement::ConstructorAttrNum;
    JSHTMLDivElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLDivElement::getValueProperty;
    JSHTMLDivElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLDivElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLDivElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDivElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDivElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDivElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDivElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLDivElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDivElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLDivElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDivElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDivElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLDivElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDivElementAttributesFunctions, countof(JSHTMLDivElementAttributesFunctions));
}

JSClassID JSHTMLDivElement::js_class_id = 0;

void JSHTMLDivElement::init(JSContext* ctx)
{
    if (JSHTMLDivElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLDivElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLDivElement::create(JSContext* ctx, HTMLDivElement* impl)
{
    JSHTMLDivElement::init(ctx);
    JSValue _proto = JSHTMLDivElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDivElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDivElement* impl = (HTMLDivElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDivElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLDivElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLDivElement* imp = (HTMLDivElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDivElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLDivElement* imp = (HTMLDivElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDivElement::getConstructor(JSContext *ctx)
{
    return JSHTMLDivElementConstructor::self(ctx);
}


}
