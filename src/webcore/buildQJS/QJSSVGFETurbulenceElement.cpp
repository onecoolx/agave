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
#include "QJSSVGFETurbulenceElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedInteger.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFETurbulenceElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFETurbulenceElementAttributesFunctions[14];
static bool JSSVGFETurbulenceElementAttributesFunctions_initialized = false;

static void init_JSSVGFETurbulenceElementAttributesFunctions()
{
    if (JSSVGFETurbulenceElementAttributesFunctions_initialized) return;
    JSSVGFETurbulenceElementAttributesFunctions_initialized = true;
    memset(JSSVGFETurbulenceElementAttributesFunctions, 0, sizeof(JSSVGFETurbulenceElementAttributesFunctions));
    JSSVGFETurbulenceElementAttributesFunctions[0].name = "baseFrequencyY";
    JSSVGFETurbulenceElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[0].magic = JSSVGFETurbulenceElement::BaseFrequencyYAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[1].name = "baseFrequencyX";
    JSSVGFETurbulenceElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[1].magic = JSSVGFETurbulenceElement::BaseFrequencyXAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[2].name = "style";
    JSSVGFETurbulenceElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[2].magic = JSSVGFETurbulenceElement::StyleAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[3].name = "stitchTiles";
    JSSVGFETurbulenceElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[3].magic = JSSVGFETurbulenceElement::StitchTilesAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[4].name = "numOctaves";
    JSSVGFETurbulenceElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[4].magic = JSSVGFETurbulenceElement::NumOctavesAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[5].name = "result";
    JSSVGFETurbulenceElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[5].magic = JSSVGFETurbulenceElement::ResultAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[6].name = "className";
    JSSVGFETurbulenceElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[6].magic = JSSVGFETurbulenceElement::ClassNameAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[7].name = "x";
    JSSVGFETurbulenceElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[7].magic = JSSVGFETurbulenceElement::XAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[8].name = "seed";
    JSSVGFETurbulenceElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[8].magic = JSSVGFETurbulenceElement::SeedAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[9].name = "y";
    JSSVGFETurbulenceElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[9].magic = JSSVGFETurbulenceElement::YAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[10].name = "type";
    JSSVGFETurbulenceElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[10].magic = JSSVGFETurbulenceElement::TypeAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[11].name = "width";
    JSSVGFETurbulenceElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[11].magic = JSSVGFETurbulenceElement::WidthAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[12].name = "height";
    JSSVGFETurbulenceElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[12].magic = JSSVGFETurbulenceElement::HeightAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementAttributesFunctions[13].name = "constructor";
    JSSVGFETurbulenceElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementAttributesFunctions[13].magic = JSSVGFETurbulenceElement::ConstructorAttrNum;
    JSSVGFETurbulenceElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGFETurbulenceElement::getValueProperty;
    JSSVGFETurbulenceElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

class JSSVGFETurbulenceElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFETurbulenceElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGFETurbulenceElementConstructorFunctions[6];
static bool JSSVGFETurbulenceElementConstructorFunctions_initialized = false;

static void init_JSSVGFETurbulenceElementConstructorFunctions()
{
    if (JSSVGFETurbulenceElementConstructorFunctions_initialized) return;
    JSSVGFETurbulenceElementConstructorFunctions_initialized = true;
    memset(JSSVGFETurbulenceElementConstructorFunctions, 0, sizeof(JSSVGFETurbulenceElementConstructorFunctions));
    JSSVGFETurbulenceElementConstructorFunctions[0].name = "SVG_TURBULENCE_TYPE_UNKNOWN";
    JSSVGFETurbulenceElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[0].magic = WebCore::SVG_TURBULENCE_TYPE_UNKNOWN;
    JSSVGFETurbulenceElementConstructorFunctions[0].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementConstructorFunctions[1].name = "SVG_TURBULENCE_TYPE_FRACTALNOISE";
    JSSVGFETurbulenceElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[1].magic = WebCore::SVG_TURBULENCE_TYPE_FRACTALNOISE;
    JSSVGFETurbulenceElementConstructorFunctions[1].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementConstructorFunctions[2].name = "SVG_TURBULENCE_TYPE_TURBULENCE";
    JSSVGFETurbulenceElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[2].magic = WebCore::SVG_TURBULENCE_TYPE_TURBULENCE;
    JSSVGFETurbulenceElementConstructorFunctions[2].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementConstructorFunctions[3].name = "SVG_STITCHTYPE_UNKNOWN";
    JSSVGFETurbulenceElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[3].magic = WebCore::SVG_STITCHTYPE_UNKNOWN;
    JSSVGFETurbulenceElementConstructorFunctions[3].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementConstructorFunctions[4].name = "SVG_STITCHTYPE_STITCH";
    JSSVGFETurbulenceElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[4].magic = WebCore::SVG_STITCHTYPE_STITCH;
    JSSVGFETurbulenceElementConstructorFunctions[4].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementConstructorFunctions[5].name = "SVG_STITCHTYPE_NOSTITCH";
    JSSVGFETurbulenceElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementConstructorFunctions[5].magic = WebCore::SVG_STITCHTYPE_NOSTITCH;
    JSSVGFETurbulenceElementConstructorFunctions[5].u.getset.get.getter_magic = JSSVGFETurbulenceElementConstructor::getValueProperty;
    JSSVGFETurbulenceElementConstructorFunctions[5].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGFETurbulenceElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFETurbulenceElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFETurbulenceElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFETurbulenceElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFETurbulenceElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFETurbulenceElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementConstructorFunctions, countof(JSSVGFETurbulenceElementConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGFETurbulenceElementPrototypeConstantsFunctions[6];
static bool JSSVGFETurbulenceElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGFETurbulenceElementPrototypeConstantsFunctions()
{
    if (JSSVGFETurbulenceElementPrototypeConstantsFunctions_initialized) return;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGFETurbulenceElementPrototypeConstantsFunctions, 0, sizeof(JSSVGFETurbulenceElementPrototypeConstantsFunctions));
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].name = "SVG_TURBULENCE_TYPE_UNKNOWN";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].magic = WebCore::SVG_TURBULENCE_TYPE_UNKNOWN;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].name = "SVG_TURBULENCE_TYPE_FRACTALNOISE";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].magic = WebCore::SVG_TURBULENCE_TYPE_FRACTALNOISE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].name = "SVG_TURBULENCE_TYPE_TURBULENCE";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].magic = WebCore::SVG_TURBULENCE_TYPE_TURBULENCE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].name = "SVG_STITCHTYPE_UNKNOWN";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].magic = WebCore::SVG_STITCHTYPE_UNKNOWN;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].name = "SVG_STITCHTYPE_STITCH";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].magic = WebCore::SVG_STITCHTYPE_STITCH;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].name = "SVG_STITCHTYPE_NOSTITCH";
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].magic = WebCore::SVG_STITCHTYPE_NOSTITCH;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSSVGFETurbulenceElementPrototype::getValueProperty;
    JSSVGFETurbulenceElementPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFETurbulenceElementPrototypeFunctions[1];
static bool JSSVGFETurbulenceElementPrototypeFunctions_initialized = false;

static void init_JSSVGFETurbulenceElementPrototypeFunctions()
{
    if (JSSVGFETurbulenceElementPrototypeFunctions_initialized) return;
    JSSVGFETurbulenceElementPrototypeFunctions_initialized = true;
    memset(JSSVGFETurbulenceElementPrototypeFunctions, 0, sizeof(JSSVGFETurbulenceElementPrototypeFunctions));
    JSSVGFETurbulenceElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFETurbulenceElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFETurbulenceElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFETurbulenceElementPrototypeFunctions[0].magic = JSSVGFETurbulenceElement::GetPresentationAttributeFuncNum;
    JSSVGFETurbulenceElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFETurbulenceElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFETurbulenceElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFETurbulenceElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFETurbulenceElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFETurbulenceElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFETurbulenceElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFETurbulenceElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFETurbulenceElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFETurbulenceElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementAttributesFunctions, countof(JSSVGFETurbulenceElementAttributesFunctions));
    init_JSSVGFETurbulenceElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementPrototypeConstantsFunctions, countof(JSSVGFETurbulenceElementPrototypeConstantsFunctions));
    init_JSSVGFETurbulenceElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementPrototypeFunctions, countof(JSSVGFETurbulenceElementPrototypeFunctions));
}

JSValue JSSVGFETurbulenceElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFETurbulenceElementClassDefine;
static bool JSSVGFETurbulenceElementClassDefine_initialized = false;

static void init_JSSVGFETurbulenceElementClassDefine()
{
    if (JSSVGFETurbulenceElementClassDefine_initialized) return;
    JSSVGFETurbulenceElementClassDefine_initialized = true;
    memset(&JSSVGFETurbulenceElementClassDefine, 0, sizeof(JSSVGFETurbulenceElementClassDefine));
    JSSVGFETurbulenceElementClassDefine.class_name = "SVGFETurbulenceElement";
    JSSVGFETurbulenceElementClassDefine.finalizer = JSSVGFETurbulenceElement::finalizer;
    JSSVGFETurbulenceElementClassDefine.gc_mark = JSSVGFETurbulenceElement::mark;
}

JSClassID JSSVGFETurbulenceElement::js_class_id = 0;

void JSSVGFETurbulenceElement::init(JSContext* ctx)
{
    if (JSSVGFETurbulenceElement::js_class_id == 0) {
        init_JSSVGFETurbulenceElementClassDefine();
        JS_NewClassID(&JSSVGFETurbulenceElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFETurbulenceElement::js_class_id, &JSSVGFETurbulenceElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFETurbulenceElementConstructor::self(ctx), JSSVGFETurbulenceElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFETurbulenceElement::js_class_id, JSSVGFETurbulenceElementPrototype::self(ctx));
    }
}

JSValue JSSVGFETurbulenceElement::create(JSContext* ctx, SVGFETurbulenceElement* impl)
{
    JSSVGFETurbulenceElement::init(ctx);
    JSValue _proto = JSSVGFETurbulenceElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFETurbulenceElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFETurbulenceElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFETurbulenceElement* impl = (SVGFETurbulenceElement*)JS_GetOpaque(val, JSSVGFETurbulenceElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFETurbulenceElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFETurbulenceElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseFrequencyXAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->baseFrequencyXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case BaseFrequencyYAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->baseFrequencyYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NumOctavesAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->numOctavesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SeedAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->seedAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StitchTilesAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->stitchTilesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case TypeAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFETurbulenceElement::getConstructor(JSContext *ctx)
{
    return JSSVGFETurbulenceElementConstructor::self(ctx);
}

JSValue JSSVGFETurbulenceElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque(this_val, JSSVGFETurbulenceElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFETurbulenceElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
