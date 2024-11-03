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

static const JSCFunctionListEntry JSHTMLObjectElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("contentDocument", JSHTMLObjectElement::getValueProperty, NULL, JSHTMLObjectElement::ContentDocumentAttrNum),
    JS_CGETSET_MAGIC_DEF("declare", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::DeclareAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLObjectElement::getValueProperty, NULL, JSHTMLObjectElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("codeType", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::CodeTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("vspace", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::VspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("border", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::BorderAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("standby", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::StandbyAttrNum),
    JS_CGETSET_MAGIC_DEF("hspace", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::HspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("codeBase", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::CodeBaseAttrNum),
    JS_CGETSET_MAGIC_DEF("archive", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::ArchiveAttrNum),
    JS_CGETSET_MAGIC_DEF("code", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::CodeAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("data", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::DataAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("useMap", JSHTMLObjectElement::getValueProperty, JSHTMLObjectElement::putValueProperty, JSHTMLObjectElement::UseMapAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLObjectElement::getValueProperty, NULL, JSHTMLObjectElement::ConstructorAttrNum)
};

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
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLObjectElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLObjectElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLObjectElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLObjectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLObjectElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLObjectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLObjectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLObjectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLObjectElementAttributesFunctions, countof(JSHTMLObjectElementAttributesFunctions));
}

static JSClassDef JSHTMLObjectElementClassDefine = 
{
    "HTMLObjectElement",
    .finalizer = JSHTMLObjectElement::finalizer,
    .gc_mark = JSHTMLObjectElement::mark,
};

JSClassID JSHTMLObjectElement::js_class_id = 0;

void JSHTMLObjectElement::init(JSContext* ctx)
{
    if (JSHTMLObjectElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLObjectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLObjectElement::js_class_id, &JSHTMLObjectElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLObjectElementConstructor::self(ctx), JSHTMLObjectElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLObjectElement::js_class_id, JSHTMLObjectElementPrototype::self(ctx));
    }
}

JSValue JSHTMLObjectElement::create(JSContext* ctx, HTMLObjectElement* impl)
{
    JSHTMLObjectElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLObjectElementPrototype::self(ctx), JSHTMLObjectElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLObjectElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLObjectElement* impl = (HTMLObjectElement*)JS_GetOpaque(val, JSHTMLObjectElement::js_class_id);
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
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case CodeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->code()).utf8().data());
        }
        case AlignAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case ArchiveAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->archive()).utf8().data());
        }
        case BorderAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->border()).utf8().data());
        }
        case CodeBaseAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeBase()).utf8().data());
        }
        case CodeTypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeType()).utf8().data());
        }
        case DataAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->data()).utf8().data());
        }
        case DeclareAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewBool(ctx, imp->declare() ? 1 : 0);
        }
        case HeightAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case HspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->hspace());
        }
        case NameAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case StandbyAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->standby()).utf8().data());
        }
        case TabIndexAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case TypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case UseMapAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->useMap()).utf8().data());
        }
        case VspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->vspace());
        }
        case WidthAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ContentDocumentAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
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
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setCode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AlignAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ArchiveAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setArchive(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case BorderAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setBorder(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeBaseAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setCodeBase(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeTypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setCodeType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DataAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setData(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DeclareAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setDeclare(valueToBoolean(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setHspace(valueToInt32(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case StandbyAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setStandby(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case UseMapAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setUseMap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VspaceAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
            imp->setVspace(valueToInt32(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLObjectElement* imp = (HTMLObjectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLObjectElement::js_class_id);
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
