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

#include "QJSHTMLTableRowElement.h"

#include "ExceptionCode.h"
#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "HTMLTableRowElement.h"
#include "QJSHTMLCollection.h"
#include "QJSHTMLElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTableRowElementAttributesFunctions[9];
static bool JSHTMLTableRowElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableRowElementAttributesFunctions()
{
    if (JSHTMLTableRowElementAttributesFunctions_initialized) return;
    JSHTMLTableRowElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableRowElementAttributesFunctions, 0, sizeof(JSHTMLTableRowElementAttributesFunctions));
    JSHTMLTableRowElementAttributesFunctions[0].name = "rowIndex";
    JSHTMLTableRowElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[0].magic = JSHTMLTableRowElement::RowIndexAttrNum;
    JSHTMLTableRowElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLTableRowElementAttributesFunctions[1].name = "vAlign";
    JSHTMLTableRowElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[1].magic = JSHTMLTableRowElement::VAlignAttrNum;
    JSHTMLTableRowElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLTableRowElement::putValueProperty;
    JSHTMLTableRowElementAttributesFunctions[2].name = "cells";
    JSHTMLTableRowElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[2].magic = JSHTMLTableRowElement::CellsAttrNum;
    JSHTMLTableRowElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLTableRowElementAttributesFunctions[3].name = "align";
    JSHTMLTableRowElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[3].magic = JSHTMLTableRowElement::AlignAttrNum;
    JSHTMLTableRowElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLTableRowElement::putValueProperty;
    JSHTMLTableRowElementAttributesFunctions[4].name = "bgColor";
    JSHTMLTableRowElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[4].magic = JSHTMLTableRowElement::BgColorAttrNum;
    JSHTMLTableRowElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLTableRowElement::putValueProperty;
    JSHTMLTableRowElementAttributesFunctions[5].name = "sectionRowIndex";
    JSHTMLTableRowElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[5].magic = JSHTMLTableRowElement::SectionRowIndexAttrNum;
    JSHTMLTableRowElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSHTMLTableRowElementAttributesFunctions[6].name = "ch";
    JSHTMLTableRowElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[6].magic = JSHTMLTableRowElement::ChAttrNum;
    JSHTMLTableRowElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLTableRowElement::putValueProperty;
    JSHTMLTableRowElementAttributesFunctions[7].name = "chOff";
    JSHTMLTableRowElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[7].magic = JSHTMLTableRowElement::ChOffAttrNum;
    JSHTMLTableRowElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLTableRowElement::putValueProperty;
    JSHTMLTableRowElementAttributesFunctions[8].name = "constructor";
    JSHTMLTableRowElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableRowElementAttributesFunctions[8].magic = JSHTMLTableRowElement::ConstructorAttrNum;
    JSHTMLTableRowElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLTableRowElement::getValueProperty;
    JSHTMLTableRowElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableRowElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableRowElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableRowElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableRowElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableRowElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableRowElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLTableRowElementPrototypeFunctions[2];
static bool JSHTMLTableRowElementPrototypeFunctions_initialized = false;

static void init_JSHTMLTableRowElementPrototypeFunctions()
{
    if (JSHTMLTableRowElementPrototypeFunctions_initialized) return;
    JSHTMLTableRowElementPrototypeFunctions_initialized = true;
    memset(JSHTMLTableRowElementPrototypeFunctions, 0, sizeof(JSHTMLTableRowElementPrototypeFunctions));
    JSHTMLTableRowElementPrototypeFunctions[0].name = "insertCell";
    JSHTMLTableRowElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLTableRowElementPrototypeFunctions[0].magic = JSHTMLTableRowElement::InsertCellFuncNum;
    JSHTMLTableRowElementPrototypeFunctions[0].u.func.length = 1;
    JSHTMLTableRowElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableRowElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLTableRowElementPrototypeFunction::callAsFunction;
    JSHTMLTableRowElementPrototypeFunctions[1].name = "deleteCell";
    JSHTMLTableRowElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableRowElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLTableRowElementPrototypeFunctions[1].magic = JSHTMLTableRowElement::DeleteCellFuncNum;
    JSHTMLTableRowElementPrototypeFunctions[1].u.func.length = 1;
    JSHTMLTableRowElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableRowElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLTableRowElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLTableRowElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableRowElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableRowElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableRowElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableRowElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableRowElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableRowElementAttributesFunctions, countof(JSHTMLTableRowElementAttributesFunctions));
    init_JSHTMLTableRowElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableRowElementPrototypeFunctions, countof(JSHTMLTableRowElementPrototypeFunctions));
}

static JSClassDef JSHTMLTableRowElementClassDefine;
static bool JSHTMLTableRowElementClassDefine_initialized = false;

static void init_JSHTMLTableRowElementClassDefine()
{
    if (JSHTMLTableRowElementClassDefine_initialized) return;
    JSHTMLTableRowElementClassDefine_initialized = true;
    memset(&JSHTMLTableRowElementClassDefine, 0, sizeof(JSHTMLTableRowElementClassDefine));
    JSHTMLTableRowElementClassDefine.class_name = "HTMLTableRowElement";
    JSHTMLTableRowElementClassDefine.finalizer = JSHTMLTableRowElement::finalizer;
    JSHTMLTableRowElementClassDefine.gc_mark = JSHTMLTableRowElement::mark;
}

JSClassID JSHTMLTableRowElement::js_class_id = 0;

void JSHTMLTableRowElement::init(JSContext* ctx)
{
    if (JSHTMLTableRowElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableRowElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableRowElement::create(JSContext* ctx, HTMLTableRowElement* impl)
{
    JSHTMLTableRowElement::init(ctx);
    JSValue _proto = JSHTMLTableRowElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableRowElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableRowElement* impl = (HTMLTableRowElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableRowElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableRowElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RowIndexAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->rowIndex());
        }
        case SectionRowIndexAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->sectionRowIndex());
        }
        case CellsAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->cells()));
        }
        case AlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableRowElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableRowElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableRowElementConstructor::self(ctx);
}

JSValue JSHTMLTableRowElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLTableRowElement::InsertCellFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->insertCell(index, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSHTMLTableRowElement::DeleteCellFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->deleteCell(index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
