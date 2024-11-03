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
#include "QJSSVGPoint.h"

#include "QJSSVGMatrix.h"
#include "QJSSVGPoint.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPointAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPoint::getValueProperty, JSSVGPoint::putValueProperty, JSSVGPoint::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPoint::getValueProperty, JSSVGPoint::putValueProperty, JSSVGPoint::XAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPointPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("matrixTransform", 1, JSSVGPointPrototypeFunction::callAsFunction, JSSVGPoint::MatrixTransformFuncNum)
};

JSValue JSSVGPointPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPoint.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPointPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPoint.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPointPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointAttributesFunctions, countof(JSSVGPointAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointPrototypeFunctions, countof(JSSVGPointPrototypeFunctions));
}

static JSClassDef JSSVGPointClassDefine = 
{
    "SVGPoint",
    .finalizer = JSSVGPoint::finalizer,
    .gc_mark = JSSVGPoint::mark,
};

JSClassID JSSVGPoint::js_class_id = 0;

void JSSVGPoint::init(JSContext* ctx)
{
    if (JSSVGPoint::js_class_id == 0) {
        JS_NewClassID(&JSSVGPoint::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPoint::js_class_id, &JSSVGPointClassDefine);
        JS_SetClassProto(ctx, JSSVGPoint::js_class_id, JSSVGPointPrototype::self(ctx));
    }
}

JSValue JSSVGPoint::create(JSContext* ctx, JSSVGPODTypeWrapper<FloatPoint>* impl, SVGElement* context)
{
    JSSVGPoint::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPointPrototype::self(ctx), JSSVGPoint::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPoint::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPoint* impl = (SVGPoint*)JS_GetOpaque(val, JSSVGPoint::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPoint::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPoint::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            FloatPoint imp(*impl());

            return JS_NewBigUint64(ctx, imp.x());
        }
        case YAttrNum: {
            FloatPoint imp(*impl());

            return JS_NewBigUint64(ctx, imp.y());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPoint::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            FloatPoint imp(*impl());

            imp.setX(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case YAttrNum: {
            FloatPoint imp(*impl());

            imp.setY(valueToFloat(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue JSSVGPointPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPoint* imp = (SVGPoint*)JS_GetOpaque2(ctx, this_val, JSSVGPoint::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    JSSVGPODTypeWrapper<FloatPoint>* wrapper = castedThisObj->impl();
    FloatPoint imp(*wrapper);

    switch (token) {
        case JSSVGPoint::MatrixTransformFuncNum: {
            AffineTransform matrix = toSVGMatrix(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<FloatPoint>(imp.matrixTransform(matrix)), castedThisObj->context());
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<FloatPoint>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<FloatPoint>, JSSVGPoint>(ctx, obj, context);
}

FloatPoint toSVGPoint(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPoint* impl = (SVGPoint*)JS_GetOpaque(val, JSSVGPoint::js_class_id);
        return (FloatPoint) *impl;
    } else {
        return FloatPoint();
}

}

#endif // ENABLE(SVG)
