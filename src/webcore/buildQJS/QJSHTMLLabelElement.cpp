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

#include "QJSHTMLLabelElement.h"

#include "HTMLFormElement.h"
#include "HTMLLabelElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLLabelElementAttributesFunctions[4];
static bool JSHTMLLabelElementAttributesFunctions_initialized = false;

static void init_JSHTMLLabelElementAttributesFunctions()
{
    if (JSHTMLLabelElementAttributesFunctions_initialized) return;
    JSHTMLLabelElementAttributesFunctions_initialized = true;
    memset(JSHTMLLabelElementAttributesFunctions, 0, sizeof(JSHTMLLabelElementAttributesFunctions));
    JSHTMLLabelElementAttributesFunctions[0].name = "form";
    JSHTMLLabelElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLabelElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLabelElementAttributesFunctions[0].magic = JSHTMLLabelElement::FormAttrNum;
    JSHTMLLabelElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLLabelElement::getValueProperty;
    JSHTMLLabelElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLLabelElementAttributesFunctions[1].name = "accessKey";
    JSHTMLLabelElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLabelElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLabelElementAttributesFunctions[1].magic = JSHTMLLabelElement::AccessKeyAttrNum;
    JSHTMLLabelElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLLabelElement::getValueProperty;
    JSHTMLLabelElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLLabelElement::putValueProperty;
    JSHTMLLabelElementAttributesFunctions[2].name = "htmlFor";
    JSHTMLLabelElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLabelElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLabelElementAttributesFunctions[2].magic = JSHTMLLabelElement::HtmlForAttrNum;
    JSHTMLLabelElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLLabelElement::getValueProperty;
    JSHTMLLabelElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLLabelElement::putValueProperty;
    JSHTMLLabelElementAttributesFunctions[3].name = "constructor";
    JSHTMLLabelElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLabelElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLabelElementAttributesFunctions[3].magic = JSHTMLLabelElement::ConstructorAttrNum;
    JSHTMLLabelElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLLabelElement::getValueProperty;
    JSHTMLLabelElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSHTMLLabelElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLLabelElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLabelElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLabelElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLabelElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLabelElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLLabelElementPrototypeFunctions[1];
static bool JSHTMLLabelElementPrototypeFunctions_initialized = false;

static void init_JSHTMLLabelElementPrototypeFunctions()
{
    if (JSHTMLLabelElementPrototypeFunctions_initialized) return;
    JSHTMLLabelElementPrototypeFunctions_initialized = true;
    memset(JSHTMLLabelElementPrototypeFunctions, 0, sizeof(JSHTMLLabelElementPrototypeFunctions));
    JSHTMLLabelElementPrototypeFunctions[0].name = "focus";
    JSHTMLLabelElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLLabelElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLLabelElementPrototypeFunctions[0].magic = JSHTMLLabelElement::FocusFuncNum;
    JSHTMLLabelElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLLabelElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLLabelElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLLabelElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLLabelElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLabelElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLabelElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLabelElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLabelElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLLabelElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLabelElementAttributesFunctions, countof(JSHTMLLabelElementAttributesFunctions));
    init_JSHTMLLabelElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLabelElementPrototypeFunctions, countof(JSHTMLLabelElementPrototypeFunctions));
}

JSClassID JSHTMLLabelElement::js_class_id = 0;

void JSHTMLLabelElement::init(JSContext* ctx)
{
    if (JSHTMLLabelElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLLabelElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLLabelElement::create(JSContext* ctx, HTMLLabelElement* impl)
{
    JSHTMLLabelElement::init(ctx);
    JSValue _proto = JSHTMLLabelElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLabelElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLabelElement* impl = (HTMLLabelElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLabelElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLabelElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case HtmlForAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->htmlFor()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLabelElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HtmlForAttrNum: {
            HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHtmlFor(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLabelElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLabelElementConstructor::self(ctx);
}

JSValue JSHTMLLabelElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLLabelElement* imp = (HTMLLabelElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLLabelElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
