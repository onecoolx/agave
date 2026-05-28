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

static JSCFunctionListEntry JSCSSRuleAttributesFunctions[5];
static bool JSCSSRuleAttributesFunctions_initialized = false;

static void init_JSCSSRuleAttributesFunctions()
{
    if (JSCSSRuleAttributesFunctions_initialized) return;
    JSCSSRuleAttributesFunctions_initialized = true;
    memset(JSCSSRuleAttributesFunctions, 0, sizeof(JSCSSRuleAttributesFunctions));
    JSCSSRuleAttributesFunctions[0].name = "type";
    JSCSSRuleAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSRuleAttributesFunctions[0].magic = JSCSSRule::TypeAttrNum;
    JSCSSRuleAttributesFunctions[0].u.getset.get.getter_magic = JSCSSRule::getValueProperty;
    JSCSSRuleAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCSSRuleAttributesFunctions[1].name = "cssText";
    JSCSSRuleAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSRuleAttributesFunctions[1].magic = JSCSSRule::CssTextAttrNum;
    JSCSSRuleAttributesFunctions[1].u.getset.get.getter_magic = JSCSSRule::getValueProperty;
    JSCSSRuleAttributesFunctions[1].u.getset.set.setter_magic = JSCSSRule::putValueProperty;
    JSCSSRuleAttributesFunctions[2].name = "parentStyleSheet";
    JSCSSRuleAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSRuleAttributesFunctions[2].magic = JSCSSRule::ParentStyleSheetAttrNum;
    JSCSSRuleAttributesFunctions[2].u.getset.get.getter_magic = JSCSSRule::getValueProperty;
    JSCSSRuleAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSCSSRuleAttributesFunctions[3].name = "parentRule";
    JSCSSRuleAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSRuleAttributesFunctions[3].magic = JSCSSRule::ParentRuleAttrNum;
    JSCSSRuleAttributesFunctions[3].u.getset.get.getter_magic = JSCSSRule::getValueProperty;
    JSCSSRuleAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSCSSRuleAttributesFunctions[4].name = "constructor";
    JSCSSRuleAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSCSSRuleAttributesFunctions[4].magic = JSCSSRule::ConstructorAttrNum;
    JSCSSRuleAttributesFunctions[4].u.getset.get.getter_magic = JSCSSRule::getValueProperty;
    JSCSSRuleAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSCSSRuleConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSCSSRuleConstructorFunctions[7];
static bool JSCSSRuleConstructorFunctions_initialized = false;

static void init_JSCSSRuleConstructorFunctions()
{
    if (JSCSSRuleConstructorFunctions_initialized) return;
    JSCSSRuleConstructorFunctions_initialized = true;
    memset(JSCSSRuleConstructorFunctions, 0, sizeof(JSCSSRuleConstructorFunctions));
    JSCSSRuleConstructorFunctions[0].name = "UNKNOWN_RULE";
    JSCSSRuleConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[0].u.i32 = (int32_t)CSSRule::UNKNOWN_RULE;
    JSCSSRuleConstructorFunctions[1].name = "STYLE_RULE";
    JSCSSRuleConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[1].u.i32 = (int32_t)CSSRule::STYLE_RULE;
    JSCSSRuleConstructorFunctions[2].name = "CHARSET_RULE";
    JSCSSRuleConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[2].u.i32 = (int32_t)CSSRule::CHARSET_RULE;
    JSCSSRuleConstructorFunctions[3].name = "IMPORT_RULE";
    JSCSSRuleConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[3].u.i32 = (int32_t)CSSRule::IMPORT_RULE;
    JSCSSRuleConstructorFunctions[4].name = "MEDIA_RULE";
    JSCSSRuleConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[4].u.i32 = (int32_t)CSSRule::MEDIA_RULE;
    JSCSSRuleConstructorFunctions[5].name = "FONT_FACE_RULE";
    JSCSSRuleConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[5].u.i32 = (int32_t)CSSRule::FONT_FACE_RULE;
    JSCSSRuleConstructorFunctions[6].name = "PAGE_RULE";
    JSCSSRuleConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRuleConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSCSSRuleConstructorFunctions[6].u.i32 = (int32_t)CSSRule::PAGE_RULE;
}

JSValue JSCSSRuleConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CSSRule.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRuleConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CSSRule.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSRuleConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSRuleConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleConstructorFunctions, countof(JSCSSRuleConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSCSSRulePrototypeConstantsFunctions[7];
static bool JSCSSRulePrototypeConstantsFunctions_initialized = false;

static void init_JSCSSRulePrototypeConstantsFunctions()
{
    if (JSCSSRulePrototypeConstantsFunctions_initialized) return;
    JSCSSRulePrototypeConstantsFunctions_initialized = true;
    memset(JSCSSRulePrototypeConstantsFunctions, 0, sizeof(JSCSSRulePrototypeConstantsFunctions));
    JSCSSRulePrototypeConstantsFunctions[0].name = "UNKNOWN_RULE";
    JSCSSRulePrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[0].u.i32 = (int32_t)CSSRule::UNKNOWN_RULE;
    JSCSSRulePrototypeConstantsFunctions[1].name = "STYLE_RULE";
    JSCSSRulePrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[1].u.i32 = (int32_t)CSSRule::STYLE_RULE;
    JSCSSRulePrototypeConstantsFunctions[2].name = "CHARSET_RULE";
    JSCSSRulePrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[2].u.i32 = (int32_t)CSSRule::CHARSET_RULE;
    JSCSSRulePrototypeConstantsFunctions[3].name = "IMPORT_RULE";
    JSCSSRulePrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[3].u.i32 = (int32_t)CSSRule::IMPORT_RULE;
    JSCSSRulePrototypeConstantsFunctions[4].name = "MEDIA_RULE";
    JSCSSRulePrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[4].u.i32 = (int32_t)CSSRule::MEDIA_RULE;
    JSCSSRulePrototypeConstantsFunctions[5].name = "FONT_FACE_RULE";
    JSCSSRulePrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[5].u.i32 = (int32_t)CSSRule::FONT_FACE_RULE;
    JSCSSRulePrototypeConstantsFunctions[6].name = "PAGE_RULE";
    JSCSSRulePrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSCSSRulePrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSCSSRulePrototypeConstantsFunctions[6].u.i32 = (int32_t)CSSRule::PAGE_RULE;
}

JSValue JSCSSRulePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCSSRule.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCSSRulePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCSSRule.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCSSRulePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCSSRuleAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRuleAttributesFunctions, countof(JSCSSRuleAttributesFunctions));
    init_JSCSSRulePrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCSSRulePrototypeConstantsFunctions, countof(JSCSSRulePrototypeConstantsFunctions));
}

static JSClassDef JSCSSRuleClassDefine;
static bool JSCSSRuleClassDefine_initialized = false;

static void init_JSCSSRuleClassDefine()
{
    if (JSCSSRuleClassDefine_initialized) return;
    JSCSSRuleClassDefine_initialized = true;
    memset(&JSCSSRuleClassDefine, 0, sizeof(JSCSSRuleClassDefine));
    JSCSSRuleClassDefine.class_name = "CSSRule";
    JSCSSRuleClassDefine.finalizer = JSCSSRule::finalizer;
    JSCSSRuleClassDefine.gc_mark = JSCSSRule::mark;
}

JSClassID JSCSSRule::js_class_id = 0;

void JSCSSRule::init(JSContext* ctx)
{
    if (JSCSSRule::js_class_id == 0) {
        init_JSCSSRuleClassDefine();
        JS_NewClassID(&JSCSSRule::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCSSRule::js_class_id, &JSCSSRuleClassDefine);
        JS_SetConstructor(ctx, JSCSSRuleConstructor::self(ctx), JSCSSRulePrototype::self(ctx));
        JS_SetClassProto(ctx, JSCSSRule::js_class_id, JSCSSRulePrototype::self(ctx));
    }
}

JSValue JSCSSRule::create(JSContext* ctx, CSSRule* impl)
{
    JSCSSRule::init(ctx);
    JSValue _proto = JSCSSRulePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCSSRule::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            CSSRule* imp = (CSSRule*)JS_GetOpaque(this_val, JSCSSRule::js_class_id);
            return JS_NewInt32(ctx, imp->type());
        }
        case CssTextAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque(this_val, JSCSSRule::js_class_id);
            return jsStringOrNull(ctx, imp->cssText());
        }
        case ParentStyleSheetAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque(this_val, JSCSSRule::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentStyleSheet()));
        }
        case ParentRuleAttrNum: {
            CSSRule* imp = (CSSRule*)JS_GetOpaque(this_val, JSCSSRule::js_class_id);
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
            CSSRule* imp = (CSSRule*)JS_GetOpaque(this_val, JSCSSRule::js_class_id);
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
