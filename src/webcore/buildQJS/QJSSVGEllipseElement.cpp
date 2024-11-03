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
#include "QJSSVGEllipseElement.h"

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
#include "SVGElement.h"
#include "SVGEllipseElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGEllipseElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGEllipseElement::getValueProperty, JSSVGEllipseElement::putValueProperty, JSSVGEllipseElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("rx", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::RxAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGEllipseElement::getValueProperty, JSSVGEllipseElement::putValueProperty, JSSVGEllipseElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("cy", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::CyAttrNum),
    JS_CGETSET_MAGIC_DEF("ry", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::RyAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("cx", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::CxAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGEllipseElement::getValueProperty, NULL, JSSVGEllipseElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGEllipseElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGEllipseElementPrototypeFunction::callAsFunction, JSSVGEllipseElement::GetBBoxFuncNum)
};

JSValue JSSVGEllipseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGEllipseElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGEllipseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGEllipseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGEllipseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGEllipseElementAttributesFunctions, countof(JSSVGEllipseElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGEllipseElementPrototypeFunctions, countof(JSSVGEllipseElementPrototypeFunctions));
}

static JSClassDef JSSVGEllipseElementClassDefine = 
{
    "SVGEllipseElement",
    .finalizer = JSSVGEllipseElement::finalizer,
    .gc_mark = JSSVGEllipseElement::mark,
};

JSClassID JSSVGEllipseElement::js_class_id = 0;

void JSSVGEllipseElement::init(JSContext* ctx)
{
    if (JSSVGEllipseElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGEllipseElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGEllipseElement::js_class_id, &JSSVGEllipseElementClassDefine);
        JS_SetClassProto(ctx, JSSVGEllipseElement::js_class_id, JSSVGEllipseElementPrototype::self(ctx));
    }
}

JSValue JSSVGEllipseElement::create(JSContext* ctx, SVGEllipseElement* impl)
{
    JSSVGEllipseElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGEllipseElementPrototype::self(ctx), JSSVGEllipseElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGEllipseElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGEllipseElement* impl = (SVGEllipseElement*)JS_GetOpaque(val, JSSVGEllipseElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGEllipseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGEllipseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CxAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case CyAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RxAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RyAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->ryAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGEllipseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGEllipseElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque2(ctx, this_val, JSSVGEllipseElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGEllipseElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGEllipseElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGEllipseElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetTransformToElementFuncNum: {
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
