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
#include "QJSSVGPoint.h"

#include "QJSSVGMatrix.h"
#include "QJSSVGPoint.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPointAttributesFunctions[2];
static bool JSSVGPointAttributesFunctions_initialized = false;

static void init_JSSVGPointAttributesFunctions()
{
    if (JSSVGPointAttributesFunctions_initialized) return;
    JSSVGPointAttributesFunctions_initialized = true;
    memset(JSSVGPointAttributesFunctions, 0, sizeof(JSSVGPointAttributesFunctions));
    JSSVGPointAttributesFunctions[0].name = "x";
    JSSVGPointAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPointAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPointAttributesFunctions[0].magic = JSSVGPoint::XAttrNum;
    JSSVGPointAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPoint::getValueProperty;
    JSSVGPointAttributesFunctions[0].u.getset.set.setter_magic = JSSVGPoint::putValueProperty;
    JSSVGPointAttributesFunctions[1].name = "y";
    JSSVGPointAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPointAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPointAttributesFunctions[1].magic = JSSVGPoint::YAttrNum;
    JSSVGPointAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPoint::getValueProperty;
    JSSVGPointAttributesFunctions[1].u.getset.set.setter_magic = JSSVGPoint::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPointPrototypeFunctions[1];
static bool JSSVGPointPrototypeFunctions_initialized = false;

static void init_JSSVGPointPrototypeFunctions()
{
    if (JSSVGPointPrototypeFunctions_initialized) return;
    JSSVGPointPrototypeFunctions_initialized = true;
    memset(JSSVGPointPrototypeFunctions, 0, sizeof(JSSVGPointPrototypeFunctions));
    JSSVGPointPrototypeFunctions[0].name = "matrixTransform";
    JSSVGPointPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPointPrototypeFunctions[0].magic = JSSVGPoint::MatrixTransformFuncNum;
    JSSVGPointPrototypeFunctions[0].u.func.length = 1;
    JSSVGPointPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPointPrototypeFunction::callAsFunction;
}

JSValue JSSVGPointPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPoint.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPointPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPoint.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPointPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPointAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointAttributesFunctions, countof(JSSVGPointAttributesFunctions));
    init_JSSVGPointPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointPrototypeFunctions, countof(JSSVGPointPrototypeFunctions));
}

static JSClassDef JSSVGPointClassDefine;
static bool JSSVGPointClassDefine_initialized = false;

static void init_JSSVGPointClassDefine()
{
    if (JSSVGPointClassDefine_initialized) return;
    JSSVGPointClassDefine_initialized = true;
    memset(&JSSVGPointClassDefine, 0, sizeof(JSSVGPointClassDefine));
    JSSVGPointClassDefine.class_name = "SVGPoint";
    JSSVGPointClassDefine.finalizer = JSSVGPoint::finalizer;
    JSSVGPointClassDefine.gc_mark = JSSVGPoint::mark;
}

JSClassID JSSVGPoint::js_class_id = 0;

void JSSVGPoint::init(JSContext* ctx)
{
    if (JSSVGPoint::js_class_id == 0) {
        init_JSSVGPointClassDefine();
        JS_NewClassID(&JSSVGPoint::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPoint::js_class_id, &JSSVGPointClassDefine);
        JS_SetClassProto(ctx, JSSVGPoint::js_class_id, JSSVGPointPrototype::self(ctx));
    }
}

JSValue JSSVGPoint::create(JSContext* ctx, JSSVGPODTypeWrapper<FloatPoint>* impl, SVGElement* context)
{
    JSSVGPoint::init(ctx);
    JSValue _proto = JSSVGPointPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPoint::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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

            return JS_NewInt32(ctx, imp.x());
        }
        case YAttrNum: {
            FloatPoint imp(*impl());

            return JS_NewInt32(ctx, imp.y());
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
    SVGPoint* imp = (SVGPoint*)JS_GetOpaque(this_val, JSSVGPoint::js_class_id);
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
