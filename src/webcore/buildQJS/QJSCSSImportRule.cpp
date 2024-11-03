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

#include "QJSCSSImportRule.h"

#include "CSSImportRule.h"
#include "CSSStyleSheet.h"
#include "MediaList.h"
#include "QJSCSSStyleSheet.h"
#include "QJSMediaList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSImportRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("href", JSCSSImportRule::getValueProperty, NULL, JSCSSImportRule::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("media", JSCSSImportRule::getValueProperty, NULL, JSCSSImportRule::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("styleSheet", JSCSSImportRule::getValueProperty, NULL, JSCSSImportRule::StyleSheetAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSImportRule::getValueProperty, NULL, JSCSSImportRule::ConstructorAttrNum)
};

class JSCSSImportRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSImportRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSImportRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSImportRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSImportRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSImportRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSImportRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSImportRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSImportRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSImportRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSImportRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSImportRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSImportRuleAttributesFunctions, countof(JSCSSImportRuleAttributesFunctions));
}

static JSClassDef JSCSSImportRuleClassDefine = 
{
    "CSSImportRule",
    .finalizer = JSCSSImportRule::finalizer,
    .gc_mark = JSCSSImportRule::mark,
};

JSClassID JSCSSImportRule::js_class_id = 0;

void JSCSSImportRule::init(JSContext* ctx)
{
    if (JSCSSImportRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSImportRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSImportRule::js_class_id, &JSCSSImportRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSImportRuleConstructor::self(ctx), JSCSSImportRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSImportRule::js_class_id, JSCSSImportRulePrototype::self(ctx));
    }
}

JSValue JSCSSImportRule::create(JSContext* ctx, CSSImportRule* impl)
{
    JSCSSImportRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSImportRulePrototype::self(ctx), JSCSSImportRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSImportRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSImportRule* impl = (CSSImportRule*)JS_GetOpaque(val, JSCSSImportRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSImportRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSImportRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            CSSImportRule* imp = (CSSImportRule*)JS_GetOpaque2(ctx, this_val, JSCSSImportRule::js_class_id);
            return jsStringOrNull(ctx, imp->href());
        }
        case MediaAttrNum: {
            CSSImportRule* imp = (CSSImportRule*)JS_GetOpaque2(ctx, this_val, JSCSSImportRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->media()));
        }
        case StyleSheetAttrNum: {
            CSSImportRule* imp = (CSSImportRule*)JS_GetOpaque2(ctx, this_val, JSCSSImportRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->styleSheet()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSImportRule::getConstructor(JSContext *ctx)
{
    return JSCSSImportRuleConstructor::self(ctx);
}


}
