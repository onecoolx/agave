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
#include "QJSSVGPathSegCurvetoCubicSmoothAbs.h"

#include "SVGPathSegCurvetoCubicSmooth.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y2", JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::Y2AttrNum),
    JS_CGETSET_MAGIC_DEF("x2", JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty, JSSVGPathSegCurvetoCubicSmoothAbs::X2AttrNum)
};

JSValue JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothAbs.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicSmoothAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions, countof(JSSVGPathSegCurvetoCubicSmoothAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicSmoothAbsClassDefine = 
{
    "SVGPathSegCurvetoCubicSmoothAbs",
    .finalizer = JSSVGPathSegCurvetoCubicSmoothAbs::finalizer,
    .gc_mark = JSSVGPathSegCurvetoCubicSmoothAbs::mark,
};

JSClassID JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id = 0;

void JSSVGPathSegCurvetoCubicSmoothAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id, &JSSVGPathSegCurvetoCubicSmoothAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id, JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::create(JSContext* ctx, SVGPathSegCurvetoCubicSmoothAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicSmoothAbs::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegCurvetoCubicSmoothAbsPrototype::self(ctx), JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicSmoothAbs* impl = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicSmoothAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicSmoothAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothAbs* imp = (SVGPathSegCurvetoCubicSmoothAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothAbs::js_class_id);
            imp->setY2(valueToFloat(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
