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
#include "QJSSVGAnimatedRect.h"

#include "FloatRect.h"
#include "QJSSVGRect.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedRectAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("baseVal", JSSVGAnimatedRect::getValueProperty, NULL, JSSVGAnimatedRect::BaseValAttrNum),
    JS_CGETSET_MAGIC_DEF("animVal", JSSVGAnimatedRect::getValueProperty, NULL, JSSVGAnimatedRect::AnimValAttrNum)
};

JSValue JSSVGAnimatedRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedRect.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedRectAttributesFunctions, countof(JSSVGAnimatedRectAttributesFunctions));
}

static JSClassDef JSSVGAnimatedRectClassDefine = 
{
    "SVGAnimatedRect",
    .finalizer = JSSVGAnimatedRect::finalizer,
    .gc_mark = JSSVGAnimatedRect::mark,
};

JSClassID JSSVGAnimatedRect::js_class_id = 0;

void JSSVGAnimatedRect::init(JSContext* ctx)
{
    if (JSSVGAnimatedRect::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedRect::js_class_id, &JSSVGAnimatedRectClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedRect::js_class_id, JSSVGAnimatedRectPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedRect::create(JSContext* ctx, SVGAnimatedRect* impl, SVGElement* context)
{
    JSSVGAnimatedRect::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedRectPrototype::self(ctx), JSSVGAnimatedRect::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedRect::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedRect* impl = (SVGAnimatedRect*)JS_GetOpaque(val, JSSVGAnimatedRect::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedRect::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedRect::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case BaseValAttrNum: {
            SVGAnimatedRect* imp = (SVGAnimatedRect*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedRect::js_class_id);
            return toJS(ctx, JSSVGPODTypeWrapperCache<FloatRect, SVGAnimatedRect>::lookupOrCreateWrapper(imp, &SVGAnimatedRect::baseVal, &SVGAnimatedRect::setBaseVal), context());
        }
        case AnimValAttrNum: {
            SVGAnimatedRect* imp = (SVGAnimatedRect*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedRect::js_class_id);
            return toJS(ctx, JSSVGPODTypeWrapperCache<FloatRect, SVGAnimatedRect>::lookupOrCreateWrapper(imp, &SVGAnimatedRect::animVal, &SVGAnimatedRect::setAnimVal), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedRect* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedRect, JSSVGAnimatedRect>(ctx, obj, context);
}

SVGAnimatedRect* toSVGAnimatedRect(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedRect* impl = (SVGAnimatedRect*)JS_GetOpaque(val, JSSVGAnimatedRect::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
