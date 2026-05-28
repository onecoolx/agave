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
#include "QJSSVGClipPathElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "QJSSVGStringList.h"
#include "SVGClipPathElement.h"
#include "SVGElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGClipPathElementAttributesFunctions[12];
static bool JSSVGClipPathElementAttributesFunctions_initialized = false;

static void init_JSSVGClipPathElementAttributesFunctions()
{
    if (JSSVGClipPathElementAttributesFunctions_initialized) return;
    JSSVGClipPathElementAttributesFunctions_initialized = true;
    memset(JSSVGClipPathElementAttributesFunctions, 0, sizeof(JSSVGClipPathElementAttributesFunctions));
    JSSVGClipPathElementAttributesFunctions[0].name = "clipPathUnits";
    JSSVGClipPathElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[0].magic = JSSVGClipPathElement::ClipPathUnitsAttrNum;
    JSSVGClipPathElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[1].name = "requiredFeatures";
    JSSVGClipPathElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[1].magic = JSSVGClipPathElement::RequiredFeaturesAttrNum;
    JSSVGClipPathElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[2].name = "requiredExtensions";
    JSSVGClipPathElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[2].magic = JSSVGClipPathElement::RequiredExtensionsAttrNum;
    JSSVGClipPathElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[3].name = "systemLanguage";
    JSSVGClipPathElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[3].magic = JSSVGClipPathElement::SystemLanguageAttrNum;
    JSSVGClipPathElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[4].name = "xmllang";
    JSSVGClipPathElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[4].magic = JSSVGClipPathElement::XmllangAttrNum;
    JSSVGClipPathElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[4].u.getset.set.setter_magic = JSSVGClipPathElement::putValueProperty;
    JSSVGClipPathElementAttributesFunctions[5].name = "xmlspace";
    JSSVGClipPathElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[5].magic = JSSVGClipPathElement::XmlspaceAttrNum;
    JSSVGClipPathElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[5].u.getset.set.setter_magic = JSSVGClipPathElement::putValueProperty;
    JSSVGClipPathElementAttributesFunctions[6].name = "externalResourcesRequired";
    JSSVGClipPathElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[6].magic = JSSVGClipPathElement::ExternalResourcesRequiredAttrNum;
    JSSVGClipPathElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[7].name = "className";
    JSSVGClipPathElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[7].magic = JSSVGClipPathElement::ClassNameAttrNum;
    JSSVGClipPathElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[8].name = "style";
    JSSVGClipPathElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[8].magic = JSSVGClipPathElement::StyleAttrNum;
    JSSVGClipPathElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[9].name = "transform";
    JSSVGClipPathElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[9].magic = JSSVGClipPathElement::TransformAttrNum;
    JSSVGClipPathElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[10].name = "nearestViewportElement";
    JSSVGClipPathElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[10].magic = JSSVGClipPathElement::NearestViewportElementAttrNum;
    JSSVGClipPathElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGClipPathElementAttributesFunctions[11].name = "farthestViewportElement";
    JSSVGClipPathElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGClipPathElementAttributesFunctions[11].magic = JSSVGClipPathElement::FarthestViewportElementAttrNum;
    JSSVGClipPathElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGClipPathElement::getValueProperty;
    JSSVGClipPathElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGClipPathElementPrototypeFunctions[6];
static bool JSSVGClipPathElementPrototypeFunctions_initialized = false;

static void init_JSSVGClipPathElementPrototypeFunctions()
{
    if (JSSVGClipPathElementPrototypeFunctions_initialized) return;
    JSSVGClipPathElementPrototypeFunctions_initialized = true;
    memset(JSSVGClipPathElementPrototypeFunctions, 0, sizeof(JSSVGClipPathElementPrototypeFunctions));
    JSSVGClipPathElementPrototypeFunctions[0].name = "hasExtension";
    JSSVGClipPathElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[0].magic = JSSVGClipPathElement::HasExtensionFuncNum;
    JSSVGClipPathElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGClipPathElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
    JSSVGClipPathElementPrototypeFunctions[1].name = "getPresentationAttribute";
    JSSVGClipPathElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[1].magic = JSSVGClipPathElement::GetPresentationAttributeFuncNum;
    JSSVGClipPathElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGClipPathElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
    JSSVGClipPathElementPrototypeFunctions[2].name = "getBBox";
    JSSVGClipPathElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[2].magic = JSSVGClipPathElement::GetBBoxFuncNum;
    JSSVGClipPathElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGClipPathElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
    JSSVGClipPathElementPrototypeFunctions[3].name = "getCTM";
    JSSVGClipPathElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[3].magic = JSSVGClipPathElement::GetCTMFuncNum;
    JSSVGClipPathElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGClipPathElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
    JSSVGClipPathElementPrototypeFunctions[4].name = "getScreenCTM";
    JSSVGClipPathElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[4].magic = JSSVGClipPathElement::GetScreenCTMFuncNum;
    JSSVGClipPathElementPrototypeFunctions[4].u.func.length = 0;
    JSSVGClipPathElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
    JSSVGClipPathElementPrototypeFunctions[5].name = "getTransformToElement";
    JSSVGClipPathElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGClipPathElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGClipPathElementPrototypeFunctions[5].magic = JSSVGClipPathElement::GetTransformToElementFuncNum;
    JSSVGClipPathElementPrototypeFunctions[5].u.func.length = 1;
    JSSVGClipPathElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGClipPathElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGClipPathElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGClipPathElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGClipPathElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGClipPathElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGClipPathElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGClipPathElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGClipPathElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGClipPathElementAttributesFunctions, countof(JSSVGClipPathElementAttributesFunctions));
    init_JSSVGClipPathElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGClipPathElementPrototypeFunctions, countof(JSSVGClipPathElementPrototypeFunctions));
}

static JSClassDef JSSVGClipPathElementClassDefine;
static bool JSSVGClipPathElementClassDefine_initialized = false;

static void init_JSSVGClipPathElementClassDefine()
{
    if (JSSVGClipPathElementClassDefine_initialized) return;
    JSSVGClipPathElementClassDefine_initialized = true;
    memset(&JSSVGClipPathElementClassDefine, 0, sizeof(JSSVGClipPathElementClassDefine));
    JSSVGClipPathElementClassDefine.class_name = "SVGClipPathElement";
    JSSVGClipPathElementClassDefine.finalizer = JSSVGClipPathElement::finalizer;
    JSSVGClipPathElementClassDefine.gc_mark = JSSVGClipPathElement::mark;
}

JSClassID JSSVGClipPathElement::js_class_id = 0;

void JSSVGClipPathElement::init(JSContext* ctx)
{
    if (JSSVGClipPathElement::js_class_id == 0) {
        init_JSSVGClipPathElementClassDefine();
        JS_NewClassID(&JSSVGClipPathElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGClipPathElement::js_class_id, &JSSVGClipPathElementClassDefine);
        JS_SetClassProto(ctx, JSSVGClipPathElement::js_class_id, JSSVGClipPathElementPrototype::self(ctx));
    }
}

JSValue JSSVGClipPathElement::create(JSContext* ctx, SVGClipPathElement* impl)
{
    JSSVGClipPathElement::init(ctx);
    JSValue _proto = JSSVGClipPathElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGClipPathElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGClipPathElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGClipPathElement* impl = (SVGClipPathElement*)JS_GetOpaque(val, JSSVGClipPathElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGClipPathElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGClipPathElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ClipPathUnitsAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->clipPathUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGClipPathElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGClipPathElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGClipPathElement* imp = (SVGClipPathElement*)JS_GetOpaque(this_val, JSSVGClipPathElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGClipPathElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGClipPathElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGClipPathElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGClipPathElement::GetTransformToElementFuncNum: {
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
