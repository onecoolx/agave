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

#include "QJSDocumentFragment.h"

#include "DocumentFragment.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDocumentFragmentAttributesFunctions[1];
static bool JSDocumentFragmentAttributesFunctions_initialized = false;

static void init_JSDocumentFragmentAttributesFunctions()
{
    if (JSDocumentFragmentAttributesFunctions_initialized) return;
    JSDocumentFragmentAttributesFunctions_initialized = true;
    memset(JSDocumentFragmentAttributesFunctions, 0, sizeof(JSDocumentFragmentAttributesFunctions));
    JSDocumentFragmentAttributesFunctions[0].name = "constructor";
    JSDocumentFragmentAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentFragmentAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentFragmentAttributesFunctions[0].magic = JSDocumentFragment::ConstructorAttrNum;
    JSDocumentFragmentAttributesFunctions[0].u.getset.get.getter_magic = JSDocumentFragment::getValueProperty;
    JSDocumentFragmentAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSDocumentFragmentConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSDocumentFragmentConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[DocumentFragment.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentFragmentConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DocumentFragment.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentFragmentConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSDocumentFragmentPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocumentFragment.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSDocumentFragmentPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocumentFragment.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentFragmentPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDocumentFragmentAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentFragmentAttributesFunctions, countof(JSDocumentFragmentAttributesFunctions));
}

JSClassID JSDocumentFragment::js_class_id = 0;

void JSDocumentFragment::init(JSContext* ctx)
{
    if (JSDocumentFragment::js_class_id == 0) {
        JSNode::init(ctx);
        JSDocumentFragment::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSDocumentFragment::create(JSContext* ctx, DocumentFragment* impl)
{
    JSDocumentFragment::init(ctx);
    JSValue _proto = JSDocumentFragmentPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocumentFragment::finalizer(JSRuntime* rt, JSValue val)
{
    DocumentFragment* impl = (DocumentFragment*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
