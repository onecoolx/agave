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
#include "QJSSVGRectElement.h"

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
#include "SVGRectElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGRectElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("height", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("ry", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::RyAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("rx", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::RxAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGRectElement::getValueProperty, JSSVGRectElement::putValueProperty, JSSVGRectElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGRectElement::getValueProperty, JSSVGRectElement::putValueProperty, JSSVGRectElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGRectElement::getValueProperty, NULL, JSSVGRectElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGRectElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGRectElementPrototypeFunction::callAsFunction, JSSVGRectElement::GetBBoxFuncNum)
};

JSValue JSSVGRectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRectElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGRectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGRectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectElementAttributesFunctions, countof(JSSVGRectElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectElementPrototypeFunctions, countof(JSSVGRectElementPrototypeFunctions));
}

static JSClassDef JSSVGRectElementClassDefine = 
{
    "SVGRectElement",
    .finalizer = JSSVGRectElement::finalizer,
    .gc_mark = JSSVGRectElement::mark,
};

JSClassID JSSVGRectElement::js_class_id = 0;

void JSSVGRectElement::init(JSContext* ctx)
{
    if (JSSVGRectElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGRectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRectElement::js_class_id, &JSSVGRectElementClassDefine);
        JS_SetClassProto(ctx, JSSVGRectElement::js_class_id, JSSVGRectElementPrototype::self(ctx));
    }
}

JSValue JSSVGRectElement::create(JSContext* ctx, SVGRectElement* impl)
{
    JSSVGRectElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGRectElementPrototype::self(ctx), JSSVGRectElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGRectElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGRectElement* impl = (SVGRectElement*)JS_GetOpaque(val, JSSVGRectElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGRectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGRectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RxAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RyAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->ryAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGRectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGRectElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque2(ctx, this_val, JSSVGRectElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGRectElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGRectElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGRectElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGRectElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGRectElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGRectElement::GetTransformToElementFuncNum: {
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
