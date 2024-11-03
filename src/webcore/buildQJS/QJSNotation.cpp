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

#include "QJSNotation.h"

#include "Notation.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSNotationAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("publicId", JSNotation::getValueProperty, NULL, JSNotation::PublicIdAttrNum),
    JS_CGETSET_MAGIC_DEF("systemId", JSNotation::getValueProperty, NULL, JSNotation::SystemIdAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSNotation::getValueProperty, NULL, JSNotation::ConstructorAttrNum)
};

class JSNotationConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSNotationConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSNotationConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Notation.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNotationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Notation.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNotationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSNotationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNotation.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSNotationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNotation.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNotationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNotationAttributesFunctions, countof(JSNotationAttributesFunctions));
}

static JSClassDef JSNotationClassDefine = 
{
    "Notation",
    .finalizer = JSNotation::finalizer,
    .gc_mark = JSNotation::mark,
};

JSClassID JSNotation::js_class_id = 0;

void JSNotation::init(JSContext* ctx)
{
    if (JSNotation::js_class_id == 0) {
        JS_NewClassID(&JSNotation::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNotation::js_class_id, &JSNotationClassDefine);
        JS_SetConstructor(ctx, JSNotationConstructor::self(ctx), JSNotationPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNotation::js_class_id, JSNotationPrototype::self(ctx));
    }
}

JSValue JSNotation::create(JSContext* ctx, Notation* impl)
{
    JSNotation::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNotationPrototype::self(ctx), JSNotation::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNotation::finalizer(JSRuntime* rt, JSValue val)
{
    Notation* impl = (Notation*)JS_GetOpaque(val, JSNotation::js_class_id);
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
            Notation* imp = (Notation*)JS_GetOpaque2(ctx, this_val, JSNotation::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            Notation* imp = (Notation*)JS_GetOpaque2(ctx, this_val, JSNotation::js_class_id);
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
