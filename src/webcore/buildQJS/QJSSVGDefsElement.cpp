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

static JSCFunctionListEntry JSSVGDefsElementAttributesFunctions[11];
static bool JSSVGDefsElementAttributesFunctions_initialized = false;

static void init_JSSVGDefsElementAttributesFunctions()
{
    if (JSSVGDefsElementAttributesFunctions_initialized) return;
    JSSVGDefsElementAttributesFunctions_initialized = true;
    memset(JSSVGDefsElementAttributesFunctions, 0, sizeof(JSSVGDefsElementAttributesFunctions));
    JSSVGDefsElementAttributesFunctions[0].name = "requiredFeatures";
    JSSVGDefsElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[0].magic = JSSVGDefsElement::RequiredFeaturesAttrNum;
    JSSVGDefsElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[1].name = "requiredExtensions";
    JSSVGDefsElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[1].magic = JSSVGDefsElement::RequiredExtensionsAttrNum;
    JSSVGDefsElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[2].name = "systemLanguage";
    JSSVGDefsElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[2].magic = JSSVGDefsElement::SystemLanguageAttrNum;
    JSSVGDefsElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[3].name = "xmllang";
    JSSVGDefsElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[3].magic = JSSVGDefsElement::XmllangAttrNum;
    JSSVGDefsElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[3].u.getset.set.setter_magic = JSSVGDefsElement::putValueProperty;
    JSSVGDefsElementAttributesFunctions[4].name = "xmlspace";
    JSSVGDefsElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[4].magic = JSSVGDefsElement::XmlspaceAttrNum;
    JSSVGDefsElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGDefsElement::putValueProperty;
    JSSVGDefsElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGDefsElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[5].magic = JSSVGDefsElement::ExternalResourcesRequiredAttrNum;
    JSSVGDefsElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[6].name = "className";
    JSSVGDefsElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[6].magic = JSSVGDefsElement::ClassNameAttrNum;
    JSSVGDefsElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[7].name = "style";
    JSSVGDefsElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[7].magic = JSSVGDefsElement::StyleAttrNum;
    JSSVGDefsElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[8].name = "transform";
    JSSVGDefsElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[8].magic = JSSVGDefsElement::TransformAttrNum;
    JSSVGDefsElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[9].name = "nearestViewportElement";
    JSSVGDefsElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[9].magic = JSSVGDefsElement::NearestViewportElementAttrNum;
    JSSVGDefsElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGDefsElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGDefsElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGDefsElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGDefsElementAttributesFunctions[10].magic = JSSVGDefsElement::FarthestViewportElementAttrNum;
    JSSVGDefsElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGDefsElement::getValueProperty;
    JSSVGDefsElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGDefsElementPrototypeFunctions[6];
static bool JSSVGDefsElementPrototypeFunctions_initialized = false;

static void init_JSSVGDefsElementPrototypeFunctions()
{
    if (JSSVGDefsElementPrototypeFunctions_initialized) return;
    JSSVGDefsElementPrototypeFunctions_initialized = true;
    memset(JSSVGDefsElementPrototypeFunctions, 0, sizeof(JSSVGDefsElementPrototypeFunctions));
    JSSVGDefsElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGDefsElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[0].magic = JSSVGDefsElement::HasExtensionFuncNum;
    JSSVGDefsElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGDefsElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
    JSSVGDefsElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGDefsElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[1].magic = JSSVGDefsElement::GetPresentationAttributeFuncNum;
    JSSVGDefsElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGDefsElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
    JSSVGDefsElementPrototypeFunctions[2].name = "getBBox";
    JSSVGDefsElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[2].magic = JSSVGDefsElement::GetBBoxFuncNum;
    JSSVGDefsElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGDefsElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
    JSSVGDefsElementPrototypeFunctions[3].name = "getCTM";
    JSSVGDefsElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[3].magic = JSSVGDefsElement::GetCTMFuncNum;
    JSSVGDefsElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGDefsElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
    JSSVGDefsElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGDefsElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[4].magic = JSSVGDefsElement::GetScreenCTMFuncNum;
    JSSVGDefsElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGDefsElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
    JSSVGDefsElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGDefsElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGDefsElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGDefsElementPrototypeFunctions[5].magic = JSSVGDefsElement::GetTransformToElementFuncNum;
    JSSVGDefsElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGDefsElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGDefsElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGDefsElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGDefsElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGDefsElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGDefsElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGDefsElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGDefsElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGDefsElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDefsElementAttributesFunctions, countof(JSSVGDefsElementAttributesFunctions));
    init_JSSVGDefsElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGDefsElementPrototypeFunctions, countof(JSSVGDefsElementPrototypeFunctions));
}

static JSClassDef JSSVGDefsElementClassDefine;
static bool JSSVGDefsElementClassDefine_initialized = false;

static void init_JSSVGDefsElementClassDefine()
{
    if (JSSVGDefsElementClassDefine_initialized) return;
    JSSVGDefsElementClassDefine_initialized = true;
    memset(&JSSVGDefsElementClassDefine, 0, sizeof(JSSVGDefsElementClassDefine));
    JSSVGDefsElementClassDefine.class_name = "SVGDefsElement";
    JSSVGDefsElementClassDefine.finalizer = JSSVGDefsElement::finalizer;
    JSSVGDefsElementClassDefine.gc_mark = JSSVGDefsElement::mark;
}

JSClassID JSSVGDefsElement::js_class_id = 0;

void JSSVGDefsElement::init(JSContext* ctx)
{
    if (JSSVGDefsElement::js_class_id == 0) {
        init_JSSVGDefsElementClassDefine();
        JS_NewClassID(&JSSVGDefsElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGDefsElement::js_class_id, &JSSVGDefsElementClassDefine);
        JS_SetClassProto(ctx, JSSVGDefsElement::js_class_id, JSSVGDefsElementPrototype::self(ctx));
    }
}

JSValue JSSVGDefsElement::create(JSContext* ctx, SVGDefsElement* impl)
{
    JSSVGDefsElement::init(ctx);
    JSValue _proto = JSSVGDefsElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGDefsElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGDefsElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGDefsElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGDefsElement* imp = (SVGDefsElement*)JS_GetOpaque(this_val, JSSVGDefsElement::js_class_id);
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
