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
#include "QJSSVGAnimatedPreserveAspectRatio.h"

#include "QJSSVGPreserveAspectRatio.h"
#include "SVGPreserveAspectRatio.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedPreserveAspectRatioAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedPreserveAspectRatio::getValueProperty, NULL, JSSVGAnimatedPreserveAspectRatio::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedPreserveAspectRatio::getValueProperty, NULL, JSSVGAnimatedPreserveAspectRatio::AnimValAttrNum)
};

JSValue JSSVGAnimatedPreserveAspectRatioPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedPreserveAspectRatio.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedPreserveAspectRatioPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedPreserveAspectRatio.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedPreserveAspectRatioPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedPreserveAspectRatioAttributesFunctions, countof(JSSVGAnimatedPreserveAspectRatioAttributesFunctions));
}

static JSClassDef JSSVGAnimatedPreserveAspectRatioClassDefine = 
{
    "SVGAnimatedPreserveAspectRatio",
    .finalizer = JSSVGAnimatedPreserveAspectRatio::finalizer,
    .gc_mark = JSSVGAnimatedPreserveAspectRatio::mark,
};

JSClassID JSSVGAnimatedPreserveAspectRatio::js_class_id = 0;

void JSSVGAnimatedPreserveAspectRatio::init(JSContext* ctx)
{
    if (JSSVGAnimatedPreserveAspectRatio::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedPreserveAspectRatio::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedPreserveAspectRatio::js_class_id, &JSSVGAnimatedPreserveAspectRatioClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedPreserveAspectRatio::js_class_id, JSSVGAnimatedPreserveAspectRatioPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedPreserveAspectRatio::create(JSContext* ctx, SVGAnimatedPreserveAspectRatio* impl, SVGElement* context)
{
    JSSVGAnimatedPreserveAspectRatio::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedPreserveAspectRatioPrototype::self(ctx), JSSVGAnimatedPreserveAspectRatio::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedPreserveAspectRatio::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedPreserveAspectRatio* impl = (SVGAnimatedPreserveAspectRatio*)JS_GetOpaque(val, JSSVGAnimatedPreserveAspectRatio::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedPreserveAspectRatio::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedPreserveAspectRatio::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedPreserveAspectRatio* imp = (SVGAnimatedPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedPreserveAspectRatio::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->baseVal()), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedPreserveAspectRatio* imp = (SVGAnimatedPreserveAspectRatio*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedPreserveAspectRatio::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animVal()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedPreserveAspectRatio* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedPreserveAspectRatio, JSSVGAnimatedPreserveAspectRatio>(ctx, obj, context);
}

SVGAnimatedPreserveAspectRatio* toSVGAnimatedPreserveAspectRatio(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedPreserveAspectRatio* impl = (SVGAnimatedPreserveAspectRatio*)JS_GetOpaque(val, JSSVGAnimatedPreserveAspectRatio::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
