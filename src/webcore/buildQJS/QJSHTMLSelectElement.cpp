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

static JSCFunctionListEntry JSHTMLSelectElementAttributesFunctions[12];
static bool JSHTMLSelectElementAttributesFunctions_initialized = false;

static void init_JSHTMLSelectElementAttributesFunctions()
{
    if (JSHTMLSelectElementAttributesFunctions_initialized) return;
    JSHTMLSelectElementAttributesFunctions_initialized = true;
    memset(JSHTMLSelectElementAttributesFunctions, 0, sizeof(JSHTMLSelectElementAttributesFunctions));
    JSHTMLSelectElementAttributesFunctions[0].name = "length";
    JSHTMLSelectElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[0].magic = JSHTMLSelectElement::LengthAttrNum;
    JSHTMLSelectElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[1].name = "size";
    JSHTMLSelectElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[1].magic = JSHTMLSelectElement::SizeAttrNum;
    JSHTMLSelectElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[2].name = "form";
    JSHTMLSelectElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[2].magic = JSHTMLSelectElement::FormAttrNum;
    JSHTMLSelectElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLSelectElementAttributesFunctions[3].name = "multiple";
    JSHTMLSelectElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[3].magic = JSHTMLSelectElement::MultipleAttrNum;
    JSHTMLSelectElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[4].name = "type";
    JSHTMLSelectElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[4].magic = JSHTMLSelectElement::TypeAttrNum;
    JSHTMLSelectElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSHTMLSelectElementAttributesFunctions[5].name = "options";
    JSHTMLSelectElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[5].magic = JSHTMLSelectElement::OptionsAttrNum;
    JSHTMLSelectElementAttributesFunctions[5].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSHTMLSelectElementAttributesFunctions[6].name = "tabIndex";
    JSHTMLSelectElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[6].magic = JSHTMLSelectElement::TabIndexAttrNum;
    JSHTMLSelectElementAttributesFunctions[6].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[6].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[7].name = "value";
    JSHTMLSelectElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[7].magic = JSHTMLSelectElement::ValueAttrNum;
    JSHTMLSelectElementAttributesFunctions[7].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[7].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[8].name = "selectedIndex";
    JSHTMLSelectElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[8].magic = JSHTMLSelectElement::SelectedIndexAttrNum;
    JSHTMLSelectElementAttributesFunctions[8].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[8].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[9].name = "disabled";
    JSHTMLSelectElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[9].magic = JSHTMLSelectElement::DisabledAttrNum;
    JSHTMLSelectElementAttributesFunctions[9].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[9].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[10].name = "name";
    JSHTMLSelectElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[10].magic = JSHTMLSelectElement::NameAttrNum;
    JSHTMLSelectElementAttributesFunctions[10].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[10].u.getset.set.setter_magic = JSHTMLSelectElement::putValueProperty;
    JSHTMLSelectElementAttributesFunctions[11].name = "constructor";
    JSHTMLSelectElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLSelectElementAttributesFunctions[11].magic = JSHTMLSelectElement::ConstructorAttrNum;
    JSHTMLSelectElementAttributesFunctions[11].u.getset.get.getter_magic = JSHTMLSelectElement::getValueProperty;
    JSHTMLSelectElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLSelectElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLSelectElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLSelectElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLSelectElementPrototypeFunctions[6];
static bool JSHTMLSelectElementPrototypeFunctions_initialized = false;

static void init_JSHTMLSelectElementPrototypeFunctions()
{
    if (JSHTMLSelectElementPrototypeFunctions_initialized) return;
    JSHTMLSelectElementPrototypeFunctions_initialized = true;
    memset(JSHTMLSelectElementPrototypeFunctions, 0, sizeof(JSHTMLSelectElementPrototypeFunctions));
    JSHTMLSelectElementPrototypeFunctions[0].name = "remove";
    JSHTMLSelectElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[0].magic = JSHTMLSelectElement::RemoveFuncNum;
    JSHTMLSelectElementPrototypeFunctions[0].u.func.length = 0;
    JSHTMLSelectElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
    JSHTMLSelectElementPrototypeFunctions[1].name = "add";
    JSHTMLSelectElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[1].magic = JSHTMLSelectElement::AddFuncNum;
    JSHTMLSelectElementPrototypeFunctions[1].u.func.length = 2;
    JSHTMLSelectElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
    JSHTMLSelectElementPrototypeFunctions[2].name = "blur";
    JSHTMLSelectElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[2].magic = JSHTMLSelectElement::BlurFuncNum;
    JSHTMLSelectElementPrototypeFunctions[2].u.func.length = 0;
    JSHTMLSelectElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
    JSHTMLSelectElementPrototypeFunctions[3].name = "focus";
    JSHTMLSelectElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[3].magic = JSHTMLSelectElement::FocusFuncNum;
    JSHTMLSelectElementPrototypeFunctions[3].u.func.length = 0;
    JSHTMLSelectElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
    JSHTMLSelectElementPrototypeFunctions[4].name = "item";
    JSHTMLSelectElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[4].magic = JSHTMLSelectElement::ItemFuncNum;
    JSHTMLSelectElementPrototypeFunctions[4].u.func.length = 1;
    JSHTMLSelectElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
    JSHTMLSelectElementPrototypeFunctions[5].name = "namedItem";
    JSHTMLSelectElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLSelectElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSHTMLSelectElementPrototypeFunctions[5].magic = JSHTMLSelectElement::NamedItemFuncNum;
    JSHTMLSelectElementPrototypeFunctions[5].u.func.length = 1;
    JSHTMLSelectElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLSelectElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSHTMLSelectElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLSelectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLSelectElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLSelectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLSelectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLSelectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLSelectElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLSelectElementAttributesFunctions, countof(JSHTMLSelectElementAttributesFunctions));
    init_JSHTMLSelectElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLSelectElementPrototypeFunctions, countof(JSHTMLSelectElementPrototypeFunctions));
}

static JSClassDef JSHTMLSelectElementClassDefine;
static bool JSHTMLSelectElementClassDefine_initialized = false;

static void init_JSHTMLSelectElementClassDefine()
{
    if (JSHTMLSelectElementClassDefine_initialized) return;
    JSHTMLSelectElementClassDefine_initialized = true;
    memset(&JSHTMLSelectElementClassDefine, 0, sizeof(JSHTMLSelectElementClassDefine));
    JSHTMLSelectElementClassDefine.class_name = "HTMLSelectElement";
    JSHTMLSelectElementClassDefine.finalizer = JSHTMLSelectElement::finalizer;
    JSHTMLSelectElementClassDefine.gc_mark = JSHTMLSelectElement::mark;
}

JSClassID JSHTMLSelectElement::js_class_id = 0;

void JSHTMLSelectElement::init(JSContext* ctx)
{
    if (JSHTMLSelectElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLSelectElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLSelectElement::create(JSContext* ctx, HTMLSelectElement* impl)
{
    JSHTMLSelectElement::init(ctx);
    JSValue _proto = JSHTMLSelectElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLSelectElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLSelectElement* impl = (HTMLSelectElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case SelectedIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->selectedIndex());
        }
        case ValueAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case LengthAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case FormAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case OptionsAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->options()));
        }
        case DisabledAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case MultipleAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->multiple() ? 1 : 0);
        }
        case NameAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->size());
        }
        case TabIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->tabIndex());
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
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSelectedIndex(valueToInt32(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LengthAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            ExceptionCode ec = 0;
            imp->setLength(valueToInt32(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
        case DisabledAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
        case MultipleAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setMultiple(valueToBoolean(ctx, value));
            break;
        }
        case NameAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
        case TabIndexAttrNum: {
            HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLSelectElement* imp = (HTMLSelectElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            return JSHTMLSelectElement::remove(ctx, this_val, argc, argv, imp);
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
