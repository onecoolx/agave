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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGFESpecularLightingElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFESpecularLightingElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFESpecularLightingElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("specularConstant", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::SpecularConstantAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("surfaceScale", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::SurfaceScaleAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("specularExponent", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::SpecularExponentAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFESpecularLightingElement::getValueProperty, NULL, JSSVGFESpecularLightingElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFESpecularLightingElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFESpecularLightingElementPrototypeFunction::callAsFunction, JSSVGFESpecularLightingElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFESpecularLightingElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFESpecularLightingElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFESpecularLightingElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFESpecularLightingElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFESpecularLightingElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpecularLightingElementAttributesFunctions, countof(JSSVGFESpecularLightingElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpecularLightingElementPrototypeFunctions, countof(JSSVGFESpecularLightingElementPrototypeFunctions));
}

static JSClassDef JSSVGFESpecularLightingElementClassDefine = 
{
    "SVGFESpecularLightingElement",
    .finalizer = JSSVGFESpecularLightingElement::finalizer,
    .gc_mark = JSSVGFESpecularLightingElement::mark,
};

JSClassID JSSVGFESpecularLightingElement::js_class_id = 0;

void JSSVGFESpecularLightingElement::init(JSContext* ctx)
{
    if (JSSVGFESpecularLightingElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFESpecularLightingElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFESpecularLightingElement::js_class_id, &JSSVGFESpecularLightingElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFESpecularLightingElement::js_class_id, JSSVGFESpecularLightingElementPrototype::self(ctx));
    }
}

JSValue JSSVGFESpecularLightingElement::create(JSContext* ctx, SVGFESpecularLightingElement* impl)
{
    JSSVGFESpecularLightingElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFESpecularLightingElementPrototype::self(ctx), JSSVGFESpecularLightingElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFESpecularLightingElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFESpecularLightingElement* impl = (SVGFESpecularLightingElement*)JS_GetOpaque(val, JSSVGFESpecularLightingElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFESpecularLightingElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFESpecularLightingElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case SurfaceScaleAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->surfaceScaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularConstantAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularConstantAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularExponentAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularExponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFESpecularLightingElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFESpecularLightingElement* imp = (SVGFESpecularLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpecularLightingElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFESpecularLightingElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
