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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGTextContentElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "FloatPoint.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGStringList.h"
#include "SVGTextContentElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGTextContentElementAttributesFunctions[11];
static bool JSSVGTextContentElementAttributesFunctions_initialized = false;

static void init_JSSVGTextContentElementAttributesFunctions()
{
    if (JSSVGTextContentElementAttributesFunctions_initialized) return;
    JSSVGTextContentElementAttributesFunctions_initialized = true;
    memset(JSSVGTextContentElementAttributesFunctions, 0, sizeof(JSSVGTextContentElementAttributesFunctions));
    JSSVGTextContentElementAttributesFunctions[0].name = "textLength";
    JSSVGTextContentElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[0].magic = JSSVGTextContentElement::TextLengthAttrNum;
    JSSVGTextContentElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[1].name = "lengthAdjust";
    JSSVGTextContentElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[1].magic = JSSVGTextContentElement::LengthAdjustAttrNum;
    JSSVGTextContentElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[2].name = "requiredFeatures";
    JSSVGTextContentElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[2].magic = JSSVGTextContentElement::RequiredFeaturesAttrNum;
    JSSVGTextContentElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[3].name = "requiredExtensions";
    JSSVGTextContentElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[3].magic = JSSVGTextContentElement::RequiredExtensionsAttrNum;
    JSSVGTextContentElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[4].name = "systemLanguage";
    JSSVGTextContentElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[4].magic = JSSVGTextContentElement::SystemLanguageAttrNum;
    JSSVGTextContentElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[5].name = "xmllang";
    JSSVGTextContentElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[5].magic = JSSVGTextContentElement::XmllangAttrNum;
    JSSVGTextContentElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[5].u.getset.set.setter_magic = JSSVGTextContentElement::putValueProperty;
    JSSVGTextContentElementAttributesFunctions[6].name = "xmlspace";
    JSSVGTextContentElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[6].magic = JSSVGTextContentElement::XmlspaceAttrNum;
    JSSVGTextContentElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[6].u.getset.set.setter_magic = JSSVGTextContentElement::putValueProperty;
    JSSVGTextContentElementAttributesFunctions[7].name = "externalResourcesRequired";
    JSSVGTextContentElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[7].magic = JSSVGTextContentElement::ExternalResourcesRequiredAttrNum;
    JSSVGTextContentElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[8].name = "className";
    JSSVGTextContentElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[8].magic = JSSVGTextContentElement::ClassNameAttrNum;
    JSSVGTextContentElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[9].name = "style";
    JSSVGTextContentElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[9].magic = JSSVGTextContentElement::StyleAttrNum;
    JSSVGTextContentElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGTextContentElementAttributesFunctions[10].name = "constructor";
    JSSVGTextContentElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextContentElementAttributesFunctions[10].magic = JSSVGTextContentElement::ConstructorAttrNum;
    JSSVGTextContentElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGTextContentElement::getValueProperty;
    JSSVGTextContentElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

class JSSVGTextContentElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGTextContentElementConstructorFunctions[3];
static bool JSSVGTextContentElementConstructorFunctions_initialized = false;

static void init_JSSVGTextContentElementConstructorFunctions()
{
    if (JSSVGTextContentElementConstructorFunctions_initialized) return;
    JSSVGTextContentElementConstructorFunctions_initialized = true;
    memset(JSSVGTextContentElementConstructorFunctions, 0, sizeof(JSSVGTextContentElementConstructorFunctions));
    JSSVGTextContentElementConstructorFunctions[0].name = "LENGTHADJUST_UNKNOWN";
    JSSVGTextContentElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementConstructorFunctions[0].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_UNKNOWN;
    JSSVGTextContentElementConstructorFunctions[1].name = "LENGTHADJUST_SPACING";
    JSSVGTextContentElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementConstructorFunctions[1].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_SPACING;
    JSSVGTextContentElementConstructorFunctions[2].name = "LENGTHADJUST_SPACINGANDGLYPHS";
    JSSVGTextContentElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementConstructorFunctions[2].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_SPACINGANDGLYPHS;
}

JSValue JSSVGTextContentElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTextContentElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTextContentElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTextContentElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextContentElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextContentElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementConstructorFunctions, countof(JSSVGTextContentElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGTextContentElementPrototypeConstantsFunctions[3];
static bool JSSVGTextContentElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGTextContentElementPrototypeConstantsFunctions()
{
    if (JSSVGTextContentElementPrototypeConstantsFunctions_initialized) return;
    JSSVGTextContentElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGTextContentElementPrototypeConstantsFunctions, 0, sizeof(JSSVGTextContentElementPrototypeConstantsFunctions));
    JSSVGTextContentElementPrototypeConstantsFunctions[0].name = "LENGTHADJUST_UNKNOWN";
    JSSVGTextContentElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_UNKNOWN;
    JSSVGTextContentElementPrototypeConstantsFunctions[1].name = "LENGTHADJUST_SPACING";
    JSSVGTextContentElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_SPACING;
    JSSVGTextContentElementPrototypeConstantsFunctions[2].name = "LENGTHADJUST_SPACINGANDGLYPHS";
    JSSVGTextContentElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTextContentElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGTextContentElement::LENGTHADJUST_SPACINGANDGLYPHS;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGTextContentElementPrototypeFunctions[11];
static bool JSSVGTextContentElementPrototypeFunctions_initialized = false;

static void init_JSSVGTextContentElementPrototypeFunctions()
{
    if (JSSVGTextContentElementPrototypeFunctions_initialized) return;
    JSSVGTextContentElementPrototypeFunctions_initialized = true;
    memset(JSSVGTextContentElementPrototypeFunctions, 0, sizeof(JSSVGTextContentElementPrototypeFunctions));
    JSSVGTextContentElementPrototypeFunctions[0].name = "getNumberOfChars";
    JSSVGTextContentElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[0].magic = JSSVGTextContentElement::GetNumberOfCharsFuncNum;
    JSSVGTextContentElementPrototypeFunctions[0].u.func.length = 0;
    JSSVGTextContentElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[1].name = "getComputedTextLength";
    JSSVGTextContentElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[1].magic = JSSVGTextContentElement::GetComputedTextLengthFuncNum;
    JSSVGTextContentElementPrototypeFunctions[1].u.func.length = 0;
    JSSVGTextContentElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[2].name = "getSubStringLength";
    JSSVGTextContentElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[2].magic = JSSVGTextContentElement::GetSubStringLengthFuncNum;
    JSSVGTextContentElementPrototypeFunctions[2].u.func.length = 2;
    JSSVGTextContentElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[3].name = "getStartPositionOfChar";
    JSSVGTextContentElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[3].magic = JSSVGTextContentElement::GetStartPositionOfCharFuncNum;
    JSSVGTextContentElementPrototypeFunctions[3].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[4].name = "getEndPositionOfChar";
    JSSVGTextContentElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[4].magic = JSSVGTextContentElement::GetEndPositionOfCharFuncNum;
    JSSVGTextContentElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[5].name = "getExtentOfChar";
    JSSVGTextContentElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[5].magic = JSSVGTextContentElement::GetExtentOfCharFuncNum;
    JSSVGTextContentElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[6].name = "getRotationOfChar";
    JSSVGTextContentElementPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[6].magic = JSSVGTextContentElement::GetRotationOfCharFuncNum;
    JSSVGTextContentElementPrototypeFunctions[6].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[7].name = "getCharNumAtPosition";
    JSSVGTextContentElementPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[7].magic = JSSVGTextContentElement::GetCharNumAtPositionFuncNum;
    JSSVGTextContentElementPrototypeFunctions[7].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[7].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[8].name = "selectSubString";
    JSSVGTextContentElementPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[8].magic = JSSVGTextContentElement::SelectSubStringFuncNum;
    JSSVGTextContentElementPrototypeFunctions[8].u.func.length = 2;
    JSSVGTextContentElementPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[8].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[9].name = "hasExtension";
    JSSVGTextContentElementPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[9].magic = JSSVGTextContentElement::HasExtensionFuncNum;
    JSSVGTextContentElementPrototypeFunctions[9].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[9].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
    JSSVGTextContentElementPrototypeFunctions[10].name = "getPresentationAttribute";
    JSSVGTextContentElementPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextContentElementPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSSVGTextContentElementPrototypeFunctions[10].magic = JSSVGTextContentElement::GetPresentationAttributeFuncNum;
    JSSVGTextContentElementPrototypeFunctions[10].u.func.length = 1;
    JSSVGTextContentElementPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextContentElementPrototypeFunctions[10].u.func.cfunc.generic_magic = JSSVGTextContentElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGTextContentElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextContentElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGTextContentElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextContentElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextContentElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextContentElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementAttributesFunctions, countof(JSSVGTextContentElementAttributesFunctions));
    init_JSSVGTextContentElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementPrototypeConstantsFunctions, countof(JSSVGTextContentElementPrototypeConstantsFunctions));
    init_JSSVGTextContentElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextContentElementPrototypeFunctions, countof(JSSVGTextContentElementPrototypeFunctions));
}

static JSClassDef JSSVGTextContentElementClassDefine;
static bool JSSVGTextContentElementClassDefine_initialized = false;

static void init_JSSVGTextContentElementClassDefine()
{
    if (JSSVGTextContentElementClassDefine_initialized) return;
    JSSVGTextContentElementClassDefine_initialized = true;
    memset(&JSSVGTextContentElementClassDefine, 0, sizeof(JSSVGTextContentElementClassDefine));
    JSSVGTextContentElementClassDefine.class_name = "SVGTextContentElement";
    JSSVGTextContentElementClassDefine.finalizer = JSSVGTextContentElement::finalizer;
    JSSVGTextContentElementClassDefine.gc_mark = JSSVGTextContentElement::mark;
}

JSClassID JSSVGTextContentElement::js_class_id = 0;

void JSSVGTextContentElement::init(JSContext* ctx)
{
    if (JSSVGTextContentElement::js_class_id == 0) {
        init_JSSVGTextContentElementClassDefine();
        JS_NewClassID(&JSSVGTextContentElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextContentElement::js_class_id, &JSSVGTextContentElementClassDefine);
        JS_SetConstructor(ctx, JSSVGTextContentElementConstructor::self(ctx), JSSVGTextContentElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTextContentElement::js_class_id, JSSVGTextContentElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextContentElement::create(JSContext* ctx, SVGTextContentElement* impl)
{
    JSSVGTextContentElement::init(ctx);
    JSValue _proto = JSSVGTextContentElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTextContentElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextContentElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextContentElement* impl = (SVGTextContentElement*)JS_GetOpaque(val, JSSVGTextContentElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextContentElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextContentElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextLengthAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->textLengthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case LengthAdjustAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->lengthAdjustAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGTextContentElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGTextContentElement::getConstructor(JSContext *ctx)
{
    return JSSVGTextContentElementConstructor::self(ctx);
}

JSValue JSSVGTextContentElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTextContentElement* imp = (SVGTextContentElement*)JS_GetOpaque(this_val, JSSVGTextContentElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTextContentElement::GetNumberOfCharsFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getNumberOfChars());
            return result;
        }
        case JSSVGTextContentElement::GetComputedTextLengthFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getComputedTextLength());
            return result;
        }
        case JSSVGTextContentElement::GetSubStringLengthFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            unsigned length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewInt32(ctx, imp->getSubStringLength(offset, length, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetStartPositionOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getStartPositionOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetEndPositionOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getEndPositionOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetExtentOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getExtentOfChar(offset, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetRotationOfCharFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewInt32(ctx, imp->getRotationOfChar(offset, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGTextContentElement::GetCharNumAtPositionFuncNum: {
            FloatPoint point = toSVGPoint(argv[0]);

            JSValue result = JS_NewInt32(ctx, imp->getCharNumAtPosition(point));
            return result;
        }
        case JSSVGTextContentElement::SelectSubStringFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            unsigned offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            unsigned length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->selectSubString(offset, length, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGTextContentElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGTextContentElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
