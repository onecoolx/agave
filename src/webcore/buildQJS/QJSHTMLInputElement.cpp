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

#include "QJSHTMLInputElement.h"

#include "HTMLFormElement.h"
#include "HTMLInputElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLInputElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("defaultChecked", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::DefaultCheckedAttrNum),
    JS_CGETSET_MAGIC_DEF("indeterminate", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::IndeterminateAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultValue", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::DefaultValueAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("size", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::SizeAttrNum),
    JS_CGETSET_MAGIC_DEF("accept", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::AcceptAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("maxLength", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::MaxLengthAttrNum),
    JS_CGETSET_MAGIC_DEF("readOnly", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::ReadOnlyAttrNum),
    JS_CGETSET_MAGIC_DEF("alt", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::AltAttrNum),
    JS_CGETSET_MAGIC_DEF("checked", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::CheckedAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLInputElement::getValueProperty, NULL, JSHTMLInputElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLInputElement::getValueProperty, NULL, JSHTMLInputElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("src", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::SrcAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("useMap", JSHTMLInputElement::getValueProperty, JSHTMLInputElement::putValueProperty, JSHTMLInputElement::UseMapAttrNum)
};

class JSHTMLInputElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLInputElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLInputElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLInputElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLInputElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLInputElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLInputElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLInputElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("click", 0, JSHTMLInputElementPrototypeFunction::callAsFunction, JSHTMLInputElement::ClickFuncNum),
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLInputElementPrototypeFunction::callAsFunction, JSHTMLInputElement::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("blur", 0, JSHTMLInputElementPrototypeFunction::callAsFunction, JSHTMLInputElement::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("select", 0, JSHTMLInputElementPrototypeFunction::callAsFunction, JSHTMLInputElement::SelectFuncNum)
};

JSValue JSHTMLInputElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLInputElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLInputElementBasePrototype::self(ctx));
        JSHTMLInputElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLInputElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLInputElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLInputElementAttributesFunctions, countof(JSHTMLInputElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLInputElementPrototypeFunctions, countof(JSHTMLInputElementPrototypeFunctions));
}

static JSClassDef JSHTMLInputElementClassDefine = 
{
    "HTMLInputElement",
    .finalizer = JSHTMLInputElement::finalizer,
    .gc_mark = JSHTMLInputElement::mark,
};

JSClassID JSHTMLInputElement::js_class_id = 0;

void JSHTMLInputElement::init(JSContext* ctx)
{
    if (JSHTMLInputElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLInputElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLInputElement::js_class_id, &JSHTMLInputElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLInputElementConstructor::self(ctx), JSHTMLInputElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLInputElement::js_class_id, JSHTMLInputElementPrototype::self(ctx));
    }
}

JSValue JSHTMLInputElement::create(JSContext* ctx, HTMLInputElement* impl)
{
    JSHTMLInputElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLInputElementPrototype::self(ctx), JSHTMLInputElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLInputElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLInputElement* impl = (HTMLInputElement*)JS_GetOpaque(val, JSHTMLInputElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLInputElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLInputElementBase::mark(rt, val, mark_func);
}

JSValue JSHTMLInputElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultValue()).utf8().data());
        }
        case DefaultCheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBool(ctx, imp->defaultChecked() ? 1 : 0);
        }
        case FormAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AcceptAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accept()).utf8().data());
        }
        case AccessKeyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AltAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case CheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBool(ctx, imp->checked() ? 1 : 0);
        }
        case DisabledAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MaxLengthAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->maxLength());
        }
        case NameAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ReadOnlyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBool(ctx, imp->readOnly() ? 1 : 0);
        }
        case SizeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->size());
        }
        case SrcAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case UseMapAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->useMap()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case IndeterminateAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            return JS_NewBool(ctx, imp->indeterminate() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLInputElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setDefaultValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DefaultCheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setDefaultChecked(valueToBoolean(ctx, value));
            break;
        }
        case AcceptAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setAccept(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AccessKeyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CheckedAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setChecked(valueToBoolean(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MaxLengthAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setMaxLength(valueToInt32(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ReadOnlyAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setReadOnly(valueToBoolean(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case UseMapAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setUseMap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case IndeterminateAttrNum: {
            HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
            imp->setIndeterminate(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLInputElement::getConstructor(JSContext *ctx)
{
    return JSHTMLInputElementConstructor::self(ctx);
}

JSValue JSHTMLInputElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLInputElement* imp = (HTMLInputElement*)JS_GetOpaque2(ctx, this_val, JSHTMLInputElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLInputElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::SelectFuncNum: {
            imp->select();
            return JS_UNDEFINED;
        }
        case JSHTMLInputElement::ClickFuncNum: {
            imp->click();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
