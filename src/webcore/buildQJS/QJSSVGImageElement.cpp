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
#include "QJSSVGImageElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGElement.h"
#include "SVGImageElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGImageElementAttributesFunctions[17];
static bool JSSVGImageElementAttributesFunctions_initialized = false;

static void init_JSSVGImageElementAttributesFunctions()
{
    if (JSSVGImageElementAttributesFunctions_initialized) return;
    JSSVGImageElementAttributesFunctions_initialized = true;
    memset(JSSVGImageElementAttributesFunctions, 0, sizeof(JSSVGImageElementAttributesFunctions));
    JSSVGImageElementAttributesFunctions[0].name = "height";
    JSSVGImageElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[0].magic = JSSVGImageElement::HeightAttrNum;
    JSSVGImageElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[1].name = "requiredFeatures";
    JSSVGImageElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[1].magic = JSSVGImageElement::RequiredFeaturesAttrNum;
    JSSVGImageElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[2].name = "systemLanguage";
    JSSVGImageElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[2].magic = JSSVGImageElement::SystemLanguageAttrNum;
    JSSVGImageElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[3].name = "farthestViewportElement";
    JSSVGImageElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[3].magic = JSSVGImageElement::FarthestViewportElementAttrNum;
    JSSVGImageElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[4].name = "x";
    JSSVGImageElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[4].magic = JSSVGImageElement::XAttrNum;
    JSSVGImageElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[5].name = "externalResourcesRequired";
    JSSVGImageElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[5].magic = JSSVGImageElement::ExternalResourcesRequiredAttrNum;
    JSSVGImageElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[6].name = "href";
    JSSVGImageElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[6].magic = JSSVGImageElement::HrefAttrNum;
    JSSVGImageElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[7].name = "width";
    JSSVGImageElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[7].magic = JSSVGImageElement::WidthAttrNum;
    JSSVGImageElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[8].name = "y";
    JSSVGImageElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[8].magic = JSSVGImageElement::YAttrNum;
    JSSVGImageElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[9].name = "transform";
    JSSVGImageElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[9].magic = JSSVGImageElement::TransformAttrNum;
    JSSVGImageElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[10].name = "preserveAspectRatio";
    JSSVGImageElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[10].magic = JSSVGImageElement::PreserveAspectRatioAttrNum;
    JSSVGImageElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[11].name = "requiredExtensions";
    JSSVGImageElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[11].magic = JSSVGImageElement::RequiredExtensionsAttrNum;
    JSSVGImageElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[12].name = "xmllang";
    JSSVGImageElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[12].magic = JSSVGImageElement::XmllangAttrNum;
    JSSVGImageElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[12].u.getset.set.setter_magic = JSSVGImageElement::putValueProperty;
    JSSVGImageElementAttributesFunctions[13].name = "xmlspace";
    JSSVGImageElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[13].magic = JSSVGImageElement::XmlspaceAttrNum;
    JSSVGImageElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[13].u.getset.set.setter_magic = JSSVGImageElement::putValueProperty;
    JSSVGImageElementAttributesFunctions[14].name = "className";
    JSSVGImageElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[14].magic = JSSVGImageElement::ClassNameAttrNum;
    JSSVGImageElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[15].name = "style";
    JSSVGImageElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[15].magic = JSSVGImageElement::StyleAttrNum;
    JSSVGImageElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGImageElementAttributesFunctions[16].name = "nearestViewportElement";
    JSSVGImageElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGImageElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGImageElementAttributesFunctions[16].magic = JSSVGImageElement::NearestViewportElementAttrNum;
    JSSVGImageElementAttributesFunctions[16].u.getset.get.getter_magic = JSSVGImageElement::getValueProperty;
    JSSVGImageElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGImageElementPrototypeFunctions[6];
static bool JSSVGImageElementPrototypeFunctions_initialized = false;

static void init_JSSVGImageElementPrototypeFunctions()
{
    if (JSSVGImageElementPrototypeFunctions_initialized) return;
    JSSVGImageElementPrototypeFunctions_initialized = true;
    memset(JSSVGImageElementPrototypeFunctions, 0, sizeof(JSSVGImageElementPrototypeFunctions));
    JSSVGImageElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGImageElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[0].magic = JSSVGImageElement::GetPresentationAttributeFuncNum;
    JSSVGImageElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGImageElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
    JSSVGImageElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGImageElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[1].magic = JSSVGImageElement::HasExtensionFuncNum;
    JSSVGImageElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGImageElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
    JSSVGImageElementPrototypeFunctions[2].name = "getCTM";
    JSSVGImageElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[2].magic = JSSVGImageElement::GetCTMFuncNum;
    JSSVGImageElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGImageElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
    JSSVGImageElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGImageElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[3].magic = JSSVGImageElement::GetScreenCTMFuncNum;
    JSSVGImageElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGImageElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
    JSSVGImageElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGImageElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[4].magic = JSSVGImageElement::GetTransformToElementFuncNum;
    JSSVGImageElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGImageElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
    JSSVGImageElementPrototypeFunctions[5].name = "getBBox";
    JSSVGImageElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGImageElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGImageElementPrototypeFunctions[5].magic = JSSVGImageElement::GetBBoxFuncNum;
    JSSVGImageElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGImageElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGImageElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGImageElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGImageElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGImageElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGImageElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGImageElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGImageElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGImageElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGImageElementAttributesFunctions, countof(JSSVGImageElementAttributesFunctions));
    init_JSSVGImageElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGImageElementPrototypeFunctions, countof(JSSVGImageElementPrototypeFunctions));
}

static JSClassDef JSSVGImageElementClassDefine;
static bool JSSVGImageElementClassDefine_initialized = false;

static void init_JSSVGImageElementClassDefine()
{
    if (JSSVGImageElementClassDefine_initialized) return;
    JSSVGImageElementClassDefine_initialized = true;
    memset(&JSSVGImageElementClassDefine, 0, sizeof(JSSVGImageElementClassDefine));
    JSSVGImageElementClassDefine.class_name = "SVGImageElement";
    JSSVGImageElementClassDefine.finalizer = JSSVGImageElement::finalizer;
    JSSVGImageElementClassDefine.gc_mark = JSSVGImageElement::mark;
}

JSClassID JSSVGImageElement::js_class_id = 0;

void JSSVGImageElement::init(JSContext* ctx)
{
    if (JSSVGImageElement::js_class_id == 0) {
        init_JSSVGImageElementClassDefine();
        JS_NewClassID(&JSSVGImageElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGImageElement::js_class_id, &JSSVGImageElementClassDefine);
        JS_SetClassProto(ctx, JSSVGImageElement::js_class_id, JSSVGImageElementPrototype::self(ctx));
    }
}

JSValue JSSVGImageElement::create(JSContext* ctx, SVGImageElement* impl)
{
    JSSVGImageElement::init(ctx);
    JSValue _proto = JSSVGImageElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGImageElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGImageElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGImageElement* impl = (SVGImageElement*)JS_GetOpaque(val, JSSVGImageElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGImageElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGImageElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGImageElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGImageElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGImageElement* imp = (SVGImageElement*)JS_GetOpaque(this_val, JSSVGImageElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGImageElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGImageElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGImageElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGImageElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGImageElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGImageElement::GetTransformToElementFuncNum: {
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
