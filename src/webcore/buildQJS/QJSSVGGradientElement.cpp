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
#include "QJSSVGGradientElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "SVGGradientElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGGradientElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("href", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("externalResourcesRequired", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::ExternalResourcesRequiredAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::StyleAttrNum),
    JS_CGETSET_MAGIC_DEF("spreadMethod", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::SpreadMethodAttrNum),
    JS_CGETSET_MAGIC_DEF("gradientUnits", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::GradientUnitsAttrNum),
    JS_CGETSET_MAGIC_DEF("gradientTransform", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::GradientTransformAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGGradientElement::getValueProperty, NULL, JSSVGGradientElement::ConstructorAttrNum)
};

class JSSVGGradientElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGGradientElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGGradientElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_UNKNOWN", JSSVGGradientElementConstructor::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_REPEAT", JSSVGGradientElementConstructor::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_REPEAT),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_PAD", JSSVGGradientElementConstructor::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_PAD),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_REFLECT", JSSVGGradientElementConstructor::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_REFLECT)
};

JSValue JSSVGGradientElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGGradientElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGGradientElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGGradientElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGGradientElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementConstructorFunctions, countof(JSSVGGradientElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGGradientElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_UNKNOWN", JSSVGGradientElementPrototype::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_REPEAT", JSSVGGradientElementPrototype::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_REPEAT),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_PAD", JSSVGGradientElementPrototype::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_PAD),
    JS_CGETSET_MAGIC_DEF("SVG_SPREADMETHOD_REFLECT", JSSVGGradientElementPrototype::getValueProperty, NULL, SVGGradientElement::SVG_SPREADMETHOD_REFLECT)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGGradientElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGGradientElementPrototypeFunction::callAsFunction, JSSVGGradientElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGGradientElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGGradientElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGGradientElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGGradientElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGGradientElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementAttributesFunctions, countof(JSSVGGradientElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementPrototypeConstantsFunctions, countof(JSSVGGradientElementPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementPrototypeFunctions, countof(JSSVGGradientElementPrototypeFunctions));
}

JSValue JSSVGGradientElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGGradientElementClassDefine = 
{
    "SVGGradientElement",
    .finalizer = JSSVGGradientElement::finalizer,
    .gc_mark = JSSVGGradientElement::mark,
};

JSClassID JSSVGGradientElement::js_class_id = 0;

void JSSVGGradientElement::init(JSContext* ctx)
{
    if (JSSVGGradientElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGGradientElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGGradientElement::js_class_id, &JSSVGGradientElementClassDefine);
        JS_SetConstructor(ctx, JSSVGGradientElementConstructor::self(ctx), JSSVGGradientElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGGradientElement::js_class_id, JSSVGGradientElementPrototype::self(ctx));
    }
}

JSValue JSSVGGradientElement::create(JSContext* ctx, SVGGradientElement* impl)
{
    JSSVGGradientElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGGradientElementPrototype::self(ctx), JSSVGGradientElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGGradientElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGGradientElement* impl = (SVGGradientElement*)JS_GetOpaque(val, JSSVGGradientElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGGradientElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGGradientElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case GradientUnitsAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->gradientUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case GradientTransformAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->gradientTransformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpreadMethodAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->spreadMethodAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGGradientElement::getConstructor(JSContext *ctx)
{
    return JSSVGGradientElementConstructor::self(ctx);
}

JSValue JSSVGGradientElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGGradientElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGGradientElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
