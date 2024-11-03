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
#include "QJSSVGUseElement.h"

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
#include "QJSSVGElementInstance.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGElementInstance.h"
#include "SVGStringList.h"
#include "SVGUseElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGUseElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGUseElement::getValueProperty, JSSVGUseElement::putValueProperty, JSSVGUseElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGUseElement::getValueProperty, JSSVGUseElement::putValueProperty, JSSVGUseElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("instanceRoot", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::InstanceRootAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedInstanceRoot", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::AnimatedInstanceRootAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGUseElement::getValueProperty, NULL, JSSVGUseElement::FarthestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGUseElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGUseElementPrototypeFunction::callAsFunction, JSSVGUseElement::GetBBoxFuncNum)
};

JSValue JSSVGUseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGUseElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGUseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGUseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGUseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUseElementAttributesFunctions, countof(JSSVGUseElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUseElementPrototypeFunctions, countof(JSSVGUseElementPrototypeFunctions));
}

static JSClassDef JSSVGUseElementClassDefine = 
{
    "SVGUseElement",
    .finalizer = JSSVGUseElement::finalizer,
    .gc_mark = JSSVGUseElement::mark,
};

JSClassID JSSVGUseElement::js_class_id = 0;

void JSSVGUseElement::init(JSContext* ctx)
{
    if (JSSVGUseElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGUseElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGUseElement::js_class_id, &JSSVGUseElementClassDefine);
        JS_SetClassProto(ctx, JSSVGUseElement::js_class_id, JSSVGUseElementPrototype::self(ctx));
    }
}

JSValue JSSVGUseElement::create(JSContext* ctx, SVGUseElement* impl)
{
    JSSVGUseElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGUseElementPrototype::self(ctx), JSSVGUseElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGUseElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGUseElement* impl = (SVGUseElement*)JS_GetOpaque(val, JSSVGUseElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGUseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGUseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case InstanceRootAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->instanceRoot()));
        }
        case AnimatedInstanceRootAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedInstanceRoot()));
        }
        case HrefAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGUseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGUseElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque2(ctx, this_val, JSSVGUseElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGUseElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGUseElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGUseElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGUseElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGUseElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGUseElement::GetTransformToElementFuncNum: {
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
