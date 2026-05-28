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

#include "QJSEntityReference.h"

#include "EntityReference.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSEntityReferenceAttributesFunctions[1];
static bool JSEntityReferenceAttributesFunctions_initialized = false;

static void init_JSEntityReferenceAttributesFunctions()
{
    if (JSEntityReferenceAttributesFunctions_initialized) return;
    JSEntityReferenceAttributesFunctions_initialized = true;
    memset(JSEntityReferenceAttributesFunctions, 0, sizeof(JSEntityReferenceAttributesFunctions));
    JSEntityReferenceAttributesFunctions[0].name = "constructor";
    JSEntityReferenceAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSEntityReferenceAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSEntityReferenceAttributesFunctions[0].magic = JSEntityReference::ConstructorAttrNum;
    JSEntityReferenceAttributesFunctions[0].u.getset.get.getter_magic = JSEntityReference::getValueProperty;
    JSEntityReferenceAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSEntityReferenceConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSEntityReferenceConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[EntityReference.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSEntityReferenceConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[EntityReference.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEntityReferenceConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSEntityReferencePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSEntityReference.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSEntityReferencePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSEntityReference.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEntityReferencePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSEntityReferenceAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEntityReferenceAttributesFunctions, countof(JSEntityReferenceAttributesFunctions));
}

JSClassID JSEntityReference::js_class_id = 0;

void JSEntityReference::init(JSContext* ctx)
{
    if (JSEntityReference::js_class_id == 0) {
        JSNode::init(ctx);
        JSEntityReference::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSEntityReference::create(JSContext* ctx, EntityReference* impl)
{
    JSEntityReference::init(ctx);
    JSValue _proto = JSEntityReferencePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSEntityReference::finalizer(JSRuntime* rt, JSValue val)
{
    EntityReference* impl = (EntityReference*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSEntityReference::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSNode::mark(rt, val, mark_func);
}

JSValue JSEntityReference::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSEntityReference::getConstructor(JSContext *ctx)
{
    return JSEntityReferenceConstructor::self(ctx);
}


}
