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

#include "QJSHTMLMapElement.h"

#include "HTMLCollection.h"
#include "HTMLMapElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLMapElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("areas", JSHTMLMapElement::getValueProperty, NULL, JSHTMLMapElement::AreasAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSHTMLMapElement::getValueProperty, JSHTMLMapElement::putValueProperty, JSHTMLMapElement::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLMapElement::getValueProperty, NULL, JSHTMLMapElement::ConstructorAttrNum)
};

class JSHTMLMapElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLMapElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLMapElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMapElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMapElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMapElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMapElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLMapElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMapElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMapElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMapElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLMapElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMapElementAttributesFunctions, countof(JSHTMLMapElementAttributesFunctions));
}

static JSClassDef JSHTMLMapElementClassDefine = 
{
    "HTMLMapElement",
    .finalizer = JSHTMLMapElement::finalizer,
    .gc_mark = JSHTMLMapElement::mark,
};

JSClassID JSHTMLMapElement::js_class_id = 0;

void JSHTMLMapElement::init(JSContext* ctx)
{
    if (JSHTMLMapElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLMapElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLMapElement::js_class_id, &JSHTMLMapElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLMapElementConstructor::self(ctx), JSHTMLMapElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLMapElement::js_class_id, JSHTMLMapElementPrototype::self(ctx));
    }
}

JSValue JSHTMLMapElement::create(JSContext* ctx, HTMLMapElement* impl)
{
    JSHTMLMapElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLMapElementPrototype::self(ctx), JSHTMLMapElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMapElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMapElement* impl = (HTMLMapElement*)JS_GetOpaque(val, JSHTMLMapElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMapElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMapElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AreasAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMapElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->areas()));
        }
        case NameAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMapElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMapElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque2(ctx, this_val, JSHTMLMapElement::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLMapElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMapElementConstructor::self(ctx);
}


}
