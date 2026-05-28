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

#include "QJSHTMLObjectElement.h"

#include "Document.h"
#include "HTMLFormElement.h"
#include "HTMLObjectElement.h"
#include "QJSDocument.h"
#include "QJSHTMLFormElement.h"
#include "qjs_dom.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLObjectElementAttributesFunctions[20];
static bool JSHTMLObjectElementAttributesFunctions_initialized = false;

static void init_JSHTMLObjectElementAttributesFunctions()
{
    if (JSHTMLObjectElementAttributesFunctions_initialized) return;
    JSHTMLObjectElementAttributesFunctions_initialized = true;
    memset(JSHTMLObjectElementAttributesFunctions, 0, sizeof(JSHTMLObjectElementAttributesFunctions));
    JSHTMLObjectElementAttributesFunctions[0].name = "contentDocument";
    JSHTMLObjectElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[0].magic = JSHTMLObjectElement::ContentDocumentAttrNum;
    JSHTMLObjectElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLObjectElementAttributesFunctions[1].name = "declare";
    JSHTMLObjectElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[1].magic = JSHTMLObjectElement::DeclareAttrNum;
    JSHTMLObjectElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[2].name = "type";
    JSHTMLObjectElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[2].magic = JSHTMLObjectElement::TypeAttrNum;
    JSHTMLObjectElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[3].name = "form";
    JSHTMLObjectElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[3].magic = JSHTMLObjectElement::FormAttrNum;
    JSHTMLObjectElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSHTMLObjectElementAttributesFunctions[4].name = "codeType";
    JSHTMLObjectElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[4].magic = JSHTMLObjectElement::CodeTypeAttrNum;
    JSHTMLObjectElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[5].name = "vspace";
    JSHTMLObjectElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[5].magic = JSHTMLObjectElement::VspaceAttrNum;
    JSHTMLObjectElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[6].name = "width";
    JSHTMLObjectElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[6].magic = JSHTMLObjectElement::WidthAttrNum;
    JSHTMLObjectElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[7].name = "border";
    JSHTMLObjectElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[7].magic = JSHTMLObjectElement::BorderAttrNum;
    JSHTMLObjectElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[8].name = "tabIndex";
    JSHTMLObjectElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[8].magic = JSHTMLObjectElement::TabIndexAttrNum;
    JSHTMLObjectElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[9].name = "standby";
    JSHTMLObjectElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[9].magic = JSHTMLObjectElement::StandbyAttrNum;
    JSHTMLObjectElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[10].name = "hspace";
    JSHTMLObjectElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[10].magic = JSHTMLObjectElement::HspaceAttrNum;
    JSHTMLObjectElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[11].name = "height";
    JSHTMLObjectElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[11].magic = JSHTMLObjectElement::HeightAttrNum;
    JSHTMLObjectElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[11].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[12].name = "codeBase";
    JSHTMLObjectElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[12].magic = JSHTMLObjectElement::CodeBaseAttrNum;
    JSHTMLObjectElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[12].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[13].name = "archive";
    JSHTMLObjectElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[13].magic = JSHTMLObjectElement::ArchiveAttrNum;
    JSHTMLObjectElementAttributesFunctions[13].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[13].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[14].name = "code";
    JSHTMLObjectElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[14].magic = JSHTMLObjectElement::CodeAttrNum;
    JSHTMLObjectElementAttributesFunctions[14].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[14].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[15].name = "align";
    JSHTMLObjectElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[15].magic = JSHTMLObjectElement::AlignAttrNum;
    JSHTMLObjectElementAttributesFunctions[15].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[15].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[16].name = "data";
    JSHTMLObjectElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[16].magic = JSHTMLObjectElement::DataAttrNum;
    JSHTMLObjectElementAttributesFunctions[16].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[16].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[17].name = "name";
    JSHTMLObjectElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[17].magic = JSHTMLObjectElement::NameAttrNum;
    JSHTMLObjectElementAttributesFunctions[17].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[17].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[18].name = "useMap";
    JSHTMLObjectElementAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[18].magic = JSHTMLObjectElement::UseMapAttrNum;
    JSHTMLObjectElementAttributesFunctions[18].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[18].u.getset.set.setter_magic = JSHTMLObjectElement::putValueProperty;
    JSHTMLObjectElementAttributesFunctions[19].name = "constructor";
    JSHTMLObjectElementAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLObjectElementAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLObjectElementAttributesFunctions[19].magic = JSHTMLObjectElement::ConstructorAttrNum;
    JSHTMLObjectElementAttributesFunctions[19].u.getset.get.getter_magic = JSHTMLObjectElement::getValueProperty;
    JSHTMLObjectElementAttributesFunctions[19].u.getset.set.setter_magic = NULL;
}

class JSHTMLObjectElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLObjectElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLObjectElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLObjectElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLObjectElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLObjectElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLObjectElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLObjectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLObjectElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLObjectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLObjectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLObjectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLObjectElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLObjectElementAttributesFunctions, countof(JSHTMLObjectElementAttributesFunctions));
}

static JSClassDef JSHTMLObjectElementClassDefine;
static bool JSHTMLObjectElementClassDefine_initialized = false;

static void init_JSHTMLObjectElementClassDefine()
{
    if (JSHTMLObjectElementClassDefine_initialized) return;
    JSHTMLObjectElementClassDefine_initialized = true;
    memset(&JSHTMLObjectElementClassDefine, 0, sizeof(JSHTMLObjectElementClassDefine));
    JSHTMLObjectElementClassDefine.class_name = "HTMLObjectElement";
    JSHTMLObjectElementClassDefine.finalizer = JSHTMLObjectElement::finalizer;
    JSHTMLObjectElementClassDefine.gc_mark = JSHTMLObjectElement::mark;
}

JSClassID JSHTMLObjectElement::js_class_id = 0;

void JSHTMLObjectElement::init(JSContext* ctx)
{
    if (JSHTMLObjectElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLObjectElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLObjectElement::create(JSContext* ctx, HTMLObjectElement* impl)
{
    JSHTMLObjectElement::init(ctx);
    JSValue _proto = JSHTMLObjectElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLObjectElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLObjectElement* impl = (HTMLObjectElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLObjectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLObjectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case CodeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->code()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ArchiveAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->archive()).utf8().data());
        }
        case BorderAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->border()).utf8().data());
        }
        case CodeBaseAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeBase()).utf8().data());
        }
        case CodeTypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeType()).utf8().data());
        }
        case DataAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->data()).utf8().data());
        }
        case DeclareAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->declare() ? 1 : 0);
        }
        case HeightAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case HspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->hspace());
        }
        case NameAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case StandbyAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->standby()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case UseMapAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->useMap()).utf8().data());
        }
        case VspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->vspace());
        }
        case WidthAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return checkNodeSecurity(ctx, imp->contentDocument()) ? toJS(ctx, QJS::getPtr(imp->contentDocument())) : JS_UNDEFINED;
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLObjectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CodeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ArchiveAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setArchive(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BorderAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeBaseAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCodeBase(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeTypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCodeType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DataAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setData(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DeclareAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDeclare(valueToBoolean(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHspace(valueToInt32(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case StandbyAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setStandby(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case UseMapAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setUseMap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVspace(valueToInt32(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLObjectElement::getConstructor(JSContext *ctx)
{
    return JSHTMLObjectElementConstructor::self(ctx);
}


}
