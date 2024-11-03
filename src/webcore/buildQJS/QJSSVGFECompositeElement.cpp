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
#include "QJSSVGFECompositeElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFECompositeElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFECompositeElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("_operator", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::_operatorAttrNum),
    JS_CGETSET_MAGIC_DEF("k1", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::K1AttrNum),
    JS_CGETSET_MAGIC_DEF("in2", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::In2AttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("k3", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::K3AttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("k2", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::K2AttrNum),
    JS_CGETSET_MAGIC_DEF("k4", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::K4AttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFECompositeElement::getValueProperty, NULL, JSSVGFECompositeElement::StyleAttrNum)
};

class JSSVGFECompositeElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFECompositeElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGFECompositeElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_OUT", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_OUT),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_ATOP", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_ATOP),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_ARITHMETIC", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_ARITHMETIC),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_OVER", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_OVER),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_UNKNOWN", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_IN", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_IN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_XOR", JSSVGFECompositeElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_XOR)
};

JSValue JSSVGFECompositeElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFECompositeElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFECompositeElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFECompositeElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFECompositeElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementConstructorFunctions, countof(JSSVGFECompositeElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGFECompositeElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_OUT", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_OUT),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_ATOP", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_ATOP),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_ARITHMETIC", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_ARITHMETIC),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_OVER", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_OVER),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_UNKNOWN", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_IN", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_IN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPOSITE_OPERATOR_XOR", JSSVGFECompositeElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPOSITE_OPERATOR_XOR)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFECompositeElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFECompositeElementPrototypeFunction::callAsFunction, JSSVGFECompositeElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFECompositeElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFECompositeElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFECompositeElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFECompositeElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFECompositeElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementAttributesFunctions, countof(JSSVGFECompositeElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementPrototypeConstantsFunctions, countof(JSSVGFECompositeElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFECompositeElementPrototypeFunctions, countof(JSSVGFECompositeElementPrototypeFunctions));
}

JSValue JSSVGFECompositeElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFECompositeElementClassDefine = 
{
    "SVGFECompositeElement",
    .finalizer = JSSVGFECompositeElement::finalizer,
    .gc_mark = JSSVGFECompositeElement::mark,
};

JSClassID JSSVGFECompositeElement::js_class_id = 0;

void JSSVGFECompositeElement::init(JSContext* ctx)
{
    if (JSSVGFECompositeElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFECompositeElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFECompositeElement::js_class_id, &JSSVGFECompositeElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFECompositeElementConstructor::self(ctx), JSSVGFECompositeElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFECompositeElement::js_class_id, JSSVGFECompositeElementPrototype::self(ctx));
    }
}

JSValue JSSVGFECompositeElement::create(JSContext* ctx, SVGFECompositeElement* impl)
{
    JSSVGFECompositeElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFECompositeElementPrototype::self(ctx), JSSVGFECompositeElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFECompositeElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFECompositeElement* impl = (SVGFECompositeElement*)JS_GetOpaque(val, JSSVGFECompositeElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFECompositeElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFECompositeElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case _operatorAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->_operatorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case K1AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K2AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K3AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k3Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case K4AttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->k4Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFECompositeElement::getConstructor(JSContext *ctx)
{
    return JSSVGFECompositeElementConstructor::self(ctx);
}

JSValue JSSVGFECompositeElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFECompositeElement* imp = (SVGFECompositeElement*)JS_GetOpaque2(ctx, this_val, JSSVGFECompositeElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFECompositeElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
