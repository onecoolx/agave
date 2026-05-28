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

static JSCFunctionListEntry JSSVGGElementAttributesFunctions[11];
static bool JSSVGGElementAttributesFunctions_initialized = false;

static void init_JSSVGGElementAttributesFunctions()
{
    if (JSSVGGElementAttributesFunctions_initialized) return;
    JSSVGGElementAttributesFunctions_initialized = true;
    memset(JSSVGGElementAttributesFunctions, 0, sizeof(JSSVGGElementAttributesFunctions));
    JSSVGGElementAttributesFunctions[0].name = "farthestViewportElement";
    JSSVGGElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[0].magic = JSSVGGElement::FarthestViewportElementAttrNum;
    JSSVGGElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[1].name = "systemLanguage";
    JSSVGGElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[1].magic = JSSVGGElement::SystemLanguageAttrNum;
    JSSVGGElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[2].name = "xmlspace";
    JSSVGGElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[2].magic = JSSVGGElement::XmlspaceAttrNum;
    JSSVGGElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[2].u.getset.set.setter_magic = JSSVGGElement::putValueProperty;
    JSSVGGElementAttributesFunctions[3].name = "requiredExtensions";
    JSSVGGElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[3].magic = JSSVGGElement::RequiredExtensionsAttrNum;
    JSSVGGElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[4].name = "nearestViewportElement";
    JSSVGGElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[4].magic = JSSVGGElement::NearestViewportElementAttrNum;
    JSSVGGElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[5].name = "style";
    JSSVGGElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[5].magic = JSSVGGElement::StyleAttrNum;
    JSSVGGElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[6].name = "requiredFeatures";
    JSSVGGElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[6].magic = JSSVGGElement::RequiredFeaturesAttrNum;
    JSSVGGElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[7].name = "xmllang";
    JSSVGGElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[7].magic = JSSVGGElement::XmllangAttrNum;
    JSSVGGElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[7].u.getset.set.setter_magic = JSSVGGElement::putValueProperty;
    JSSVGGElementAttributesFunctions[8].name = "externalResourcesRequired";
    JSSVGGElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[8].magic = JSSVGGElement::ExternalResourcesRequiredAttrNum;
    JSSVGGElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[9].name = "className";
    JSSVGGElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[9].magic = JSSVGGElement::ClassNameAttrNum;
    JSSVGGElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGGElementAttributesFunctions[10].name = "transform";
    JSSVGGElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGElementAttributesFunctions[10].magic = JSSVGGElement::TransformAttrNum;
    JSSVGGElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGGElement::getValueProperty;
    JSSVGGElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGGElementPrototypeFunctions[6];
static bool JSSVGGElementPrototypeFunctions_initialized = false;

static void init_JSSVGGElementPrototypeFunctions()
{
    if (JSSVGGElementPrototypeFunctions_initialized) return;
    JSSVGGElementPrototypeFunctions_initialized = true;
    memset(JSSVGGElementPrototypeFunctions, 0, sizeof(JSSVGGElementPrototypeFunctions));
    JSSVGGElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGGElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[0].magic = JSSVGGElement::GetPresentationAttributeFuncNum;
    JSSVGGElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGGElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
    JSSVGGElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGGElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[1].magic = JSSVGGElement::HasExtensionFuncNum;
    JSSVGGElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGGElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
    JSSVGGElementPrototypeFunctions[2].name = "getCTM";
    JSSVGGElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[2].magic = JSSVGGElement::GetCTMFuncNum;
    JSSVGGElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGGElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
    JSSVGGElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGGElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[3].magic = JSSVGGElement::GetScreenCTMFuncNum;
    JSSVGGElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGGElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
    JSSVGGElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGGElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[4].magic = JSSVGGElement::GetTransformToElementFuncNum;
    JSSVGGElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGGElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
    JSSVGGElementPrototypeFunctions[5].name = "getBBox";
    JSSVGGElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGGElementPrototypeFunctions[5].magic = JSSVGGElement::GetBBoxFuncNum;
    JSSVGGElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGGElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGGElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGGElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGGElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGGElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGGElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGGElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGGElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGElementAttributesFunctions, countof(JSSVGGElementAttributesFunctions));
    init_JSSVGGElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGElementPrototypeFunctions, countof(JSSVGGElementPrototypeFunctions));
}

static JSClassDef JSSVGGElementClassDefine;
static bool JSSVGGElementClassDefine_initialized = false;

static void init_JSSVGGElementClassDefine()
{
    if (JSSVGGElementClassDefine_initialized) return;
    JSSVGGElementClassDefine_initialized = true;
    memset(&JSSVGGElementClassDefine, 0, sizeof(JSSVGGElementClassDefine));
    JSSVGGElementClassDefine.class_name = "SVGGElement";
    JSSVGGElementClassDefine.finalizer = JSSVGGElement::finalizer;
    JSSVGGElementClassDefine.gc_mark = JSSVGGElement::mark;
}

JSClassID JSSVGGElement::js_class_id = 0;

void JSSVGGElement::init(JSContext* ctx)
{
    if (JSSVGGElement::js_class_id == 0) {
        init_JSSVGGElementClassDefine();
        JS_NewClassID(&JSSVGGElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGGElement::js_class_id, &JSSVGGElementClassDefine);
        JS_SetClassProto(ctx, JSSVGGElement::js_class_id, JSSVGGElementPrototype::self(ctx));
    }
}

JSValue JSSVGGElement::create(JSContext* ctx, SVGGElement* impl)
{
    JSSVGGElement::init(ctx);
    JSValue _proto = JSSVGGElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGGElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGGElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGGElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGGElement* imp = (SVGGElement*)JS_GetOpaque(this_val, JSSVGGElement::js_class_id);
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
