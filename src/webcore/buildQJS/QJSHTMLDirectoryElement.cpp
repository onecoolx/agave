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

#include "QJSHTMLDirectoryElement.h"

#include "HTMLDirectoryElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLDirectoryElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("compact", JSHTMLDirectoryElement::getValueProperty, JSHTMLDirectoryElement::putValueProperty, JSHTMLDirectoryElement::CompactAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLDirectoryElement::getValueProperty, NULL, JSHTMLDirectoryElement::ConstructorAttrNum)
};

class JSHTMLDirectoryElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLDirectoryElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLDirectoryElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDirectoryElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDirectoryElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDirectoryElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDirectoryElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLDirectoryElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDirectoryElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLDirectoryElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDirectoryElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDirectoryElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDirectoryElementAttributesFunctions, countof(JSHTMLDirectoryElementAttributesFunctions));
}

static JSClassDef JSHTMLDirectoryElementClassDefine = 
{
    "HTMLDirectoryElement",
    .finalizer = JSHTMLDirectoryElement::finalizer,
    .gc_mark = JSHTMLDirectoryElement::mark,
};

JSClassID JSHTMLDirectoryElement::js_class_id = 0;

void JSHTMLDirectoryElement::init(JSContext* ctx)
{
    if (JSHTMLDirectoryElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLDirectoryElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLDirectoryElement::js_class_id, &JSHTMLDirectoryElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLDirectoryElementConstructor::self(ctx), JSHTMLDirectoryElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLDirectoryElement::js_class_id, JSHTMLDirectoryElementPrototype::self(ctx));
    }
}

JSValue JSHTMLDirectoryElement::create(JSContext* ctx, HTMLDirectoryElement* impl)
{
    JSHTMLDirectoryElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLDirectoryElementPrototype::self(ctx), JSHTMLDirectoryElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDirectoryElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDirectoryElement* impl = (HTMLDirectoryElement*)JS_GetOpaque(val, JSHTMLDirectoryElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDirectoryElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLDirectoryElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDirectoryElement* imp = (HTMLDirectoryElement*)JS_GetOpaque2(ctx, this_val, JSHTMLDirectoryElement::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDirectoryElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDirectoryElement* imp = (HTMLDirectoryElement*)JS_GetOpaque2(ctx, this_val, JSHTMLDirectoryElement::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDirectoryElement::getConstructor(JSContext *ctx)
{
    return JSHTMLDirectoryElementConstructor::self(ctx);
}


}
