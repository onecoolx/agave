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
#include "QJSSVGPathElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGPathSegArcAbs.h"
#include "QJSSVGPathSegArcRel.h"
#include "QJSSVGPathSegClosePath.h"
#include "QJSSVGPathSegCurvetoCubicAbs.h"
#include "QJSSVGPathSegCurvetoCubicRel.h"
#include "QJSSVGPathSegCurvetoCubicSmoothAbs.h"
#include "QJSSVGPathSegCurvetoCubicSmoothRel.h"
#include "QJSSVGPathSegCurvetoQuadraticAbs.h"
#include "QJSSVGPathSegCurvetoQuadraticRel.h"
#include "QJSSVGPathSegCurvetoQuadraticSmoothAbs.h"
#include "QJSSVGPathSegCurvetoQuadraticSmoothRel.h"
#include "QJSSVGPathSegLinetoAbs.h"
#include "QJSSVGPathSegLinetoHorizontalAbs.h"
#include "QJSSVGPathSegLinetoHorizontalRel.h"
#include "QJSSVGPathSegLinetoRel.h"
#include "QJSSVGPathSegLinetoVerticalAbs.h"
#include "QJSSVGPathSegLinetoVerticalRel.h"
#include "QJSSVGPathSegList.h"
#include "QJSSVGPathSegMovetoAbs.h"
#include "QJSSVGPathSegMovetoRel.h"
#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGPathElement.h"
#include "SVGPathSegArc.h"
#include "SVGPathSegClosePath.h"
#include "SVGPathSegCurvetoCubic.h"
#include "SVGPathSegCurvetoCubicSmooth.h"
#include "SVGPathSegCurvetoQuadratic.h"
#include "SVGPathSegCurvetoQuadraticSmooth.h"
#include "SVGPathSegLineto.h"
#include "SVGPathSegLinetoHorizontal.h"
#include "SVGPathSegLinetoVertical.h"
#include "SVGPathSegList.h"
#include "SVGPathSegMoveto.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathElementAttributesFunctions[16];
static bool JSSVGPathElementAttributesFunctions_initialized = false;

static void init_JSSVGPathElementAttributesFunctions()
{
    if (JSSVGPathElementAttributesFunctions_initialized) return;
    JSSVGPathElementAttributesFunctions_initialized = true;
    memset(JSSVGPathElementAttributesFunctions, 0, sizeof(JSSVGPathElementAttributesFunctions));
    JSSVGPathElementAttributesFunctions[0].name = "systemLanguage";
    JSSVGPathElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[0].magic = JSSVGPathElement::SystemLanguageAttrNum;
    JSSVGPathElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[1].name = "externalResourcesRequired";
    JSSVGPathElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[1].magic = JSSVGPathElement::ExternalResourcesRequiredAttrNum;
    JSSVGPathElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[2].name = "className";
    JSSVGPathElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[2].magic = JSSVGPathElement::ClassNameAttrNum;
    JSSVGPathElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[3].name = "requiredFeatures";
    JSSVGPathElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[3].magic = JSSVGPathElement::RequiredFeaturesAttrNum;
    JSSVGPathElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[4].name = "farthestViewportElement";
    JSSVGPathElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[4].magic = JSSVGPathElement::FarthestViewportElementAttrNum;
    JSSVGPathElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[5].name = "xmllang";
    JSSVGPathElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[5].magic = JSSVGPathElement::XmllangAttrNum;
    JSSVGPathElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[5].u.getset.set.setter_magic = JSSVGPathElement::putValueProperty;
    JSSVGPathElementAttributesFunctions[6].name = "requiredExtensions";
    JSSVGPathElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[6].magic = JSSVGPathElement::RequiredExtensionsAttrNum;
    JSSVGPathElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[7].name = "pathLength";
    JSSVGPathElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[7].magic = JSSVGPathElement::PathLengthAttrNum;
    JSSVGPathElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[8].name = "style";
    JSSVGPathElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[8].magic = JSSVGPathElement::StyleAttrNum;
    JSSVGPathElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[9].name = "pathSegList";
    JSSVGPathElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[9].magic = JSSVGPathElement::PathSegListAttrNum;
    JSSVGPathElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[10].name = "xmlspace";
    JSSVGPathElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[10].magic = JSSVGPathElement::XmlspaceAttrNum;
    JSSVGPathElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[10].u.getset.set.setter_magic = JSSVGPathElement::putValueProperty;
    JSSVGPathElementAttributesFunctions[11].name = "transform";
    JSSVGPathElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[11].magic = JSSVGPathElement::TransformAttrNum;
    JSSVGPathElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[12].name = "nearestViewportElement";
    JSSVGPathElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[12].magic = JSSVGPathElement::NearestViewportElementAttrNum;
    JSSVGPathElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[13].name = "normalizedPathSegList";
    JSSVGPathElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[13].magic = JSSVGPathElement::NormalizedPathSegListAttrNum;
    JSSVGPathElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[14].name = "animatedPathSegList";
    JSSVGPathElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[14].magic = JSSVGPathElement::AnimatedPathSegListAttrNum;
    JSSVGPathElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGPathElementAttributesFunctions[15].name = "animatedNormalizedPathSegList";
    JSSVGPathElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathElementAttributesFunctions[15].magic = JSSVGPathElement::AnimatedNormalizedPathSegListAttrNum;
    JSSVGPathElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGPathElement::getValueProperty;
    JSSVGPathElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPathElementPrototypeFunctions[28];
static bool JSSVGPathElementPrototypeFunctions_initialized = false;

static void init_JSSVGPathElementPrototypeFunctions()
{
    if (JSSVGPathElementPrototypeFunctions_initialized) return;
    JSSVGPathElementPrototypeFunctions_initialized = true;
    memset(JSSVGPathElementPrototypeFunctions, 0, sizeof(JSSVGPathElementPrototypeFunctions));
    JSSVGPathElementPrototypeFunctions[0].name = "createSVGPathSegMovetoAbs";
    JSSVGPathElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[0].magic = JSSVGPathElement::CreateSVGPathSegMovetoAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[0].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[1].name = "getTotalLength";
    JSSVGPathElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[1].magic = JSSVGPathElement::GetTotalLengthFuncNum;
    JSSVGPathElementPrototypeFunctions[1].u.func.length = 0;
    JSSVGPathElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[2].name = "createSVGPathSegMovetoRel";
    JSSVGPathElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[2].magic = JSSVGPathElement::CreateSVGPathSegMovetoRelFuncNum;
    JSSVGPathElementPrototypeFunctions[2].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[3].name = "createSVGPathSegCurvetoCubicRel";
    JSSVGPathElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[3].magic = JSSVGPathElement::CreateSVGPathSegCurvetoCubicRelFuncNum;
    JSSVGPathElementPrototypeFunctions[3].u.func.length = 6;
    JSSVGPathElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[4].name = "createSVGPathSegClosePath";
    JSSVGPathElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[4].magic = JSSVGPathElement::CreateSVGPathSegClosePathFuncNum;
    JSSVGPathElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGPathElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[5].name = "getPathSegAtLength";
    JSSVGPathElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[5].magic = JSSVGPathElement::GetPathSegAtLengthFuncNum;
    JSSVGPathElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[6].name = "getTransformToElement";
    JSSVGPathElementPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[6].magic = JSSVGPathElement::GetTransformToElementFuncNum;
    JSSVGPathElementPrototypeFunctions[6].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[7].name = "createSVGPathSegCurvetoQuadraticSmoothRel";
    JSSVGPathElementPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[7].magic = JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothRelFuncNum;
    JSSVGPathElementPrototypeFunctions[7].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[7].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[8].name = "createSVGPathSegLinetoHorizontalAbs";
    JSSVGPathElementPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[8].magic = JSSVGPathElement::CreateSVGPathSegLinetoHorizontalAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[8].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[8].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[9].name = "createSVGPathSegCurvetoCubicAbs";
    JSSVGPathElementPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[9].magic = JSSVGPathElement::CreateSVGPathSegCurvetoCubicAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[9].u.func.length = 6;
    JSSVGPathElementPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[9].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[10].name = "getScreenCTM";
    JSSVGPathElementPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[10].magic = JSSVGPathElement::GetScreenCTMFuncNum;
    JSSVGPathElementPrototypeFunctions[10].u.func.length = 0;
    JSSVGPathElementPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[10].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[11].name = "createSVGPathSegCurvetoCubicSmoothRel";
    JSSVGPathElementPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[11].magic = JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothRelFuncNum;
    JSSVGPathElementPrototypeFunctions[11].u.func.length = 4;
    JSSVGPathElementPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[11].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[12].name = "getPresentationAttribute";
    JSSVGPathElementPrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[12].magic = JSSVGPathElement::GetPresentationAttributeFuncNum;
    JSSVGPathElementPrototypeFunctions[12].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[12].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[13].name = "createSVGPathSegCurvetoQuadraticAbs";
    JSSVGPathElementPrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[13].magic = JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[13].u.func.length = 4;
    JSSVGPathElementPrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[13].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[14].name = "getPointAtLength";
    JSSVGPathElementPrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[14].magic = JSSVGPathElement::GetPointAtLengthFuncNum;
    JSSVGPathElementPrototypeFunctions[14].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[14].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[15].name = "createSVGPathSegArcAbs";
    JSSVGPathElementPrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[15].magic = JSSVGPathElement::CreateSVGPathSegArcAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[15].u.func.length = 7;
    JSSVGPathElementPrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[15].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[16].name = "createSVGPathSegLinetoRel";
    JSSVGPathElementPrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[16].magic = JSSVGPathElement::CreateSVGPathSegLinetoRelFuncNum;
    JSSVGPathElementPrototypeFunctions[16].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[16].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[17].name = "hasExtension";
    JSSVGPathElementPrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[17].magic = JSSVGPathElement::HasExtensionFuncNum;
    JSSVGPathElementPrototypeFunctions[17].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[17].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[18].name = "createSVGPathSegCurvetoQuadraticSmoothAbs";
    JSSVGPathElementPrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[18].magic = JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[18].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[18].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[19].name = "createSVGPathSegCurvetoCubicSmoothAbs";
    JSSVGPathElementPrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[19].magic = JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[19].u.func.length = 4;
    JSSVGPathElementPrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[19].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[20].name = "createSVGPathSegLinetoAbs";
    JSSVGPathElementPrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[20].magic = JSSVGPathElement::CreateSVGPathSegLinetoAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[20].u.func.length = 2;
    JSSVGPathElementPrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[20].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[21].name = "createSVGPathSegCurvetoQuadraticRel";
    JSSVGPathElementPrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[21].magic = JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticRelFuncNum;
    JSSVGPathElementPrototypeFunctions[21].u.func.length = 4;
    JSSVGPathElementPrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[21].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[22].name = "createSVGPathSegArcRel";
    JSSVGPathElementPrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[22].magic = JSSVGPathElement::CreateSVGPathSegArcRelFuncNum;
    JSSVGPathElementPrototypeFunctions[22].u.func.length = 7;
    JSSVGPathElementPrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[22].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[23].name = "createSVGPathSegLinetoHorizontalRel";
    JSSVGPathElementPrototypeFunctions[23].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[23].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[23].magic = JSSVGPathElement::CreateSVGPathSegLinetoHorizontalRelFuncNum;
    JSSVGPathElementPrototypeFunctions[23].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[23].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[23].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[24].name = "createSVGPathSegLinetoVerticalAbs";
    JSSVGPathElementPrototypeFunctions[24].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[24].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[24].magic = JSSVGPathElement::CreateSVGPathSegLinetoVerticalAbsFuncNum;
    JSSVGPathElementPrototypeFunctions[24].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[24].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[24].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[25].name = "createSVGPathSegLinetoVerticalRel";
    JSSVGPathElementPrototypeFunctions[25].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[25].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[25].magic = JSSVGPathElement::CreateSVGPathSegLinetoVerticalRelFuncNum;
    JSSVGPathElementPrototypeFunctions[25].u.func.length = 1;
    JSSVGPathElementPrototypeFunctions[25].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[25].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[26].name = "getBBox";
    JSSVGPathElementPrototypeFunctions[26].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[26].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[26].magic = JSSVGPathElement::GetBBoxFuncNum;
    JSSVGPathElementPrototypeFunctions[26].u.func.length = 0;
    JSSVGPathElementPrototypeFunctions[26].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[26].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
    JSSVGPathElementPrototypeFunctions[27].name = "getCTM";
    JSSVGPathElementPrototypeFunctions[27].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathElementPrototypeFunctions[27].def_type = JS_DEF_CFUNC;
    JSSVGPathElementPrototypeFunctions[27].magic = JSSVGPathElement::GetCTMFuncNum;
    JSSVGPathElementPrototypeFunctions[27].u.func.length = 0;
    JSSVGPathElementPrototypeFunctions[27].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathElementPrototypeFunctions[27].u.func.cfunc.generic_magic = JSSVGPathElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathElementAttributesFunctions, countof(JSSVGPathElementAttributesFunctions));
    init_JSSVGPathElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathElementPrototypeFunctions, countof(JSSVGPathElementPrototypeFunctions));
}

static JSClassDef JSSVGPathElementClassDefine;
static bool JSSVGPathElementClassDefine_initialized = false;

static void init_JSSVGPathElementClassDefine()
{
    if (JSSVGPathElementClassDefine_initialized) return;
    JSSVGPathElementClassDefine_initialized = true;
    memset(&JSSVGPathElementClassDefine, 0, sizeof(JSSVGPathElementClassDefine));
    JSSVGPathElementClassDefine.class_name = "SVGPathElement";
    JSSVGPathElementClassDefine.finalizer = JSSVGPathElement::finalizer;
    JSSVGPathElementClassDefine.gc_mark = JSSVGPathElement::mark;
}

JSClassID JSSVGPathElement::js_class_id = 0;

void JSSVGPathElement::init(JSContext* ctx)
{
    if (JSSVGPathElement::js_class_id == 0) {
        init_JSSVGPathElementClassDefine();
        JS_NewClassID(&JSSVGPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathElement::js_class_id, &JSSVGPathElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPathElement::js_class_id, JSSVGPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGPathElement::create(JSContext* ctx, SVGPathElement* impl)
{
    JSSVGPathElement::init(ctx);
    JSValue _proto = JSSVGPathElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathElement* impl = (SVGPathElement*)JS_GetOpaque(val, JSSVGPathElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPathElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PathLengthAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pathLengthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->pathSegList()), imp);
        }
        case NormalizedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->normalizedPathSegList()), imp);
        }
        case AnimatedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPathSegList()), imp);
        }
        case AnimatedNormalizedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedNormalizedPathSegList()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPathElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque(this_val, JSSVGPathElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPathElement::GetTotalLengthFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getTotalLength());
            return result;
        }
        case JSSVGPathElement::GetPointAtLengthFuncNum: {
            float distance = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getPointAtLength(distance)), imp);
            return result;
        }
        case JSSVGPathElement::GetPathSegAtLengthFuncNum: {
            float distance = valueToFloat(ctx, argv[0]);

            JSValue result = JS_NewInt32(ctx, imp->getPathSegAtLength(distance));
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegClosePathFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegClosePath()), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegMovetoAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegMovetoAbs(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegMovetoRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegMovetoRel(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoAbs(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoRel(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoCubicAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x1 = valueToFloat(ctx, argv[2]);
            float y1 = valueToFloat(ctx, argv[3]);
            float x2 = valueToFloat(ctx, argv[4]);
            float y2 = valueToFloat(ctx, argv[5]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoCubicAbs(x, y, x1, y1, x2, y2)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoCubicRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x1 = valueToFloat(ctx, argv[2]);
            float y1 = valueToFloat(ctx, argv[3]);
            float x2 = valueToFloat(ctx, argv[4]);
            float y2 = valueToFloat(ctx, argv[5]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoCubicRel(x, y, x1, y1, x2, y2)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x1 = valueToFloat(ctx, argv[2]);
            float y1 = valueToFloat(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoQuadraticAbs(x, y, x1, y1)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x1 = valueToFloat(ctx, argv[2]);
            float y1 = valueToFloat(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoQuadraticRel(x, y, x1, y1)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegArcAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float r1 = valueToFloat(ctx, argv[2]);
            float r2 = valueToFloat(ctx, argv[3]);
            float angle = valueToFloat(ctx, argv[4]);
            bool largeArcFlag = valueToBoolean(ctx, argv[5]);
            bool sweepFlag = valueToBoolean(ctx, argv[6]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegArcAbs(x, y, r1, r2, angle, largeArcFlag, sweepFlag)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegArcRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float r1 = valueToFloat(ctx, argv[2]);
            float r2 = valueToFloat(ctx, argv[3]);
            float angle = valueToFloat(ctx, argv[4]);
            bool largeArcFlag = valueToBoolean(ctx, argv[5]);
            bool sweepFlag = valueToBoolean(ctx, argv[6]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegArcRel(x, y, r1, r2, angle, largeArcFlag, sweepFlag)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoHorizontalAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoHorizontalAbs(x)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoHorizontalRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoHorizontalRel(x)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoVerticalAbsFuncNum: {
            float y = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoVerticalAbs(y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegLinetoVerticalRelFuncNum: {
            float y = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegLinetoVerticalRel(y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x2 = valueToFloat(ctx, argv[2]);
            float y2 = valueToFloat(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoCubicSmoothAbs(x, y, x2, y2)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float x2 = valueToFloat(ctx, argv[2]);
            float y2 = valueToFloat(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoCubicSmoothRel(x, y, x2, y2)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothAbsFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoQuadraticSmoothAbs(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothRelFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGPathSegCurvetoQuadraticSmoothRel(x, y)), imp);
            return result;
        }
        case JSSVGPathElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPathElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGPathElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGPathElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGPathElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGPathElement::GetTransformToElementFuncNum: {
            ExceptionCode ec = 0;
            SVGElement* element = toSVGElement(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getTransformToElement(element, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
