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

#include "QJSCSSValueList.h"

#include "CSSValue.h"
#include "CSSValueList.h"
#include "ExceptionCode.h"
#include "QJSCSSValue.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSValueListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSCSSValueList::getValueProperty, NULL, JSCSSValueList::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSValueList::getValueProperty, NULL, JSCSSValueList::ConstructorAttrNum)
};

class JSCSSValueListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSValueListConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSValueListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSValueList.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSValueListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSValueList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSValueListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSValueListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("item", 1, JSCSSValueListPrototypeFunction::callAsFunction, JSCSSValueList::ItemFuncNum)
};

JSValue JSCSSValueListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSValueList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSValuePrototype::self(ctx));
        JSCSSValueListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSValueList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSValueListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueListAttributesFunctions, countof(JSCSSValueListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSValueListPrototypeFunctions, countof(JSCSSValueListPrototypeFunctions));
}

static JSClassDef JSCSSValueListClassDefine = 
{
    "CSSValueList",
    .finalizer = JSCSSValueList::finalizer,
    .gc_mark = JSCSSValueList::mark,
};

JSClassID JSCSSValueList::js_class_id = 0;

void JSCSSValueList::init(JSContext* ctx)
{
    if (JSCSSValueList::js_class_id == 0) {
        JS_NewClassID(&JSCSSValueList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSValueList::js_class_id, &JSCSSValueListClassDefine);
        JS_SetConstructor(ctx, JSCSSValueListConstructor::self(ctx), JSCSSValueListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSValueList::js_class_id, JSCSSValueListPrototype::self(ctx));
    }
}

JSValue JSCSSValueList::create(JSContext* ctx, CSSValueList* impl)
{
    JSCSSValueList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSValueListPrototype::self(ctx), JSCSSValueList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSValueList::finalizer(JSRuntime* rt, JSValue val)
{
    CSSValueList* impl = (CSSValueList*)JS_GetOpaque(val, JSCSSValueList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSValueList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSValue::mark(rt, val, mark_func);
}

JSValue JSCSSValueList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            CSSValueList* imp = (CSSValueList*)JS_GetOpaque2(ctx, this_val, JSCSSValueList::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSValueList::getConstructor(JSContext *ctx)
{
    return JSCSSValueListConstructor::self(ctx);
}

JSValue JSCSSValueListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSValueList* imp = (CSSValueList*)JS_GetOpaque2(ctx, this_val, JSCSSValueList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSValueList::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->item(index)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSCSSValueList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    CSSValueList* imp = (CSSValueList*)JS_GetOpaque2(ctx, this_obj, JSCSSValueList::js_class_id);
    return toJS(ctx, imp->item(idx));
}


}
