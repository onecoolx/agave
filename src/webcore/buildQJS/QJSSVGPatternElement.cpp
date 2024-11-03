/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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

static const JSCFunctionListEntry JSSVGPatternElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("patternContentUnits", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::PatternContentUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGPatternElement::getValueProperty, JSSVGPatternElement::putValueProperty, JSSVGPatternElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGPatternElement::getValueProperty, JSSVGPatternElement::putValueProperty, JSSVGPatternElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("patternTransform", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::PatternTransformAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("patternUnits", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::PatternUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("viewBox", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::ViewBoxAttrNum),
    JS_CGETSET_MAGIC_DEF("preserveAspectRatio", JSSVGPatternElement::getValueProperty, NULL, JSSVGPatternElement::PreserveAspectRatioAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPatternElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGPatternElementPrototypeFunction::callAsFunction, JSSVGPatternElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGPatternElementPrototypeFunction::callAsFunction, JSSVGPatternElement::HasExtensionFuncNum)
};

JSValue JSSVGPatternElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPatternElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGPatternElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPatternElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPatternElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPatternElementAttributesFunctions, countof(JSSVGPatternElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPatternElementPrototypeFunctions, countof(JSSVGPatternElementPrototypeFunctions));
}

static JSClassDef JSSVGPatternElementClassDefine = 
{
    "SVGPatternElement",
    .finalizer = JSSVGPatternElement::finalizer,
    .gc_mark = JSSVGPatternElement::mark,
};

JSClassID JSSVGPatternElement::js_class_id = 0;

void JSSVGPatternElement::init(JSContext* ctx)
{
    if (JSSVGPatternElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGPatternElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPatternElement::js_class_id, &JSSVGPatternElementClassDefine);
        JS_SetClassProto(ctx, JSSVGPatternElement::js_class_id, JSSVGPatternElementPrototype::self(ctx));
    }
}

JSValue JSSVGPatternElement::create(JSContext* ctx, SVGPatternElement* impl)
{
    JSSVGPatternElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPatternElementPrototype::self(ctx), JSSVGPatternElement::js_class_id);
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
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->patternUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PatternContentUnitsAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->patternContentUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PatternTransformAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->patternTransformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ViewBoxAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            RefPtr<SVGAnimatedRect> obj = imp->viewBoxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PreserveAspectRatioAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
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
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGPatternElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPatternElement* imp = (SVGPatternElement*)JS_GetOpaque2(ctx, this_val, JSSVGPatternElement::js_class_id);
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
