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
#include "QJSSVGFEDistantLightElement.h"

#include "QJSSVGAnimatedNumber.h"
#include "SVGFEDistantLightElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEDistantLightElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("azimuth", JSSVGFEDistantLightElement::getValueProperty, NULL, JSSVGFEDistantLightElement::AzimuthAttrNum),
    JS_CGETSET_MAGIC_DEF("elevation", JSSVGFEDistantLightElement::getValueProperty, NULL, JSSVGFEDistantLightElement::ElevationAttrNum)
};

JSValue JSSVGFEDistantLightElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEDistantLightElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEDistantLightElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEDistantLightElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEDistantLightElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDistantLightElementAttributesFunctions, countof(JSSVGFEDistantLightElementAttributesFunctions));
}

static JSClassDef JSSVGFEDistantLightElementClassDefine = 
{
    "SVGFEDistantLightElement",
    .finalizer = JSSVGFEDistantLightElement::finalizer,
    .gc_mark = JSSVGFEDistantLightElement::mark,
};

JSClassID JSSVGFEDistantLightElement::js_class_id = 0;

void JSSVGFEDistantLightElement::init(JSContext* ctx)
{
    if (JSSVGFEDistantLightElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEDistantLightElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEDistantLightElement::js_class_id, &JSSVGFEDistantLightElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEDistantLightElement::js_class_id, JSSVGFEDistantLightElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEDistantLightElement::create(JSContext* ctx, SVGFEDistantLightElement* impl)
{
    JSSVGFEDistantLightElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEDistantLightElementPrototype::self(ctx), JSSVGFEDistantLightElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEDistantLightElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEDistantLightElement* impl = (SVGFEDistantLightElement*)JS_GetOpaque(val, JSSVGFEDistantLightElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEDistantLightElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEDistantLightElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AzimuthAttrNum: {
            SVGFEDistantLightElement* imp = (SVGFEDistantLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDistantLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->azimuthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ElevationAttrNum: {
            SVGFEDistantLightElement* imp = (SVGFEDistantLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEDistantLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->elevationAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
