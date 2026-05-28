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

#include "QJSComment.h"

#include "Comment.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCommentAttributesFunctions[1];
static bool JSCommentAttributesFunctions_initialized = false;

static void init_JSCommentAttributesFunctions()
{
    if (JSCommentAttributesFunctions_initialized) return;
    JSCommentAttributesFunctions_initialized = true;
    memset(JSCommentAttributesFunctions, 0, sizeof(JSCommentAttributesFunctions));
    JSCommentAttributesFunctions[0].name = "constructor";
    JSCommentAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCommentAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCommentAttributesFunctions[0].magic = JSComment::ConstructorAttrNum;
    JSCommentAttributesFunctions[0].u.getset.get.getter_magic = JSComment::getValueProperty;
    JSCommentAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSCommentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSCommentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Comment.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCommentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Comment.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCommentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCommentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSComment.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSCharacterDataPrototype::self(ctx));
        JSCommentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSComment.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCommentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCommentAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCommentAttributesFunctions, countof(JSCommentAttributesFunctions));
}

JSClassID JSComment::js_class_id = 0;

void JSComment::init(JSContext* ctx)
{
    if (JSComment::js_class_id == 0) {
        JSNode::init(ctx);
        JSComment::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSComment::create(JSContext* ctx, Comment* impl)
{
    JSComment::init(ctx);
    JSValue _proto = JSCommentPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSComment::finalizer(JSRuntime* rt, JSValue val)
{
    Comment* impl = (Comment*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSComment::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCharacterData::mark(rt, val, mark_func);
}

JSValue JSComment::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSComment::getConstructor(JSContext *ctx)
{
    return JSCommentConstructor::self(ctx);
}


}
