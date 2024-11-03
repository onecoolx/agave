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
#include "QJSSVGForeignObjectElement.h"

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
#include "SVGForeignObjectElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGForeignObjectElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGForeignObjectElement::getValueProperty, JSSVGForeignObjectElement::putValueProperty, JSSVGForeignObjectElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGForeignObjectElement::getValueProperty, JSSVGForeignObjectElement::putValueProperty, JSSVGForeignObjectElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGForeignObjectElement::getValueProperty, NULL, JSSVGForeignObjectElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGForeignObjectElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGForeignObjectElementPrototypeFunction::callAsFunction, JSSVGForeignObjectElement::GetBBoxFuncNum)
};

JSValue JSSVGForeignObjectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGForeignObjectElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGForeignObjectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGForeignObjectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGForeignObjectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGForeignObjectElementAttributesFunctions, countof(JSSVGForeignObjectElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGForeignObjectElementPrototypeFunctions, countof(JSSVGForeignObjectElementPrototypeFunctions));
}

static JSClassDef JSSVGForeignObjectElementClassDefine = 
{
    "SVGForeignObjectElement",
    .finalizer = JSSVGForeignObjectElement::finalizer,
    .gc_mark = JSSVGForeignObjectElement::mark,
};

JSClassID JSSVGForeignObjectElement::js_class_id = 0;

void JSSVGForeignObjectElement::init(JSContext* ctx)
{
    if (JSSVGForeignObjectElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGForeignObjectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGForeignObjectElement::js_class_id, &JSSVGForeignObjectElementClassDefine);
        JS_SetClassProto(ctx, JSSVGForeignObjectElement::js_class_id, JSSVGForeignObjectElementPrototype::self(ctx));
    }
}

JSValue JSSVGForeignObjectElement::create(JSContext* ctx, SVGForeignObjectElement* impl)
{
    JSSVGForeignObjectElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGForeignObjectElementPrototype::self(ctx), JSSVGForeignObjectElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGForeignObjectElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGForeignObjectElement* impl = (SVGForeignObjectElement*)JS_GetOpaque(val, JSSVGForeignObjectElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGForeignObjectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGForeignObjectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGForeignObjectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGForeignObjectElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque2(ctx, this_val, JSSVGForeignObjectElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGForeignObjectElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGForeignObjectElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGForeignObjectElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetTransformToElementFuncNum: {
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
