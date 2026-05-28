/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGPolylineElement.h"

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
#include "QJSSVGPointList.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGPointList.h"
#include "SVGPolylineElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPolylineElementAttributesFunctions[13];
static bool JSSVGPolylineElementAttributesFunctions_initialized = false;

static void init_JSSVGPolylineElementAttributesFunctions()
{
    if (JSSVGPolylineElementAttributesFunctions_initialized) return;
    JSSVGPolylineElementAttributesFunctions_initialized = true;
    memset(JSSVGPolylineElementAttributesFunctions, 0, sizeof(JSSVGPolylineElementAttributesFunctions));
    JSSVGPolylineElementAttributesFunctions[0].name = "requiredFeatures";
    JSSVGPolylineElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[0].magic = JSSVGPolylineElement::RequiredFeaturesAttrNum;
    JSSVGPolylineElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[1].name = "requiredExtensions";
    JSSVGPolylineElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[1].magic = JSSVGPolylineElement::RequiredExtensionsAttrNum;
    JSSVGPolylineElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[2].name = "systemLanguage";
    JSSVGPolylineElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[2].magic = JSSVGPolylineElement::SystemLanguageAttrNum;
    JSSVGPolylineElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[3].name = "xmllang";
    JSSVGPolylineElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[3].magic = JSSVGPolylineElement::XmllangAttrNum;
    JSSVGPolylineElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPolylineElement::putValueProperty;
    JSSVGPolylineElementAttributesFunctions[4].name = "xmlspace";
    JSSVGPolylineElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[4].magic = JSSVGPolylineElement::XmlspaceAttrNum;
    JSSVGPolylineElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPolylineElement::putValueProperty;
    JSSVGPolylineElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGPolylineElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[5].magic = JSSVGPolylineElement::ExternalResourcesRequiredAttrNum;
    JSSVGPolylineElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[6].name = "className";
    JSSVGPolylineElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[6].magic = JSSVGPolylineElement::ClassNameAttrNum;
    JSSVGPolylineElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[7].name = "style";
    JSSVGPolylineElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[7].magic = JSSVGPolylineElement::StyleAttrNum;
    JSSVGPolylineElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[8].name = "transform";
    JSSVGPolylineElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[8].magic = JSSVGPolylineElement::TransformAttrNum;
    JSSVGPolylineElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[9].name = "nearestViewportElement";
    JSSVGPolylineElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[9].magic = JSSVGPolylineElement::NearestViewportElementAttrNum;
    JSSVGPolylineElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGPolylineElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[10].magic = JSSVGPolylineElement::FarthestViewportElementAttrNum;
    JSSVGPolylineElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[11].name = "points";
    JSSVGPolylineElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[11].magic = JSSVGPolylineElement::PointsAttrNum;
    JSSVGPolylineElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGPolylineElementAttributesFunctions[12].name = "animatedPoints";
    JSSVGPolylineElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolylineElementAttributesFunctions[12].magic = JSSVGPolylineElement::AnimatedPointsAttrNum;
    JSSVGPolylineElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGPolylineElement::getValueProperty;
    JSSVGPolylineElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPolylineElementPrototypeFunctions[6];
static bool JSSVGPolylineElementPrototypeFunctions_initialized = false;

static void init_JSSVGPolylineElementPrototypeFunctions()
{
    if (JSSVGPolylineElementPrototypeFunctions_initialized) return;
    JSSVGPolylineElementPrototypeFunctions_initialized = true;
    memset(JSSVGPolylineElementPrototypeFunctions, 0, sizeof(JSSVGPolylineElementPrototypeFunctions));
    JSSVGPolylineElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGPolylineElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[0].magic = JSSVGPolylineElement::HasExtensionFuncNum;
    JSSVGPolylineElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGPolylineElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
    JSSVGPolylineElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGPolylineElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[1].magic = JSSVGPolylineElement::GetPresentationAttributeFuncNum;
    JSSVGPolylineElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGPolylineElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
    JSSVGPolylineElementPrototypeFunctions[2].name = "getBBox";
    JSSVGPolylineElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[2].magic = JSSVGPolylineElement::GetBBoxFuncNum;
    JSSVGPolylineElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGPolylineElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
    JSSVGPolylineElementPrototypeFunctions[3].name = "getCTM";
    JSSVGPolylineElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[3].magic = JSSVGPolylineElement::GetCTMFuncNum;
    JSSVGPolylineElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGPolylineElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
    JSSVGPolylineElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGPolylineElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[4].magic = JSSVGPolylineElement::GetScreenCTMFuncNum;
    JSSVGPolylineElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGPolylineElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
    JSSVGPolylineElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGPolylineElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolylineElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGPolylineElementPrototypeFunctions[5].magic = JSSVGPolylineElement::GetTransformToElementFuncNum;
    JSSVGPolylineElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGPolylineElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolylineElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGPolylineElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGPolylineElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPolylineElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPolylineElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPolylineElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPolylineElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPolylineElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolylineElementAttributesFunctions, countof(JSSVGPolylineElementAttributesFunctions));
    init_JSSVGPolylineElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolylineElementPrototypeFunctions, countof(JSSVGPolylineElementPrototypeFunctions));
}

static JSClassDef JSSVGPolylineElementClassDefine;
static bool JSSVGPolylineElementClassDefine_initialized = false;

static void init_JSSVGPolylineElementClassDefine()
{
    if (JSSVGPolylineElementClassDefine_initialized) return;
    JSSVGPolylineElementClassDefine_initialized = true;
    memset(&JSSVGPolylineElementClassDefine, 0, sizeof(JSSVGPolylineElementClassDefine));
    JSSVGPolylineElementClassDefine.class_name = "SVGPolylineElement";
    JSSVGPolylineElementClassDefine.finalizer = JSSVGPolylineElement::finalizer;
    JSSVGPolylineElementClassDefine.gc_mark = JSSVGPolylineElement::mark;
}

JSClassID JSSVGPolylineElement::js_class_id = 0;

void JSSVGPolylineElement::init(JSContext* ctx)
{
    if (JSSVGPolylineElement::js_class_id == 0) {
        init_JSSVGPolylineElementClassDefine();
        JS_NewClassID(&JSSVGPolylineElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPolylineElement::js_class_id, &JSSVGPolylineElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPolylineElement::js_class_id, JSSVGPolylineElementPrototype::self(ctx));
    }
}

JSValue JSSVGPolylineElement::create(JSContext* ctx, SVGPolylineElement* impl)
{
    JSSVGPolylineElement::init(ctx);
    JSValue _proto = JSSVGPolylineElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPolylineElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPolylineElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPolylineElement* impl = (SVGPolylineElement*)JS_GetOpaque(val, JSSVGPolylineElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPolylineElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPolylineElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PointsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->points()), imp);
        }
        case AnimatedPointsAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPoints()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPolylineElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPolylineElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPolylineElement* imp = (SVGPolylineElement*)JS_GetOpaque(this_val, JSSVGPolylineElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPolylineElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPolylineElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGPolylineElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGPolylineElement::GetTransformToElementFuncNum: {
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
