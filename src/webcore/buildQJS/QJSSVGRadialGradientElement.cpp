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
#include "QJSSVGRadialGradientElement.h"

#include "QJSSVGAnimatedLength.h"
#include "SVGRadialGradientElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGRadialGradientElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("fx", JSSVGRadialGradientElement::getValueProperty, NULL, JSSVGRadialGradientElement::FxAttrNum),
    JS_CGETSET_MAGIC_DEF("cy", JSSVGRadialGradientElement::getValueProperty, NULL, JSSVGRadialGradientElement::CyAttrNum),
    JS_CGETSET_MAGIC_DEF("cx", JSSVGRadialGradientElement::getValueProperty, NULL, JSSVGRadialGradientElement::CxAttrNum),
    JS_CGETSET_MAGIC_DEF("r", JSSVGRadialGradientElement::getValueProperty, NULL, JSSVGRadialGradientElement::RAttrNum),
    JS_CGETSET_MAGIC_DEF("fy", JSSVGRadialGradientElement::getValueProperty, NULL, JSSVGRadialGradientElement::FyAttrNum)
};

JSValue JSSVGRadialGradientElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRadialGradientElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGGradientElementPrototype::self(ctx));
        JSSVGRadialGradientElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRadialGradientElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGRadialGradientElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRadialGradientElementAttributesFunctions, countof(JSSVGRadialGradientElementAttributesFunctions));
}

static JSClassDef JSSVGRadialGradientElementClassDefine = 
{
    "SVGRadialGradientElement",
    .finalizer = JSSVGRadialGradientElement::finalizer,
    .gc_mark = JSSVGRadialGradientElement::mark,
};

JSClassID JSSVGRadialGradientElement::js_class_id = 0;

void JSSVGRadialGradientElement::init(JSContext* ctx)
{
    if (JSSVGRadialGradientElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGRadialGradientElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRadialGradientElement::js_class_id, &JSSVGRadialGradientElementClassDefine);
        JS_SetClassProto(ctx, JSSVGRadialGradientElement::js_class_id, JSSVGRadialGradientElementPrototype::self(ctx));
    }
}

JSValue JSSVGRadialGradientElement::create(JSContext* ctx, SVGRadialGradientElement* impl)
{
    JSSVGRadialGradientElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGRadialGradientElementPrototype::self(ctx), JSSVGRadialGradientElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGRadialGradientElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGRadialGradientElement* impl = (SVGRadialGradientElement*)JS_GetOpaque(val, JSSVGRadialGradientElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGRadialGradientElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGGradientElement::mark(rt, val, mark_func);
}

JSValue JSSVGRadialGradientElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CxAttrNum: {
            SVGRadialGradientElement* imp = (SVGRadialGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGRadialGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case CyAttrNum: {
            SVGRadialGradientElement* imp = (SVGRadialGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGRadialGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->cyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case RAttrNum: {
            SVGRadialGradientElement* imp = (SVGRadialGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGRadialGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->rAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FxAttrNum: {
            SVGRadialGradientElement* imp = (SVGRadialGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGRadialGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->fxAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case FyAttrNum: {
            SVGRadialGradientElement* imp = (SVGRadialGradientElement*)JS_GetOpaque2(ctx, this_val, JSSVGRadialGradientElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->fyAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
