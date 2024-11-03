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

#include "QJSHTMLTableElement.h"

#include "ExceptionCode.h"
#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "HTMLTableCaptionElement.h"
#include "HTMLTableElement.h"
#include "HTMLTableSectionElement.h"
#include "QJSHTMLCollection.h"
#include "QJSHTMLElement.h"
#include "QJSHTMLTableCaptionElement.h"
#include "QJSHTMLTableSectionElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTableElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("bgColor", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::BgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("tFoot", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::TFootAttrNum),
    JS_CGETSET_MAGIC_DEF("summary", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::SummaryAttrNum),
    JS_CGETSET_MAGIC_DEF("cellPadding", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::CellPaddingAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("caption", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::CaptionAttrNum),
    JS_CGETSET_MAGIC_DEF("tBodies", JSHTMLTableElement::getValueProperty, NULL, JSHTMLTableElement::TBodiesAttrNum),
    JS_CGETSET_MAGIC_DEF("tHead", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::THeadAttrNum),
    JS_CGETSET_MAGIC_DEF("frame", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::FrameAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableElement::getValueProperty, NULL, JSHTMLTableElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("rules", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::RulesAttrNum),
    JS_CGETSET_MAGIC_DEF("rows", JSHTMLTableElement::getValueProperty, NULL, JSHTMLTableElement::RowsAttrNum),
    JS_CGETSET_MAGIC_DEF("border", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::BorderAttrNum),
    JS_CGETSET_MAGIC_DEF("cellSpacing", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::CellSpacingAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLTableElement::getValueProperty, JSHTMLTableElement::putValueProperty, JSHTMLTableElement::WidthAttrNum)
};

class JSHTMLTableElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLTableElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("deleteTFoot", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::DeleteTFootFuncNum),
    JS_CFUNC_MAGIC_DEF("createTHead", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::CreateTHeadFuncNum),
    JS_CFUNC_MAGIC_DEF("createCaption", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::CreateCaptionFuncNum),
    JS_CFUNC_MAGIC_DEF("createTFoot", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::CreateTFootFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteTHead", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::DeleteTHeadFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteCaption", 0, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::DeleteCaptionFuncNum),
    JS_CFUNC_MAGIC_DEF("insertRow", 1, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::InsertRowFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteRow", 1, JSHTMLTableElementPrototypeFunction::callAsFunction, JSHTMLTableElement::DeleteRowFuncNum)
};

JSValue JSHTMLTableElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableElementAttributesFunctions, countof(JSHTMLTableElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableElementPrototypeFunctions, countof(JSHTMLTableElementPrototypeFunctions));
}

static JSClassDef JSHTMLTableElementClassDefine = 
{
    "HTMLTableElement",
    .finalizer = JSHTMLTableElement::finalizer,
    .gc_mark = JSHTMLTableElement::mark,
};

JSClassID JSHTMLTableElement::js_class_id = 0;

void JSHTMLTableElement::init(JSContext* ctx)
{
    if (JSHTMLTableElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableElement::js_class_id, &JSHTMLTableElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableElementConstructor::self(ctx), JSHTMLTableElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableElement::js_class_id, JSHTMLTableElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableElement::create(JSContext* ctx, HTMLTableElement* impl)
{
    JSHTMLTableElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableElementPrototype::self(ctx), JSHTMLTableElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableElement* impl = (HTMLTableElement*)JS_GetOpaque(val, JSHTMLTableElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CaptionAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->caption()));
        }
        case THeadAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tHead()));
        }
        case TFootAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tFoot()));
        }
        case RowsAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->rows()));
        }
        case TBodiesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tBodies()));
        }
        case AlignAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case BorderAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->border()).utf8().data());
        }
        case CellPaddingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cellPadding()).utf8().data());
        }
        case CellSpacingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cellSpacing()).utf8().data());
        }
        case FrameAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frame()).utf8().data());
        }
        case RulesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rules()).utf8().data());
        }
        case SummaryAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->summary()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CaptionAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setCaption(toHTMLTableCaptionElement(value));
            break;
        }
        case THeadAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setTHead(toHTMLTableSectionElement(value));
            break;
        }
        case TFootAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setTFoot(toHTMLTableSectionElement(value));
            break;
        }
        case AlignAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BorderAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CellPaddingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setCellPadding(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CellSpacingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setCellSpacing(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FrameAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setFrame(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RulesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setRules(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SummaryAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setSummary(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableElementConstructor::self(ctx);
}

JSValue JSHTMLTableElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLTableElement::CreateTHeadFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createTHead()));
            return result;
        }
        case JSHTMLTableElement::DeleteTHeadFuncNum: {
            imp->deleteTHead();
            return JS_UNDEFINED;
        }
        case JSHTMLTableElement::CreateTFootFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createTFoot()));
            return result;
        }
        case JSHTMLTableElement::DeleteTFootFuncNum: {
            imp->deleteTFoot();
            return JS_UNDEFINED;
        }
        case JSHTMLTableElement::CreateCaptionFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createCaption()));
            return result;
        }
        case JSHTMLTableElement::DeleteCaptionFuncNum: {
            imp->deleteCaption();
            return JS_UNDEFINED;
        }
        case JSHTMLTableElement::InsertRowFuncNum: {
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
        case JSHTMLTableElement::DeleteRowFuncNum: {
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


}
