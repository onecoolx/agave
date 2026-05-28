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
#include "QJSSVGCircleElement.h"

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
#include "SVGCircleElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGCircleElementAttributesFunctions[14];
static bool JSSVGCircleElementAttributesFunctions_initialized = false;

static void init_JSSVGCircleElementAttributesFunctions()
{
    if (JSSVGCircleElementAttributesFunctions_initialized) return;
    JSSVGCircleElementAttributesFunctions_initialized = true;
    memset(JSSVGCircleElementAttributesFunctions, 0, sizeof(JSSVGCircleElementAttributesFunctions));
    JSSVGCircleElementAttributesFunctions[0].name = "xmlspace";
    JSSVGCircleElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[0].magic = JSSVGCircleElement::XmlspaceAttrNum;
    JSSVGCircleElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGCircleElement::putValueProperty;
    JSSVGCircleElementAttributesFunctions[1].name = "requiredFeatures";
    JSSVGCircleElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[1].magic = JSSVGCircleElement::RequiredFeaturesAttrNum;
    JSSVGCircleElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[2].name = "style";
    JSSVGCircleElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[2].magic = JSSVGCircleElement::StyleAttrNum;
    JSSVGCircleElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[3].name = "cy";
    JSSVGCircleElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[3].magic = JSSVGCircleElement::CyAttrNum;
    JSSVGCircleElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[4].name = "systemLanguage";
    JSSVGCircleElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[4].magic = JSSVGCircleElement::SystemLanguageAttrNum;
    JSSVGCircleElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[5].name = "xmllang";
    JSSVGCircleElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[5].magic = JSSVGCircleElement::XmllangAttrNum;
    JSSVGCircleElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[5].u.getset.set.setter_magic = JSSVGCircleElement::putValueProperty;
    JSSVGCircleElementAttributesFunctions[6].name = "cx";
    JSSVGCircleElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[6].magic = JSSVGCircleElement::CxAttrNum;
    JSSVGCircleElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[7].name = "className";
    JSSVGCircleElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[7].magic = JSSVGCircleElement::ClassNameAttrNum;
    JSSVGCircleElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[8].name = "externalResourcesRequired";
    JSSVGCircleElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[8].magic = JSSVGCircleElement::ExternalResourcesRequiredAttrNum;
    JSSVGCircleElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[9].name = "requiredExtensions";
    JSSVGCircleElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[9].magic = JSSVGCircleElement::RequiredExtensionsAttrNum;
    JSSVGCircleElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[10].name = "r";
    JSSVGCircleElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[10].magic = JSSVGCircleElement::RAttrNum;
    JSSVGCircleElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[11].name = "transform";
    JSSVGCircleElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[11].magic = JSSVGCircleElement::TransformAttrNum;
    JSSVGCircleElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[12].name = "nearestViewportElement";
    JSSVGCircleElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[12].magic = JSSVGCircleElement::NearestViewportElementAttrNum;
    JSSVGCircleElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGCircleElementAttributesFunctions[13].name = "farthestViewportElement";
    JSSVGCircleElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGCircleElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGCircleElementAttributesFunctions[13].magic = JSSVGCircleElement::FarthestViewportElementAttrNum;
    JSSVGCircleElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGCircleElement::getValueProperty;
    JSSVGCircleElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGCircleElementPrototypeFunctions[6];
static bool JSSVGCircleElementPrototypeFunctions_initialized = false;

static void init_JSSVGCircleElementPrototypeFunctions()
{
    if (JSSVGCircleElementPrototypeFunctions_initialized) return;
    JSSVGCircleElementPrototypeFunctions_initialized = true;
    memset(JSSVGCircleElementPrototypeFunctions, 0, sizeof(JSSVGCircleElementPrototypeFunctions));
    JSSVGCircleElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGCircleElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[0].magic = JSSVGCircleElement::GetPresentationAttributeFuncNum;
    JSSVGCircleElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGCircleElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
    JSSVGCircleElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGCircleElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[1].magic = JSSVGCircleElement::HasExtensionFuncNum;
    JSSVGCircleElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGCircleElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
    JSSVGCircleElementPrototypeFunctions[2].name = "getCTM";
    JSSVGCircleElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[2].magic = JSSVGCircleElement::GetCTMFuncNum;
    JSSVGCircleElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGCircleElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
    JSSVGCircleElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGCircleElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[3].magic = JSSVGCircleElement::GetScreenCTMFuncNum;
    JSSVGCircleElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGCircleElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
    JSSVGCircleElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGCircleElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[4].magic = JSSVGCircleElement::GetTransformToElementFuncNum;
    JSSVGCircleElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGCircleElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
    JSSVGCircleElementPrototypeFunctions[5].name = "getBBox";
    JSSVGCircleElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGCircleElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGCircleElementPrototypeFunctions[5].magic = JSSVGCircleElement::GetBBoxFuncNum;
    JSSVGCircleElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGCircleElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGCircleElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGCircleElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGCircleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGCircleElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGCircleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGCircleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGCircleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGCircleElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCircleElementAttributesFunctions, countof(JSSVGCircleElementAttributesFunctions));
    init_JSSVGCircleElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGCircleElementPrototypeFunctions, countof(JSSVGCircleElementPrototypeFunctions));
}

static JSClassDef JSSVGCircleElementClassDefine;
static bool JSSVGCircleElementClassDefine_initialized = false;

static void init_JSSVGCircleElementClassDefine()
{
    if (JSSVGCircleElementClassDefine_initialized) return;
    JSSVGCircleElementClassDefine_initialized = true;
    memset(&JSSVGCircleElementClassDefine, 0, sizeof(JSSVGCircleElementClassDefine));
    JSSVGCircleElementClassDefine.class_name = "SVGCircleElement";
    JSSVGCircleElementClassDefine.finalizer = JSSVGCircleElement::finalizer;
    JSSVGCircleElementClassDefine.gc_mark = JSSVGCircleElement::mark;
}

JSClassID JSSVGCircleElement::js_class_id = 0;

void JSSVGCircleElement::init(JSContext* ctx)
{
    if (JSSVGCircleElement::js_class_id == 0) {
        init_JSSVGCircleElementClassDefine();
        JS_NewClassID(&JSSVGCircleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGCircleElement::js_class_id, &JSSVGCircleElementClassDefine);
        JS_SetClassProto(ctx, JSSVGCircleElement::js_class_id, JSSVGCircleElementPrototype::self(ctx));
    }
}

JSValue JSSVGCircleElement::create(JSContext* ctx, SVGCircleElement* impl)
{
    JSSVGCircleElement::init(ctx);
    JSValue _proto = JSSVGCircleElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGCircleElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGCircleElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGCircleElement* impl = (SVGCircleElement*)JS_GetOpaque(val, JSSVGCircleElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGCircleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGCircleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CxAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case CyAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGCircleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGCircleElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGCircleElement* imp = (SVGCircleElement*)JS_GetOpaque(this_val, JSSVGCircleElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGCircleElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGCircleElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGCircleElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGCircleElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGCircleElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGCircleElement::GetTransformToElementFuncNum: {
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
