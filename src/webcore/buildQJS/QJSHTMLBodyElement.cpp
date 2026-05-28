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

#include "QJSHTMLBodyElement.h"

#include "HTMLBodyElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLBodyElementAttributesFunctions[11];
static bool JSHTMLBodyElementAttributesFunctions_initialized = false;

static void init_JSHTMLBodyElementAttributesFunctions()
{
    if (JSHTMLBodyElementAttributesFunctions_initialized) return;
    JSHTMLBodyElementAttributesFunctions_initialized = true;
    memset(JSHTMLBodyElementAttributesFunctions, 0, sizeof(JSHTMLBodyElementAttributesFunctions));
    JSHTMLBodyElementAttributesFunctions[0].name = "aLink";
    JSHTMLBodyElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[0].magic = JSHTMLBodyElement::ALinkAttrNum;
    JSHTMLBodyElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[1].name = "text";
    JSHTMLBodyElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[1].magic = JSHTMLBodyElement::TextAttrNum;
    JSHTMLBodyElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[2].name = "link";
    JSHTMLBodyElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[2].magic = JSHTMLBodyElement::LinkAttrNum;
    JSHTMLBodyElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[3].name = "background";
    JSHTMLBodyElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[3].magic = JSHTMLBodyElement::BackgroundAttrNum;
    JSHTMLBodyElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[4].name = "vLink";
    JSHTMLBodyElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[4].magic = JSHTMLBodyElement::VLinkAttrNum;
    JSHTMLBodyElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[5].name = "scrollLeft";
    JSHTMLBodyElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[5].magic = JSHTMLBodyElement::ScrollLeftAttrNum;
    JSHTMLBodyElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[6].name = "bgColor";
    JSHTMLBodyElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[6].magic = JSHTMLBodyElement::BgColorAttrNum;
    JSHTMLBodyElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[7].name = "scrollTop";
    JSHTMLBodyElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[7].magic = JSHTMLBodyElement::ScrollTopAttrNum;
    JSHTMLBodyElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLBodyElement::putValueProperty;
    JSHTMLBodyElementAttributesFunctions[8].name = "scrollWidth";
    JSHTMLBodyElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[8].magic = JSHTMLBodyElement::ScrollWidthAttrNum;
    JSHTMLBodyElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSHTMLBodyElementAttributesFunctions[9].name = "scrollHeight";
    JSHTMLBodyElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[9].magic = JSHTMLBodyElement::ScrollHeightAttrNum;
    JSHTMLBodyElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLBodyElementAttributesFunctions[10].name = "constructor";
    JSHTMLBodyElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBodyElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBodyElementAttributesFunctions[10].magic = JSHTMLBodyElement::ConstructorAttrNum;
    JSHTMLBodyElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLBodyElement::getValueProperty;
    JSHTMLBodyElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

class JSHTMLBodyElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLBodyElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBodyElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBodyElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBodyElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBodyElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBodyElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBodyElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBodyElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBodyElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBodyElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLBodyElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBodyElementAttributesFunctions, countof(JSHTMLBodyElementAttributesFunctions));
}

static JSClassDef JSHTMLBodyElementClassDefine;
static bool JSHTMLBodyElementClassDefine_initialized = false;

static void init_JSHTMLBodyElementClassDefine()
{
    if (JSHTMLBodyElementClassDefine_initialized) return;
    JSHTMLBodyElementClassDefine_initialized = true;
    memset(&JSHTMLBodyElementClassDefine, 0, sizeof(JSHTMLBodyElementClassDefine));
    JSHTMLBodyElementClassDefine.class_name = "HTMLBodyElement";
    JSHTMLBodyElementClassDefine.finalizer = JSHTMLBodyElement::finalizer;
    JSHTMLBodyElementClassDefine.gc_mark = JSHTMLBodyElement::mark;
}

JSClassID JSHTMLBodyElement::js_class_id = 0;

void JSHTMLBodyElement::init(JSContext* ctx)
{
    if (JSHTMLBodyElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLBodyElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLBodyElement::create(JSContext* ctx, HTMLBodyElement* impl)
{
    JSHTMLBodyElement::init(ctx);
    JSValue _proto = JSHTMLBodyElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBodyElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBodyElement* impl = (HTMLBodyElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBodyElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBodyElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ALinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->aLink()).utf8().data());
        }
        case BackgroundAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->background()).utf8().data());
        }
        case BgColorAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->bgColor()).utf8().data());
        }
        case LinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->link()).utf8().data());
        }
        case TextAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case VLinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vLink()).utf8().data());
        }
        case ScrollLeftAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollLeft());
        }
        case ScrollTopAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollTop());
        }
        case ScrollWidthAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollWidth());
        }
        case ScrollHeightAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->scrollHeight());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBodyElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ALinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setALink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BackgroundAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBackground(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BgColorAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBgColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TextAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VLinkAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVLink(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ScrollLeftAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrollLeft(valueToInt32(ctx, value));
            break;
        }
        case ScrollTopAttrNum: {
            HTMLBodyElement* imp = (HTMLBodyElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setScrollTop(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBodyElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBodyElementConstructor::self(ctx);
}


}
