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
#include "QJSSVGSVGElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "ExceptionCode.h"
#include "FloatRect.h"
#include "NameNodeList.h"
#include "NodeList.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSNodeList.h"
#include "QJSSVGAngle.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGElement.h"
#include "QJSSVGLength.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGNumber.h"
#include "QJSSVGPoint.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "QJSSVGTransform.h"
#include "SVGAngle.h"
#include "SVGElement.h"
#include "SVGLength.h"
#include "SVGSVGElement.h"
#include "SVGStringList.h"
#include "SVGTransform.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGSVGElementAttributesFunctions[27];
static bool JSSVGSVGElementAttributesFunctions_initialized = false;

static void init_JSSVGSVGElementAttributesFunctions()
{
    if (JSSVGSVGElementAttributesFunctions_initialized) return;
    JSSVGSVGElementAttributesFunctions_initialized = true;
    memset(JSSVGSVGElementAttributesFunctions, 0, sizeof(JSSVGSVGElementAttributesFunctions));
    JSSVGSVGElementAttributesFunctions[0].name = "x";
    JSSVGSVGElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[0].magic = JSSVGSVGElement::XAttrNum;
    JSSVGSVGElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[1].name = "y";
    JSSVGSVGElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[1].magic = JSSVGSVGElement::YAttrNum;
    JSSVGSVGElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[2].name = "width";
    JSSVGSVGElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[2].magic = JSSVGSVGElement::WidthAttrNum;
    JSSVGSVGElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[3].name = "height";
    JSSVGSVGElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[3].magic = JSSVGSVGElement::HeightAttrNum;
    JSSVGSVGElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[4].name = "contentScriptType";
    JSSVGSVGElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[4].magic = JSSVGSVGElement::ContentScriptTypeAttrNum;
    JSSVGSVGElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[5].name = "contentStyleType";
    JSSVGSVGElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[5].magic = JSSVGSVGElement::ContentStyleTypeAttrNum;
    JSSVGSVGElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[5].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[6].name = "viewport";
    JSSVGSVGElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[6].magic = JSSVGSVGElement::ViewportAttrNum;
    JSSVGSVGElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[7].name = "pixelUnitToMillimeterX";
    JSSVGSVGElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[7].magic = JSSVGSVGElement::PixelUnitToMillimeterXAttrNum;
    JSSVGSVGElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[8].name = "pixelUnitToMillimeterY";
    JSSVGSVGElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[8].magic = JSSVGSVGElement::PixelUnitToMillimeterYAttrNum;
    JSSVGSVGElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[9].name = "screenPixelToMillimeterX";
    JSSVGSVGElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[9].magic = JSSVGSVGElement::ScreenPixelToMillimeterXAttrNum;
    JSSVGSVGElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[10].name = "screenPixelToMillimeterY";
    JSSVGSVGElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[10].magic = JSSVGSVGElement::ScreenPixelToMillimeterYAttrNum;
    JSSVGSVGElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[11].name = "useCurrentView";
    JSSVGSVGElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[11].magic = JSSVGSVGElement::UseCurrentViewAttrNum;
    JSSVGSVGElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[11].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[12].name = "currentScale";
    JSSVGSVGElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[12].magic = JSSVGSVGElement::CurrentScaleAttrNum;
    JSSVGSVGElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[12].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[13].name = "currentTranslate";
    JSSVGSVGElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[13].magic = JSSVGSVGElement::CurrentTranslateAttrNum;
    JSSVGSVGElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[14].name = "requiredFeatures";
    JSSVGSVGElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[14].magic = JSSVGSVGElement::RequiredFeaturesAttrNum;
    JSSVGSVGElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[15].name = "requiredExtensions";
    JSSVGSVGElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[15].magic = JSSVGSVGElement::RequiredExtensionsAttrNum;
    JSSVGSVGElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[16].name = "systemLanguage";
    JSSVGSVGElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[16].magic = JSSVGSVGElement::SystemLanguageAttrNum;
    JSSVGSVGElementAttributesFunctions[16].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[17].name = "xmllang";
    JSSVGSVGElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[17].magic = JSSVGSVGElement::XmllangAttrNum;
    JSSVGSVGElementAttributesFunctions[17].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[17].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[18].name = "xmlspace";
    JSSVGSVGElementAttributesFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[18].magic = JSSVGSVGElement::XmlspaceAttrNum;
    JSSVGSVGElementAttributesFunctions[18].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[18].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
    JSSVGSVGElementAttributesFunctions[19].name = "externalResourcesRequired";
    JSSVGSVGElementAttributesFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[19].magic = JSSVGSVGElement::ExternalResourcesRequiredAttrNum;
    JSSVGSVGElementAttributesFunctions[19].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[19].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[20].name = "className";
    JSSVGSVGElementAttributesFunctions[20].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[20].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[20].magic = JSSVGSVGElement::ClassNameAttrNum;
    JSSVGSVGElementAttributesFunctions[20].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[20].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[21].name = "style";
    JSSVGSVGElementAttributesFunctions[21].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[21].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[21].magic = JSSVGSVGElement::StyleAttrNum;
    JSSVGSVGElementAttributesFunctions[21].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[21].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[22].name = "nearestViewportElement";
    JSSVGSVGElementAttributesFunctions[22].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[22].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[22].magic = JSSVGSVGElement::NearestViewportElementAttrNum;
    JSSVGSVGElementAttributesFunctions[22].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[22].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[23].name = "farthestViewportElement";
    JSSVGSVGElementAttributesFunctions[23].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[23].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[23].magic = JSSVGSVGElement::FarthestViewportElementAttrNum;
    JSSVGSVGElementAttributesFunctions[23].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[23].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[24].name = "viewBox";
    JSSVGSVGElementAttributesFunctions[24].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[24].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[24].magic = JSSVGSVGElement::ViewBoxAttrNum;
    JSSVGSVGElementAttributesFunctions[24].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[24].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[25].name = "preserveAspectRatio";
    JSSVGSVGElementAttributesFunctions[25].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[25].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[25].magic = JSSVGSVGElement::PreserveAspectRatioAttrNum;
    JSSVGSVGElementAttributesFunctions[25].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[25].u.getset.set.setter_magic = NULL;
    JSSVGSVGElementAttributesFunctions[26].name = "zoomAndPan";
    JSSVGSVGElementAttributesFunctions[26].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementAttributesFunctions[26].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSVGElementAttributesFunctions[26].magic = JSSVGSVGElement::ZoomAndPanAttrNum;
    JSSVGSVGElementAttributesFunctions[26].u.getset.get.getter_magic = JSSVGSVGElement::getValueProperty;
    JSSVGSVGElementAttributesFunctions[26].u.getset.set.setter_magic = JSSVGSVGElement::putValueProperty;
}

/* Constants table */

static JSCFunctionListEntry JSSVGSVGElementPrototypeConstantsFunctions[3];
static bool JSSVGSVGElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGSVGElementPrototypeConstantsFunctions()
{
    if (JSSVGSVGElementPrototypeConstantsFunctions_initialized) return;
    JSSVGSVGElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGSVGElementPrototypeConstantsFunctions, 0, sizeof(JSSVGSVGElementPrototypeConstantsFunctions));
    JSSVGSVGElementPrototypeConstantsFunctions[0].name = "SVG_ZOOMANDPAN_UNKNOWN";
    JSSVGSVGElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGSVGElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGSVGElement::SVG_ZOOMANDPAN_UNKNOWN;
    JSSVGSVGElementPrototypeConstantsFunctions[1].name = "SVG_ZOOMANDPAN_DISABLE";
    JSSVGSVGElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGSVGElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGSVGElement::SVG_ZOOMANDPAN_DISABLE;
    JSSVGSVGElementPrototypeConstantsFunctions[2].name = "SVG_ZOOMANDPAN_MAGNIFY";
    JSSVGSVGElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGSVGElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGSVGElement::SVG_ZOOMANDPAN_MAGNIFY;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGSVGElementPrototypeFunctions[28];
static bool JSSVGSVGElementPrototypeFunctions_initialized = false;

static void init_JSSVGSVGElementPrototypeFunctions()
{
    if (JSSVGSVGElementPrototypeFunctions_initialized) return;
    JSSVGSVGElementPrototypeFunctions_initialized = true;
    memset(JSSVGSVGElementPrototypeFunctions, 0, sizeof(JSSVGSVGElementPrototypeFunctions));
    JSSVGSVGElementPrototypeFunctions[0].name = "suspendRedraw";
    JSSVGSVGElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[0].magic = JSSVGSVGElement::SuspendRedrawFuncNum;
    JSSVGSVGElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[1].name = "unsuspendRedraw";
    JSSVGSVGElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[1].magic = JSSVGSVGElement::UnsuspendRedrawFuncNum;
    JSSVGSVGElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[2].name = "unsuspendRedrawAll";
    JSSVGSVGElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[2].magic = JSSVGSVGElement::UnsuspendRedrawAllFuncNum;
    JSSVGSVGElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[3].name = "forceRedraw";
    JSSVGSVGElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[3].magic = JSSVGSVGElement::ForceRedrawFuncNum;
    JSSVGSVGElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[4].name = "pauseAnimations";
    JSSVGSVGElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[4].magic = JSSVGSVGElement::PauseAnimationsFuncNum;
    JSSVGSVGElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[5].name = "unpauseAnimations";
    JSSVGSVGElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[5].magic = JSSVGSVGElement::UnpauseAnimationsFuncNum;
    JSSVGSVGElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[6].name = "animationsPaused";
    JSSVGSVGElementPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[6].magic = JSSVGSVGElement::AnimationsPausedFuncNum;
    JSSVGSVGElementPrototypeFunctions[6].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[7].name = "getCurrentTime";
    JSSVGSVGElementPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[7].magic = JSSVGSVGElement::GetCurrentTimeFuncNum;
    JSSVGSVGElementPrototypeFunctions[7].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[7].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[8].name = "setCurrentTime";
    JSSVGSVGElementPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[8].magic = JSSVGSVGElement::SetCurrentTimeFuncNum;
    JSSVGSVGElementPrototypeFunctions[8].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[8].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[9].name = "getIntersectionList";
    JSSVGSVGElementPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[9].magic = JSSVGSVGElement::GetIntersectionListFuncNum;
    JSSVGSVGElementPrototypeFunctions[9].u.func.length = 2;
    JSSVGSVGElementPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[9].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[10].name = "getEnclosureList";
    JSSVGSVGElementPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[10].magic = JSSVGSVGElement::GetEnclosureListFuncNum;
    JSSVGSVGElementPrototypeFunctions[10].u.func.length = 2;
    JSSVGSVGElementPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[10].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[11].name = "checkIntersection";
    JSSVGSVGElementPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[11].magic = JSSVGSVGElement::CheckIntersectionFuncNum;
    JSSVGSVGElementPrototypeFunctions[11].u.func.length = 2;
    JSSVGSVGElementPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[11].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[12].name = "checkEnclosure";
    JSSVGSVGElementPrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[12].magic = JSSVGSVGElement::CheckEnclosureFuncNum;
    JSSVGSVGElementPrototypeFunctions[12].u.func.length = 2;
    JSSVGSVGElementPrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[12].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[13].name = "deselectAll";
    JSSVGSVGElementPrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[13].magic = JSSVGSVGElement::DeselectAllFuncNum;
    JSSVGSVGElementPrototypeFunctions[13].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[13].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[14].name = "createSVGNumber";
    JSSVGSVGElementPrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[14].magic = JSSVGSVGElement::CreateSVGNumberFuncNum;
    JSSVGSVGElementPrototypeFunctions[14].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[14].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[15].name = "createSVGLength";
    JSSVGSVGElementPrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[15].magic = JSSVGSVGElement::CreateSVGLengthFuncNum;
    JSSVGSVGElementPrototypeFunctions[15].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[15].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[16].name = "createSVGAngle";
    JSSVGSVGElementPrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[16].magic = JSSVGSVGElement::CreateSVGAngleFuncNum;
    JSSVGSVGElementPrototypeFunctions[16].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[16].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[17].name = "createSVGPoint";
    JSSVGSVGElementPrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[17].magic = JSSVGSVGElement::CreateSVGPointFuncNum;
    JSSVGSVGElementPrototypeFunctions[17].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[17].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[18].name = "createSVGMatrix";
    JSSVGSVGElementPrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[18].magic = JSSVGSVGElement::CreateSVGMatrixFuncNum;
    JSSVGSVGElementPrototypeFunctions[18].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[18].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[19].name = "createSVGRect";
    JSSVGSVGElementPrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[19].magic = JSSVGSVGElement::CreateSVGRectFuncNum;
    JSSVGSVGElementPrototypeFunctions[19].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[19].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[20].name = "createSVGTransform";
    JSSVGSVGElementPrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[20].magic = JSSVGSVGElement::CreateSVGTransformFuncNum;
    JSSVGSVGElementPrototypeFunctions[20].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[20].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[21].name = "createSVGTransformFromMatrix";
    JSSVGSVGElementPrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[21].magic = JSSVGSVGElement::CreateSVGTransformFromMatrixFuncNum;
    JSSVGSVGElementPrototypeFunctions[21].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[21].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[22].name = "hasExtension";
    JSSVGSVGElementPrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[22].magic = JSSVGSVGElement::HasExtensionFuncNum;
    JSSVGSVGElementPrototypeFunctions[22].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[22].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[23].name = "getPresentationAttribute";
    JSSVGSVGElementPrototypeFunctions[23].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[23].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[23].magic = JSSVGSVGElement::GetPresentationAttributeFuncNum;
    JSSVGSVGElementPrototypeFunctions[23].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[23].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[23].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[24].name = "getBBox";
    JSSVGSVGElementPrototypeFunctions[24].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[24].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[24].magic = JSSVGSVGElement::GetBBoxFuncNum;
    JSSVGSVGElementPrototypeFunctions[24].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[24].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[24].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[25].name = "getCTM";
    JSSVGSVGElementPrototypeFunctions[25].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[25].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[25].magic = JSSVGSVGElement::GetCTMFuncNum;
    JSSVGSVGElementPrototypeFunctions[25].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[25].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[25].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[26].name = "getScreenCTM";
    JSSVGSVGElementPrototypeFunctions[26].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[26].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[26].magic = JSSVGSVGElement::GetScreenCTMFuncNum;
    JSSVGSVGElementPrototypeFunctions[26].u.func.length = 0;
    JSSVGSVGElementPrototypeFunctions[26].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[26].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
    JSSVGSVGElementPrototypeFunctions[27].name = "getTransformToElement";
    JSSVGSVGElementPrototypeFunctions[27].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSVGElementPrototypeFunctions[27].def_type = JS_DEF_CFUNC;
    JSSVGSVGElementPrototypeFunctions[27].magic = JSSVGSVGElement::GetTransformToElementFuncNum;
    JSSVGSVGElementPrototypeFunctions[27].u.func.length = 1;
    JSSVGSVGElementPrototypeFunctions[27].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSVGElementPrototypeFunctions[27].u.func.cfunc.generic_magic = JSSVGSVGElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGSVGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSVGElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSVGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSVGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGSVGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGSVGElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementAttributesFunctions, countof(JSSVGSVGElementAttributesFunctions));
    init_JSSVGSVGElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementPrototypeConstantsFunctions, countof(JSSVGSVGElementPrototypeConstantsFunctions));
    init_JSSVGSVGElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementPrototypeFunctions, countof(JSSVGSVGElementPrototypeFunctions));
}

static JSClassDef JSSVGSVGElementClassDefine;
static bool JSSVGSVGElementClassDefine_initialized = false;

static void init_JSSVGSVGElementClassDefine()
{
    if (JSSVGSVGElementClassDefine_initialized) return;
    JSSVGSVGElementClassDefine_initialized = true;
    memset(&JSSVGSVGElementClassDefine, 0, sizeof(JSSVGSVGElementClassDefine));
    JSSVGSVGElementClassDefine.class_name = "SVGSVGElement";
    JSSVGSVGElementClassDefine.finalizer = JSSVGSVGElement::finalizer;
    JSSVGSVGElementClassDefine.gc_mark = JSSVGSVGElement::mark;
}

JSClassID JSSVGSVGElement::js_class_id = 0;

void JSSVGSVGElement::init(JSContext* ctx)
{
    if (JSSVGSVGElement::js_class_id == 0) {
        init_JSSVGSVGElementClassDefine();
        JS_NewClassID(&JSSVGSVGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSVGElement::js_class_id, &JSSVGSVGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSVGElement::js_class_id, JSSVGSVGElementPrototype::self(ctx));
    }
}

JSValue JSSVGSVGElement::create(JSContext* ctx, SVGSVGElement* impl)
{
    JSSVGSVGElement::init(ctx);
    JSValue _proto = JSSVGSVGElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGSVGElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGSVGElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGSVGElement* impl = (SVGSVGElement*)JS_GetOpaque(val, JSSVGSVGElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGSVGElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGSVGElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ContentScriptTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentScriptType()).utf8().data());
        }
        case ContentStyleTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentStyleType()).utf8().data());
        }
        case ViewportAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->viewport()), imp);
        }
        case PixelUnitToMillimeterXAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->pixelUnitToMillimeterX());
        }
        case PixelUnitToMillimeterYAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->pixelUnitToMillimeterY());
        }
        case ScreenPixelToMillimeterXAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->screenPixelToMillimeterX());
        }
        case ScreenPixelToMillimeterYAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->screenPixelToMillimeterY());
        }
        case UseCurrentViewAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBool(ctx, imp->useCurrentView() ? 1 : 0);
        }
        case CurrentScaleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->currentScale());
        }
        case CurrentTranslateAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->currentTranslate()), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case NearestViewportElementAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case ViewBoxAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZoomAndPanAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            return JS_NewInt32(ctx, imp->zoomAndPan());
        }
    }
    return JS_NULL;
}

JSValue JSSVGSVGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ContentScriptTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setContentScriptType(valueToString(ctx, value));
            break;
        }
        case ContentStyleTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setContentStyleType(valueToString(ctx, value));
            break;
        }
        case UseCurrentViewAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setUseCurrentView(valueToBoolean(ctx, value));
            break;
        }
        case CurrentScaleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setCurrentScale(valueToFloat(ctx, value));
            break;
        }
        case XmllangAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
        case ZoomAndPanAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
            imp->setZoomAndPan(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSVGElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque(this_val, JSSVGSVGElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGSVGElement::SuspendRedrawFuncNum: {
            bool maxWaitMillisecondsOk;
            unsigned maxWaitMilliseconds = valueToInt32(ctx, argv[0], maxWaitMillisecondsOk);
            if (!maxWaitMillisecondsOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewInt32(ctx, imp->suspendRedraw(maxWaitMilliseconds));
            return result;
        }
        case JSSVGSVGElement::UnsuspendRedrawFuncNum: {
            ExceptionCode ec = 0;
            bool suspendHandleIdOk;
            unsigned suspendHandleId = valueToInt32(ctx, argv[0], suspendHandleIdOk);
            if (!suspendHandleIdOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->unsuspendRedraw(suspendHandleId, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::UnsuspendRedrawAllFuncNum: {
            imp->unsuspendRedrawAll();
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::ForceRedrawFuncNum: {
            imp->forceRedraw();
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::PauseAnimationsFuncNum: {
            imp->pauseAnimations();
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::UnpauseAnimationsFuncNum: {
            imp->unpauseAnimations();
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::AnimationsPausedFuncNum: {

            JSValue result = JS_NewBool(ctx, imp->animationsPaused() ? 1 : 0);
            return result;
        }
        case JSSVGSVGElement::GetCurrentTimeFuncNum: {

            JSValue result = JS_NewInt32(ctx, imp->getCurrentTime());
            return result;
        }
        case JSSVGSVGElement::SetCurrentTimeFuncNum: {
            float seconds = valueToFloat(ctx, argv[0]);
            imp->setCurrentTime(seconds);
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::GetIntersectionListFuncNum: {
            FloatRect rect = toSVGRect(argv[0]);
            SVGElement* referenceElement = toSVGElement(argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getIntersectionList(rect, referenceElement)));
            return result;
        }
        case JSSVGSVGElement::GetEnclosureListFuncNum: {
            FloatRect rect = toSVGRect(argv[0]);
            SVGElement* referenceElement = toSVGElement(argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getEnclosureList(rect, referenceElement)));
            return result;
        }
        case JSSVGSVGElement::CheckIntersectionFuncNum: {
            SVGElement* element = toSVGElement(argv[0]);
            FloatRect rect = toSVGRect(argv[1]);

            JSValue result = JS_NewBool(ctx, imp->checkIntersection(element, rect) ? 1 : 0);
            return result;
        }
        case JSSVGSVGElement::CheckEnclosureFuncNum: {
            SVGElement* element = toSVGElement(argv[0]);
            FloatRect rect = toSVGRect(argv[1]);

            JSValue result = JS_NewBool(ctx, imp->checkEnclosure(element, rect) ? 1 : 0);
            return result;
        }
        case JSSVGSVGElement::DeselectAllFuncNum: {
            imp->deselectAll();
            return JS_UNDEFINED;
        }
        case JSSVGSVGElement::CreateSVGNumberFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->createSVGNumber()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGLengthFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->createSVGLength()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGAngleFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->createSVGAngle()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGPointFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->createSVGPoint()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGMatrixFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->createSVGMatrix()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGRectFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->createSVGRect()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGTransformFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGTransform>(imp->createSVGTransform()), imp);
            return result;
        }
        case JSSVGSVGElement::CreateSVGTransformFromMatrixFuncNum: {
            AffineTransform matrix = toSVGMatrix(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGTransform>(imp->createSVGTransformFromMatrix(matrix)), imp);
            return result;
        }
        case JSSVGSVGElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGSVGElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGSVGElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGSVGElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGSVGElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGSVGElement::GetTransformToElementFuncNum: {
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
