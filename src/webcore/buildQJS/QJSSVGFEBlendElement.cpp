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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFEBlendElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEBlendElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEBlendElementAttributesFunctions[11];
static bool JSSVGFEBlendElementAttributesFunctions_initialized = false;

static void init_JSSVGFEBlendElementAttributesFunctions()
{
    if (JSSVGFEBlendElementAttributesFunctions_initialized) return;
    JSSVGFEBlendElementAttributesFunctions_initialized = true;
    memset(JSSVGFEBlendElementAttributesFunctions, 0, sizeof(JSSVGFEBlendElementAttributesFunctions));
    JSSVGFEBlendElementAttributesFunctions[0].name = "x";
    JSSVGFEBlendElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[0].magic = JSSVGFEBlendElement::XAttrNum;
    JSSVGFEBlendElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[1].name = "height";
    JSSVGFEBlendElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[1].magic = JSSVGFEBlendElement::HeightAttrNum;
    JSSVGFEBlendElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[2].name = "mode";
    JSSVGFEBlendElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[2].magic = JSSVGFEBlendElement::ModeAttrNum;
    JSSVGFEBlendElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[3].name = "in2";
    JSSVGFEBlendElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[3].magic = JSSVGFEBlendElement::In2AttrNum;
    JSSVGFEBlendElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[4].name = "y";
    JSSVGFEBlendElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[4].magic = JSSVGFEBlendElement::YAttrNum;
    JSSVGFEBlendElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[5].name = "in1";
    JSSVGFEBlendElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[5].magic = JSSVGFEBlendElement::In1AttrNum;
    JSSVGFEBlendElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[6].name = "result";
    JSSVGFEBlendElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[6].magic = JSSVGFEBlendElement::ResultAttrNum;
    JSSVGFEBlendElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[7].name = "width";
    JSSVGFEBlendElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[7].magic = JSSVGFEBlendElement::WidthAttrNum;
    JSSVGFEBlendElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[8].name = "className";
    JSSVGFEBlendElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[8].magic = JSSVGFEBlendElement::ClassNameAttrNum;
    JSSVGFEBlendElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[9].name = "style";
    JSSVGFEBlendElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[9].magic = JSSVGFEBlendElement::StyleAttrNum;
    JSSVGFEBlendElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementAttributesFunctions[10].name = "constructor";
    JSSVGFEBlendElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementAttributesFunctions[10].magic = JSSVGFEBlendElement::ConstructorAttrNum;
    JSSVGFEBlendElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFEBlendElement::getValueProperty;
    JSSVGFEBlendElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

class JSSVGFEBlendElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFEBlendElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGFEBlendElementConstructorFunctions[6];
static bool JSSVGFEBlendElementConstructorFunctions_initialized = false;

static void init_JSSVGFEBlendElementConstructorFunctions()
{
    if (JSSVGFEBlendElementConstructorFunctions_initialized) return;
    JSSVGFEBlendElementConstructorFunctions_initialized = true;
    memset(JSSVGFEBlendElementConstructorFunctions, 0, sizeof(JSSVGFEBlendElementConstructorFunctions));
    JSSVGFEBlendElementConstructorFunctions[0].name = "SVG_FEBLEND_MODE_NORMAL";
    JSSVGFEBlendElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[0].magic = WebCore::SVG_FEBLEND_MODE_NORMAL;
    JSSVGFEBlendElementConstructorFunctions[0].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementConstructorFunctions[1].name = "SVG_FEBLEND_MODE_UNKNOWN";
    JSSVGFEBlendElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[1].magic = WebCore::SVG_FEBLEND_MODE_UNKNOWN;
    JSSVGFEBlendElementConstructorFunctions[1].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementConstructorFunctions[2].name = "SVG_FEBLEND_MODE_MULTIPLY";
    JSSVGFEBlendElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[2].magic = WebCore::SVG_FEBLEND_MODE_MULTIPLY;
    JSSVGFEBlendElementConstructorFunctions[2].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementConstructorFunctions[3].name = "SVG_FEBLEND_MODE_SCREEN";
    JSSVGFEBlendElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[3].magic = WebCore::SVG_FEBLEND_MODE_SCREEN;
    JSSVGFEBlendElementConstructorFunctions[3].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementConstructorFunctions[4].name = "SVG_FEBLEND_MODE_DARKEN";
    JSSVGFEBlendElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[4].magic = WebCore::SVG_FEBLEND_MODE_DARKEN;
    JSSVGFEBlendElementConstructorFunctions[4].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementConstructorFunctions[5].name = "SVG_FEBLEND_MODE_LIGHTEN";
    JSSVGFEBlendElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementConstructorFunctions[5].magic = WebCore::SVG_FEBLEND_MODE_LIGHTEN;
    JSSVGFEBlendElementConstructorFunctions[5].u.getset.get.getter_magic = JSSVGFEBlendElementConstructor::getValueProperty;
    JSSVGFEBlendElementConstructorFunctions[5].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGFEBlendElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEBlendElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEBlendElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEBlendElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEBlendElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEBlendElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementConstructorFunctions, countof(JSSVGFEBlendElementConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGFEBlendElementPrototypeConstantsFunctions[6];
static bool JSSVGFEBlendElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGFEBlendElementPrototypeConstantsFunctions()
{
    if (JSSVGFEBlendElementPrototypeConstantsFunctions_initialized) return;
    JSSVGFEBlendElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGFEBlendElementPrototypeConstantsFunctions, 0, sizeof(JSSVGFEBlendElementPrototypeConstantsFunctions));
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].name = "SVG_FEBLEND_MODE_NORMAL";
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].magic = WebCore::SVG_FEBLEND_MODE_NORMAL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].name = "SVG_FEBLEND_MODE_UNKNOWN";
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].magic = WebCore::SVG_FEBLEND_MODE_UNKNOWN;
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].name = "SVG_FEBLEND_MODE_MULTIPLY";
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].magic = WebCore::SVG_FEBLEND_MODE_MULTIPLY;
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].name = "SVG_FEBLEND_MODE_SCREEN";
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].magic = WebCore::SVG_FEBLEND_MODE_SCREEN;
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].name = "SVG_FEBLEND_MODE_DARKEN";
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].magic = WebCore::SVG_FEBLEND_MODE_DARKEN;
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].name = "SVG_FEBLEND_MODE_LIGHTEN";
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].magic = WebCore::SVG_FEBLEND_MODE_LIGHTEN;
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSSVGFEBlendElementPrototype::getValueProperty;
    JSSVGFEBlendElementPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEBlendElementPrototypeFunctions[1];
static bool JSSVGFEBlendElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEBlendElementPrototypeFunctions()
{
    if (JSSVGFEBlendElementPrototypeFunctions_initialized) return;
    JSSVGFEBlendElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEBlendElementPrototypeFunctions, 0, sizeof(JSSVGFEBlendElementPrototypeFunctions));
    JSSVGFEBlendElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEBlendElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEBlendElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEBlendElementPrototypeFunctions[0].magic = JSSVGFEBlendElement::GetPresentationAttributeFuncNum;
    JSSVGFEBlendElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEBlendElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEBlendElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEBlendElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEBlendElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEBlendElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEBlendElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEBlendElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEBlendElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEBlendElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementAttributesFunctions, countof(JSSVGFEBlendElementAttributesFunctions));
    init_JSSVGFEBlendElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementPrototypeConstantsFunctions, countof(JSSVGFEBlendElementPrototypeConstantsFunctions));
    init_JSSVGFEBlendElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementPrototypeFunctions, countof(JSSVGFEBlendElementPrototypeFunctions));
}

JSValue JSSVGFEBlendElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFEBlendElementClassDefine;
static bool JSSVGFEBlendElementClassDefine_initialized = false;

static void init_JSSVGFEBlendElementClassDefine()
{
    if (JSSVGFEBlendElementClassDefine_initialized) return;
    JSSVGFEBlendElementClassDefine_initialized = true;
    memset(&JSSVGFEBlendElementClassDefine, 0, sizeof(JSSVGFEBlendElementClassDefine));
    JSSVGFEBlendElementClassDefine.class_name = "SVGFEBlendElement";
    JSSVGFEBlendElementClassDefine.finalizer = JSSVGFEBlendElement::finalizer;
    JSSVGFEBlendElementClassDefine.gc_mark = JSSVGFEBlendElement::mark;
}

JSClassID JSSVGFEBlendElement::js_class_id = 0;

void JSSVGFEBlendElement::init(JSContext* ctx)
{
    if (JSSVGFEBlendElement::js_class_id == 0) {
        init_JSSVGFEBlendElementClassDefine();
        JS_NewClassID(&JSSVGFEBlendElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEBlendElement::js_class_id, &JSSVGFEBlendElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEBlendElementConstructor::self(ctx), JSSVGFEBlendElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEBlendElement::js_class_id, JSSVGFEBlendElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEBlendElement::create(JSContext* ctx, SVGFEBlendElement* impl)
{
    JSSVGFEBlendElement::init(ctx);
    JSValue _proto = JSSVGFEBlendElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEBlendElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEBlendElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEBlendElement* impl = (SVGFEBlendElement*)JS_GetOpaque(val, JSSVGFEBlendElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEBlendElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEBlendElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case ModeAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->modeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEBlendElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEBlendElementConstructor::self(ctx);
}

JSValue JSSVGFEBlendElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque(this_val, JSSVGFEBlendElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEBlendElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
