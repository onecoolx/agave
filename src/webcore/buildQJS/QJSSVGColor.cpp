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
#include "QJSSVGColor.h"

#include "SVGColor.h"
#include "qjs_css.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGColorAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("rgbColor", JSSVGColor::getValueProperty, NULL, JSSVGColor::RgbColorAttrNum),
    JS_CGETSET_MAGIC_DEF("colorType", JSSVGColor::getValueProperty, NULL, JSSVGColor::ColorTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGColor::getValueProperty, NULL, JSSVGColor::ConstructorAttrNum)
};

class JSSVGColorConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGColorConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGColorConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_UNKNOWN", JSSVGColorConstructor::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_RGBCOLOR_ICCCOLOR", JSSVGColorConstructor::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_CURRENTCOLOR", JSSVGColorConstructor::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_RGBCOLOR", JSSVGColorConstructor::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_RGBCOLOR)
};

JSValue JSSVGColorConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGColor.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGColorConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGColor.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGColorConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorConstructorFunctions, countof(JSSVGColorConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGColorPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_UNKNOWN", JSSVGColorPrototype::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_RGBCOLOR_ICCCOLOR", JSSVGColorPrototype::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_CURRENTCOLOR", JSSVGColorPrototype::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_COLORTYPE_RGBCOLOR", JSSVGColorPrototype::getValueProperty, NULL, SVGColor::SVG_COLORTYPE_RGBCOLOR)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGColorPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("setRGBColorICCColor", 2, JSSVGColorPrototypeFunction::callAsFunction, JSSVGColor::SetRGBColorICCColorFuncNum),
    JS_CFUNC_MAGIC_DEF("setRGBColor", 1, JSSVGColorPrototypeFunction::callAsFunction, JSSVGColor::SetRGBColorFuncNum),
    JS_CFUNC_MAGIC_DEF("setColor", 3, JSSVGColorPrototypeFunction::callAsFunction, JSSVGColor::SetColorFuncNum)
};

JSValue JSSVGColorPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGColor.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSValuePrototype::self(ctx));
        JSSVGColorPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGColor.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGColorPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorAttributesFunctions, countof(JSSVGColorAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorPrototypeConstantsFunctions, countof(JSSVGColorPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorPrototypeFunctions, countof(JSSVGColorPrototypeFunctions));
}

JSValue JSSVGColorPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGColorClassDefine = 
{
    "SVGColor",
    .finalizer = JSSVGColor::finalizer,
    .gc_mark = JSSVGColor::mark,
};

JSClassID JSSVGColor::js_class_id = 0;

void JSSVGColor::init(JSContext* ctx)
{
    if (JSSVGColor::js_class_id == 0) {
        JS_NewClassID(&JSSVGColor::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGColor::js_class_id, &JSSVGColorClassDefine);
        JS_SetConstructor(ctx, JSSVGColorConstructor::self(ctx), JSSVGColorPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGColor::js_class_id, JSSVGColorPrototype::self(ctx));
    }
}

JSValue JSSVGColor::create(JSContext* ctx, SVGColor* impl)
{
    JSSVGColor::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGColorPrototype::self(ctx), JSSVGColor::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGColor::finalizer(JSRuntime* rt, JSValue val)
{
    SVGColor* impl = (SVGColor*)JS_GetOpaque(val, JSSVGColor::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGColor::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCSSValue::mark(rt, val, mark_func);
}

JSValue JSSVGColor::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColorTypeAttrNum: {
            SVGColor* imp = (SVGColor*)JS_GetOpaque2(ctx, this_val, JSSVGColor::js_class_id);
            return JS_NewBigUint64(ctx, imp->colorType());
        }
        case RgbColorAttrNum: {
            SVGColor* imp = (SVGColor*)JS_GetOpaque2(ctx, this_val, JSSVGColor::js_class_id);
            return getJSRGBColor(ctx, imp->rgbColor());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGColor::getConstructor(JSContext *ctx)
{
    return JSSVGColorConstructor::self(ctx);
}

JSValue JSSVGColorPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGColor* imp = (SVGColor*)JS_GetOpaque2(ctx, this_val, JSSVGColor::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGColor::SetRGBColorFuncNum: {
            ExceptionCode ec = 0;
            String rgbColor = valueToString(ctx, argv[0]);
            imp->setRGBColor(rgbColor, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGColor::SetRGBColorICCColorFuncNum: {
            ExceptionCode ec = 0;
            String rgbColor = valueToString(ctx, argv[0]);
            String iccColor = valueToString(ctx, argv[1]);
            imp->setRGBColorICCColor(rgbColor, iccColor, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGColor::SetColorFuncNum: {
            ExceptionCode ec = 0;
            unsigned short colorType = valueToInt32(ctx, argv[0]);
            String rgbColor = valueToString(ctx, argv[1]);
            String iccColor = valueToString(ctx, argv[2]);
            imp->setColor(colorType, rgbColor, iccColor, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
