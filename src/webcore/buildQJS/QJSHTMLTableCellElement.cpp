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

#include "QJSHTMLTableCellElement.h"

#include "HTMLTableCellElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTableCellElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cellIndex", JSHTMLTableCellElement::getValueProperty, NULL, JSHTMLTableCellElement::CellIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("colSpan", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::ColSpanAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("noWrap", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::NoWrapAttrNum),
    JS_CGETSET_MAGIC_DEF("ch", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::ChAttrNum),
    JS_CGETSET_MAGIC_DEF("headers", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::HeadersAttrNum),
    JS_CGETSET_MAGIC_DEF("vAlign", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::VAlignAttrNum),
    JS_CGETSET_MAGIC_DEF("chOff", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::ChOffAttrNum),
    JS_CGETSET_MAGIC_DEF("abbr", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::AbbrAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("axis", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::AxisAttrNum),
    JS_CGETSET_MAGIC_DEF("bgColor", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::BgColorAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("rowSpan", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::RowSpanAttrNum),
    JS_CGETSET_MAGIC_DEF("scope", JSHTMLTableCellElement::getValueProperty, JSHTMLTableCellElement::putValueProperty, JSHTMLTableCellElement::ScopeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTableCellElement::getValueProperty, NULL, JSHTMLTableCellElement::ConstructorAttrNum)
};

class JSHTMLTableCellElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTableCellElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTableCellElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableCellElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableCellElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableCellElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableCellElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableCellElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableCellElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableCellElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableCellElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTableCellElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableCellElementAttributesFunctions, countof(JSHTMLTableCellElementAttributesFunctions));
}

static JSClassDef JSHTMLTableCellElementClassDefine = 
{
    "HTMLTableCellElement",
    .finalizer = JSHTMLTableCellElement::finalizer,
    .gc_mark = JSHTMLTableCellElement::mark,
};

JSClassID JSHTMLTableCellElement::js_class_id = 0;

void JSHTMLTableCellElement::init(JSContext* ctx)
{
    if (JSHTMLTableCellElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTableCellElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTableCellElement::js_class_id, &JSHTMLTableCellElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTableCellElementConstructor::self(ctx), JSHTMLTableCellElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTableCellElement::js_class_id, JSHTMLTableCellElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTableCellElement::create(JSContext* ctx, HTMLTableCellElement* impl)
{
    JSHTMLTableCellElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTableCellElementPrototype::self(ctx), JSHTMLTableCellElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableCellElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableCellElement* impl = (HTMLTableCellElement*)JS_GetOpaque(val, JSHTMLTableCellElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableCellElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableCellElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CellIndexAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->cellIndex());
        }
        case AbbrAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->abbr()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AxisAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->axis()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case ColSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->colSpan());
        }
        case HeadersAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->headers()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case NoWrapAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewBool(ctx, imp->noWrap() ? 1 : 0);
        }
        case RowSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->rowSpan());
        }
        case ScopeAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scope()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableCellElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AbbrAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setAbbr(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AxisAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setAxis(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ColSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setColSpan(valueToInt32(ctx, value));
            break;
        }
        case HeadersAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setHeaders(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoWrapAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setNoWrap(valueToBoolean(ctx, value));
            break;
        }
        case RowSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setRowSpan(valueToInt32(ctx, value));
            break;
        }
        case ScopeAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setScope(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTableCellElement::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableCellElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableCellElementConstructor::self(ctx);
}


}
