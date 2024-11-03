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
#include "QJSSVGPolygonElement.h"

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
#include "SVGPolygonElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPolygonElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("transform", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGPolygonElement::getValueProperty, JSSVGPolygonElement::putValueProperty, JSSVGPolygonElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGPolygonElement::getValueProperty, JSSVGPolygonElement::putValueProperty, JSSVGPolygonElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("points", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::PointsAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedPoints", JSSVGPolygonElement::getValueProperty, NULL, JSSVGPolygonElement::AnimatedPointsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPolygonElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGPolygonElementPrototypeFunction::callAsFunction, JSSVGPolygonElement::GetBBoxFuncNum)
};

JSValue JSSVGPolygonElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPolygonElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPolygonElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPolygonElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPolygonElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolygonElementAttributesFunctions, countof(JSSVGPolygonElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolygonElementPrototypeFunctions, countof(JSSVGPolygonElementPrototypeFunctions));
}

static JSClassDef JSSVGPolygonElementClassDefine = 
{
    "SVGPolygonElement",
    .finalizer = JSSVGPolygonElement::finalizer,
    .gc_mark = JSSVGPolygonElement::mark,
};

JSClassID JSSVGPolygonElement::js_class_id = 0;

void JSSVGPolygonElement::init(JSContext* ctx)
{
    if (JSSVGPolygonElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGPolygonElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPolygonElement::js_class_id, &JSSVGPolygonElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPolygonElement::js_class_id, JSSVGPolygonElementPrototype::self(ctx));
    }
}

JSValue JSSVGPolygonElement::create(JSContext* ctx, SVGPolygonElement* impl)
{
    JSSVGPolygonElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPolygonElementPrototype::self(ctx), JSSVGPolygonElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPolygonElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPolygonElement* impl = (SVGPolygonElement*)JS_GetOpaque(val, JSSVGPolygonElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPolygonElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPolygonElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PointsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->points()), imp);
        }
        case AnimatedPointsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPoints()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPolygonElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPolygonElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque2(ctx, this_val, JSSVGPolygonElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPolygonElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPolygonElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGPolygonElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetTransformToElementFuncNum: {
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
