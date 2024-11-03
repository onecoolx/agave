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

#include "QJSCSSMediaRule.h"

#include "CSSMediaRule.h"
#include "CSSRuleList.h"
#include "ExceptionCode.h"
#include "MediaList.h"
#include "QJSCSSRuleList.h"
#include "QJSMediaList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSMediaRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("media", JSCSSMediaRule::getValueProperty, NULL, JSCSSMediaRule::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("cssRules", JSCSSMediaRule::getValueProperty, NULL, JSCSSMediaRule::CssRulesAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSMediaRule::getValueProperty, NULL, JSCSSMediaRule::ConstructorAttrNum)
};

class JSCSSMediaRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSMediaRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSMediaRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSMediaRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSMediaRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSMediaRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSMediaRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSMediaRulePrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("insertRule", 2, JSCSSMediaRulePrototypeFunction::callAsFunction, JSCSSMediaRule::InsertRuleFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteRule", 1, JSCSSMediaRulePrototypeFunction::callAsFunction, JSCSSMediaRule::DeleteRuleFuncNum)
};

JSValue JSCSSMediaRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSMediaRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSMediaRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSMediaRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSMediaRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSMediaRuleAttributesFunctions, countof(JSCSSMediaRuleAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSMediaRulePrototypeFunctions, countof(JSCSSMediaRulePrototypeFunctions));
}

static JSClassDef JSCSSMediaRuleClassDefine = 
{
    "CSSMediaRule",
    .finalizer = JSCSSMediaRule::finalizer,
    .gc_mark = JSCSSMediaRule::mark,
};

JSClassID JSCSSMediaRule::js_class_id = 0;

void JSCSSMediaRule::init(JSContext* ctx)
{
    if (JSCSSMediaRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSMediaRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSMediaRule::js_class_id, &JSCSSMediaRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSMediaRuleConstructor::self(ctx), JSCSSMediaRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSMediaRule::js_class_id, JSCSSMediaRulePrototype::self(ctx));
    }
}

JSValue JSCSSMediaRule::create(JSContext* ctx, CSSMediaRule* impl)
{
    JSCSSMediaRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSMediaRulePrototype::self(ctx), JSCSSMediaRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSMediaRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSMediaRule* impl = (CSSMediaRule*)JS_GetOpaque(val, JSCSSMediaRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSMediaRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSMediaRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case MediaAttrNum: {
            CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque2(ctx, this_val, JSCSSMediaRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->media()));
        }
        case CssRulesAttrNum: {
            CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque2(ctx, this_val, JSCSSMediaRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->cssRules()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSMediaRule::getConstructor(JSContext *ctx)
{
    return JSCSSMediaRuleConstructor::self(ctx);
}

JSValue JSCSSMediaRulePrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque2(ctx, this_val, JSCSSMediaRule::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSMediaRule::InsertRuleFuncNum: {
            ExceptionCode ec = 0;
            String rule = valueToString(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBigUint64(ctx, imp->insertRule(rule, index, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSCSSMediaRule::DeleteRuleFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->deleteRule(index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
