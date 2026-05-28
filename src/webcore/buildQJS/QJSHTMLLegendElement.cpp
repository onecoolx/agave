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

#include "QJSHTMLLegendElement.h"

#include "HTMLFormElement.h"
#include "HTMLLegendElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLLegendElementAttributesFunctions[4];
static bool JSHTMLLegendElementAttributesFunctions_initialized = false;

static void init_JSHTMLLegendElementAttributesFunctions()
{
    if (JSHTMLLegendElementAttributesFunctions_initialized) return;
    JSHTMLLegendElementAttributesFunctions_initialized = true;
    memset(JSHTMLLegendElementAttributesFunctions, 0, sizeof(JSHTMLLegendElementAttributesFunctions));
    JSHTMLLegendElementAttributesFunctions[0].name = "form";
    JSHTMLLegendElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLegendElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLegendElementAttributesFunctions[0].magic = JSHTMLLegendElement::FormAttrNum;
    JSHTMLLegendElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLLegendElement::getValueProperty;
    JSHTMLLegendElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLLegendElementAttributesFunctions[1].name = "accessKey";
    JSHTMLLegendElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLegendElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLegendElementAttributesFunctions[1].magic = JSHTMLLegendElement::AccessKeyAttrNum;
    JSHTMLLegendElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLLegendElement::getValueProperty;
    JSHTMLLegendElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLLegendElement::putValueProperty;
    JSHTMLLegendElementAttributesFunctions[2].name = "align";
    JSHTMLLegendElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLegendElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLegendElementAttributesFunctions[2].magic = JSHTMLLegendElement::AlignAttrNum;
    JSHTMLLegendElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLLegendElement::getValueProperty;
    JSHTMLLegendElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLLegendElement::putValueProperty;
    JSHTMLLegendElementAttributesFunctions[3].name = "constructor";
    JSHTMLLegendElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLLegendElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLLegendElementAttributesFunctions[3].magic = JSHTMLLegendElement::ConstructorAttrNum;
    JSHTMLLegendElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLLegendElement::getValueProperty;
    JSHTMLLegendElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSHTMLLegendElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLLegendElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLLegendElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLLegendElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLLegendElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLegendElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLLegendElementPrototypeFunctions[1];
static bool JSHTMLLegendElementPrototypeFunctions_initialized = false;

static void init_JSHTMLLegendElementPrototypeFunctions()
{
    if (JSHTMLLegendElementPrototypeFunctions_initialized) return;
    JSHTMLLegendElementPrototypeFunctions_initialized = true;
    memset(JSHTMLLegendElementPrototypeFunctions, 0, sizeof(JSHTMLLegendElementPrototypeFunctions));
    JSHTMLLegendElementPrototypeFunctions[0].name = "focus";
    JSHTMLLegendElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLLegendElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLLegendElementPrototypeFunctions[0].magic = JSHTMLLegendElement::FocusFuncNum;
    JSHTMLLegendElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLLegendElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLLegendElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLLegendElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLLegendElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLLegendElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLLegendElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLLegendElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLLegendElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLLegendElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLegendElementAttributesFunctions, countof(JSHTMLLegendElementAttributesFunctions));
    init_JSHTMLLegendElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLLegendElementPrototypeFunctions, countof(JSHTMLLegendElementPrototypeFunctions));
}

static JSClassDef JSHTMLLegendElementClassDefine;
static bool JSHTMLLegendElementClassDefine_initialized = false;

static void init_JSHTMLLegendElementClassDefine()
{
    if (JSHTMLLegendElementClassDefine_initialized) return;
    JSHTMLLegendElementClassDefine_initialized = true;
    memset(&JSHTMLLegendElementClassDefine, 0, sizeof(JSHTMLLegendElementClassDefine));
    JSHTMLLegendElementClassDefine.class_name = "HTMLLegendElement";
    JSHTMLLegendElementClassDefine.finalizer = JSHTMLLegendElement::finalizer;
    JSHTMLLegendElementClassDefine.gc_mark = JSHTMLLegendElement::mark;
}

JSClassID JSHTMLLegendElement::js_class_id = 0;

void JSHTMLLegendElement::init(JSContext* ctx)
{
    if (JSHTMLLegendElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLLegendElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLLegendElement::create(JSContext* ctx, HTMLLegendElement* impl)
{
    JSHTMLLegendElement::init(ctx);
    JSValue _proto = JSHTMLLegendElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLLegendElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLLegendElement* impl = (HTMLLegendElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLLegendElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLLegendElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLLegendElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLLegendElement::getConstructor(JSContext *ctx)
{
    return JSHTMLLegendElementConstructor::self(ctx);
}

JSValue JSHTMLLegendElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLLegendElement* imp = (HTMLLegendElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLLegendElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
