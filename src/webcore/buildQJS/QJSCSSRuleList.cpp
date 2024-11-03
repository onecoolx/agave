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

#include "QJSCSSRuleList.h"

#include "CSSRule.h"
#include "CSSRuleList.h"
#include "ExceptionCode.h"
#include "QJSCSSRule.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSRuleListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSCSSRuleList::getValueProperty, NULL, JSCSSRuleList::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSRuleList::getValueProperty, NULL, JSCSSRuleList::ConstructorAttrNum)
};

class JSCSSRuleListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSRuleListConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSRuleListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSRuleList.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRuleListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSRuleList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSRuleListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSRuleListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("item", 1, JSCSSRuleListPrototypeFunction::callAsFunction, JSCSSRuleList::ItemFuncNum)
};

JSValue JSCSSRuleListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSRuleList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRuleListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSRuleList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSRuleListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleListAttributesFunctions, countof(JSCSSRuleListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleListPrototypeFunctions, countof(JSCSSRuleListPrototypeFunctions));
}

static JSClassDef JSCSSRuleListClassDefine = 
{
    "CSSRuleList",
    .finalizer = JSCSSRuleList::finalizer,
    .gc_mark = JSCSSRuleList::mark,
};

JSClassID JSCSSRuleList::js_class_id = 0;

void JSCSSRuleList::init(JSContext* ctx)
{
    if (JSCSSRuleList::js_class_id == 0) {
        JS_NewClassID(&JSCSSRuleList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSRuleList::js_class_id, &JSCSSRuleListClassDefine);
        JS_SetConstructor(ctx, JSCSSRuleListConstructor::self(ctx), JSCSSRuleListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSRuleList::js_class_id, JSCSSRuleListPrototype::self(ctx));
    }
}

JSValue JSCSSRuleList::create(JSContext* ctx, CSSRuleList* impl)
{
    JSCSSRuleList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSRuleListPrototype::self(ctx), JSCSSRuleList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSRuleList::finalizer(JSRuntime* rt, JSValue val)
{
    CSSRuleList* impl = (CSSRuleList*)JS_GetOpaque(val, JSCSSRuleList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSRuleList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCSSRuleList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            CSSRuleList* imp = (CSSRuleList*)JS_GetOpaque2(ctx, this_val, JSCSSRuleList::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSRuleList::getConstructor(JSContext *ctx)
{
    return JSCSSRuleListConstructor::self(ctx);
}

JSValue JSCSSRuleListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSRuleList* imp = (CSSRuleList*)JS_GetOpaque2(ctx, this_val, JSCSSRuleList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSRuleList::ItemFuncNum: {
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

JSValue JSCSSRuleList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    CSSRuleList* imp = (CSSRuleList*)JS_GetOpaque2(ctx, this_obj, JSCSSRuleList::js_class_id);
    return toJS(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, CSSRuleList* obj)
{
    return QJS::cacheDOMObject<CSSRuleList, JSCSSRuleList>(ctx, obj);
}

CSSRuleList* toCSSRuleList(JSValue val)
{
    if (JS_IsObject(val)) {
        CSSRuleList* impl = (CSSRuleList*)JS_GetOpaque(val, JSCSSRuleList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
