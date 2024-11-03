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

#include "QJSDocumentFragment.h"

#include "DocumentFragment.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSDocumentFragmentAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSDocumentFragment::getValueProperty, NULL, JSDocumentFragment::ConstructorAttrNum)
};

class JSDocumentFragmentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSDocumentFragmentConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSDocumentFragmentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[DocumentFragment.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentFragmentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DocumentFragment.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentFragmentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSDocumentFragmentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocumentFragment.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSDocumentFragmentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocumentFragment.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentFragmentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentFragmentAttributesFunctions, countof(JSDocumentFragmentAttributesFunctions));
}

static JSClassDef JSDocumentFragmentClassDefine = 
{
    "DocumentFragment",
    .finalizer = JSDocumentFragment::finalizer,
    .gc_mark = JSDocumentFragment::mark,
};

JSClassID JSDocumentFragment::js_class_id = 0;

void JSDocumentFragment::init(JSContext* ctx)
{
    if (JSDocumentFragment::js_class_id == 0) {
        JS_NewClassID(&JSDocumentFragment::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDocumentFragment::js_class_id, &JSDocumentFragmentClassDefine);
        JS_SetConstructor(ctx, JSDocumentFragmentConstructor::self(ctx), JSDocumentFragmentPrototype::self(ctx));
        JS_SetClassProto(ctx, JSDocumentFragment::js_class_id, JSDocumentFragmentPrototype::self(ctx));
    }
}

JSValue JSDocumentFragment::create(JSContext* ctx, DocumentFragment* impl)
{
    JSDocumentFragment::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDocumentFragmentPrototype::self(ctx), JSDocumentFragment::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocumentFragment::finalizer(JSRuntime* rt, JSValue val)
{
    DocumentFragment* impl = (DocumentFragment*)JS_GetOpaque(val, JSDocumentFragment::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDocumentFragment::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEventTargetNode::mark(rt, val, mark_func);
}

JSValue JSDocumentFragment::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSDocumentFragment::getConstructor(JSContext *ctx)
{
    return JSDocumentFragmentConstructor::self(ctx);
}


}
