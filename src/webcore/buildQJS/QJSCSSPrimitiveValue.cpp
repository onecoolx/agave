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

#include "QJSCSSPrimitiveValue.h"

#include "CSSPrimitiveValue.h"
#include "Counter.h"
#include "QJSCounter.h"
#include "QJSRect.h"
#include "Rect.h"
#include "qjs_css.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSPrimitiveValueAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("primitiveType", JSCSSPrimitiveValue::getValueProperty, NULL, JSCSSPrimitiveValue::PrimitiveTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSPrimitiveValue::getValueProperty, NULL, JSCSSPrimitiveValue::ConstructorAttrNum)
};

class JSCSSPrimitiveValueConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSPrimitiveValueConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSCSSPrimitiveValueConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("CSS_IN", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_IN),
    JS_CGETSET_MAGIC_DEF("CSS_EMS", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_EMS),
    JS_CGETSET_MAGIC_DEF("CSS_EXS", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_EXS),
    JS_CGETSET_MAGIC_DEF("CSS_RAD", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_RAD),
    JS_CGETSET_MAGIC_DEF("CSS_KHZ", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_KHZ),
    JS_CGETSET_MAGIC_DEF("CSS_PX", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_PX),
    JS_CGETSET_MAGIC_DEF("CSS_CM", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_CM),
    JS_CGETSET_MAGIC_DEF("CSS_S", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_S),
    JS_CGETSET_MAGIC_DEF("CSS_DIMENSION", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_DIMENSION),
    JS_CGETSET_MAGIC_DEF("CSS_UNKNOWN", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("CSS_GRAD", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_GRAD),
    JS_CGETSET_MAGIC_DEF("CSS_IDENT", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_IDENT),
    JS_CGETSET_MAGIC_DEF("CSS_URI", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_URI),
    JS_CGETSET_MAGIC_DEF("CSS_NUMBER", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_NUMBER),
    JS_CGETSET_MAGIC_DEF("CSS_COUNTER", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_COUNTER),
    JS_CGETSET_MAGIC_DEF("CSS_PERCENTAGE", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_PERCENTAGE),
    JS_CGETSET_MAGIC_DEF("CSS_MM", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_MM),
    JS_CGETSET_MAGIC_DEF("CSS_PT", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_PT),
    JS_CGETSET_MAGIC_DEF("CSS_PC", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_PC),
    JS_CGETSET_MAGIC_DEF("CSS_DEG", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_DEG),
    JS_CGETSET_MAGIC_DEF("CSS_MS", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_MS),
    JS_CGETSET_MAGIC_DEF("CSS_HZ", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_HZ),
    JS_CGETSET_MAGIC_DEF("CSS_STRING", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_STRING),
    JS_CGETSET_MAGIC_DEF("CSS_ATTR", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_ATTR),
    JS_CGETSET_MAGIC_DEF("CSS_RECT", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_RECT),
    JS_CGETSET_MAGIC_DEF("CSS_RGBCOLOR", JSCSSPrimitiveValueConstructor::getValueProperty, NULL, CSSPrimitiveValue::CSS_RGBCOLOR)
};

JSValue JSCSSPrimitiveValueConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSPrimitiveValue.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSPrimitiveValueConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSPrimitiveValue.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSPrimitiveValueConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValueConstructorFunctions, countof(JSCSSPrimitiveValueConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSCSSPrimitiveValuePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("CSS_IN", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_IN),
    JS_CGETSET_MAGIC_DEF("CSS_EMS", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_EMS),
    JS_CGETSET_MAGIC_DEF("CSS_EXS", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_EXS),
    JS_CGETSET_MAGIC_DEF("CSS_RAD", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_RAD),
    JS_CGETSET_MAGIC_DEF("CSS_KHZ", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_KHZ),
    JS_CGETSET_MAGIC_DEF("CSS_PX", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_PX),
    JS_CGETSET_MAGIC_DEF("CSS_CM", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_CM),
    JS_CGETSET_MAGIC_DEF("CSS_S", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_S),
    JS_CGETSET_MAGIC_DEF("CSS_DIMENSION", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_DIMENSION),
    JS_CGETSET_MAGIC_DEF("CSS_UNKNOWN", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("CSS_GRAD", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_GRAD),
    JS_CGETSET_MAGIC_DEF("CSS_IDENT", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_IDENT),
    JS_CGETSET_MAGIC_DEF("CSS_URI", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_URI),
    JS_CGETSET_MAGIC_DEF("CSS_NUMBER", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_NUMBER),
    JS_CGETSET_MAGIC_DEF("CSS_COUNTER", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_COUNTER),
    JS_CGETSET_MAGIC_DEF("CSS_PERCENTAGE", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_PERCENTAGE),
    JS_CGETSET_MAGIC_DEF("CSS_MM", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_MM),
    JS_CGETSET_MAGIC_DEF("CSS_PT", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_PT),
    JS_CGETSET_MAGIC_DEF("CSS_PC", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_PC),
    JS_CGETSET_MAGIC_DEF("CSS_DEG", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_DEG),
    JS_CGETSET_MAGIC_DEF("CSS_MS", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_MS),
    JS_CGETSET_MAGIC_DEF("CSS_HZ", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_HZ),
    JS_CGETSET_MAGIC_DEF("CSS_STRING", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_STRING),
    JS_CGETSET_MAGIC_DEF("CSS_ATTR", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_ATTR),
    JS_CGETSET_MAGIC_DEF("CSS_RECT", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_RECT),
    JS_CGETSET_MAGIC_DEF("CSS_RGBCOLOR", JSCSSPrimitiveValuePrototype::getValueProperty, NULL, CSSPrimitiveValue::CSS_RGBCOLOR)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSPrimitiveValuePrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("setStringValue", 2, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::SetStringValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getStringValue", 0, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::GetStringValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getRGBColorValue", 0, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::GetRGBColorValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getCounterValue", 0, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::GetCounterValueFuncNum),
    JS_CFUNC_MAGIC_DEF("setFloatValue", 2, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::SetFloatValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getFloatValue", 1, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::GetFloatValueFuncNum),
    JS_CFUNC_MAGIC_DEF("getRectValue", 0, JSCSSPrimitiveValuePrototypeFunction::callAsFunction, JSCSSPrimitiveValue::GetRectValueFuncNum)
};

JSValue JSCSSPrimitiveValuePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSPrimitiveValue.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSValuePrototype::self(ctx));
        JSCSSPrimitiveValuePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSPrimitiveValue.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSPrimitiveValuePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValueAttributesFunctions, countof(JSCSSPrimitiveValueAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValuePrototypeConstantsFunctions, countof(JSCSSPrimitiveValuePrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPrimitiveValuePrototypeFunctions, countof(JSCSSPrimitiveValuePrototypeFunctions));
}

JSValue JSCSSPrimitiveValuePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSCSSPrimitiveValueClassDefine = 
{
    "CSSPrimitiveValue",
    .finalizer = JSCSSPrimitiveValue::finalizer,
    .gc_mark = JSCSSPrimitiveValue::mark,
};

JSClassID JSCSSPrimitiveValue::js_class_id = 0;

void JSCSSPrimitiveValue::init(JSContext* ctx)
{
    if (JSCSSPrimitiveValue::js_class_id == 0) {
        JS_NewClassID(&JSCSSPrimitiveValue::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSPrimitiveValue::js_class_id, &JSCSSPrimitiveValueClassDefine);
        JS_SetConstructor(ctx, JSCSSPrimitiveValueConstructor::self(ctx), JSCSSPrimitiveValuePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSPrimitiveValue::js_class_id, JSCSSPrimitiveValuePrototype::self(ctx));
    }
}

JSValue JSCSSPrimitiveValue::create(JSContext* ctx, CSSPrimitiveValue* impl)
{
    JSCSSPrimitiveValue::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSPrimitiveValuePrototype::self(ctx), JSCSSPrimitiveValue::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSPrimitiveValue::finalizer(JSRuntime* rt, JSValue val)
{
    CSSPrimitiveValue* impl = (CSSPrimitiveValue*)JS_GetOpaque(val, JSCSSPrimitiveValue::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSPrimitiveValue::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSValue::mark(rt, val, mark_func);
}

JSValue JSCSSPrimitiveValue::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PrimitiveTypeAttrNum: {
            CSSPrimitiveValue* imp = (CSSPrimitiveValue*)JS_GetOpaque2(ctx, this_val, JSCSSPrimitiveValue::js_class_id);
            return JS_NewBigUint64(ctx, imp->primitiveType());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSPrimitiveValue::getConstructor(JSContext *ctx)
{
    return JSCSSPrimitiveValueConstructor::self(ctx);
}

JSValue JSCSSPrimitiveValuePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSPrimitiveValue* imp = (CSSPrimitiveValue*)JS_GetOpaque2(ctx, this_val, JSCSSPrimitiveValue::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSPrimitiveValue::SetFloatValueFuncNum: {
            ExceptionCode ec = 0;
            unsigned short unitType = valueToInt32(ctx, argv[0]);
            float floatValue = valueToFloat(ctx, argv[1]);
            imp->setFloatValue(unitType, floatValue, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCSSPrimitiveValue::GetFloatValueFuncNum: {
            unsigned short unitType = valueToInt32(ctx, argv[0]);

            JSValue result = JS_NewBigUint64(ctx, imp->getFloatValue(unitType));
            return result;
        }
        case JSCSSPrimitiveValue::SetStringValueFuncNum: {
            ExceptionCode ec = 0;
            unsigned short stringType = valueToInt32(ctx, argv[0]);
            String stringValue = valueToString(ctx, argv[1]);
            imp->setStringValue(stringType, stringValue, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCSSPrimitiveValue::GetStringValueFuncNum: {

            JSValue result = JS_NewString(ctx, ((const String&)imp->getStringValue()).utf8().data());
            return result;
        }
        case JSCSSPrimitiveValue::GetCounterValueFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->getCounterValue()));
            return result;
        }
        case JSCSSPrimitiveValue::GetRectValueFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->getRectValue()));
            return result;
        }
        case JSCSSPrimitiveValue::GetRGBColorValueFuncNum: {

            JSValue result = getJSRGBColor(ctx, imp->getRGBColorValue());
            return result;
        }
    }
    return JS_NULL;
}


}
