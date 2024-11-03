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

#include "QJSHTMLTitleElement.h"

#include "HTMLTitleElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLTitleElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("text", JSHTMLTitleElement::getValueProperty, JSHTMLTitleElement::putValueProperty, JSHTMLTitleElement::TextAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLTitleElement::getValueProperty, NULL, JSHTMLTitleElement::ConstructorAttrNum)
};

class JSHTMLTitleElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLTitleElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLTitleElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLTitleElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLTitleElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLTitleElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTitleElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLTitleElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLTitleElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLTitleElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLTitleElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLTitleElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLTitleElementAttributesFunctions, countof(JSHTMLTitleElementAttributesFunctions));
}

static JSClassDef JSHTMLTitleElementClassDefine = 
{
    "HTMLTitleElement",
    .finalizer = JSHTMLTitleElement::finalizer,
    .gc_mark = JSHTMLTitleElement::mark,
};

JSClassID JSHTMLTitleElement::js_class_id = 0;

void JSHTMLTitleElement::init(JSContext* ctx)
{
    if (JSHTMLTitleElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLTitleElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLTitleElement::js_class_id, &JSHTMLTitleElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLTitleElementConstructor::self(ctx), JSHTMLTitleElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLTitleElement::js_class_id, JSHTMLTitleElementPrototype::self(ctx));
    }
}

JSValue JSHTMLTitleElement::create(JSContext* ctx, HTMLTitleElement* impl)
{
    JSHTMLTitleElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLTitleElementPrototype::self(ctx), JSHTMLTitleElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLTitleElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLTitleElement* impl = (HTMLTitleElement*)JS_GetOpaque(val, JSHTMLTitleElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLTitleElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLTitleElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLTitleElement* imp = (HTMLTitleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTitleElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->text()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLTitleElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case TextAttrNum: {
            HTMLTitleElement* imp = (HTMLTitleElement*)JS_GetOpaque2(ctx, this_val, JSHTMLTitleElement::js_class_id);
            imp->setText(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLTitleElement::getConstructor(JSContext *ctx)
{
    return JSHTMLTitleElementConstructor::self(ctx);
}


}
