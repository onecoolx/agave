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
#include "QJSSVGLineElement.h"

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
#include "SVGLineElement.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGLineElementAttributesFunctions[15];
static bool JSSVGLineElementAttributesFunctions_initialized = false;

static void init_JSSVGLineElementAttributesFunctions()
{
    if (JSSVGLineElementAttributesFunctions_initialized) return;
    JSSVGLineElementAttributesFunctions_initialized = true;
    memset(JSSVGLineElementAttributesFunctions, 0, sizeof(JSSVGLineElementAttributesFunctions));
    JSSVGLineElementAttributesFunctions[0].name = "xmllang";
    JSSVGLineElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[0].magic = JSSVGLineElement::XmllangAttrNum;
    JSSVGLineElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[0].u.getset.set.setter_magic = JSSVGLineElement::putValueProperty;
    JSSVGLineElementAttributesFunctions[1].name = "xmlspace";
    JSSVGLineElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[1].magic = JSSVGLineElement::XmlspaceAttrNum;
    JSSVGLineElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[1].u.getset.set.setter_magic = JSSVGLineElement::putValueProperty;
    JSSVGLineElementAttributesFunctions[2].name = "transform";
    JSSVGLineElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[2].magic = JSSVGLineElement::TransformAttrNum;
    JSSVGLineElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[3].name = "externalResourcesRequired";
    JSSVGLineElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[3].magic = JSSVGLineElement::ExternalResourcesRequiredAttrNum;
    JSSVGLineElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[4].name = "x1";
    JSSVGLineElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[4].magic = JSSVGLineElement::X1AttrNum;
    JSSVGLineElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[5].name = "x2";
    JSSVGLineElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[5].magic = JSSVGLineElement::X2AttrNum;
    JSSVGLineElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[6].name = "requiredExtensions";
    JSSVGLineElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[6].magic = JSSVGLineElement::RequiredExtensionsAttrNum;
    JSSVGLineElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[7].name = "requiredFeatures";
    JSSVGLineElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[7].magic = JSSVGLineElement::RequiredFeaturesAttrNum;
    JSSVGLineElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[8].name = "style";
    JSSVGLineElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[8].magic = JSSVGLineElement::StyleAttrNum;
    JSSVGLineElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[9].name = "y1";
    JSSVGLineElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[9].magic = JSSVGLineElement::Y1AttrNum;
    JSSVGLineElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[10].name = "farthestViewportElement";
    JSSVGLineElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[10].magic = JSSVGLineElement::FarthestViewportElementAttrNum;
    JSSVGLineElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[11].name = "y2";
    JSSVGLineElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[11].magic = JSSVGLineElement::Y2AttrNum;
    JSSVGLineElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[12].name = "systemLanguage";
    JSSVGLineElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[12].magic = JSSVGLineElement::SystemLanguageAttrNum;
    JSSVGLineElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[13].name = "className";
    JSSVGLineElementAttributesFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[13].magic = JSSVGLineElement::ClassNameAttrNum;
    JSSVGLineElementAttributesFunctions[13].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGLineElementAttributesFunctions[14].name = "nearestViewportElement";
    JSSVGLineElementAttributesFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLineElementAttributesFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLineElementAttributesFunctions[14].magic = JSSVGLineElement::NearestViewportElementAttrNum;
    JSSVGLineElementAttributesFunctions[14].u.getset.get.getter_magic = JSSVGLineElement::getValueProperty;
    JSSVGLineElementAttributesFunctions[14].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGLineElementPrototypeFunctions[6];
static bool JSSVGLineElementPrototypeFunctions_initialized = false;

static void init_JSSVGLineElementPrototypeFunctions()
{
    if (JSSVGLineElementPrototypeFunctions_initialized) return;
    JSSVGLineElementPrototypeFunctions_initialized = true;
    memset(JSSVGLineElementPrototypeFunctions, 0, sizeof(JSSVGLineElementPrototypeFunctions));
    JSSVGLineElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGLineElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[0].magic = JSSVGLineElement::GetPresentationAttributeFuncNum;
    JSSVGLineElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGLineElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
    JSSVGLineElementPrototypeFunctions[1].name = "hasExtension";
    JSSVGLineElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[1].magic = JSSVGLineElement::HasExtensionFuncNum;
    JSSVGLineElementPrototypeFunctions[1].u.func.length = 1;
    JSSVGLineElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
    JSSVGLineElementPrototypeFunctions[2].name = "getCTM";
    JSSVGLineElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[2].magic = JSSVGLineElement::GetCTMFuncNum;
    JSSVGLineElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGLineElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
    JSSVGLineElementPrototypeFunctions[3].name = "getScreenCTM";
    JSSVGLineElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[3].magic = JSSVGLineElement::GetScreenCTMFuncNum;
    JSSVGLineElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGLineElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
    JSSVGLineElementPrototypeFunctions[4].name = "getTransformToElement";
    JSSVGLineElementPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[4].magic = JSSVGLineElement::GetTransformToElementFuncNum;
    JSSVGLineElementPrototypeFunctions[4].u.func.length = 1;
    JSSVGLineElementPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
    JSSVGLineElementPrototypeFunctions[5].name = "getBBox";
    JSSVGLineElementPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLineElementPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGLineElementPrototypeFunctions[5].magic = JSSVGLineElement::GetBBoxFuncNum;
    JSSVGLineElementPrototypeFunctions[5].u.func.length = 0;
    JSSVGLineElementPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLineElementPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGLineElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGLineElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLineElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGLineElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLineElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGLineElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGLineElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLineElementAttributesFunctions, countof(JSSVGLineElementAttributesFunctions));
    init_JSSVGLineElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLineElementPrototypeFunctions, countof(JSSVGLineElementPrototypeFunctions));
}

static JSClassDef JSSVGLineElementClassDefine;
static bool JSSVGLineElementClassDefine_initialized = false;

static void init_JSSVGLineElementClassDefine()
{
    if (JSSVGLineElementClassDefine_initialized) return;
    JSSVGLineElementClassDefine_initialized = true;
    memset(&JSSVGLineElementClassDefine, 0, sizeof(JSSVGLineElementClassDefine));
    JSSVGLineElementClassDefine.class_name = "SVGLineElement";
    JSSVGLineElementClassDefine.finalizer = JSSVGLineElement::finalizer;
    JSSVGLineElementClassDefine.gc_mark = JSSVGLineElement::mark;
}

JSClassID JSSVGLineElement::js_class_id = 0;

void JSSVGLineElement::init(JSContext* ctx)
{
    if (JSSVGLineElement::js_class_id == 0) {
        init_JSSVGLineElementClassDefine();
        JS_NewClassID(&JSSVGLineElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLineElement::js_class_id, &JSSVGLineElementClassDefine);
        JS_SetClassProto(ctx, JSSVGLineElement::js_class_id, JSSVGLineElementPrototype::self(ctx));
    }
}

JSValue JSSVGLineElement::create(JSContext* ctx, SVGLineElement* impl)
{
    JSSVGLineElement::init(ctx);
    JSValue _proto = JSSVGLineElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGLineElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGLineElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGLineElement* impl = (SVGLineElement*)JS_GetOpaque(val, JSSVGLineElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGLineElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGLineElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case X1AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y1AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case X2AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->x2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case Y2AttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->y2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case TransformAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGLineElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGLineElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGLineElement* imp = (SVGLineElement*)JS_GetOpaque(this_val, JSSVGLineElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGLineElement::HasExtensionFuncNum: {
            String extension = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->hasExtension(extension) ? 1 : 0);
            return result;
        }
        case JSSVGLineElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
        case JSSVGLineElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGLineElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGLineElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGLineElement::GetTransformToElementFuncNum: {
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
