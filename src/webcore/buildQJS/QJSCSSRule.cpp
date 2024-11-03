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

#include "QJSCSSRule.h"

#include "CSSRule.h"
#include "CSSStyleSheet.h"
#include "QJSCSSRule.h"
#include "QJSCSSStyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCSSRuleAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cssText", JSCSSRule::getValueProperty, JSCSSRule::putValueProperty, JSCSSRule::CssTextAttrNum),
    JS_CGETSET_MAGIC_DEF("parentStyleSheet", JSCSSRule::getValueProperty, NULL, JSCSSRule::ParentStyleSheetAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSCSSRule::getValueProperty, NULL, JSCSSRule::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("parentRule", JSCSSRule::getValueProperty, NULL, JSCSSRule::ParentRuleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCSSRule::getValueProperty, NULL, JSCSSRule::ConstructorAttrNum)
};

class JSCSSRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCSSRuleConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSCSSRuleConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MEDIA_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::MEDIA_RULE),
    JS_CGETSET_MAGIC_DEF("IMPORT_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::IMPORT_RULE),
    JS_CGETSET_MAGIC_DEF("UNKNOWN_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::UNKNOWN_RULE),
    JS_CGETSET_MAGIC_DEF("STYLE_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::STYLE_RULE),
    JS_CGETSET_MAGIC_DEF("PAGE_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::PAGE_RULE),
    JS_CGETSET_MAGIC_DEF("CHARSET_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::CHARSET_RULE),
    JS_CGETSET_MAGIC_DEF("FONT_FACE_RULE", JSCSSRuleConstructor::getValueProperty, NULL, CSSRule::FONT_FACE_RULE)
};

JSValue JSCSSRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSRule.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleConstructorFunctions, countof(JSCSSRuleConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSCSSRulePrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("MEDIA_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::MEDIA_RULE),
    JS_CGETSET_MAGIC_DEF("IMPORT_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::IMPORT_RULE),
    JS_CGETSET_MAGIC_DEF("UNKNOWN_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::UNKNOWN_RULE),
    JS_CGETSET_MAGIC_DEF("STYLE_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::STYLE_RULE),
    JS_CGETSET_MAGIC_DEF("PAGE_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::PAGE_RULE),
    JS_CGETSET_MAGIC_DEF("CHARSET_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::CHARSET_RULE),
    JS_CGETSET_MAGIC_DEF("FONT_FACE_RULE", JSCSSRulePrototype::getValueProperty, NULL, CSSRule::FONT_FACE_RULE)
};

JSValue JSCSSRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSRule.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCSSRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleAttributesFunctions, countof(JSCSSRuleAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRulePrototypeConstantsFunctions, countof(JSCSSRulePrototypeConstantsFunctions));
}

JSValue JSCSSRulePrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSCSSRuleClassDefine = 
{
    "CSSRule",
    .finalizer = JSCSSRule::finalizer,
    .gc_mark = JSCSSRule::mark,
};

JSClassID JSCSSRule::js_class_id = 0;

void JSCSSRule::init(JSContext* ctx)
{
    if (JSCSSRule::js_class_id == 0) {
        JS_NewClassID(&JSCSSRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSRule::js_class_id, &JSCSSRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSRuleConstructor::self(ctx), JSCSSRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSRule::js_class_id, JSCSSRulePrototype::self(ctx));
    }
}

JSValue JSCSSRule::create(JSContext* ctx, CSSRule* impl)
{
    JSCSSRule::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCSSRulePrototype::self(ctx), JSCSSRule::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCSSRule::finalizer(JSRuntime* rt, JSValue val)
{
    CSSRule* impl = (CSSRule*)JS_GetOpaque(val, JSCSSRule::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCSSRule::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCSSRule::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque2(ctx, this_val, JSCSSRule::js_class_id);
            return JS_NewBigUint64(ctx, imp->type());
        }
        case CssTextAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque2(ctx, this_val, JSCSSRule::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case ParentStyleSheetAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque2(ctx, this_val, JSCSSRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentStyleSheet()));
        }
        case ParentRuleAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque2(ctx, this_val, JSCSSRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentRule()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCSSRule::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CssTextAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque2(ctx, this_val, JSCSSRule::js_class_id);
            ExceptionCode ec = 0;
            imp->setCssText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCSSRule::getConstructor(JSContext *ctx)
{
    return JSCSSRuleConstructor::self(ctx);
}


CSSRule* toCSSRule(JSValue val)
{
    if (JS_IsObject(val)) {
        CSSRule* impl = (CSSRule*)JS_GetOpaque(val, JSCSSRule::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
