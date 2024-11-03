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
#include "QJSSVGClipPathElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGClipPathElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGClipPathElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("clipPathUnits", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::ClipPathUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGClipPathElement::getValueProperty, JSSVGClipPathElement::putValueProperty, JSSVGClipPathElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGClipPathElement::getValueProperty, JSSVGClipPathElement::putValueProperty, JSSVGClipPathElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGClipPathElement::getValueProperty, NULL, JSSVGClipPathElement::FarthestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGClipPathElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGClipPathElementPrototypeFunction::callAsFunction, JSSVGClipPathElement::GetBBoxFuncNum)
};

JSValue JSSVGClipPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGClipPathElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGClipPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGClipPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGClipPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGClipPathElementAttributesFunctions, countof(JSSVGClipPathElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGClipPathElementPrototypeFunctions, countof(JSSVGClipPathElementPrototypeFunctions));
}

static JSClassDef JSSVGClipPathElementClassDefine = 
{
    "SVGClipPathElement",
    .finalizer = JSSVGClipPathElement::finalizer,
    .gc_mark = JSSVGClipPathElement::mark,
};

JSClassID JSSVGClipPathElement::js_class_id = 0;

void JSSVGClipPathElement::init(JSContext* ctx)
{
    if (JSSVGClipPathElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGClipPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGClipPathElement::js_class_id, &JSSVGClipPathElementClassDefine);
        JS_SetClassProto(ctx, JSSVGClipPathElement::js_class_id, JSSVGClipPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGClipPathElement::create(JSContext* ctx, SVGClipPathElement* impl)
{
    JSSVGClipPathElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGClipPathElementPrototype::self(ctx), JSSVGClipPathElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGClipPathElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGClipPathElement* impl = (SVGClipPathElement*)JS_GetOpaque(val, JSSVGClipPathElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGClipPathElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGClipPathElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ClipPathUnitsAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->clipPathUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGClipPathElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGClipPathElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque2(ctx, this_val, JSSVGClipPathElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGClipPathElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGClipPathElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGClipPathElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetTransformToElementFuncNum: {
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
