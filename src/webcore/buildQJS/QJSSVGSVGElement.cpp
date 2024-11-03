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

static const JSCFunctionListEntry JSSVGSVGElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("screenPixelToMillimeterY", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ScreenPixelToMillimeterYAttrNum),
    JS_CGETSET_MAGIC_DEF("viewBox", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ViewBoxAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("pixelUnitToMillimeterX", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::PixelUnitToMillimeterXAttrNum),
    JS_CGETSET_MAGIC_DEF("pixelUnitToMillimeterY", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::PixelUnitToMillimeterYAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("zoomAndPan", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::ZoomAndPanAttrNum),
    JS_CGETSET_MAGIC_DEF("screenPixelToMillimeterX", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ScreenPixelToMillimeterXAttrNum),
    JS_CGETSET_MAGIC_DEF("contentStyleType", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::ContentStyleTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("viewport", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ViewportAttrNum),
    JS_CGETSET_MAGIC_DEF("currentTranslate", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::CurrentTranslateAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("contentScriptType", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::ContentScriptTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("useCurrentView", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::UseCurrentViewAttrNum),
    JS_CGETSET_MAGIC_DEF("currentScale", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::CurrentScaleAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGSVGElement::getValueProperty, JSSVGSVGElement::putValueProperty, JSSVGSVGElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGSVGElement::getValueProperty, NULL, JSSVGSVGElement::PreserveAspectRatioAttrNum)
};

/* Functions table */

static const JSCFunctionListEntry JSSVGSVGElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_UNKNOWN", JSSVGSVGElementPrototype::getValueProperty, NULL, SVGSVGElement::SVG_ZOOMANDPAN_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_DISABLE", JSSVGSVGElementPrototype::getValueProperty, NULL, SVGSVGElement::SVG_ZOOMANDPAN_DISABLE),
    JS_CGETSET_MAGIC_DEF("SVG_ZOOMANDPAN_MAGNIFY", JSSVGSVGElementPrototype::getValueProperty, NULL, SVGSVGElement::SVG_ZOOMANDPAN_MAGNIFY)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGSVGElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("checkIntersection", 2, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CheckIntersectionFuncNum),
    JS_CFUNC_MAGIC_DEF("getEnclosureList", 2, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetEnclosureListFuncNum),
    JS_CFUNC_MAGIC_DEF("unpauseAnimations", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::UnpauseAnimationsFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGTransformFromMatrix", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGTransformFromMatrixFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGLength", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGLengthFuncNum),
    JS_CFUNC_MAGIC_DEF("setCurrentTime", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::SetCurrentTimeFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getCurrentTime", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetCurrentTimeFuncNum),
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("unsuspendRedraw", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::UnsuspendRedrawFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGMatrix", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGMatrixFuncNum),
    JS_CFUNC_MAGIC_DEF("animationsPaused", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::AnimationsPausedFuncNum),
    JS_CFUNC_MAGIC_DEF("unsuspendRedrawAll", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::UnsuspendRedrawAllFuncNum),
    JS_CFUNC_MAGIC_DEF("pauseAnimations", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::PauseAnimationsFuncNum),
    JS_CFUNC_MAGIC_DEF("suspendRedraw", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::SuspendRedrawFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getIntersectionList", 2, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetIntersectionListFuncNum),
    JS_CFUNC_MAGIC_DEF("forceRedraw", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::ForceRedrawFuncNum),
    JS_CFUNC_MAGIC_DEF("checkEnclosure", 2, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CheckEnclosureFuncNum),
    JS_CFUNC_MAGIC_DEF("deselectAll", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::DeselectAllFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGNumber", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGNumberFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGAngle", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGAngleFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGPoint", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGPointFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGRect", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGRectFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGTransform", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::CreateSVGTransformFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGSVGElementPrototypeFunction::callAsFunction, JSSVGSVGElement::GetBBoxFuncNum)
};

JSValue JSSVGSVGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSVGElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSVGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSVGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGSVGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementAttributesFunctions, countof(JSSVGSVGElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementPrototypeConstantsFunctions, countof(JSSVGSVGElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSVGElementPrototypeFunctions, countof(JSSVGSVGElementPrototypeFunctions));
}

JSValue JSSVGSVGElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGSVGElementClassDefine = 
{
    "SVGSVGElement",
    .finalizer = JSSVGSVGElement::finalizer,
    .gc_mark = JSSVGSVGElement::mark,
};

JSClassID JSSVGSVGElement::js_class_id = 0;

void JSSVGSVGElement::init(JSContext* ctx)
{
    if (JSSVGSVGElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGSVGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSVGElement::js_class_id, &JSSVGSVGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSVGElement::js_class_id, JSSVGSVGElementPrototype::self(ctx));
    }
}

JSValue JSSVGSVGElement::create(JSContext* ctx, SVGSVGElement* impl)
{
    JSSVGSVGElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGSVGElementPrototype::self(ctx), JSSVGSVGElement::js_class_id);
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
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ContentScriptTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentScriptType()).utf8().data());
        }
        case ContentStyleTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->contentStyleType()).utf8().data());
        }
        case ViewportAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->viewport()), imp);
        }
        case PixelUnitToMillimeterXAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->pixelUnitToMillimeterX());
        }
        case PixelUnitToMillimeterYAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->pixelUnitToMillimeterY());
        }
        case ScreenPixelToMillimeterXAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenPixelToMillimeterX());
        }
        case ScreenPixelToMillimeterYAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->screenPixelToMillimeterY());
        }
        case UseCurrentViewAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBool(ctx, imp->useCurrentView() ? 1 : 0);
        }
        case CurrentScaleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->currentScale());
        }
        case CurrentTranslateAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp->currentTranslate()), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case NearestViewportElementAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case ViewBoxAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZoomAndPanAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->zoomAndPan());
        }
    }
    return JS_NULL;
}

JSValue JSSVGSVGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ContentScriptTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setContentScriptType(valueToString(ctx, value));
            break;
        }
        case ContentStyleTypeAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setContentStyleType(valueToString(ctx, value));
            break;
        }
        case UseCurrentViewAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setUseCurrentView(valueToBoolean(ctx, value));
            break;
        }
        case CurrentScaleAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setCurrentScale(valueToFloat(ctx, value));
            break;
        }
        case XmllangAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
        case ZoomAndPanAttrNum: {
            SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
            imp->setZoomAndPan(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSVGElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSVGElement* imp = (SVGSVGElement*)JS_GetOpaque2(ctx, this_val, JSSVGSVGElement::js_class_id);
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

            JSValue result = JS_NewBigUint64(ctx, imp->suspendRedraw(maxWaitMilliseconds));
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

            JSValue result = JS_NewBigUint64(ctx, imp->getCurrentTime());
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
