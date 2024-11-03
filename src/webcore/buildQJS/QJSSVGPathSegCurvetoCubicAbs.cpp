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
#include "QJSSVGPathSegCurvetoCubicAbs.h"

#include "SVGPathSegCurvetoCubic.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegCurvetoCubicAbsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("x1", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::X1AttrNum),
    JS_CGETSET_MAGIC_DEF("x2", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::X2AttrNum),
    JS_CGETSET_MAGIC_DEF("y1", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::Y1AttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::YAttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y2", JSSVGPathSegCurvetoCubicAbs::getValueProperty, JSSVGPathSegCurvetoCubicAbs::putValueProperty, JSSVGPathSegCurvetoCubicAbs::Y2AttrNum)
};

JSValue JSSVGPathSegCurvetoCubicAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicAbs.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegCurvetoCubicAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegCurvetoCubicAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegCurvetoCubicAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegCurvetoCubicAbsAttributesFunctions, countof(JSSVGPathSegCurvetoCubicAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegCurvetoCubicAbsClassDefine = 
{
    "SVGPathSegCurvetoCubicAbs",
    .finalizer = JSSVGPathSegCurvetoCubicAbs::finalizer,
    .gc_mark = JSSVGPathSegCurvetoCubicAbs::mark,
};

JSClassID JSSVGPathSegCurvetoCubicAbs::js_class_id = 0;

void JSSVGPathSegCurvetoCubicAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegCurvetoCubicAbs::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegCurvetoCubicAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegCurvetoCubicAbs::js_class_id, &JSSVGPathSegCurvetoCubicAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegCurvetoCubicAbs::js_class_id, JSSVGPathSegCurvetoCubicAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegCurvetoCubicAbs::create(JSContext* ctx, SVGPathSegCurvetoCubicAbs* impl, SVGElement* context)
{
    JSSVGPathSegCurvetoCubicAbs::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegCurvetoCubicAbsPrototype::self(ctx), JSSVGPathSegCurvetoCubicAbs::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegCurvetoCubicAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegCurvetoCubicAbs* impl = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque(val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegCurvetoCubicAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegCurvetoCubicAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x1());
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y1());
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x2());
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y2());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegCurvetoCubicAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case X1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX1(valueToFloat(ctx, value));
            break;
        }
        case Y1AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setY1(valueToFloat(ctx, value));
            break;
        }
        case X2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
            imp->setX2(valueToFloat(ctx, value));
            break;
        }
        case Y2AttrNum: {
            SVGPathSegCurvetoCubicAbs* imp = (SVGPathSegCurvetoCubicAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegCurvetoCubicAbs::js_class_id);
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
