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

#include "QJSCanvasRenderingContext2D.h"

#include "CanvasGradient.h"
#include "CanvasPattern.h"
#include "CanvasRenderingContext2D.h"
#include "CanvasStyle.h"
#include "HTMLCanvasElement.h"
#include "QJSCanvasGradient.h"
#include "QJSHTMLCanvasElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCanvasRenderingContext2DAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("lineWidth", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::LineWidthAttrNum),
    JS_CGETSET_MAGIC_DEF("shadowOffsetY", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::ShadowOffsetYAttrNum),
    JS_CGETSET_MAGIC_DEF("strokeStyle", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::StrokeStyleAttrNum),
    JS_CGETSET_MAGIC_DEF("globalAlpha", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::GlobalAlphaAttrNum),
    JS_CGETSET_MAGIC_DEF("lineJoin", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::LineJoinAttrNum),
    JS_CGETSET_MAGIC_DEF("shadowOffsetX", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::ShadowOffsetXAttrNum),
    JS_CGETSET_MAGIC_DEF("globalCompositeOperation", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::GlobalCompositeOperationAttrNum),
    JS_CGETSET_MAGIC_DEF("lineCap", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::LineCapAttrNum),
    JS_CGETSET_MAGIC_DEF("shadowColor", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::ShadowColorAttrNum),
    JS_CGETSET_MAGIC_DEF("canvas", JSCanvasRenderingContext2D::getValueProperty, NULL, JSCanvasRenderingContext2D::CanvasAttrNum),
    JS_CGETSET_MAGIC_DEF("shadowBlur", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::ShadowBlurAttrNum),
    JS_CGETSET_MAGIC_DEF("miterLimit", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::MiterLimitAttrNum),
    JS_CGETSET_MAGIC_DEF("fillStyle", JSCanvasRenderingContext2D::getValueProperty, JSCanvasRenderingContext2D::putValueProperty, JSCanvasRenderingContext2D::FillStyleAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSCanvasRenderingContext2DPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("fill", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::FillFuncNum),
    JS_CFUNC_MAGIC_DEF("lineTo", 2, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::LineToFuncNum),
    JS_CFUNC_MAGIC_DEF("save", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SaveFuncNum),
    JS_CFUNC_MAGIC_DEF("restore", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::RestoreFuncNum),
    JS_CFUNC_MAGIC_DEF("drawImageFromRect", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::DrawImageFromRectFuncNum),
    JS_CFUNC_MAGIC_DEF("setLineJoin", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetLineJoinFuncNum),
    JS_CFUNC_MAGIC_DEF("createLinearGradient", 4, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::CreateLinearGradientFuncNum),
    JS_CFUNC_MAGIC_DEF("setShadow", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetShadowFuncNum),
    JS_CFUNC_MAGIC_DEF("clip", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ClipFuncNum),
    JS_CFUNC_MAGIC_DEF("rotate", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::RotateFuncNum),
    JS_CFUNC_MAGIC_DEF("setLineWidth", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetLineWidthFuncNum),
    JS_CFUNC_MAGIC_DEF("translate", 2, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::TranslateFuncNum),
    JS_CFUNC_MAGIC_DEF("setFillColor", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetFillColorFuncNum),
    JS_CFUNC_MAGIC_DEF("createPattern", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::CreatePatternFuncNum),
    JS_CFUNC_MAGIC_DEF("quadraticCurveTo", 4, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::QuadraticCurveToFuncNum),
    JS_CFUNC_MAGIC_DEF("scale", 2, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ScaleFuncNum),
    JS_CFUNC_MAGIC_DEF("setCompositeOperation", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetCompositeOperationFuncNum),
    JS_CFUNC_MAGIC_DEF("beginPath", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::BeginPathFuncNum),
    JS_CFUNC_MAGIC_DEF("bezierCurveTo", 6, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::BezierCurveToFuncNum),
    JS_CFUNC_MAGIC_DEF("moveTo", 2, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::MoveToFuncNum),
    JS_CFUNC_MAGIC_DEF("clearRect", 4, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ClearRectFuncNum),
    JS_CFUNC_MAGIC_DEF("arc", 6, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ArcFuncNum),
    JS_CFUNC_MAGIC_DEF("createRadialGradient", 6, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::CreateRadialGradientFuncNum),
    JS_CFUNC_MAGIC_DEF("fillRect", 4, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::FillRectFuncNum),
    JS_CFUNC_MAGIC_DEF("closePath", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ClosePathFuncNum),
    JS_CFUNC_MAGIC_DEF("arcTo", 5, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ArcToFuncNum),
    JS_CFUNC_MAGIC_DEF("rect", 4, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::RectFuncNum),
    JS_CFUNC_MAGIC_DEF("stroke", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::StrokeFuncNum),
    JS_CFUNC_MAGIC_DEF("setAlpha", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetAlphaFuncNum),
    JS_CFUNC_MAGIC_DEF("setLineCap", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetLineCapFuncNum),
    JS_CFUNC_MAGIC_DEF("setMiterLimit", 1, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetMiterLimitFuncNum),
    JS_CFUNC_MAGIC_DEF("clearShadow", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::ClearShadowFuncNum),
    JS_CFUNC_MAGIC_DEF("setStrokeColor", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::SetStrokeColorFuncNum),
    JS_CFUNC_MAGIC_DEF("strokeRect", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::StrokeRectFuncNum),
    JS_CFUNC_MAGIC_DEF("drawImage", 0, JSCanvasRenderingContext2DPrototypeFunction::callAsFunction, JSCanvasRenderingContext2D::DrawImageFuncNum)
};

JSValue JSCanvasRenderingContext2DPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCanvasRenderingContext2D.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCanvasRenderingContext2DPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCanvasRenderingContext2D.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCanvasRenderingContext2DPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCanvasRenderingContext2DAttributesFunctions, countof(JSCanvasRenderingContext2DAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCanvasRenderingContext2DPrototypeFunctions, countof(JSCanvasRenderingContext2DPrototypeFunctions));
}

static JSClassDef JSCanvasRenderingContext2DClassDefine = 
{
    "CanvasRenderingContext2D",
    .finalizer = JSCanvasRenderingContext2D::finalizer,
    .gc_mark = JSCanvasRenderingContext2D::mark,
};

JSClassID JSCanvasRenderingContext2D::js_class_id = 0;

void JSCanvasRenderingContext2D::init(JSContext* ctx)
{
    if (JSCanvasRenderingContext2D::js_class_id == 0) {
        JS_NewClassID(&JSCanvasRenderingContext2D::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCanvasRenderingContext2D::js_class_id, &JSCanvasRenderingContext2DClassDefine);
        JS_SetClassProto(ctx, JSCanvasRenderingContext2D::js_class_id, JSCanvasRenderingContext2DPrototype::self(ctx));
    }
}

JSValue JSCanvasRenderingContext2D::create(JSContext* ctx, CanvasRenderingContext2D* impl)
{
    JSCanvasRenderingContext2D::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCanvasRenderingContext2DPrototype::self(ctx), JSCanvasRenderingContext2D::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCanvasRenderingContext2D::finalizer(JSRuntime* rt, JSValue val)
{
    CanvasRenderingContext2D* impl = (CanvasRenderingContext2D*)JS_GetOpaque(val, JSCanvasRenderingContext2D::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCanvasRenderingContext2D::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCanvasRenderingContext2D::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CanvasAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->canvas()));
        }
        case GlobalAlphaAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->globalAlpha());
        }
        case GlobalCompositeOperationAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->globalCompositeOperation()).utf8().data());
        }
        case LineWidthAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->lineWidth());
        }
        case LineCapAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lineCap()).utf8().data());
        }
        case LineJoinAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lineJoin()).utf8().data());
        }
        case MiterLimitAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->miterLimit());
        }
        case ShadowOffsetXAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->shadowOffsetX());
        }
        case ShadowOffsetYAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->shadowOffsetY());
        }
        case ShadowBlurAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewBigUint64(ctx, imp->shadowBlur());
        }
        case ShadowColorAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shadowColor()).utf8().data());
        }
        case StrokeStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JSCanvasRenderingContext2D::strokeStyle(ctx, imp);
        }
        case FillStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            return JSCanvasRenderingContext2D::fillStyle(ctx, imp);
        }
    }
    return JS_NULL;
}

JSValue JSCanvasRenderingContext2D::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case GlobalAlphaAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setGlobalAlpha(valueToFloat(ctx, value));
            break;
        }
        case GlobalCompositeOperationAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setGlobalCompositeOperation(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LineWidthAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineWidth(valueToFloat(ctx, value));
            break;
        }
        case LineCapAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineCap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LineJoinAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineJoin(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MiterLimitAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setMiterLimit(valueToFloat(ctx, value));
            break;
        }
        case ShadowOffsetXAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowOffsetX(valueToFloat(ctx, value));
            break;
        }
        case ShadowOffsetYAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowOffsetY(valueToFloat(ctx, value));
            break;
        }
        case ShadowBlurAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowBlur(valueToFloat(ctx, value));
            break;
        }
        case ShadowColorAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case StrokeStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            JSCanvasRenderingContext2D::setStrokeStyle(ctx, value, imp);
            break;
        }
        case FillStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
            JSCanvasRenderingContext2D::setFillStyle(ctx, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2DPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque2(ctx, this_val, JSCanvasRenderingContext2D::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCanvasRenderingContext2D::SaveFuncNum: {
            imp->save();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::RestoreFuncNum: {
            imp->restore();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ScaleFuncNum: {
            float sx = valueToFloat(ctx, argv[0]);
            float sy = valueToFloat(ctx, argv[1]);
            imp->scale(sx, sy);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::RotateFuncNum: {
            float angle = valueToFloat(ctx, argv[0]);
            imp->rotate(angle);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::TranslateFuncNum: {
            float tx = valueToFloat(ctx, argv[0]);
            float ty = valueToFloat(ctx, argv[1]);
            imp->translate(tx, ty);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::CreateLinearGradientFuncNum: {
            float x0 = valueToFloat(ctx, argv[0]);
            float y0 = valueToFloat(ctx, argv[1]);
            float x1 = valueToFloat(ctx, argv[2]);
            float y1 = valueToFloat(ctx, argv[3]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createLinearGradient(x0, y0, x1, y1)));
            return result;
        }
        case JSCanvasRenderingContext2D::CreateRadialGradientFuncNum: {
            float x0 = valueToFloat(ctx, argv[0]);
            float y0 = valueToFloat(ctx, argv[1]);
            float r0 = valueToFloat(ctx, argv[2]);
            float x1 = valueToFloat(ctx, argv[3]);
            float y1 = valueToFloat(ctx, argv[4]);
            float r1 = valueToFloat(ctx, argv[5]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createRadialGradient(x0, y0, r0, x1, y1, r1)));
            return result;
        }
        case JSCanvasRenderingContext2D::ClearRectFuncNum: {
            ExceptionCode ec = 0;
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float width = valueToFloat(ctx, argv[2]);
            float height = valueToFloat(ctx, argv[3]);
            imp->clearRect(x, y, width, height, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::FillRectFuncNum: {
            ExceptionCode ec = 0;
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float width = valueToFloat(ctx, argv[2]);
            float height = valueToFloat(ctx, argv[3]);
            imp->fillRect(x, y, width, height, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::BeginPathFuncNum: {
            imp->beginPath();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ClosePathFuncNum: {
            imp->closePath();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::MoveToFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            imp->moveTo(x, y);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::LineToFuncNum: {
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            imp->lineTo(x, y);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::QuadraticCurveToFuncNum: {
            float cpx = valueToFloat(ctx, argv[0]);
            float cpy = valueToFloat(ctx, argv[1]);
            float x = valueToFloat(ctx, argv[2]);
            float y = valueToFloat(ctx, argv[3]);
            imp->quadraticCurveTo(cpx, cpy, x, y);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::BezierCurveToFuncNum: {
            float cp1x = valueToFloat(ctx, argv[0]);
            float cp1y = valueToFloat(ctx, argv[1]);
            float cp2x = valueToFloat(ctx, argv[2]);
            float cp2y = valueToFloat(ctx, argv[3]);
            float x = valueToFloat(ctx, argv[4]);
            float y = valueToFloat(ctx, argv[5]);
            imp->bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ArcToFuncNum: {
            ExceptionCode ec = 0;
            float x1 = valueToFloat(ctx, argv[0]);
            float y1 = valueToFloat(ctx, argv[1]);
            float x2 = valueToFloat(ctx, argv[2]);
            float y2 = valueToFloat(ctx, argv[3]);
            float radius = valueToFloat(ctx, argv[4]);
            imp->arcTo(x1, y1, x2, y2, radius, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::RectFuncNum: {
            ExceptionCode ec = 0;
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float width = valueToFloat(ctx, argv[2]);
            float height = valueToFloat(ctx, argv[3]);
            imp->rect(x, y, width, height, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ArcFuncNum: {
            ExceptionCode ec = 0;
            float x = valueToFloat(ctx, argv[0]);
            float y = valueToFloat(ctx, argv[1]);
            float radius = valueToFloat(ctx, argv[2]);
            float startAngle = valueToFloat(ctx, argv[3]);
            float endAngle = valueToFloat(ctx, argv[4]);
            bool clockwise = valueToBoolean(ctx, argv[5]);
            imp->arc(x, y, radius, startAngle, endAngle, clockwise, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::FillFuncNum: {
            imp->fill();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::StrokeFuncNum: {
            imp->stroke();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ClipFuncNum: {
            imp->clip();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetAlphaFuncNum: {
            float alpha = valueToFloat(ctx, argv[0]);
            imp->setAlpha(alpha);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetCompositeOperationFuncNum: {
            String compositeOperation = valueToString(ctx, argv[0]);
            imp->setCompositeOperation(compositeOperation);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetLineWidthFuncNum: {
            float width = valueToFloat(ctx, argv[0]);
            imp->setLineWidth(width);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetLineCapFuncNum: {
            String cap = valueToString(ctx, argv[0]);
            imp->setLineCap(cap);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetLineJoinFuncNum: {
            String join = valueToString(ctx, argv[0]);
            imp->setLineJoin(join);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetMiterLimitFuncNum: {
            float limit = valueToFloat(ctx, argv[0]);
            imp->setMiterLimit(limit);
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::ClearShadowFuncNum: {
            imp->clearShadow();
            return JS_UNDEFINED;
        }
        case JSCanvasRenderingContext2D::SetStrokeColorFuncNum: {
            return JSCanvasRenderingContext2D::setStrokeColor(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::SetFillColorFuncNum: {
            return JSCanvasRenderingContext2D::setFillColor(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::StrokeRectFuncNum: {
            return JSCanvasRenderingContext2D::strokeRect(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::DrawImageFuncNum: {
            return JSCanvasRenderingContext2D::drawImage(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::DrawImageFromRectFuncNum: {
            return JSCanvasRenderingContext2D::drawImageFromRect(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::SetShadowFuncNum: {
            return JSCanvasRenderingContext2D::setShadow(ctx, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::CreatePatternFuncNum: {
            return JSCanvasRenderingContext2D::createPattern(ctx, argc, argv, imp);
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, CanvasRenderingContext2D* obj)
{
    return QJS::cacheDOMObject<CanvasRenderingContext2D, JSCanvasRenderingContext2D>(ctx, obj);
}

CanvasRenderingContext2D* toCanvasRenderingContext2D(JSValue val)
{
    if (JS_IsObject(val)) {
        CanvasRenderingContext2D* impl = (CanvasRenderingContext2D*)JS_GetOpaque(val, JSCanvasRenderingContext2D::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
