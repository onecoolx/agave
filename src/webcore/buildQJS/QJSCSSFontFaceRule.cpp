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

#include "QJSCSSFontFaceRule.h"

#include "CSSFontFaceRule.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "QJSCSSStyleDeclaration.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSFontFaceRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("style", JSCSSFontFaceRule::getValueProperty, NULL, JSCSSFontFaceRule::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSFontFaceRule::getValueProperty, NULL, JSCSSFontFaceRule::ConstructorAttrNum)
};

class JSCSSFontFaceRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSFontFaceRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSFontFaceRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSFontFaceRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSFontFaceRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSFontFaceRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSFontFaceRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSFontFaceRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSFontFaceRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSFontFaceRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSFontFaceRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSFontFaceRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSFontFaceRuleAttributesFunctions, countof(JSCSSFontFaceRuleAttributesFunctions));
}

static JSClassDef JSCSSFontFaceRuleClassDefine = 
{
    "CSSFontFaceRule",
    .finalizer = JSCSSFontFaceRule::finalizer,
    .gc_mark = JSCSSFontFaceRule::mark,
};

JSClassID JSCSSFontFaceRule::js_class_id = 0;

void JSCSSFontFaceRule::init(JSContext* ctx)
{
    if (JSCSSFontFaceRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSFontFaceRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSFontFaceRule::js_class_id, &JSCSSFontFaceRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSFontFaceRuleConstructor::self(ctx), JSCSSFontFaceRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSFontFaceRule::js_class_id, JSCSSFontFaceRulePrototype::self(ctx));
    }
}

JSValue JSCSSFontFaceRule::create(JSContext* ctx, CSSFontFaceRule* impl)
{
    JSCSSFontFaceRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSFontFaceRulePrototype::self(ctx), JSCSSFontFaceRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSFontFaceRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSFontFaceRule* impl = (CSSFontFaceRule*)JS_GetOpaque(val, JSCSSFontFaceRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSFontFaceRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSFontFaceRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case StyleAttrNum: {
            CSSFontFaceRule* imp = (CSSFontFaceRule*)JS_GetOpaque2(ctx, this_val, JSCSSFontFaceRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSFontFaceRule::getConstructor(JSContext *ctx)
{
    return JSCSSFontFaceRuleConstructor::self(ctx);
}


}
