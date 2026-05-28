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

#include "QJSCSSValue.h"

#include "CSSValue.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSValueAttributesFunctions[3];
static bool JSCSSValueAttributesFunctions_initialized = false;

static void init_JSCSSValueAttributesFunctions()
{
    if (JSCSSValueAttributesFunctions_initialized) return;
    JSCSSValueAttributesFunctions_initialized = true;
    memset(JSCSSValueAttributesFunctions, 0, sizeof(JSCSSValueAttributesFunctions));
    JSCSSValueAttributesFunctions[0].name = "cssText";
    JSCSSValueAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSValueAttributesFunctions[0].magic = JSCSSValue::CssTextAttrNum;
    JSCSSValueAttributesFunctions[0].u.getset.get.getter_magic = JSCSSValue::getValueProperty;
    JSCSSValueAttributesFunctions[0].u.getset.set.setter_magic = JSCSSValue::putValueProperty;
    JSCSSValueAttributesFunctions[1].name = "cssValueType";
    JSCSSValueAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSValueAttributesFunctions[1].magic = JSCSSValue::CssValueTypeAttrNum;
    JSCSSValueAttributesFunctions[1].u.getset.get.getter_magic = JSCSSValue::getValueProperty;
    JSCSSValueAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSValueAttributesFunctions[2].name = "constructor";
    JSCSSValueAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSValueAttributesFunctions[2].magic = JSCSSValue::ConstructorAttrNum;
    JSCSSValueAttributesFunctions[2].u.getset.get.getter_magic = JSCSSValue::getValueProperty;
    JSCSSValueAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSCSSValueConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSCSSValueConstructorFunctions[4];
static bool JSCSSValueConstructorFunctions_initialized = false;

static void init_JSCSSValueConstructorFunctions()
{
    if (JSCSSValueConstructorFunctions_initialized) return;
    JSCSSValueConstructorFunctions_initialized = true;
    memset(JSCSSValueConstructorFunctions, 0, sizeof(JSCSSValueConstructorFunctions));
    JSCSSValueConstructorFunctions[0].name = "CSS_INHERIT";
    JSCSSValueConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSCSSValueConstructorFunctions[0].u.i32 = (int32_t)CSSValue::CSS_INHERIT;
    JSCSSValueConstructorFunctions[1].name = "CSS_PRIMITIVE_VALUE";
    JSCSSValueConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSCSSValueConstructorFunctions[1].u.i32 = (int32_t)CSSValue::CSS_PRIMITIVE_VALUE;
    JSCSSValueConstructorFunctions[2].name = "CSS_VALUE_LIST";
    JSCSSValueConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSCSSValueConstructorFunctions[2].u.i32 = (int32_t)CSSValue::CSS_VALUE_LIST;
    JSCSSValueConstructorFunctions[3].name = "CSS_CUSTOM";
    JSCSSValueConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValueConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSCSSValueConstructorFunctions[3].u.i32 = (int32_t)CSSValue::CSS_CUSTOM;
}

JSValue JSCSSValueConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSValue.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSValueConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSValue.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSValueConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSValueConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueConstructorFunctions, countof(JSCSSValueConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSCSSValuePrototypeConstantsFunctions[4];
static bool JSCSSValuePrototypeConstantsFunctions_initialized = false;

static void init_JSCSSValuePrototypeConstantsFunctions()
{
    if (JSCSSValuePrototypeConstantsFunctions_initialized) return;
    JSCSSValuePrototypeConstantsFunctions_initialized = true;
    memset(JSCSSValuePrototypeConstantsFunctions, 0, sizeof(JSCSSValuePrototypeConstantsFunctions));
    JSCSSValuePrototypeConstantsFunctions[0].name = "CSS_INHERIT";
    JSCSSValuePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValuePrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSCSSValuePrototypeConstantsFunctions[0].u.i32 = (int32_t)CSSValue::CSS_INHERIT;
    JSCSSValuePrototypeConstantsFunctions[1].name = "CSS_PRIMITIVE_VALUE";
    JSCSSValuePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValuePrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSCSSValuePrototypeConstantsFunctions[1].u.i32 = (int32_t)CSSValue::CSS_PRIMITIVE_VALUE;
    JSCSSValuePrototypeConstantsFunctions[2].name = "CSS_VALUE_LIST";
    JSCSSValuePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValuePrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSCSSValuePrototypeConstantsFunctions[2].u.i32 = (int32_t)CSSValue::CSS_VALUE_LIST;
    JSCSSValuePrototypeConstantsFunctions[3].name = "CSS_CUSTOM";
    JSCSSValuePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSValuePrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSCSSValuePrototypeConstantsFunctions[3].u.i32 = (int32_t)CSSValue::CSS_CUSTOM;
}

JSValue JSCSSValuePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSValue.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSValuePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSValue.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSValuePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSValueAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueAttributesFunctions, countof(JSCSSValueAttributesFunctions));
    init_JSCSSValuePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValuePrototypeConstantsFunctions, countof(JSCSSValuePrototypeConstantsFunctions));
}

static JSClassDef JSCSSValueClassDefine;
static bool JSCSSValueClassDefine_initialized = false;

static void init_JSCSSValueClassDefine()
{
    if (JSCSSValueClassDefine_initialized) return;
    JSCSSValueClassDefine_initialized = true;
    memset(&JSCSSValueClassDefine, 0, sizeof(JSCSSValueClassDefine));
    JSCSSValueClassDefine.class_name = "CSSValue";
    JSCSSValueClassDefine.finalizer = JSCSSValue::finalizer;
    JSCSSValueClassDefine.gc_mark = JSCSSValue::mark;
}

JSClassID JSCSSValue::js_class_id = 0;

void JSCSSValue::init(JSContext* ctx)
{
    if (JSCSSValue::js_class_id == 0) {
        init_JSCSSValueClassDefine();
        JS_NewClassID(&JSCSSValue::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSValue::js_class_id, &JSCSSValueClassDefine);
        JS_SetConstructor(ctx, JSCSSValueConstructor::self(ctx), JSCSSValuePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSValue::js_class_id, JSCSSValuePrototype::self(ctx));
    }
}

JSValue JSCSSValue::create(JSContext* ctx, CSSValue* impl)
{
    JSCSSValue::init(ctx);
    JSValue _proto = JSCSSValuePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSValue::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            CSSValue* imp = (CSSValue*)JS_GetOpaque(this_val, JSCSSValue::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case CssValueTypeAttrNum: {
            CSSValue* imp = (CSSValue*)JS_GetOpaque(this_val, JSCSSValue::js_class_id);
            return JS_NewInt32(ctx, imp->cssValueType());
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
            CSSValue* imp = (CSSValue*)JS_GetOpaque(this_val, JSCSSValue::js_class_id);
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
