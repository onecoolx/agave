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

#include "QJSCSSValue.h"

#include "CSSValue.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSValueAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cssText", JSCSSValue::getValueProperty, JSCSSValue::putValueProperty, JSCSSValue::CssTextAttrNum),
    JS_CGETSET_MAGIC_DEF("cssValueType", JSCSSValue::getValueProperty, NULL, JSCSSValue::CssValueTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSValue::getValueProperty, NULL, JSCSSValue::ConstructorAttrNum)
};

class JSCSSValueConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSValueConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSCSSValueConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("CSS_INHERIT", JSCSSValueConstructor::getValueProperty, NULL, CSSValue::CSS_INHERIT),
    JS_CGETSET_MAGIC_DEF("CSS_CUSTOM", JSCSSValueConstructor::getValueProperty, NULL, CSSValue::CSS_CUSTOM),
    JS_CGETSET_MAGIC_DEF("CSS_PRIMITIVE_VALUE", JSCSSValueConstructor::getValueProperty, NULL, CSSValue::CSS_PRIMITIVE_VALUE),
    JS_CGETSET_MAGIC_DEF("CSS_VALUE_LIST", JSCSSValueConstructor::getValueProperty, NULL, CSSValue::CSS_VALUE_LIST)
};

JSValue JSCSSValueConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSValue.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSValueConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSValue.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSValueConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueConstructorFunctions, countof(JSCSSValueConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSCSSValuePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("CSS_INHERIT", JSCSSValuePrototype::getValueProperty, NULL, CSSValue::CSS_INHERIT),
    JS_CGETSET_MAGIC_DEF("CSS_CUSTOM", JSCSSValuePrototype::getValueProperty, NULL, CSSValue::CSS_CUSTOM),
    JS_CGETSET_MAGIC_DEF("CSS_PRIMITIVE_VALUE", JSCSSValuePrototype::getValueProperty, NULL, CSSValue::CSS_PRIMITIVE_VALUE),
    JS_CGETSET_MAGIC_DEF("CSS_VALUE_LIST", JSCSSValuePrototype::getValueProperty, NULL, CSSValue::CSS_VALUE_LIST)
};

JSValue JSCSSValuePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSValue.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSValuePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSValue.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSValuePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueAttributesFunctions, countof(JSCSSValueAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValuePrototypeConstantsFunctions, countof(JSCSSValuePrototypeConstantsFunctions));
}

JSValue JSCSSValuePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSCSSValueClassDefine = 
{
    "CSSValue",
    .finalizer = JSCSSValue::finalizer,
    .gc_mark = JSCSSValue::mark,
};

JSClassID JSCSSValue::js_class_id = 0;

void JSCSSValue::init(JSContext* ctx)
{
    if (JSCSSValue::js_class_id == 0) {
        JS_NewClassID(&JSCSSValue::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSValue::js_class_id, &JSCSSValueClassDefine);
        JS_SetConstructor(ctx, JSCSSValueConstructor::self(ctx), JSCSSValuePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSValue::js_class_id, JSCSSValuePrototype::self(ctx));
    }
}

JSValue JSCSSValue::create(JSContext* ctx, CSSValue* impl)
{
    JSCSSValue::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSValuePrototype::self(ctx), JSCSSValue::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSValue::finalizer(JSRuntime* rt, JSValue val)
{
    CSSValue* impl = (CSSValue*)JS_GetOpaque(val, JSCSSValue::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSValue::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCSSValue::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CssTextAttrNum: {
            CSSValue* imp = (CSSValue*)JS_GetOpaque2(ctx, this_val, JSCSSValue::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case CssValueTypeAttrNum: {
            CSSValue* imp = (CSSValue*)JS_GetOpaque2(ctx, this_val, JSCSSValue::js_class_id);
            return JS_NewBigUint64(ctx, imp->cssValueType());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSValue::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CssTextAttrNum: {
            CSSValue* imp = (CSSValue*)JS_GetOpaque2(ctx, this_val, JSCSSValue::js_class_id);
            ExceptionCode ec = 0;
            imp->setCssText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSValue::getConstructor(JSContext *ctx)
{
    return JSCSSValueConstructor::self(ctx);
}


CSSValue* toCSSValue(JSValue val)
{
    if (JS_IsObject(val)) {
        CSSValue* impl = (CSSValue*)JS_GetOpaque(val, JSCSSValue::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
