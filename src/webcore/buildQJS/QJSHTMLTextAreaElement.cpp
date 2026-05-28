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

static JSCFunctionListEntry JSHTMLTextAreaElementAttributesFunctions[14];
static bool JSHTMLTextAreaElementAttributesFunctions_initialized = false;

static void init_JSHTMLTextAreaElementAttributesFunctions()
{
    if (JSHTMLTextAreaElementAttributesFunctions_initialized) return;
    JSHTMLTextAreaElementAttributesFunctions_initialized = true;
    memset(JSHTMLTextAreaElementAttributesFunctions, 0, sizeof(JSHTMLTextAreaElementAttributesFunctions));
    JSHTMLTextAreaElementAttributesFunctions[0].name = "defaultValue";
    JSHTMLTextAreaElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[0].magic = JSHTMLTextAreaElement::DefaultValueAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[1].name = "form";
    JSHTMLTextAreaElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[1].magic = JSHTMLTextAreaElement::FormAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSHTMLTextAreaElementAttributesFunctions[2].name = "accessKey";
    JSHTMLTextAreaElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[2].magic = JSHTMLTextAreaElement::AccessKeyAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[3].name = "cols";
    JSHTMLTextAreaElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[3].magic = JSHTMLTextAreaElement::ColsAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[4].name = "disabled";
    JSHTMLTextAreaElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[4].magic = JSHTMLTextAreaElement::DisabledAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[5].name = "name";
    JSHTMLTextAreaElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[5].magic = JSHTMLTextAreaElement::NameAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[6].name = "readOnly";
    JSHTMLTextAreaElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[6].magic = JSHTMLTextAreaElement::ReadOnlyAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[7].name = "rows";
    JSHTMLTextAreaElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[7].magic = JSHTMLTextAreaElement::RowsAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[8].name = "tabIndex";
    JSHTMLTextAreaElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[8].magic = JSHTMLTextAreaElement::TabIndexAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[9].name = "type";
    JSHTMLTextAreaElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[9].magic = JSHTMLTextAreaElement::TypeAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLTextAreaElementAttributesFunctions[10].name = "value";
    JSHTMLTextAreaElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[10].magic = JSHTMLTextAreaElement::ValueAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[11].name = "selectionStart";
    JSHTMLTextAreaElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[11].magic = JSHTMLTextAreaElement::SelectionStartAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[12].name = "selectionEnd";
    JSHTMLTextAreaElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[12].magic = JSHTMLTextAreaElement::SelectionEndAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLTextAreaElement::putValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[13].name = "constructor";
    JSHTMLTextAreaElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTextAreaElementAttributesFunctions[13].magic = JSHTMLTextAreaElement::ConstructorAttrNum;
    JSHTMLTextAreaElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLTextAreaElement::getValueProperty;
    JSHTMLTextAreaElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

class JSHTMLTextAreaElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTextAreaElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTextAreaElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTextAreaElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTextAreaElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTextAreaElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLTextAreaElementPrototypeFunctions[4];
static bool JSHTMLTextAreaElementPrototypeFunctions_initialized = false;

static void init_JSHTMLTextAreaElementPrototypeFunctions()
{
    if (JSHTMLTextAreaElementPrototypeFunctions_initialized) return;
    JSHTMLTextAreaElementPrototypeFunctions_initialized = true;
    memset(JSHTMLTextAreaElementPrototypeFunctions, 0, sizeof(JSHTMLTextAreaElementPrototypeFunctions));
    JSHTMLTextAreaElementPrototypeFunctions[0].name = "blur";
    JSHTMLTextAreaElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLTextAreaElementPrototypeFunctions[0].magic = JSHTMLTextAreaElement::BlurFuncNum;
    JSHTMLTextAreaElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLTextAreaElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTextAreaElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLTextAreaElementPrototypeFunction::callAsFunction;
    JSHTMLTextAreaElementPrototypeFunctions[1].name = "focus";
    JSHTMLTextAreaElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLTextAreaElementPrototypeFunctions[1].magic = JSHTMLTextAreaElement::FocusFuncNum;
    JSHTMLTextAreaElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLTextAreaElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTextAreaElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLTextAreaElementPrototypeFunction::callAsFunction;
    JSHTMLTextAreaElementPrototypeFunctions[2].name = "select";
    JSHTMLTextAreaElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLTextAreaElementPrototypeFunctions[2].magic = JSHTMLTextAreaElement::SelectFuncNum;
    JSHTMLTextAreaElementPrototypeFunctions[2].u.func.length = 0;
    JSHTMLTextAreaElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTextAreaElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLTextAreaElementPrototypeFunction::callAsFunction;
    JSHTMLTextAreaElementPrototypeFunctions[3].name = "setSelectionRange";
    JSHTMLTextAreaElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTextAreaElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSHTMLTextAreaElementPrototypeFunctions[3].magic = JSHTMLTextAreaElement::SetSelectionRangeFuncNum;
    JSHTMLTextAreaElementPrototypeFunctions[3].u.func.length = 2;
    JSHTMLTextAreaElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTextAreaElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSHTMLTextAreaElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLTextAreaElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTextAreaElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTextAreaElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTextAreaElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTextAreaElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTextAreaElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTextAreaElementAttributesFunctions, countof(JSHTMLTextAreaElementAttributesFunctions));
    init_JSHTMLTextAreaElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTextAreaElementPrototypeFunctions, countof(JSHTMLTextAreaElementPrototypeFunctions));
}

static JSClassDef JSHTMLTextAreaElementClassDefine;
static bool JSHTMLTextAreaElementClassDefine_initialized = false;

static void init_JSHTMLTextAreaElementClassDefine()
{
    if (JSHTMLTextAreaElementClassDefine_initialized) return;
    JSHTMLTextAreaElementClassDefine_initialized = true;
    memset(&JSHTMLTextAreaElementClassDefine, 0, sizeof(JSHTMLTextAreaElementClassDefine));
    JSHTMLTextAreaElementClassDefine.class_name = "HTMLTextAreaElement";
    JSHTMLTextAreaElementClassDefine.finalizer = JSHTMLTextAreaElement::finalizer;
    JSHTMLTextAreaElementClassDefine.gc_mark = JSHTMLTextAreaElement::mark;
}

JSClassID JSHTMLTextAreaElement::js_class_id = 0;

void JSHTMLTextAreaElement::init(JSContext* ctx)
{
    if (JSHTMLTextAreaElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTextAreaElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTextAreaElement::create(JSContext* ctx, HTMLTextAreaElement* impl)
{
    JSHTMLTextAreaElement::init(ctx);
    JSValue _proto = JSHTMLTextAreaElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTextAreaElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTextAreaElement* impl = (HTMLTextAreaElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->defaultValue()).utf8().data());
        }
        case FormAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case AccessKeyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->accessKey()).utf8().data());
        }
        case ColsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->cols());
        }
        case DisabledAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ReadOnlyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->readOnly() ? 1 : 0);
        }
        case RowsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->rows());
        }
        case TabIndexAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case SelectionStartAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->selectionStart());
        }
        case SelectionEndAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->selectionEnd());
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
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDefaultValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AccessKeyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAccessKey(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ColsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCols(valueToInt32(ctx, value));
            break;
        }
        case DisabledAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ReadOnlyAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setReadOnly(valueToBoolean(ctx, value));
            break;
        }
        case RowsAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRows(valueToInt32(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectionStartAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSelectionStart(valueToInt32(ctx, value));
            break;
        }
        case SelectionEndAttrNum: {
            HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLTextAreaElement* imp = (HTMLTextAreaElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
