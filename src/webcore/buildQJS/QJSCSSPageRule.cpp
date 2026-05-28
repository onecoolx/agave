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

#include "QJSCSSPageRule.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSPageRule.h"
#include "CSSStyleDeclaration.h"
#include "QJSCSSStyleDeclaration.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSPageRuleAttributesFunctions[3];
static bool JSCSSPageRuleAttributesFunctions_initialized = false;

static void init_JSCSSPageRuleAttributesFunctions()
{
    if (JSCSSPageRuleAttributesFunctions_initialized) return;
    JSCSSPageRuleAttributesFunctions_initialized = true;
    memset(JSCSSPageRuleAttributesFunctions, 0, sizeof(JSCSSPageRuleAttributesFunctions));
    JSCSSPageRuleAttributesFunctions[0].name = "selectorText";
    JSCSSPageRuleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPageRuleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPageRuleAttributesFunctions[0].magic = JSCSSPageRule::SelectorTextAttrNum;
    JSCSSPageRuleAttributesFunctions[0].u.getset.get.getter_magic = JSCSSPageRule::getValueProperty;
    JSCSSPageRuleAttributesFunctions[0].u.getset.set.setter_magic = JSCSSPageRule::putValueProperty;
    JSCSSPageRuleAttributesFunctions[1].name = "style";
    JSCSSPageRuleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPageRuleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPageRuleAttributesFunctions[1].magic = JSCSSPageRule::StyleAttrNum;
    JSCSSPageRuleAttributesFunctions[1].u.getset.get.getter_magic = JSCSSPageRule::getValueProperty;
    JSCSSPageRuleAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSPageRuleAttributesFunctions[2].name = "constructor";
    JSCSSPageRuleAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSPageRuleAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSPageRuleAttributesFunctions[2].magic = JSCSSPageRule::ConstructorAttrNum;
    JSCSSPageRuleAttributesFunctions[2].u.getset.get.getter_magic = JSCSSPageRule::getValueProperty;
    JSCSSPageRuleAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSCSSPageRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSPageRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCSSPageRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSPageRule.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSPageRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSPageRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSPageRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCSSPageRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSPageRule.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSRulePrototype::self(ctx));
        JSCSSPageRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSPageRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSPageRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSPageRuleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSPageRuleAttributesFunctions, countof(JSCSSPageRuleAttributesFunctions));
}

static JSClassDef JSCSSPageRuleClassDefine;
static bool JSCSSPageRuleClassDefine_initialized = false;

static void init_JSCSSPageRuleClassDefine()
{
    if (JSCSSPageRuleClassDefine_initialized) return;
    JSCSSPageRuleClassDefine_initialized = true;
    memset(&JSCSSPageRuleClassDefine, 0, sizeof(JSCSSPageRuleClassDefine));
    JSCSSPageRuleClassDefine.class_name = "CSSPageRule";
    JSCSSPageRuleClassDefine.finalizer = JSCSSPageRule::finalizer;
    JSCSSPageRuleClassDefine.gc_mark = JSCSSPageRule::mark;
}

JSClassID JSCSSPageRule::js_class_id = 0;

void JSCSSPageRule::init(JSContext* ctx)
{
    if (JSCSSPageRule::js_class_id == 0) {
        init_JSCSSPageRuleClassDefine();
        JS_NewClassID(&JSCSSPageRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSPageRule::js_class_id, &JSCSSPageRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSPageRuleConstructor::self(ctx), JSCSSPageRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSPageRule::js_class_id, JSCSSPageRulePrototype::self(ctx));
    }
}

JSValue JSCSSPageRule::create(JSContext* ctx, CSSPageRule* impl)
{
    JSCSSPageRule::init(ctx);
    JSValue _proto = JSCSSPageRulePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSPageRule::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSPageRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSPageRule* impl = (CSSPageRule*)JS_GetOpaque(val, JSCSSPageRule::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSPageRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSRule::mark(rt, val, mark_func);
}

JSValue JSCSSPageRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case SelectorTextAttrNum: {
            CSSPageRule* imp = (CSSPageRule*)JS_GetOpaque(this_val, JSCSSPageRule::js_class_id);
            return jsStringOrNull(ctx, imp->selectorText());
        }
        case StyleAttrNum: {
            CSSPageRule* imp = (CSSPageRule*)JS_GetOpaque(this_val, JSCSSPageRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSPageRule::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case SelectorTextAttrNum: {
            CSSPageRule* imp = (CSSPageRule*)JS_GetOpaque(this_val, JSCSSPageRule::js_class_id);
            ExceptionCode ec = 0;
            imp->setSelectorText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSPageRule::getConstructor(JSContext *ctx)
{
    return JSCSSPageRuleConstructor::self(ctx);
}


}
