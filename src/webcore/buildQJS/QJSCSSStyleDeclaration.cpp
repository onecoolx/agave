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

static const JSCFunctionListEntry JSCSSStyleDeclarationAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cssText", JSCSSStyleDeclaration::getValueProperty, JSCSSStyleDeclaration::putValueProperty, JSCSSStyleDeclaration::CssTextAttrNum),
    JS_CGETSET_MAGIC_DEF("length", JSCSSStyleDeclaration::getValueProperty, NULL, JSCSSStyleDeclaration::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("parentRule", JSCSSStyleDeclaration::getValueProperty, NULL, JSCSSStyleDeclaration::ParentRuleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSStyleDeclaration::getValueProperty, NULL, JSCSSStyleDeclaration::ConstructorAttrNum)
};

class JSCSSStyleDeclarationConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSStyleDeclarationConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSStyleDeclarationConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSStyleDeclaration.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSStyleDeclarationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSStyleDeclaration.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSStyleDeclarationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSStyleDeclarationPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("removeProperty", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::RemovePropertyFuncNum),
    JS_CFUNC_MAGIC_DEF("getPropertyPriority", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::GetPropertyPriorityFuncNum),
    JS_CFUNC_MAGIC_DEF("getPropertyValue", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::GetPropertyValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getPropertyShorthand", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::GetPropertyShorthandFuncNum),
    JS_CFUNC_MAGIC_DEF("getPropertyCSSValue", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::GetPropertyCSSValueFuncNum),
    JS_CFUNC_MAGIC_DEF("item", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::ItemFuncNum),
    JS_CFUNC_MAGIC_DEF("setProperty", 3, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::SetPropertyFuncNum),
    JS_CFUNC_MAGIC_DEF("isPropertyImplicit", 1, JSCSSStyleDeclarationPrototypeFunction::callAsFunction, JSCSSStyleDeclaration::IsPropertyImplicitFuncNum)
};

JSValue JSCSSStyleDeclarationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSStyleDeclaration.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSStyleDeclarationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSStyleDeclaration.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSStyleDeclarationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleDeclarationAttributesFunctions, countof(JSCSSStyleDeclarationAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleDeclarationPrototypeFunctions, countof(JSCSSStyleDeclarationPrototypeFunctions));
}

static JSClassDef JSCSSStyleDeclarationClassDefine = 
{
    "CSSStyleDeclaration",
    .finalizer = JSCSSStyleDeclaration::finalizer,
    .gc_mark = JSCSSStyleDeclaration::mark,
};

JSClassID JSCSSStyleDeclaration::js_class_id = 0;

void JSCSSStyleDeclaration::init(JSContext* ctx)
{
    if (JSCSSStyleDeclaration::js_class_id == 0) {
        JS_NewClassID(&JSCSSStyleDeclaration::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSStyleDeclaration::js_class_id, &JSCSSStyleDeclarationClassDefine);
        JS_SetConstructor(ctx, JSCSSStyleDeclarationConstructor::self(ctx), JSCSSStyleDeclarationPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSStyleDeclaration::js_class_id, JSCSSStyleDeclarationPrototype::self(ctx));
    }
}

JSValue JSCSSStyleDeclaration::create(JSContext* ctx, CSSStyleDeclaration* impl)
{
    JSCSSStyleDeclaration::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSStyleDeclarationPrototype::self(ctx), JSCSSStyleDeclaration::js_class_id);
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
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_val, JSCSSStyleDeclaration::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case LengthAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_val, JSCSSStyleDeclaration::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ParentRuleAttrNum: {
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_val, JSCSSStyleDeclaration::js_class_id);
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
            CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_val, JSCSSStyleDeclaration::js_class_id);
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
    CSSStyleDeclaration* imp = (CSSStyleDeclaration*)JS_GetOpaque2(ctx, this_val, JSCSSStyleDeclaration::js_class_id);
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
