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
#include "QJSSVGTextElement.h"

#include "QJSSVGAnimatedTransformList.h"
#include "QJSSVGElement.h"
#include "QJSSVGMatrix.h"
#include "QJSSVGRect.h"
#include "SVGElement.h"
#include "SVGTextElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTextElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("nearestViewportElement", JSSVGTextElement::getValueProperty, NULL, JSSVGTextElement::NearestViewportElementAttrNum),
    JS_CGETSET_MAGIC_DEF("transform", JSSVGTextElement::getValueProperty, NULL, JSSVGTextElement::TransformAttrNum),
    JS_CGETSET_MAGIC_DEF("farthestViewportElement", JSSVGTextElement::getValueProperty, NULL, JSSVGTextElement::FarthestViewportElementAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGTextElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getTransformToElement", 1, JSSVGTextElementPrototypeFunction::callAsFunction, JSSVGTextElement::GetTransformToElementFuncNum),
    JS_CFUNC_MAGIC_DEF("getScreenCTM", 0, JSSVGTextElementPrototypeFunction::callAsFunction, JSSVGTextElement::GetScreenCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getCTM", 0, JSSVGTextElementPrototypeFunction::callAsFunction, JSSVGTextElement::GetCTMFuncNum),
    JS_CFUNC_MAGIC_DEF("getBBox", 0, JSSVGTextElementPrototypeFunction::callAsFunction, JSSVGTextElement::GetBBoxFuncNum)
};

JSValue JSSVGTextElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextPositioningElementPrototype::self(ctx));
        JSSVGTextElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTextElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextElementAttributesFunctions, countof(JSSVGTextElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextElementPrototypeFunctions, countof(JSSVGTextElementPrototypeFunctions));
}

static JSClassDef JSSVGTextElementClassDefine = 
{
    "SVGTextElement",
    .finalizer = JSSVGTextElement::finalizer,
    .gc_mark = JSSVGTextElement::mark,
};

JSClassID JSSVGTextElement::js_class_id = 0;

void JSSVGTextElement::init(JSContext* ctx)
{
    if (JSSVGTextElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGTextElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextElement::js_class_id, &JSSVGTextElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTextElement::js_class_id, JSSVGTextElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextElement::create(JSContext* ctx, SVGTextElement* impl)
{
    JSSVGTextElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTextElementPrototype::self(ctx), JSSVGTextElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTextElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTextElement* impl = (SVGTextElement*)JS_GetOpaque(val, JSSVGTextElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTextElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGTextPositioningElement::mark(rt, val, mark_func);
}

JSValue JSSVGTextElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TransformAttrNum: {
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGTextElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque2(ctx, this_val, JSSVGTextElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTextElement::GetBBoxFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatRect>(imp->getBBox()), imp);
            return result;
        }
        case JSSVGTextElement::GetCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getCTM()), imp);
            return result;
        }
        case JSSVGTextElement::GetScreenCTMFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getScreenCTM()), imp);
            return result;
        }
        case JSSVGTextElement::GetTransformToElementFuncNum: {
            ExceptionCode ec = 0;
            SVGElement* element = toSVGElement(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp->getTransformToElement(element, ec)), imp);
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
