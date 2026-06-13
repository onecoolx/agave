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

#include "QJSHTMLElement.h"

#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLElementAttributesFunctions[13];
static bool JSHTMLElementAttributesFunctions_initialized = false;

static void init_JSHTMLElementAttributesFunctions()
{
    if (JSHTMLElementAttributesFunctions_initialized) return;
    JSHTMLElementAttributesFunctions_initialized = true;
    memset(JSHTMLElementAttributesFunctions, 0, sizeof(JSHTMLElementAttributesFunctions));
    JSHTMLElementAttributesFunctions[0].name = "id";
    JSHTMLElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[0].magic = JSHTMLElement::IdAttrNum;
    JSHTMLElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[1].name = "title";
    JSHTMLElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[1].magic = JSHTMLElement::TitleAttrNum;
    JSHTMLElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[2].name = "lang";
    JSHTMLElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[2].magic = JSHTMLElement::LangAttrNum;
    JSHTMLElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[3].name = "dir";
    JSHTMLElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[3].magic = JSHTMLElement::DirAttrNum;
    JSHTMLElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[4].name = "className";
    JSHTMLElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[4].magic = JSHTMLElement::ClassNameAttrNum;
    JSHTMLElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[5].name = "innerHTML";
    JSHTMLElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[5].magic = JSHTMLElement::InnerHTMLAttrNum;
    JSHTMLElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[6].name = "innerText";
    JSHTMLElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[6].magic = JSHTMLElement::InnerTextAttrNum;
    JSHTMLElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[7].name = "outerHTML";
    JSHTMLElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[7].magic = JSHTMLElement::OuterHTMLAttrNum;
    JSHTMLElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[8].name = "outerText";
    JSHTMLElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[8].magic = JSHTMLElement::OuterTextAttrNum;
    JSHTMLElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[9].name = "children";
    JSHTMLElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[9].magic = JSHTMLElement::ChildrenAttrNum;
    JSHTMLElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSHTMLElementAttributesFunctions[10].name = "contentEditable";
    JSHTMLElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[10].magic = JSHTMLElement::ContentEditableAttrNum;
    JSHTMLElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLElement::putValueProperty;
    JSHTMLElementAttributesFunctions[11].name = "isContentEditable";
    JSHTMLElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[11].magic = JSHTMLElement::IsContentEditableAttrNum;
    JSHTMLElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSHTMLElementAttributesFunctions[12].name = "constructor";
    JSHTMLElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLElementAttributesFunctions[12].magic = JSHTMLElement::ConstructorAttrNum;
    JSHTMLElementAttributesFunctions[12].u.getset.get.getter_magic = JSHTMLElement::getValueProperty;
    JSHTMLElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
}

class JSHTMLElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSElementPrototype::self(ctx));
        JSHTMLElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLElementAttributesFunctions, countof(JSHTMLElementAttributesFunctions));
}

JSClassID JSHTMLElement::js_class_id = 0;

void JSHTMLElement::init(JSContext* ctx)
{
    if (JSHTMLElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLElement::create(JSContext* ctx, HTMLElement* impl)
{
    JSHTMLElement::init(ctx);
    JSValue _proto = JSHTMLElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLElement* impl = (HTMLElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
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
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->id()).utf8().data());
        }
        case TitleAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->title()).utf8().data());
        }
        case LangAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lang()).utf8().data());
        }
        case DirAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->dir()).utf8().data());
        }
        case ClassNameAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->className()).utf8().data());
        }
        case InnerHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->innerHTML()).utf8().data());
        }
        case InnerTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->innerText()).utf8().data());
        }
        case OuterHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->outerHTML()).utf8().data());
        }
        case OuterTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->outerText()).utf8().data());
        }
        case ChildrenAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->children()));
        }
        case ContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentEditable()).utf8().data());
        }
        case IsContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setId(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TitleAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setTitle(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LangAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLang(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DirAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDir(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ClassNameAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setClassName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case InnerHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setInnerHTML(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case InnerTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setInnerText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case OuterHTMLAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setOuterHTML(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case OuterTextAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setOuterText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case ContentEditableAttrNum: {
            HTMLElement* imp = (HTMLElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
        HTMLElement* impl = (HTMLElement*)JS_GetOpaque(val, JSNode::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
