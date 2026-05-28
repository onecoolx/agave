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

static JSCFunctionListEntry JSHTMLTableElementAttributesFunctions[15];
static bool JSHTMLTableElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableElementAttributesFunctions()
{
    if (JSHTMLTableElementAttributesFunctions_initialized) return;
    JSHTMLTableElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableElementAttributesFunctions, 0, sizeof(JSHTMLTableElementAttributesFunctions));
    JSHTMLTableElementAttributesFunctions[0].name = "caption";
    JSHTMLTableElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[0].magic = JSHTMLTableElement::CaptionAttrNum;
    JSHTMLTableElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[1].name = "tHead";
    JSHTMLTableElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[1].magic = JSHTMLTableElement::THeadAttrNum;
    JSHTMLTableElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[2].name = "tFoot";
    JSHTMLTableElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[2].magic = JSHTMLTableElement::TFootAttrNum;
    JSHTMLTableElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[3].name = "rows";
    JSHTMLTableElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[3].magic = JSHTMLTableElement::RowsAttrNum;
    JSHTMLTableElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSHTMLTableElementAttributesFunctions[4].name = "tBodies";
    JSHTMLTableElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[4].magic = JSHTMLTableElement::TBodiesAttrNum;
    JSHTMLTableElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSHTMLTableElementAttributesFunctions[5].name = "align";
    JSHTMLTableElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[5].magic = JSHTMLTableElement::AlignAttrNum;
    JSHTMLTableElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[6].name = "bgColor";
    JSHTMLTableElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[6].magic = JSHTMLTableElement::BgColorAttrNum;
    JSHTMLTableElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[7].name = "border";
    JSHTMLTableElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[7].magic = JSHTMLTableElement::BorderAttrNum;
    JSHTMLTableElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[8].name = "cellPadding";
    JSHTMLTableElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[8].magic = JSHTMLTableElement::CellPaddingAttrNum;
    JSHTMLTableElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[9].name = "cellSpacing";
    JSHTMLTableElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[9].magic = JSHTMLTableElement::CellSpacingAttrNum;
    JSHTMLTableElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[10].name = "frame";
    JSHTMLTableElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[10].magic = JSHTMLTableElement::FrameAttrNum;
    JSHTMLTableElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[11].name = "rules";
    JSHTMLTableElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[11].magic = JSHTMLTableElement::RulesAttrNum;
    JSHTMLTableElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[12].name = "summary";
    JSHTMLTableElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[12].magic = JSHTMLTableElement::SummaryAttrNum;
    JSHTMLTableElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[13].name = "width";
    JSHTMLTableElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[13].magic = JSHTMLTableElement::WidthAttrNum;
    JSHTMLTableElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[13].u.getset.set.setter_magic = JSHTMLTableElement::putValueProperty;
    JSHTMLTableElementAttributesFunctions[14].name = "constructor";
    JSHTMLTableElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableElementAttributesFunctions[14].magic = JSHTMLTableElement::ConstructorAttrNum;
    JSHTMLTableElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLTableElement::getValueProperty;
    JSHTMLTableElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLTableElementPrototypeFunctions[8];
static bool JSHTMLTableElementPrototypeFunctions_initialized = false;

static void init_JSHTMLTableElementPrototypeFunctions()
{
    if (JSHTMLTableElementPrototypeFunctions_initialized) return;
    JSHTMLTableElementPrototypeFunctions_initialized = true;
    memset(JSHTMLTableElementPrototypeFunctions, 0, sizeof(JSHTMLTableElementPrototypeFunctions));
    JSHTMLTableElementPrototypeFunctions[0].name = "createTHead";
    JSHTMLTableElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[0].magic = JSHTMLTableElement::CreateTHeadFuncNum;
    JSHTMLTableElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[1].name = "deleteTHead";
    JSHTMLTableElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[1].magic = JSHTMLTableElement::DeleteTHeadFuncNum;
    JSHTMLTableElementPrototypeFunctions[1].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[2].name = "createTFoot";
    JSHTMLTableElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[2].magic = JSHTMLTableElement::CreateTFootFuncNum;
    JSHTMLTableElementPrototypeFunctions[2].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[3].name = "deleteTFoot";
    JSHTMLTableElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[3].magic = JSHTMLTableElement::DeleteTFootFuncNum;
    JSHTMLTableElementPrototypeFunctions[3].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[4].name = "createCaption";
    JSHTMLTableElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[4].magic = JSHTMLTableElement::CreateCaptionFuncNum;
    JSHTMLTableElementPrototypeFunctions[4].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[5].name = "deleteCaption";
    JSHTMLTableElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[5].magic = JSHTMLTableElement::DeleteCaptionFuncNum;
    JSHTMLTableElementPrototypeFunctions[5].u.func.length = 0;
    JSHTMLTableElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[6].name = "insertRow";
    JSHTMLTableElementPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[6].magic = JSHTMLTableElement::InsertRowFuncNum;
    JSHTMLTableElementPrototypeFunctions[6].u.func.length = 1;
    JSHTMLTableElementPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[6].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
    JSHTMLTableElementPrototypeFunctions[7].name = "deleteRow";
    JSHTMLTableElementPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLTableElementPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSHTMLTableElementPrototypeFunctions[7].magic = JSHTMLTableElement::DeleteRowFuncNum;
    JSHTMLTableElementPrototypeFunctions[7].u.func.length = 1;
    JSHTMLTableElementPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLTableElementPrototypeFunctions[7].u.func.cfunc.generic_magic = JSHTMLTableElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLTableElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableElementAttributesFunctions, countof(JSHTMLTableElementAttributesFunctions));
    init_JSHTMLTableElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableElementPrototypeFunctions, countof(JSHTMLTableElementPrototypeFunctions));
}

JSClassID JSHTMLTableElement::js_class_id = 0;

void JSHTMLTableElement::init(JSContext* ctx)
{
    if (JSHTMLTableElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableElement::create(JSContext* ctx, HTMLTableElement* impl)
{
    JSHTMLTableElement::init(ctx);
    JSValue _proto = JSHTMLTableElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableElement* impl = (HTMLTableElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->caption()));
        }
        case THeadAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tHead()));
        }
        case TFootAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tFoot()));
        }
        case RowsAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->rows()));
        }
        case TBodiesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->tBodies()));
        }
        case AlignAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case BorderAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->border()).utf8().data());
        }
        case CellPaddingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cellPadding()).utf8().data());
        }
        case CellSpacingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cellSpacing()).utf8().data());
        }
        case FrameAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->frame()).utf8().data());
        }
        case RulesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rules()).utf8().data());
        }
        case SummaryAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->summary()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCaption(toHTMLTableCaptionElement(value));
            break;
        }
        case THeadAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTHead(toHTMLTableSectionElement(value));
            break;
        }
        case TFootAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTFoot(toHTMLTableSectionElement(value));
            break;
        }
        case AlignAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BorderAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CellPaddingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCellPadding(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CellSpacingAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCellSpacing(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FrameAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFrame(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RulesAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRules(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SummaryAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSummary(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLTableElement* imp = (HTMLTableElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
