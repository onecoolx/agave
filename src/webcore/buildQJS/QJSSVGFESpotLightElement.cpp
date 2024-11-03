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
#include "QJSSVGFESpotLightElement.h"

#include "QJSSVGAnimatedNumber.h"
#include "SVGFESpotLightElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFESpotLightElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("pointsAtZ", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::PointsAtZAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::XAttrNum),
    JS_CGETSET_MAGIC_DEF("pointsAtX", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::PointsAtXAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("specularExponent", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::SpecularExponentAttrNum),
    JS_CGETSET_MAGIC_DEF("z", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::ZAttrNum),
    JS_CGETSET_MAGIC_DEF("pointsAtY", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::PointsAtYAttrNum),
    JS_CGETSET_MAGIC_DEF("limitingConeAngle", JSSVGFESpotLightElement::getValueProperty, NULL, JSSVGFESpotLightElement::LimitingConeAngleAttrNum)
};

JSValue JSSVGFESpotLightElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFESpotLightElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFESpotLightElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFESpotLightElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFESpotLightElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFESpotLightElementAttributesFunctions, countof(JSSVGFESpotLightElementAttributesFunctions));
}

static JSClassDef JSSVGFESpotLightElementClassDefine = 
{
    "SVGFESpotLightElement",
    .finalizer = JSSVGFESpotLightElement::finalizer,
    .gc_mark = JSSVGFESpotLightElement::mark,
};

JSClassID JSSVGFESpotLightElement::js_class_id = 0;

void JSSVGFESpotLightElement::init(JSContext* ctx)
{
    if (JSSVGFESpotLightElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFESpotLightElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFESpotLightElement::js_class_id, &JSSVGFESpotLightElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFESpotLightElement::js_class_id, JSSVGFESpotLightElementPrototype::self(ctx));
    }
}

JSValue JSSVGFESpotLightElement::create(JSContext* ctx, SVGFESpotLightElement* impl)
{
    JSSVGFESpotLightElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFESpotLightElementPrototype::self(ctx), JSSVGFESpotLightElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFESpotLightElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFESpotLightElement* impl = (SVGFESpotLightElement*)JS_GetOpaque(val, JSSVGFESpotLightElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFESpotLightElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFESpotLightElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->zAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtXAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtYAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case PointsAtZAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->pointsAtZAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpecularExponentAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->specularExponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case LimitingConeAngleAttrNum: {
            SVGFESpotLightElement* imp = (SVGFESpotLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFESpotLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->limitingConeAngleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
