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
#include "QJSSVGFEPointLightElement.h"

#include "QJSSVGAnimatedNumber.h"
#include "SVGFEPointLightElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGFEPointLightElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("z", JSSVGFEPointLightElement::getValueProperty, NULL, JSSVGFEPointLightElement::ZAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGFEPointLightElement::getValueProperty, NULL, JSSVGFEPointLightElement::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGFEPointLightElement::getValueProperty, NULL, JSSVGFEPointLightElement::XAttrNum)
};

JSValue JSSVGFEPointLightElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEPointLightElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEPointLightElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEPointLightElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGFEPointLightElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEPointLightElementAttributesFunctions, countof(JSSVGFEPointLightElementAttributesFunctions));
}

static JSClassDef JSSVGFEPointLightElementClassDefine = 
{
    "SVGFEPointLightElement",
    .finalizer = JSSVGFEPointLightElement::finalizer,
    .gc_mark = JSSVGFEPointLightElement::mark,
};

JSClassID JSSVGFEPointLightElement::js_class_id = 0;

void JSSVGFEPointLightElement::init(JSContext* ctx)
{
    if (JSSVGFEPointLightElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGFEPointLightElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEPointLightElement::js_class_id, &JSSVGFEPointLightElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEPointLightElement::js_class_id, JSSVGFEPointLightElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEPointLightElement::create(JSContext* ctx, SVGFEPointLightElement* impl)
{
    JSSVGFEPointLightElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGFEPointLightElementPrototype::self(ctx), JSSVGFEPointLightElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEPointLightElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEPointLightElement* impl = (SVGFEPointLightElement*)JS_GetOpaque(val, JSSVGFEPointLightElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEPointLightElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEPointLightElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGFEPointLightElement* imp = (SVGFEPointLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEPointLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEPointLightElement* imp = (SVGFEPointLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEPointLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ZAttrNum: {
            SVGFEPointLightElement* imp = (SVGFEPointLightElement*)JS_GetOpaque2(ctx, this_val, JSSVGFEPointLightElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->zAnimated();
            return toJS(ctx, obj.get(), imp);
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
