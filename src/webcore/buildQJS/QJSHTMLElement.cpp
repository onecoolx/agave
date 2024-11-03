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

#include "QJSHTMLElement.h"

#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("innerText", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::InnerTextAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLElement::getValueProperty, NULL, JSHTMLElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("outerHTML", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::OuterHTMLAttrNum),
    JS_CGETSET_MAGIC_DEF("id", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::IdAttrNum),
    JS_CGETSET_MAGIC_DEF("isContentEditable", JSHTMLElement::getValueProperty, NULL, JSHTMLElement::IsContentEditableAttrNum),
    JS_CGETSET_MAGIC_DEF("dir", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::DirAttrNum),
    JS_CGETSET_MAGIC_DEF("title", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::TitleAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("contentEditable", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::ContentEditableAttrNum),
    JS_CGETSET_MAGIC_DEF("lang", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::LangAttrNum),
    JS_CGETSET_MAGIC_DEF("innerHTML", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::InnerHTMLAttrNum),
    JS_CGETSET_MAGIC_DEF("outerText", JSHTMLElement::getValueProperty, JSHTMLElement::putValueProperty, JSHTMLElement::OuterTextAttrNum),
    JS_CGETSET_MAGIC_DEF("children", JSHTMLElement::getValueProperty, NULL, JSHTMLElement::ChildrenAttrNum)
};

class JSHTMLElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSElementPrototype::self(ctx));
        JSHTMLElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLElementAttributesFunctions, countof(JSHTMLElementAttributesFunctions));
}

static JSClassDef JSHTMLElementClassDefine = 
{
    "HTMLElement",
    .finalizer = JSHTMLElement::finalizer,
    .gc_mark = JSHTMLElement::mark,
};

JSClassID JSHTMLElement::js_class_id = 0;

void JSHTMLElement::init(JSContext* ctx)
{
    if (JSHTMLElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLElement::js_class_id, &JSHTMLElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLElementConstructor::self(ctx), JSHTMLElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLElement::js_class_id, JSHTMLElementPrototype::self(ctx));
    }
}

JSValue JSHTMLElement::create(JSContext* ctx, HTMLElement* impl)
{
    JSHTMLElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLElementPrototype::self(ctx), JSHTMLElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLElement* impl = (HTMLElement*)JS_GetOpaque(val, JSHTMLElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSElement::mark(rt, val, mark_func);
}

JSValue JSHTMLElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case IdAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->id()).utf8().data());
        }
        case TitleAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->title()).utf8().data());
        }
        case LangAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lang()).utf8().data());
        }
        case DirAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->dir()).utf8().data());
        }
        case ClassNameAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->className()).utf8().data());
        }
        case InnerHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->innerHTML()).utf8().data());
        }
        case InnerTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->innerText()).utf8().data());
        }
        case OuterHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->outerHTML()).utf8().data());
        }
        case OuterTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->outerText()).utf8().data());
        }
        case ChildrenAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->children()));
        }
        case ContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentEditable()).utf8().data());
        }
        case IsContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            return JS_NewBool(ctx, imp->isContentEditable() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case IdAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setId(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TitleAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setTitle(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LangAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setLang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DirAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setDir(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ClassNameAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setClassName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case InnerHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setInnerHTML(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case InnerTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setInnerText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case OuterHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setOuterHTML(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case OuterTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setOuterText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case ContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque2(ctx, this_val, JSHTMLElement::js_class_id);
            imp->setContentEditable(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLElement::getConstructor(JSContext *ctx)
{
    return JSHTMLElementConstructor::self(ctx);
}


HTMLElement* toHTMLElement(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLElement* impl = (HTMLElement*)JS_GetOpaque(val, JSHTMLElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
