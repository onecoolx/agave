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
#include "QJSSVGFETileElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFETileElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFETileElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("className", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::ClassNameAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("in1", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::In1AttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("result", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::ResultAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSSVGFETileElement::getValueProperty, NULL, JSSVGFETileElement::StyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGFETileElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getPresentationAttribute", 1, JSSVGFETileElementPrototypeFunction::callAsFunction, JSSVGFETileElement::GetPresentationAttributeFuncNum)
};

JSValue JSSVGFETileElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFETileElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFETileElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFETileElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFETileElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETileElementAttributesFunctions, countof(JSSVGFETileElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFETileElementPrototypeFunctions, countof(JSSVGFETileElementPrototypeFunctions));
}

static JSClassDef JSSVGFETileElementClassDefine = 
{
    "SVGFETileElement",
    .finalizer = JSSVGFETileElement::finalizer,
    .gc_mark = JSSVGFETileElement::mark,
};

JSClassID JSSVGFETileElement::js_class_id = 0;

void JSSVGFETileElement::init(JSContext* ctx)
{
    if (JSSVGFETileElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFETileElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFETileElement::js_class_id, &JSSVGFETileElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFETileElement::js_class_id, JSSVGFETileElementPrototype::self(ctx));
    }
}

JSValue JSSVGFETileElement::create(JSContext* ctx, SVGFETileElement* impl)
{
    JSSVGFETileElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFETileElementPrototype::self(ctx), JSSVGFETileElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFETileElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFETileElement* impl = (SVGFETileElement*)JS_GetOpaque(val, JSSVGFETileElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFETileElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFETileElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFETileElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFETileElement* imp = (SVGFETileElement*)JS_GetOpaque2(ctx, this_val, JSSVGFETileElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFETileElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
