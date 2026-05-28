/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>


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

static JSCFunctionListEntry JSSVGTextElementAttributesFunctions[3];
static bool JSSVGTextElementAttributesFunctions_initialized = false;

static void init_JSSVGTextElementAttributesFunctions()
{
    if (JSSVGTextElementAttributesFunctions_initialized) return;
    JSSVGTextElementAttributesFunctions_initialized = true;
    memset(JSSVGTextElementAttributesFunctions, 0, sizeof(JSSVGTextElementAttributesFunctions));
    JSSVGTextElementAttributesFunctions[0].name = "nearestViewportElement";
    JSSVGTextElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextElementAttributesFunctions[0].magic = JSSVGTextElement::NearestViewportElementAttrNum;
    JSSVGTextElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTextElement::getValueProperty;
    JSSVGTextElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGTextElementAttributesFunctions[1].name = "transform";
    JSSVGTextElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextElementAttributesFunctions[1].magic = JSSVGTextElement::TransformAttrNum;
    JSSVGTextElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTextElement::getValueProperty;
    JSSVGTextElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTextElementAttributesFunctions[2].name = "farthestViewportElement";
    JSSVGTextElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTextElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTextElementAttributesFunctions[2].magic = JSSVGTextElement::FarthestViewportElementAttrNum;
    JSSVGTextElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTextElement::getValueProperty;
    JSSVGTextElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGTextElementPrototypeFunctions[4];
static bool JSSVGTextElementPrototypeFunctions_initialized = false;

static void init_JSSVGTextElementPrototypeFunctions()
{
    if (JSSVGTextElementPrototypeFunctions_initialized) return;
    JSSVGTextElementPrototypeFunctions_initialized = true;
    memset(JSSVGTextElementPrototypeFunctions, 0, sizeof(JSSVGTextElementPrototypeFunctions));
    JSSVGTextElementPrototypeFunctions[0].name = "getTransformToElement";
    JSSVGTextElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGTextElementPrototypeFunctions[0].magic = JSSVGTextElement::GetTransformToElementFuncNum;
    JSSVGTextElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGTextElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGTextElementPrototypeFunction::callAsFunction;
    JSSVGTextElementPrototypeFunctions[1].name = "getScreenCTM";
    JSSVGTextElementPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextElementPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGTextElementPrototypeFunctions[1].magic = JSSVGTextElement::GetScreenCTMFuncNum;
    JSSVGTextElementPrototypeFunctions[1].u.func.length = 0;
    JSSVGTextElementPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextElementPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGTextElementPrototypeFunction::callAsFunction;
    JSSVGTextElementPrototypeFunctions[2].name = "getCTM";
    JSSVGTextElementPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextElementPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGTextElementPrototypeFunctions[2].magic = JSSVGTextElement::GetCTMFuncNum;
    JSSVGTextElementPrototypeFunctions[2].u.func.length = 0;
    JSSVGTextElementPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextElementPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGTextElementPrototypeFunction::callAsFunction;
    JSSVGTextElementPrototypeFunctions[3].name = "getBBox";
    JSSVGTextElementPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTextElementPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGTextElementPrototypeFunctions[3].magic = JSSVGTextElement::GetBBoxFuncNum;
    JSSVGTextElementPrototypeFunctions[3].u.func.length = 0;
    JSSVGTextElementPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTextElementPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGTextElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGTextElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTextElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGTextPositioningElementPrototype::self(ctx));
        JSSVGTextElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTextElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTextElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTextElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextElementAttributesFunctions, countof(JSSVGTextElementAttributesFunctions));
    init_JSSVGTextElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTextElementPrototypeFunctions, countof(JSSVGTextElementPrototypeFunctions));
}

static JSClassDef JSSVGTextElementClassDefine;
static bool JSSVGTextElementClassDefine_initialized = false;

static void init_JSSVGTextElementClassDefine()
{
    if (JSSVGTextElementClassDefine_initialized) return;
    JSSVGTextElementClassDefine_initialized = true;
    memset(&JSSVGTextElementClassDefine, 0, sizeof(JSSVGTextElementClassDefine));
    JSSVGTextElementClassDefine.class_name = "SVGTextElement";
    JSSVGTextElementClassDefine.finalizer = JSSVGTextElement::finalizer;
    JSSVGTextElementClassDefine.gc_mark = JSSVGTextElement::mark;
}

JSClassID JSSVGTextElement::js_class_id = 0;

void JSSVGTextElement::init(JSContext* ctx)
{
    if (JSSVGTextElement::js_class_id == 0) {
        init_JSSVGTextElementClassDefine();
        JS_NewClassID(&JSSVGTextElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTextElement::js_class_id, &JSSVGTextElementClassDefine);
        JS_SetClassProto(ctx, JSSVGTextElement::js_class_id, JSSVGTextElementPrototype::self(ctx));
    }
}

JSValue JSSVGTextElement::create(JSContext* ctx, SVGTextElement* impl)
{
    JSSVGTextElement::init(ctx);
    JSValue _proto = JSSVGTextElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTextElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque(this_val, JSSVGTextElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->transformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case NearestViewportElementAttrNum: {
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque(this_val, JSSVGTextElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->nearestViewportElement()));
        }
        case FarthestViewportElementAttrNum: {
            SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque(this_val, JSSVGTextElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->farthestViewportElement()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGTextElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTextElement* imp = (SVGTextElement*)JS_GetOpaque(this_val, JSSVGTextElement::js_class_id);
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
