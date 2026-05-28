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

#include "QJSHTMLEmbedElement.h"

#include "HTMLEmbedElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLEmbedElementAttributesFunctions[7];
static bool JSHTMLEmbedElementAttributesFunctions_initialized = false;

static void init_JSHTMLEmbedElementAttributesFunctions()
{
    if (JSHTMLEmbedElementAttributesFunctions_initialized) return;
    JSHTMLEmbedElementAttributesFunctions_initialized = true;
    memset(JSHTMLEmbedElementAttributesFunctions, 0, sizeof(JSHTMLEmbedElementAttributesFunctions));
    JSHTMLEmbedElementAttributesFunctions[0].name = "align";
    JSHTMLEmbedElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[0].magic = JSHTMLEmbedElement::AlignAttrNum;
    JSHTMLEmbedElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[1].name = "height";
    JSHTMLEmbedElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[1].magic = JSHTMLEmbedElement::HeightAttrNum;
    JSHTMLEmbedElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[2].name = "name";
    JSHTMLEmbedElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[2].magic = JSHTMLEmbedElement::NameAttrNum;
    JSHTMLEmbedElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[3].name = "src";
    JSHTMLEmbedElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[3].magic = JSHTMLEmbedElement::SrcAttrNum;
    JSHTMLEmbedElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[4].name = "type";
    JSHTMLEmbedElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[4].magic = JSHTMLEmbedElement::TypeAttrNum;
    JSHTMLEmbedElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[5].name = "width";
    JSHTMLEmbedElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[5].magic = JSHTMLEmbedElement::WidthAttrNum;
    JSHTMLEmbedElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLEmbedElement::putValueProperty;
    JSHTMLEmbedElementAttributesFunctions[6].name = "constructor";
    JSHTMLEmbedElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLEmbedElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLEmbedElementAttributesFunctions[6].magic = JSHTMLEmbedElement::ConstructorAttrNum;
    JSHTMLEmbedElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLEmbedElement::getValueProperty;
    JSHTMLEmbedElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

class JSHTMLEmbedElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLEmbedElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLEmbedElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLEmbedElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLEmbedElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLEmbedElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLEmbedElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLEmbedElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLEmbedElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLEmbedElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLEmbedElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLEmbedElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLEmbedElementAttributesFunctions, countof(JSHTMLEmbedElementAttributesFunctions));
}

static JSClassDef JSHTMLEmbedElementClassDefine;
static bool JSHTMLEmbedElementClassDefine_initialized = false;

static void init_JSHTMLEmbedElementClassDefine()
{
    if (JSHTMLEmbedElementClassDefine_initialized) return;
    JSHTMLEmbedElementClassDefine_initialized = true;
    memset(&JSHTMLEmbedElementClassDefine, 0, sizeof(JSHTMLEmbedElementClassDefine));
    JSHTMLEmbedElementClassDefine.class_name = "HTMLEmbedElement";
    JSHTMLEmbedElementClassDefine.finalizer = JSHTMLEmbedElement::finalizer;
    JSHTMLEmbedElementClassDefine.gc_mark = JSHTMLEmbedElement::mark;
}

JSClassID JSHTMLEmbedElement::js_class_id = 0;

void JSHTMLEmbedElement::init(JSContext* ctx)
{
    if (JSHTMLEmbedElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLEmbedElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLEmbedElement::create(JSContext* ctx, HTMLEmbedElement* impl)
{
    JSHTMLEmbedElement::init(ctx);
    JSValue _proto = JSHTMLEmbedElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLEmbedElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLEmbedElement* impl = (HTMLEmbedElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLEmbedElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLEmbedElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case NameAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SrcAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLEmbedElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLEmbedElement* imp = (HTMLEmbedElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLEmbedElement::getConstructor(JSContext *ctx)
{
    return JSHTMLEmbedElementConstructor::self(ctx);
}


}
