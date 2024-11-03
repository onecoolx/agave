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

#include "QJSCSSStyleRule.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSStyleRule.h"
#include "QJSCSSStyleDeclaration.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSStyleRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("selectorText", JSCSSStyleRule::getValueProperty, JSCSSStyleRule::putValueProperty, JSCSSStyleRule::SelectorTextAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSCSSStyleRule::getValueProperty, NULL, JSCSSStyleRule::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSStyleRule::getValueProperty, NULL, JSCSSStyleRule::ConstructorAttrNum)
};

class JSCSSStyleRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSStyleRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSStyleRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSStyleRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSStyleRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSStyleRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSStyleRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSStyleRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSStyleRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSStyleRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSStyleRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSStyleRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleRuleAttributesFunctions, countof(JSCSSStyleRuleAttributesFunctions));
}

static JSClassDef JSCSSStyleRuleClassDefine = 
{
    "CSSStyleRule",
    .finalizer = JSCSSStyleRule::finalizer,
    .gc_mark = JSCSSStyleRule::mark,
};

JSClassID JSCSSStyleRule::js_class_id = 0;

void JSCSSStyleRule::init(JSContext* ctx)
{
    if (JSCSSStyleRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSStyleRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSStyleRule::js_class_id, &JSCSSStyleRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSStyleRuleConstructor::self(ctx), JSCSSStyleRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSStyleRule::js_class_id, JSCSSStyleRulePrototype::self(ctx));
    }
}

JSValue JSCSSStyleRule::create(JSContext* ctx, CSSStyleRule* impl)
{
    JSCSSStyleRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSStyleRulePrototype::self(ctx), JSCSSStyleRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSStyleRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSStyleRule* impl = (CSSStyleRule*)JS_GetOpaque(val, JSCSSStyleRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSStyleRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSStyleRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case SelectorTextAttrNum: {
            CSSStyleRule* imp = (CSSStyleRule*)JS_GetOpaque2(ctx, this_val, JSCSSStyleRule::js_class_id);
            return jsStringOrNull(ctx, imp->selectorText());
        }
        case StyleAttrNum: {
            CSSStyleRule* imp = (CSSStyleRule*)JS_GetOpaque2(ctx, this_val, JSCSSStyleRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSStyleRule::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case SelectorTextAttrNum: {
            CSSStyleRule* imp = (CSSStyleRule*)JS_GetOpaque2(ctx, this_val, JSCSSStyleRule::js_class_id);
            ExceptionCode ec = 0;
            imp->setSelectorText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSStyleRule::getConstructor(JSContext *ctx)
{
    return JSCSSStyleRuleConstructor::self(ctx);
}


}
