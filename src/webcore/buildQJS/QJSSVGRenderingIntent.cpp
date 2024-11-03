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
#include "QJSSVGRenderingIntent.h"

#include "SVGRenderingIntent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGRenderingIntentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGRenderingIntent::getValueProperty, NULL, JSSVGRenderingIntent::ConstructorAttrNum)
};

class JSSVGRenderingIntentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGRenderingIntentConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGRenderingIntentConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_UNKNOWN", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_SATURATION", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_SATURATION),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_AUTO", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_AUTO),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_PERCEPTUAL", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_PERCEPTUAL),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_RELATIVE_COLORIMETRIC", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_RELATIVE_COLORIMETRIC),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_ABSOLUTE_COLORIMETRIC", JSSVGRenderingIntentConstructor::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_ABSOLUTE_COLORIMETRIC)
};

JSValue JSSVGRenderingIntentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGRenderingIntent.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRenderingIntentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGRenderingIntent.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGRenderingIntentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentConstructorFunctions, countof(JSSVGRenderingIntentConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGRenderingIntentPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_UNKNOWN", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_SATURATION", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_SATURATION),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_AUTO", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_AUTO),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_PERCEPTUAL", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_PERCEPTUAL),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_RELATIVE_COLORIMETRIC", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_RELATIVE_COLORIMETRIC),
    JS_CGETSET_MAGIC_DEF("RENDERING_INTENT_ABSOLUTE_COLORIMETRIC", JSSVGRenderingIntentPrototype::getValueProperty, NULL, SVGRenderingIntent::RENDERING_INTENT_ABSOLUTE_COLORIMETRIC)
};

JSValue JSSVGRenderingIntentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRenderingIntent.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRenderingIntentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRenderingIntent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGRenderingIntentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentAttributesFunctions, countof(JSSVGRenderingIntentAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentPrototypeConstantsFunctions, countof(JSSVGRenderingIntentPrototypeConstantsFunctions));
}

JSValue JSSVGRenderingIntentPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGRenderingIntentClassDefine = 
{
    "SVGRenderingIntent",
    .finalizer = JSSVGRenderingIntent::finalizer,
    .gc_mark = JSSVGRenderingIntent::mark,
};

JSClassID JSSVGRenderingIntent::js_class_id = 0;

void JSSVGRenderingIntent::init(JSContext* ctx)
{
    if (JSSVGRenderingIntent::js_class_id == 0) {
        JS_NewClassID(&JSSVGRenderingIntent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRenderingIntent::js_class_id, &JSSVGRenderingIntentClassDefine);
        JS_SetConstructor(ctx, JSSVGRenderingIntentConstructor::self(ctx), JSSVGRenderingIntentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGRenderingIntent::js_class_id, JSSVGRenderingIntentPrototype::self(ctx));
    }
}

JSValue JSSVGRenderingIntent::create(JSContext* ctx, SVGRenderingIntent* impl, SVGElement* context)
{
    JSSVGRenderingIntent::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGRenderingIntentPrototype::self(ctx), JSSVGRenderingIntent::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGRenderingIntent::finalizer(JSRuntime* rt, JSValue val)
{
    SVGRenderingIntent* impl = (SVGRenderingIntent*)JS_GetOpaque(val, JSSVGRenderingIntent::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGRenderingIntent::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGRenderingIntent::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGRenderingIntent::getConstructor(JSContext *ctx)
{
    return JSSVGRenderingIntentConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, SVGRenderingIntent* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGRenderingIntent, JSSVGRenderingIntent>(ctx, obj, context);
}

SVGRenderingIntent* toSVGRenderingIntent(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGRenderingIntent* impl = (SVGRenderingIntent*)JS_GetOpaque(val, JSSVGRenderingIntent::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
