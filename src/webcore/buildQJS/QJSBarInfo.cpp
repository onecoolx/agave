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

#include "QJSBarInfo.h"

#include "BarInfo.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSBarInfoAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("visible", JSBarInfo::getValueProperty, NULL, JSBarInfo::VisibleAttrNum)
};

JSValue JSBarInfoPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSBarInfo.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSBarInfoPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSBarInfo.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSBarInfoPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSBarInfoAttributesFunctions, countof(JSBarInfoAttributesFunctions));
}

static JSClassDef JSBarInfoClassDefine = 
{
    "BarInfo",
    .finalizer = JSBarInfo::finalizer,
    .gc_mark = JSBarInfo::mark,
};

JSClassID JSBarInfo::js_class_id = 0;

void JSBarInfo::init(JSContext* ctx)
{
    if (JSBarInfo::js_class_id == 0) {
        JS_NewClassID(&JSBarInfo::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSBarInfo::js_class_id, &JSBarInfoClassDefine);
        JS_SetClassProto(ctx, JSBarInfo::js_class_id, JSBarInfoPrototype::self(ctx));
    }
}

JSValue JSBarInfo::create(JSContext* ctx, BarInfo* impl)
{
    JSBarInfo::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSBarInfoPrototype::self(ctx), JSBarInfo::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSBarInfo::finalizer(JSRuntime* rt, JSValue val)
{
    BarInfo* impl = (BarInfo*)JS_GetOpaque(val, JSBarInfo::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSBarInfo::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSBarInfo::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case VisibleAttrNum: {
            BarInfo* imp = (BarInfo*)JS_GetOpaque2(ctx, this_val, JSBarInfo::js_class_id);
            return JS_NewBool(ctx, imp->visible() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, BarInfo* obj)
{
    return QJS::cacheDOMObject<BarInfo, JSBarInfo>(ctx, obj);
}

BarInfo* toBarInfo(JSValue val)
{
    if (JS_IsObject(val)) {
        BarInfo* impl = (BarInfo*)JS_GetOpaque(val, JSBarInfo::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
