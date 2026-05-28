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

#include "QJSHTMLButtonElement.h"

#include "HTMLButtonElement.h"
#include "HTMLFormElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLButtonElementAttributesFunctions[8];
static bool JSHTMLButtonElementAttributesFunctions_initialized = false;

static void init_JSHTMLButtonElementAttributesFunctions()
{
    if (JSHTMLButtonElementAttributesFunctions_initialized) return;
    JSHTMLButtonElementAttributesFunctions_initialized = true;
    memset(JSHTMLButtonElementAttributesFunctions, 0, sizeof(JSHTMLButtonElementAttributesFunctions));
    JSHTMLButtonElementAttributesFunctions[0].name = "disabled";
    JSHTMLButtonElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[0].magic = JSHTMLButtonElement::DisabledAttrNum;
    JSHTMLButtonElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLButtonElement::putValueProperty;
    JSHTMLButtonElementAttributesFunctions[1].name = "type";
    JSHTMLButtonElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[1].magic = JSHTMLButtonElement::TypeAttrNum;
    JSHTMLButtonElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSHTMLButtonElementAttributesFunctions[2].name = "form";
    JSHTMLButtonElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[2].magic = JSHTMLButtonElement::FormAttrNum;
    JSHTMLButtonElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLButtonElementAttributesFunctions[3].name = "accessKey";
    JSHTMLButtonElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[3].magic = JSHTMLButtonElement::AccessKeyAttrNum;
    JSHTMLButtonElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLButtonElement::putValueProperty;
    JSHTMLButtonElementAttributesFunctions[4].name = "name";
    JSHTMLButtonElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[4].magic = JSHTMLButtonElement::NameAttrNum;
    JSHTMLButtonElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLButtonElement::putValueProperty;
    JSHTMLButtonElementAttributesFunctions[5].name = "tabIndex";
    JSHTMLButtonElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[5].magic = JSHTMLButtonElement::TabIndexAttrNum;
    JSHTMLButtonElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLButtonElement::putValueProperty;
    JSHTMLButtonElementAttributesFunctions[6].name = "value";
    JSHTMLButtonElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[6].magic = JSHTMLButtonElement::ValueAttrNum;
    JSHTMLButtonElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLButtonElement::putValueProperty;
    JSHTMLButtonElementAttributesFunctions[7].name = "constructor";
    JSHTMLButtonElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLButtonElementAttributesFunctions[7].magic = JSHTMLButtonElement::ConstructorAttrNum;
    JSHTMLButtonElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLButtonElement::getValueProperty;
    JSHTMLButtonElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSHTMLButtonElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLButtonElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLButtonElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLButtonElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLButtonElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLButtonElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLButtonElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLButtonElementPrototypeFunctions[1];
static bool JSHTMLButtonElementPrototypeFunctions_initialized = false;

static void init_JSHTMLButtonElementPrototypeFunctions()
{
    if (JSHTMLButtonElementPrototypeFunctions_initialized) return;
    JSHTMLButtonElementPrototypeFunctions_initialized = true;
    memset(JSHTMLButtonElementPrototypeFunctions, 0, sizeof(JSHTMLButtonElementPrototypeFunctions));
    JSHTMLButtonElementPrototypeFunctions[0].name = "click";
    JSHTMLButtonElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLButtonElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLButtonElementPrototypeFunctions[0].magic = JSHTMLButtonElement::ClickFuncNum;
    JSHTMLButtonElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLButtonElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLButtonElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLButtonElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLButtonElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLButtonElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLButtonElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLButtonElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLButtonElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLButtonElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLButtonElementAttributesFunctions, countof(JSHTMLButtonElementAttributesFunctions));
    init_JSHTMLButtonElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLButtonElementPrototypeFunctions, countof(JSHTMLButtonElementPrototypeFunctions));
}

static JSClassDef JSHTMLButtonElementClassDefine;
static bool JSHTMLButtonElementClassDefine_initialized = false;

static void init_JSHTMLButtonElementClassDefine()
{
    if (JSHTMLButtonElementClassDefine_initialized) return;
    JSHTMLButtonElementClassDefine_initialized = true;
    memset(&JSHTMLButtonElementClassDefine, 0, sizeof(JSHTMLButtonElementClassDefine));
    JSHTMLButtonElementClassDefine.class_name = "HTMLButtonElement";
    JSHTMLButtonElementClassDefine.finalizer = JSHTMLButtonElement::finalizer;
    JSHTMLButtonElementClassDefine.gc_mark = JSHTMLButtonElement::mark;
}

JSClassID JSHTMLButtonElement::js_class_id = 0;

void JSHTMLButtonElement::init(JSContext* ctx)
{
    if (JSHTMLButtonElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLButtonElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLButtonElement::create(JSContext* ctx, HTMLButtonElement* impl)
{
    JSHTMLButtonElement::init(ctx);
    JSValue _proto = JSHTMLButtonElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLButtonElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLButtonElement* impl = (HTMLButtonElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLButtonElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLButtonElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case DisabledAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLButtonElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AccessKeyAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLButtonElement::getConstructor(JSContext *ctx)
{
    return JSHTMLButtonElementConstructor::self(ctx);
}

JSValue JSHTMLButtonElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLButtonElement::ClickFuncNum: {
            imp->click();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
