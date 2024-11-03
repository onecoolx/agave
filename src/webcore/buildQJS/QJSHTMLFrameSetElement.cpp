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

#include "QJSHTMLFrameSetElement.h"

#include "HTMLFrameSetElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLFrameSetElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("cols", JSHTMLFrameSetElement::getValueProperty, JSHTMLFrameSetElement::putValueProperty, JSHTMLFrameSetElement::ColsAttrNum),
    JS_CGETSET_MAGIC_DEF("rows", JSHTMLFrameSetElement::getValueProperty, JSHTMLFrameSetElement::putValueProperty, JSHTMLFrameSetElement::RowsAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLFrameSetElement::getValueProperty, NULL, JSHTMLFrameSetElement::ConstructorAttrNum)
};

class JSHTMLFrameSetElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLFrameSetElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLFrameSetElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFrameSetElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFrameSetElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFrameSetElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFrameSetElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFrameSetElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFrameSetElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFrameSetElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFrameSetElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLFrameSetElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFrameSetElementAttributesFunctions, countof(JSHTMLFrameSetElementAttributesFunctions));
}

static JSClassDef JSHTMLFrameSetElementClassDefine = 
{
    "HTMLFrameSetElement",
    .finalizer = JSHTMLFrameSetElement::finalizer,
    .gc_mark = JSHTMLFrameSetElement::mark,
};

JSClassID JSHTMLFrameSetElement::js_class_id = 0;

void JSHTMLFrameSetElement::init(JSContext* ctx)
{
    if (JSHTMLFrameSetElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLFrameSetElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLFrameSetElement::js_class_id, &JSHTMLFrameSetElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLFrameSetElementConstructor::self(ctx), JSHTMLFrameSetElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLFrameSetElement::js_class_id, JSHTMLFrameSetElementPrototype::self(ctx));
    }
}

JSValue JSHTMLFrameSetElement::create(JSContext* ctx, HTMLFrameSetElement* impl)
{
    JSHTMLFrameSetElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLFrameSetElementPrototype::self(ctx), JSHTMLFrameSetElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFrameSetElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFrameSetElement* impl = (HTMLFrameSetElement*)JS_GetOpaque(val, JSHTMLFrameSetElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLFrameSetElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFrameSetElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColsAttrNum: {
            HTMLFrameSetElement* imp = (HTMLFrameSetElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameSetElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cols()).utf8().data());
        }
        case RowsAttrNum: {
            HTMLFrameSetElement* imp = (HTMLFrameSetElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameSetElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->rows()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFrameSetElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ColsAttrNum: {
            HTMLFrameSetElement* imp = (HTMLFrameSetElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameSetElement::js_class_id);
            imp->setCols(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case RowsAttrNum: {
            HTMLFrameSetElement* imp = (HTMLFrameSetElement*)JS_GetOpaque2(ctx, this_val, JSHTMLFrameSetElement::js_class_id);
            imp->setRows(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLFrameSetElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFrameSetElementConstructor::self(ctx);
}


}
