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
#include "QJSSVGFEDisplacementMapElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEDisplacementMapElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEDisplacementMapElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("height", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("xChannelSelector", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::XChannelSelectorAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("in2", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::In2AttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("yChannelSelector", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::YChannelSelectorAttrNum),
    JS_CGETSET_MAGIC_DEF("scale", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::ScaleAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFEDisplacementMapElement::getValueProperty, NULL, JSSVGFEDisplacementMapElement::ResultAttrNum)
};

class JSSVGFEDisplacementMapElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFEDisplacementMapElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGFEDisplacementMapElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_R", JSSVGFEDisplacementMapElementConstructor::getValueProperty, NULL, WebCore::SVG_CHANNEL_R),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_UNKNOWN", JSSVGFEDisplacementMapElementConstructor::getValueProperty, NULL, WebCore::SVG_CHANNEL_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_G", JSSVGFEDisplacementMapElementConstructor::getValueProperty, NULL, WebCore::SVG_CHANNEL_G),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_B", JSSVGFEDisplacementMapElementConstructor::getValueProperty, NULL, WebCore::SVG_CHANNEL_B),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_A", JSSVGFEDisplacementMapElementConstructor::getValueProperty, NULL, WebCore::SVG_CHANNEL_A)
};

JSValue JSSVGFEDisplacementMapElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEDisplacementMapElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEDisplacementMapElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEDisplacementMapElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEDisplacementMapElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementConstructorFunctions, countof(JSSVGFEDisplacementMapElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_R", JSSVGFEDisplacementMapElementPrototype::getValueProperty, NULL, WebCore::SVG_CHANNEL_R),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_UNKNOWN", JSSVGFEDisplacementMapElementPrototype::getValueProperty, NULL, WebCore::SVG_CHANNEL_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_G", JSSVGFEDisplacementMapElementPrototype::getValueProperty, NULL, WebCore::SVG_CHANNEL_G),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_B", JSSVGFEDisplacementMapElementPrototype::getValueProperty, NULL, WebCore::SVG_CHANNEL_B),
    JS_CGETSET_MAGIC_DEF("SVG_CHANNEL_A", JSSVGFEDisplacementMapElementPrototype::getValueProperty, NULL, WebCore::SVG_CHANNEL_A)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFEDisplacementMapElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFEDisplacementMapElementPrototypeFunction::callAsFunction, JSSVGFEDisplacementMapElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFEDisplacementMapElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEDisplacementMapElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEDisplacementMapElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEDisplacementMapElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEDisplacementMapElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementAttributesFunctions, countof(JSSVGFEDisplacementMapElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementPrototypeConstantsFunctions, countof(JSSVGFEDisplacementMapElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementPrototypeFunctions, countof(JSSVGFEDisplacementMapElementPrototypeFunctions));
}

JSValue JSSVGFEDisplacementMapElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFEDisplacementMapElementClassDefine = 
{
    "SVGFEDisplacementMapElement",
    .finalizer = JSSVGFEDisplacementMapElement::finalizer,
    .gc_mark = JSSVGFEDisplacementMapElement::mark,
};

JSClassID JSSVGFEDisplacementMapElement::js_class_id = 0;

void JSSVGFEDisplacementMapElement::init(JSContext* ctx)
{
    if (JSSVGFEDisplacementMapElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEDisplacementMapElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEDisplacementMapElement::js_class_id, &JSSVGFEDisplacementMapElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEDisplacementMapElementConstructor::self(ctx), JSSVGFEDisplacementMapElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEDisplacementMapElement::js_class_id, JSSVGFEDisplacementMapElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEDisplacementMapElement::create(JSContext* ctx, SVGFEDisplacementMapElement* impl)
{
    JSSVGFEDisplacementMapElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEDisplacementMapElementPrototype::self(ctx), JSSVGFEDisplacementMapElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEDisplacementMapElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEDisplacementMapElement* impl = (SVGFEDisplacementMapElement*)JS_GetOpaque(val, JSSVGFEDisplacementMapElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEDisplacementMapElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEDisplacementMapElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case ScaleAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->scaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XChannelSelectorAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->xChannelSelectorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YChannelSelectorAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->yChannelSelectorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEDisplacementMapElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEDisplacementMapElementConstructor::self(ctx);
}

JSValue JSSVGFEDisplacementMapElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDisplacementMapElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEDisplacementMapElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
