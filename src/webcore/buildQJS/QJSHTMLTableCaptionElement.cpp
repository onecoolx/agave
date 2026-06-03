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

#include "QJSHTMLTableCaptionElement.h"

#include "HTMLTableCaptionElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTableCaptionElementAttributesFunctions[2];
static bool JSHTMLTableCaptionElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableCaptionElementAttributesFunctions()
{
    if (JSHTMLTableCaptionElementAttributesFunctions_initialized) return;
    JSHTMLTableCaptionElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableCaptionElementAttributesFunctions, 0, sizeof(JSHTMLTableCaptionElementAttributesFunctions));
    JSHTMLTableCaptionElementAttributesFunctions[0].name = "align";
    JSHTMLTableCaptionElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCaptionElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCaptionElementAttributesFunctions[0].magic = JSHTMLTableCaptionElement::AlignAttrNum;
    JSHTMLTableCaptionElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableCaptionElement::getValueProperty;
    JSHTMLTableCaptionElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTableCaptionElement::putValueProperty;
    JSHTMLTableCaptionElementAttributesFunctions[1].name = "constructor";
    JSHTMLTableCaptionElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCaptionElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCaptionElementAttributesFunctions[1].magic = JSHTMLTableCaptionElement::ConstructorAttrNum;
    JSHTMLTableCaptionElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableCaptionElement::getValueProperty;
    JSHTMLTableCaptionElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableCaptionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableCaptionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableCaptionElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableCaptionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableCaptionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableCaptionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableCaptionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableCaptionElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableCaptionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableCaptionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableCaptionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableCaptionElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableCaptionElementAttributesFunctions, countof(JSHTMLTableCaptionElementAttributesFunctions));
}

JSClassID JSHTMLTableCaptionElement::js_class_id = 0;

void JSHTMLTableCaptionElement::init(JSContext* ctx)
{
    if (JSHTMLTableCaptionElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableCaptionElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableCaptionElement::create(JSContext* ctx, HTMLTableCaptionElement* impl)
{
    JSHTMLTableCaptionElement::init(ctx);
    JSValue _proto = JSHTMLTableCaptionElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableCaptionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableCaptionElement* impl = (HTMLTableCaptionElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLTableCaptionElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableCaptionElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableCaptionElement* imp = (HTMLTableCaptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableCaptionElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableCaptionElement* imp = (HTMLTableCaptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableCaptionElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableCaptionElementConstructor::self(ctx);
}


HTMLTableCaptionElement* toHTMLTableCaptionElement(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLTableCaptionElement* impl = (HTMLTableCaptionElement*)JS_GetOpaque(val, JSHTMLTableCaptionElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
