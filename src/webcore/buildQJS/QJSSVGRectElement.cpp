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
#include "QJSSVGRectElement.h"

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
#include "SVGRectElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGRectElementAttributesFunctions[17];
static bool JSSVGRectElementAttributesFunctions_initialized = false;

static void init_JSSVGRectElementAttributesFunctions()
{
    if (JSSVGRectElementAttributesFunctions_initialized) return;
    JSSVGRectElementAttributesFunctions_initialized = true;
    memset(JSSVGRectElementAttributesFunctions, 0, sizeof(JSSVGRectElementAttributesFunctions));
    JSSVGRectElementAttributesFunctions[0].name = "x";
    JSSVGRectElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[0].magic = JSSVGRectElement::XAttrNum;
    JSSVGRectElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[1].name = "y";
    JSSVGRectElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[1].magic = JSSVGRectElement::YAttrNum;
    JSSVGRectElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[2].name = "width";
    JSSVGRectElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[2].magic = JSSVGRectElement::WidthAttrNum;
    JSSVGRectElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[3].name = "height";
    JSSVGRectElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[3].magic = JSSVGRectElement::HeightAttrNum;
    JSSVGRectElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[4].name = "rx";
    JSSVGRectElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[4].magic = JSSVGRectElement::RxAttrNum;
    JSSVGRectElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[5].name = "ry";
    JSSVGRectElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[5].magic = JSSVGRectElement::RyAttrNum;
    JSSVGRectElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[6].name = "requiredFeatures";
    JSSVGRectElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[6].magic = JSSVGRectElement::RequiredFeaturesAttrNum;
    JSSVGRectElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[7].name = "requiredExtensions";
    JSSVGRectElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[7].magic = JSSVGRectElement::RequiredExtensionsAttrNum;
    JSSVGRectElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[8].name = "systemLanguage";
    JSSVGRectElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[8].magic = JSSVGRectElement::SystemLanguageAttrNum;
    JSSVGRectElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[9].name = "xmllang";
    JSSVGRectElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[9].magic = JSSVGRectElement::XmllangAttrNum;
    JSSVGRectElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[9].u.getset.set.setter_magic = JSSVGRectElement::putValueProperty;
    JSSVGRectElementAttributesFunctions[10].name = "xmlspace";
    JSSVGRectElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[10].magic = JSSVGRectElement::XmlspaceAttrNum;
    JSSVGRectElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[10].u.getset.set.setter_magic = JSSVGRectElement::putValueProperty;
    JSSVGRectElementAttributesFunctions[11].name = "externalResourcesRequired";
    JSSVGRectElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[11].magic = JSSVGRectElement::ExternalResourcesRequiredAttrNum;
    JSSVGRectElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[12].name = "className";
    JSSVGRectElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[12].magic = JSSVGRectElement::ClassNameAttrNum;
    JSSVGRectElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[13].name = "style";
    JSSVGRectElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[13].magic = JSSVGRectElement::StyleAttrNum;
    JSSVGRectElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[14].name = "transform";
    JSSVGRectElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[14].magic = JSSVGRectElement::TransformAttrNum;
    JSSVGRectElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[15].name = "nearestViewportElement";
    JSSVGRectElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[15].magic = JSSVGRectElement::NearestViewportElementAttrNum;
    JSSVGRectElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGRectElementAttributesFunctions[16].name = "farthestViewportElement";
    JSSVGRectElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRectElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRectElementAttributesFunctions[16].magic = JSSVGRectElement::FarthestViewportElementAttrNum;
    JSSVGRectElementAttributesFunctions[16].u.getset.get.getter_magic = JSSVGRectElement::getValueProperty;
    JSSVGRectElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGRectElementPrototypeFunctions[6];
static bool JSSVGRectElementPrototypeFunctions_initialized = false;

static void init_JSSVGRectElementPrototypeFunctions()
{
    if (JSSVGRectElementPrototypeFunctions_initialized) return;
    JSSVGRectElementPrototypeFunctions_initialized = true;
    memset(JSSVGRectElementPrototypeFunctions, 0, sizeof(JSSVGRectElementPrototypeFunctions));
    JSSVGRectElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGRectElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[0].magic = JSSVGRectElement::HasExtensionFuncNum;
    JSSVGRectElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGRectElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
    JSSVGRectElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGRectElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[1].magic = JSSVGRectElement::GetPresentationAttributeFuncNum;
    JSSVGRectElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGRectElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
    JSSVGRectElementPrototypeFunctions[2].name = "getBBox";
    JSSVGRectElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[2].magic = JSSVGRectElement::GetBBoxFuncNum;
    JSSVGRectElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGRectElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
    JSSVGRectElementPrototypeFunctions[3].name = "getCTM";
    JSSVGRectElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[3].magic = JSSVGRectElement::GetCTMFuncNum;
    JSSVGRectElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGRectElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
    JSSVGRectElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGRectElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[4].magic = JSSVGRectElement::GetScreenCTMFuncNum;
    JSSVGRectElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGRectElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
    JSSVGRectElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGRectElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGRectElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGRectElementPrototypeFunctions[5].magic = JSSVGRectElement::GetTransformToElementFuncNum;
    JSSVGRectElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGRectElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGRectElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGRectElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGRectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRectElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGRectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGRectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGRectElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectElementAttributesFunctions, countof(JSSVGRectElementAttributesFunctions));
    init_JSSVGRectElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRectElementPrototypeFunctions, countof(JSSVGRectElementPrototypeFunctions));
}

static JSClassDef JSSVGRectElementClassDefine;
static bool JSSVGRectElementClassDefine_initialized = false;

static void init_JSSVGRectElementClassDefine()
{
    if (JSSVGRectElementClassDefine_initialized) return;
    JSSVGRectElementClassDefine_initialized = true;
    memset(&JSSVGRectElementClassDefine, 0, sizeof(JSSVGRectElementClassDefine));
    JSSVGRectElementClassDefine.class_name = "SVGRectElement";
    JSSVGRectElementClassDefine.finalizer = JSSVGRectElement::finalizer;
    JSSVGRectElementClassDefine.gc_mark = JSSVGRectElement::mark;
}

JSClassID JSSVGRectElement::js_class_id = 0;

void JSSVGRectElement::init(JSContext* ctx)
{
    if (JSSVGRectElement::js_class_id == 0) {
        init_JSSVGRectElementClassDefine();
        JS_NewClassID(&JSSVGRectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRectElement::js_class_id, &JSSVGRectElementClassDefine);
        JS_SetClassProto(ctx, JSSVGRectElement::js_class_id, JSSVGRectElementPrototype::self(ctx));
    }
}

JSValue JSSVGRectElement::create(JSContext* ctx, SVGRectElement* impl)
{
    JSSVGRectElement::init(ctx);
    JSValue _proto = JSSVGRectElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGRectElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGRectElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGRectElement* impl = (SVGRectElement*)JS_GetOpaque(val, JSSVGRectElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGRectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGRectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RxAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RyAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->ryAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGRectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGRectElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGRectElement* imp = (SVGRectElement*)JS_GetOpaque(this_val, JSSVGRectElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGRectElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGRectElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGRectElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGRectElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGRectElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGRectElement::GetTransformToElementFuncNum: {
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
