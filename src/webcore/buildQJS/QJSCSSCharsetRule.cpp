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

#include "QJSCSSCharsetRule.h"

#include "CSSCharsetRule.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSCharsetRuleAttributesFunctions[2];
static bool JSCSSCharsetRuleAttributesFunctions_initialized = false;

static void init_JSCSSCharsetRuleAttributesFunctions()
{
    if (JSCSSCharsetRuleAttributesFunctions_initialized) return;
    JSCSSCharsetRuleAttributesFunctions_initialized = true;
    memset(JSCSSCharsetRuleAttributesFunctions, 0, sizeof(JSCSSCharsetRuleAttributesFunctions));
    JSCSSCharsetRuleAttributesFunctions[0].name = "encoding";
    JSCSSCharsetRuleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSCharsetRuleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSCharsetRuleAttributesFunctions[0].magic = JSCSSCharsetRule::EncodingAttrNum;
    JSCSSCharsetRuleAttributesFunctions[0].u.getset.get.getter_magic = JSCSSCharsetRule::getValueProperty;
    JSCSSCharsetRuleAttributesFunctions[0].u.getset.set.setter_magic = JSCSSCharsetRule::putValueProperty;
    JSCSSCharsetRuleAttributesFunctions[1].name = "constructor";
    JSCSSCharsetRuleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSCharsetRuleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSCharsetRuleAttributesFunctions[1].magic = JSCSSCharsetRule::ConstructorAttrNum;
    JSCSSCharsetRuleAttributesFunctions[1].u.getset.get.getter_magic = JSCSSCharsetRule::getValueProperty;
    JSCSSCharsetRuleAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSCSSCharsetRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSCSSCharsetRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSCharsetRule.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSCharsetRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSCharsetRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSCharsetRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSCharsetRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSCharsetRule.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSCharsetRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSCharsetRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSCharsetRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSCharsetRuleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSCharsetRuleAttributesFunctions, countof(JSCSSCharsetRuleAttributesFunctions));
}

static JSClassDef JSCSSCharsetRuleClassDefine;
static bool JSCSSCharsetRuleClassDefine_initialized = false;

static void init_JSCSSCharsetRuleClassDefine()
{
    if (JSCSSCharsetRuleClassDefine_initialized) return;
    JSCSSCharsetRuleClassDefine_initialized = true;
    memset(&JSCSSCharsetRuleClassDefine, 0, sizeof(JSCSSCharsetRuleClassDefine));
    JSCSSCharsetRuleClassDefine.class_name = "CSSCharsetRule";
    JSCSSCharsetRuleClassDefine.finalizer = JSCSSCharsetRule::finalizer;
    JSCSSCharsetRuleClassDefine.gc_mark = JSCSSCharsetRule::mark;
}

JSClassID JSCSSCharsetRule::js_class_id = 0;

void JSCSSCharsetRule::init(JSContext* ctx)
{
    if (JSCSSCharsetRule::js_class_id == 0) {
        init_JSCSSCharsetRuleClassDefine();
        JS_NewClassID(&JSCSSCharsetRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSCharsetRule::js_class_id, &JSCSSCharsetRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSCharsetRuleConstructor::self(ctx), JSCSSCharsetRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSCharsetRule::js_class_id, JSCSSCharsetRulePrototype::self(ctx));
    }
}

JSValue JSCSSCharsetRule::create(JSContext* ctx, CSSCharsetRule* impl)
{
    JSCSSCharsetRule::init(ctx);
    JSValue _proto = JSCSSCharsetRulePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSCharsetRule::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            CSSCharsetRule* imp = (CSSCharsetRule*)JS_GetOpaque(this_val, JSCSSCharsetRule::js_class_id);
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
            CSSCharsetRule* imp = (CSSCharsetRule*)JS_GetOpaque(this_val, JSCSSCharsetRule::js_class_id);
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
