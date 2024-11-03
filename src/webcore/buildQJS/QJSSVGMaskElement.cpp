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

static const JSCFunctionListEntry JSSVGMaskElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("xmlspace", JSSVGMaskElement::getValueProperty, JSSVGMaskElement::putValueProperty, JSSVGMaskElement::XmlspaceAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("maskContentUnits", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::MaskContentUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("maskUnits", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::MaskUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("xmllang", JSSVGMaskElement::getValueProperty, JSSVGMaskElement::putValueProperty, JSSVGMaskElement::XmllangAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredExtensions", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::RequiredExtensionsAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("requiredFeatures", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::RequiredFeaturesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemLanguage", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::SystemLanguageAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGMaskElement::getValueProperty, NULL, JSSVGMaskElement::ExternalResourcesRequiredAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGMaskElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGMaskElementPrototypeFunction::callAsFunction, JSSVGMaskElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasExtension", 1, JSSVGMaskElementPrototypeFunction::callAsFunction, JSSVGMaskElement::HasExtensionFuncNum)
};

JSValue JSSVGMaskElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMaskElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGMaskElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMaskElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGMaskElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMaskElementAttributesFunctions, countof(JSSVGMaskElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMaskElementPrototypeFunctions, countof(JSSVGMaskElementPrototypeFunctions));
}

static JSClassDef JSSVGMaskElementClassDefine = 
{
    "SVGMaskElement",
    .finalizer = JSSVGMaskElement::finalizer,
    .gc_mark = JSSVGMaskElement::mark,
};

JSClassID JSSVGMaskElement::js_class_id = 0;

void JSSVGMaskElement::init(JSContext* ctx)
{
    if (JSSVGMaskElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGMaskElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMaskElement::js_class_id, &JSSVGMaskElementClassDefine);
        JS_SetClassProto(ctx, JSSVGMaskElement::js_class_id, JSSVGMaskElementPrototype::self(ctx));
    }
}

JSValue JSSVGMaskElement::create(JSContext* ctx, SVGMaskElement* impl)
{
    JSSVGMaskElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGMaskElementPrototype::self(ctx), JSSVGMaskElement::js_class_id);
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
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->maskUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case MaskContentUnitsAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->maskContentUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RequiredFeaturesAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredFeatures()), imp);
        }
        case RequiredExtensionsAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->requiredExtensions()), imp);
        }
        case SystemLanguageAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->systemLanguage()), imp);
        }
        case XmllangAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmllang()).utf8().data());
        }
        case XmlspaceAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->xmlspace()).utf8().data());
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGMaskElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XmllangAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            imp->setXmllang(valueToString(ctx, value));
            break;
        }
        case XmlspaceAttrNum: {
            SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
            imp->setXmlspace(valueToString(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSSVGMaskElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGMaskElement* imp = (SVGMaskElement*)JS_GetOpaque2(ctx, this_val, JSSVGMaskElement::js_class_id);
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
