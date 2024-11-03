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

#include "QJSHTMLUListElement.h"

#include "HTMLUListElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLUListElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("compact", JSHTMLUListElement::getValueProperty, JSHTMLUListElement::putValueProperty, JSHTMLUListElement::CompactAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSHTMLUListElement::getValueProperty, JSHTMLUListElement::putValueProperty, JSHTMLUListElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLUListElement::getValueProperty, NULL, JSHTMLUListElement::ConstructorAttrNum)
};

class JSHTMLUListElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLUListElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLUListElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLUListElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLUListElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLUListElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLUListElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLUListElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLUListElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLUListElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLUListElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLUListElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLUListElementAttributesFunctions, countof(JSHTMLUListElementAttributesFunctions));
}

static JSClassDef JSHTMLUListElementClassDefine = 
{
    "HTMLUListElement",
    .finalizer = JSHTMLUListElement::finalizer,
    .gc_mark = JSHTMLUListElement::mark,
};

JSClassID JSHTMLUListElement::js_class_id = 0;

void JSHTMLUListElement::init(JSContext* ctx)
{
    if (JSHTMLUListElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLUListElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLUListElement::js_class_id, &JSHTMLUListElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLUListElementConstructor::self(ctx), JSHTMLUListElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLUListElement::js_class_id, JSHTMLUListElementPrototype::self(ctx));
    }
}

JSValue JSHTMLUListElement::create(JSContext* ctx, HTMLUListElement* impl)
{
    JSHTMLUListElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLUListElementPrototype::self(ctx), JSHTMLUListElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLUListElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLUListElement* impl = (HTMLUListElement*)JS_GetOpaque(val, JSHTMLUListElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLUListElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLUListElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLUListElement* imp = (HTMLUListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLUListElement::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case TypeAttrNum: {
            HTMLUListElement* imp = (HTMLUListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLUListElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLUListElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLUListElement* imp = (HTMLUListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLUListElement::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLUListElement* imp = (HTMLUListElement*)JS_GetOpaque2(ctx, this_val, JSHTMLUListElement::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLUListElement::getConstructor(JSContext *ctx)
{
    return JSHTMLUListElementConstructor::self(ctx);
}


}
