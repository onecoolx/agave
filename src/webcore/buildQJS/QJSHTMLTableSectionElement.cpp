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

#include "QJSHTMLTableSectionElement.h"

#include "ExceptionCode.h"
#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "HTMLTableSectionElement.h"
#include "QJSHTMLCollection.h"
#include "QJSHTMLElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTableSectionElementAttributesFunctions[6];
static bool JSHTMLTableSectionElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableSectionElementAttributesFunctions()
{
    if (JSHTMLTableSectionElementAttributesFunctions_initialized) return;
    JSHTMLTableSectionElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableSectionElementAttributesFunctions, 0, sizeof(JSHTMLTableSectionElementAttributesFunctions));
    JSHTMLTableSectionElementAttributesFunctions[0].name = "align";
    JSHTMLTableSectionElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[0].magic = JSHTMLTableSectionElement::AlignAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTableSectionElement::putValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[1].name = "ch";
    JSHTMLTableSectionElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[1].magic = JSHTMLTableSectionElement::ChAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLTableSectionElement::putValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[2].name = "chOff";
    JSHTMLTableSectionElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[2].magic = JSHTMLTableSectionElement::ChOffAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLTableSectionElement::putValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[3].name = "vAlign";
    JSHTMLTableSectionElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[3].magic = JSHTMLTableSectionElement::VAlignAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLTableSectionElement::putValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[4].name = "rows";
    JSHTMLTableSectionElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[4].magic = JSHTMLTableSectionElement::RowsAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSHTMLTableSectionElementAttributesFunctions[5].name = "constructor";
    JSHTMLTableSectionElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableSectionElementAttributesFunctions[5].magic = JSHTMLTableSectionElement::ConstructorAttrNum;
    JSHTMLTableSectionElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTableSectionElement::getValueProperty;
    JSHTMLTableSectionElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableSectionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableSectionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableSectionElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableSectionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableSectionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableSectionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLTableSectionElementPrototypeFunctions[2];
static bool JSHTMLTableSectionElementPrototypeFunctions_initialized = false;

static void init_JSHTMLTableSectionElementPrototypeFunctions()
{
    if (JSHTMLTableSectionElementPrototypeFunctions_initialized) return;
    JSHTMLTableSectionElementPrototypeFunctions_initialized = true;
    memset(JSHTMLTableSectionElementPrototypeFunctions, 0, sizeof(JSHTMLTableSectionElementPrototypeFunctions));
    JSHTMLTableSectionElementPrototypeFunctions[0].name = "insertRow";
    JSHTMLTableSectionElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLTableSectionElementPrototypeFunctions[0].magic = JSHTMLTableSectionElement::InsertRowFuncNum;
    JSHTMLTableSectionElementPrototypeFunctions[0].u.func.length = 1;
    JSHTMLTableSectionElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableSectionElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLTableSectionElementPrototypeFunction::callAsFunction;
    JSHTMLTableSectionElementPrototypeFunctions[1].name = "deleteRow";
    JSHTMLTableSectionElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableSectionElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLTableSectionElementPrototypeFunctions[1].magic = JSHTMLTableSectionElement::DeleteRowFuncNum;
    JSHTMLTableSectionElementPrototypeFunctions[1].u.func.length = 1;
    JSHTMLTableSectionElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableSectionElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLTableSectionElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLTableSectionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableSectionElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableSectionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableSectionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableSectionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableSectionElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableSectionElementAttributesFunctions, countof(JSHTMLTableSectionElementAttributesFunctions));
    init_JSHTMLTableSectionElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableSectionElementPrototypeFunctions, countof(JSHTMLTableSectionElementPrototypeFunctions));
}

static JSClassDef JSHTMLTableSectionElementClassDefine;
static bool JSHTMLTableSectionElementClassDefine_initialized = false;

static void init_JSHTMLTableSectionElementClassDefine()
{
    if (JSHTMLTableSectionElementClassDefine_initialized) return;
    JSHTMLTableSectionElementClassDefine_initialized = true;
    memset(&JSHTMLTableSectionElementClassDefine, 0, sizeof(JSHTMLTableSectionElementClassDefine));
    JSHTMLTableSectionElementClassDefine.class_name = "HTMLTableSectionElement";
    JSHTMLTableSectionElementClassDefine.finalizer = JSHTMLTableSectionElement::finalizer;
    JSHTMLTableSectionElementClassDefine.gc_mark = JSHTMLTableSectionElement::mark;
}

JSClassID JSHTMLTableSectionElement::js_class_id = 0;

void JSHTMLTableSectionElement::init(JSContext* ctx)
{
    if (JSHTMLTableSectionElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableSectionElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableSectionElement::create(JSContext* ctx, HTMLTableSectionElement* impl)
{
    JSHTMLTableSectionElement::init(ctx);
    JSValue _proto = JSHTMLTableSectionElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableSectionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableSectionElement* impl = (HTMLTableSectionElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableSectionElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableSectionElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case RowsAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->rows()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableSectionElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableSectionElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableSectionElementConstructor::self(ctx);
}

JSValue JSHTMLTableSectionElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLTableSectionElement::InsertRowFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->insertRow(index, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSHTMLTableSectionElement::DeleteRowFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->deleteRow(index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


HTMLTableSectionElement* toHTMLTableSectionElement(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLTableSectionElement* impl = (HTMLTableSectionElement*)JS_GetOpaque(val, JSHTMLTableSectionElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
