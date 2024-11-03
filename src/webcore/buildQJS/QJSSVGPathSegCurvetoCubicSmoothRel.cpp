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
#include "QJSSVGPathSegCurvetoCubicSmoothRel.h"

#include "SVGPathSegCurvetoCubicSmooth.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegCurvetoCubicSmoothRelAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegCurvetoCubicSmoothRel::getValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::putValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegCurvetoCubicSmoothRel::getValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::putValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y2", JSSVGPathSegCurvetoCubicSmoothRel::getValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::putValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::Y2AttrNum),
    JS_CGETSET_MAGIC_DEF("x2", JSSVGPathSegCurvetoCubicSmoothRel::getValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::putValueProperty, JSSVGPathSegCurvetoCubicSmoothRel::X2AttrNum)
};

JSValue JSSVGPathSegCurvetoCubicSmoothRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothRel.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicSmoothRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicSmoothRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicSmoothRelAttributesFunctions, countof(JSSVGPathSegCurvetoCubicSmoothRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicSmoothRelClassDefine = 
{
    "SVGPathSegCurvetoCubicSmoothRel",
    .finalizer = JSSVGPathSegCurvetoCubicSmoothRel::finalizer,
    .gc_mark = JSSVGPathSegCurvetoCubicSmoothRel::mark,
};

JSClassID JSSVGPathSegCurvetoCubicSmoothRel::js_class_id = 0;

void JSSVGPathSegCurvetoCubicSmoothRel::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicSmoothRel::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicSmoothRel::js_class_id, &JSSVGPathSegCurvetoCubicSmoothRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id, JSSVGPathSegCurvetoCubicSmoothRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicSmoothRel::create(JSContext* ctx, SVGPathSegCurvetoCubicSmoothRel* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicSmoothRel::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegCurvetoCubicSmoothRelPrototype::self(ctx), JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicSmoothRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicSmoothRel* impl = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicSmoothRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicSmoothRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicSmoothRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicSmoothRel* imp = (SVGPathSegCurvetoCubicSmoothRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicSmoothRel::js_class_id);
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
