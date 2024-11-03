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

static const JSCFunctionListEntry JSHTMLTableSectionElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("ch", JSHTMLTableSectionElement::getValueProperty, JSHTMLTableSectionElement::putValueProperty, JSHTMLTableSectionElement::ChAttrNum),
    JS_CGETSET_MAGIC_DEF("rows", JSHTMLTableSectionElement::getValueProperty, NULL, JSHTMLTableSectionElement::RowsAttrNum),
    JS_CGETSET_MAGIC_DEF("vAlign", JSHTMLTableSectionElement::getValueProperty, JSHTMLTableSectionElement::putValueProperty, JSHTMLTableSectionElement::VAlignAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableSectionElement::getValueProperty, NULL, JSHTMLTableSectionElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableSectionElement::getValueProperty, JSHTMLTableSectionElement::putValueProperty, JSHTMLTableSectionElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("chOff", JSHTMLTableSectionElement::getValueProperty, JSHTMLTableSectionElement::putValueProperty, JSHTMLTableSectionElement::ChOffAttrNum)
};

class JSHTMLTableSectionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableSectionElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableSectionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableSectionElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableSectionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableSectionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableSectionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLTableSectionElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("insertRow", 1, JSHTMLTableSectionElementPrototypeFunction::callAsFunction, JSHTMLTableSectionElement::InsertRowFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteRow", 1, JSHTMLTableSectionElementPrototypeFunction::callAsFunction, JSHTMLTableSectionElement::DeleteRowFuncNum)
};

JSValue JSHTMLTableSectionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableSectionElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableSectionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableSectionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableSectionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableSectionElementAttributesFunctions, countof(JSHTMLTableSectionElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableSectionElementPrototypeFunctions, countof(JSHTMLTableSectionElementPrototypeFunctions));
}

static JSClassDef JSHTMLTableSectionElementClassDefine = 
{
    "HTMLTableSectionElement",
    .finalizer = JSHTMLTableSectionElement::finalizer,
    .gc_mark = JSHTMLTableSectionElement::mark,
};

JSClassID JSHTMLTableSectionElement::js_class_id = 0;

void JSHTMLTableSectionElement::init(JSContext* ctx)
{
    if (JSHTMLTableSectionElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableSectionElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableSectionElement::js_class_id, &JSHTMLTableSectionElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableSectionElementConstructor::self(ctx), JSHTMLTableSectionElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableSectionElement::js_class_id, JSHTMLTableSectionElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableSectionElement::create(JSContext* ctx, HTMLTableSectionElement* impl)
{
    JSHTMLTableSectionElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableSectionElementPrototype::self(ctx), JSHTMLTableSectionElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableSectionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableSectionElement* impl = (HTMLTableSectionElement*)JS_GetOpaque(val, JSHTMLTableSectionElement::js_class_id);
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
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case RowsAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
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
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
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
    HTMLTableSectionElement* imp = (HTMLTableSectionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableSectionElement::js_class_id);
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
