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

#include "QJSComment.h"

#include "Comment.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCommentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSComment::getValueProperty, NULL, JSComment::ConstructorAttrNum)
};

class JSCommentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCommentConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCommentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Comment.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCommentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Comment.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCommentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSCommentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSComment.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCharacterDataPrototype::self(ctx));
        JSCommentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSComment.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCommentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCommentAttributesFunctions, countof(JSCommentAttributesFunctions));
}

static JSClassDef JSCommentClassDefine = 
{
    "Comment",
    .finalizer = JSComment::finalizer,
    .gc_mark = JSComment::mark,
};

JSClassID JSComment::js_class_id = 0;

void JSComment::init(JSContext* ctx)
{
    if (JSComment::js_class_id == 0) {
        JS_NewClassID(&JSComment::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSComment::js_class_id, &JSCommentClassDefine);
        JS_SetConstructor(ctx, JSCommentConstructor::self(ctx), JSCommentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSComment::js_class_id, JSCommentPrototype::self(ctx));
    }
}

JSValue JSComment::create(JSContext* ctx, Comment* impl)
{
    JSComment::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCommentPrototype::self(ctx), JSComment::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSComment::finalizer(JSRuntime* rt, JSValue val)
{
    Comment* impl = (Comment*)JS_GetOpaque(val, JSComment::js_class_id);
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
