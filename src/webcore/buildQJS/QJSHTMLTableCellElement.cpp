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

#include "QJSHTMLTableCellElement.h"

#include "HTMLTableCellElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTableCellElementAttributesFunctions[16];
static bool JSHTMLTableCellElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableCellElementAttributesFunctions()
{
    if (JSHTMLTableCellElementAttributesFunctions_initialized) return;
    JSHTMLTableCellElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableCellElementAttributesFunctions, 0, sizeof(JSHTMLTableCellElementAttributesFunctions));
    JSHTMLTableCellElementAttributesFunctions[0].name = "cellIndex";
    JSHTMLTableCellElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[0].magic = JSHTMLTableCellElement::CellIndexAttrNum;
    JSHTMLTableCellElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLTableCellElementAttributesFunctions[1].name = "abbr";
    JSHTMLTableCellElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[1].magic = JSHTMLTableCellElement::AbbrAttrNum;
    JSHTMLTableCellElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[2].name = "align";
    JSHTMLTableCellElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[2].magic = JSHTMLTableCellElement::AlignAttrNum;
    JSHTMLTableCellElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[3].name = "axis";
    JSHTMLTableCellElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[3].magic = JSHTMLTableCellElement::AxisAttrNum;
    JSHTMLTableCellElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[4].name = "bgColor";
    JSHTMLTableCellElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[4].magic = JSHTMLTableCellElement::BgColorAttrNum;
    JSHTMLTableCellElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[5].name = "ch";
    JSHTMLTableCellElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[5].magic = JSHTMLTableCellElement::ChAttrNum;
    JSHTMLTableCellElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[6].name = "chOff";
    JSHTMLTableCellElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[6].magic = JSHTMLTableCellElement::ChOffAttrNum;
    JSHTMLTableCellElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[7].name = "colSpan";
    JSHTMLTableCellElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[7].magic = JSHTMLTableCellElement::ColSpanAttrNum;
    JSHTMLTableCellElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[8].name = "headers";
    JSHTMLTableCellElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[8].magic = JSHTMLTableCellElement::HeadersAttrNum;
    JSHTMLTableCellElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[9].name = "height";
    JSHTMLTableCellElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[9].magic = JSHTMLTableCellElement::HeightAttrNum;
    JSHTMLTableCellElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[10].name = "noWrap";
    JSHTMLTableCellElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[10].magic = JSHTMLTableCellElement::NoWrapAttrNum;
    JSHTMLTableCellElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[11].name = "rowSpan";
    JSHTMLTableCellElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[11].magic = JSHTMLTableCellElement::RowSpanAttrNum;
    JSHTMLTableCellElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[12].name = "scope";
    JSHTMLTableCellElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[12].magic = JSHTMLTableCellElement::ScopeAttrNum;
    JSHTMLTableCellElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[13].name = "vAlign";
    JSHTMLTableCellElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[13].magic = JSHTMLTableCellElement::VAlignAttrNum;
    JSHTMLTableCellElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[13].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[14].name = "width";
    JSHTMLTableCellElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[14].magic = JSHTMLTableCellElement::WidthAttrNum;
    JSHTMLTableCellElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[14].u.getset.set.setter_magic = JSHTMLTableCellElement::putValueProperty;
    JSHTMLTableCellElementAttributesFunctions[15].name = "constructor";
    JSHTMLTableCellElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableCellElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableCellElementAttributesFunctions[15].magic = JSHTMLTableCellElement::ConstructorAttrNum;
    JSHTMLTableCellElementAttributesFunctions[15].u.getset.get.getter_magic = JSHTMLTableCellElement::getValueProperty;
    JSHTMLTableCellElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableCellElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableCellElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableCellElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableCellElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableCellElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableCellElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableCellElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableCellElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableCellElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableCellElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableCellElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableCellElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableCellElementAttributesFunctions, countof(JSHTMLTableCellElementAttributesFunctions));
}

JSClassID JSHTMLTableCellElement::js_class_id = 0;

void JSHTMLTableCellElement::init(JSContext* ctx)
{
    if (JSHTMLTableCellElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableCellElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableCellElement::create(JSContext* ctx, HTMLTableCellElement* impl)
{
    JSHTMLTableCellElement::init(ctx);
    JSValue _proto = JSHTMLTableCellElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableCellElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableCellElement* impl = (HTMLTableCellElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->cellIndex());
        }
        case AbbrAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->abbr()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AxisAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->axis()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case ColSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->colSpan());
        }
        case HeadersAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->headers()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case NoWrapAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->noWrap() ? 1 : 0);
        }
        case RowSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->rowSpan());
        }
        case ScopeAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->scope()).utf8().data());
        }
        case VAlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAbbr(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AxisAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAxis(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ColSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setColSpan(valueToInt32(ctx, value));
            break;
        }
        case HeadersAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeaders(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoWrapAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setNoWrap(valueToBoolean(ctx, value));
            break;
        }
        case RowSpanAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setRowSpan(valueToInt32(ctx, value));
            break;
        }
        case ScopeAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScope(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableCellElement* imp = (HTMLTableCellElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
