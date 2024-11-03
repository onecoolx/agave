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
#include "QJSSVGDefsElement.h"

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
#include "SVGDefsElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGDefsElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGDefsElement::getValueProperty, JSSVGDefsElement::putValueProperty, JSSVGDefsElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGDefsElement::getValueProperty, JSSVGDefsElement::putValueProperty, JSSVGDefsElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGDefsElement::getValueProperty, NULL, JSSVGDefsElement::TransformAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGDefsElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGDefsElementPrototypeFunction::callAsFunction, JSSVGDefsElement::GetBBoxFuncNum)
};

JSValue JSSVGDefsElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGDefsElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGDefsElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGDefsElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGDefsElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDefsElementAttributesFunctions, countof(JSSVGDefsElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDefsElementPrototypeFunctions, countof(JSSVGDefsElementPrototypeFunctions));
}

static JSClassDef JSSVGDefsElementClassDefine = 
{
    "SVGDefsElement",
    .finalizer = JSSVGDefsElement::finalizer,
    .gc_mark = JSSVGDefsElement::mark,
};

JSClassID JSSVGDefsElement::js_class_id = 0;

void JSSVGDefsElement::init(JSContext* ctx)
{
    if (JSSVGDefsElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGDefsElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGDefsElement::js_class_id, &JSSVGDefsElementClassDefine);
        JS_SetClassProto(ctx, JSSVGDefsElement::js_class_id, JSSVGDefsElementPrototype::self(ctx));
    }
}

JSValue JSSVGDefsElement::create(JSContext* ctx, SVGDefsElement* impl)
{
    JSSVGDefsElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGDefsElementPrototype::self(ctx), JSSVGDefsElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGDefsElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGDefsElement* impl = (SVGDefsElement*)JS_GetOpaque(val, JSSVGDefsElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGDefsElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGDefsElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGDefsElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGDefsElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque2(ctx, this_val, JSSVGDefsElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGDefsElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGDefsElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGDefsElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGDefsElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGDefsElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGDefsElement::GetTransformToElementFuncNum: {
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
