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

#include "QJSHTMLDListElement.h"

#include "HTMLDListElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLDListElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("compact", JSHTMLDListElement::getValueProperty, JSHTMLDListElement::putValueProperty, JSHTMLDListElement::CompactAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLDListElement::getValueProperty, NULL, JSHTMLDListElement::ConstructorAttrNum)
};

class JSHTMLDListElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLDListElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLDListElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDListElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDListElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDListElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDListElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLDListElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDListElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLDListElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDListElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLDListElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDListElementAttributesFunctions, countof(JSHTMLDListElementAttributesFunctions));
}

static JSClassDef JSHTMLDListElementClassDefine = 
{
    "HTMLDListElement",
    .finalizer = JSHTMLDListElement::finalizer,
    .gc_mark = JSHTMLDListElement::mark,
};

JSClassID JSHTMLDListElement::js_class_id = 0;

void JSHTMLDListElement::init(JSContext* ctx)
{
    if (JSHTMLDListElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLDListElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLDListElement::js_class_id, &JSHTMLDListElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLDListElementConstructor::self(ctx), JSHTMLDListElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLDListElement::js_class_id, JSHTMLDListElementPrototype::self(ctx));
    }
}

JSValue JSHTMLDListElement::create(JSContext* ctx, HTMLDListElement* impl)
{
    JSHTMLDListElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLDListElementPrototype::self(ctx), JSHTMLDListElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDListElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDListElement* impl = (HTMLDListElement*)JS_GetOpaque(val, JSHTMLDListElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDListElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLDListElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDListElement* imp = (HTMLDListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLDListElement::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDListElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDListElement* imp = (HTMLDListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLDListElement::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDListElement::getConstructor(JSContext *ctx)
{
    return JSHTMLDListElementConstructor::self(ctx);
}


}
