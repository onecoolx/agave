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
#include "QJSSVGAnimatedPoints.h"

#include "QJSSVGPointList.h"
#include "SVGAnimatedPoints.h"
#include "SVGPointList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGAnimatedPointsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("points", JSSVGAnimatedPoints::getValueProperty, NULL, JSSVGAnimatedPoints::PointsAttrNum),
    JS_CGETSET_MAGIC_DEF("animatedPoints", JSSVGAnimatedPoints::getValueProperty, NULL, JSSVGAnimatedPoints::AnimatedPointsAttrNum)
};

JSValue JSSVGAnimatedPointsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedPoints.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGAnimatedPointsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGAnimatedPoints.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGAnimatedPointsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGAnimatedPointsAttributesFunctions, countof(JSSVGAnimatedPointsAttributesFunctions));
}

static JSClassDef JSSVGAnimatedPointsClassDefine = 
{
    "SVGAnimatedPoints",
    .finalizer = JSSVGAnimatedPoints::finalizer,
    .gc_mark = JSSVGAnimatedPoints::mark,
};

JSClassID JSSVGAnimatedPoints::js_class_id = 0;

void JSSVGAnimatedPoints::init(JSContext* ctx)
{
    if (JSSVGAnimatedPoints::js_class_id == 0) {
        JS_NewClassID(&JSSVGAnimatedPoints::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGAnimatedPoints::js_class_id, &JSSVGAnimatedPointsClassDefine);
        JS_SetClassProto(ctx, JSSVGAnimatedPoints::js_class_id, JSSVGAnimatedPointsPrototype::self(ctx));
    }
}

JSValue JSSVGAnimatedPoints::create(JSContext* ctx, SVGAnimatedPoints* impl, SVGElement* context)
{
    JSSVGAnimatedPoints::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGAnimatedPointsPrototype::self(ctx), JSSVGAnimatedPoints::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGAnimatedPoints::finalizer(JSRuntime* rt, JSValue val)
{
    SVGAnimatedPoints* impl = (SVGAnimatedPoints*)JS_GetOpaque(val, JSSVGAnimatedPoints::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGAnimatedPoints::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGAnimatedPoints::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PointsAttrNum: {
            SVGAnimatedPoints* imp = (SVGAnimatedPoints*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedPoints::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->points()), context());
        }
        case AnimatedPointsAttrNum: {
            SVGAnimatedPoints* imp = (SVGAnimatedPoints*)JS_GetOpaque2(ctx, this_val, JSSVGAnimatedPoints::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->animatedPoints()), context());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGAnimatedPoints* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGAnimatedPoints, JSSVGAnimatedPoints>(ctx, obj, context);
}

SVGAnimatedPoints* toSVGAnimatedPoints(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGAnimatedPoints* impl = (SVGAnimatedPoints*)JS_GetOpaque(val, JSSVGAnimatedPoints::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
