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

#include "QJSCSSStyleSheet.h"

#include "CSSRule.h"
#include "CSSRuleList.h"
#include "CSSStyleSheet.h"
#include "ExceptionCode.h"
#include "QJSCSSRule.h"
#include "QJSCSSRuleList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCSSStyleSheetAttributesFunctions[3];
static bool JSCSSStyleSheetAttributesFunctions_initialized = false;

static void init_JSCSSStyleSheetAttributesFunctions()
{
    if (JSCSSStyleSheetAttributesFunctions_initialized) return;
    JSCSSStyleSheetAttributesFunctions_initialized = true;
    memset(JSCSSStyleSheetAttributesFunctions, 0, sizeof(JSCSSStyleSheetAttributesFunctions));
    JSCSSStyleSheetAttributesFunctions[0].name = "ownerRule";
    JSCSSStyleSheetAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleSheetAttributesFunctions[0].magic = JSCSSStyleSheet::OwnerRuleAttrNum;
    JSCSSStyleSheetAttributesFunctions[0].u.getset.get.getter_magic = JSCSSStyleSheet::getValueProperty;
    JSCSSStyleSheetAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSStyleSheetAttributesFunctions[1].name = "cssRules";
    JSCSSStyleSheetAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleSheetAttributesFunctions[1].magic = JSCSSStyleSheet::CssRulesAttrNum;
    JSCSSStyleSheetAttributesFunctions[1].u.getset.get.getter_magic = JSCSSStyleSheet::getValueProperty;
    JSCSSStyleSheetAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCSSStyleSheetAttributesFunctions[2].name = "rules";
    JSCSSStyleSheetAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSStyleSheetAttributesFunctions[2].magic = JSCSSStyleSheet::RulesAttrNum;
    JSCSSStyleSheetAttributesFunctions[2].u.getset.get.getter_magic = JSCSSStyleSheet::getValueProperty;
    JSCSSStyleSheetAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSCSSStyleSheetPrototypeFunctions[4];
static bool JSCSSStyleSheetPrototypeFunctions_initialized = false;

static void init_JSCSSStyleSheetPrototypeFunctions()
{
    if (JSCSSStyleSheetPrototypeFunctions_initialized) return;
    JSCSSStyleSheetPrototypeFunctions_initialized = true;
    memset(JSCSSStyleSheetPrototypeFunctions, 0, sizeof(JSCSSStyleSheetPrototypeFunctions));
    JSCSSStyleSheetPrototypeFunctions[0].name = "insertRule";
    JSCSSStyleSheetPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCSSStyleSheetPrototypeFunctions[0].magic = JSCSSStyleSheet::InsertRuleFuncNum;
    JSCSSStyleSheetPrototypeFunctions[0].u.func.length = 2;
    JSCSSStyleSheetPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleSheetPrototypeFunctions[0].u.func.cfunc.generic_magic = JSCSSStyleSheetPrototypeFunction::callAsFunction;
    JSCSSStyleSheetPrototypeFunctions[1].name = "deleteRule";
    JSCSSStyleSheetPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCSSStyleSheetPrototypeFunctions[1].magic = JSCSSStyleSheet::DeleteRuleFuncNum;
    JSCSSStyleSheetPrototypeFunctions[1].u.func.length = 1;
    JSCSSStyleSheetPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleSheetPrototypeFunctions[1].u.func.cfunc.generic_magic = JSCSSStyleSheetPrototypeFunction::callAsFunction;
    JSCSSStyleSheetPrototypeFunctions[2].name = "addRule";
    JSCSSStyleSheetPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSCSSStyleSheetPrototypeFunctions[2].magic = JSCSSStyleSheet::AddRuleFuncNum;
    JSCSSStyleSheetPrototypeFunctions[2].u.func.length = 3;
    JSCSSStyleSheetPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleSheetPrototypeFunctions[2].u.func.cfunc.generic_magic = JSCSSStyleSheetPrototypeFunction::callAsFunction;
    JSCSSStyleSheetPrototypeFunctions[3].name = "removeRule";
    JSCSSStyleSheetPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCSSStyleSheetPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSCSSStyleSheetPrototypeFunctions[3].magic = JSCSSStyleSheet::RemoveRuleFuncNum;
    JSCSSStyleSheetPrototypeFunctions[3].u.func.length = 1;
    JSCSSStyleSheetPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSCSSStyleSheetPrototypeFunctions[3].u.func.cfunc.generic_magic = JSCSSStyleSheetPrototypeFunction::callAsFunction;
}

JSValue JSCSSStyleSheetPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSStyleSheet.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSStyleSheetPrototype::self(ctx));
        JSCSSStyleSheetPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSStyleSheet.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSStyleSheetPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSStyleSheetAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleSheetAttributesFunctions, countof(JSCSSStyleSheetAttributesFunctions));
    init_JSCSSStyleSheetPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleSheetPrototypeFunctions, countof(JSCSSStyleSheetPrototypeFunctions));
}

static JSClassDef JSCSSStyleSheetClassDefine;
static bool JSCSSStyleSheetClassDefine_initialized = false;

static void init_JSCSSStyleSheetClassDefine()
{
    if (JSCSSStyleSheetClassDefine_initialized) return;
    JSCSSStyleSheetClassDefine_initialized = true;
    memset(&JSCSSStyleSheetClassDefine, 0, sizeof(JSCSSStyleSheetClassDefine));
    JSCSSStyleSheetClassDefine.class_name = "CSSStyleSheet";
    JSCSSStyleSheetClassDefine.finalizer = JSCSSStyleSheet::finalizer;
    JSCSSStyleSheetClassDefine.gc_mark = JSCSSStyleSheet::mark;
}

JSClassID JSCSSStyleSheet::js_class_id = 0;

void JSCSSStyleSheet::init(JSContext* ctx)
{
    if (JSCSSStyleSheet::js_class_id == 0) {
        init_JSCSSStyleSheetClassDefine();
        JS_NewClassID(&JSCSSStyleSheet::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSStyleSheet::js_class_id, &JSCSSStyleSheetClassDefine);
        JS_SetClassProto(ctx, JSCSSStyleSheet::js_class_id, JSCSSStyleSheetPrototype::self(ctx));
    }
}

JSValue JSCSSStyleSheet::create(JSContext* ctx, CSSStyleSheet* impl)
{
    JSCSSStyleSheet::init(ctx);
    JSValue _proto = JSCSSStyleSheetPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSStyleSheet::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSStyleSheet::finalizer(JSRuntime* rt, JSValue val)
{
    CSSStyleSheet* impl = (CSSStyleSheet*)JS_GetOpaque(val, JSCSSStyleSheet::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSStyleSheet::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSStyleSheet::mark(rt, val, mark_func);
}

JSValue JSCSSStyleSheet::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case OwnerRuleAttrNum: {
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque(this_val, JSCSSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerRule()));
        }
        case CssRulesAttrNum: {
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque(this_val, JSCSSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->cssRules()));
        }
        case RulesAttrNum: {
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque(this_val, JSCSSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->rules()));
        }
    }
    return JS_NULL;
}

JSValue JSCSSStyleSheet::getConstructor(JSContext *ctx)
{
    return JSStyleSheet::getConstructor(ctx);
}

JSValue JSCSSStyleSheetPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque(this_val, JSCSSStyleSheet::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCSSStyleSheet::InsertRuleFuncNum: {
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
        case JSCSSStyleSheet::DeleteRuleFuncNum: {
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
        case JSCSSStyleSheet::AddRuleFuncNum: {
            ExceptionCode ec = 0;
            String selector = valueToString(ctx, argv[0]);
            String style = valueToString(ctx, argv[1]);
            if (argc < 3) {

                  JSValue result = JS_NewInt32(ctx, imp->addRule(selector, style, ec));
                  setDOMException(ctx, ec);
                  return result;
            }

            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[2], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewInt32(ctx, imp->addRule(selector, style, index, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSCSSStyleSheet::RemoveRuleFuncNum: {
            ExceptionCode ec = 0;
            if (argc < 1) {
                  imp->removeRule(ec);
                  setDOMException(ctx, ec);
                  return JS_UNDEFINED;
            }

            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->removeRule(index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
