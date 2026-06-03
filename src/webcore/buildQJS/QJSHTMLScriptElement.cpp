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

#include "QJSHTMLScriptElement.h"

#include "HTMLScriptElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLScriptElementAttributesFunctions[8];
static bool JSHTMLScriptElementAttributesFunctions_initialized = false;

static void init_JSHTMLScriptElementAttributesFunctions()
{
    if (JSHTMLScriptElementAttributesFunctions_initialized) return;
    JSHTMLScriptElementAttributesFunctions_initialized = true;
    memset(JSHTMLScriptElementAttributesFunctions, 0, sizeof(JSHTMLScriptElementAttributesFunctions));
    JSHTMLScriptElementAttributesFunctions[0].name = "text";
    JSHTMLScriptElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[0].magic = JSHTMLScriptElement::TextAttrNum;
    JSHTMLScriptElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[1].name = "htmlFor";
    JSHTMLScriptElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[1].magic = JSHTMLScriptElement::HtmlForAttrNum;
    JSHTMLScriptElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[2].name = "event";
    JSHTMLScriptElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[2].magic = JSHTMLScriptElement::EventAttrNum;
    JSHTMLScriptElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[3].name = "charset";
    JSHTMLScriptElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[3].magic = JSHTMLScriptElement::CharsetAttrNum;
    JSHTMLScriptElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[4].name = "defer";
    JSHTMLScriptElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[4].magic = JSHTMLScriptElement::DeferAttrNum;
    JSHTMLScriptElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[5].name = "src";
    JSHTMLScriptElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[5].magic = JSHTMLScriptElement::SrcAttrNum;
    JSHTMLScriptElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[6].name = "type";
    JSHTMLScriptElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[6].magic = JSHTMLScriptElement::TypeAttrNum;
    JSHTMLScriptElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLScriptElement::putValueProperty;
    JSHTMLScriptElementAttributesFunctions[7].name = "constructor";
    JSHTMLScriptElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLScriptElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLScriptElementAttributesFunctions[7].magic = JSHTMLScriptElement::ConstructorAttrNum;
    JSHTMLScriptElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLScriptElement::getValueProperty;
    JSHTMLScriptElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSHTMLScriptElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLScriptElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLScriptElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLScriptElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLScriptElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLScriptElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLScriptElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLScriptElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLScriptElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLScriptElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLScriptElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLScriptElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLScriptElementAttributesFunctions, countof(JSHTMLScriptElementAttributesFunctions));
}

JSClassID JSHTMLScriptElement::js_class_id = 0;

void JSHTMLScriptElement::init(JSContext* ctx)
{
    if (JSHTMLScriptElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLScriptElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLScriptElement::create(JSContext* ctx, HTMLScriptElement* impl)
{
    JSHTMLScriptElement::init(ctx);
    JSValue _proto = JSHTMLScriptElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLScriptElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLScriptElement* impl = (HTMLScriptElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLScriptElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLScriptElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case HtmlForAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->htmlFor()).utf8().data());
        }
        case EventAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->event()).utf8().data());
        }
        case CharsetAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->charset()).utf8().data());
        }
        case DeferAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->defer() ? 1 : 0);
        }
        case SrcAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->src()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLScriptElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case HtmlForAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setHtmlFor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case EventAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setEvent(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case CharsetAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCharset(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DeferAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDefer(valueToBoolean(ctx, value));
            break;
        }
        case SrcAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSrc(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLScriptElement* imp = (HTMLScriptElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLScriptElement::getConstructor(JSContext *ctx)
{
    return JSHTMLScriptElementConstructor::self(ctx);
}


}
