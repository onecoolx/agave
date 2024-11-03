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
#include "QJSSVGPathSegArcRel.h"

#include "SVGPathSegArc.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegArcRelAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("r2", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::R2AttrNum),
    JS_CGETSET_MAGIC_DEF("x", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::XAttrNum),
    JS_CGETSET_MAGIC_DEF("y", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::YAttrNum),
    JS_CGETSET_MAGIC_DEF("r1", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::R1AttrNum),
    JS_CGETSET_MAGIC_DEF("angle", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::AngleAttrNum),
    JS_CGETSET_MAGIC_DEF("largeArcFlag", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::LargeArcFlagAttrNum),
    JS_CGETSET_MAGIC_DEF("sweepFlag", JSSVGPathSegArcRel::getValueProperty, JSSVGPathSegArcRel::putValueProperty, JSSVGPathSegArcRel::SweepFlagAttrNum)
};

JSValue JSSVGPathSegArcRelPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcRel.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGPathSegPrototype::self(ctx));
        JSSVGPathSegArcRelPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegArcRel.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegArcRelPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegArcRelAttributesFunctions, countof(JSSVGPathSegArcRelAttributesFunctions));
}

static JSClassDef JSSVGPathSegArcRelClassDefine = 
{
    "SVGPathSegArcRel",
    .finalizer = JSSVGPathSegArcRel::finalizer,
    .gc_mark = JSSVGPathSegArcRel::mark,
};

JSClassID JSSVGPathSegArcRel::js_class_id = 0;

void JSSVGPathSegArcRel::init(JSContext* ctx)
{
    if (JSSVGPathSegArcRel::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegArcRel::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegArcRel::js_class_id, &JSSVGPathSegArcRelClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegArcRel::js_class_id, JSSVGPathSegArcRelPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegArcRel::create(JSContext* ctx, SVGPathSegArcRel* impl, SVGElement* context)
{
    JSSVGPathSegArcRel::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegArcRelPrototype::self(ctx), JSSVGPathSegArcRel::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegArcRel::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegArcRel* impl = (SVGPathSegArcRel*)JS_GetOpaque(val, JSSVGPathSegArcRel::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegArcRel::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGPathSeg::mark(rt, val, mark_func);
}

JSValue JSSVGPathSegArcRel::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->x());
        }
        case YAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->y());
        }
        case R1AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->r1());
        }
        case R2AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->r2());
        }
        case AngleAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBigUint64(ctx, imp->angle());
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBool(ctx, imp->largeArcFlag() ? 1 : 0);
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            return JS_NewBool(ctx, imp->sweepFlag() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegArcRel::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case XAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setX(valueToFloat(ctx, value));
            break;
        }
        case YAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setY(valueToFloat(ctx, value));
            break;
        }
        case R1AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setR1(valueToFloat(ctx, value));
            break;
        }
        case R2AttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setR2(valueToFloat(ctx, value));
            break;
        }
        case AngleAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setAngle(valueToFloat(ctx, value));
            break;
        }
        case LargeArcFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
            imp->setLargeArcFlag(valueToBoolean(ctx, value));
            break;
        }
        case SweepFlagAttrNum: {
            SVGPathSegArcRel* imp = (SVGPathSegArcRel*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegArcRel::js_class_id);
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
