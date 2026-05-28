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
#include "QJSSVGTransform.h"

#include "QJSSVGMatrix.h"
#include "SVGTransform.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGTransformAttributesFunctions[4];
static bool JSSVGTransformAttributesFunctions_initialized = false;

static void init_JSSVGTransformAttributesFunctions()
{
    if (JSSVGTransformAttributesFunctions_initialized) return;
    JSSVGTransformAttributesFunctions_initialized = true;
    memset(JSSVGTransformAttributesFunctions, 0, sizeof(JSSVGTransformAttributesFunctions));
    JSSVGTransformAttributesFunctions[0].name = "type";
    JSSVGTransformAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTransformAttributesFunctions[0].magic = JSSVGTransform::TypeAttrNum;
    JSSVGTransformAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTransform::getValueProperty;
    JSSVGTransformAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGTransformAttributesFunctions[1].name = "matrix";
    JSSVGTransformAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTransformAttributesFunctions[1].magic = JSSVGTransform::MatrixAttrNum;
    JSSVGTransformAttributesFunctions[1].u.getset.get.getter_magic = JSSVGTransform::getValueProperty;
    JSSVGTransformAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGTransformAttributesFunctions[2].name = "angle";
    JSSVGTransformAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTransformAttributesFunctions[2].magic = JSSVGTransform::AngleAttrNum;
    JSSVGTransformAttributesFunctions[2].u.getset.get.getter_magic = JSSVGTransform::getValueProperty;
    JSSVGTransformAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGTransformAttributesFunctions[3].name = "constructor";
    JSSVGTransformAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTransformAttributesFunctions[3].magic = JSSVGTransform::ConstructorAttrNum;
    JSSVGTransformAttributesFunctions[3].u.getset.get.getter_magic = JSSVGTransform::getValueProperty;
    JSSVGTransformAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSSVGTransformConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGTransformConstructorFunctions[7];
static bool JSSVGTransformConstructorFunctions_initialized = false;

static void init_JSSVGTransformConstructorFunctions()
{
    if (JSSVGTransformConstructorFunctions_initialized) return;
    JSSVGTransformConstructorFunctions_initialized = true;
    memset(JSSVGTransformConstructorFunctions, 0, sizeof(JSSVGTransformConstructorFunctions));
    JSSVGTransformConstructorFunctions[0].name = "SVG_TRANSFORM_UNKNOWN";
    JSSVGTransformConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[0].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_UNKNOWN;
    JSSVGTransformConstructorFunctions[1].name = "SVG_TRANSFORM_MATRIX";
    JSSVGTransformConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[1].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_MATRIX;
    JSSVGTransformConstructorFunctions[2].name = "SVG_TRANSFORM_TRANSLATE";
    JSSVGTransformConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[2].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_TRANSLATE;
    JSSVGTransformConstructorFunctions[3].name = "SVG_TRANSFORM_SCALE";
    JSSVGTransformConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[3].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SCALE;
    JSSVGTransformConstructorFunctions[4].name = "SVG_TRANSFORM_ROTATE";
    JSSVGTransformConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[4].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_ROTATE;
    JSSVGTransformConstructorFunctions[5].name = "SVG_TRANSFORM_SKEWX";
    JSSVGTransformConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[5].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SKEWX;
    JSSVGTransformConstructorFunctions[6].name = "SVG_TRANSFORM_SKEWY";
    JSSVGTransformConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformConstructorFunctions[6].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SKEWY;
}

JSValue JSSVGTransformConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTransform.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTransform.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTransformConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTransformConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformConstructorFunctions, countof(JSSVGTransformConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGTransformPrototypeConstantsFunctions[7];
static bool JSSVGTransformPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGTransformPrototypeConstantsFunctions()
{
    if (JSSVGTransformPrototypeConstantsFunctions_initialized) return;
    JSSVGTransformPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGTransformPrototypeConstantsFunctions, 0, sizeof(JSSVGTransformPrototypeConstantsFunctions));
    JSSVGTransformPrototypeConstantsFunctions[0].name = "SVG_TRANSFORM_UNKNOWN";
    JSSVGTransformPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_UNKNOWN;
    JSSVGTransformPrototypeConstantsFunctions[1].name = "SVG_TRANSFORM_MATRIX";
    JSSVGTransformPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_MATRIX;
    JSSVGTransformPrototypeConstantsFunctions[2].name = "SVG_TRANSFORM_TRANSLATE";
    JSSVGTransformPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_TRANSLATE;
    JSSVGTransformPrototypeConstantsFunctions[3].name = "SVG_TRANSFORM_SCALE";
    JSSVGTransformPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SCALE;
    JSSVGTransformPrototypeConstantsFunctions[4].name = "SVG_TRANSFORM_ROTATE";
    JSSVGTransformPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_ROTATE;
    JSSVGTransformPrototypeConstantsFunctions[5].name = "SVG_TRANSFORM_SKEWX";
    JSSVGTransformPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SKEWX;
    JSSVGTransformPrototypeConstantsFunctions[6].name = "SVG_TRANSFORM_SKEWY";
    JSSVGTransformPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGTransformPrototypeConstantsFunctions[6].u.i32 = (int32_t)SVGTransform::SVG_TRANSFORM_SKEWY;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGTransformPrototypeFunctions[6];
static bool JSSVGTransformPrototypeFunctions_initialized = false;

static void init_JSSVGTransformPrototypeFunctions()
{
    if (JSSVGTransformPrototypeFunctions_initialized) return;
    JSSVGTransformPrototypeFunctions_initialized = true;
    memset(JSSVGTransformPrototypeFunctions, 0, sizeof(JSSVGTransformPrototypeFunctions));
    JSSVGTransformPrototypeFunctions[0].name = "setMatrix";
    JSSVGTransformPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[0].magic = JSSVGTransform::SetMatrixFuncNum;
    JSSVGTransformPrototypeFunctions[0].u.func.length = 1;
    JSSVGTransformPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
    JSSVGTransformPrototypeFunctions[1].name = "setTranslate";
    JSSVGTransformPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[1].magic = JSSVGTransform::SetTranslateFuncNum;
    JSSVGTransformPrototypeFunctions[1].u.func.length = 2;
    JSSVGTransformPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
    JSSVGTransformPrototypeFunctions[2].name = "setScale";
    JSSVGTransformPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[2].magic = JSSVGTransform::SetScaleFuncNum;
    JSSVGTransformPrototypeFunctions[2].u.func.length = 2;
    JSSVGTransformPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
    JSSVGTransformPrototypeFunctions[3].name = "setRotate";
    JSSVGTransformPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[3].magic = JSSVGTransform::SetRotateFuncNum;
    JSSVGTransformPrototypeFunctions[3].u.func.length = 3;
    JSSVGTransformPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
    JSSVGTransformPrototypeFunctions[4].name = "setSkewX";
    JSSVGTransformPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[4].magic = JSSVGTransform::SetSkewXFuncNum;
    JSSVGTransformPrototypeFunctions[4].u.func.length = 1;
    JSSVGTransformPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
    JSSVGTransformPrototypeFunctions[5].name = "setSkewY";
    JSSVGTransformPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGTransformPrototypeFunctions[5].magic = JSSVGTransform::SetSkewYFuncNum;
    JSSVGTransformPrototypeFunctions[5].u.func.length = 1;
    JSSVGTransformPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGTransformPrototypeFunction::callAsFunction;
}

JSValue JSSVGTransformPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTransform.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTransform.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTransformPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTransformAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformAttributesFunctions, countof(JSSVGTransformAttributesFunctions));
    init_JSSVGTransformPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformPrototypeConstantsFunctions, countof(JSSVGTransformPrototypeConstantsFunctions));
    init_JSSVGTransformPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformPrototypeFunctions, countof(JSSVGTransformPrototypeFunctions));
}

static JSClassDef JSSVGTransformClassDefine;
static bool JSSVGTransformClassDefine_initialized = false;

static void init_JSSVGTransformClassDefine()
{
    if (JSSVGTransformClassDefine_initialized) return;
    JSSVGTransformClassDefine_initialized = true;
    memset(&JSSVGTransformClassDefine, 0, sizeof(JSSVGTransformClassDefine));
    JSSVGTransformClassDefine.class_name = "SVGTransform";
    JSSVGTransformClassDefine.finalizer = JSSVGTransform::finalizer;
    JSSVGTransformClassDefine.gc_mark = JSSVGTransform::mark;
}

JSClassID JSSVGTransform::js_class_id = 0;

void JSSVGTransform::init(JSContext* ctx)
{
    if (JSSVGTransform::js_class_id == 0) {
        init_JSSVGTransformClassDefine();
        JS_NewClassID(&JSSVGTransform::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTransform::js_class_id, &JSSVGTransformClassDefine);
        JS_SetConstructor(ctx, JSSVGTransformConstructor::self(ctx), JSSVGTransformPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTransform::js_class_id, JSSVGTransformPrototype::self(ctx));
    }
}

JSValue JSSVGTransform::create(JSContext* ctx, JSSVGPODTypeWrapper<SVGTransform>* impl, SVGElement* context)
{
    JSSVGTransform::init(ctx);
    JSValue _proto = JSSVGTransformPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTransform::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTransform::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTransform* impl = (SVGTransform*)JS_GetOpaque(val, JSSVGTransform::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTransform::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGTransform::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            SVGTransform imp(*impl());

            return JS_NewInt32(ctx, imp.type());
        }
        case MatrixAttrNum: {
            SVGTransform imp(*impl());

            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp.matrix()), context());
        }
        case AngleAttrNum: {
            SVGTransform imp(*impl());

            return JS_NewInt32(ctx, imp.angle());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGTransform::getConstructor(JSContext *ctx)
{
    return JSSVGTransformConstructor::self(ctx);
}

JSValue JSSVGTransformPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTransform* imp = (SVGTransform*)JS_GetOpaque(this_val, JSSVGTransform::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    JSSVGPODTypeWrapper<SVGTransform>* wrapper = castedThisObj->impl();
    SVGTransform imp(*wrapper);

    switch (token) {
        case JSSVGTransform::SetMatrixFuncNum: {
            AffineTransform matrix = toSVGMatrix(argv[0]);
            imp.setMatrix(matrix);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGTransform::SetTranslateFuncNum: {
            float tx = valueToFloat(ctx, argv[0]);
            float ty = valueToFloat(ctx, argv[1]);
            imp.setTranslate(tx, ty);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGTransform::SetScaleFuncNum: {
            float sx = valueToFloat(ctx, argv[0]);
            float sy = valueToFloat(ctx, argv[1]);
            imp.setScale(sx, sy);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGTransform::SetRotateFuncNum: {
            float angle = valueToFloat(ctx, argv[0]);
            float cx = valueToFloat(ctx, argv[1]);
            float cy = valueToFloat(ctx, argv[2]);
            imp.setRotate(angle, cx, cy);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGTransform::SetSkewXFuncNum: {
            float angle = valueToFloat(ctx, argv[0]);
            imp.setSkewX(angle);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
        case JSSVGTransform::SetSkewYFuncNum: {
            float angle = valueToFloat(ctx, argv[0]);
            imp.setSkewY(angle);
            wrapper->commitChange(exec, imp);

            if (castedThisObj->context())
                castedThisObj->context()->notifyAttributeChange();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, JSSVGPODTypeWrapper<SVGTransform>* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<JSSVGPODTypeWrapper<SVGTransform>, JSSVGTransform>(ctx, obj, context);
}

SVGTransform toSVGTransform(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGTransform* impl = (SVGTransform*)JS_GetOpaque(val, JSSVGTransform::js_class_id);
        return (SVGTransform) *impl;
    } else {
        return SVGTransform();
}

}

#endif // ENABLE(SVG)
