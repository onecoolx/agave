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
#include "QJSSVGPolygonElement.h"

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
#include "SVGPolygonElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPolygonElementAttributesFunctions[13];
static bool JSSVGPolygonElementAttributesFunctions_initialized = false;

static void init_JSSVGPolygonElementAttributesFunctions()
{
    if (JSSVGPolygonElementAttributesFunctions_initialized) return;
    JSSVGPolygonElementAttributesFunctions_initialized = true;
    memset(JSSVGPolygonElementAttributesFunctions, 0, sizeof(JSSVGPolygonElementAttributesFunctions));
    JSSVGPolygonElementAttributesFunctions[0].name = "requiredFeatures";
    JSSVGPolygonElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[0].magic = JSSVGPolygonElement::RequiredFeaturesAttrNum;
    JSSVGPolygonElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[1].name = "requiredExtensions";
    JSSVGPolygonElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[1].magic = JSSVGPolygonElement::RequiredExtensionsAttrNum;
    JSSVGPolygonElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[2].name = "systemLanguage";
    JSSVGPolygonElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[2].magic = JSSVGPolygonElement::SystemLanguageAttrNum;
    JSSVGPolygonElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[3].name = "xmllang";
    JSSVGPolygonElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[3].magic = JSSVGPolygonElement::XmllangAttrNum;
    JSSVGPolygonElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[3].u.getset.set.setter_magic = JSSVGPolygonElement::putValueProperty;
    JSSVGPolygonElementAttributesFunctions[4].name = "xmlspace";
    JSSVGPolygonElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[4].magic = JSSVGPolygonElement::XmlspaceAttrNum;
    JSSVGPolygonElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGPolygonElement::putValueProperty;
    JSSVGPolygonElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGPolygonElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[5].magic = JSSVGPolygonElement::ExternalResourcesRequiredAttrNum;
    JSSVGPolygonElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[6].name = "className";
    JSSVGPolygonElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[6].magic = JSSVGPolygonElement::ClassNameAttrNum;
    JSSVGPolygonElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[7].name = "style";
    JSSVGPolygonElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[7].magic = JSSVGPolygonElement::StyleAttrNum;
    JSSVGPolygonElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[8].name = "transform";
    JSSVGPolygonElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[8].magic = JSSVGPolygonElement::TransformAttrNum;
    JSSVGPolygonElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[9].name = "nearestViewportElement";
    JSSVGPolygonElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[9].magic = JSSVGPolygonElement::NearestViewportElementAttrNum;
    JSSVGPolygonElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGPolygonElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[10].magic = JSSVGPolygonElement::FarthestViewportElementAttrNum;
    JSSVGPolygonElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[11].name = "points";
    JSSVGPolygonElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[11].magic = JSSVGPolygonElement::PointsAttrNum;
    JSSVGPolygonElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGPolygonElementAttributesFunctions[12].name = "animatedPoints";
    JSSVGPolygonElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPolygonElementAttributesFunctions[12].magic = JSSVGPolygonElement::AnimatedPointsAttrNum;
    JSSVGPolygonElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGPolygonElement::getValueProperty;
    JSSVGPolygonElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPolygonElementPrototypeFunctions[6];
static bool JSSVGPolygonElementPrototypeFunctions_initialized = false;

static void init_JSSVGPolygonElementPrototypeFunctions()
{
    if (JSSVGPolygonElementPrototypeFunctions_initialized) return;
    JSSVGPolygonElementPrototypeFunctions_initialized = true;
    memset(JSSVGPolygonElementPrototypeFunctions, 0, sizeof(JSSVGPolygonElementPrototypeFunctions));
    JSSVGPolygonElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGPolygonElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[0].magic = JSSVGPolygonElement::HasExtensionFuncNum;
    JSSVGPolygonElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGPolygonElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
    JSSVGPolygonElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGPolygonElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[1].magic = JSSVGPolygonElement::GetPresentationAttributeFuncNum;
    JSSVGPolygonElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGPolygonElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
    JSSVGPolygonElementPrototypeFunctions[2].name = "getBBox";
    JSSVGPolygonElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[2].magic = JSSVGPolygonElement::GetBBoxFuncNum;
    JSSVGPolygonElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGPolygonElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
    JSSVGPolygonElementPrototypeFunctions[3].name = "getCTM";
    JSSVGPolygonElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[3].magic = JSSVGPolygonElement::GetCTMFuncNum;
    JSSVGPolygonElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGPolygonElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
    JSSVGPolygonElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGPolygonElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[4].magic = JSSVGPolygonElement::GetScreenCTMFuncNum;
    JSSVGPolygonElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGPolygonElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
    JSSVGPolygonElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGPolygonElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPolygonElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGPolygonElementPrototypeFunctions[5].magic = JSSVGPolygonElement::GetTransformToElementFuncNum;
    JSSVGPolygonElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGPolygonElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPolygonElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGPolygonElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGPolygonElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPolygonElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPolygonElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPolygonElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPolygonElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPolygonElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolygonElementAttributesFunctions, countof(JSSVGPolygonElementAttributesFunctions));
    init_JSSVGPolygonElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPolygonElementPrototypeFunctions, countof(JSSVGPolygonElementPrototypeFunctions));
}

static JSClassDef JSSVGPolygonElementClassDefine;
static bool JSSVGPolygonElementClassDefine_initialized = false;

static void init_JSSVGPolygonElementClassDefine()
{
    if (JSSVGPolygonElementClassDefine_initialized) return;
    JSSVGPolygonElementClassDefine_initialized = true;
    memset(&JSSVGPolygonElementClassDefine, 0, sizeof(JSSVGPolygonElementClassDefine));
    JSSVGPolygonElementClassDefine.class_name = "SVGPolygonElement";
    JSSVGPolygonElementClassDefine.finalizer = JSSVGPolygonElement::finalizer;
    JSSVGPolygonElementClassDefine.gc_mark = JSSVGPolygonElement::mark;
}

JSClassID JSSVGPolygonElement::js_class_id = 0;

void JSSVGPolygonElement::init(JSContext* ctx)
{
    if (JSSVGPolygonElement::js_class_id == 0) {
        init_JSSVGPolygonElementClassDefine();
        JS_NewClassID(&JSSVGPolygonElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPolygonElement::js_class_id, &JSSVGPolygonElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPolygonElement::js_class_id, JSSVGPolygonElementPrototype::self(ctx));
    }
}

JSValue JSSVGPolygonElement::create(JSContext* ctx, SVGPolygonElement* impl)
{
    JSSVGPolygonElement::init(ctx);
    JSValue _proto = JSSVGPolygonElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPolygonElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPolygonElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPolygonElement* impl = (SVGPolygonElement*)JS_GetOpaque(val, JSSVGPolygonElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPolygonElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPolygonElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RequiredFeaturesAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
        case PointsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->points()), imp);
        }
        case AnimatedPointsAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPoints()), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPolygonElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPolygonElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPolygonElement* imp = (SVGPolygonElement*)JS_GetOpaque(this_val, JSSVGPolygonElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPolygonElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPolygonElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGPolygonElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGPolygonElement::GetTransformToElementFuncNum: {
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
