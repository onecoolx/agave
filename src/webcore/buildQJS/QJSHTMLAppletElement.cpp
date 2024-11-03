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

#include "QJSHTMLAppletElement.h"

#include "HTMLAppletElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLAppletElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("hspace", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::HspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("alt", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::AltAttrNum),
    JS_CGETSET_MAGIC_DEF("object", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::ObjectAttrNum),
    JS_CGETSET_MAGIC_DEF("vspace", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::VspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("align", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("codeBase", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::CodeBaseAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLAppletElement::getValueProperty, NULL, JSHTMLAppletElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("archive", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::ArchiveAttrNum),
    JS_CGETSET_MAGIC_DEF("code", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::CodeAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLAppletElement::getValueProperty, JSHTMLAppletElement::putValueProperty, JSHTMLAppletElement::NameAttrNum)
};

class JSHTMLAppletElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLAppletElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLAppletElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLAppletElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLAppletElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLAppletElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAppletElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLAppletElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLAppletElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLAppletElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLAppletElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLAppletElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLAppletElementAttributesFunctions, countof(JSHTMLAppletElementAttributesFunctions));
}

static JSClassDef JSHTMLAppletElementClassDefine = 
{
    "HTMLAppletElement",
    .finalizer = JSHTMLAppletElement::finalizer,
    .gc_mark = JSHTMLAppletElement::mark,
};

JSClassID JSHTMLAppletElement::js_class_id = 0;

void JSHTMLAppletElement::init(JSContext* ctx)
{
    if (JSHTMLAppletElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLAppletElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLAppletElement::js_class_id, &JSHTMLAppletElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLAppletElementConstructor::self(ctx), JSHTMLAppletElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLAppletElement::js_class_id, JSHTMLAppletElementPrototype::self(ctx));
    }
}

JSValue JSHTMLAppletElement::create(JSContext* ctx, HTMLAppletElement* impl)
{
    JSHTMLAppletElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLAppletElementPrototype::self(ctx), JSHTMLAppletElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLAppletElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLAppletElement* impl = (HTMLAppletElement*)JS_GetOpaque(val, JSHTMLAppletElement::js_class_id);
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
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case AltAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->alt()).utf8().data());
        }
        case ArchiveAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->archive()).utf8().data());
        }
        case CodeAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->code()).utf8().data());
        }
        case CodeBaseAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->codeBase()).utf8().data());
        }
        case HeightAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->height()).utf8().data());
        }
        case HspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->hspace()).utf8().data());
        }
        case NameAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ObjectAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->object()).utf8().data());
        }
        case VspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->vspace()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
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
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case AltAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setAlt(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ArchiveAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setArchive(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setCode(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CodeBaseAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setCodeBase(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setHeight(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setHspace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ObjectAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setObject(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case VspaceAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
            imp->setVspace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLAppletElement* imp = (HTMLAppletElement*)JS_GetOpaque2(ctx, this_val, JSHTMLAppletElement::js_class_id);
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
