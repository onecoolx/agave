/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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

#include "QJSHTMLButtonElement.h"

#include "HTMLButtonElement.h"
#include "HTMLFormElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLButtonElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLButtonElement::getValueProperty, JSHTMLButtonElement::putValueProperty, JSHTMLButtonElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLButtonElement::getValueProperty, NULL, JSHTMLButtonElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLButtonElement::getValueProperty, NULL, JSHTMLButtonElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLButtonElement::getValueProperty, JSHTMLButtonElement::putValueProperty, JSHTMLButtonElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLButtonElement::getValueProperty, JSHTMLButtonElement::putValueProperty, JSHTMLButtonElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLButtonElement::getValueProperty, JSHTMLButtonElement::putValueProperty, JSHTMLButtonElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLButtonElement::getValueProperty, JSHTMLButtonElement::putValueProperty, JSHTMLButtonElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLButtonElement::getValueProperty, NULL, JSHTMLButtonElement::ConstructorAttrNum)
};

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
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLButtonElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLButtonElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLButtonElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLButtonElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("click", 0, JSHTMLButtonElementPrototypeFunction::callAsFunction, JSHTMLButtonElement::ClickFuncNum)
};

JSValue JSHTMLButtonElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLButtonElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLButtonElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLButtonElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLButtonElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLButtonElementAttributesFunctions, countof(JSHTMLButtonElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLButtonElementPrototypeFunctions, countof(JSHTMLButtonElementPrototypeFunctions));
}

static JSClassDef JSHTMLButtonElementClassDefine = 
{
    "HTMLButtonElement",
    .finalizer = JSHTMLButtonElement::finalizer,
    .gc_mark = JSHTMLButtonElement::mark,
};

JSClassID JSHTMLButtonElement::js_class_id = 0;

void JSHTMLButtonElement::init(JSContext* ctx)
{
    if (JSHTMLButtonElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLButtonElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLButtonElement::js_class_id, &JSHTMLButtonElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLButtonElementConstructor::self(ctx), JSHTMLButtonElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLButtonElement::js_class_id, JSHTMLButtonElementPrototype::self(ctx));
    }
}

JSValue JSHTMLButtonElement::create(JSContext* ctx, HTMLButtonElement* impl)
{
    JSHTMLButtonElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLButtonElementPrototype::self(ctx), JSHTMLButtonElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLButtonElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLButtonElement* impl = (HTMLButtonElement*)JS_GetOpaque(val, JSHTMLButtonElement::js_class_id);
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
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case DisabledAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
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
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
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
    HTMLButtonElement* imp = (HTMLButtonElement*)JS_GetOpaque2(ctx, this_val, JSHTMLButtonElement::js_class_id);
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
