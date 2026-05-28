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
#include "QJSSVGUseElement.h"

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
#include "QJSSVGElementInstance.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGElementInstance.h"
#include "SVGStringList.h"
#include "SVGUseElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGUseElementAttributesFunctions[18];
static bool JSSVGUseElementAttributesFunctions_initialized = false;

static void init_JSSVGUseElementAttributesFunctions()
{
    if (JSSVGUseElementAttributesFunctions_initialized) return;
    JSSVGUseElementAttributesFunctions_initialized = true;
    memset(JSSVGUseElementAttributesFunctions, 0, sizeof(JSSVGUseElementAttributesFunctions));
    JSSVGUseElementAttributesFunctions[0].name = "xmlspace";
    JSSVGUseElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[0].magic = JSSVGUseElement::XmlspaceAttrNum;
    JSSVGUseElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGUseElement::putValueProperty;
    JSSVGUseElementAttributesFunctions[1].name = "requiredFeatures";
    JSSVGUseElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[1].magic = JSSVGUseElement::RequiredFeaturesAttrNum;
    JSSVGUseElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[2].name = "y";
    JSSVGUseElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[2].magic = JSSVGUseElement::YAttrNum;
    JSSVGUseElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[3].name = "xmllang";
    JSSVGUseElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[3].magic = JSSVGUseElement::XmllangAttrNum;
    JSSVGUseElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[3].u.getset.set.setter_magic = JSSVGUseElement::putValueProperty;
    JSSVGUseElementAttributesFunctions[4].name = "transform";
    JSSVGUseElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[4].magic = JSSVGUseElement::TransformAttrNum;
    JSSVGUseElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[5].name = "style";
    JSSVGUseElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[5].magic = JSSVGUseElement::StyleAttrNum;
    JSSVGUseElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[6].name = "nearestViewportElement";
    JSSVGUseElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[6].magic = JSSVGUseElement::NearestViewportElementAttrNum;
    JSSVGUseElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[7].name = "x";
    JSSVGUseElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[7].magic = JSSVGUseElement::XAttrNum;
    JSSVGUseElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[8].name = "systemLanguage";
    JSSVGUseElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[8].magic = JSSVGUseElement::SystemLanguageAttrNum;
    JSSVGUseElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[9].name = "width";
    JSSVGUseElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[9].magic = JSSVGUseElement::WidthAttrNum;
    JSSVGUseElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[10].name = "instanceRoot";
    JSSVGUseElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[10].magic = JSSVGUseElement::InstanceRootAttrNum;
    JSSVGUseElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[11].name = "href";
    JSSVGUseElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[11].magic = JSSVGUseElement::HrefAttrNum;
    JSSVGUseElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[12].name = "requiredExtensions";
    JSSVGUseElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[12].magic = JSSVGUseElement::RequiredExtensionsAttrNum;
    JSSVGUseElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[13].name = "height";
    JSSVGUseElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[13].magic = JSSVGUseElement::HeightAttrNum;
    JSSVGUseElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[14].name = "animatedInstanceRoot";
    JSSVGUseElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[14].magic = JSSVGUseElement::AnimatedInstanceRootAttrNum;
    JSSVGUseElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[15].name = "externalResourcesRequired";
    JSSVGUseElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[15].magic = JSSVGUseElement::ExternalResourcesRequiredAttrNum;
    JSSVGUseElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[16].name = "className";
    JSSVGUseElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[16].magic = JSSVGUseElement::ClassNameAttrNum;
    JSSVGUseElementAttributesFunctions[16].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSSVGUseElementAttributesFunctions[17].name = "farthestViewportElement";
    JSSVGUseElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGUseElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGUseElementAttributesFunctions[17].magic = JSSVGUseElement::FarthestViewportElementAttrNum;
    JSSVGUseElementAttributesFunctions[17].u.getset.get.getter_magic = JSSVGUseElement::getValueProperty;
    JSSVGUseElementAttributesFunctions[17].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGUseElementPrototypeFunctions[6];
static bool JSSVGUseElementPrototypeFunctions_initialized = false;

static void init_JSSVGUseElementPrototypeFunctions()
{
    if (JSSVGUseElementPrototypeFunctions_initialized) return;
    JSSVGUseElementPrototypeFunctions_initialized = true;
    memset(JSSVGUseElementPrototypeFunctions, 0, sizeof(JSSVGUseElementPrototypeFunctions));
    JSSVGUseElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGUseElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[0].magic = JSSVGUseElement::GetPresentationAttributeFuncNum;
    JSSVGUseElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGUseElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
    JSSVGUseElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGUseElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[1].magic = JSSVGUseElement::HasExtensionFuncNum;
    JSSVGUseElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGUseElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
    JSSVGUseElementPrototypeFunctions[2].name = "getCTM";
    JSSVGUseElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[2].magic = JSSVGUseElement::GetCTMFuncNum;
    JSSVGUseElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGUseElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
    JSSVGUseElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGUseElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[3].magic = JSSVGUseElement::GetScreenCTMFuncNum;
    JSSVGUseElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGUseElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
    JSSVGUseElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGUseElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[4].magic = JSSVGUseElement::GetTransformToElementFuncNum;
    JSSVGUseElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGUseElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
    JSSVGUseElementPrototypeFunctions[5].name = "getBBox";
    JSSVGUseElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGUseElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGUseElementPrototypeFunctions[5].magic = JSSVGUseElement::GetBBoxFuncNum;
    JSSVGUseElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGUseElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGUseElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGUseElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGUseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGUseElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGUseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGUseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGUseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGUseElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUseElementAttributesFunctions, countof(JSSVGUseElementAttributesFunctions));
    init_JSSVGUseElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGUseElementPrototypeFunctions, countof(JSSVGUseElementPrototypeFunctions));
}

static JSClassDef JSSVGUseElementClassDefine;
static bool JSSVGUseElementClassDefine_initialized = false;

static void init_JSSVGUseElementClassDefine()
{
    if (JSSVGUseElementClassDefine_initialized) return;
    JSSVGUseElementClassDefine_initialized = true;
    memset(&JSSVGUseElementClassDefine, 0, sizeof(JSSVGUseElementClassDefine));
    JSSVGUseElementClassDefine.class_name = "SVGUseElement";
    JSSVGUseElementClassDefine.finalizer = JSSVGUseElement::finalizer;
    JSSVGUseElementClassDefine.gc_mark = JSSVGUseElement::mark;
}

JSClassID JSSVGUseElement::js_class_id = 0;

void JSSVGUseElement::init(JSContext* ctx)
{
    if (JSSVGUseElement::js_class_id == 0) {
        init_JSSVGUseElementClassDefine();
        JS_NewClassID(&JSSVGUseElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGUseElement::js_class_id, &JSSVGUseElementClassDefine);
        JS_SetClassProto(ctx, JSSVGUseElement::js_class_id, JSSVGUseElementPrototype::self(ctx));
    }
}

JSValue JSSVGUseElement::create(JSContext* ctx, SVGUseElement* impl)
{
    JSSVGUseElement::init(ctx);
    JSValue _proto = JSSVGUseElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGUseElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGUseElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGUseElement* impl = (SVGUseElement*)JS_GetOpaque(val, JSSVGUseElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGUseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGUseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case InstanceRootAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->instanceRoot()));
        }
        case AnimatedInstanceRootAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedInstanceRoot()));
        }
        case HrefAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGUseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGUseElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGUseElement* imp = (SVGUseElement*)JS_GetOpaque(this_val, JSSVGUseElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGUseElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGUseElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGUseElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGUseElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGUseElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGUseElement::GetTransformToElementFuncNum: {
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
