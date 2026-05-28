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
#include "QJSSVGColor.h"

#include "SVGColor.h"
#include "qjs_css.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGColorAttributesFunctions[3];
static bool JSSVGColorAttributesFunctions_initialized = false;

static void init_JSSVGColorAttributesFunctions()
{
    if (JSSVGColorAttributesFunctions_initialized) return;
    JSSVGColorAttributesFunctions_initialized = true;
    memset(JSSVGColorAttributesFunctions, 0, sizeof(JSSVGColorAttributesFunctions));
    JSSVGColorAttributesFunctions[0].name = "rgbColor";
    JSSVGColorAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGColorAttributesFunctions[0].magic = JSSVGColor::RgbColorAttrNum;
    JSSVGColorAttributesFunctions[0].u.getset.get.getter_magic = JSSVGColor::getValueProperty;
    JSSVGColorAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGColorAttributesFunctions[1].name = "colorType";
    JSSVGColorAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGColorAttributesFunctions[1].magic = JSSVGColor::ColorTypeAttrNum;
    JSSVGColorAttributesFunctions[1].u.getset.get.getter_magic = JSSVGColor::getValueProperty;
    JSSVGColorAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGColorAttributesFunctions[2].name = "constructor";
    JSSVGColorAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGColorAttributesFunctions[2].magic = JSSVGColor::ConstructorAttrNum;
    JSSVGColorAttributesFunctions[2].u.getset.get.getter_magic = JSSVGColor::getValueProperty;
    JSSVGColorAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSSVGColorConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGColorConstructorFunctions[4];
static bool JSSVGColorConstructorFunctions_initialized = false;

static void init_JSSVGColorConstructorFunctions()
{
    if (JSSVGColorConstructorFunctions_initialized) return;
    JSSVGColorConstructorFunctions_initialized = true;
    memset(JSSVGColorConstructorFunctions, 0, sizeof(JSSVGColorConstructorFunctions));
    JSSVGColorConstructorFunctions[0].name = "SVG_COLORTYPE_UNKNOWN";
    JSSVGColorConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGColorConstructorFunctions[0].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_UNKNOWN;
    JSSVGColorConstructorFunctions[1].name = "SVG_COLORTYPE_RGBCOLOR";
    JSSVGColorConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGColorConstructorFunctions[1].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_RGBCOLOR;
    JSSVGColorConstructorFunctions[2].name = "SVG_COLORTYPE_RGBCOLOR_ICCCOLOR";
    JSSVGColorConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGColorConstructorFunctions[2].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_RGBCOLOR_ICCCOLOR;
    JSSVGColorConstructorFunctions[3].name = "SVG_COLORTYPE_CURRENTCOLOR";
    JSSVGColorConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGColorConstructorFunctions[3].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_CURRENTCOLOR;
}

JSValue JSSVGColorConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGColor.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGColorConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGColor.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGColorConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGColorConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorConstructorFunctions, countof(JSSVGColorConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGColorPrototypeConstantsFunctions[4];
static bool JSSVGColorPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGColorPrototypeConstantsFunctions()
{
    if (JSSVGColorPrototypeConstantsFunctions_initialized) return;
    JSSVGColorPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGColorPrototypeConstantsFunctions, 0, sizeof(JSSVGColorPrototypeConstantsFunctions));
    JSSVGColorPrototypeConstantsFunctions[0].name = "SVG_COLORTYPE_UNKNOWN";
    JSSVGColorPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGColorPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_UNKNOWN;
    JSSVGColorPrototypeConstantsFunctions[1].name = "SVG_COLORTYPE_RGBCOLOR";
    JSSVGColorPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGColorPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_RGBCOLOR;
    JSSVGColorPrototypeConstantsFunctions[2].name = "SVG_COLORTYPE_RGBCOLOR_ICCCOLOR";
    JSSVGColorPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGColorPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_RGBCOLOR_ICCCOLOR;
    JSSVGColorPrototypeConstantsFunctions[3].name = "SVG_COLORTYPE_CURRENTCOLOR";
    JSSVGColorPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGColorPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGColor::SVG_COLORTYPE_CURRENTCOLOR;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGColorPrototypeFunctions[3];
static bool JSSVGColorPrototypeFunctions_initialized = false;

static void init_JSSVGColorPrototypeFunctions()
{
    if (JSSVGColorPrototypeFunctions_initialized) return;
    JSSVGColorPrototypeFunctions_initialized = true;
    memset(JSSVGColorPrototypeFunctions, 0, sizeof(JSSVGColorPrototypeFunctions));
    JSSVGColorPrototypeFunctions[0].name = "setRGBColorICCColor";
    JSSVGColorPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGColorPrototypeFunctions[0].magic = JSSVGColor::SetRGBColorICCColorFuncNum;
    JSSVGColorPrototypeFunctions[0].u.func.length = 2;
    JSSVGColorPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGColorPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGColorPrototypeFunction::callAsFunction;
    JSSVGColorPrototypeFunctions[1].name = "setRGBColor";
    JSSVGColorPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGColorPrototypeFunctions[1].magic = JSSVGColor::SetRGBColorFuncNum;
    JSSVGColorPrototypeFunctions[1].u.func.length = 1;
    JSSVGColorPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGColorPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGColorPrototypeFunction::callAsFunction;
    JSSVGColorPrototypeFunctions[2].name = "setColor";
    JSSVGColorPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGColorPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGColorPrototypeFunctions[2].magic = JSSVGColor::SetColorFuncNum;
    JSSVGColorPrototypeFunctions[2].u.func.length = 3;
    JSSVGColorPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGColorPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGColorPrototypeFunction::callAsFunction;
}

JSValue JSSVGColorPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGColor.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCSSValuePrototype::self(ctx));
        JSSVGColorPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGColor.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGColorPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGColorAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorAttributesFunctions, countof(JSSVGColorAttributesFunctions));
    init_JSSVGColorPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorPrototypeConstantsFunctions, countof(JSSVGColorPrototypeConstantsFunctions));
    init_JSSVGColorPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGColorPrototypeFunctions, countof(JSSVGColorPrototypeFunctions));
}

static JSClassDef JSSVGColorClassDefine;
static bool JSSVGColorClassDefine_initialized = false;

static void init_JSSVGColorClassDefine()
{
    if (JSSVGColorClassDefine_initialized) return;
    JSSVGColorClassDefine_initialized = true;
    memset(&JSSVGColorClassDefine, 0, sizeof(JSSVGColorClassDefine));
    JSSVGColorClassDefine.class_name = "SVGColor";
    JSSVGColorClassDefine.finalizer = JSSVGColor::finalizer;
    JSSVGColorClassDefine.gc_mark = JSSVGColor::mark;
}

JSClassID JSSVGColor::js_class_id = 0;

void JSSVGColor::init(JSContext* ctx)
{
    if (JSSVGColor::js_class_id == 0) {
        init_JSSVGColorClassDefine();
        JS_NewClassID(&JSSVGColor::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGColor::js_class_id, &JSSVGColorClassDefine);
        JS_SetConstructor(ctx, JSSVGColorConstructor::self(ctx), JSSVGColorPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGColor::js_class_id, JSSVGColorPrototype::self(ctx));
    }
}

JSValue JSSVGColor::create(JSContext* ctx, SVGColor* impl)
{
    JSSVGColor::init(ctx);
    JSValue _proto = JSSVGColorPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGColor::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGColor* imp = (SVGColor*)JS_GetOpaque(this_val, JSSVGColor::js_class_id);
            return JS_NewInt32(ctx, imp->colorType());
        }
        case RgbColorAttrNum: {
            SVGColor* imp = (SVGColor*)JS_GetOpaque(this_val, JSSVGColor::js_class_id);
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
    SVGColor* imp = (SVGColor*)JS_GetOpaque(this_val, JSSVGColor::js_class_id);
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
