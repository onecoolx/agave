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
#include "QJSSVGSwitchElement.h"

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
#include "SVGStringList.h"
#include "SVGSwitchElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGSwitchElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGSwitchElement::getValueProperty, JSSVGSwitchElement::putValueProperty, JSSVGSwitchElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGSwitchElement::getValueProperty, JSSVGSwitchElement::putValueProperty, JSSVGSwitchElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGSwitchElement::getValueProperty, NULL, JSSVGSwitchElement::TransformAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGSwitchElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGSwitchElementPrototypeFunction::callAsFunction, JSSVGSwitchElement::GetBBoxFuncNum)
};

JSValue JSSVGSwitchElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSwitchElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSwitchElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSwitchElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGSwitchElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSwitchElementAttributesFunctions, countof(JSSVGSwitchElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSwitchElementPrototypeFunctions, countof(JSSVGSwitchElementPrototypeFunctions));
}

static JSClassDef JSSVGSwitchElementClassDefine = 
{
    "SVGSwitchElement",
    .finalizer = JSSVGSwitchElement::finalizer,
    .gc_mark = JSSVGSwitchElement::mark,
};

JSClassID JSSVGSwitchElement::js_class_id = 0;

void JSSVGSwitchElement::init(JSContext* ctx)
{
    if (JSSVGSwitchElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGSwitchElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSwitchElement::js_class_id, &JSSVGSwitchElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSwitchElement::js_class_id, JSSVGSwitchElementPrototype::self(ctx));
    }
}

JSValue JSSVGSwitchElement::create(JSContext* ctx, SVGSwitchElement* impl)
{
    JSSVGSwitchElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGSwitchElementPrototype::self(ctx), JSSVGSwitchElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGSwitchElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGSwitchElement* impl = (SVGSwitchElement*)JS_GetOpaque(val, JSSVGSwitchElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGSwitchElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGSwitchElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGSwitchElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSwitchElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque2(ctx, this_val, JSSVGSwitchElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGSwitchElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGSwitchElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGSwitchElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGSwitchElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGSwitchElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGSwitchElement::GetTransformToElementFuncNum: {
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
