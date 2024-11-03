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
#include "QJSSVGAElement.h"

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
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGAElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("transform", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGAElement::getValueProperty, JSSVGAElement::putValueProperty, JSSVGAElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGAElement::getValueProperty, JSSVGAElement::putValueProperty, JSSVGAElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGAElement::getValueProperty, NULL, JSSVGAElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGAElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGAElementPrototypeFunction::callAsFunction, JSSVGAElement::GetBBoxFuncNum)
};

JSValue JSSVGAElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGAElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAElementAttributesFunctions, countof(JSSVGAElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAElementPrototypeFunctions, countof(JSSVGAElementPrototypeFunctions));
}

static JSClassDef JSSVGAElementClassDefine = 
{
    "SVGAElement",
    .finalizer = JSSVGAElement::finalizer,
    .gc_mark = JSSVGAElement::mark,
};

JSClassID JSSVGAElement::js_class_id = 0;

void JSSVGAElement::init(JSContext* ctx)
{
    if (JSSVGAElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGAElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAElement::js_class_id, &JSSVGAElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAElement::js_class_id, JSSVGAElementPrototype::self(ctx));
    }
}

JSValue JSSVGAElement::create(JSContext* ctx, SVGAElement* impl)
{
    JSSVGAElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAElementPrototype::self(ctx), JSSVGAElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAElement* impl = (SVGAElement*)JS_GetOpaque(val, JSSVGAElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGAElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TargetAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->targetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGAElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGAElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGAElement* imp = (SVGAElement*)JS_GetOpaque2(ctx, this_val, JSSVGAElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGAElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGAElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGAElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGAElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGAElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGAElement::GetTransformToElementFuncNum: {
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
