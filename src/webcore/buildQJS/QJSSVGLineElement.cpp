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
#include "QJSSVGLineElement.h"

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
#include "SVGLineElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGLineElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGLineElement::getValueProperty, JSSVGLineElement::putValueProperty, JSSVGLineElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGLineElement::getValueProperty, JSSVGLineElement::putValueProperty, JSSVGLineElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("x1", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::X1AttrNum),
    JS_CGETSET_MAGIC_DEF("x2", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::X2AttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("y1", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::Y1AttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::FarthestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("y2", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::Y2AttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGLineElement::getValueProperty, NULL, JSSVGLineElement::NearestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGLineElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::HasExtensionFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGLineElementPrototypeFunction::callAsFunction, JSSVGLineElement::GetBBoxFuncNum)
};

JSValue JSSVGLineElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLineElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGLineElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLineElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGLineElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLineElementAttributesFunctions, countof(JSSVGLineElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLineElementPrototypeFunctions, countof(JSSVGLineElementPrototypeFunctions));
}

static JSClassDef JSSVGLineElementClassDefine = 
{
    "SVGLineElement",
    .finalizer = JSSVGLineElement::finalizer,
    .gc_mark = JSSVGLineElement::mark,
};

JSClassID JSSVGLineElement::js_class_id = 0;

void JSSVGLineElement::init(JSContext* ctx)
{
    if (JSSVGLineElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGLineElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLineElement::js_class_id, &JSSVGLineElementClassDefine);
        JS_SetClassProto(ctx, JSSVGLineElement::js_class_id, JSSVGLineElementPrototype::self(ctx));
    }
}

JSValue JSSVGLineElement::create(JSContext* ctx, SVGLineElement* impl)
{
    JSSVGLineElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGLineElementPrototype::self(ctx), JSSVGLineElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGLineElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGLineElement* impl = (SVGLineElement*)JS_GetOpaque(val, JSSVGLineElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGLineElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGLineElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case X1AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y1AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case X2AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y2AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGLineElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGLineElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque2(ctx, this_val, JSSVGLineElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGLineElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGLineElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGLineElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGLineElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGLineElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGLineElement::GetTransformToElementFuncNum: {
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
