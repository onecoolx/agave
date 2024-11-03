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
#include "QJSSVGCircleElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGCircleElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGCircleElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGCircleElement::getValueProperty, JSSVGCircleElement::putValueProperty, JSSVGCircleElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("cy", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::CyAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGCircleElement::getValueProperty, JSSVGCircleElement::putValueProperty, JSSVGCircleElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("cx", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::CxAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("r", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::RAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGCircleElement::getValueProperty, NULL, JSSVGCircleElement::FarthestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGCircleElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGCircleElementPrototypeFunction::callAsFunction, JSSVGCircleElement::GetBBoxFuncNum)
};

JSValue JSSVGCircleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGCircleElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGCircleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGCircleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGCircleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCircleElementAttributesFunctions, countof(JSSVGCircleElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCircleElementPrototypeFunctions, countof(JSSVGCircleElementPrototypeFunctions));
}

static JSClassDef JSSVGCircleElementClassDefine = 
{
    "SVGCircleElement",
    .finalizer = JSSVGCircleElement::finalizer,
    .gc_mark = JSSVGCircleElement::mark,
};

JSClassID JSSVGCircleElement::js_class_id = 0;

void JSSVGCircleElement::init(JSContext* ctx)
{
    if (JSSVGCircleElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGCircleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGCircleElement::js_class_id, &JSSVGCircleElementClassDefine);
        JS_SetClassProto(ctx, JSSVGCircleElement::js_class_id, JSSVGCircleElementPrototype::self(ctx));
    }
}

JSValue JSSVGCircleElement::create(JSContext* ctx, SVGCircleElement* impl)
{
    JSSVGCircleElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGCircleElementPrototype::self(ctx), JSSVGCircleElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGCircleElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGCircleElement* impl = (SVGCircleElement*)JS_GetOpaque(val, JSSVGCircleElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGCircleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGCircleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CxAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case CyAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGCircleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGCircleElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque2(ctx, this_val, JSSVGCircleElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGCircleElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGCircleElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGCircleElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGCircleElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGCircleElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGCircleElement::GetTransformToElementFuncNum: {
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
