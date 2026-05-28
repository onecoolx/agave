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

#include "QJSHTMLAppletElement.h"

#include "HTMLAppletElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLAppletElementAttributesFunctions[12];
static bool JSHTMLAppletElementAttributesFunctions_initialized = false;

static void init_JSHTMLAppletElementAttributesFunctions()
{
    if (JSHTMLAppletElementAttributesFunctions_initialized) return;
    JSHTMLAppletElementAttributesFunctions_initialized = true;
    memset(JSHTMLAppletElementAttributesFunctions, 0, sizeof(JSHTMLAppletElementAttributesFunctions));
    JSHTMLAppletElementAttributesFunctions[0].name = "align";
    JSHTMLAppletElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[0].magic = JSHTMLAppletElement::AlignAttrNum;
    JSHTMLAppletElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[1].name = "alt";
    JSHTMLAppletElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[1].magic = JSHTMLAppletElement::AltAttrNum;
    JSHTMLAppletElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[2].name = "archive";
    JSHTMLAppletElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[2].magic = JSHTMLAppletElement::ArchiveAttrNum;
    JSHTMLAppletElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[3].name = "code";
    JSHTMLAppletElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[3].magic = JSHTMLAppletElement::CodeAttrNum;
    JSHTMLAppletElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[4].name = "codeBase";
    JSHTMLAppletElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[4].magic = JSHTMLAppletElement::CodeBaseAttrNum;
    JSHTMLAppletElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[5].name = "height";
    JSHTMLAppletElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[5].magic = JSHTMLAppletElement::HeightAttrNum;
    JSHTMLAppletElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[6].name = "hspace";
    JSHTMLAppletElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[6].magic = JSHTMLAppletElement::HspaceAttrNum;
    JSHTMLAppletElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[7].name = "name";
    JSHTMLAppletElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[7].magic = JSHTMLAppletElement::NameAttrNum;
    JSHTMLAppletElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[8].name = "object";
    JSHTMLAppletElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[8].magic = JSHTMLAppletElement::ObjectAttrNum;
    JSHTMLAppletElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[9].name = "vspace";
    JSHTMLAppletElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[9].magic = JSHTMLAppletElement::VspaceAttrNum;
    JSHTMLAppletElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[10].name = "width";
    JSHTMLAppletElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[10].magic = JSHTMLAppletElement::WidthAttrNum;
    JSHTMLAppletElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLAppletElement::putValueProperty;
    JSHTMLAppletElementAttributesFunctions[11].name = "constructor";
    JSHTMLAppletElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLAppletElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLAppletElementAttributesFunctions[11].magic = JSHTMLAppletElement::ConstructorAttrNum;
    JSHTMLAppletElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLAppletElement::getValueProperty;
    JSHTMLAppletElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
}

class JSHTMLAppletElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLAppletElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAppletElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAppletElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAppletElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAppletElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLAppletElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAppletElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAppletElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAppletElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLAppletElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLAppletElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAppletElementAttributesFunctions, countof(JSHTMLAppletElementAttributesFunctions));
}

static JSClassDef JSHTMLAppletElementClassDefine;
static bool JSHTMLAppletElementClassDefine_initialized = false;

static void init_JSHTMLAppletElementClassDefine()
{
    if (JSHTMLAppletElementClassDefine_initialized) return;
    JSHTMLAppletElementClassDefine_initialized = true;
    memset(&JSHTMLAppletElementClassDefine, 0, sizeof(JSHTMLAppletElementClassDefine));
    JSHTMLAppletElementClassDefine.class_name = "HTMLAppletElement";
    JSHTMLAppletElementClassDefine.finalizer = JSHTMLAppletElement::finalizer;
    JSHTMLAppletElementClassDefine.gc_mark = JSHTMLAppletElement::mark;
}

JSClassID JSHTMLAppletElement::js_class_id = 0;

void JSHTMLAppletElement::init(JSContext* ctx)
{
    if (JSHTMLAppletElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLAppletElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLAppletElement::create(JSContext* ctx, HTMLAppletElement* impl)
{
    JSHTMLAppletElement::init(ctx);
    JSValue _proto = JSHTMLAppletElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAppletElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAppletElement* impl = (HTMLAppletElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLAppletElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLAppletElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AltAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case ArchiveAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->archive()).utf8().data());
        }
        case CodeAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->code()).utf8().data());
        }
        case CodeBaseAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeBase()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case HspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hspace()).utf8().data());
        }
        case NameAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ObjectAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->object()).utf8().data());
        }
        case VspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vspace()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLAppletElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ArchiveAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setArchive(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeBaseAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCodeBase(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHspace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ObjectAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setObject(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVspace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLAppletElement::getConstructor(JSContext *ctx)
{
    return JSHTMLAppletElementConstructor::self(ctx);
}


}
