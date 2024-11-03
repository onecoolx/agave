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
#include "QJSSVGFEDiffuseLightingElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEDiffuseLightingElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEDiffuseLightingElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("surfaceScale", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::SurfaceScaleAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("diffuseConstant", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::DiffuseConstantAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("kernelUnitLengthY", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::KernelUnitLengthYAttrNum),
    JS_CGETSET_MAGIC_DEF("kernelUnitLengthX", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::KernelUnitLengthXAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEDiffuseLightingElement::getValueProperty, NULL, JSSVGFEDiffuseLightingElement::ClassNameAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEDiffuseLightingElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEDiffuseLightingElementPrototypeFunction::callAsFunction, JSSVGFEDiffuseLightingElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEDiffuseLightingElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEDiffuseLightingElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEDiffuseLightingElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEDiffuseLightingElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEDiffuseLightingElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDiffuseLightingElementAttributesFunctions, countof(JSSVGFEDiffuseLightingElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDiffuseLightingElementPrototypeFunctions, countof(JSSVGFEDiffuseLightingElementPrototypeFunctions));
}

static JSClassDef JSSVGFEDiffuseLightingElementClassDefine = 
{
    "SVGFEDiffuseLightingElement",
    .finalizer = JSSVGFEDiffuseLightingElement::finalizer,
    .gc_mark = JSSVGFEDiffuseLightingElement::mark,
};

JSClassID JSSVGFEDiffuseLightingElement::js_class_id = 0;

void JSSVGFEDiffuseLightingElement::init(JSContext* ctx)
{
    if (JSSVGFEDiffuseLightingElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEDiffuseLightingElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEDiffuseLightingElement::js_class_id, &JSSVGFEDiffuseLightingElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEDiffuseLightingElement::js_class_id, JSSVGFEDiffuseLightingElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEDiffuseLightingElement::create(JSContext* ctx, SVGFEDiffuseLightingElement* impl)
{
    JSSVGFEDiffuseLightingElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEDiffuseLightingElementPrototype::self(ctx), JSSVGFEDiffuseLightingElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEDiffuseLightingElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEDiffuseLightingElement* impl = (SVGFEDiffuseLightingElement*)JS_GetOpaque(val, JSSVGFEDiffuseLightingElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEDiffuseLightingElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEDiffuseLightingElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case SurfaceScaleAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->surfaceScaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DiffuseConstantAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->diffuseConstantAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case KernelUnitLengthXAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->kernelUnitLengthXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case KernelUnitLengthYAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->kernelUnitLengthYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEDiffuseLightingElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDiffuseLightingElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEDiffuseLightingElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
