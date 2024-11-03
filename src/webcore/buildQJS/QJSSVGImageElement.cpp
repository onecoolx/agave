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
#include "QJSSVGImageElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGImageElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGImageElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("height", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::PreserveAspectRatioAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGImageElement::getValueProperty, JSSVGImageElement::putValueProperty, JSSVGImageElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGImageElement::getValueProperty, JSSVGImageElement::putValueProperty, JSSVGImageElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGImageElement::getValueProperty, NULL, JSSVGImageElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGImageElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGImageElementPrototypeFunction::callAsFunction, JSSVGImageElement::GetBBoxFuncNum)
};

JSValue JSSVGImageElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGImageElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGImageElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGImageElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGImageElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGImageElementAttributesFunctions, countof(JSSVGImageElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGImageElementPrototypeFunctions, countof(JSSVGImageElementPrototypeFunctions));
}

static JSClassDef JSSVGImageElementClassDefine = 
{
    "SVGImageElement",
    .finalizer = JSSVGImageElement::finalizer,
    .gc_mark = JSSVGImageElement::mark,
};

JSClassID JSSVGImageElement::js_class_id = 0;

void JSSVGImageElement::init(JSContext* ctx)
{
    if (JSSVGImageElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGImageElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGImageElement::js_class_id, &JSSVGImageElementClassDefine);
        JS_SetClassProto(ctx, JSSVGImageElement::js_class_id, JSSVGImageElementPrototype::self(ctx));
    }
}

JSValue JSSVGImageElement::create(JSContext* ctx, SVGImageElement* impl)
{
    JSSVGImageElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGImageElementPrototype::self(ctx), JSSVGImageElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGImageElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGImageElement* impl = (SVGImageElement*)JS_GetOpaque(val, JSSVGImageElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGImageElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGImageElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGImageElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGImageElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque2(ctx, this_val, JSSVGImageElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGImageElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGImageElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGImageElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGImageElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGImageElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGImageElement::GetTransformToElementFuncNum: {
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
