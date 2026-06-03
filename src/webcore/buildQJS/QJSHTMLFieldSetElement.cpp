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

#include "QJSHTMLFieldSetElement.h"

#include "HTMLFieldSetElement.h"
#include "HTMLFormElement.h"
#include "QJSHTMLFormElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLFieldSetElementAttributesFunctions[2];
static bool JSHTMLFieldSetElementAttributesFunctions_initialized = false;

static void init_JSHTMLFieldSetElementAttributesFunctions()
{
    if (JSHTMLFieldSetElementAttributesFunctions_initialized) return;
    JSHTMLFieldSetElementAttributesFunctions_initialized = true;
    memset(JSHTMLFieldSetElementAttributesFunctions, 0, sizeof(JSHTMLFieldSetElementAttributesFunctions));
    JSHTMLFieldSetElementAttributesFunctions[0].name = "form";
    JSHTMLFieldSetElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFieldSetElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFieldSetElementAttributesFunctions[0].magic = JSHTMLFieldSetElement::FormAttrNum;
    JSHTMLFieldSetElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLFieldSetElement::getValueProperty;
    JSHTMLFieldSetElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLFieldSetElementAttributesFunctions[1].name = "constructor";
    JSHTMLFieldSetElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFieldSetElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFieldSetElementAttributesFunctions[1].magic = JSHTMLFieldSetElement::ConstructorAttrNum;
    JSHTMLFieldSetElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLFieldSetElement::getValueProperty;
    JSHTMLFieldSetElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLFieldSetElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLFieldSetElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFieldSetElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFieldSetElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFieldSetElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFieldSetElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFieldSetElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFieldSetElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFieldSetElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFieldSetElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFieldSetElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLFieldSetElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFieldSetElementAttributesFunctions, countof(JSHTMLFieldSetElementAttributesFunctions));
}

JSClassID JSHTMLFieldSetElement::js_class_id = 0;

void JSHTMLFieldSetElement::init(JSContext* ctx)
{
    if (JSHTMLFieldSetElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLFieldSetElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLFieldSetElement::create(JSContext* ctx, HTMLFieldSetElement* impl)
{
    JSHTMLFieldSetElement::init(ctx);
    JSValue _proto = JSHTMLFieldSetElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFieldSetElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFieldSetElement* impl = (HTMLFieldSetElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLFieldSetElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFieldSetElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLFieldSetElement* imp = (HTMLFieldSetElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFieldSetElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFieldSetElementConstructor::self(ctx);
}


}
