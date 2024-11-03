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

#include "QJSHTMLBaseElement.h"

#include "HTMLBaseElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLBaseElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLBaseElement::getValueProperty, NULL, JSHTMLBaseElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("href", JSHTMLBaseElement::getValueProperty, JSHTMLBaseElement::putValueProperty, JSHTMLBaseElement::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSHTMLBaseElement::getValueProperty, JSHTMLBaseElement::putValueProperty, JSHTMLBaseElement::TargetAttrNum)
};

class JSHTMLBaseElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLBaseElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLBaseElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBaseElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBaseElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBaseElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBaseElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBaseElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBaseElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBaseElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBaseElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLBaseElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBaseElementAttributesFunctions, countof(JSHTMLBaseElementAttributesFunctions));
}

static JSClassDef JSHTMLBaseElementClassDefine = 
{
    "HTMLBaseElement",
    .finalizer = JSHTMLBaseElement::finalizer,
    .gc_mark = JSHTMLBaseElement::mark,
};

JSClassID JSHTMLBaseElement::js_class_id = 0;

void JSHTMLBaseElement::init(JSContext* ctx)
{
    if (JSHTMLBaseElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLBaseElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLBaseElement::js_class_id, &JSHTMLBaseElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLBaseElementConstructor::self(ctx), JSHTMLBaseElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLBaseElement::js_class_id, JSHTMLBaseElementPrototype::self(ctx));
    }
}

JSValue JSHTMLBaseElement::create(JSContext* ctx, HTMLBaseElement* impl)
{
    JSHTMLBaseElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLBaseElementPrototype::self(ctx), JSHTMLBaseElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBaseElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBaseElement* impl = (HTMLBaseElement*)JS_GetOpaque(val, JSHTMLBaseElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBaseElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBaseElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HrefAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->href()).utf8().data());
        }
        case TargetAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->target()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBaseElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case HrefAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseElement::js_class_id);
            imp->setHref(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TargetAttrNum: {
            HTMLBaseElement* imp = (HTMLBaseElement*)JS_GetOpaque2(ctx, this_val, JSHTMLBaseElement::js_class_id);
            imp->setTarget(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBaseElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBaseElementConstructor::self(ctx);
}


}
