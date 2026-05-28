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
#include "QJSSVGEllipseElement.h"

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
#include "SVGEllipseElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGEllipseElementAttributesFunctions[15];
static bool JSSVGEllipseElementAttributesFunctions_initialized = false;

static void init_JSSVGEllipseElementAttributesFunctions()
{
    if (JSSVGEllipseElementAttributesFunctions_initialized) return;
    JSSVGEllipseElementAttributesFunctions_initialized = true;
    memset(JSSVGEllipseElementAttributesFunctions, 0, sizeof(JSSVGEllipseElementAttributesFunctions));
    JSSVGEllipseElementAttributesFunctions[0].name = "xmllang";
    JSSVGEllipseElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[0].magic = JSSVGEllipseElement::XmllangAttrNum;
    JSSVGEllipseElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGEllipseElement::putValueProperty;
    JSSVGEllipseElementAttributesFunctions[1].name = "rx";
    JSSVGEllipseElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[1].magic = JSSVGEllipseElement::RxAttrNum;
    JSSVGEllipseElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[2].name = "xmlspace";
    JSSVGEllipseElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[2].magic = JSSVGEllipseElement::XmlspaceAttrNum;
    JSSVGEllipseElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[2].u.getset.set.setter_magic = JSSVGEllipseElement::putValueProperty;
    JSSVGEllipseElementAttributesFunctions[3].name = "cy";
    JSSVGEllipseElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[3].magic = JSSVGEllipseElement::CyAttrNum;
    JSSVGEllipseElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[4].name = "ry";
    JSSVGEllipseElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[4].magic = JSSVGEllipseElement::RyAttrNum;
    JSSVGEllipseElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGEllipseElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[5].magic = JSSVGEllipseElement::ExternalResourcesRequiredAttrNum;
    JSSVGEllipseElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[6].name = "className";
    JSSVGEllipseElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[6].magic = JSSVGEllipseElement::ClassNameAttrNum;
    JSSVGEllipseElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[7].name = "requiredExtensions";
    JSSVGEllipseElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[7].magic = JSSVGEllipseElement::RequiredExtensionsAttrNum;
    JSSVGEllipseElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[8].name = "cx";
    JSSVGEllipseElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[8].magic = JSSVGEllipseElement::CxAttrNum;
    JSSVGEllipseElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[9].name = "style";
    JSSVGEllipseElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[9].magic = JSSVGEllipseElement::StyleAttrNum;
    JSSVGEllipseElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGEllipseElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[10].magic = JSSVGEllipseElement::FarthestViewportElementAttrNum;
    JSSVGEllipseElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[11].name = "requiredFeatures";
    JSSVGEllipseElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[11].magic = JSSVGEllipseElement::RequiredFeaturesAttrNum;
    JSSVGEllipseElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[12].name = "systemLanguage";
    JSSVGEllipseElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[12].magic = JSSVGEllipseElement::SystemLanguageAttrNum;
    JSSVGEllipseElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[13].name = "transform";
    JSSVGEllipseElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[13].magic = JSSVGEllipseElement::TransformAttrNum;
    JSSVGEllipseElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGEllipseElementAttributesFunctions[14].name = "nearestViewportElement";
    JSSVGEllipseElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGEllipseElementAttributesFunctions[14].magic = JSSVGEllipseElement::NearestViewportElementAttrNum;
    JSSVGEllipseElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGEllipseElement::getValueProperty;
    JSSVGEllipseElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGEllipseElementPrototypeFunctions[6];
static bool JSSVGEllipseElementPrototypeFunctions_initialized = false;

static void init_JSSVGEllipseElementPrototypeFunctions()
{
    if (JSSVGEllipseElementPrototypeFunctions_initialized) return;
    JSSVGEllipseElementPrototypeFunctions_initialized = true;
    memset(JSSVGEllipseElementPrototypeFunctions, 0, sizeof(JSSVGEllipseElementPrototypeFunctions));
    JSSVGEllipseElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGEllipseElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[0].magic = JSSVGEllipseElement::GetPresentationAttributeFuncNum;
    JSSVGEllipseElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGEllipseElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
    JSSVGEllipseElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGEllipseElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[1].magic = JSSVGEllipseElement::HasExtensionFuncNum;
    JSSVGEllipseElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGEllipseElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
    JSSVGEllipseElementPrototypeFunctions[2].name = "getCTM";
    JSSVGEllipseElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[2].magic = JSSVGEllipseElement::GetCTMFuncNum;
    JSSVGEllipseElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGEllipseElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
    JSSVGEllipseElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGEllipseElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[3].magic = JSSVGEllipseElement::GetScreenCTMFuncNum;
    JSSVGEllipseElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGEllipseElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
    JSSVGEllipseElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGEllipseElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[4].magic = JSSVGEllipseElement::GetTransformToElementFuncNum;
    JSSVGEllipseElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGEllipseElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
    JSSVGEllipseElementPrototypeFunctions[5].name = "getBBox";
    JSSVGEllipseElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGEllipseElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGEllipseElementPrototypeFunctions[5].magic = JSSVGEllipseElement::GetBBoxFuncNum;
    JSSVGEllipseElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGEllipseElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGEllipseElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGEllipseElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGEllipseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGEllipseElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGEllipseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGEllipseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGEllipseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGEllipseElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGEllipseElementAttributesFunctions, countof(JSSVGEllipseElementAttributesFunctions));
    init_JSSVGEllipseElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGEllipseElementPrototypeFunctions, countof(JSSVGEllipseElementPrototypeFunctions));
}

static JSClassDef JSSVGEllipseElementClassDefine;
static bool JSSVGEllipseElementClassDefine_initialized = false;

static void init_JSSVGEllipseElementClassDefine()
{
    if (JSSVGEllipseElementClassDefine_initialized) return;
    JSSVGEllipseElementClassDefine_initialized = true;
    memset(&JSSVGEllipseElementClassDefine, 0, sizeof(JSSVGEllipseElementClassDefine));
    JSSVGEllipseElementClassDefine.class_name = "SVGEllipseElement";
    JSSVGEllipseElementClassDefine.finalizer = JSSVGEllipseElement::finalizer;
    JSSVGEllipseElementClassDefine.gc_mark = JSSVGEllipseElement::mark;
}

JSClassID JSSVGEllipseElement::js_class_id = 0;

void JSSVGEllipseElement::init(JSContext* ctx)
{
    if (JSSVGEllipseElement::js_class_id == 0) {
        init_JSSVGEllipseElementClassDefine();
        JS_NewClassID(&JSSVGEllipseElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGEllipseElement::js_class_id, &JSSVGEllipseElementClassDefine);
        JS_SetClassProto(ctx, JSSVGEllipseElement::js_class_id, JSSVGEllipseElementPrototype::self(ctx));
    }
}

JSValue JSSVGEllipseElement::create(JSContext* ctx, SVGEllipseElement* impl)
{
    JSSVGEllipseElement::init(ctx);
    JSValue _proto = JSSVGEllipseElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGEllipseElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGEllipseElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGEllipseElement* impl = (SVGEllipseElement*)JS_GetOpaque(val, JSSVGEllipseElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGEllipseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGEllipseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CxAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case CyAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RxAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RyAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->ryAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGEllipseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGEllipseElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGEllipseElement* imp = (SVGEllipseElement*)JS_GetOpaque(this_val, JSSVGEllipseElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGEllipseElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGEllipseElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGEllipseElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGEllipseElement::GetTransformToElementFuncNum: {
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
