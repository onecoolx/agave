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
#include "QJSSVGFEBlendElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEBlendElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEBlendElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("mode", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::ModeAttrNum),
    JS_CGETSET_MAGIC_DEF("in2", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::In2AttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGFEBlendElement::getValueProperty, NULL, JSSVGFEBlendElement::ConstructorAttrNum)
};

class JSSVGFEBlendElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFEBlendElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGFEBlendElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_NORMAL", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_NORMAL),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_UNKNOWN", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_MULTIPLY", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_MULTIPLY),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_SCREEN", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_SCREEN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_DARKEN", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_DARKEN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_LIGHTEN", JSSVGFEBlendElementConstructor::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_LIGHTEN)
};

JSValue JSSVGFEBlendElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEBlendElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEBlendElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEBlendElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEBlendElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementConstructorFunctions, countof(JSSVGFEBlendElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGFEBlendElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_NORMAL", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_NORMAL),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_UNKNOWN", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_MULTIPLY", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_MULTIPLY),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_SCREEN", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_SCREEN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_DARKEN", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_DARKEN),
    JS_CGETSET_MAGIC_DEF("SVG_FEBLEND_MODE_LIGHTEN", JSSVGFEBlendElementPrototype::getValueProperty, NULL, WebCore::SVG_FEBLEND_MODE_LIGHTEN)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEBlendElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEBlendElementPrototypeFunction::callAsFunction, JSSVGFEBlendElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEBlendElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEBlendElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEBlendElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEBlendElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEBlendElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementAttributesFunctions, countof(JSSVGFEBlendElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementPrototypeConstantsFunctions, countof(JSSVGFEBlendElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEBlendElementPrototypeFunctions, countof(JSSVGFEBlendElementPrototypeFunctions));
}

JSValue JSSVGFEBlendElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFEBlendElementClassDefine = 
{
    "SVGFEBlendElement",
    .finalizer = JSSVGFEBlendElement::finalizer,
    .gc_mark = JSSVGFEBlendElement::mark,
};

JSClassID JSSVGFEBlendElement::js_class_id = 0;

void JSSVGFEBlendElement::init(JSContext* ctx)
{
    if (JSSVGFEBlendElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEBlendElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEBlendElement::js_class_id, &JSSVGFEBlendElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEBlendElementConstructor::self(ctx), JSSVGFEBlendElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEBlendElement::js_class_id, JSSVGFEBlendElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEBlendElement::create(JSContext* ctx, SVGFEBlendElement* impl)
{
    JSSVGFEBlendElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEBlendElementPrototype::self(ctx), JSSVGFEBlendElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEBlendElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEBlendElement* impl = (SVGFEBlendElement*)JS_GetOpaque(val, JSSVGFEBlendElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEBlendElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEBlendElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case ModeAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->modeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEBlendElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEBlendElementConstructor::self(ctx);
}

JSValue JSSVGFEBlendElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEBlendElement* imp = (SVGFEBlendElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEBlendElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEBlendElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
