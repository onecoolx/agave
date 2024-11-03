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

#include "QJSHTMLTextAreaElement.h"

#include "ExceptionCode.h"
#include "HTMLFormElement.h"
#include "HTMLTextAreaElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTextAreaElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultValue", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::DefaultValueAttrNum),
    JS_CGETSET_MAGIC_DEF("rows", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::RowsAttrNum),
    JS_CGETSET_MAGIC_DEF("accessKey", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::AccessKeyAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLTextAreaElement::getValueProperty, NULL, JSHTMLTextAreaElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("readOnly", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::ReadOnlyAttrNum),
    JS_CGETSET_MAGIC_DEF("cols", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::ColsAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLTextAreaElement::getValueProperty, NULL, JSHTMLTextAreaElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("selectionStart", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::SelectionStartAttrNum),
    JS_CGETSET_MAGIC_DEF("selectionEnd", JSHTMLTextAreaElement::getValueProperty, JSHTMLTextAreaElement::putValueProperty, JSHTMLTextAreaElement::SelectionEndAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTextAreaElement::getValueProperty, NULL, JSHTMLTextAreaElement::ConstructorAttrNum)
};

class JSHTMLTextAreaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTextAreaElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTextAreaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTextAreaElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTextAreaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTextAreaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTextAreaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLTextAreaElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLTextAreaElementPrototypeFunction::callAsFunction, JSHTMLTextAreaElement::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("blur", 0, JSHTMLTextAreaElementPrototypeFunction::callAsFunction, JSHTMLTextAreaElement::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("select", 0, JSHTMLTextAreaElementPrototypeFunction::callAsFunction, JSHTMLTextAreaElement::SelectFuncNum),
    JS_CFUNC_MAGIC_DEF("setSelectionRange", 2, JSHTMLTextAreaElementPrototypeFunction::callAsFunction, JSHTMLTextAreaElement::SetSelectionRangeFuncNum)
};

JSValue JSHTMLTextAreaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTextAreaElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTextAreaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTextAreaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTextAreaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTextAreaElementAttributesFunctions, countof(JSHTMLTextAreaElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTextAreaElementPrototypeFunctions, countof(JSHTMLTextAreaElementPrototypeFunctions));
}

static JSClassDef JSHTMLTextAreaElementClassDefine = 
{
    "HTMLTextAreaElement",
    .finalizer = JSHTMLTextAreaElement::finalizer,
    .gc_mark = JSHTMLTextAreaElement::mark,
};

JSClassID JSHTMLTextAreaElement::js_class_id = 0;

void JSHTMLTextAreaElement::init(JSContext* ctx)
{
    if (JSHTMLTextAreaElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTextAreaElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTextAreaElement::js_class_id, &JSHTMLTextAreaElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTextAreaElementConstructor::self(ctx), JSHTMLTextAreaElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTextAreaElement::js_class_id, JSHTMLTextAreaElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTextAreaElement::create(JSContext* ctx, HTMLTextAreaElement* impl)
{
    JSHTMLTextAreaElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTextAreaElementPrototype::self(ctx), JSHTMLTextAreaElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTextAreaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTextAreaElement* impl = (HTMLTextAreaElement*)JS_GetOpaque(val, JSHTMLTextAreaElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTextAreaElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTextAreaElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultValue()).utf8().data());
        }
        case FormAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case ColsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->cols());
        }
        case DisabledAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ReadOnlyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBool(ctx, imp->readOnly() ? 1 : 0);
        }
        case RowsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->rows());
        }
        case TabIndexAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case SelectionStartAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->selectionStart());
        }
        case SelectionEndAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->selectionEnd());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTextAreaElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DefaultValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setDefaultValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AccessKeyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ColsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setCols(valueToInt32(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ReadOnlyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setReadOnly(valueToBoolean(ctx, value));
            break;
        }
        case RowsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setRows(valueToInt32(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectionStartAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setSelectionStart(valueToInt32(ctx, value));
            break;
        }
        case SelectionEndAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
            imp->setSelectionEnd(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTextAreaElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTextAreaElementConstructor::self(ctx);
}

JSValue JSHTMLTextAreaElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTextAreaElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLTextAreaElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSHTMLTextAreaElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSHTMLTextAreaElement::SelectFuncNum: {
            imp->select();
            return JS_UNDEFINED;
        }
        case JSHTMLTextAreaElement::SetSelectionRangeFuncNum: {
            bool startOk;
            int start = valueToInt32(ctx, argv[0], startOk);
            if (!startOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool endOk;
            int end = valueToInt32(ctx, argv[1], endOk);
            if (!endOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->setSelectionRange(start, end);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
