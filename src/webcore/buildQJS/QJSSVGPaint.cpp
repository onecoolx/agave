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
#include "QJSSVGPaint.h"

#include "SVGPaint.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPaintAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("paintType", JSSVGPaint::getValueProperty, NULL, JSSVGPaint::PaintTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("uri", JSSVGPaint::getValueProperty, NULL, JSSVGPaint::UriAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGPaint::getValueProperty, NULL, JSSVGPaint::ConstructorAttrNum)
};

class JSSVGPaintConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGPaintConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGPaintConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_CURRENTCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_NONE", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_RGBCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_RGBCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_UNKNOWN", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_NONE", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_RGBCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_CURRENTCOLOR", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI", JSSVGPaintConstructor::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI)
};

JSValue JSSVGPaintConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPaint.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPaintConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPaint.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPaintConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintConstructorFunctions, countof(JSSVGPaintConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGPaintPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_CURRENTCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_NONE", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_RGBCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_RGBCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_UNKNOWN", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_NONE", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_NONE),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_RGBCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI_RGBCOLOR_ICCCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_CURRENTCOLOR", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_CURRENTCOLOR),
    JS_CGETSET_MAGIC_DEF("SVG_PAINTTYPE_URI", JSSVGPaintPrototype::getValueProperty, NULL, SVGPaint::SVG_PAINTTYPE_URI)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPaintPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("setPaint", 4, JSSVGPaintPrototypeFunction::callAsFunction, JSSVGPaint::SetPaintFuncNum),
    JS_CFUNC_MAGIC_DEF("setUri", 1, JSSVGPaintPrototypeFunction::callAsFunction, JSSVGPaint::SetUriFuncNum)
};

JSValue JSSVGPaintPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPaint.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGColorPrototype::self(ctx));
        JSSVGPaintPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPaint.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPaintPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintAttributesFunctions, countof(JSSVGPaintAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintPrototypeConstantsFunctions, countof(JSSVGPaintPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPaintPrototypeFunctions, countof(JSSVGPaintPrototypeFunctions));
}

JSValue JSSVGPaintPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGPaintClassDefine = 
{
    "SVGPaint",
    .finalizer = JSSVGPaint::finalizer,
    .gc_mark = JSSVGPaint::mark,
};

JSClassID JSSVGPaint::js_class_id = 0;

void JSSVGPaint::init(JSContext* ctx)
{
    if (JSSVGPaint::js_class_id == 0) {
        JS_NewClassID(&JSSVGPaint::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPaint::js_class_id, &JSSVGPaintClassDefine);
        JS_SetConstructor(ctx, JSSVGPaintConstructor::self(ctx), JSSVGPaintPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPaint::js_class_id, JSSVGPaintPrototype::self(ctx));
    }
}

JSValue JSSVGPaint::create(JSContext* ctx, SVGPaint* impl)
{
    JSSVGPaint::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPaintPrototype::self(ctx), JSSVGPaint::js_class_id);
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
            SVGPaint* imp = (SVGPaint*)JS_GetOpaque2(ctx, this_val, JSSVGPaint::js_class_id);
            return JS_NewBigUint64(ctx, imp->paintType());
        }
        case UriAttrNum: {
            SVGPaint* imp = (SVGPaint*)JS_GetOpaque2(ctx, this_val, JSSVGPaint::js_class_id);
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
    SVGPaint* imp = (SVGPaint*)JS_GetOpaque2(ctx, this_val, JSSVGPaint::js_class_id);
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
