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
#include "QJSSVGMaskElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGStringList.h"
#include "SVGMaskElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGMaskElementAttributesFunctions[14];
static bool JSSVGMaskElementAttributesFunctions_initialized = false;

static void init_JSSVGMaskElementAttributesFunctions()
{
    if (JSSVGMaskElementAttributesFunctions_initialized) return;
    JSSVGMaskElementAttributesFunctions_initialized = true;
    memset(JSSVGMaskElementAttributesFunctions, 0, sizeof(JSSVGMaskElementAttributesFunctions));
    JSSVGMaskElementAttributesFunctions[0].name = "maskUnits";
    JSSVGMaskElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[0].magic = JSSVGMaskElement::MaskUnitsAttrNum;
    JSSVGMaskElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[1].name = "maskContentUnits";
    JSSVGMaskElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[1].magic = JSSVGMaskElement::MaskContentUnitsAttrNum;
    JSSVGMaskElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[2].name = "x";
    JSSVGMaskElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[2].magic = JSSVGMaskElement::XAttrNum;
    JSSVGMaskElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[3].name = "y";
    JSSVGMaskElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[3].magic = JSSVGMaskElement::YAttrNum;
    JSSVGMaskElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[4].name = "width";
    JSSVGMaskElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[4].magic = JSSVGMaskElement::WidthAttrNum;
    JSSVGMaskElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[5].name = "height";
    JSSVGMaskElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[5].magic = JSSVGMaskElement::HeightAttrNum;
    JSSVGMaskElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[6].name = "requiredFeatures";
    JSSVGMaskElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[6].magic = JSSVGMaskElement::RequiredFeaturesAttrNum;
    JSSVGMaskElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[7].name = "requiredExtensions";
    JSSVGMaskElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[7].magic = JSSVGMaskElement::RequiredExtensionsAttrNum;
    JSSVGMaskElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[8].name = "systemLanguage";
    JSSVGMaskElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[8].magic = JSSVGMaskElement::SystemLanguageAttrNum;
    JSSVGMaskElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[9].name = "xmllang";
    JSSVGMaskElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[9].magic = JSSVGMaskElement::XmllangAttrNum;
    JSSVGMaskElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[9].u.getset.set.setter_magic = JSSVGMaskElement::putValueProperty;
    JSSVGMaskElementAttributesFunctions[10].name = "xmlspace";
    JSSVGMaskElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[10].magic = JSSVGMaskElement::XmlspaceAttrNum;
    JSSVGMaskElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[10].u.getset.set.setter_magic = JSSVGMaskElement::putValueProperty;
    JSSVGMaskElementAttributesFunctions[11].name = "externalResourcesRequired";
    JSSVGMaskElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[11].magic = JSSVGMaskElement::ExternalResourcesRequiredAttrNum;
    JSSVGMaskElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[12].name = "className";
    JSSVGMaskElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[12].magic = JSSVGMaskElement::ClassNameAttrNum;
    JSSVGMaskElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGMaskElementAttributesFunctions[13].name = "style";
    JSSVGMaskElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMaskElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMaskElementAttributesFunctions[13].magic = JSSVGMaskElement::StyleAttrNum;
    JSSVGMaskElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGMaskElement::getValueProperty;
    JSSVGMaskElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGMaskElementPrototypeFunctions[2];
static bool JSSVGMaskElementPrototypeFunctions_initialized = false;

static void init_JSSVGMaskElementPrototypeFunctions()
{
    if (JSSVGMaskElementPrototypeFunctions_initialized) return;
    JSSVGMaskElementPrototypeFunctions_initialized = true;
    memset(JSSVGMaskElementPrototypeFunctions, 0, sizeof(JSSVGMaskElementPrototypeFunctions));
    JSSVGMaskElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGMaskElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMaskElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGMaskElementPrototypeFunctions[0].magic = JSSVGMaskElement::HasExtensionFuncNum;
    JSSVGMaskElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGMaskElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMaskElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGMaskElementPrototypeFunction::callAsFunction;
    JSSVGMaskElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGMaskElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMaskElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGMaskElementPrototypeFunctions[1].magic = JSSVGMaskElement::GetPresentationAttributeFuncNum;
    JSSVGMaskElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGMaskElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMaskElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGMaskElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGMaskElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMaskElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGMaskElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMaskElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGMaskElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGMaskElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMaskElementAttributesFunctions, countof(JSSVGMaskElementAttributesFunctions));
    init_JSSVGMaskElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMaskElementPrototypeFunctions, countof(JSSVGMaskElementPrototypeFunctions));
}

static JSClassDef JSSVGMaskElementClassDefine;
static bool JSSVGMaskElementClassDefine_initialized = false;

static void init_JSSVGMaskElementClassDefine()
{
    if (JSSVGMaskElementClassDefine_initialized) return;
    JSSVGMaskElementClassDefine_initialized = true;
    memset(&JSSVGMaskElementClassDefine, 0, sizeof(JSSVGMaskElementClassDefine));
    JSSVGMaskElementClassDefine.class_name = "SVGMaskElement";
    JSSVGMaskElementClassDefine.finalizer = JSSVGMaskElement::finalizer;
    JSSVGMaskElementClassDefine.gc_mark = JSSVGMaskElement::mark;
}

JSClassID JSSVGMaskElement::js_class_id = 0;

void JSSVGMaskElement::init(JSContext* ctx)
{
    if (JSSVGMaskElement::js_class_id == 0) {
        init_JSSVGMaskElementClassDefine();
        JS_NewClassID(&JSSVGMaskElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMaskElement::js_class_id, &JSSVGMaskElementClassDefine);
        JS_SetClassProto(ctx, JSSVGMaskElement::js_class_id, JSSVGMaskElementPrototype::self(ctx));
    }
}

JSValue JSSVGMaskElement::create(JSContext* ctx, SVGMaskElement* impl)
{
    JSSVGMaskElement::init(ctx);
    JSValue _proto = JSSVGMaskElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGMaskElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGMaskElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGMaskElement* impl = (SVGMaskElement*)JS_GetOpaque(val, JSSVGMaskElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGMaskElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGMaskElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case MaskUnitsAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->maskUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MaskContentUnitsAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->maskContentUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGMaskElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGMaskElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque(this_val, JSSVGMaskElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGMaskElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGMaskElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
