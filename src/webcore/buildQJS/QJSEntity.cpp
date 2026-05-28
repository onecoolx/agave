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

#include "QJSEntity.h"

#include "Entity.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSEntityAttributesFunctions[4];
static bool JSEntityAttributesFunctions_initialized = false;

static void init_JSEntityAttributesFunctions()
{
    if (JSEntityAttributesFunctions_initialized) return;
    JSEntityAttributesFunctions_initialized = true;
    memset(JSEntityAttributesFunctions, 0, sizeof(JSEntityAttributesFunctions));
    JSEntityAttributesFunctions[0].name = "publicId";
    JSEntityAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSEntityAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSEntityAttributesFunctions[0].magic = JSEntity::PublicIdAttrNum;
    JSEntityAttributesFunctions[0].u.getset.get.getter_magic = JSEntity::getValueProperty;
    JSEntityAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSEntityAttributesFunctions[1].name = "systemId";
    JSEntityAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSEntityAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSEntityAttributesFunctions[1].magic = JSEntity::SystemIdAttrNum;
    JSEntityAttributesFunctions[1].u.getset.get.getter_magic = JSEntity::getValueProperty;
    JSEntityAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSEntityAttributesFunctions[2].name = "notationName";
    JSEntityAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSEntityAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSEntityAttributesFunctions[2].magic = JSEntity::NotationNameAttrNum;
    JSEntityAttributesFunctions[2].u.getset.get.getter_magic = JSEntity::getValueProperty;
    JSEntityAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSEntityAttributesFunctions[3].name = "constructor";
    JSEntityAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSEntityAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSEntityAttributesFunctions[3].magic = JSEntity::ConstructorAttrNum;
    JSEntityAttributesFunctions[3].u.getset.get.getter_magic = JSEntity::getValueProperty;
    JSEntityAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSEntityConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSEntityConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Entity.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSEntityConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Entity.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEntityConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSEntityPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSEntity.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSEntityPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSEntity.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSEntityPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSEntityAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSEntityAttributesFunctions, countof(JSEntityAttributesFunctions));
}

static JSClassDef JSEntityClassDefine;
static bool JSEntityClassDefine_initialized = false;

static void init_JSEntityClassDefine()
{
    if (JSEntityClassDefine_initialized) return;
    JSEntityClassDefine_initialized = true;
    memset(&JSEntityClassDefine, 0, sizeof(JSEntityClassDefine));
    JSEntityClassDefine.class_name = "Entity";
    JSEntityClassDefine.finalizer = JSEntity::finalizer;
    JSEntityClassDefine.gc_mark = JSEntity::mark;
}

JSClassID JSEntity::js_class_id = 0;

void JSEntity::init(JSContext* ctx)
{
    if (JSEntity::js_class_id == 0) {
        JSNode::init(ctx);
        JSEntity::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSEntity::create(JSContext* ctx, Entity* impl)
{
    JSEntity::init(ctx);
    JSValue _proto = JSEntityPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSEntity::finalizer(JSRuntime* rt, JSValue val)
{
    Entity* impl = (Entity*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            Entity* imp = (Entity*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            Entity* imp = (Entity*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->systemId());
        }
        case NotationNameAttrNum: {
            Entity* imp = (Entity*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
