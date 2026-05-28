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

static JSCFunctionListEntry JSSVGAElementAttributesFunctions[13];
static bool JSSVGAElementAttributesFunctions_initialized = false;

static void init_JSSVGAElementAttributesFunctions()
{
    if (JSSVGAElementAttributesFunctions_initialized) return;
    JSSVGAElementAttributesFunctions_initialized = true;
    memset(JSSVGAElementAttributesFunctions, 0, sizeof(JSSVGAElementAttributesFunctions));
    JSSVGAElementAttributesFunctions[0].name = "transform";
    JSSVGAElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[0].magic = JSSVGAElement::TransformAttrNum;
    JSSVGAElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[1].name = "xmlspace";
    JSSVGAElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[1].magic = JSSVGAElement::XmlspaceAttrNum;
    JSSVGAElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGAElement::putValueProperty;
    JSSVGAElementAttributesFunctions[2].name = "requiredFeatures";
    JSSVGAElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[2].magic = JSSVGAElement::RequiredFeaturesAttrNum;
    JSSVGAElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[3].name = "farthestViewportElement";
    JSSVGAElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[3].magic = JSSVGAElement::FarthestViewportElementAttrNum;
    JSSVGAElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[4].name = "target";
    JSSVGAElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[4].magic = JSSVGAElement::TargetAttrNum;
    JSSVGAElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[5].name = "style";
    JSSVGAElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[5].magic = JSSVGAElement::StyleAttrNum;
    JSSVGAElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[6].name = "systemLanguage";
    JSSVGAElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[6].magic = JSSVGAElement::SystemLanguageAttrNum;
    JSSVGAElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[7].name = "href";
    JSSVGAElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[7].magic = JSSVGAElement::HrefAttrNum;
    JSSVGAElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[8].name = "externalResourcesRequired";
    JSSVGAElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[8].magic = JSSVGAElement::ExternalResourcesRequiredAttrNum;
    JSSVGAElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[9].name = "requiredExtensions";
    JSSVGAElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[9].magic = JSSVGAElement::RequiredExtensionsAttrNum;
    JSSVGAElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[10].name = "xmllang";
    JSSVGAElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[10].magic = JSSVGAElement::XmllangAttrNum;
    JSSVGAElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[10].u.getset.set.setter_magic = JSSVGAElement::putValueProperty;
    JSSVGAElementAttributesFunctions[11].name = "className";
    JSSVGAElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[11].magic = JSSVGAElement::ClassNameAttrNum;
    JSSVGAElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGAElementAttributesFunctions[12].name = "nearestViewportElement";
    JSSVGAElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGAElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGAElementAttributesFunctions[12].magic = JSSVGAElement::NearestViewportElementAttrNum;
    JSSVGAElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGAElement::getValueProperty;
    JSSVGAElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGAElementPrototypeFunctions[6];
static bool JSSVGAElementPrototypeFunctions_initialized = false;

static void init_JSSVGAElementPrototypeFunctions()
{
    if (JSSVGAElementPrototypeFunctions_initialized) return;
    JSSVGAElementPrototypeFunctions_initialized = true;
    memset(JSSVGAElementPrototypeFunctions, 0, sizeof(JSSVGAElementPrototypeFunctions));
    JSSVGAElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGAElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[0].magic = JSSVGAElement::GetPresentationAttributeFuncNum;
    JSSVGAElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGAElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
    JSSVGAElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGAElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[1].magic = JSSVGAElement::HasExtensionFuncNum;
    JSSVGAElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGAElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
    JSSVGAElementPrototypeFunctions[2].name = "getCTM";
    JSSVGAElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[2].magic = JSSVGAElement::GetCTMFuncNum;
    JSSVGAElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGAElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
    JSSVGAElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGAElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[3].magic = JSSVGAElement::GetScreenCTMFuncNum;
    JSSVGAElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGAElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
    JSSVGAElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGAElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[4].magic = JSSVGAElement::GetTransformToElementFuncNum;
    JSSVGAElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGAElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
    JSSVGAElementPrototypeFunctions[5].name = "getBBox";
    JSSVGAElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGAElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGAElementPrototypeFunctions[5].magic = JSSVGAElement::GetBBoxFuncNum;
    JSSVGAElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGAElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGAElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGAElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGAElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGAElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGAElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGAElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAElementAttributesFunctions, countof(JSSVGAElementAttributesFunctions));
    init_JSSVGAElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAElementPrototypeFunctions, countof(JSSVGAElementPrototypeFunctions));
}

static JSClassDef JSSVGAElementClassDefine;
static bool JSSVGAElementClassDefine_initialized = false;

static void init_JSSVGAElementClassDefine()
{
    if (JSSVGAElementClassDefine_initialized) return;
    JSSVGAElementClassDefine_initialized = true;
    memset(&JSSVGAElementClassDefine, 0, sizeof(JSSVGAElementClassDefine));
    JSSVGAElementClassDefine.class_name = "SVGAElement";
    JSSVGAElementClassDefine.finalizer = JSSVGAElement::finalizer;
    JSSVGAElementClassDefine.gc_mark = JSSVGAElement::mark;
}

JSClassID JSSVGAElement::js_class_id = 0;

void JSSVGAElement::init(JSContext* ctx)
{
    if (JSSVGAElement::js_class_id == 0) {
        init_JSSVGAElementClassDefine();
        JS_NewClassID(&JSSVGAElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAElement::js_class_id, &JSSVGAElementClassDefine);
        JS_SetClassProto(ctx, JSSVGAElement::js_class_id, JSSVGAElementPrototype::self(ctx));
    }
}

JSValue JSSVGAElement::create(JSContext* ctx, SVGAElement* impl)
{
    JSSVGAElement::init(ctx);
    JSValue _proto = JSSVGAElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGAElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->targetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGAElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGAElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGAElement* imp = (SVGAElement*)JS_GetOpaque(this_val, JSSVGAElement::js_class_id);
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
