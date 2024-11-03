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
#include "QJSSVGPolylineElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGPointList.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGPointList.h"
#include "SVGPolylineElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPolylineElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("transform", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGPolylineElement::getValueProperty, JSSVGPolylineElement::putValueProperty, JSSVGPolylineElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGPolylineElement::getValueProperty, JSSVGPolylineElement::putValueProperty, JSSVGPolylineElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("points", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::PointsAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedPoints", JSSVGPolylineElement::getValueProperty, NULL, JSSVGPolylineElement::AnimatedPointsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPolylineElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGPolylineElementPrototypeFunction::callAsFunction, JSSVGPolylineElement::GetBBoxFuncNum)
};

JSValue JSSVGPolylineElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPolylineElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPolylineElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPolylineElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPolylineElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolylineElementAttributesFunctions, countof(JSSVGPolylineElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolylineElementPrototypeFunctions, countof(JSSVGPolylineElementPrototypeFunctions));
}

static JSClassDef JSSVGPolylineElementClassDefine = 
{
    "SVGPolylineElement",
    .finalizer = JSSVGPolylineElement::finalizer,
    .gc_mark = JSSVGPolylineElement::mark,
};

JSClassID JSSVGPolylineElement::js_class_id = 0;

void JSSVGPolylineElement::init(JSContext* ctx)
{
    if (JSSVGPolylineElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGPolylineElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPolylineElement::js_class_id, &JSSVGPolylineElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPolylineElement::js_class_id, JSSVGPolylineElementPrototype::self(ctx));
    }
}

JSValue JSSVGPolylineElement::create(JSContext* ctx, SVGPolylineElement* impl)
{
    JSSVGPolylineElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPolylineElementPrototype::self(ctx), JSSVGPolylineElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPolylineElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPolylineElement* impl = (SVGPolylineElement*)JS_GetOpaque(val, JSSVGPolylineElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPolylineElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPolylineElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PointsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->points()), imp);
        }
        case AnimatedPointsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPoints()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPolylineElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPolylineElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolylineElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPolylineElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPolylineElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGPolylineElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetTransformToElementFuncNum: {
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
