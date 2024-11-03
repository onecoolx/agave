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

#include "QJSEntity.h"

#include "Entity.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSEntityAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("notationName", JSEntity::getValueProperty, NULL, JSEntity::NotationNameAttrNum),
    JS_CGETSET_MAGIC_DEF("publicId", JSEntity::getValueProperty, NULL, JSEntity::PublicIdAttrNum),
    JS_CGETSET_MAGIC_DEF("systemId", JSEntity::getValueProperty, NULL, JSEntity::SystemIdAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSEntity::getValueProperty, NULL, JSEntity::ConstructorAttrNum)
};

class JSEntityConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSEntityConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSEntityConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Entity.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSEntityConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Entity.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSEntityConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSEntityPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSEntity.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSEntityPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSEntity.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSEntityPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSEntityAttributesFunctions, countof(JSEntityAttributesFunctions));
}

static JSClassDef JSEntityClassDefine = 
{
    "Entity",
    .finalizer = JSEntity::finalizer,
    .gc_mark = JSEntity::mark,
};

JSClassID JSEntity::js_class_id = 0;

void JSEntity::init(JSContext* ctx)
{
    if (JSEntity::js_class_id == 0) {
        JS_NewClassID(&JSEntity::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSEntity::js_class_id, &JSEntityClassDefine);
        JS_SetConstructor(ctx, JSEntityConstructor::self(ctx), JSEntityPrototype::self(ctx));
        JS_SetClassProto(ctx, JSEntity::js_class_id, JSEntityPrototype::self(ctx));
    }
}

JSValue JSEntity::create(JSContext* ctx, Entity* impl)
{
    JSEntity::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSEntityPrototype::self(ctx), JSEntity::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSEntity::finalizer(JSRuntime* rt, JSValue val)
{
    Entity* impl = (Entity*)JS_GetOpaque(val, JSEntity::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSEntity::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSNode::mark(rt, val, mark_func);
}

JSValue JSEntity::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PublicIdAttrNum: {
            Entity* imp = (Entity*)JS_GetOpaque2(ctx, this_val, JSEntity::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            Entity* imp = (Entity*)JS_GetOpaque2(ctx, this_val, JSEntity::js_class_id);
            return jsStringOrNull(ctx, imp->systemId());
        }
        case NotationNameAttrNum: {
            Entity* imp = (Entity*)JS_GetOpaque2(ctx, this_val, JSEntity::js_class_id);
            return jsStringOrNull(ctx, imp->notationName());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSEntity::getConstructor(JSContext *ctx)
{
    return JSEntityConstructor::self(ctx);
}


}
