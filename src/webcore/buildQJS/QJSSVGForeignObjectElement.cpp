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
#include "QJSSVGForeignObjectElement.h"

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
#include "SVGForeignObjectElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGForeignObjectElementAttributesFunctions[15];
static bool JSSVGForeignObjectElementAttributesFunctions_initialized = false;

static void init_JSSVGForeignObjectElementAttributesFunctions()
{
    if (JSSVGForeignObjectElementAttributesFunctions_initialized) return;
    JSSVGForeignObjectElementAttributesFunctions_initialized = true;
    memset(JSSVGForeignObjectElementAttributesFunctions, 0, sizeof(JSSVGForeignObjectElementAttributesFunctions));
    JSSVGForeignObjectElementAttributesFunctions[0].name = "x";
    JSSVGForeignObjectElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[0].magic = JSSVGForeignObjectElement::XAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[1].name = "y";
    JSSVGForeignObjectElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[1].magic = JSSVGForeignObjectElement::YAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[2].name = "width";
    JSSVGForeignObjectElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[2].magic = JSSVGForeignObjectElement::WidthAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[3].name = "height";
    JSSVGForeignObjectElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[3].magic = JSSVGForeignObjectElement::HeightAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[4].name = "requiredFeatures";
    JSSVGForeignObjectElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[4].magic = JSSVGForeignObjectElement::RequiredFeaturesAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[5].name = "requiredExtensions";
    JSSVGForeignObjectElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[5].magic = JSSVGForeignObjectElement::RequiredExtensionsAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[6].name = "systemLanguage";
    JSSVGForeignObjectElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[6].magic = JSSVGForeignObjectElement::SystemLanguageAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[7].name = "xmllang";
    JSSVGForeignObjectElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[7].magic = JSSVGForeignObjectElement::XmllangAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[7].u.getset.set.setter_magic = JSSVGForeignObjectElement::putValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[8].name = "xmlspace";
    JSSVGForeignObjectElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[8].magic = JSSVGForeignObjectElement::XmlspaceAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[8].u.getset.set.setter_magic = JSSVGForeignObjectElement::putValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[9].name = "externalResourcesRequired";
    JSSVGForeignObjectElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[9].magic = JSSVGForeignObjectElement::ExternalResourcesRequiredAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[10].name = "className";
    JSSVGForeignObjectElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[10].magic = JSSVGForeignObjectElement::ClassNameAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[11].name = "style";
    JSSVGForeignObjectElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[11].magic = JSSVGForeignObjectElement::StyleAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[12].name = "transform";
    JSSVGForeignObjectElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[12].magic = JSSVGForeignObjectElement::TransformAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[13].name = "nearestViewportElement";
    JSSVGForeignObjectElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[13].magic = JSSVGForeignObjectElement::NearestViewportElementAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGForeignObjectElementAttributesFunctions[14].name = "farthestViewportElement";
    JSSVGForeignObjectElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGForeignObjectElementAttributesFunctions[14].magic = JSSVGForeignObjectElement::FarthestViewportElementAttrNum;
    JSSVGForeignObjectElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGForeignObjectElement::getValueProperty;
    JSSVGForeignObjectElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGForeignObjectElementPrototypeFunctions[6];
static bool JSSVGForeignObjectElementPrototypeFunctions_initialized = false;

static void init_JSSVGForeignObjectElementPrototypeFunctions()
{
    if (JSSVGForeignObjectElementPrototypeFunctions_initialized) return;
    JSSVGForeignObjectElementPrototypeFunctions_initialized = true;
    memset(JSSVGForeignObjectElementPrototypeFunctions, 0, sizeof(JSSVGForeignObjectElementPrototypeFunctions));
    JSSVGForeignObjectElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGForeignObjectElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[0].magic = JSSVGForeignObjectElement::HasExtensionFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGForeignObjectElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
    JSSVGForeignObjectElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGForeignObjectElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[1].magic = JSSVGForeignObjectElement::GetPresentationAttributeFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGForeignObjectElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
    JSSVGForeignObjectElementPrototypeFunctions[2].name = "getBBox";
    JSSVGForeignObjectElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[2].magic = JSSVGForeignObjectElement::GetBBoxFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGForeignObjectElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
    JSSVGForeignObjectElementPrototypeFunctions[3].name = "getCTM";
    JSSVGForeignObjectElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[3].magic = JSSVGForeignObjectElement::GetCTMFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGForeignObjectElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
    JSSVGForeignObjectElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGForeignObjectElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[4].magic = JSSVGForeignObjectElement::GetScreenCTMFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGForeignObjectElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
    JSSVGForeignObjectElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGForeignObjectElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGForeignObjectElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGForeignObjectElementPrototypeFunctions[5].magic = JSSVGForeignObjectElement::GetTransformToElementFuncNum;
    JSSVGForeignObjectElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGForeignObjectElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGForeignObjectElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGForeignObjectElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGForeignObjectElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGForeignObjectElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGForeignObjectElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGForeignObjectElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGForeignObjectElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGForeignObjectElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGForeignObjectElementAttributesFunctions, countof(JSSVGForeignObjectElementAttributesFunctions));
    init_JSSVGForeignObjectElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGForeignObjectElementPrototypeFunctions, countof(JSSVGForeignObjectElementPrototypeFunctions));
}

static JSClassDef JSSVGForeignObjectElementClassDefine;
static bool JSSVGForeignObjectElementClassDefine_initialized = false;

static void init_JSSVGForeignObjectElementClassDefine()
{
    if (JSSVGForeignObjectElementClassDefine_initialized) return;
    JSSVGForeignObjectElementClassDefine_initialized = true;
    memset(&JSSVGForeignObjectElementClassDefine, 0, sizeof(JSSVGForeignObjectElementClassDefine));
    JSSVGForeignObjectElementClassDefine.class_name = "SVGForeignObjectElement";
    JSSVGForeignObjectElementClassDefine.finalizer = JSSVGForeignObjectElement::finalizer;
    JSSVGForeignObjectElementClassDefine.gc_mark = JSSVGForeignObjectElement::mark;
}

JSClassID JSSVGForeignObjectElement::js_class_id = 0;

void JSSVGForeignObjectElement::init(JSContext* ctx)
{
    if (JSSVGForeignObjectElement::js_class_id == 0) {
        init_JSSVGForeignObjectElementClassDefine();
        JS_NewClassID(&JSSVGForeignObjectElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGForeignObjectElement::js_class_id, &JSSVGForeignObjectElementClassDefine);
        JS_SetClassProto(ctx, JSSVGForeignObjectElement::js_class_id, JSSVGForeignObjectElementPrototype::self(ctx));
    }
}

JSValue JSSVGForeignObjectElement::create(JSContext* ctx, SVGForeignObjectElement* impl)
{
    JSSVGForeignObjectElement::init(ctx);
    JSValue _proto = JSSVGForeignObjectElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGForeignObjectElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGForeignObjectElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGForeignObjectElement* impl = (SVGForeignObjectElement*)JS_GetOpaque(val, JSSVGForeignObjectElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGForeignObjectElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGForeignObjectElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGForeignObjectElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGForeignObjectElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGForeignObjectElement* imp = (SVGForeignObjectElement*)JS_GetOpaque(this_val, JSSVGForeignObjectElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGForeignObjectElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGForeignObjectElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGForeignObjectElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGForeignObjectElement::GetTransformToElementFuncNum: {
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
