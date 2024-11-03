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

#include "QJSHTMLCanvasElement.h"

#include "HTMLCanvasElement.h"
#include "QJSCanvasRenderingContext2D.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLCanvasElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLCanvasElement::getValueProperty, NULL, JSHTMLCanvasElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLCanvasElement::getValueProperty, JSHTMLCanvasElement::putValueProperty, JSHTMLCanvasElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("height", JSHTMLCanvasElement::getValueProperty, JSHTMLCanvasElement::putValueProperty, JSHTMLCanvasElement::HeightAttrNum)
};

class JSHTMLCanvasElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLCanvasElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLCanvasElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLCanvasElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCanvasElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLCanvasElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLCanvasElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLCanvasElementPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getContext", 1, JSHTMLCanvasElementPrototypeFunction::callAsFunction, JSHTMLCanvasElement::GetContextFuncNum)
};

JSValue JSHTMLCanvasElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLCanvasElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLCanvasElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLCanvasElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLCanvasElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCanvasElementAttributesFunctions, countof(JSHTMLCanvasElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCanvasElementPrototypeFunctions, countof(JSHTMLCanvasElementPrototypeFunctions));
}

static JSClassDef JSHTMLCanvasElementClassDefine = 
{
    "HTMLCanvasElement",
    .finalizer = JSHTMLCanvasElement::finalizer,
    .gc_mark = JSHTMLCanvasElement::mark,
};

JSClassID JSHTMLCanvasElement::js_class_id = 0;

void JSHTMLCanvasElement::init(JSContext* ctx)
{
    if (JSHTMLCanvasElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLCanvasElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLCanvasElement::js_class_id, &JSHTMLCanvasElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLCanvasElementConstructor::self(ctx), JSHTMLCanvasElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLCanvasElement::js_class_id, JSHTMLCanvasElementPrototype::self(ctx));
    }
}

JSValue JSHTMLCanvasElement::create(JSContext* ctx, HTMLCanvasElement* impl)
{
    JSHTMLCanvasElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLCanvasElementPrototype::self(ctx), JSHTMLCanvasElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLCanvasElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLCanvasElement* impl = (HTMLCanvasElement*)JS_GetOpaque(val, JSHTMLCanvasElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLCanvasElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLCanvasElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque2(ctx, this_val, JSHTMLCanvasElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque2(ctx, this_val, JSHTMLCanvasElement::js_class_id);
            return JS_NewBigUint64(ctx, imp->height());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLCanvasElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque2(ctx, this_val, JSHTMLCanvasElement::js_class_id);
            imp->setWidth(valueToInt32(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque2(ctx, this_val, JSHTMLCanvasElement::js_class_id);
            imp->setHeight(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLCanvasElement::getConstructor(JSContext *ctx)
{
    return JSHTMLCanvasElementConstructor::self(ctx);
}

JSValue JSHTMLCanvasElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque2(ctx, this_val, JSHTMLCanvasElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLCanvasElement::GetContextFuncNum: {
            String contextId = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getContext(contextId)));
            return result;
        }
    }
    return JS_NULL;
}


}
