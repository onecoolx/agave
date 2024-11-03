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
#include "QJSSVGTransform.h"

#include "QJSSVGMatrix.h"
#include "SVGTransform.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTransformAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("angle", JSSVGTransform::getValueProperty, NULL, JSSVGTransform::AngleAttrNum),
    JS_CGETSET_MAGIC_DEF("matrix", JSSVGTransform::getValueProperty, NULL, JSSVGTransform::MatrixAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSSVGTransform::getValueProperty, NULL, JSSVGTransform::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGTransform::getValueProperty, NULL, JSSVGTransform::ConstructorAttrNum)
};

class JSSVGTransformConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGTransformConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGTransformConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SCALE", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SCALE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_UNKNOWN", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_ROTATE", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_ROTATE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SKEWY", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SKEWY),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_MATRIX", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_MATRIX),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_TRANSLATE", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_TRANSLATE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SKEWX", JSSVGTransformConstructor::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SKEWX)
};

JSValue JSSVGTransformConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGTransform.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGTransform.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTransformConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformConstructorFunctions, countof(JSSVGTransformConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGTransformPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SCALE", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SCALE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_UNKNOWN", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_ROTATE", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_ROTATE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SKEWY", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SKEWY),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_MATRIX", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_MATRIX),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_TRANSLATE", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_TRANSLATE),
    JS_CGETSET_MAGIC_DEF("SVG_TRANSFORM_SKEWX", JSSVGTransformPrototype::getValueProperty, NULL, SVGTransform::SVG_TRANSFORM_SKEWX)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGTransformPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("setTranslate", 2, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetTranslateFuncNum),
    JS_CFUNC_MAGIC_DEF("setMatrix", 1, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetMatrixFuncNum),
    JS_CFUNC_MAGIC_DEF("setSkewY", 1, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetSkewYFuncNum),
    JS_CFUNC_MAGIC_DEF("setScale", 2, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetScaleFuncNum),
    JS_CFUNC_MAGIC_DEF("setRotate", 3, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetRotateFuncNum),
    JS_CFUNC_MAGIC_DEF("setSkewX", 1, JSSVGTransformPrototypeFunction::callAsFunction, JSSVGTransform::SetSkewXFuncNum)
};

JSValue JSSVGTransformPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTransform.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTransform.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTransformPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformAttributesFunctions, countof(JSSVGTransformAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformPrototypeConstantsFunctions, countof(JSSVGTransformPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformPrototypeFunctions, countof(JSSVGTransformPrototypeFunctions));
}

JSValue JSSVGTransformPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGTransformClassDefine = 
{
    "SVGTransform",
    .finalizer = JSSVGTransform::finalizer,
    .gc_mark = JSSVGTransform::mark,
};

JSClassID JSSVGTransform::js_class_id = 0;

void JSSVGTransform::init(JSContext* ctx)
{
    if (JSSVGTransform::js_class_id == 0) {
        JS_NewClassID(&JSSVGTransform::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTransform::js_class_id, &JSSVGTransformClassDefine);
        JS_SetConstructor(ctx, JSSVGTransformConstructor::self(ctx), JSSVGTransformPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGTransform::js_class_id, JSSVGTransformPrototype::self(ctx));
    }
}

JSValue JSSVGTransform::create(JSContext* ctx, JSSVGPODTypeWrapper<SVGTransform>* impl, SVGElement* context)
{
    JSSVGTransform::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTransformPrototype::self(ctx), JSSVGTransform::js_class_id);
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

            return JS_NewBigUint64(ctx, imp.type());
        }
        case MatrixAttrNum: {
            SVGTransform imp(*impl());

            return toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<AffineTransform>(imp.matrix()), context());
        }
        case AngleAttrNum: {
            SVGTransform imp(*impl());

            return JS_NewBigUint64(ctx, imp.angle());
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
    SVGTransform* imp = (SVGTransform*)JS_GetOpaque2(ctx, this_val, JSSVGTransform::js_class_id);
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
