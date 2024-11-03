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
#include "QJSSVGMatrix.h"


using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGMatrixAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("b", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::BAttrNum),
    JS_CGETSET_MAGIC_DEF("c", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::CAttrNum),
    JS_CGETSET_MAGIC_DEF("f", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::FAttrNum),
    JS_CGETSET_MAGIC_DEF("a", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::AAttrNum),
    JS_CGETSET_MAGIC_DEF("d", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::DAttrNum),
    JS_CGETSET_MAGIC_DEF("e", JSSVGMatrix::getValueProperty, JSSVGMatrix::putValueProperty, JSSVGMatrix::EAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGMatrixPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("skewX", 1, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::SkewXFuncNum),
    JS_CFUNC_MAGIC_DEF("rotateFromVector", 2, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::RotateFromVectorFuncNum),
    JS_CFUNC_MAGIC_DEF("flipX", 0, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::FlipXFuncNum),
    JS_CFUNC_MAGIC_DEF("multiply", 1, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::MultiplyFuncNum),
    JS_CFUNC_MAGIC_DEF("inverse", 0, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::InverseFuncNum),
    JS_CFUNC_MAGIC_DEF("rotate", 1, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::RotateFuncNum),
    JS_CFUNC_MAGIC_DEF("scaleNonUniform", 2, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::ScaleNonUniformFuncNum),
    JS_CFUNC_MAGIC_DEF("scale", 1, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::ScaleFuncNum),
    JS_CFUNC_MAGIC_DEF("translate", 2, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::TranslateFuncNum),
    JS_CFUNC_MAGIC_DEF("flipY", 0, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::FlipYFuncNum),
    JS_CFUNC_MAGIC_DEF("skewY", 1, JSSVGMatrixPrototypeFunction::callAsFunction, JSSVGMatrix::SkewYFuncNum)
};

JSValue JSSVGMatrixPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMatrix.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGMatrixPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMatrix.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGMatrixPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMatrixAttributesFunctions, countof(JSSVGMatrixAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMatrixPrototypeFunctions, countof(JSSVGMatrixPrototypeFunctions));
}

static JSClassDef JSSVGMatrixClassDefine = 
{
    "SVGMatrix",
    .finalizer = JSSVGMatrix::finalizer,
    .gc_mark = JSSVGMatrix::mark,
};

JSClassID JSSVGMatrix::js_class_id = 0;

void JSSVGMatrix::init(JSContext* ctx)
{
    if (JSSVGMatrix::js_class_id == 0) {
        JS_NewClassID(&JSSVGMatrix::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMatrix::js_class_id, &JSSVGMatrixClassDefine);
        JS_SetClassProto(ctx, JSSVGMatrix::js_class_id, JSSVGMatrixPrototype::self(ctx));
    }
}

JSValue JSSVGMatrix::create(JSContext* ctx, JSSVGPODTypeWrapper<AffineTransform>* impl, SVGElement* context)
{
    JSSVGMatrix::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGMatrixPrototype::self(ctx), JSSVGMatrix::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGMatrix::finalizer(JSRuntime* rt, JSValue val)
{
    SVGMatrix* impl = (SVGMatrix*)JS_GetOpaque(val, JSSVGMatrix::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGMatrix::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGMatrix::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.a());
        }
        case BAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.b());
        }
        case CAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.c());
        }
        case DAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.d());
        }
        case EAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.e());
        }
        case FAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.f());
        }
    }
    return JS_NULL;
}

JSValue JSSVGMatrix::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AAttrNum: {
            AffineTransform imp(*impl());

            imp.setA(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case BAttrNum: {
            AffineTransform imp(*impl());

            imp.setB(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case CAttrNum: {
            AffineTransform imp(*impl());

            imp.setC(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case DAttrNum: {
            AffineTransform imp(*impl());

            imp.setD(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case EAttrNum: {
            AffineTransform imp(*impl());

            imp.setE(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
        case FAttrNum: {
            AffineTransform imp(*impl());

            imp.setF(valueToDouble(ctx, value));
            m_impl->commitChange(ctx, imp);
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}

JSValue JSSVGMatrixPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGMatrix* imp = (SVGMatrix*)JS_GetOpaque2(ctx, this_val, JSSVGMatrix::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    JSSVGPODTypeWrapper<AffineTransform>* wrapper = castedThisObj->impl();
    AffineTransform imp(*wrapper);

    switch (token) {
        case JSSVGMatrix::MultiplyFuncNum: {
            return JSSVGMatrix::multiply(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::InverseFuncNum: {
            return JSSVGMatrix::inverse(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::TranslateFuncNum: {
            return JSSVGMatrix::translate(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::ScaleFuncNum: {
            return JSSVGMatrix::scale(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::ScaleNonUniformFuncNum: {
            return JSSVGMatrix::scaleNonUniform(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::RotateFuncNum: {
            return JSSVGMatrix::rotate(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::RotateFromVectorFuncNum: {
            return JSSVGMatrix::rotateFromVector(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::FlipXFuncNum: {
            return JSSVGMatrix::flipX(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::FlipYFuncNum: {
            return JSSVGMatrix::flipY(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::SkewXFuncNum: {
            return JSSVGMatrix::skewX(ctx, argc, argv, imp);
        }
        case JSSVGMatrix::SkewYFuncNum: {
            return JSSVGMatrix::skewY(ctx, argc, argv, imp);
        }
    }
    (void)imp;
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<AffineTransform>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<AffineTransform>, JSSVGMatrix>(ctx, obj, context);
}

AffineTransform toSVGMatrix(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGMatrix* impl = (SVGMatrix*)JS_GetOpaque(val, JSSVGMatrix::js_class_id);
        return (AffineTransform) *impl;
    } else {
        return AffineTransform();
}

}

#endif // ENABLE(SVG)
