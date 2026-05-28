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
#include "QJSSVGPaint.h"

#include "SVGPaint.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPaintAttributesFunctions[3];
static bool JSSVGPaintAttributesFunctions_initialized = false;

static void init_JSSVGPaintAttributesFunctions()
{
    if (JSSVGPaintAttributesFunctions_initialized) return;
    JSSVGPaintAttributesFunctions_initialized = true;
    memset(JSSVGPaintAttributesFunctions, 0, sizeof(JSSVGPaintAttributesFunctions));
    JSSVGPaintAttributesFunctions[0].name = "paintType";
    JSSVGPaintAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPaintAttributesFunctions[0].magic = JSSVGPaint::PaintTypeAttrNum;
    JSSVGPaintAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPaint::getValueProperty;
    JSSVGPaintAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPaintAttributesFunctions[1].name = "uri";
    JSSVGPaintAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPaintAttributesFunctions[1].magic = JSSVGPaint::UriAttrNum;
    JSSVGPaintAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPaint::getValueProperty;
    JSSVGPaintAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPaintAttributesFunctions[2].name = "constructor";
    JSSVGPaintAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPaintAttributesFunctions[2].magic = JSSVGPaint::ConstructorAttrNum;
    JSSVGPaintAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPaint::getValueProperty;
    JSSVGPaintAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSSVGPaintConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGPaintConstructorFunctions[10];
static bool JSSVGPaintConstructorFunctions_initialized = false;

static void init_JSSVGPaintConstructorFunctions()
{
    if (JSSVGPaintConstructorFunctions_initialized) return;
    JSSVGPaintConstructorFunctions_initialized = true;
    memset(JSSVGPaintConstructorFunctions, 0, sizeof(JSSVGPaintConstructorFunctions));
    JSSVGPaintConstructorFunctions[0].name = "SVG_PAINTTYPE_UNKNOWN";
    JSSVGPaintConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[0].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_UNKNOWN;
    JSSVGPaintConstructorFunctions[1].name = "SVG_PAINTTYPE_RGBCOLOR";
    JSSVGPaintConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[1].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_RGBCOLOR;
    JSSVGPaintConstructorFunctions[2].name = "SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR";
    JSSVGPaintConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[2].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR;
    JSSVGPaintConstructorFunctions[3].name = "SVG_PAINTTYPE_NONE";
    JSSVGPaintConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[3].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_NONE;
    JSSVGPaintConstructorFunctions[4].name = "SVG_PAINTTYPE_CURRENTCOLOR";
    JSSVGPaintConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[4].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_CURRENTCOLOR;
    JSSVGPaintConstructorFunctions[5].name = "SVG_PAINTTYPE_URI_NONE";
    JSSVGPaintConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[5].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_NONE;
    JSSVGPaintConstructorFunctions[6].name = "SVG_PAINTTYPE_URI_CURRENTCOLOR";
    JSSVGPaintConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[6].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_CURRENTCOLOR;
    JSSVGPaintConstructorFunctions[7].name = "SVG_PAINTTYPE_URI_RGBCOLOR";
    JSSVGPaintConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[7].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR;
    JSSVGPaintConstructorFunctions[8].name = "SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR";
    JSSVGPaintConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[8].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR;
    JSSVGPaintConstructorFunctions[9].name = "SVG_PAINTTYPE_URI";
    JSSVGPaintConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintConstructorFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintConstructorFunctions[9].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI;
}

JSValue JSSVGPaintConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPaint.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPaintConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPaint.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPaintConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPaintConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintConstructorFunctions, countof(JSSVGPaintConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGPaintPrototypeConstantsFunctions[10];
static bool JSSVGPaintPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGPaintPrototypeConstantsFunctions()
{
    if (JSSVGPaintPrototypeConstantsFunctions_initialized) return;
    JSSVGPaintPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGPaintPrototypeConstantsFunctions, 0, sizeof(JSSVGPaintPrototypeConstantsFunctions));
    JSSVGPaintPrototypeConstantsFunctions[0].name = "SVG_PAINTTYPE_UNKNOWN";
    JSSVGPaintPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_UNKNOWN;
    JSSVGPaintPrototypeConstantsFunctions[1].name = "SVG_PAINTTYPE_RGBCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_RGBCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[2].name = "SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[3].name = "SVG_PAINTTYPE_NONE";
    JSSVGPaintPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_NONE;
    JSSVGPaintPrototypeConstantsFunctions[4].name = "SVG_PAINTTYPE_CURRENTCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_CURRENTCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[5].name = "SVG_PAINTTYPE_URI_NONE";
    JSSVGPaintPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_NONE;
    JSSVGPaintPrototypeConstantsFunctions[6].name = "SVG_PAINTTYPE_URI_CURRENTCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[6].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_CURRENTCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[7].name = "SVG_PAINTTYPE_URI_RGBCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[7].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[8].name = "SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR";
    JSSVGPaintPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[8].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR;
    JSSVGPaintPrototypeConstantsFunctions[9].name = "SVG_PAINTTYPE_URI";
    JSSVGPaintPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeConstantsFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSSVGPaintPrototypeConstantsFunctions[9].u.i32 = (int32_t)SVGPaint::SVG_PAINTTYPE_URI;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPaintPrototypeFunctions[2];
static bool JSSVGPaintPrototypeFunctions_initialized = false;

static void init_JSSVGPaintPrototypeFunctions()
{
    if (JSSVGPaintPrototypeFunctions_initialized) return;
    JSSVGPaintPrototypeFunctions_initialized = true;
    memset(JSSVGPaintPrototypeFunctions, 0, sizeof(JSSVGPaintPrototypeFunctions));
    JSSVGPaintPrototypeFunctions[0].name = "setPaint";
    JSSVGPaintPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPaintPrototypeFunctions[0].magic = JSSVGPaint::SetPaintFuncNum;
    JSSVGPaintPrototypeFunctions[0].u.func.length = 4;
    JSSVGPaintPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPaintPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPaintPrototypeFunction::callAsFunction;
    JSSVGPaintPrototypeFunctions[1].name = "setUri";
    JSSVGPaintPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPaintPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPaintPrototypeFunctions[1].magic = JSSVGPaint::SetUriFuncNum;
    JSSVGPaintPrototypeFunctions[1].u.func.length = 1;
    JSSVGPaintPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPaintPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPaintPrototypeFunction::callAsFunction;
}

JSValue JSSVGPaintPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPaint.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGColorPrototype::self(ctx));
        JSSVGPaintPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPaint.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPaintPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPaintAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintAttributesFunctions, countof(JSSVGPaintAttributesFunctions));
    init_JSSVGPaintPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintPrototypeConstantsFunctions, countof(JSSVGPaintPrototypeConstantsFunctions));
    init_JSSVGPaintPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintPrototypeFunctions, countof(JSSVGPaintPrototypeFunctions));
}

static JSClassDef JSSVGPaintClassDefine;
static bool JSSVGPaintClassDefine_initialized = false;

static void init_JSSVGPaintClassDefine()
{
    if (JSSVGPaintClassDefine_initialized) return;
    JSSVGPaintClassDefine_initialized = true;
    memset(&JSSVGPaintClassDefine, 0, sizeof(JSSVGPaintClassDefine));
    JSSVGPaintClassDefine.class_name = "SVGPaint";
    JSSVGPaintClassDefine.finalizer = JSSVGPaint::finalizer;
    JSSVGPaintClassDefine.gc_mark = JSSVGPaint::mark;
}

JSClassID JSSVGPaint::js_class_id = 0;

void JSSVGPaint::init(JSContext* ctx)
{
    if (JSSVGPaint::js_class_id == 0) {
        init_JSSVGPaintClassDefine();
        JS_NewClassID(&JSSVGPaint::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPaint::js_class_id, &JSSVGPaintClassDefine);
        JS_SetConstructor(ctx, JSSVGPaintConstructor::self(ctx), JSSVGPaintPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPaint::js_class_id, JSSVGPaintPrototype::self(ctx));
    }
}

JSValue JSSVGPaint::create(JSContext* ctx, SVGPaint* impl)
{
    JSSVGPaint::init(ctx);
    JSValue _proto = JSSVGPaintPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPaint::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPaint::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPaint* impl = (SVGPaint*)JS_GetOpaque(val, JSSVGPaint::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPaint::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGColor::mark(rt, val, mark_func);
}

JSValue JSSVGPaint::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PaintTypeAttrNum: {
            SVGPaint* imp = (SVGPaint*)JS_GetOpaque(this_val, JSSVGPaint::js_class_id);
            return JS_NewInt32(ctx, imp->paintType());
        }
        case UriAttrNum: {
            SVGPaint* imp = (SVGPaint*)JS_GetOpaque(this_val, JSSVGPaint::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->uri()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGPaint::getConstructor(JSContext *ctx)
{
    return JSSVGPaintConstructor::self(ctx);
}

JSValue JSSVGPaintPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPaint* imp = (SVGPaint*)JS_GetOpaque(this_val, JSSVGPaint::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPaint::SetUriFuncNum: {
            String uri = valueToString(ctx, argv[0]);
            imp->setUri(uri);
            return JS_UNDEFINED;
        }
        case JSSVGPaint::SetPaintFuncNum: {
            ExceptionCode ec = 0;
            SVGPaint::SVGPaintType paintType = static_cast<SVGPaint::SVGPaintType>(valueToInt32(ctx, argv[0]));
            String uri = valueToString(ctx, argv[1]);
            String rgbColor = valueToString(ctx, argv[2]);
            String iccColor = valueToString(ctx, argv[3]);
            imp->setPaint(paintType, uri, rgbColor, iccColor, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
