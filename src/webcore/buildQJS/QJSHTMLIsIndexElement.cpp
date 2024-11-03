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

#include "QJSHTMLIsIndexElement.h"

#include "HTMLFormElement.h"
#include "HTMLIsIndexElement.h"
#include "QJSHTMLFormElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLIsIndexElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLIsIndexElement::getValueProperty, NULL, JSHTMLIsIndexElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("form", JSHTMLIsIndexElement::getValueProperty, NULL, JSHTMLIsIndexElement::FormAttrNum),
    JS_CGETSET_MAGIC_DEF("prompt", JSHTMLIsIndexElement::getValueProperty, JSHTMLIsIndexElement::putValueProperty, JSHTMLIsIndexElement::PromptAttrNum)
};

class JSHTMLIsIndexElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLIsIndexElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLIsIndexElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLIsIndexElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLIsIndexElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLIsIndexElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLIsIndexElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLIsIndexElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLIsIndexElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLInputElementPrototype::self(ctx));
        JSHTMLIsIndexElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLIsIndexElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLIsIndexElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLIsIndexElementAttributesFunctions, countof(JSHTMLIsIndexElementAttributesFunctions));
}

static JSClassDef JSHTMLIsIndexElementClassDefine = 
{
    "HTMLIsIndexElement",
    .finalizer = JSHTMLIsIndexElement::finalizer,
    .gc_mark = JSHTMLIsIndexElement::mark,
};

JSClassID JSHTMLIsIndexElement::js_class_id = 0;

void JSHTMLIsIndexElement::init(JSContext* ctx)
{
    if (JSHTMLIsIndexElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLIsIndexElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLIsIndexElement::js_class_id, &JSHTMLIsIndexElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLIsIndexElementConstructor::self(ctx), JSHTMLIsIndexElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLIsIndexElement::js_class_id, JSHTMLIsIndexElementPrototype::self(ctx));
    }
}

JSValue JSHTMLIsIndexElement::create(JSContext* ctx, HTMLIsIndexElement* impl)
{
    JSHTMLIsIndexElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLIsIndexElementPrototype::self(ctx), JSHTMLIsIndexElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLIsIndexElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLIsIndexElement* impl = (HTMLIsIndexElement*)JS_GetOpaque(val, JSHTMLIsIndexElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLIsIndexElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLInputElement::mark(rt, val, mark_func);
}

JSValue JSHTMLIsIndexElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case FormAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIsIndexElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->form()));
        }
        case PromptAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIsIndexElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->prompt()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLIsIndexElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case PromptAttrNum: {
            HTMLIsIndexElement* imp = (HTMLIsIndexElement*)JS_GetOpaque2(ctx, this_val, JSHTMLIsIndexElement::js_class_id);
            imp->setPrompt(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLIsIndexElement::getConstructor(JSContext *ctx)
{
    return JSHTMLIsIndexElementConstructor::self(ctx);
}


}
