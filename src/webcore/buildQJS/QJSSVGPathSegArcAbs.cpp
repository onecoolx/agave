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
#include "QJSSVGPathSegArcAbs.h"

#include "SVGPathSegArc.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegArcAbsAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("r2", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::R2AttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::YAttrNum),
    JS_CGETSET_MAGIC_DEF("r1", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::R1AttrNum),
    JS_CGETSET_MAGIC_DEF("angle", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::AngleAttrNum),
    JS_CGETSET_MAGIC_DEF("largeArcFlag", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::LargeArcFlagAttrNum),
    JS_CGETSET_MAGIC_DEF("sweepFlag", JSSVGPathSegArcAbs::getValueProperty, JSSVGPathSegArcAbs::putValueProperty, JSSVGPathSegArcAbs::SweepFlagAttrNum)
};

JSValue JSSVGPathSegArcAbsPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcAbs.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegArcAbsPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcAbs.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegArcAbsPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegArcAbsAttributesFunctions, countof(JSSVGPathSegArcAbsAttributesFunctions));
}

static JSClassDef JSSVGPathSegArcAbsClassDefine = 
{
    "SVGPathSegArcAbs",
    .finalizer = JSSVGPathSegArcAbs::finalizer,
    .gc_mark = JSSVGPathSegArcAbs::mark,
};

JSClassID JSSVGPathSegArcAbs::js_class_id = 0;

void JSSVGPathSegArcAbs::init(JSContext* ctx)
{
    if (JSSVGPathSegArcAbs::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegArcAbs::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegArcAbs::js_class_id, &JSSVGPathSegArcAbsClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegArcAbs::js_class_id, JSSVGPathSegArcAbsPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegArcAbs::create(JSContext* ctx, SVGPathSegArcAbs* impl, SVGElement* context)
{
    JSSVGPathSegArcAbs::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegArcAbsPrototype::self(ctx), JSSVGPathSegArcAbs::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegArcAbs::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegArcAbs* impl = (SVGPathSegArcAbs*)JS_GetOpaque(val, JSSVGPathSegArcAbs::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegArcAbs::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegArcAbs::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case R1AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->r1());
        }
        case R2AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->r2());
        }
        case AngleAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBigUint64(ctx, imp->angle());
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBool(ctx, imp->largeArcFlag() ? 1 : 0);
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            return JS_NewBool(ctx, imp->sweepFlag() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegArcAbs::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case R1AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setR1(valueToFloat(ctx, value));
            break;
        }
        case R2AttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setR2(valueToFloat(ctx, value));
            break;
        }
        case AngleAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setAngle(valueToFloat(ctx, value));
            break;
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setLargeArcFlag(valueToBoolean(ctx, value));
            break;
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcAbs* imp = (SVGPathSegArcAbs*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcAbs::js_class_id);
            imp->setSweepFlag(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
    if (context())
        context()->notifyAttributeChange();
}


}

#endif // ENABLE(SVG)
