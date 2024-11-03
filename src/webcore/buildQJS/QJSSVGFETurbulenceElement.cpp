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
#include "QJSSVGFETurbulenceElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedInteger.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFETurbulenceElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFETurbulenceElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseFrequencyY", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::BaseFrequencyYAttrNum),
    JS_CGETSET_MAGIC_DEF("baseFrequencyX", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::BaseFrequencyXAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("stitchTiles", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::StitchTilesAttrNum),
    JS_CGETSET_MAGIC_DEF("numOctaves", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::NumOctavesAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("seed", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::SeedAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGFETurbulenceElement::getValueProperty, NULL, JSSVGFETurbulenceElement::ConstructorAttrNum)
};

class JSSVGFETurbulenceElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGFETurbulenceElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGFETurbulenceElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_UNKNOWN", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_FRACTALNOISE", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_FRACTALNOISE),
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_TURBULENCE", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_TURBULENCE),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_UNKNOWN", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_STITCH", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_STITCH),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_NOSTITCH", JSSVGFETurbulenceElementConstructor::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_NOSTITCH)
};

JSValue JSSVGFETurbulenceElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFETurbulenceElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFETurbulenceElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFETurbulenceElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFETurbulenceElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementConstructorFunctions, countof(JSSVGFETurbulenceElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGFETurbulenceElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_UNKNOWN", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_FRACTALNOISE", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_FRACTALNOISE),
    JS_CGETSET_MAGIC_DEF("SVG_TURBULENCE_TYPE_TURBULENCE", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_TURBULENCE_TYPE_TURBULENCE),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_UNKNOWN", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_STITCH", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_STITCH),
    JS_CGETSET_MAGIC_DEF("SVG_STITCHTYPE_NOSTITCH", JSSVGFETurbulenceElementPrototype::getValueProperty, NULL, WebCore::SVG_STITCHTYPE_NOSTITCH)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFETurbulenceElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFETurbulenceElementPrototypeFunction::callAsFunction, JSSVGFETurbulenceElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFETurbulenceElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFETurbulenceElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFETurbulenceElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFETurbulenceElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFETurbulenceElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementAttributesFunctions, countof(JSSVGFETurbulenceElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementPrototypeConstantsFunctions, countof(JSSVGFETurbulenceElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETurbulenceElementPrototypeFunctions, countof(JSSVGFETurbulenceElementPrototypeFunctions));
}

JSValue JSSVGFETurbulenceElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGFETurbulenceElementClassDefine = 
{
    "SVGFETurbulenceElement",
    .finalizer = JSSVGFETurbulenceElement::finalizer,
    .gc_mark = JSSVGFETurbulenceElement::mark,
};

JSClassID JSSVGFETurbulenceElement::js_class_id = 0;

void JSSVGFETurbulenceElement::init(JSContext* ctx)
{
    if (JSSVGFETurbulenceElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFETurbulenceElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFETurbulenceElement::js_class_id, &JSSVGFETurbulenceElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFETurbulenceElementConstructor::self(ctx), JSSVGFETurbulenceElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFETurbulenceElement::js_class_id, JSSVGFETurbulenceElementPrototype::self(ctx));
    }
}

JSValue JSSVGFETurbulenceElement::create(JSContext* ctx, SVGFETurbulenceElement* impl)
{
    JSSVGFETurbulenceElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFETurbulenceElementPrototype::self(ctx), JSSVGFETurbulenceElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFETurbulenceElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFETurbulenceElement* impl = (SVGFETurbulenceElement*)JS_GetOpaque(val, JSSVGFETurbulenceElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFETurbulenceElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFETurbulenceElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseFrequencyXAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->baseFrequencyXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case BaseFrequencyYAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->baseFrequencyYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NumOctavesAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedInteger> obj = imp->numOctavesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SeedAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->seedAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StitchTilesAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->stitchTilesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case TypeAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFETurbulenceElement::getConstructor(JSContext *ctx)
{
    return JSSVGFETurbulenceElementConstructor::self(ctx);
}

JSValue JSSVGFETurbulenceElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFETurbulenceElement* imp = (SVGFETurbulenceElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETurbulenceElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFETurbulenceElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
