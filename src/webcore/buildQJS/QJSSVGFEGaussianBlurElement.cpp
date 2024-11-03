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
#include "QJSSVGFEGaussianBlurElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEGaussianBlurElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEGaussianBlurElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("stdDeviationY", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::StdDeviationYAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("stdDeviationX", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::StdDeviationXAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEGaussianBlurElement::getValueProperty, NULL, JSSVGFEGaussianBlurElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEGaussianBlurElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction, JSSVGFEGaussianBlurElement::GetPresentationAttributeFuncNum),
    JS_CFUNC_MAGIC_DEF("setStdDeviation", 2, JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction, JSSVGFEGaussianBlurElement::SetStdDeviationFuncNum)
};

JSValue JSSVGFEGaussianBlurElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEGaussianBlurElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEGaussianBlurElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEGaussianBlurElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEGaussianBlurElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEGaussianBlurElementAttributesFunctions, countof(JSSVGFEGaussianBlurElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEGaussianBlurElementPrototypeFunctions, countof(JSSVGFEGaussianBlurElementPrototypeFunctions));
}

static JSClassDef JSSVGFEGaussianBlurElementClassDefine = 
{
    "SVGFEGaussianBlurElement",
    .finalizer = JSSVGFEGaussianBlurElement::finalizer,
    .gc_mark = JSSVGFEGaussianBlurElement::mark,
};

JSClassID JSSVGFEGaussianBlurElement::js_class_id = 0;

void JSSVGFEGaussianBlurElement::init(JSContext* ctx)
{
    if (JSSVGFEGaussianBlurElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEGaussianBlurElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEGaussianBlurElement::js_class_id, &JSSVGFEGaussianBlurElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEGaussianBlurElement::js_class_id, JSSVGFEGaussianBlurElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEGaussianBlurElement::create(JSContext* ctx, SVGFEGaussianBlurElement* impl)
{
    JSSVGFEGaussianBlurElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEGaussianBlurElementPrototype::self(ctx), JSSVGFEGaussianBlurElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEGaussianBlurElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEGaussianBlurElement* impl = (SVGFEGaussianBlurElement*)JS_GetOpaque(val, JSSVGFEGaussianBlurElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEGaussianBlurElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEGaussianBlurElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case StdDeviationXAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->stdDeviationXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StdDeviationYAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->stdDeviationYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEGaussianBlurElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEGaussianBlurElement* imp = (SVGFEGaussianBlurElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEGaussianBlurElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEGaussianBlurElement::SetStdDeviationFuncNum: {
            float stdDeviationX = valueToFloat(ctx, argv[0]);
            float stdDeviationY = valueToFloat(ctx, argv[1]);
            imp->setStdDeviation(stdDeviationX, stdDeviationY);
            return JS_UNDEFINED;
        }
        case JSSVGFEGaussianBlurElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
