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

static JSCFunctionListEntry JSCSSMediaRuleAttributesFunctions[3];
static bool JSCSSMediaRuleAttributesFunctions_initialized = false;

static void init_JSCSSMediaRuleAttributesFunctions()
{
    if (JSCSSMediaRuleAttributesFunctions_initialized) return;
    JSCSSMediaRuleAttributesFunctions_initialized = true;
    memset(JSCSSMediaRuleAttributesFunctions, 0, sizeof(JSCSSMediaRuleAttributesFunctions));
    JSCSSMediaRuleAttributesFunctions[0].name = "media";
    JSCSSMediaRuleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSMediaRuleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSMediaRuleAttributesFunctions[0].magic = JSCSSMediaRule::MediaAttrNum;
    JSCSSMediaRuleAttributesFunctions[0].u.getset.get.getter_magic = JSCSSMediaRule::getValueProperty;
    JSCSSMediaRuleAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSMediaRuleAttributesFunctions[1].name = "cssRules";
    JSCSSMediaRuleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSMediaRuleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSMediaRuleAttributesFunctions[1].magic = JSCSSMediaRule::CssRulesAttrNum;
    JSCSSMediaRuleAttributesFunctions[1].u.getset.get.getter_magic = JSCSSMediaRule::getValueProperty;
    JSCSSMediaRuleAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSMediaRuleAttributesFunctions[2].name = "constructor";
    JSCSSMediaRuleAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSMediaRuleAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSMediaRuleAttributesFunctions[2].magic = JSCSSMediaRule::ConstructorAttrNum;
    JSCSSMediaRuleAttributesFunctions[2].u.getset.get.getter_magic = JSCSSMediaRule::getValueProperty;
    JSCSSMediaRuleAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSMediaRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSMediaRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSMediaRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSCSSMediaRulePrototypeFunctions[2];
static bool JSCSSMediaRulePrototypeFunctions_initialized = false;

static void init_JSCSSMediaRulePrototypeFunctions()
{
    if (JSCSSMediaRulePrototypeFunctions_initialized) return;
    JSCSSMediaRulePrototypeFunctions_initialized = true;
    memset(JSCSSMediaRulePrototypeFunctions, 0, sizeof(JSCSSMediaRulePrototypeFunctions));
    JSCSSMediaRulePrototypeFunctions[0].name = "insertRule";
    JSCSSMediaRulePrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSMediaRulePrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCSSMediaRulePrototypeFunctions[0].magic = JSCSSMediaRule::InsertRuleFuncNum;
    JSCSSMediaRulePrototypeFunctions[0].u.func.length = 2;
    JSCSSMediaRulePrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSMediaRulePrototypeFunctions[0].u.func.cfunc.generic_magic = JSCSSMediaRulePrototypeFunction::callAsFunction;
    JSCSSMediaRulePrototypeFunctions[1].name = "deleteRule";
    JSCSSMediaRulePrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSMediaRulePrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCSSMediaRulePrototypeFunctions[1].magic = JSCSSMediaRule::DeleteRuleFuncNum;
    JSCSSMediaRulePrototypeFunctions[1].u.func.length = 1;
    JSCSSMediaRulePrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSMediaRulePrototypeFunctions[1].u.func.cfunc.generic_magic = JSCSSMediaRulePrototypeFunction::callAsFunction;
}

JSValue JSCSSMediaRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSMediaRule.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSMediaRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSMediaRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSMediaRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSMediaRuleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSMediaRuleAttributesFunctions, countof(JSCSSMediaRuleAttributesFunctions));
    init_JSCSSMediaRulePrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSMediaRulePrototypeFunctions, countof(JSCSSMediaRulePrototypeFunctions));
}

static JSClassDef JSCSSMediaRuleClassDefine;
static bool JSCSSMediaRuleClassDefine_initialized = false;

static void init_JSCSSMediaRuleClassDefine()
{
    if (JSCSSMediaRuleClassDefine_initialized) return;
    JSCSSMediaRuleClassDefine_initialized = true;
    memset(&JSCSSMediaRuleClassDefine, 0, sizeof(JSCSSMediaRuleClassDefine));
    JSCSSMediaRuleClassDefine.class_name = "CSSMediaRule";
    JSCSSMediaRuleClassDefine.finalizer = JSCSSMediaRule::finalizer;
    JSCSSMediaRuleClassDefine.gc_mark = JSCSSMediaRule::mark;
}

JSClassID JSCSSMediaRule::js_class_id = 0;

void JSCSSMediaRule::init(JSContext* ctx)
{
    if (JSCSSMediaRule::js_class_id == 0) {
        init_JSCSSMediaRuleClassDefine();
        JS_NewClassID(&JSCSSMediaRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSMediaRule::js_class_id, &JSCSSMediaRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSMediaRuleConstructor::self(ctx), JSCSSMediaRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSMediaRule::js_class_id, JSCSSMediaRulePrototype::self(ctx));
    }
}

JSValue JSCSSMediaRule::create(JSContext* ctx, CSSMediaRule* impl)
{
    JSCSSMediaRule::init(ctx);
    JSValue _proto = JSCSSMediaRulePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSMediaRule::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque(this_val, JSCSSMediaRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->media()));
        }
        case CssRulesAttrNum: {
            CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque(this_val, JSCSSMediaRule::js_class_id);
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
    CSSMediaRule* imp = (CSSMediaRule*)JS_GetOpaque(this_val, JSCSSMediaRule::js_class_id);
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

            JSValue result = JS_NewInt32(ctx, imp->insertRule(rule, index, ec));
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
