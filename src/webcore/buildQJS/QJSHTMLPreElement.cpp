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

#include "QJSHTMLPreElement.h"

#include "HTMLPreElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLPreElementAttributesFunctions[3];
static bool JSHTMLPreElementAttributesFunctions_initialized = false;

static void init_JSHTMLPreElementAttributesFunctions()
{
    if (JSHTMLPreElementAttributesFunctions_initialized) return;
    JSHTMLPreElementAttributesFunctions_initialized = true;
    memset(JSHTMLPreElementAttributesFunctions, 0, sizeof(JSHTMLPreElementAttributesFunctions));
    JSHTMLPreElementAttributesFunctions[0].name = "wrap";
    JSHTMLPreElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLPreElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLPreElementAttributesFunctions[0].magic = JSHTMLPreElement::WrapAttrNum;
    JSHTMLPreElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLPreElement::getValueProperty;
    JSHTMLPreElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLPreElement::putValueProperty;
    JSHTMLPreElementAttributesFunctions[1].name = "width";
    JSHTMLPreElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLPreElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLPreElementAttributesFunctions[1].magic = JSHTMLPreElement::WidthAttrNum;
    JSHTMLPreElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLPreElement::getValueProperty;
    JSHTMLPreElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLPreElement::putValueProperty;
    JSHTMLPreElementAttributesFunctions[2].name = "constructor";
    JSHTMLPreElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLPreElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLPreElementAttributesFunctions[2].magic = JSHTMLPreElement::ConstructorAttrNum;
    JSHTMLPreElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLPreElement::getValueProperty;
    JSHTMLPreElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSHTMLPreElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLPreElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLPreElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLPreElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLPreElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLPreElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLPreElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLPreElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLPreElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLPreElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLPreElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLPreElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLPreElementAttributesFunctions, countof(JSHTMLPreElementAttributesFunctions));
}

static JSClassDef JSHTMLPreElementClassDefine;
static bool JSHTMLPreElementClassDefine_initialized = false;

static void init_JSHTMLPreElementClassDefine()
{
    if (JSHTMLPreElementClassDefine_initialized) return;
    JSHTMLPreElementClassDefine_initialized = true;
    memset(&JSHTMLPreElementClassDefine, 0, sizeof(JSHTMLPreElementClassDefine));
    JSHTMLPreElementClassDefine.class_name = "HTMLPreElement";
    JSHTMLPreElementClassDefine.finalizer = JSHTMLPreElement::finalizer;
    JSHTMLPreElementClassDefine.gc_mark = JSHTMLPreElement::mark;
}

JSClassID JSHTMLPreElement::js_class_id = 0;

void JSHTMLPreElement::init(JSContext* ctx)
{
    if (JSHTMLPreElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLPreElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLPreElement::create(JSContext* ctx, HTMLPreElement* impl)
{
    JSHTMLPreElement::init(ctx);
    JSValue _proto = JSHTMLPreElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLPreElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLPreElement* impl = (HTMLPreElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLPreElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLPreElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case WrapAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->wrap() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLPreElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case WidthAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToInt32(ctx, value));
            break;
        }
        case WrapAttrNum: {
            HTMLPreElement* imp = (HTMLPreElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWrap(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLPreElement::getConstructor(JSContext *ctx)
{
    return JSHTMLPreElementConstructor::self(ctx);
}


}
