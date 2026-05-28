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
#include "QJSSVGPatternElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedPreserveAspectRatio.h"
#include "QJSSVGAnimatedRect.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGStringList.h"
#include "SVGPatternElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPatternElementAttributesFunctions[18];
static bool JSSVGPatternElementAttributesFunctions_initialized = false;

static void init_JSSVGPatternElementAttributesFunctions()
{
    if (JSSVGPatternElementAttributesFunctions_initialized) return;
    JSSVGPatternElementAttributesFunctions_initialized = true;
    memset(JSSVGPatternElementAttributesFunctions, 0, sizeof(JSSVGPatternElementAttributesFunctions));
    JSSVGPatternElementAttributesFunctions[0].name = "patternUnits";
    JSSVGPatternElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[0].magic = JSSVGPatternElement::PatternUnitsAttrNum;
    JSSVGPatternElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[1].name = "patternContentUnits";
    JSSVGPatternElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[1].magic = JSSVGPatternElement::PatternContentUnitsAttrNum;
    JSSVGPatternElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[2].name = "patternTransform";
    JSSVGPatternElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[2].magic = JSSVGPatternElement::PatternTransformAttrNum;
    JSSVGPatternElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[3].name = "x";
    JSSVGPatternElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[3].magic = JSSVGPatternElement::XAttrNum;
    JSSVGPatternElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[4].name = "y";
    JSSVGPatternElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[4].magic = JSSVGPatternElement::YAttrNum;
    JSSVGPatternElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[5].name = "width";
    JSSVGPatternElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[5].magic = JSSVGPatternElement::WidthAttrNum;
    JSSVGPatternElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[6].name = "height";
    JSSVGPatternElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[6].magic = JSSVGPatternElement::HeightAttrNum;
    JSSVGPatternElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[7].name = "href";
    JSSVGPatternElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[7].magic = JSSVGPatternElement::HrefAttrNum;
    JSSVGPatternElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[8].name = "requiredFeatures";
    JSSVGPatternElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[8].magic = JSSVGPatternElement::RequiredFeaturesAttrNum;
    JSSVGPatternElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[9].name = "requiredExtensions";
    JSSVGPatternElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[9].magic = JSSVGPatternElement::RequiredExtensionsAttrNum;
    JSSVGPatternElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[10].name = "systemLanguage";
    JSSVGPatternElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[10].magic = JSSVGPatternElement::SystemLanguageAttrNum;
    JSSVGPatternElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[11].name = "xmllang";
    JSSVGPatternElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[11].magic = JSSVGPatternElement::XmllangAttrNum;
    JSSVGPatternElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[11].u.getset.set.setter_magic = JSSVGPatternElement::putValueProperty;
    JSSVGPatternElementAttributesFunctions[12].name = "xmlspace";
    JSSVGPatternElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[12].magic = JSSVGPatternElement::XmlspaceAttrNum;
    JSSVGPatternElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[12].u.getset.set.setter_magic = JSSVGPatternElement::putValueProperty;
    JSSVGPatternElementAttributesFunctions[13].name = "externalResourcesRequired";
    JSSVGPatternElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[13].magic = JSSVGPatternElement::ExternalResourcesRequiredAttrNum;
    JSSVGPatternElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[14].name = "className";
    JSSVGPatternElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[14].magic = JSSVGPatternElement::ClassNameAttrNum;
    JSSVGPatternElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[15].name = "style";
    JSSVGPatternElementAttributesFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[15].magic = JSSVGPatternElement::StyleAttrNum;
    JSSVGPatternElementAttributesFunctions[15].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[16].name = "viewBox";
    JSSVGPatternElementAttributesFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[16].magic = JSSVGPatternElement::ViewBoxAttrNum;
    JSSVGPatternElementAttributesFunctions[16].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[16].u.getset.set.setter_magic = NULL;
    JSSVGPatternElementAttributesFunctions[17].name = "preserveAspectRatio";
    JSSVGPatternElementAttributesFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPatternElementAttributesFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPatternElementAttributesFunctions[17].magic = JSSVGPatternElement::PreserveAspectRatioAttrNum;
    JSSVGPatternElementAttributesFunctions[17].u.getset.get.getter_magic = JSSVGPatternElement::getValueProperty;
    JSSVGPatternElementAttributesFunctions[17].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPatternElementPrototypeFunctions[2];
static bool JSSVGPatternElementPrototypeFunctions_initialized = false;

static void init_JSSVGPatternElementPrototypeFunctions()
{
    if (JSSVGPatternElementPrototypeFunctions_initialized) return;
    JSSVGPatternElementPrototypeFunctions_initialized = true;
    memset(JSSVGPatternElementPrototypeFunctions, 0, sizeof(JSSVGPatternElementPrototypeFunctions));
    JSSVGPatternElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGPatternElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPatternElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPatternElementPrototypeFunctions[0].magic = JSSVGPatternElement::HasExtensionFuncNum;
    JSSVGPatternElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGPatternElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPatternElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPatternElementPrototypeFunction::callAsFunction;
    JSSVGPatternElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGPatternElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPatternElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPatternElementPrototypeFunctions[1].magic = JSSVGPatternElement::GetPresentationAttributeFuncNum;
    JSSVGPatternElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGPatternElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPatternElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPatternElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGPatternElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPatternElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPatternElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPatternElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPatternElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPatternElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPatternElementAttributesFunctions, countof(JSSVGPatternElementAttributesFunctions));
    init_JSSVGPatternElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPatternElementPrototypeFunctions, countof(JSSVGPatternElementPrototypeFunctions));
}

static JSClassDef JSSVGPatternElementClassDefine;
static bool JSSVGPatternElementClassDefine_initialized = false;

static void init_JSSVGPatternElementClassDefine()
{
    if (JSSVGPatternElementClassDefine_initialized) return;
    JSSVGPatternElementClassDefine_initialized = true;
    memset(&JSSVGPatternElementClassDefine, 0, sizeof(JSSVGPatternElementClassDefine));
    JSSVGPatternElementClassDefine.class_name = "SVGPatternElement";
    JSSVGPatternElementClassDefine.finalizer = JSSVGPatternElement::finalizer;
    JSSVGPatternElementClassDefine.gc_mark = JSSVGPatternElement::mark;
}

JSClassID JSSVGPatternElement::js_class_id = 0;

void JSSVGPatternElement::init(JSContext* ctx)
{
    if (JSSVGPatternElement::js_class_id == 0) {
        init_JSSVGPatternElementClassDefine();
        JS_NewClassID(&JSSVGPatternElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPatternElement::js_class_id, &JSSVGPatternElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPatternElement::js_class_id, JSSVGPatternElementPrototype::self(ctx));
    }
}

JSValue JSSVGPatternElement::create(JSContext* ctx, SVGPatternElement* impl)
{
    JSSVGPatternElement::init(ctx);
    JSValue _proto = JSSVGPatternElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPatternElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPatternElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPatternElement* impl = (SVGPatternElement*)JS_GetOpaque(val, JSSVGPatternElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPatternElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGPatternElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PatternUnitsAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->patternUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PatternContentUnitsAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->patternContentUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PatternTransformAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->patternTransformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedPreserveAspectRatio> obj = imp->preserveAspectRatioAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPatternElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPatternElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque(this_val, JSSVGPatternElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPatternElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGPatternElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
