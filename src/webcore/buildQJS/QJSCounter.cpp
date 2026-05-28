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

#include "QJSCounter.h"

#include "Counter.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCounterAttributesFunctions[4];
static bool JSCounterAttributesFunctions_initialized = false;

static void init_JSCounterAttributesFunctions()
{
    if (JSCounterAttributesFunctions_initialized) return;
    JSCounterAttributesFunctions_initialized = true;
    memset(JSCounterAttributesFunctions, 0, sizeof(JSCounterAttributesFunctions));
    JSCounterAttributesFunctions[0].name = "identifier";
    JSCounterAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCounterAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCounterAttributesFunctions[0].magic = JSCounter::IdentifierAttrNum;
    JSCounterAttributesFunctions[0].u.getset.get.getter_magic = JSCounter::getValueProperty;
    JSCounterAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSCounterAttributesFunctions[1].name = "constructor";
    JSCounterAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCounterAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCounterAttributesFunctions[1].magic = JSCounter::ConstructorAttrNum;
    JSCounterAttributesFunctions[1].u.getset.get.getter_magic = JSCounter::getValueProperty;
    JSCounterAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCounterAttributesFunctions[2].name = "separator";
    JSCounterAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCounterAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCounterAttributesFunctions[2].magic = JSCounter::SeparatorAttrNum;
    JSCounterAttributesFunctions[2].u.getset.get.getter_magic = JSCounter::getValueProperty;
    JSCounterAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSCounterAttributesFunctions[3].name = "listStyle";
    JSCounterAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSCounterAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSCounterAttributesFunctions[3].magic = JSCounter::ListStyleAttrNum;
    JSCounterAttributesFunctions[3].u.getset.get.getter_magic = JSCounter::getValueProperty;
    JSCounterAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSCounterConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCounterConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCounterConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Counter.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCounterConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Counter.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCounterConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCounterPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCounter.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCounterPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCounter.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCounterPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCounterAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCounterAttributesFunctions, countof(JSCounterAttributesFunctions));
}

static JSClassDef JSCounterClassDefine;
static bool JSCounterClassDefine_initialized = false;

static void init_JSCounterClassDefine()
{
    if (JSCounterClassDefine_initialized) return;
    JSCounterClassDefine_initialized = true;
    memset(&JSCounterClassDefine, 0, sizeof(JSCounterClassDefine));
    JSCounterClassDefine.class_name = "Counter";
    JSCounterClassDefine.finalizer = JSCounter::finalizer;
    JSCounterClassDefine.gc_mark = JSCounter::mark;
}

JSClassID JSCounter::js_class_id = 0;

void JSCounter::init(JSContext* ctx)
{
    if (JSCounter::js_class_id == 0) {
        init_JSCounterClassDefine();
        JS_NewClassID(&JSCounter::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCounter::js_class_id, &JSCounterClassDefine);
        JS_SetConstructor(ctx, JSCounterConstructor::self(ctx), JSCounterPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCounter::js_class_id, JSCounterPrototype::self(ctx));
    }
}

JSValue JSCounter::create(JSContext* ctx, Counter* impl)
{
    JSCounter::init(ctx);
    JSValue _proto = JSCounterPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSCounter::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCounter::finalizer(JSRuntime* rt, JSValue val)
{
    Counter* impl = (Counter*)JS_GetOpaque(val, JSCounter::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCounter::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSCounter::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case IdentifierAttrNum: {
            Counter* imp = (Counter*)JS_GetOpaque(this_val, JSCounter::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->identifier()).utf8().data());
        }
        case ListStyleAttrNum: {
            Counter* imp = (Counter*)JS_GetOpaque(this_val, JSCounter::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->listStyle()).utf8().data());
        }
        case SeparatorAttrNum: {
            Counter* imp = (Counter*)JS_GetOpaque(this_val, JSCounter::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->separator()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCounter::getConstructor(JSContext *ctx)
{
    return JSCounterConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, Counter* obj)
{
    return QJS::cacheDOMObject<Counter, JSCounter>(ctx, obj);
}

Counter* toCounter(JSValue val)
{
    if (JS_IsObject(val)) {
        Counter* impl = (Counter*)JS_GetOpaque(val, JSCounter::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
