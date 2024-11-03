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
#include "QJSSVGFEColorMatrixElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumberList.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEColorMatrixElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEColorMatrixElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("values", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::ValuesAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGFEColorMatrixElement::getValueProperty, NULL, JSSVGFEColorMatrixElement::ConstructorAttrNum)
};

class JSSVGFEColorMatrixElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFEColorMatrixElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGFEColorMatrixElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_UNKNOWN", JSSVGFEColorMatrixElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_SATURATE", JSSVGFEColorMatrixElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_SATURATE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_MATRIX", JSSVGFEColorMatrixElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_MATRIX),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_HUEROTATE", JSSVGFEColorMatrixElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_HUEROTATE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA", JSSVGFEColorMatrixElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA)
};

JSValue JSSVGFEColorMatrixElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEColorMatrixElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEColorMatrixElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEColorMatrixElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEColorMatrixElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementConstructorFunctions, countof(JSSVGFEColorMatrixElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGFEColorMatrixElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_UNKNOWN", JSSVGFEColorMatrixElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_SATURATE", JSSVGFEColorMatrixElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_SATURATE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_MATRIX", JSSVGFEColorMatrixElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_MATRIX),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_HUEROTATE", JSSVGFEColorMatrixElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_HUEROTATE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA", JSSVGFEColorMatrixElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEColorMatrixElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEColorMatrixElementPrototypeFunction::callAsFunction, JSSVGFEColorMatrixElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEColorMatrixElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEColorMatrixElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEColorMatrixElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEColorMatrixElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEColorMatrixElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementAttributesFunctions, countof(JSSVGFEColorMatrixElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementPrototypeConstantsFunctions, countof(JSSVGFEColorMatrixElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementPrototypeFunctions, countof(JSSVGFEColorMatrixElementPrototypeFunctions));
}

JSValue JSSVGFEColorMatrixElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFEColorMatrixElementClassDefine = 
{
    "SVGFEColorMatrixElement",
    .finalizer = JSSVGFEColorMatrixElement::finalizer,
    .gc_mark = JSSVGFEColorMatrixElement::mark,
};

JSClassID JSSVGFEColorMatrixElement::js_class_id = 0;

void JSSVGFEColorMatrixElement::init(JSContext* ctx)
{
    if (JSSVGFEColorMatrixElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEColorMatrixElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEColorMatrixElement::js_class_id, &JSSVGFEColorMatrixElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEColorMatrixElementConstructor::self(ctx), JSSVGFEColorMatrixElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEColorMatrixElement::js_class_id, JSSVGFEColorMatrixElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEColorMatrixElement::create(JSContext* ctx, SVGFEColorMatrixElement* impl)
{
    JSSVGFEColorMatrixElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEColorMatrixElementPrototype::self(ctx), JSSVGFEColorMatrixElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEColorMatrixElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEColorMatrixElement* impl = (SVGFEColorMatrixElement*)JS_GetOpaque(val, JSSVGFEColorMatrixElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEColorMatrixElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEColorMatrixElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case TypeAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ValuesAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->valuesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEColorMatrixElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEColorMatrixElementConstructor::self(ctx);
}

JSValue JSSVGFEColorMatrixElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEColorMatrixElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEColorMatrixElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
