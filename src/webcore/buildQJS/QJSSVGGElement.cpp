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
#include "QJSSVGGElement.h"

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
#include "SVGElement.h"
#include "SVGGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGGElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGGElement::getValueProperty, JSSVGGElement::putValueProperty, JSSVGGElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGGElement::getValueProperty, JSSVGGElement::putValueProperty, JSSVGGElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGGElement::getValueProperty, NULL, JSSVGGElement::TransformAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGGElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGGElementPrototypeFunction::callAsFunction, JSSVGGElement::GetBBoxFuncNum)
};

JSValue JSSVGGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGGElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGElementAttributesFunctions, countof(JSSVGGElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGElementPrototypeFunctions, countof(JSSVGGElementPrototypeFunctions));
}

static JSClassDef JSSVGGElementClassDefine = 
{
    "SVGGElement",
    .finalizer = JSSVGGElement::finalizer,
    .gc_mark = JSSVGGElement::mark,
};

JSClassID JSSVGGElement::js_class_id = 0;

void JSSVGGElement::init(JSContext* ctx)
{
    if (JSSVGGElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGGElement::js_class_id, &JSSVGGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGGElement::js_class_id, JSSVGGElementPrototype::self(ctx));
    }
}

JSValue JSSVGGElement::create(JSContext* ctx, SVGGElement* impl)
{
    JSSVGGElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGGElementPrototype::self(ctx), JSSVGGElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGGElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGGElement* impl = (SVGGElement*)JS_GetOpaque(val, JSSVGGElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGGElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGGElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGGElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGGElement* imp = (SVGGElement*)JS_GetOpaque2(ctx, this_val, JSSVGGElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGGElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGGElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGGElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGGElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGGElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGGElement::GetTransformToElementFuncNum: {
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
