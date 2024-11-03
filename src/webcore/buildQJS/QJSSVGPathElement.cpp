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

static const JSCFunctionListEntry JSSVGPathElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGPathElement::getValueProperty, JSSVGPathElement::putValueProperty, JSSVGPathElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("pathLength", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::PathLengthAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("pathSegList", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::PathSegListAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGPathElement::getValueProperty, JSSVGPathElement::putValueProperty, JSSVGPathElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("normalizedPathSegList", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::NormalizedPathSegListAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedPathSegList", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::AnimatedPathSegListAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedNormalizedPathSegList", JSSVGPathElement::getValueProperty, NULL, JSSVGPathElement::AnimatedNormalizedPathSegListAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPathElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("createSVGPathSegMovetoAbs", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegMovetoAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("getTotalLength", 0, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetTotalLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegMovetoRel", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegMovetoRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoCubicRel", 6, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoCubicRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegClosePath", 0, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegClosePathFuncNum),
    JS_CFUNC_MAGIC_DEF("getPathSegAtLength", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetPathSegAtLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoQuadraticSmoothRel", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoHorizontalAbs", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoHorizontalAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoCubicAbs", 6, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoCubicAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoCubicSmoothRel", 4, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothRelFuncNum),
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoQuadraticAbs", 4, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("getPointAtLength", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetPointAtLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegArcAbs", 7, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegArcAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoRel", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoRelFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoQuadraticSmoothAbs", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticSmoothAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoCubicSmoothAbs", 4, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoCubicSmoothAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoAbs", 2, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegCurvetoQuadraticRel", 4, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegCurvetoQuadraticRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegArcRel", 7, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegArcRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoHorizontalRel", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoHorizontalRelFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoVerticalAbs", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoVerticalAbsFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPathSegLinetoVerticalRel", 1, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::CreateSVGPathSegLinetoVerticalRelFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetBBoxFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGPathElementPrototypeFunction::callAsFunction, JSSVGPathElement::GetCTMFuncNum)
};

JSValue JSSVGPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathElementAttributesFunctions, countof(JSSVGPathElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathElementPrototypeFunctions, countof(JSSVGPathElementPrototypeFunctions));
}

static JSClassDef JSSVGPathElementClassDefine = 
{
    "SVGPathElement",
    .finalizer = JSSVGPathElement::finalizer,
    .gc_mark = JSSVGPathElement::mark,
};

JSClassID JSSVGPathElement::js_class_id = 0;

void JSSVGPathElement::init(JSContext* ctx)
{
    if (JSSVGPathElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathElement::js_class_id, &JSSVGPathElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPathElement::js_class_id, JSSVGPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGPathElement::create(JSContext* ctx, SVGPathElement* impl)
{
    JSSVGPathElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathElementPrototype::self(ctx), JSSVGPathElement::js_class_id);
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
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pathLengthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->pathSegList()), imp);
        }
        case NormalizedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->normalizedPathSegList()), imp);
        }
        case AnimatedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPathSegList()), imp);
        }
        case AnimatedNormalizedPathSegListAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedNormalizedPathSegList()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPathElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPathElement* imp = (SVGPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGPathElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPathElement::GetTotalLengthFuncNum: {

            JSValue result = JS_NewBigUint64(ctx, imp->getTotalLength());
            return result;
        }
        case JSSVGPathElement::GetPointAtLengthFuncNum: {
            float distance = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->getPointAtLength(distance)), imp);
            return result;
        }
        case JSSVGPathElement::GetPathSegAtLengthFuncNum: {
            float distance = valueToFloat(ctx, argv[0]);

            JSValue result = JS_NewBigUint64(ctx, imp->getPathSegAtLength(distance));
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
