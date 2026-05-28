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

#include "QJSCSSStyleDeclaration.h"

#include "CSSRule.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "QJSCSSRule.h"
#include "QJSCSSValue.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSStyleDeclarationAttributesFunctions[4];
static bool JSCSSStyleDeclarationAttributesFunctions_initialized = false;

static void init_JSCSSStyleDeclarationAttributesFunctions()
{
    if (JSCSSStyleDeclarationAttributesFunctions_initialized) return;
    JSCSSStyleDeclarationAttributesFunctions_initialized = true;
    memset(JSCSSStyleDeclarationAttributesFunctions, 0, sizeof(JSCSSStyleDeclarationAttributesFunctions));
    JSCSSStyleDeclarationAttributesFunctions[0].name = "cssText";
    JSCSSStyleDeclarationAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleDeclarationAttributesFunctions[0].magic = JSCSSStyleDeclaration::CssTextAttrNum;
    JSCSSStyleDeclarationAttributesFunctions[0].u.getset.get.getter_magic = JSCSSStyleDeclaration::getValueProperty;
    JSCSSStyleDeclarationAttributesFunctions[0].u.getset.set.setter_magic = JSCSSStyleDeclaration::putValueProperty;
    JSCSSStyleDeclarationAttributesFunctions[1].name = "length";
    JSCSSStyleDeclarationAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleDeclarationAttributesFunctions[1].magic = JSCSSStyleDeclaration::LengthAttrNum;
    JSCSSStyleDeclarationAttributesFunctions[1].u.getset.get.getter_magic = JSCSSStyleDeclaration::getValueProperty;
    JSCSSStyleDeclarationAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSStyleDeclarationAttributesFunctions[2].name = "parentRule";
    JSCSSStyleDeclarationAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleDeclarationAttributesFunctions[2].magic = JSCSSStyleDeclaration::ParentRuleAttrNum;
    JSCSSStyleDeclarationAttributesFunctions[2].u.getset.get.getter_magic = JSCSSStyleDeclaration::getValueProperty;
    JSCSSStyleDeclarationAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSCSSStyleDeclarationAttributesFunctions[3].name = "constructor";
    JSCSSStyleDeclarationAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleDeclarationAttributesFunctions[3].magic = JSCSSStyleDeclaration::ConstructorAttrNum;
    JSCSSStyleDeclarationAttributesFunctions[3].u.getset.get.getter_magic = JSCSSStyleDeclaration::getValueProperty;
    JSCSSStyleDeclarationAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSCSSStyleDeclarationConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSCSSStyleDeclarationConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSStyleDeclaration.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSStyleDeclarationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSStyleDeclaration.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSStyleDeclarationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSCSSStyleDeclarationPrototypeFunctions[8];
static bool JSCSSStyleDeclarationPrototypeFunctions_initialized = false;

static void init_JSCSSStyleDeclarationPrototypeFunctions()
{
    if (JSCSSStyleDeclarationPrototypeFunctions_initialized) return;
    JSCSSStyleDeclarationPrototypeFunctions_initialized = true;
    memset(JSCSSStyleDeclarationPrototypeFunctions, 0, sizeof(JSCSSStyleDeclarationPrototypeFunctions));
    JSCSSStyleDeclarationPrototypeFunctions[0].name = "removeProperty";
    JSCSSStyleDeclarationPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[0].magic = JSCSSStyleDeclaration::RemovePropertyFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[0].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[0].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[1].name = "getPropertyPriority";
    JSCSSStyleDeclarationPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[1].magic = JSCSSStyleDeclaration::GetPropertyPriorityFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[1].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[1].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[2].name = "getPropertyValue";
    JSCSSStyleDeclarationPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[2].magic = JSCSSStyleDeclaration::GetPropertyValueFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[2].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[2].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[3].name = "getPropertyShorthand";
    JSCSSStyleDeclarationPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[3].magic = JSCSSStyleDeclaration::GetPropertyShorthandFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[3].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[3].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[4].name = "getPropertyCSSValue";
    JSCSSStyleDeclarationPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[4].magic = JSCSSStyleDeclaration::GetPropertyCSSValueFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[4].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[4].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[5].name = "item";
    JSCSSStyleDeclarationPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[5].magic = JSCSSStyleDeclaration::ItemFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[5].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[5].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[6].name = "setProperty";
    JSCSSStyleDeclarationPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[6].magic = JSCSSStyleDeclaration::SetPropertyFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[6].u.func.length = 3;
    JSCSSStyleDeclarationPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[6].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
    JSCSSStyleDeclarationPrototypeFunctions[7].name = "isPropertyImplicit";
    JSCSSStyleDeclarationPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleDeclarationPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSCSSStyleDeclarationPrototypeFunctions[7].magic = JSCSSStyleDeclaration::IsPropertyImplicitFuncNum;
    JSCSSStyleDeclarationPrototypeFunctions[7].u.func.length = 1;
    JSCSSStyleDeclarationPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleDeclarationPrototypeFunctions[7].u.func.cfunc.generic_magic = JSCSSStyleDeclarationPrototypeFunction::callAsFunction;
}

JSValue JSCSSStyleDeclarationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSStyleDeclaration.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSStyleDeclarationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSStyleDeclaration.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSStyleDeclarationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSStyleDeclarationAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleDeclarationAttributesFunctions, countof(JSCSSStyleDeclarationAttributesFunctions));
    init_JSCSSStyleDeclarationPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleDeclarationPrototypeFunctions, countof(JSCSSStyleDeclarationPrototypeFunctions));
}

static String cssPropertyName(const char* name)
{
    if (!name || !name[0])
        return String();
    Vector<char> buf;
    for (int i = 0; name[i]; ++i) {
        char c = name[i];
        if (c >= 'A' && c <= 'Z') {
            buf.append('-');
            buf.append(c - 'A' + 'a');
        } else {
            buf.append(c);
        }
    }
    buf.append('\0');
    String prop(buf.data());
    if (prop.startsWith("webkit-") || prop.startsWith("khtml-"))
        prop = "-" + prop;
    return prop;
}

static int js_cssstyledecl_get_own_property(JSContext *ctx, JSPropertyDescriptor *desc,
                                             JSValueConst obj, JSAtom prop)
{
    CSSStyleDeclaration* impl = (CSSStyleDeclaration*)JS_GetOpaque(obj, JSCSSStyleDeclaration::js_class_id);
    if (!impl)
        return 0;
    const char* str = JS_AtomToCString(ctx, prop);
    if (!str)
        return 0;
    if ((str[0] >= '0' && str[0] <= '9') || !strcmp(str, "length") || !strcmp(str, "cssText") || !strcmp(str, "constructor")) {
        JS_FreeCString(ctx, str);
        return 0;
    }
    if (desc) {
        desc->flags = JS_PROP_ENUMERABLE | JS_PROP_WRITABLE;
        desc->value = JSCSSStyleDeclaration::nameGetter(ctx, obj, str);
        desc->getter = JS_UNDEFINED;
        desc->setter = JS_UNDEFINED;
    }
    JS_FreeCString(ctx, str);
    return 1;
}

static int js_cssstyledecl_set_property(JSContext *ctx, JSValueConst obj, JSAtom atom,
                                         JSValueConst value, JSValueConst receiver, int flags)
{
    CSSStyleDeclaration* impl = (CSSStyleDeclaration*)JS_GetOpaque(obj, JSCSSStyleDeclaration::js_class_id);
    if (!impl)
        return 0;
    const char* str = JS_AtomToCString(ctx, atom);
    if (!str)
        return 0;
    if ((str[0] >= '0' && str[0] <= '9') || !strcmp(str, "length") || !strcmp(str, "cssText") || !strcmp(str, "constructor")) {
        JS_FreeCString(ctx, str);
        return 0; // let QuickJS handle built-in properties normally
    }
    String val = valueToString(ctx, value);
    ExceptionCode ec = 0;
    impl->setProperty(cssPropertyName(str), val, ec);
    JS_FreeCString(ctx, str);
    return 1;
}

static JSClassExoticMethods js_cssstyledecl_exotic;
static bool js_cssstyledecl_exotic_initialized = false;

static void init_js_cssstyledecl_exotic()
{
    if (js_cssstyledecl_exotic_initialized) return;
    js_cssstyledecl_exotic_initialized = true;
    memset(&js_cssstyledecl_exotic, 0, sizeof(js_cssstyledecl_exotic));
    js_cssstyledecl_exotic.get_own_property = js_cssstyledecl_get_own_property;
    js_cssstyledecl_exotic.set_property = js_cssstyledecl_set_property;
}

static JSClassDef JSCSSStyleDeclarationClassDefine;
static bool JSCSSStyleDeclarationClassDefine_initialized = false;

static void init_JSCSSStyleDeclarationClassDefine()
{
    if (JSCSSStyleDeclarationClassDefine_initialized) return;
    JSCSSStyleDeclarationClassDefine_initialized = true;
    init_js_cssstyledecl_exotic();
    memset(&JSCSSStyleDeclarationClassDefine, 0, sizeof(JSCSSStyleDeclarationClassDefine));
    JSCSSStyleDeclarationClassDefine.class_name = "CSSStyleDeclaration";
    JSCSSStyleDeclarationClassDefine.finalizer = JSCSSStyleDeclaration::finalizer;
    JSCSSStyleDeclarationClassDefine.gc_mark = JSCSSStyleDeclaration::mark;
    JSCSSStyleDeclarationClassDefine.exotic = &js_cssstyledecl_exotic;
}

JSClassID JSCSSStyleDeclaration::js_class_id = 0;

void JSCSSStyleDeclaration::init(JSContext* ctx)
{
    if (JSCSSStyleDeclaration::js_class_id == 0) {
        init_JSCSSStyleDeclarationClassDefine();
        JS_NewClassID(&JSCSSStyleDeclaration::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSStyleDeclaration::js_class_id, &JSCSSStyleDeclarationClassDefine);
        JS_SetConstructor(ctx, JSCSSStyleDeclarationConstructor::self(ctx), JSCSSStyleDeclarationPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSStyleDeclaration::js_class_id, JSCSSStyleDeclarationPrototype::self(ctx));
    }
}

JSValue JSCSSStyleDeclaration::create(JSContext* ctx, CSSStyleDeclaration* impl)
{
    JSCSSStyleDeclaration::init(ctx);
    JSValue _proto = JSCSSStyleDeclarationPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSStyleDeclaration::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSStyleDeclaration::finalizer(JSRuntime* rt, JSValue val)
{
    CSSStyleDeclaration* impl = (CSSStyleDeclaration*)JS_GetOpaque(val, JSCSSStyleDeclaration::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSStyleDeclaration::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCSSStyleDeclaration::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CssTextAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque(this_val, JSCSSStyleDeclaration::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case LengthAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque(this_val, JSCSSStyleDeclaration::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case ParentRuleAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque(this_val, JSCSSStyleDeclaration::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentRule()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSStyleDeclaration::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CssTextAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque(this_val, JSCSSStyleDeclaration::js_class_id);
            ExceptionCode ec = 0;
            imp->setCssText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSStyleDeclaration::getConstructor(JSContext *ctx)
{
    return JSCSSStyleDeclarationConstructor::self(ctx);
}

JSValue JSCSSStyleDeclarationPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque(this_val, JSCSSStyleDeclaration::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSStyleDeclaration::GetPropertyValueFuncNum: {
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getPropertyValue(propertyName));
            return result;
        }
        case JSCSSStyleDeclaration::GetPropertyCSSValueFuncNum: {
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPropertyCSSValue(propertyName)));
            return result;
        }
        case JSCSSStyleDeclaration::RemovePropertyFuncNum: {
            ExceptionCode ec = 0;
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->removeProperty(propertyName, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSCSSStyleDeclaration::GetPropertyPriorityFuncNum: {
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getPropertyPriority(propertyName));
            return result;
        }
        case JSCSSStyleDeclaration::SetPropertyFuncNum: {
            ExceptionCode ec = 0;
            String propertyName = valueToString(ctx, argv[0]);
            String value = valueToStringWithNullCheck(ctx, argv[1]);
            String priority = valueToString(ctx, argv[2]);
            imp->setProperty(propertyName, value, priority, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCSSStyleDeclaration::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = jsStringOrNull(ctx, imp->item(index));
            return result;
        }
        case JSCSSStyleDeclaration::GetPropertyShorthandFuncNum: {
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = jsStringOrNull(ctx, imp->getPropertyShorthand(propertyName));
            return result;
        }
        case JSCSSStyleDeclaration::IsPropertyImplicitFuncNum: {
            String propertyName = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->isPropertyImplicit(propertyName) ? 1 : 0);
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSCSSStyleDeclaration::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_obj, JSCSSStyleDeclaration::js_class_id);
    return jsStringOrNull(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, CSSStyleDeclaration* obj)
{
    return QJS::cacheDOMObject<CSSStyleDeclaration, JSCSSStyleDeclaration>(ctx, obj);
}

CSSStyleDeclaration* toCSSStyleDeclaration(JSValue val)
{
    if (JS_IsObject(val)) {
        CSSStyleDeclaration* impl = (CSSStyleDeclaration*)JS_GetOpaque(val, JSCSSStyleDeclaration::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
