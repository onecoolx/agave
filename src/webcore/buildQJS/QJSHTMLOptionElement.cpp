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

#include "QJSHTMLOptionElement.h"

#include "HTMLFormElement.h"
#include "HTMLOptionElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLOptionElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("index", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::IndexAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLOptionElement::getValueProperty, NULL, JSHTMLOptionElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("text", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::TextAttrNum),
    JS_CGETSET_MAGIC_DEF("defaultSelected", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::DefaultSelectedAttrNum),
    JS_CGETSET_MAGIC_DEF("label", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::LabelAttrNum),
    JS_CGETSET_MAGIC_DEF("selected", JSHTMLOptionElement::getValueProperty, JSHTMLOptionElement::putValueProperty, JSHTMLOptionElement::SelectedAttrNum)
};

JSValue JSHTMLOptionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOptionElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLOptionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOptionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLOptionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionElementAttributesFunctions, countof(JSHTMLOptionElementAttributesFunctions));
}

static JSClassDef JSHTMLOptionElementClassDefine = 
{
    "HTMLOptionElement",
    .finalizer = JSHTMLOptionElement::finalizer,
    .gc_mark = JSHTMLOptionElement::mark,
};

JSClassID JSHTMLOptionElement::js_class_id = 0;

void JSHTMLOptionElement::init(JSContext* ctx)
{
    if (JSHTMLOptionElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLOptionElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLOptionElement::js_class_id, &JSHTMLOptionElementClassDefine);
        JS_SetClassProto(ctx, JSHTMLOptionElement::js_class_id, JSHTMLOptionElementPrototype::self(ctx));
    }
}

JSValue JSHTMLOptionElement::create(JSContext* ctx, HTMLOptionElement* impl)
{
    JSHTMLOptionElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLOptionElementPrototype::self(ctx), JSHTMLOptionElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOptionElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOptionElement* impl = (HTMLOptionElement*)JS_GetOpaque(val, JSHTMLOptionElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
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
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case DefaultSelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewBool(ctx, imp->defaultSelected() ? 1 : 0);
        }
        case TextAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case IndexAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->index());
        }
        case DisabledAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case LabelAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->label()).utf8().data());
        }
        case SelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewBool(ctx, imp->selected() ? 1 : 0);
        }
        case ValueAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
    }
    return JS_NULL;
}

JSValue JSHTMLOptionElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DefaultSelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            imp->setDefaultSelected(valueToBoolean(ctx, value));
            break;
        }
        case TextAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case IndexAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setIndex(valueToInt32(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DisabledAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case LabelAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            imp->setLabel(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SelectedAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
            imp->setSelected(valueToBoolean(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLOptionElement* imp = (HTMLOptionElement*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionElement::js_class_id);
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
        HTMLOptionElement* impl = (HTMLOptionElement*)JS_GetOpaque(val, JSHTMLOptionElement::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
