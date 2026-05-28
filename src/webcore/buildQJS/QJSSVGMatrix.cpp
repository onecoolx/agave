/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>


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

static JSCFunctionListEntry JSSVGMatrixAttributesFunctions[6];
static bool JSSVGMatrixAttributesFunctions_initialized = false;

static void init_JSSVGMatrixAttributesFunctions()
{
    if (JSSVGMatrixAttributesFunctions_initialized) return;
    JSSVGMatrixAttributesFunctions_initialized = true;
    memset(JSSVGMatrixAttributesFunctions, 0, sizeof(JSSVGMatrixAttributesFunctions));
    JSSVGMatrixAttributesFunctions[0].name = "a";
    JSSVGMatrixAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[0].magic = JSSVGMatrix::AAttrNum;
    JSSVGMatrixAttributesFunctions[0].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[0].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
    JSSVGMatrixAttributesFunctions[1].name = "b";
    JSSVGMatrixAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[1].magic = JSSVGMatrix::BAttrNum;
    JSSVGMatrixAttributesFunctions[1].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[1].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
    JSSVGMatrixAttributesFunctions[2].name = "c";
    JSSVGMatrixAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[2].magic = JSSVGMatrix::CAttrNum;
    JSSVGMatrixAttributesFunctions[2].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[2].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
    JSSVGMatrixAttributesFunctions[3].name = "d";
    JSSVGMatrixAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[3].magic = JSSVGMatrix::DAttrNum;
    JSSVGMatrixAttributesFunctions[3].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[3].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
    JSSVGMatrixAttributesFunctions[4].name = "e";
    JSSVGMatrixAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[4].magic = JSSVGMatrix::EAttrNum;
    JSSVGMatrixAttributesFunctions[4].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[4].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
    JSSVGMatrixAttributesFunctions[5].name = "f";
    JSSVGMatrixAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGMatrixAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGMatrixAttributesFunctions[5].magic = JSSVGMatrix::FAttrNum;
    JSSVGMatrixAttributesFunctions[5].u.getset.get.getter_magic = JSSVGMatrix::getValueProperty;
    JSSVGMatrixAttributesFunctions[5].u.getset.set.setter_magic = JSSVGMatrix::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGMatrixPrototypeFunctions[11];
static bool JSSVGMatrixPrototypeFunctions_initialized = false;

static void init_JSSVGMatrixPrototypeFunctions()
{
    if (JSSVGMatrixPrototypeFunctions_initialized) return;
    JSSVGMatrixPrototypeFunctions_initialized = true;
    memset(JSSVGMatrixPrototypeFunctions, 0, sizeof(JSSVGMatrixPrototypeFunctions));
    JSSVGMatrixPrototypeFunctions[0].name = "multiply";
    JSSVGMatrixPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[0].magic = JSSVGMatrix::MultiplyFuncNum;
    JSSVGMatrixPrototypeFunctions[0].u.func.length = 1;
    JSSVGMatrixPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[1].name = "inverse";
    JSSVGMatrixPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[1].magic = JSSVGMatrix::InverseFuncNum;
    JSSVGMatrixPrototypeFunctions[1].u.func.length = 0;
    JSSVGMatrixPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[2].name = "translate";
    JSSVGMatrixPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[2].magic = JSSVGMatrix::TranslateFuncNum;
    JSSVGMatrixPrototypeFunctions[2].u.func.length = 2;
    JSSVGMatrixPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[3].name = "scale";
    JSSVGMatrixPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[3].magic = JSSVGMatrix::ScaleFuncNum;
    JSSVGMatrixPrototypeFunctions[3].u.func.length = 1;
    JSSVGMatrixPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[4].name = "scaleNonUniform";
    JSSVGMatrixPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[4].magic = JSSVGMatrix::ScaleNonUniformFuncNum;
    JSSVGMatrixPrototypeFunctions[4].u.func.length = 2;
    JSSVGMatrixPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[5].name = "rotate";
    JSSVGMatrixPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[5].magic = JSSVGMatrix::RotateFuncNum;
    JSSVGMatrixPrototypeFunctions[5].u.func.length = 1;
    JSSVGMatrixPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[6].name = "rotateFromVector";
    JSSVGMatrixPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[6].magic = JSSVGMatrix::RotateFromVectorFuncNum;
    JSSVGMatrixPrototypeFunctions[6].u.func.length = 2;
    JSSVGMatrixPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[7].name = "flipX";
    JSSVGMatrixPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[7].magic = JSSVGMatrix::FlipXFuncNum;
    JSSVGMatrixPrototypeFunctions[7].u.func.length = 0;
    JSSVGMatrixPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[7].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[8].name = "flipY";
    JSSVGMatrixPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[8].magic = JSSVGMatrix::FlipYFuncNum;
    JSSVGMatrixPrototypeFunctions[8].u.func.length = 0;
    JSSVGMatrixPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[8].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[9].name = "skewX";
    JSSVGMatrixPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[9].magic = JSSVGMatrix::SkewXFuncNum;
    JSSVGMatrixPrototypeFunctions[9].u.func.length = 1;
    JSSVGMatrixPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[9].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
    JSSVGMatrixPrototypeFunctions[10].name = "skewY";
    JSSVGMatrixPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGMatrixPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSSVGMatrixPrototypeFunctions[10].magic = JSSVGMatrix::SkewYFuncNum;
    JSSVGMatrixPrototypeFunctions[10].u.func.length = 1;
    JSSVGMatrixPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGMatrixPrototypeFunctions[10].u.func.cfunc.generic_magic = JSSVGMatrixPrototypeFunction::callAsFunction;
}

JSValue JSSVGMatrixPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGMatrix.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGMatrixPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGMatrix.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGMatrixPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGMatrixAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMatrixAttributesFunctions, countof(JSSVGMatrixAttributesFunctions));
    init_JSSVGMatrixPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGMatrixPrototypeFunctions, countof(JSSVGMatrixPrototypeFunctions));
}

static JSClassDef JSSVGMatrixClassDefine;
static bool JSSVGMatrixClassDefine_initialized = false;

static void init_JSSVGMatrixClassDefine()
{
    if (JSSVGMatrixClassDefine_initialized) return;
    JSSVGMatrixClassDefine_initialized = true;
    memset(&JSSVGMatrixClassDefine, 0, sizeof(JSSVGMatrixClassDefine));
    JSSVGMatrixClassDefine.class_name = "SVGMatrix";
    JSSVGMatrixClassDefine.finalizer = JSSVGMatrix::finalizer;
    JSSVGMatrixClassDefine.gc_mark = JSSVGMatrix::mark;
}

JSClassID JSSVGMatrix::js_class_id = 0;

void JSSVGMatrix::init(JSContext* ctx)
{
    if (JSSVGMatrix::js_class_id == 0) {
        init_JSSVGMatrixClassDefine();
        JS_NewClassID(&JSSVGMatrix::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGMatrix::js_class_id, &JSSVGMatrixClassDefine);
        JS_SetClassProto(ctx, JSSVGMatrix::js_class_id, JSSVGMatrixPrototype::self(ctx));
    }
}

JSValue JSSVGMatrix::create(JSContext* ctx, JSSVGPODTypeWrapper<AffineTransform>* impl, SVGElement* context)
{
    JSSVGMatrix::init(ctx);
    JSValue _proto = JSSVGMatrixPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGMatrix::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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

            return JS_NewInt32(ctx, imp.a());
        }
        case BAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewInt32(ctx, imp.b());
        }
        case CAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewInt32(ctx, imp.c());
        }
        case DAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewInt32(ctx, imp.d());
        }
        case EAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewInt32(ctx, imp.e());
        }
        case FAttrNum: {
            AffineTransform imp(*impl());

            return JS_NewInt32(ctx, imp.f());
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
    SVGMatrix* imp = (SVGMatrix*)JS_GetOpaque(this_val, JSSVGMatrix::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    JSSVGPODTypeWrapper<AffineTransform>* wrapper = castedThisObj->impl();
    AffineTransform imp(*wrapper);

    switch (token) {
        case JSSVGMatrix::MultiplyFuncNum: {
            return JSSVGMatrix::multiply(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::InverseFuncNum: {
            return JSSVGMatrix::inverse(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::TranslateFuncNum: {
            return JSSVGMatrix::translate(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::ScaleFuncNum: {
            return JSSVGMatrix::scale(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::ScaleNonUniformFuncNum: {
            return JSSVGMatrix::scaleNonUniform(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::RotateFuncNum: {
            return JSSVGMatrix::rotate(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::RotateFromVectorFuncNum: {
            return JSSVGMatrix::rotateFromVector(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::FlipXFuncNum: {
            return JSSVGMatrix::flipX(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::FlipYFuncNum: {
            return JSSVGMatrix::flipY(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::SkewXFuncNum: {
            return JSSVGMatrix::skewX(ctx, this_val, argc, argv, imp);
        }
        case JSSVGMatrix::SkewYFuncNum: {
            return JSSVGMatrix::skewY(ctx, this_val, argc, argv, imp);
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
