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
#include "QJSSVGFECompositeElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFECompositeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFECompositeElementAttributesFunctions[15];
static bool JSSVGFECompositeElementAttributesFunctions_initialized = false;

static void init_JSSVGFECompositeElementAttributesFunctions()
{
    if (JSSVGFECompositeElementAttributesFunctions_initialized) return;
    JSSVGFECompositeElementAttributesFunctions_initialized = true;
    memset(JSSVGFECompositeElementAttributesFunctions, 0, sizeof(JSSVGFECompositeElementAttributesFunctions));
    JSSVGFECompositeElementAttributesFunctions[0].name = "y";
    JSSVGFECompositeElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[0].magic = JSSVGFECompositeElement::YAttrNum;
    JSSVGFECompositeElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[1].name = "_operator";
    JSSVGFECompositeElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[1].magic = JSSVGFECompositeElement::_operatorAttrNum;
    JSSVGFECompositeElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[2].name = "k1";
    JSSVGFECompositeElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[2].magic = JSSVGFECompositeElement::K1AttrNum;
    JSSVGFECompositeElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[3].name = "in2";
    JSSVGFECompositeElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[3].magic = JSSVGFECompositeElement::In2AttrNum;
    JSSVGFECompositeElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[4].name = "in1";
    JSSVGFECompositeElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[4].magic = JSSVGFECompositeElement::In1AttrNum;
    JSSVGFECompositeElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[5].name = "x";
    JSSVGFECompositeElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[5].magic = JSSVGFECompositeElement::XAttrNum;
    JSSVGFECompositeElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[6].name = "k3";
    JSSVGFECompositeElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[6].magic = JSSVGFECompositeElement::K3AttrNum;
    JSSVGFECompositeElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[7].name = "constructor";
    JSSVGFECompositeElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[7].magic = JSSVGFECompositeElement::ConstructorAttrNum;
    JSSVGFECompositeElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[8].name = "k2";
    JSSVGFECompositeElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[8].magic = JSSVGFECompositeElement::K2AttrNum;
    JSSVGFECompositeElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[9].name = "k4";
    JSSVGFECompositeElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[9].magic = JSSVGFECompositeElement::K4AttrNum;
    JSSVGFECompositeElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[10].name = "width";
    JSSVGFECompositeElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[10].magic = JSSVGFECompositeElement::WidthAttrNum;
    JSSVGFECompositeElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[11].name = "height";
    JSSVGFECompositeElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[11].magic = JSSVGFECompositeElement::HeightAttrNum;
    JSSVGFECompositeElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[12].name = "result";
    JSSVGFECompositeElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[12].magic = JSSVGFECompositeElement::ResultAttrNum;
    JSSVGFECompositeElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[13].name = "className";
    JSSVGFECompositeElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[13].magic = JSSVGFECompositeElement::ClassNameAttrNum;
    JSSVGFECompositeElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGFECompositeElementAttributesFunctions[14].name = "style";
    JSSVGFECompositeElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFECompositeElementAttributesFunctions[14].magic = JSSVGFECompositeElement::StyleAttrNum;
    JSSVGFECompositeElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGFECompositeElement::getValueProperty;
    JSSVGFECompositeElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

class JSSVGFECompositeElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGFECompositeElementConstructorFunctions[7];
static bool JSSVGFECompositeElementConstructorFunctions_initialized = false;

static void init_JSSVGFECompositeElementConstructorFunctions()
{
    if (JSSVGFECompositeElementConstructorFunctions_initialized) return;
    JSSVGFECompositeElementConstructorFunctions_initialized = true;
    memset(JSSVGFECompositeElementConstructorFunctions, 0, sizeof(JSSVGFECompositeElementConstructorFunctions));
    JSSVGFECompositeElementConstructorFunctions[0].name = "SVG_FECOMPOSITE_OPERATOR_UNKNOWN";
    JSSVGFECompositeElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_UNKNOWN;
    JSSVGFECompositeElementConstructorFunctions[1].name = "SVG_FECOMPOSITE_OPERATOR_OVER";
    JSSVGFECompositeElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_OVER;
    JSSVGFECompositeElementConstructorFunctions[2].name = "SVG_FECOMPOSITE_OPERATOR_IN";
    JSSVGFECompositeElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_IN;
    JSSVGFECompositeElementConstructorFunctions[3].name = "SVG_FECOMPOSITE_OPERATOR_OUT";
    JSSVGFECompositeElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_OUT;
    JSSVGFECompositeElementConstructorFunctions[4].name = "SVG_FECOMPOSITE_OPERATOR_ATOP";
    JSSVGFECompositeElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_ATOP;
    JSSVGFECompositeElementConstructorFunctions[5].name = "SVG_FECOMPOSITE_OPERATOR_XOR";
    JSSVGFECompositeElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[5].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_XOR;
    JSSVGFECompositeElementConstructorFunctions[6].name = "SVG_FECOMPOSITE_OPERATOR_ARITHMETIC";
    JSSVGFECompositeElementConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementConstructorFunctions[6].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_ARITHMETIC;
}

JSValue JSSVGFECompositeElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFECompositeElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFECompositeElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFECompositeElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFECompositeElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFECompositeElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementConstructorFunctions, countof(JSSVGFECompositeElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGFECompositeElementPrototypeConstantsFunctions[7];
static bool JSSVGFECompositeElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGFECompositeElementPrototypeConstantsFunctions()
{
    if (JSSVGFECompositeElementPrototypeConstantsFunctions_initialized) return;
    JSSVGFECompositeElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGFECompositeElementPrototypeConstantsFunctions, 0, sizeof(JSSVGFECompositeElementPrototypeConstantsFunctions));
    JSSVGFECompositeElementPrototypeConstantsFunctions[0].name = "SVG_FECOMPOSITE_OPERATOR_UNKNOWN";
    JSSVGFECompositeElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_UNKNOWN;
    JSSVGFECompositeElementPrototypeConstantsFunctions[1].name = "SVG_FECOMPOSITE_OPERATOR_OVER";
    JSSVGFECompositeElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_OVER;
    JSSVGFECompositeElementPrototypeConstantsFunctions[2].name = "SVG_FECOMPOSITE_OPERATOR_IN";
    JSSVGFECompositeElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_IN;
    JSSVGFECompositeElementPrototypeConstantsFunctions[3].name = "SVG_FECOMPOSITE_OPERATOR_OUT";
    JSSVGFECompositeElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_OUT;
    JSSVGFECompositeElementPrototypeConstantsFunctions[4].name = "SVG_FECOMPOSITE_OPERATOR_ATOP";
    JSSVGFECompositeElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_ATOP;
    JSSVGFECompositeElementPrototypeConstantsFunctions[5].name = "SVG_FECOMPOSITE_OPERATOR_XOR";
    JSSVGFECompositeElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[5].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_XOR;
    JSSVGFECompositeElementPrototypeConstantsFunctions[6].name = "SVG_FECOMPOSITE_OPERATOR_ARITHMETIC";
    JSSVGFECompositeElementPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGFECompositeElementPrototypeConstantsFunctions[6].u.i32 = (int32_t)WebCore::SVG_FECOMPOSITE_OPERATOR_ARITHMETIC;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFECompositeElementPrototypeFunctions[1];
static bool JSSVGFECompositeElementPrototypeFunctions_initialized = false;

static void init_JSSVGFECompositeElementPrototypeFunctions()
{
    if (JSSVGFECompositeElementPrototypeFunctions_initialized) return;
    JSSVGFECompositeElementPrototypeFunctions_initialized = true;
    memset(JSSVGFECompositeElementPrototypeFunctions, 0, sizeof(JSSVGFECompositeElementPrototypeFunctions));
    JSSVGFECompositeElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFECompositeElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFECompositeElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFECompositeElementPrototypeFunctions[0].magic = JSSVGFECompositeElement::GetPresentationAttributeFuncNum;
    JSSVGFECompositeElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFECompositeElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFECompositeElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFECompositeElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFECompositeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFECompositeElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFECompositeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFECompositeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFECompositeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFECompositeElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementAttributesFunctions, countof(JSSVGFECompositeElementAttributesFunctions));
    init_JSSVGFECompositeElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementPrototypeConstantsFunctions, countof(JSSVGFECompositeElementPrototypeConstantsFunctions));
    init_JSSVGFECompositeElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementPrototypeFunctions, countof(JSSVGFECompositeElementPrototypeFunctions));
}

static JSClassDef JSSVGFECompositeElementClassDefine;
static bool JSSVGFECompositeElementClassDefine_initialized = false;

static void init_JSSVGFECompositeElementClassDefine()
{
    if (JSSVGFECompositeElementClassDefine_initialized) return;
    JSSVGFECompositeElementClassDefine_initialized = true;
    memset(&JSSVGFECompositeElementClassDefine, 0, sizeof(JSSVGFECompositeElementClassDefine));
    JSSVGFECompositeElementClassDefine.class_name = "SVGFECompositeElement";
    JSSVGFECompositeElementClassDefine.finalizer = JSSVGFECompositeElement::finalizer;
    JSSVGFECompositeElementClassDefine.gc_mark = JSSVGFECompositeElement::mark;
}

JSClassID JSSVGFECompositeElement::js_class_id = 0;

void JSSVGFECompositeElement::init(JSContext* ctx)
{
    if (JSSVGFECompositeElement::js_class_id == 0) {
        init_JSSVGFECompositeElementClassDefine();
        JS_NewClassID(&JSSVGFECompositeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFECompositeElement::js_class_id, &JSSVGFECompositeElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFECompositeElementConstructor::self(ctx), JSSVGFECompositeElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFECompositeElement::js_class_id, JSSVGFECompositeElementPrototype::self(ctx));
    }
}

JSValue JSSVGFECompositeElement::create(JSContext* ctx, SVGFECompositeElement* impl)
{
    JSSVGFECompositeElement::init(ctx);
    JSValue _proto = JSSVGFECompositeElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFECompositeElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFECompositeElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFECompositeElement* impl = (SVGFECompositeElement*)JS_GetOpaque(val, JSSVGFECompositeElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFECompositeElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFECompositeElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case _operatorAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->_operatorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case K1AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K2AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K3AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k3Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K4AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k4Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFECompositeElement::getConstructor(JSContext *ctx)
{
    return JSSVGFECompositeElementConstructor::self(ctx);
}

JSValue JSSVGFECompositeElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque(this_val, JSSVGFECompositeElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFECompositeElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
