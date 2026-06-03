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

#include "QJSHTMLParagraphElement.h"

#include "HTMLParagraphElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLParagraphElementAttributesFunctions[2];
static bool JSHTMLParagraphElementAttributesFunctions_initialized = false;

static void init_JSHTMLParagraphElementAttributesFunctions()
{
    if (JSHTMLParagraphElementAttributesFunctions_initialized) return;
    JSHTMLParagraphElementAttributesFunctions_initialized = true;
    memset(JSHTMLParagraphElementAttributesFunctions, 0, sizeof(JSHTMLParagraphElementAttributesFunctions));
    JSHTMLParagraphElementAttributesFunctions[0].name = "align";
    JSHTMLParagraphElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParagraphElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParagraphElementAttributesFunctions[0].magic = JSHTMLParagraphElement::AlignAttrNum;
    JSHTMLParagraphElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLParagraphElement::getValueProperty;
    JSHTMLParagraphElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLParagraphElement::putValueProperty;
    JSHTMLParagraphElementAttributesFunctions[1].name = "constructor";
    JSHTMLParagraphElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParagraphElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParagraphElementAttributesFunctions[1].magic = JSHTMLParagraphElement::ConstructorAttrNum;
    JSHTMLParagraphElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLParagraphElement::getValueProperty;
    JSHTMLParagraphElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLParagraphElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLParagraphElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLParagraphElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLParagraphElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLParagraphElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLParagraphElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLParagraphElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLParagraphElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLParagraphElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLParagraphElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLParagraphElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLParagraphElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLParagraphElementAttributesFunctions, countof(JSHTMLParagraphElementAttributesFunctions));
}

JSClassID JSHTMLParagraphElement::js_class_id = 0;

void JSHTMLParagraphElement::init(JSContext* ctx)
{
    if (JSHTMLParagraphElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLParagraphElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLParagraphElement::create(JSContext* ctx, HTMLParagraphElement* impl)
{
    JSHTMLParagraphElement::init(ctx);
    JSValue _proto = JSHTMLParagraphElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLParagraphElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLParagraphElement* impl = (HTMLParagraphElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLParagraphElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLParagraphElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLParagraphElement* imp = (HTMLParagraphElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLParagraphElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLParagraphElement* imp = (HTMLParagraphElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLParagraphElement::getConstructor(JSContext *ctx)
{
    return JSHTMLParagraphElementConstructor::self(ctx);
}


}
