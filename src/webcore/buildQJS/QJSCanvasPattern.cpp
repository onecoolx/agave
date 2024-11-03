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

#include "QJSCanvasPattern.h"

#include "CanvasPattern.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
JSValue JSCanvasPatternPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCanvasPattern.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCanvasPatternPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCanvasPattern.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCanvasPatternPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
}

static JSClassDef JSCanvasPatternClassDefine = 
{
    "CanvasPattern",
    .finalizer = JSCanvasPattern::finalizer,
    .gc_mark = JSCanvasPattern::mark,
};

JSClassID JSCanvasPattern::js_class_id = 0;

void JSCanvasPattern::init(JSContext* ctx)
{
    if (JSCanvasPattern::js_class_id == 0) {
        JS_NewClassID(&JSCanvasPattern::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCanvasPattern::js_class_id, &JSCanvasPatternClassDefine);
        JS_SetClassProto(ctx, JSCanvasPattern::js_class_id, JSCanvasPatternPrototype::self(ctx));
    }
}

JSValue JSCanvasPattern::create(JSContext* ctx, CanvasPattern* impl)
{
    JSCanvasPattern::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCanvasPatternPrototype::self(ctx), JSCanvasPattern::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCanvasPattern::finalizer(JSRuntime* rt, JSValue val)
{
    CanvasPattern* impl = (CanvasPattern*)JS_GetOpaque(val, JSCanvasPattern::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCanvasPattern::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue toJS(JSContext *ctx, CanvasPattern* obj)
{
    return QJS::cacheDOMObject<CanvasPattern, JSCanvasPattern>(ctx, obj);
}

CanvasPattern* toCanvasPattern(JSValue val)
{
    if (JS_IsObject(val)) {
        CanvasPattern* impl = (CanvasPattern*)JS_GetOpaque(val, JSCanvasPattern::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
