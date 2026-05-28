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

static JSCFunctionListEntry JSCanvasRenderingContext2DAttributesFunctions[13];
static bool JSCanvasRenderingContext2DAttributesFunctions_initialized = false;

static void init_JSCanvasRenderingContext2DAttributesFunctions()
{
    if (JSCanvasRenderingContext2DAttributesFunctions_initialized) return;
    JSCanvasRenderingContext2DAttributesFunctions_initialized = true;
    memset(JSCanvasRenderingContext2DAttributesFunctions, 0, sizeof(JSCanvasRenderingContext2DAttributesFunctions));
    JSCanvasRenderingContext2DAttributesFunctions[0].name = "canvas";
    JSCanvasRenderingContext2DAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[0].magic = JSCanvasRenderingContext2D::CanvasAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[0].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCanvasRenderingContext2DAttributesFunctions[1].name = "globalAlpha";
    JSCanvasRenderingContext2DAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[1].magic = JSCanvasRenderingContext2D::GlobalAlphaAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[1].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[1].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[2].name = "globalCompositeOperation";
    JSCanvasRenderingContext2DAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[2].magic = JSCanvasRenderingContext2D::GlobalCompositeOperationAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[2].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[2].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[3].name = "lineWidth";
    JSCanvasRenderingContext2DAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[3].magic = JSCanvasRenderingContext2D::LineWidthAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[3].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[3].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[4].name = "lineCap";
    JSCanvasRenderingContext2DAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[4].magic = JSCanvasRenderingContext2D::LineCapAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[4].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[4].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[5].name = "lineJoin";
    JSCanvasRenderingContext2DAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[5].magic = JSCanvasRenderingContext2D::LineJoinAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[5].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[5].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[6].name = "miterLimit";
    JSCanvasRenderingContext2DAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[6].magic = JSCanvasRenderingContext2D::MiterLimitAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[6].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[6].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[7].name = "shadowOffsetX";
    JSCanvasRenderingContext2DAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[7].magic = JSCanvasRenderingContext2D::ShadowOffsetXAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[7].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[7].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[8].name = "shadowOffsetY";
    JSCanvasRenderingContext2DAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[8].magic = JSCanvasRenderingContext2D::ShadowOffsetYAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[8].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[8].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[9].name = "shadowBlur";
    JSCanvasRenderingContext2DAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[9].magic = JSCanvasRenderingContext2D::ShadowBlurAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[9].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[9].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[10].name = "shadowColor";
    JSCanvasRenderingContext2DAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[10].magic = JSCanvasRenderingContext2D::ShadowColorAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[10].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[10].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[11].name = "strokeStyle";
    JSCanvasRenderingContext2DAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[11].magic = JSCanvasRenderingContext2D::StrokeStyleAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[11].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[11].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[12].name = "fillStyle";
    JSCanvasRenderingContext2DAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSCanvasRenderingContext2DAttributesFunctions[12].magic = JSCanvasRenderingContext2D::FillStyleAttrNum;
    JSCanvasRenderingContext2DAttributesFunctions[12].u.getset.get.getter_magic = JSCanvasRenderingContext2D::getValueProperty;
    JSCanvasRenderingContext2DAttributesFunctions[12].u.getset.set.setter_magic = JSCanvasRenderingContext2D::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSCanvasRenderingContext2DPrototypeFunctions[35];
static bool JSCanvasRenderingContext2DPrototypeFunctions_initialized = false;

static void init_JSCanvasRenderingContext2DPrototypeFunctions()
{
    if (JSCanvasRenderingContext2DPrototypeFunctions_initialized) return;
    JSCanvasRenderingContext2DPrototypeFunctions_initialized = true;
    memset(JSCanvasRenderingContext2DPrototypeFunctions, 0, sizeof(JSCanvasRenderingContext2DPrototypeFunctions));
    JSCanvasRenderingContext2DPrototypeFunctions[0].name = "save";
    JSCanvasRenderingContext2DPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[0].magic = JSCanvasRenderingContext2D::SaveFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[0].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[0].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[1].name = "restore";
    JSCanvasRenderingContext2DPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[1].magic = JSCanvasRenderingContext2D::RestoreFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[1].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[1].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[2].name = "scale";
    JSCanvasRenderingContext2DPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[2].magic = JSCanvasRenderingContext2D::ScaleFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[2].u.func.length = 2;
    JSCanvasRenderingContext2DPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[2].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[3].name = "rotate";
    JSCanvasRenderingContext2DPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[3].magic = JSCanvasRenderingContext2D::RotateFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[3].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[3].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[4].name = "translate";
    JSCanvasRenderingContext2DPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[4].magic = JSCanvasRenderingContext2D::TranslateFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[4].u.func.length = 2;
    JSCanvasRenderingContext2DPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[4].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[5].name = "createLinearGradient";
    JSCanvasRenderingContext2DPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[5].magic = JSCanvasRenderingContext2D::CreateLinearGradientFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[5].u.func.length = 4;
    JSCanvasRenderingContext2DPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[5].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[6].name = "createRadialGradient";
    JSCanvasRenderingContext2DPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[6].magic = JSCanvasRenderingContext2D::CreateRadialGradientFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[6].u.func.length = 6;
    JSCanvasRenderingContext2DPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[6].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[7].name = "clearRect";
    JSCanvasRenderingContext2DPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[7].magic = JSCanvasRenderingContext2D::ClearRectFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[7].u.func.length = 4;
    JSCanvasRenderingContext2DPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[7].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[8].name = "fillRect";
    JSCanvasRenderingContext2DPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[8].magic = JSCanvasRenderingContext2D::FillRectFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[8].u.func.length = 4;
    JSCanvasRenderingContext2DPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[8].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[9].name = "beginPath";
    JSCanvasRenderingContext2DPrototypeFunctions[9].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[9].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[9].magic = JSCanvasRenderingContext2D::BeginPathFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[9].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[9].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[9].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[10].name = "closePath";
    JSCanvasRenderingContext2DPrototypeFunctions[10].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[10].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[10].magic = JSCanvasRenderingContext2D::ClosePathFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[10].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[10].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[10].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[11].name = "moveTo";
    JSCanvasRenderingContext2DPrototypeFunctions[11].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[11].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[11].magic = JSCanvasRenderingContext2D::MoveToFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[11].u.func.length = 2;
    JSCanvasRenderingContext2DPrototypeFunctions[11].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[11].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[12].name = "lineTo";
    JSCanvasRenderingContext2DPrototypeFunctions[12].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[12].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[12].magic = JSCanvasRenderingContext2D::LineToFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[12].u.func.length = 2;
    JSCanvasRenderingContext2DPrototypeFunctions[12].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[12].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[13].name = "quadraticCurveTo";
    JSCanvasRenderingContext2DPrototypeFunctions[13].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[13].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[13].magic = JSCanvasRenderingContext2D::QuadraticCurveToFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[13].u.func.length = 4;
    JSCanvasRenderingContext2DPrototypeFunctions[13].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[13].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[14].name = "bezierCurveTo";
    JSCanvasRenderingContext2DPrototypeFunctions[14].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[14].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[14].magic = JSCanvasRenderingContext2D::BezierCurveToFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[14].u.func.length = 6;
    JSCanvasRenderingContext2DPrototypeFunctions[14].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[14].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[15].name = "arcTo";
    JSCanvasRenderingContext2DPrototypeFunctions[15].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[15].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[15].magic = JSCanvasRenderingContext2D::ArcToFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[15].u.func.length = 5;
    JSCanvasRenderingContext2DPrototypeFunctions[15].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[15].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[16].name = "rect";
    JSCanvasRenderingContext2DPrototypeFunctions[16].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[16].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[16].magic = JSCanvasRenderingContext2D::RectFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[16].u.func.length = 4;
    JSCanvasRenderingContext2DPrototypeFunctions[16].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[16].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[17].name = "arc";
    JSCanvasRenderingContext2DPrototypeFunctions[17].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[17].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[17].magic = JSCanvasRenderingContext2D::ArcFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[17].u.func.length = 6;
    JSCanvasRenderingContext2DPrototypeFunctions[17].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[17].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[18].name = "fill";
    JSCanvasRenderingContext2DPrototypeFunctions[18].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[18].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[18].magic = JSCanvasRenderingContext2D::FillFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[18].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[18].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[18].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[19].name = "stroke";
    JSCanvasRenderingContext2DPrototypeFunctions[19].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[19].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[19].magic = JSCanvasRenderingContext2D::StrokeFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[19].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[19].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[19].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[20].name = "clip";
    JSCanvasRenderingContext2DPrototypeFunctions[20].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[20].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[20].magic = JSCanvasRenderingContext2D::ClipFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[20].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[20].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[20].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[21].name = "setAlpha";
    JSCanvasRenderingContext2DPrototypeFunctions[21].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[21].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[21].magic = JSCanvasRenderingContext2D::SetAlphaFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[21].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[21].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[21].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[22].name = "setCompositeOperation";
    JSCanvasRenderingContext2DPrototypeFunctions[22].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[22].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[22].magic = JSCanvasRenderingContext2D::SetCompositeOperationFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[22].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[22].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[22].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[23].name = "setLineWidth";
    JSCanvasRenderingContext2DPrototypeFunctions[23].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[23].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[23].magic = JSCanvasRenderingContext2D::SetLineWidthFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[23].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[23].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[23].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[24].name = "setLineCap";
    JSCanvasRenderingContext2DPrototypeFunctions[24].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[24].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[24].magic = JSCanvasRenderingContext2D::SetLineCapFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[24].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[24].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[24].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[25].name = "setLineJoin";
    JSCanvasRenderingContext2DPrototypeFunctions[25].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[25].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[25].magic = JSCanvasRenderingContext2D::SetLineJoinFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[25].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[25].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[25].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[26].name = "setMiterLimit";
    JSCanvasRenderingContext2DPrototypeFunctions[26].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[26].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[26].magic = JSCanvasRenderingContext2D::SetMiterLimitFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[26].u.func.length = 1;
    JSCanvasRenderingContext2DPrototypeFunctions[26].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[26].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[27].name = "clearShadow";
    JSCanvasRenderingContext2DPrototypeFunctions[27].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[27].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[27].magic = JSCanvasRenderingContext2D::ClearShadowFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[27].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[27].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[27].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[28].name = "setStrokeColor";
    JSCanvasRenderingContext2DPrototypeFunctions[28].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[28].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[28].magic = JSCanvasRenderingContext2D::SetStrokeColorFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[28].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[28].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[28].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[29].name = "setFillColor";
    JSCanvasRenderingContext2DPrototypeFunctions[29].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[29].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[29].magic = JSCanvasRenderingContext2D::SetFillColorFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[29].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[29].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[29].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[30].name = "strokeRect";
    JSCanvasRenderingContext2DPrototypeFunctions[30].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[30].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[30].magic = JSCanvasRenderingContext2D::StrokeRectFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[30].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[30].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[30].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[31].name = "drawImage";
    JSCanvasRenderingContext2DPrototypeFunctions[31].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[31].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[31].magic = JSCanvasRenderingContext2D::DrawImageFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[31].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[31].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[31].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[32].name = "drawImageFromRect";
    JSCanvasRenderingContext2DPrototypeFunctions[32].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[32].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[32].magic = JSCanvasRenderingContext2D::DrawImageFromRectFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[32].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[32].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[32].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[33].name = "setShadow";
    JSCanvasRenderingContext2DPrototypeFunctions[33].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[33].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[33].magic = JSCanvasRenderingContext2D::SetShadowFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[33].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[33].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[33].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
    JSCanvasRenderingContext2DPrototypeFunctions[34].name = "createPattern";
    JSCanvasRenderingContext2DPrototypeFunctions[34].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCanvasRenderingContext2DPrototypeFunctions[34].def_type = JS_DEF_CFUNC;
    JSCanvasRenderingContext2DPrototypeFunctions[34].magic = JSCanvasRenderingContext2D::CreatePatternFuncNum;
    JSCanvasRenderingContext2DPrototypeFunctions[34].u.func.length = 0;
    JSCanvasRenderingContext2DPrototypeFunctions[34].u.func.cproto = JS_CFUNC_generic_magic;
    JSCanvasRenderingContext2DPrototypeFunctions[34].u.func.cfunc.generic_magic = JSCanvasRenderingContext2DPrototypeFunction::callAsFunction;
}

JSValue JSCanvasRenderingContext2DPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCanvasRenderingContext2D.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCanvasRenderingContext2DPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCanvasRenderingContext2D.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCanvasRenderingContext2DPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCanvasRenderingContext2DAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCanvasRenderingContext2DAttributesFunctions, countof(JSCanvasRenderingContext2DAttributesFunctions));
    init_JSCanvasRenderingContext2DPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCanvasRenderingContext2DPrototypeFunctions, countof(JSCanvasRenderingContext2DPrototypeFunctions));
}

static JSClassDef JSCanvasRenderingContext2DClassDefine;
static bool JSCanvasRenderingContext2DClassDefine_initialized = false;

static void init_JSCanvasRenderingContext2DClassDefine()
{
    if (JSCanvasRenderingContext2DClassDefine_initialized) return;
    JSCanvasRenderingContext2DClassDefine_initialized = true;
    memset(&JSCanvasRenderingContext2DClassDefine, 0, sizeof(JSCanvasRenderingContext2DClassDefine));
    JSCanvasRenderingContext2DClassDefine.class_name = "CanvasRenderingContext2D";
    JSCanvasRenderingContext2DClassDefine.finalizer = JSCanvasRenderingContext2D::finalizer;
    JSCanvasRenderingContext2DClassDefine.gc_mark = JSCanvasRenderingContext2D::mark;
}

JSClassID JSCanvasRenderingContext2D::js_class_id = 0;

void JSCanvasRenderingContext2D::init(JSContext* ctx)
{
    if (JSCanvasRenderingContext2D::js_class_id == 0) {
        init_JSCanvasRenderingContext2DClassDefine();
        JS_NewClassID(&JSCanvasRenderingContext2D::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCanvasRenderingContext2D::js_class_id, &JSCanvasRenderingContext2DClassDefine);
        JS_SetClassProto(ctx, JSCanvasRenderingContext2D::js_class_id, JSCanvasRenderingContext2DPrototype::self(ctx));
    }
}

JSValue JSCanvasRenderingContext2D::create(JSContext* ctx, CanvasRenderingContext2D* impl)
{
    JSCanvasRenderingContext2D::init(ctx);
    JSValue _proto = JSCanvasRenderingContext2DPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCanvasRenderingContext2D::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->canvas()));
        }
        case GlobalAlphaAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->globalAlpha());
        }
        case GlobalCompositeOperationAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->globalCompositeOperation()).utf8().data());
        }
        case LineWidthAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->lineWidth());
        }
        case LineCapAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lineCap()).utf8().data());
        }
        case LineJoinAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->lineJoin()).utf8().data());
        }
        case MiterLimitAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->miterLimit());
        }
        case ShadowOffsetXAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->shadowOffsetX());
        }
        case ShadowOffsetYAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->shadowOffsetY());
        }
        case ShadowBlurAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewInt32(ctx, imp->shadowBlur());
        }
        case ShadowColorAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->shadowColor()).utf8().data());
        }
        case StrokeStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JSCanvasRenderingContext2D::strokeStyle(ctx, this_val, imp);
        }
        case FillStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            return JSCanvasRenderingContext2D::fillStyle(ctx, this_val, imp);
        }
    }
    return JS_NULL;
}

JSValue JSCanvasRenderingContext2D::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case GlobalAlphaAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setGlobalAlpha(valueToFloat(ctx, value));
            break;
        }
        case GlobalCompositeOperationAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setGlobalCompositeOperation(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LineWidthAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineWidth(valueToFloat(ctx, value));
            break;
        }
        case LineCapAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineCap(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case LineJoinAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setLineJoin(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case MiterLimitAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setMiterLimit(valueToFloat(ctx, value));
            break;
        }
        case ShadowOffsetXAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowOffsetX(valueToFloat(ctx, value));
            break;
        }
        case ShadowOffsetYAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowOffsetY(valueToFloat(ctx, value));
            break;
        }
        case ShadowBlurAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowBlur(valueToFloat(ctx, value));
            break;
        }
        case ShadowColorAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            imp->setShadowColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case StrokeStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            JSCanvasRenderingContext2D::setStrokeStyle(ctx, this_val, value, imp);
            break;
        }
        case FillStyleAttrNum: {
            CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
            JSCanvasRenderingContext2D::setFillStyle(ctx, this_val, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2DPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CanvasRenderingContext2D* imp = (CanvasRenderingContext2D*)JS_GetOpaque(this_val, JSCanvasRenderingContext2D::js_class_id);
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
            return JSCanvasRenderingContext2D::setStrokeColor(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::SetFillColorFuncNum: {
            return JSCanvasRenderingContext2D::setFillColor(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::StrokeRectFuncNum: {
            return JSCanvasRenderingContext2D::strokeRect(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::DrawImageFuncNum: {
            return JSCanvasRenderingContext2D::drawImage(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::DrawImageFromRectFuncNum: {
            return JSCanvasRenderingContext2D::drawImageFromRect(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::SetShadowFuncNum: {
            return JSCanvasRenderingContext2D::setShadow(ctx, this_val, argc, argv, imp);
        }
        case JSCanvasRenderingContext2D::CreatePatternFuncNum: {
            return JSCanvasRenderingContext2D::createPattern(ctx, this_val, argc, argv, imp);
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
