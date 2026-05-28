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
#include "QJSSVGRenderingIntent.h"

#include "SVGRenderingIntent.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGRenderingIntentAttributesFunctions[1];
static bool JSSVGRenderingIntentAttributesFunctions_initialized = false;

static void init_JSSVGRenderingIntentAttributesFunctions()
{
    if (JSSVGRenderingIntentAttributesFunctions_initialized) return;
    JSSVGRenderingIntentAttributesFunctions_initialized = true;
    memset(JSSVGRenderingIntentAttributesFunctions, 0, sizeof(JSSVGRenderingIntentAttributesFunctions));
    JSSVGRenderingIntentAttributesFunctions[0].name = "constructor";
    JSSVGRenderingIntentAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGRenderingIntentAttributesFunctions[0].magic = JSSVGRenderingIntent::ConstructorAttrNum;
    JSSVGRenderingIntentAttributesFunctions[0].u.getset.get.getter_magic = JSSVGRenderingIntent::getValueProperty;
    JSSVGRenderingIntentAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSSVGRenderingIntentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGRenderingIntentConstructorFunctions[6];
static bool JSSVGRenderingIntentConstructorFunctions_initialized = false;

static void init_JSSVGRenderingIntentConstructorFunctions()
{
    if (JSSVGRenderingIntentConstructorFunctions_initialized) return;
    JSSVGRenderingIntentConstructorFunctions_initialized = true;
    memset(JSSVGRenderingIntentConstructorFunctions, 0, sizeof(JSSVGRenderingIntentConstructorFunctions));
    JSSVGRenderingIntentConstructorFunctions[0].name = "RENDERING_INTENT_UNKNOWN";
    JSSVGRenderingIntentConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[0].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_UNKNOWN;
    JSSVGRenderingIntentConstructorFunctions[1].name = "RENDERING_INTENT_AUTO";
    JSSVGRenderingIntentConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[1].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_AUTO;
    JSSVGRenderingIntentConstructorFunctions[2].name = "RENDERING_INTENT_PERCEPTUAL";
    JSSVGRenderingIntentConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[2].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_PERCEPTUAL;
    JSSVGRenderingIntentConstructorFunctions[3].name = "RENDERING_INTENT_RELATIVE_COLORIMETRIC";
    JSSVGRenderingIntentConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[3].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_RELATIVE_COLORIMETRIC;
    JSSVGRenderingIntentConstructorFunctions[4].name = "RENDERING_INTENT_SATURATION";
    JSSVGRenderingIntentConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[4].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_SATURATION;
    JSSVGRenderingIntentConstructorFunctions[5].name = "RENDERING_INTENT_ABSOLUTE_COLORIMETRIC";
    JSSVGRenderingIntentConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentConstructorFunctions[5].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_ABSOLUTE_COLORIMETRIC;
}

JSValue JSSVGRenderingIntentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGRenderingIntent.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRenderingIntentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGRenderingIntent.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGRenderingIntentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGRenderingIntentConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentConstructorFunctions, countof(JSSVGRenderingIntentConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGRenderingIntentPrototypeConstantsFunctions[6];
static bool JSSVGRenderingIntentPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGRenderingIntentPrototypeConstantsFunctions()
{
    if (JSSVGRenderingIntentPrototypeConstantsFunctions_initialized) return;
    JSSVGRenderingIntentPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGRenderingIntentPrototypeConstantsFunctions, 0, sizeof(JSSVGRenderingIntentPrototypeConstantsFunctions));
    JSSVGRenderingIntentPrototypeConstantsFunctions[0].name = "RENDERING_INTENT_UNKNOWN";
    JSSVGRenderingIntentPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_UNKNOWN;
    JSSVGRenderingIntentPrototypeConstantsFunctions[1].name = "RENDERING_INTENT_AUTO";
    JSSVGRenderingIntentPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_AUTO;
    JSSVGRenderingIntentPrototypeConstantsFunctions[2].name = "RENDERING_INTENT_PERCEPTUAL";
    JSSVGRenderingIntentPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_PERCEPTUAL;
    JSSVGRenderingIntentPrototypeConstantsFunctions[3].name = "RENDERING_INTENT_RELATIVE_COLORIMETRIC";
    JSSVGRenderingIntentPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_RELATIVE_COLORIMETRIC;
    JSSVGRenderingIntentPrototypeConstantsFunctions[4].name = "RENDERING_INTENT_SATURATION";
    JSSVGRenderingIntentPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[4].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_SATURATION;
    JSSVGRenderingIntentPrototypeConstantsFunctions[5].name = "RENDERING_INTENT_ABSOLUTE_COLORIMETRIC";
    JSSVGRenderingIntentPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGRenderingIntentPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGRenderingIntentPrototypeConstantsFunctions[5].u.i32 = (int32_t)SVGRenderingIntent::RENDERING_INTENT_ABSOLUTE_COLORIMETRIC;
}

JSValue JSSVGRenderingIntentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGRenderingIntent.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGRenderingIntentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGRenderingIntent.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGRenderingIntentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGRenderingIntentAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentAttributesFunctions, countof(JSSVGRenderingIntentAttributesFunctions));
    init_JSSVGRenderingIntentPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGRenderingIntentPrototypeConstantsFunctions, countof(JSSVGRenderingIntentPrototypeConstantsFunctions));
}

static JSClassDef JSSVGRenderingIntentClassDefine;
static bool JSSVGRenderingIntentClassDefine_initialized = false;

static void init_JSSVGRenderingIntentClassDefine()
{
    if (JSSVGRenderingIntentClassDefine_initialized) return;
    JSSVGRenderingIntentClassDefine_initialized = true;
    memset(&JSSVGRenderingIntentClassDefine, 0, sizeof(JSSVGRenderingIntentClassDefine));
    JSSVGRenderingIntentClassDefine.class_name = "SVGRenderingIntent";
    JSSVGRenderingIntentClassDefine.finalizer = JSSVGRenderingIntent::finalizer;
    JSSVGRenderingIntentClassDefine.gc_mark = JSSVGRenderingIntent::mark;
}

JSClassID JSSVGRenderingIntent::js_class_id = 0;

void JSSVGRenderingIntent::init(JSContext* ctx)
{
    if (JSSVGRenderingIntent::js_class_id == 0) {
        init_JSSVGRenderingIntentClassDefine();
        JS_NewClassID(&JSSVGRenderingIntent::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGRenderingIntent::js_class_id, &JSSVGRenderingIntentClassDefine);
        JS_SetConstructor(ctx, JSSVGRenderingIntentConstructor::self(ctx), JSSVGRenderingIntentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGRenderingIntent::js_class_id, JSSVGRenderingIntentPrototype::self(ctx));
    }
}

JSValue JSSVGRenderingIntent::create(JSContext* ctx, SVGRenderingIntent* impl, SVGElement* context)
{
    JSSVGRenderingIntent::init(ctx);
    JSValue _proto = JSSVGRenderingIntentPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGRenderingIntent::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
