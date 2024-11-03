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

#include "QJSCSSCharsetRule.h"

#include "CSSCharsetRule.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSCharsetRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("encoding", JSCSSCharsetRule::getValueProperty, JSCSSCharsetRule::putValueProperty, JSCSSCharsetRule::EncodingAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSCharsetRule::getValueProperty, NULL, JSCSSCharsetRule::ConstructorAttrNum)
};

class JSCSSCharsetRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSCharsetRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSCharsetRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSCharsetRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSCharsetRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSCharsetRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSCharsetRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSCharsetRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSCharsetRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSCharsetRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSCharsetRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSCharsetRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSCharsetRuleAttributesFunctions, countof(JSCSSCharsetRuleAttributesFunctions));
}

static JSClassDef JSCSSCharsetRuleClassDefine = 
{
    "CSSCharsetRule",
    .finalizer = JSCSSCharsetRule::finalizer,
    .gc_mark = JSCSSCharsetRule::mark,
};

JSClassID JSCSSCharsetRule::js_class_id = 0;

void JSCSSCharsetRule::init(JSContext* ctx)
{
    if (JSCSSCharsetRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSCharsetRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSCharsetRule::js_class_id, &JSCSSCharsetRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSCharsetRuleConstructor::self(ctx), JSCSSCharsetRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSCharsetRule::js_class_id, JSCSSCharsetRulePrototype::self(ctx));
    }
}

JSValue JSCSSCharsetRule::create(JSContext* ctx, CSSCharsetRule* impl)
{
    JSCSSCharsetRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSCharsetRulePrototype::self(ctx), JSCSSCharsetRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSCharsetRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSCharsetRule* impl = (CSSCharsetRule*)JS_GetOpaque(val, JSCSSCharsetRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSCharsetRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSCharsetRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case EncodingAttrNum: {
            CSSCharsetRule* imp = (CSSCharsetRule*)JS_GetOpaque2(ctx, this_val, JSCSSCharsetRule::js_class_id);
            return jsStringOrNull(ctx, imp->encoding());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSCharsetRule::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case EncodingAttrNum: {
            CSSCharsetRule* imp = (CSSCharsetRule*)JS_GetOpaque2(ctx, this_val, JSCSSCharsetRule::js_class_id);
            ExceptionCode ec = 0;
            imp->setEncoding(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSCharsetRule::getConstructor(JSContext *ctx)
{
    return JSCSSCharsetRuleConstructor::self(ctx);
}


}
