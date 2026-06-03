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

#include "QJSHTMLIsIndexElement.h"

#include "HTMLFormElement.h"
#include "HTMLIsIndexElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLIsIndexElementAttributesFunctions[3];
static bool JSHTMLIsIndexElementAttributesFunctions_initialized = false;

static void init_JSHTMLIsIndexElementAttributesFunctions()
{
    if (JSHTMLIsIndexElementAttributesFunctions_initialized) return;
    JSHTMLIsIndexElementAttributesFunctions_initialized = true;
    memset(JSHTMLIsIndexElementAttributesFunctions, 0, sizeof(JSHTMLIsIndexElementAttributesFunctions));
    JSHTMLIsIndexElementAttributesFunctions[0].name = "form";
    JSHTMLIsIndexElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIsIndexElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIsIndexElementAttributesFunctions[0].magic = JSHTMLIsIndexElement::FormAttrNum;
    JSHTMLIsIndexElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLIsIndexElement::getValueProperty;
    JSHTMLIsIndexElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLIsIndexElementAttributesFunctions[1].name = "prompt";
    JSHTMLIsIndexElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIsIndexElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIsIndexElementAttributesFunctions[1].magic = JSHTMLIsIndexElement::PromptAttrNum;
    JSHTMLIsIndexElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLIsIndexElement::getValueProperty;
    JSHTMLIsIndexElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLIsIndexElement::putValueProperty;
    JSHTMLIsIndexElementAttributesFunctions[2].name = "constructor";
    JSHTMLIsIndexElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLIsIndexElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLIsIndexElementAttributesFunctions[2].magic = JSHTMLIsIndexElement::ConstructorAttrNum;
    JSHTMLIsIndexElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLIsIndexElement::getValueProperty;
    JSHTMLIsIndexElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSHTMLIsIndexElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLIsIndexElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLIsIndexElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLIsIndexElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLIsIndexElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLIsIndexElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLIsIndexElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLIsIndexElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLInputElementPrototype::self(ctx));
        JSHTMLIsIndexElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLIsIndexElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLIsIndexElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLIsIndexElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLIsIndexElementAttributesFunctions, countof(JSHTMLIsIndexElementAttributesFunctions));
}

JSClassID JSHTMLIsIndexElement::js_class_id = 0;

void JSHTMLIsIndexElement::init(JSContext* ctx)
{
    if (JSHTMLIsIndexElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLIsIndexElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLIsIndexElement::create(JSContext* ctx, HTMLIsIndexElement* impl)
{
    JSHTMLIsIndexElement::init(ctx);
    JSValue _proto = JSHTMLIsIndexElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLIsIndexElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLIsIndexElement* impl = (HTMLIsIndexElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLIsIndexElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLInputElement::mark(rt, val, mark_func);
}

JSValue JSHTMLIsIndexElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case PromptAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->prompt()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLIsIndexElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case PromptAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setPrompt(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLIsIndexElement::getConstructor(JSContext *ctx)
{
    return JSHTMLIsIndexElementConstructor::self(ctx);
}


}
