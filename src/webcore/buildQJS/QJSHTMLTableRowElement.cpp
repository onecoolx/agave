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

static const JSCFunctionListEntry JSHTMLTableRowElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("rowIndex", JSHTMLTableRowElement::getValueProperty, NULL, JSHTMLTableRowElement::RowIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("vAlign", JSHTMLTableRowElement::getValueProperty, JSHTMLTableRowElement::putValueProperty, JSHTMLTableRowElement::VAlignAttrNum),
    JS_CGETSET_MAGIC_DEF("cells", JSHTMLTableRowElement::getValueProperty, NULL, JSHTMLTableRowElement::CellsAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableRowElement::getValueProperty, JSHTMLTableRowElement::putValueProperty, JSHTMLTableRowElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("bgColor", JSHTMLTableRowElement::getValueProperty, JSHTMLTableRowElement::putValueProperty, JSHTMLTableRowElement::BgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("sectionRowIndex", JSHTMLTableRowElement::getValueProperty, NULL, JSHTMLTableRowElement::SectionRowIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("ch", JSHTMLTableRowElement::getValueProperty, JSHTMLTableRowElement::putValueProperty, JSHTMLTableRowElement::ChAttrNum),
    JS_CGETSET_MAGIC_DEF("chOff", JSHTMLTableRowElement::getValueProperty, JSHTMLTableRowElement::putValueProperty, JSHTMLTableRowElement::ChOffAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableRowElement::getValueProperty, NULL, JSHTMLTableRowElement::ConstructorAttrNum)
};

class JSHTMLTableRowElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableRowElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableRowElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableRowElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableRowElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableRowElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableRowElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLTableRowElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("insertCell", 1, JSHTMLTableRowElementPrototypeFunction::callAsFunction, JSHTMLTableRowElement::InsertCellFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteCell", 1, JSHTMLTableRowElementPrototypeFunction::callAsFunction, JSHTMLTableRowElement::DeleteCellFuncNum)
};

JSValue JSHTMLTableRowElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableRowElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableRowElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableRowElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableRowElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableRowElementAttributesFunctions, countof(JSHTMLTableRowElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableRowElementPrototypeFunctions, countof(JSHTMLTableRowElementPrototypeFunctions));
}

static JSClassDef JSHTMLTableRowElementClassDefine = 
{
    "HTMLTableRowElement",
    .finalizer = JSHTMLTableRowElement::finalizer,
    .gc_mark = JSHTMLTableRowElement::mark,
};

JSClassID JSHTMLTableRowElement::js_class_id = 0;

void JSHTMLTableRowElement::init(JSContext* ctx)
{
    if (JSHTMLTableRowElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableRowElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableRowElement::js_class_id, &JSHTMLTableRowElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableRowElementConstructor::self(ctx), JSHTMLTableRowElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableRowElement::js_class_id, JSHTMLTableRowElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableRowElement::create(JSContext* ctx, HTMLTableRowElement* impl)
{
    JSHTMLTableRowElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableRowElementPrototype::self(ctx), JSHTMLTableRowElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableRowElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableRowElement* impl = (HTMLTableRowElement*)JS_GetOpaque(val, JSHTMLTableRowElement::js_class_id);
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
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->rowIndex());
        }
        case SectionRowIndexAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->sectionRowIndex());
        }
        case CellsAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->cells()));
        }
        case AlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
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
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
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
    HTMLTableRowElement* imp = (HTMLTableRowElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableRowElement::js_class_id);
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
