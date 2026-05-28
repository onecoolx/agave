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

static JSCFunctionListEntry JSSVGSwitchElementAttributesFunctions[11];
static bool JSSVGSwitchElementAttributesFunctions_initialized = false;

static void init_JSSVGSwitchElementAttributesFunctions()
{
    if (JSSVGSwitchElementAttributesFunctions_initialized) return;
    JSSVGSwitchElementAttributesFunctions_initialized = true;
    memset(JSSVGSwitchElementAttributesFunctions, 0, sizeof(JSSVGSwitchElementAttributesFunctions));
    JSSVGSwitchElementAttributesFunctions[0].name = "requiredFeatures";
    JSSVGSwitchElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[0].magic = JSSVGSwitchElement::RequiredFeaturesAttrNum;
    JSSVGSwitchElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[1].name = "requiredExtensions";
    JSSVGSwitchElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[1].magic = JSSVGSwitchElement::RequiredExtensionsAttrNum;
    JSSVGSwitchElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[2].name = "systemLanguage";
    JSSVGSwitchElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[2].magic = JSSVGSwitchElement::SystemLanguageAttrNum;
    JSSVGSwitchElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[3].name = "xmllang";
    JSSVGSwitchElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[3].magic = JSSVGSwitchElement::XmllangAttrNum;
    JSSVGSwitchElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[3].u.getset.set.setter_magic = JSSVGSwitchElement::putValueProperty;
    JSSVGSwitchElementAttributesFunctions[4].name = "xmlspace";
    JSSVGSwitchElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[4].magic = JSSVGSwitchElement::XmlspaceAttrNum;
    JSSVGSwitchElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGSwitchElement::putValueProperty;
    JSSVGSwitchElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGSwitchElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[5].magic = JSSVGSwitchElement::ExternalResourcesRequiredAttrNum;
    JSSVGSwitchElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[6].name = "className";
    JSSVGSwitchElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[6].magic = JSSVGSwitchElement::ClassNameAttrNum;
    JSSVGSwitchElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[7].name = "style";
    JSSVGSwitchElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[7].magic = JSSVGSwitchElement::StyleAttrNum;
    JSSVGSwitchElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[8].name = "transform";
    JSSVGSwitchElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[8].magic = JSSVGSwitchElement::TransformAttrNum;
    JSSVGSwitchElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[9].name = "nearestViewportElement";
    JSSVGSwitchElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[9].magic = JSSVGSwitchElement::NearestViewportElementAttrNum;
    JSSVGSwitchElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGSwitchElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGSwitchElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGSwitchElementAttributesFunctions[10].magic = JSSVGSwitchElement::FarthestViewportElementAttrNum;
    JSSVGSwitchElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGSwitchElement::getValueProperty;
    JSSVGSwitchElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGSwitchElementPrototypeFunctions[6];
static bool JSSVGSwitchElementPrototypeFunctions_initialized = false;

static void init_JSSVGSwitchElementPrototypeFunctions()
{
    if (JSSVGSwitchElementPrototypeFunctions_initialized) return;
    JSSVGSwitchElementPrototypeFunctions_initialized = true;
    memset(JSSVGSwitchElementPrototypeFunctions, 0, sizeof(JSSVGSwitchElementPrototypeFunctions));
    JSSVGSwitchElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGSwitchElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[0].magic = JSSVGSwitchElement::HasExtensionFuncNum;
    JSSVGSwitchElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGSwitchElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
    JSSVGSwitchElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGSwitchElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[1].magic = JSSVGSwitchElement::GetPresentationAttributeFuncNum;
    JSSVGSwitchElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGSwitchElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
    JSSVGSwitchElementPrototypeFunctions[2].name = "getBBox";
    JSSVGSwitchElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[2].magic = JSSVGSwitchElement::GetBBoxFuncNum;
    JSSVGSwitchElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGSwitchElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
    JSSVGSwitchElementPrototypeFunctions[3].name = "getCTM";
    JSSVGSwitchElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[3].magic = JSSVGSwitchElement::GetCTMFuncNum;
    JSSVGSwitchElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGSwitchElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
    JSSVGSwitchElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGSwitchElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[4].magic = JSSVGSwitchElement::GetScreenCTMFuncNum;
    JSSVGSwitchElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGSwitchElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
    JSSVGSwitchElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGSwitchElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGSwitchElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGSwitchElementPrototypeFunctions[5].magic = JSSVGSwitchElement::GetTransformToElementFuncNum;
    JSSVGSwitchElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGSwitchElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGSwitchElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGSwitchElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGSwitchElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGSwitchElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGSwitchElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGSwitchElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGSwitchElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGSwitchElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSwitchElementAttributesFunctions, countof(JSSVGSwitchElementAttributesFunctions));
    init_JSSVGSwitchElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGSwitchElementPrototypeFunctions, countof(JSSVGSwitchElementPrototypeFunctions));
}

static JSClassDef JSSVGSwitchElementClassDefine;
static bool JSSVGSwitchElementClassDefine_initialized = false;

static void init_JSSVGSwitchElementClassDefine()
{
    if (JSSVGSwitchElementClassDefine_initialized) return;
    JSSVGSwitchElementClassDefine_initialized = true;
    memset(&JSSVGSwitchElementClassDefine, 0, sizeof(JSSVGSwitchElementClassDefine));
    JSSVGSwitchElementClassDefine.class_name = "SVGSwitchElement";
    JSSVGSwitchElementClassDefine.finalizer = JSSVGSwitchElement::finalizer;
    JSSVGSwitchElementClassDefine.gc_mark = JSSVGSwitchElement::mark;
}

JSClassID JSSVGSwitchElement::js_class_id = 0;

void JSSVGSwitchElement::init(JSContext* ctx)
{
    if (JSSVGSwitchElement::js_class_id == 0) {
        init_JSSVGSwitchElementClassDefine();
        JS_NewClassID(&JSSVGSwitchElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGSwitchElement::js_class_id, &JSSVGSwitchElementClassDefine);
        JS_SetClassProto(ctx, JSSVGSwitchElement::js_class_id, JSSVGSwitchElementPrototype::self(ctx));
    }
}

JSValue JSSVGSwitchElement::create(JSContext* ctx, SVGSwitchElement* impl)
{
    JSSVGSwitchElement::init(ctx);
    JSValue _proto = JSSVGSwitchElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGSwitchElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGSwitchElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGSwitchElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGSwitchElement* imp = (SVGSwitchElement*)JS_GetOpaque(this_val, JSSVGSwitchElement::js_class_id);
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
