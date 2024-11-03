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

#include "QJSHTMLSelectElement.h"

#include "ExceptionCode.h"
#include "HTMLFormElement.h"
#include "HTMLOptionsCollection.h"
#include "HTMLSelectElement.h"
#include "Node.h"
#include "QJSHTMLElement.h"
#include "QJSHTMLFormElement.h"
#include "QJSHTMLOptionsCollection.h"
#include "QJSNode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLSelectElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("size", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::SizeAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLSelectElement::getValueProperty, NULL, JSHTMLSelectElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("multiple", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::MultipleAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLSelectElement::getValueProperty, NULL, JSHTMLSelectElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("options", JSHTMLSelectElement::getValueProperty, NULL, JSHTMLSelectElement::OptionsAttrNum),
    JS_CGETSET_MAGIC_DEF("tabIndex", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::TabIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("selectedIndex", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::SelectedIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLSelectElement::getValueProperty, JSHTMLSelectElement::putValueProperty, JSHTMLSelectElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLSelectElement::getValueProperty, NULL, JSHTMLSelectElement::ConstructorAttrNum)
};

class JSHTMLSelectElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLSelectElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLSelectElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLSelectElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLSelectElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLSelectElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLSelectElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLSelectElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("remove", 0, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::RemoveFuncNum),
    JS_CFUNC_MAGIC_DEF("add", 2, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::AddFuncNum),
    JS_CFUNC_MAGIC_DEF("blur", 0, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::BlurFuncNum),
    JS_CFUNC_MAGIC_DEF("focus", 0, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::FocusFuncNum),
    JS_CFUNC_MAGIC_DEF("item", 1, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::ItemFuncNum),
    JS_CFUNC_MAGIC_DEF("namedItem", 1, JSHTMLSelectElementPrototypeFunction::callAsFunction, JSHTMLSelectElement::NamedItemFuncNum)
};

JSValue JSHTMLSelectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLSelectElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLSelectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLSelectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLSelectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLSelectElementAttributesFunctions, countof(JSHTMLSelectElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLSelectElementPrototypeFunctions, countof(JSHTMLSelectElementPrototypeFunctions));
}

static JSClassDef JSHTMLSelectElementClassDefine = 
{
    "HTMLSelectElement",
    .finalizer = JSHTMLSelectElement::finalizer,
    .gc_mark = JSHTMLSelectElement::mark,
};

JSClassID JSHTMLSelectElement::js_class_id = 0;

void JSHTMLSelectElement::init(JSContext* ctx)
{
    if (JSHTMLSelectElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLSelectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLSelectElement::js_class_id, &JSHTMLSelectElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLSelectElementConstructor::self(ctx), JSHTMLSelectElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLSelectElement::js_class_id, JSHTMLSelectElementPrototype::self(ctx));
    }
}

JSValue JSHTMLSelectElement::create(JSContext* ctx, HTMLSelectElement* impl)
{
    JSHTMLSelectElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLSelectElementPrototype::self(ctx), JSHTMLSelectElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLSelectElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLSelectElement* impl = (HTMLSelectElement*)JS_GetOpaque(val, JSHTMLSelectElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLSelectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLSelectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SelectedIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->selectedIndex());
        }
        case ValueAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case LengthAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case FormAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case OptionsAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->options()));
        }
        case DisabledAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MultipleAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBool(ctx, imp->multiple() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->size());
        }
        case TabIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->tabIndex());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLSelectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case SelectedIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setSelectedIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LengthAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            ExceptionCode ec = 0;
            imp->setLength(valueToInt32(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DisabledAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MultipleAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setMultiple(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
            imp->setTabIndex(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLSelectElement::getConstructor(JSContext *ctx)
{
    return JSHTMLSelectElementConstructor::self(ctx);
}

JSValue JSHTMLSelectElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_val, JSHTMLSelectElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLSelectElement::AddFuncNum: {
            ExceptionCode ec = 0;
            HTMLElement* element = toHTMLElement(argv[0]);
            HTMLElement* before = toHTMLElement(argv[1]);
            imp->add(element, before, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSHTMLSelectElement::RemoveFuncNum: {
            return JSHTMLSelectElement::remove(ctx, argc, argv, imp);
        }
        case JSHTMLSelectElement::BlurFuncNum: {
            imp->blur();
            return JS_UNDEFINED;
        }
        case JSHTMLSelectElement::FocusFuncNum: {
            imp->focus();
            return JS_UNDEFINED;
        }
        case JSHTMLSelectElement::ItemFuncNum: {
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (index < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->item(index)));
            return result;
        }
        case JSHTMLSelectElement::NamedItemFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->namedItem(name)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSHTMLSelectElement::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque2(ctx, this_obj, JSHTMLSelectElement::js_class_id);
    return toJS(ctx, imp->item(idx));
}


}
