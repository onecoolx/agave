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

static const JSCFunctionListEntry JSCSSStyleSheetAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cssRules", JSCSSStyleSheet::getValueProperty, NULL, JSCSSStyleSheet::CssRulesAttrNum),
    JS_CGETSET_MAGIC_DEF("ownerRule", JSCSSStyleSheet::getValueProperty, NULL, JSCSSStyleSheet::OwnerRuleAttrNum),
    JS_CGETSET_MAGIC_DEF("rules", JSCSSStyleSheet::getValueProperty, NULL, JSCSSStyleSheet::RulesAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSCSSStyleSheetPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("deleteRule", 1, JSCSSStyleSheetPrototypeFunction::callAsFunction, JSCSSStyleSheet::DeleteRuleFuncNum),
    JS_CFUNC_MAGIC_DEF("insertRule", 2, JSCSSStyleSheetPrototypeFunction::callAsFunction, JSCSSStyleSheet::InsertRuleFuncNum),
    JS_CFUNC_MAGIC_DEF("removeRule", 1, JSCSSStyleSheetPrototypeFunction::callAsFunction, JSCSSStyleSheet::RemoveRuleFuncNum),
    JS_CFUNC_MAGIC_DEF("addRule", 3, JSCSSStyleSheetPrototypeFunction::callAsFunction, JSCSSStyleSheet::AddRuleFuncNum)
};

JSValue JSCSSStyleSheetPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSStyleSheet.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSStyleSheetPrototype::self(ctx));
        JSCSSStyleSheetPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSStyleSheet.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSStyleSheetPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleSheetAttributesFunctions, countof(JSCSSStyleSheetAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSStyleSheetPrototypeFunctions, countof(JSCSSStyleSheetPrototypeFunctions));
}

static JSClassDef JSCSSStyleSheetClassDefine = 
{
    "CSSStyleSheet",
    .finalizer = JSCSSStyleSheet::finalizer,
    .gc_mark = JSCSSStyleSheet::mark,
};

JSClassID JSCSSStyleSheet::js_class_id = 0;

void JSCSSStyleSheet::init(JSContext* ctx)
{
    if (JSCSSStyleSheet::js_class_id == 0) {
        JS_NewClassID(&JSCSSStyleSheet::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSStyleSheet::js_class_id, &JSCSSStyleSheetClassDefine);
        JS_SetClassProto(ctx, JSCSSStyleSheet::js_class_id, JSCSSStyleSheetPrototype::self(ctx));
    }
}

JSValue JSCSSStyleSheet::create(JSContext* ctx, CSSStyleSheet* impl)
{
    JSCSSStyleSheet::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSStyleSheetPrototype::self(ctx), JSCSSStyleSheet::js_class_id);
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
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque2(ctx, this_val, JSCSSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerRule()));
        }
        case CssRulesAttrNum: {
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque2(ctx, this_val, JSCSSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->cssRules()));
        }
        case RulesAttrNum: {
            CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque2(ctx, this_val, JSCSSStyleSheet::js_class_id);
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
    CSSStyleSheet* imp = (CSSStyleSheet*)JS_GetOpaque2(ctx, this_val, JSCSSStyleSheet::js_class_id);
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

            JSValue result = JS_NewBigUint64(ctx, imp->insertRule(rule, index, ec));
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

                  JSValue result = JS_NewBigUint64(ctx, imp->addRule(selector, style, ec));
                  setDOMException(ctx, ec);
                  return result;
            }

            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[2], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewBigUint64(ctx, imp->addRule(selector, style, index, ec));
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
