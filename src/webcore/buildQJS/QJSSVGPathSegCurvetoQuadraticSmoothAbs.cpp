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
#include "QJSSVGPathSegCurvetoQuadraticSmoothAbs.h"

#include "SVGPathSegCurvetoQuadraticSmooth.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticSmoothAbs::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticSmoothAbs::XAttrNum)
};

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticSmoothAbs.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticSmoothAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions, countof(JSSVGPathSegCurvetoQuadraticSmoothAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine = 
{
    "SVGPathSegCurvetoQuadraticSmoothAbs",
    .finalizer = JSSVGPathSegCurvetoQuadraticSmoothAbs::finalizer,
    .gc_mark = JSSVGPathSegCurvetoQuadraticSmoothAbs::mark,
};

JSClassID JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id = 0;

void JSSVGPathSegCurvetoQuadraticSmoothAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id, &JSSVGPathSegCurvetoQuadraticSmoothAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id, JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::create(JSContext* ctx, SVGPathSegCurvetoQuadraticSmoothAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoQuadraticSmoothAbs::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegCurvetoQuadraticSmoothAbsPrototype::self(ctx), JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoQuadraticSmoothAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoQuadraticSmoothAbs* impl = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoQuadraticSmoothAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoQuadraticSmoothAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticSmoothAbs* imp = (SVGPathSegCurvetoQuadraticSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticSmoothAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
