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

#include "QJSHTMLTitleElement.h"

#include "HTMLTitleElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTitleElementAttributesFunctions[2];
static bool JSHTMLTitleElementAttributesFunctions_initialized = false;

static void init_JSHTMLTitleElementAttributesFunctions()
{
    if (JSHTMLTitleElementAttributesFunctions_initialized) return;
    JSHTMLTitleElementAttributesFunctions_initialized = true;
    memset(JSHTMLTitleElementAttributesFunctions, 0, sizeof(JSHTMLTitleElementAttributesFunctions));
    JSHTMLTitleElementAttributesFunctions[0].name = "text";
    JSHTMLTitleElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTitleElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTitleElementAttributesFunctions[0].magic = JSHTMLTitleElement::TextAttrNum;
    JSHTMLTitleElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTitleElement::getValueProperty;
    JSHTMLTitleElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTitleElement::putValueProperty;
    JSHTMLTitleElementAttributesFunctions[1].name = "constructor";
    JSHTMLTitleElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTitleElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTitleElementAttributesFunctions[1].magic = JSHTMLTitleElement::ConstructorAttrNum;
    JSHTMLTitleElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTitleElement::getValueProperty;
    JSHTMLTitleElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLTitleElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTitleElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTitleElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTitleElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTitleElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTitleElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTitleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTitleElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTitleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTitleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTitleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTitleElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTitleElementAttributesFunctions, countof(JSHTMLTitleElementAttributesFunctions));
}

JSClassID JSHTMLTitleElement::js_class_id = 0;

void JSHTMLTitleElement::init(JSContext* ctx)
{
    if (JSHTMLTitleElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTitleElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTitleElement::create(JSContext* ctx, HTMLTitleElement* impl)
{
    JSHTMLTitleElement::init(ctx);
    JSValue _proto = JSHTMLTitleElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTitleElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTitleElement* impl = (HTMLTitleElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLTitleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTitleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLTitleElement* imp = (HTMLTitleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTitleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLTitleElement* imp = (HTMLTitleElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTitleElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTitleElementConstructor::self(ctx);
}


}
