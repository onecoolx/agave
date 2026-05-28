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

#include "QJSHTMLTableColElement.h"

#include "HTMLTableColElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLTableColElementAttributesFunctions[7];
static bool JSHTMLTableColElementAttributesFunctions_initialized = false;

static void init_JSHTMLTableColElementAttributesFunctions()
{
    if (JSHTMLTableColElementAttributesFunctions_initialized) return;
    JSHTMLTableColElementAttributesFunctions_initialized = true;
    memset(JSHTMLTableColElementAttributesFunctions, 0, sizeof(JSHTMLTableColElementAttributesFunctions));
    JSHTMLTableColElementAttributesFunctions[0].name = "align";
    JSHTMLTableColElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[0].magic = JSHTMLTableColElement::AlignAttrNum;
    JSHTMLTableColElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[1].name = "ch";
    JSHTMLTableColElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[1].magic = JSHTMLTableColElement::ChAttrNum;
    JSHTMLTableColElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[2].name = "chOff";
    JSHTMLTableColElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[2].magic = JSHTMLTableColElement::ChOffAttrNum;
    JSHTMLTableColElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[3].name = "span";
    JSHTMLTableColElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[3].magic = JSHTMLTableColElement::SpanAttrNum;
    JSHTMLTableColElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[4].name = "vAlign";
    JSHTMLTableColElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[4].magic = JSHTMLTableColElement::VAlignAttrNum;
    JSHTMLTableColElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[5].name = "width";
    JSHTMLTableColElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[5].magic = JSHTMLTableColElement::WidthAttrNum;
    JSHTMLTableColElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLTableColElement::putValueProperty;
    JSHTMLTableColElementAttributesFunctions[6].name = "constructor";
    JSHTMLTableColElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLTableColElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLTableColElementAttributesFunctions[6].magic = JSHTMLTableColElement::ConstructorAttrNum;
    JSHTMLTableColElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLTableColElement::getValueProperty;
    JSHTMLTableColElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

class JSHTMLTableColElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLTableColElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTableColElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTableColElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTableColElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableColElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTableColElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTableColElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTableColElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTableColElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLTableColElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLTableColElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTableColElementAttributesFunctions, countof(JSHTMLTableColElementAttributesFunctions));
}

static JSClassDef JSHTMLTableColElementClassDefine;
static bool JSHTMLTableColElementClassDefine_initialized = false;

static void init_JSHTMLTableColElementClassDefine()
{
    if (JSHTMLTableColElementClassDefine_initialized) return;
    JSHTMLTableColElementClassDefine_initialized = true;
    memset(&JSHTMLTableColElementClassDefine, 0, sizeof(JSHTMLTableColElementClassDefine));
    JSHTMLTableColElementClassDefine.class_name = "HTMLTableColElement";
    JSHTMLTableColElementClassDefine.finalizer = JSHTMLTableColElement::finalizer;
    JSHTMLTableColElementClassDefine.gc_mark = JSHTMLTableColElement::mark;
}

JSClassID JSHTMLTableColElement::js_class_id = 0;

void JSHTMLTableColElement::init(JSContext* ctx)
{
    if (JSHTMLTableColElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLTableColElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLTableColElement::create(JSContext* ctx, HTMLTableColElement* impl)
{
    JSHTMLTableColElement::init(ctx);
    JSValue _proto = JSHTMLTableColElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTableColElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTableColElement* impl = (HTMLTableColElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTableColElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTableColElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ChAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->ch()).utf8().data());
        }
        case ChOffAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->chOff()).utf8().data());
        }
        case SpanAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->span());
        }
        case VAlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vAlign()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTableColElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCh(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ChOffAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setChOff(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SpanAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSpan(valueToInt32(ctx, value));
            break;
        }
        case VAlignAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLTableColElement* imp = (HTMLTableColElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTableColElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTableColElementConstructor::self(ctx);
}


}
