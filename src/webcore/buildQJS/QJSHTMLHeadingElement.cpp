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

#include "QJSHTMLHeadingElement.h"

#include "HTMLHeadingElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLHeadingElementAttributesFunctions[2];
static bool JSHTMLHeadingElementAttributesFunctions_initialized = false;

static void init_JSHTMLHeadingElementAttributesFunctions()
{
    if (JSHTMLHeadingElementAttributesFunctions_initialized) return;
    JSHTMLHeadingElementAttributesFunctions_initialized = true;
    memset(JSHTMLHeadingElementAttributesFunctions, 0, sizeof(JSHTMLHeadingElementAttributesFunctions));
    JSHTMLHeadingElementAttributesFunctions[0].name = "align";
    JSHTMLHeadingElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHeadingElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHeadingElementAttributesFunctions[0].magic = JSHTMLHeadingElement::AlignAttrNum;
    JSHTMLHeadingElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLHeadingElement::getValueProperty;
    JSHTMLHeadingElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLHeadingElement::putValueProperty;
    JSHTMLHeadingElementAttributesFunctions[1].name = "constructor";
    JSHTMLHeadingElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHeadingElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHeadingElementAttributesFunctions[1].magic = JSHTMLHeadingElement::ConstructorAttrNum;
    JSHTMLHeadingElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLHeadingElement::getValueProperty;
    JSHTMLHeadingElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLHeadingElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLHeadingElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLHeadingElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLHeadingElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLHeadingElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHeadingElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLHeadingElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLHeadingElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLHeadingElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLHeadingElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHeadingElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLHeadingElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLHeadingElementAttributesFunctions, countof(JSHTMLHeadingElementAttributesFunctions));
}

JSClassID JSHTMLHeadingElement::js_class_id = 0;

void JSHTMLHeadingElement::init(JSContext* ctx)
{
    if (JSHTMLHeadingElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLHeadingElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLHeadingElement::create(JSContext* ctx, HTMLHeadingElement* impl)
{
    JSHTMLHeadingElement::init(ctx);
    JSValue _proto = JSHTMLHeadingElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLHeadingElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLHeadingElement* impl = (HTMLHeadingElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLHeadingElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLHeadingElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLHeadingElement* imp = (HTMLHeadingElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLHeadingElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLHeadingElement* imp = (HTMLHeadingElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLHeadingElement::getConstructor(JSContext *ctx)
{
    return JSHTMLHeadingElementConstructor::self(ctx);
}


}
