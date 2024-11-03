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
#include "QJSSVGPathSegCurvetoQuadraticAbs.h"

#include "SVGPathSegCurvetoQuadratic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegCurvetoQuadraticAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticAbs::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegCurvetoQuadraticAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticAbs::XAttrNum),
    JS_CGETSET_MAGIC_DEF("x1", JSSVGPathSegCurvetoQuadraticAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticAbs::X1AttrNum),
    JS_CGETSET_MAGIC_DEF("y1", JSSVGPathSegCurvetoQuadraticAbs::getValueProperty, JSSVGPathSegCurvetoQuadraticAbs::putValueProperty, JSSVGPathSegCurvetoQuadraticAbs::Y1AttrNum)
};

JSValue JSSVGPathSegCurvetoQuadraticAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticAbs.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoQuadraticAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoQuadraticAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegCurvetoQuadraticAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions, countof(JSSVGPathSegCurvetoQuadraticAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoQuadraticAbsClassDefine = 
{
    "SVGPathSegCurvetoQuadraticAbs",
    .finalizer = JSSVGPathSegCurvetoQuadraticAbs::finalizer,
    .gc_mark = JSSVGPathSegCurvetoQuadraticAbs::mark,
};

JSClassID JSSVGPathSegCurvetoQuadraticAbs::js_class_id = 0;

void JSSVGPathSegCurvetoQuadraticAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoQuadraticAbs::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoQuadraticAbs::js_class_id, &JSSVGPathSegCurvetoQuadraticAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoQuadraticAbs::js_class_id, JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::create(JSContext* ctx, SVGPathSegCurvetoQuadraticAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoQuadraticAbs::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ctx), JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoQuadraticAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoQuadraticAbs* impl = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoQuadraticAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y1());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoQuadraticAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoQuadraticAbs* imp = (SVGPathSegCurvetoQuadraticAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoQuadraticAbs::js_class_id);
            imp->setY1(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
