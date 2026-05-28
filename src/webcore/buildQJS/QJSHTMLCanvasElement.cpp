/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
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

#include <string.h>

#include "QJSHTMLCanvasElement.h"

#include "HTMLCanvasElement.h"
#include "QJSCanvasRenderingContext2D.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLCanvasElementAttributesFunctions[3];
static bool JSHTMLCanvasElementAttributesFunctions_initialized = false;

static void init_JSHTMLCanvasElementAttributesFunctions()
{
    if (JSHTMLCanvasElementAttributesFunctions_initialized) return;
    JSHTMLCanvasElementAttributesFunctions_initialized = true;
    memset(JSHTMLCanvasElementAttributesFunctions, 0, sizeof(JSHTMLCanvasElementAttributesFunctions));
    JSHTMLCanvasElementAttributesFunctions[0].name = "constructor";
    JSHTMLCanvasElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLCanvasElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLCanvasElementAttributesFunctions[0].magic = JSHTMLCanvasElement::ConstructorAttrNum;
    JSHTMLCanvasElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLCanvasElement::getValueProperty;
    JSHTMLCanvasElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLCanvasElementAttributesFunctions[1].name = "width";
    JSHTMLCanvasElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLCanvasElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLCanvasElementAttributesFunctions[1].magic = JSHTMLCanvasElement::WidthAttrNum;
    JSHTMLCanvasElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLCanvasElement::getValueProperty;
    JSHTMLCanvasElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLCanvasElement::putValueProperty;
    JSHTMLCanvasElementAttributesFunctions[2].name = "height";
    JSHTMLCanvasElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLCanvasElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLCanvasElementAttributesFunctions[2].magic = JSHTMLCanvasElement::HeightAttrNum;
    JSHTMLCanvasElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLCanvasElement::getValueProperty;
    JSHTMLCanvasElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLCanvasElement::putValueProperty;
}

class JSHTMLCanvasElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLCanvasElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLCanvasElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCanvasElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLCanvasElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLCanvasElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLCanvasElementPrototypeFunctions[1];
static bool JSHTMLCanvasElementPrototypeFunctions_initialized = false;

static void init_JSHTMLCanvasElementPrototypeFunctions()
{
    if (JSHTMLCanvasElementPrototypeFunctions_initialized) return;
    JSHTMLCanvasElementPrototypeFunctions_initialized = true;
    memset(JSHTMLCanvasElementPrototypeFunctions, 0, sizeof(JSHTMLCanvasElementPrototypeFunctions));
    JSHTMLCanvasElementPrototypeFunctions[0].name = "getContext";
    JSHTMLCanvasElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLCanvasElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLCanvasElementPrototypeFunctions[0].magic = JSHTMLCanvasElement::GetContextFuncNum;
    JSHTMLCanvasElementPrototypeFunctions[0].u.func.length = 1;
    JSHTMLCanvasElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLCanvasElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLCanvasElementPrototypeFunction::callAsFunction;
}

JSValue JSHTMLCanvasElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLCanvasElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLCanvasElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLCanvasElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLCanvasElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLCanvasElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCanvasElementAttributesFunctions, countof(JSHTMLCanvasElementAttributesFunctions));
    init_JSHTMLCanvasElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCanvasElementPrototypeFunctions, countof(JSHTMLCanvasElementPrototypeFunctions));
}

static JSClassDef JSHTMLCanvasElementClassDefine;
static bool JSHTMLCanvasElementClassDefine_initialized = false;

static void init_JSHTMLCanvasElementClassDefine()
{
    if (JSHTMLCanvasElementClassDefine_initialized) return;
    JSHTMLCanvasElementClassDefine_initialized = true;
    memset(&JSHTMLCanvasElementClassDefine, 0, sizeof(JSHTMLCanvasElementClassDefine));
    JSHTMLCanvasElementClassDefine.class_name = "HTMLCanvasElement";
    JSHTMLCanvasElementClassDefine.finalizer = JSHTMLCanvasElement::finalizer;
    JSHTMLCanvasElementClassDefine.gc_mark = JSHTMLCanvasElement::mark;
}

JSClassID JSHTMLCanvasElement::js_class_id = 0;

void JSHTMLCanvasElement::init(JSContext* ctx)
{
    if (JSHTMLCanvasElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLCanvasElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLCanvasElement::create(JSContext* ctx, HTMLCanvasElement* impl)
{
    JSHTMLCanvasElement::init(ctx);
    JSValue _proto = JSHTMLCanvasElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLCanvasElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLCanvasElement* impl = (HTMLCanvasElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case HeightAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->height());
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
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToInt32(ctx, value));
            break;
        }
        case HeightAttrNum: {
            HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    HTMLCanvasElement* imp = (HTMLCanvasElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
