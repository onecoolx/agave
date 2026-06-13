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

#include "QJSHTMLOptionElement.h"

#include "HTMLFormElement.h"
#include "HTMLOptionElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLOptionElementAttributesFunctions[8];
static bool JSHTMLOptionElementAttributesFunctions_initialized = false;

static void init_JSHTMLOptionElementAttributesFunctions()
{
    if (JSHTMLOptionElementAttributesFunctions_initialized) return;
    JSHTMLOptionElementAttributesFunctions_initialized = true;
    memset(JSHTMLOptionElementAttributesFunctions, 0, sizeof(JSHTMLOptionElementAttributesFunctions));
    JSHTMLOptionElementAttributesFunctions[0].name = "form";
    JSHTMLOptionElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[0].magic = JSHTMLOptionElement::FormAttrNum;
    JSHTMLOptionElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLOptionElementAttributesFunctions[1].name = "defaultSelected";
    JSHTMLOptionElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[1].magic = JSHTMLOptionElement::DefaultSelectedAttrNum;
    JSHTMLOptionElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[2].name = "text";
    JSHTMLOptionElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[2].magic = JSHTMLOptionElement::TextAttrNum;
    JSHTMLOptionElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[3].name = "index";
    JSHTMLOptionElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[3].magic = JSHTMLOptionElement::IndexAttrNum;
    JSHTMLOptionElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[4].name = "disabled";
    JSHTMLOptionElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[4].magic = JSHTMLOptionElement::DisabledAttrNum;
    JSHTMLOptionElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[5].name = "label";
    JSHTMLOptionElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[5].magic = JSHTMLOptionElement::LabelAttrNum;
    JSHTMLOptionElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[5].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[6].name = "selected";
    JSHTMLOptionElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[6].magic = JSHTMLOptionElement::SelectedAttrNum;
    JSHTMLOptionElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
    JSHTMLOptionElementAttributesFunctions[7].name = "value";
    JSHTMLOptionElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionElementAttributesFunctions[7].magic = JSHTMLOptionElement::ValueAttrNum;
    JSHTMLOptionElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLOptionElement::getValueProperty;
    JSHTMLOptionElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLOptionElement::putValueProperty;
}

JSValue JSHTMLOptionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOptionElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLOptionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOptionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLOptionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLOptionElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionElementAttributesFunctions, countof(JSHTMLOptionElementAttributesFunctions));
}

JSClassID JSHTMLOptionElement::js_class_id = 0;

void JSHTMLOptionElement::init(JSContext* ctx)
{
    if (JSHTMLOptionElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLOptionElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLOptionElement::create(JSContext* ctx, HTMLOptionElement* impl)
{
    JSHTMLOptionElement::init(ctx);
    JSValue _proto = JSHTMLOptionElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOptionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOptionElement* impl = (HTMLOptionElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLOptionElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLOptionElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case DefaultSelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->defaultSelected() ? 1 : 0);
        }
        case TextAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case IndexAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->index());
        }
        case DisabledAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case LabelAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->label()).utf8().data());
        }
        case SelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->selected() ? 1 : 0);
        }
        case ValueAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
    }
    return JS_NULL;
}

JSValue JSHTMLOptionElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DefaultSelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDefaultSelected(valueToBoolean(ctx, value));
            break;
        }
        case TextAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case IndexAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setIndex(valueToInt32(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DisabledAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case LabelAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setLabel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSelected(valueToBoolean(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLOptionElement::getConstructor(JSContext *ctx)
{
    return JSHTMLElement::getConstructor(ctx);
}


HTMLOptionElement* toHTMLOptionElement(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLOptionElement* impl = (HTMLOptionElement*)JS_GetOpaque(val, JSNode::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
