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

#if ENABLE(QJS)

#include "QJSCanvasRenderingContext2D.h"

#include "CanvasGradient.h"
#include "CanvasPattern.h"
#include "CanvasRenderingContext2D.h"
#include "CanvasStyle.h"
#include "ExceptionCode.h"
#include "FloatRect.h"
#include "HTMLCanvasElement.h"
#include "HTMLImageElement.h"
#include "QJSCanvasGradient.h"
#include "QJSCanvasPattern.h"
#include "QJSHTMLCanvasElement.h"
#include "QJSHTMLImageElement.h"
#include "qjs_binding.h"
#include "qjs_html.h"

using namespace QJS;

namespace WebCore {

static JSValue toJS(JSContext *ctx, CanvasStyle* style)
{
    if (style->gradient())
        return toJS(ctx, style->gradient());
    if (style->pattern())
        return toJS(ctx, style->pattern());

    return JS_NewString(ctx, style->color().utf8().data());
}

static PassRefPtr<CanvasStyle> toHTMLCanvasStyle(JSContext *ctx, JSValue value)
{
    if (JS_IsString(value))
        return new CanvasStyle(valueToString(ctx, value));

    if (!JS_IsObject(value))
        return 0;

    CanvasGradient* gradient = (CanvasGradient*)JS_GetOpaque(value, JSCanvasGradient::js_class_id);
    if (gradient)
        return new CanvasStyle(gradient);

    CanvasPattern* pattern = (CanvasPattern*)JS_GetOpaque(value, JSCanvasPattern::js_class_id);
    if (pattern)
        return new CanvasStyle(pattern);

    return 0;
}

JSValue JSCanvasRenderingContext2D::strokeStyle(JSContext *ctx, JSValueConst this_val, CanvasRenderingContext2D *impl)
{
    return toJS(ctx, impl->strokeStyle());        
}

void JSCanvasRenderingContext2D::setStrokeStyle(JSContext *ctx, JSValueConst this_val, JSValue value, CanvasRenderingContext2D *impl)
{
    impl->setStrokeStyle(toHTMLCanvasStyle(ctx, value));
}

JSValue JSCanvasRenderingContext2D::fillStyle(JSContext *ctx, JSValueConst this_val, CanvasRenderingContext2D *impl)
{
    return toJS(ctx, impl->fillStyle());
}

void JSCanvasRenderingContext2D::setFillStyle(JSContext *ctx, JSValueConst this_val, JSValue value, CanvasRenderingContext2D *impl)
{
    impl->setFillStyle(toHTMLCanvasStyle(ctx, value));
}

JSValue JSCanvasRenderingContext2D::setFillColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{
    CanvasRenderingContext2D* context = impl;

    // string arg = named color
    // number arg = gray color
    // string arg, number arg = named color, alpha
    // number arg, number arg = gray color, alpha
    // 4 args = r, g, b, a
    // 5 args = c, m, y, k, a
    switch (argc) {
        case 1:
            if (JS_IsString(argv[0]))
                context->setFillColor(valueToString(ctx, argv[0]));
            else
                context->setFillColor(valueToFloat(ctx, argv[0]));
            break;
        case 2:
            if (JS_IsString(argv[0]))
                context->setFillColor(valueToString(ctx, argv[0]), valueToFloat(ctx, argv[1]));
            else
                context->setFillColor(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]));
            break;
        case 4:
            context->setFillColor(valueToFloat(ctx, argv[0]),
                                  valueToFloat(ctx, argv[1]),
                                  valueToFloat(ctx, argv[2]),
                                  valueToFloat(ctx, argv[3]));
            break;
        case 5:
            context->setFillColor(valueToFloat(ctx, argv[0]),
                                  valueToFloat(ctx, argv[1]),
                                  valueToFloat(ctx, argv[2]),
                                  valueToFloat(ctx, argv[3]),
                                  valueToFloat(ctx, argv[4]));
            break;
        default:
            return JS_ThrowSyntaxError(ctx, "Syntax Error");
    }
    return JS_UNDEFINED;
}    

JSValue JSCanvasRenderingContext2D::setStrokeColor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;

    // string arg = named color
    // number arg = gray color
    // string arg, number arg = named color, alpha
    // number arg, number arg = gray color, alpha
    // 4 args = r, g, b, a
    // 5 args = c, m, y, k, a
    switch (argc) {
        case 1:
            if (JS_IsString(argv[0]))
                context->setStrokeColor(valueToString(ctx, argv[0]));
            else
                context->setStrokeColor(valueToFloat(ctx, argv[0]));
            break;
        case 2:
            if (JS_IsString(argv[0]))
                context->setStrokeColor(valueToString(ctx, argv[0]), valueToFloat(ctx, argv[1]));
            else
                context->setStrokeColor(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]));
            break;
        case 4:
            context->setStrokeColor(valueToFloat(ctx, argv[0]),
                                    valueToFloat(ctx, argv[1]),
                                    valueToFloat(ctx, argv[2]),
                                    valueToFloat(ctx, argv[3]));
            break;
        case 5:
            context->setStrokeColor(valueToFloat(ctx, argv[0]),
                                    valueToFloat(ctx, argv[1]),
                                    valueToFloat(ctx, argv[2]),
                                    valueToFloat(ctx, argv[3]),
                                    valueToFloat(ctx, argv[4]));
            break;
        default:
            return JS_ThrowSyntaxError(ctx, "Syntax Error");
    }
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2D::strokeRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;    
    ExceptionCode ec;
    
    if (argc <= 4) {
        context->strokeRect(valueToFloat(ctx, argv[0]),
                            valueToFloat(ctx, argv[1]),
                            valueToFloat(ctx, argv[2]),
                            valueToFloat(ctx, argv[3]), ec);
    } else {
        context->strokeRect(valueToFloat(ctx, argv[0]),
                            valueToFloat(ctx, argv[1]),
                            valueToFloat(ctx, argv[2]),
                            valueToFloat(ctx, argv[3]),
                            valueToFloat(ctx, argv[4]), ec);
    }
    setDOMException(ctx, ec);
    
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2D::drawImage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;

    // DrawImage has three variants:
    //     drawImage(img, dx, dy)
    //     drawImage(img, dx, dy, dw, dh)
    //     drawImage(img, sx, sy, sw, sh, dx, dy, dw, dh)
    // Composite operation is specified with globalCompositeOperation.
    // The img parameter can be a <img> or <canvas> element.
    JSValue value = argv[0];
    if (!JS_IsObject(value))
        return JS_ThrowTypeError(ctx, "Type Error");

    HTMLImageElement* imgElt = (HTMLImageElement*)JS_GetOpaque(value, JSHTMLImageElement::js_class_id);
    HTMLCanvasElement* canvas = (HTMLCanvasElement*)JS_GetOpaque(value, JSHTMLCanvasElement::js_class_id);
    
    ExceptionCode ec = 0;
    if (imgElt) {
        switch (argc) {
            case 3:
                context->drawImage(imgElt, valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]));
                break;
            case 5:
                context->drawImage(imgElt, valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]),
                                   valueToFloat(ctx, argv[3]), valueToFloat(ctx, argv[4]), ec);
                setDOMException(ctx, ec);
                break;
            case 9:
                context->drawImage(imgElt, FloatRect(valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]),
                                   valueToFloat(ctx, argv[3]), valueToFloat(ctx, argv[4])),
                                   FloatRect(valueToFloat(ctx, argv[5]), valueToFloat(ctx, argv[6]),
                                   valueToFloat(ctx, argv[7]), valueToFloat(ctx, argv[8])), ec);
                setDOMException(ctx, ec);
                break;
            default:
                return JS_ThrowSyntaxError(ctx, "Syntax Error");
        }
    } else if (canvas) {
        switch (argc) {
            case 3:
                context->drawImage(canvas, valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]));
                break;
            case 5:
                context->drawImage(canvas, valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]),
                                   valueToFloat(ctx, argv[3]), valueToFloat(ctx, argv[4]), ec);
                setDOMException(ctx, ec);
                break;
            case 9:
                context->drawImage(canvas, FloatRect(valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]),
                                   valueToFloat(ctx, argv[3]), valueToFloat(ctx, argv[4])),
                                   FloatRect(valueToFloat(ctx, argv[5]), valueToFloat(ctx, argv[6]),
                                   valueToFloat(ctx, argv[7]), valueToFloat(ctx, argv[8])), ec);
                setDOMException(ctx, ec);
                break;
            default:
                return JS_ThrowSyntaxError(ctx, "Syntax Error");
        }
    } else {
        setDOMException(ctx, TYPE_MISMATCH_ERR);
        return JS_NULL;
    }
    
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2D::drawImageFromRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;
    
    JSValue value = argv[0];
    if (!JS_IsObject(value))
        return JS_ThrowTypeError(ctx, "Type Error");

    HTMLImageElement* imgElt = (HTMLImageElement*)JS_GetOpaque(value, JSHTMLImageElement::js_class_id);
    
    if (!imgElt)
        return JS_ThrowTypeError(ctx, "Type Error");
    context->drawImageFromRect(imgElt,
                               valueToFloat(ctx, argv[1]), valueToFloat(ctx, argv[2]),
                               valueToFloat(ctx, argv[3]), valueToFloat(ctx, argv[4]),
                               valueToFloat(ctx, argv[5]), valueToFloat(ctx, argv[6]),
                               valueToFloat(ctx, argv[7]), valueToFloat(ctx, argv[8]),
                               valueToString(ctx, argv[9]));
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2D::setShadow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;

    switch (argc) {
        case 3:
            context->setShadow(valueToFloat(ctx, argv[0]),
                               valueToFloat(ctx, argv[1]),
                               valueToFloat(ctx, argv[2]));
            break;
        case 4:
            if (JS_IsString(argv[3]))
                context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                                   valueToFloat(ctx, argv[2]), valueToString(ctx, argv[3]));
            else
                context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                                   valueToFloat(ctx, argv[2]), valueToFloat(ctx, argv[3]));
            break;
        case 5:
            if (JS_IsString(argv[3]))
                context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                                   valueToFloat(ctx, argv[2]), valueToString(ctx, argv[3]),
                                   valueToFloat(ctx, argv[4]));
            else
                context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                                   valueToFloat(ctx, argv[2]), valueToFloat(ctx, argv[3]),
                                   valueToFloat(ctx, argv[4]));
            break;
        case 7:
            context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                               valueToFloat(ctx, argv[2]), valueToFloat(ctx, argv[3]),
                               valueToFloat(ctx, argv[4]), valueToFloat(ctx, argv[5]),
                               valueToFloat(ctx, argv[6]));
            break;
        case 8:
            context->setShadow(valueToFloat(ctx, argv[0]), valueToFloat(ctx, argv[1]),
                               valueToFloat(ctx, argv[2]), valueToFloat(ctx, argv[3]),
                               valueToFloat(ctx, argv[4]), valueToFloat(ctx, argv[5]),
                               valueToFloat(ctx, argv[6]), valueToFloat(ctx, argv[7]));
            break;
        default:
            return JS_ThrowSyntaxError(ctx, "Syntax Error");
    }
    
    return JS_UNDEFINED;
}

JSValue JSCanvasRenderingContext2D::createPattern(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, CanvasRenderingContext2D *impl)
{ 
    CanvasRenderingContext2D* context = impl;

    JSValue value = argv[0];
    if (!JS_IsObject(value))
        return JS_ThrowTypeError(ctx, "Type Error");
    
    HTMLImageElement* imgElt = (HTMLImageElement*)JS_GetOpaque(value, JSHTMLImageElement::js_class_id);
    HTMLCanvasElement* canvas = (HTMLCanvasElement*)JS_GetOpaque(value, JSHTMLCanvasElement::js_class_id);

    if (imgElt) {
        ExceptionCode ec;
        JSValue pattern = toJS(ctx,
            context->createPattern(imgElt, valueToString(ctx, argv[1]), ec).get());
        setDOMException(ctx, ec);
        return pattern;
    }
    if (canvas) {
        ExceptionCode ec;
        JSValue pattern = toJS(ctx,
            context->createPattern(canvas, valueToString(ctx, argv[1]), ec).get());
        setDOMException(ctx, ec);
        return pattern;
    }
    setDOMException(ctx, TYPE_MISMATCH_ERR);
    return JS_NULL;
}

} // namespace WebCore
#endif
