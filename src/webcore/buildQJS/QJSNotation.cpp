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

#include "QJSNotation.h"

#include "Notation.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSNotationAttributesFunctions[3];
static bool JSNotationAttributesFunctions_initialized = false;

static void init_JSNotationAttributesFunctions()
{
    if (JSNotationAttributesFunctions_initialized) return;
    JSNotationAttributesFunctions_initialized = true;
    memset(JSNotationAttributesFunctions, 0, sizeof(JSNotationAttributesFunctions));
    JSNotationAttributesFunctions[0].name = "publicId";
    JSNotationAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNotationAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNotationAttributesFunctions[0].magic = JSNotation::PublicIdAttrNum;
    JSNotationAttributesFunctions[0].u.getset.get.getter_magic = JSNotation::getValueProperty;
    JSNotationAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSNotationAttributesFunctions[1].name = "systemId";
    JSNotationAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNotationAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNotationAttributesFunctions[1].magic = JSNotation::SystemIdAttrNum;
    JSNotationAttributesFunctions[1].u.getset.get.getter_magic = JSNotation::getValueProperty;
    JSNotationAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSNotationAttributesFunctions[2].name = "constructor";
    JSNotationAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNotationAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSNotationAttributesFunctions[2].magic = JSNotation::ConstructorAttrNum;
    JSNotationAttributesFunctions[2].u.getset.get.getter_magic = JSNotation::getValueProperty;
    JSNotationAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSNotationConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSNotationConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Notation.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNotationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Notation.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNotationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSNotationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNotation.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSNotationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNotation.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNotationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSNotationAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNotationAttributesFunctions, countof(JSNotationAttributesFunctions));
}

static JSClassDef JSNotationClassDefine;
static bool JSNotationClassDefine_initialized = false;

static void init_JSNotationClassDefine()
{
    if (JSNotationClassDefine_initialized) return;
    JSNotationClassDefine_initialized = true;
    memset(&JSNotationClassDefine, 0, sizeof(JSNotationClassDefine));
    JSNotationClassDefine.class_name = "Notation";
    JSNotationClassDefine.finalizer = JSNotation::finalizer;
    JSNotationClassDefine.gc_mark = JSNotation::mark;
}

JSClassID JSNotation::js_class_id = 0;

void JSNotation::init(JSContext* ctx)
{
    if (JSNotation::js_class_id == 0) {
        JSNode::init(ctx);
        JSNotation::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSNotation::create(JSContext* ctx, Notation* impl)
{
    JSNotation::init(ctx);
    JSValue _proto = JSNotationPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNotation::finalizer(JSRuntime* rt, JSValue val)
{
    Notation* impl = (Notation*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSNotation::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSNode::mark(rt, val, mark_func);
}

JSValue JSNotation::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PublicIdAttrNum: {
            Notation* imp = (Notation*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            Notation* imp = (Notation*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->systemId());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSNotation::getConstructor(JSContext *ctx)
{
    return JSNotationConstructor::self(ctx);
}


}
