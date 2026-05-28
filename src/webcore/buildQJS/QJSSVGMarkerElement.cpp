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
#include "QJSSVGMarkerElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAngle.h"
#include "QJSSVGAnimatedAngle.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "SVGMarkerElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGMarkerElementAttributesFunctions[15];
static bool JSSVGMarkerElementAttributesFunctions_initialized = false;

static void init_JSSVGMarkerElementAttributesFunctions()
{
    if (JSSVGMarkerElementAttributesFunctions_initialized) return;
    JSSVGMarkerElementAttributesFunctions_initialized = true;
    memset(JSSVGMarkerElementAttributesFunctions, 0, sizeof(JSSVGMarkerElementAttributesFunctions));
    JSSVGMarkerElementAttributesFunctions[0].name = "refX";
    JSSVGMarkerElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[0].magic = JSSVGMarkerElement::RefXAttrNum;
    JSSVGMarkerElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[1].name = "xmlspace";
    JSSVGMarkerElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[1].magic = JSSVGMarkerElement::XmlspaceAttrNum;
    JSSVGMarkerElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGMarkerElement::putValueProperty;
    JSSVGMarkerElementAttributesFunctions[2].name = "orientAngle";
    JSSVGMarkerElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[2].magic = JSSVGMarkerElement::OrientAngleAttrNum;
    JSSVGMarkerElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[3].name = "externalResourcesRequired";
    JSSVGMarkerElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[3].magic = JSSVGMarkerElement::ExternalResourcesRequiredAttrNum;
    JSSVGMarkerElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[4].name = "className";
    JSSVGMarkerElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[4].magic = JSSVGMarkerElement::ClassNameAttrNum;
    JSSVGMarkerElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[5].name = "refY";
    JSSVGMarkerElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[5].magic = JSSVGMarkerElement::RefYAttrNum;
    JSSVGMarkerElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[6].name = "markerUnits";
    JSSVGMarkerElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[6].magic = JSSVGMarkerElement::MarkerUnitsAttrNum;
    JSSVGMarkerElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[7].name = "style";
    JSSVGMarkerElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[7].magic = JSSVGMarkerElement::StyleAttrNum;
    JSSVGMarkerElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[8].name = "constructor";
    JSSVGMarkerElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[8].magic = JSSVGMarkerElement::ConstructorAttrNum;
    JSSVGMarkerElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[9].name = "markerWidth";
    JSSVGMarkerElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[9].magic = JSSVGMarkerElement::MarkerWidthAttrNum;
    JSSVGMarkerElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[10].name = "markerHeight";
    JSSVGMarkerElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[10].magic = JSSVGMarkerElement::MarkerHeightAttrNum;
    JSSVGMarkerElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[11].name = "orientType";
    JSSVGMarkerElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[11].magic = JSSVGMarkerElement::OrientTypeAttrNum;
    JSSVGMarkerElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[12].name = "xmllang";
    JSSVGMarkerElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[12].magic = JSSVGMarkerElement::XmllangAttrNum;
    JSSVGMarkerElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[12].u.getset.set.setter_magic = JSSVGMarkerElement::putValueProperty;
    JSSVGMarkerElementAttributesFunctions[13].name = "viewBox";
    JSSVGMarkerElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[13].magic = JSSVGMarkerElement::ViewBoxAttrNum;
    JSSVGMarkerElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementAttributesFunctions[14].name = "preserveAspectRatio";
    JSSVGMarkerElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementAttributesFunctions[14].magic = JSSVGMarkerElement::PreserveAspectRatioAttrNum;
    JSSVGMarkerElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGMarkerElement::getValueProperty;
    JSSVGMarkerElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

class JSSVGMarkerElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGMarkerElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGMarkerElementConstructorFunctions[6];
static bool JSSVGMarkerElementConstructorFunctions_initialized = false;

static void init_JSSVGMarkerElementConstructorFunctions()
{
    if (JSSVGMarkerElementConstructorFunctions_initialized) return;
    JSSVGMarkerElementConstructorFunctions_initialized = true;
    memset(JSSVGMarkerElementConstructorFunctions, 0, sizeof(JSSVGMarkerElementConstructorFunctions));
    JSSVGMarkerElementConstructorFunctions[0].name = "SVG_MARKERUNITS_UNKNOWN";
    JSSVGMarkerElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[0].magic = SVGMarkerElement::SVG_MARKERUNITS_UNKNOWN;
    JSSVGMarkerElementConstructorFunctions[0].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementConstructorFunctions[1].name = "SVG_MARKERUNITS_USERSPACEONUSE";
    JSSVGMarkerElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[1].magic = SVGMarkerElement::SVG_MARKERUNITS_USERSPACEONUSE;
    JSSVGMarkerElementConstructorFunctions[1].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementConstructorFunctions[2].name = "SVG_MARKER_ORIENT_UNKNOWN";
    JSSVGMarkerElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[2].magic = SVGMarkerElement::SVG_MARKER_ORIENT_UNKNOWN;
    JSSVGMarkerElementConstructorFunctions[2].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementConstructorFunctions[3].name = "SVG_MARKERUNITS_STROKEWIDTH";
    JSSVGMarkerElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[3].magic = SVGMarkerElement::SVG_MARKERUNITS_STROKEWIDTH;
    JSSVGMarkerElementConstructorFunctions[3].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementConstructorFunctions[4].name = "SVG_MARKER_ORIENT_ANGLE";
    JSSVGMarkerElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[4].magic = SVGMarkerElement::SVG_MARKER_ORIENT_ANGLE;
    JSSVGMarkerElementConstructorFunctions[4].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementConstructorFunctions[5].name = "SVG_MARKER_ORIENT_AUTO";
    JSSVGMarkerElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementConstructorFunctions[5].magic = SVGMarkerElement::SVG_MARKER_ORIENT_AUTO;
    JSSVGMarkerElementConstructorFunctions[5].u.getset.get.getter_magic = JSSVGMarkerElementConstructor::getValueProperty;
    JSSVGMarkerElementConstructorFunctions[5].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGMarkerElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGMarkerElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGMarkerElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGMarkerElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGMarkerElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGMarkerElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementConstructorFunctions, countof(JSSVGMarkerElementConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGMarkerElementPrototypeConstantsFunctions[6];
static bool JSSVGMarkerElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGMarkerElementPrototypeConstantsFunctions()
{
    if (JSSVGMarkerElementPrototypeConstantsFunctions_initialized) return;
    JSSVGMarkerElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGMarkerElementPrototypeConstantsFunctions, 0, sizeof(JSSVGMarkerElementPrototypeConstantsFunctions));
    JSSVGMarkerElementPrototypeConstantsFunctions[0].name = "SVG_MARKERUNITS_UNKNOWN";
    JSSVGMarkerElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[0].magic = SVGMarkerElement::SVG_MARKERUNITS_UNKNOWN;
    JSSVGMarkerElementPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementPrototypeConstantsFunctions[1].name = "SVG_MARKERUNITS_USERSPACEONUSE";
    JSSVGMarkerElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[1].magic = SVGMarkerElement::SVG_MARKERUNITS_USERSPACEONUSE;
    JSSVGMarkerElementPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementPrototypeConstantsFunctions[2].name = "SVG_MARKER_ORIENT_UNKNOWN";
    JSSVGMarkerElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[2].magic = SVGMarkerElement::SVG_MARKER_ORIENT_UNKNOWN;
    JSSVGMarkerElementPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementPrototypeConstantsFunctions[3].name = "SVG_MARKERUNITS_STROKEWIDTH";
    JSSVGMarkerElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[3].magic = SVGMarkerElement::SVG_MARKERUNITS_STROKEWIDTH;
    JSSVGMarkerElementPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementPrototypeConstantsFunctions[4].name = "SVG_MARKER_ORIENT_ANGLE";
    JSSVGMarkerElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[4].magic = SVGMarkerElement::SVG_MARKER_ORIENT_ANGLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGMarkerElementPrototypeConstantsFunctions[5].name = "SVG_MARKER_ORIENT_AUTO";
    JSSVGMarkerElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMarkerElementPrototypeConstantsFunctions[5].magic = SVGMarkerElement::SVG_MARKER_ORIENT_AUTO;
    JSSVGMarkerElementPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSSVGMarkerElementPrototype::getValueProperty;
    JSSVGMarkerElementPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGMarkerElementPrototypeFunctions[3];
static bool JSSVGMarkerElementPrototypeFunctions_initialized = false;

static void init_JSSVGMarkerElementPrototypeFunctions()
{
    if (JSSVGMarkerElementPrototypeFunctions_initialized) return;
    JSSVGMarkerElementPrototypeFunctions_initialized = true;
    memset(JSSVGMarkerElementPrototypeFunctions, 0, sizeof(JSSVGMarkerElementPrototypeFunctions));
    JSSVGMarkerElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGMarkerElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGMarkerElementPrototypeFunctions[0].magic = JSSVGMarkerElement::GetPresentationAttributeFuncNum;
    JSSVGMarkerElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGMarkerElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMarkerElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGMarkerElementPrototypeFunction::callAsFunction;
    JSSVGMarkerElementPrototypeFunctions[1].name = "setOrientToAuto";
    JSSVGMarkerElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGMarkerElementPrototypeFunctions[1].magic = JSSVGMarkerElement::SetOrientToAutoFuncNum;
    JSSVGMarkerElementPrototypeFunctions[1].u.func.length = 0;
    JSSVGMarkerElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMarkerElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGMarkerElementPrototypeFunction::callAsFunction;
    JSSVGMarkerElementPrototypeFunctions[2].name = "setOrientToAngle";
    JSSVGMarkerElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMarkerElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGMarkerElementPrototypeFunctions[2].magic = JSSVGMarkerElement::SetOrientToAngleFuncNum;
    JSSVGMarkerElementPrototypeFunctions[2].u.func.length = 1;
    JSSVGMarkerElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMarkerElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGMarkerElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGMarkerElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMarkerElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGMarkerElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMarkerElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGMarkerElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGMarkerElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementAttributesFunctions, countof(JSSVGMarkerElementAttributesFunctions));
    init_JSSVGMarkerElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementPrototypeConstantsFunctions, countof(JSSVGMarkerElementPrototypeConstantsFunctions));
    init_JSSVGMarkerElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMarkerElementPrototypeFunctions, countof(JSSVGMarkerElementPrototypeFunctions));
}

JSValue JSSVGMarkerElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGMarkerElementClassDefine;
static bool JSSVGMarkerElementClassDefine_initialized = false;

static void init_JSSVGMarkerElementClassDefine()
{
    if (JSSVGMarkerElementClassDefine_initialized) return;
    JSSVGMarkerElementClassDefine_initialized = true;
    memset(&JSSVGMarkerElementClassDefine, 0, sizeof(JSSVGMarkerElementClassDefine));
    JSSVGMarkerElementClassDefine.class_name = "SVGMarkerElement";
    JSSVGMarkerElementClassDefine.finalizer = JSSVGMarkerElement::finalizer;
    JSSVGMarkerElementClassDefine.gc_mark = JSSVGMarkerElement::mark;
}

JSClassID JSSVGMarkerElement::js_class_id = 0;

void JSSVGMarkerElement::init(JSContext* ctx)
{
    if (JSSVGMarkerElement::js_class_id == 0) {
        init_JSSVGMarkerElementClassDefine();
        JS_NewClassID(&JSSVGMarkerElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMarkerElement::js_class_id, &JSSVGMarkerElementClassDefine);
        JS_SetConstructor(ctx, JSSVGMarkerElementConstructor::self(ctx), JSSVGMarkerElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGMarkerElement::js_class_id, JSSVGMarkerElementPrototype::self(ctx));
    }
}

JSValue JSSVGMarkerElement::create(JSContext* ctx, SVGMarkerElement* impl)
{
    JSSVGMarkerElement::init(ctx);
    JSValue _proto = JSSVGMarkerElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGMarkerElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGMarkerElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGMarkerElement* impl = (SVGMarkerElement*)JS_GetOpaque(val, JSSVGMarkerElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGMarkerElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGMarkerElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RefXAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->refXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RefYAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->refYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerUnitsAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->markerUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerWidthAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->markerWidthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MarkerHeightAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->markerHeightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OrientTypeAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->orientTypeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OrientAngleAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedAngle> obj = imp->orientAngleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XmllangAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGMarkerElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGMarkerElement::getConstructor(JSContext *ctx)
{
    return JSSVGMarkerElementConstructor::self(ctx);
}

JSValue JSSVGMarkerElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGMarkerElement* imp = (SVGMarkerElement*)JS_GetOpaque(this_val, JSSVGMarkerElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGMarkerElement::SetOrientToAutoFuncNum: {
            imp->setOrientToAuto();
            return JS_UNDEFINED;
        }
        case JSSVGMarkerElement::SetOrientToAngleFuncNum: {
            SVGAngle* angle = toSVGAngle(argv[0]);
            imp->setOrientToAngle(angle);
            return JS_UNDEFINED;
        }
        case JSSVGMarkerElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
