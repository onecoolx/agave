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

#include "QJSHTMLDirectoryElement.h"

#include "HTMLDirectoryElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLDirectoryElementAttributesFunctions[2];
static bool JSHTMLDirectoryElementAttributesFunctions_initialized = false;

static void init_JSHTMLDirectoryElementAttributesFunctions()
{
    if (JSHTMLDirectoryElementAttributesFunctions_initialized) return;
    JSHTMLDirectoryElementAttributesFunctions_initialized = true;
    memset(JSHTMLDirectoryElementAttributesFunctions, 0, sizeof(JSHTMLDirectoryElementAttributesFunctions));
    JSHTMLDirectoryElementAttributesFunctions[0].name = "compact";
    JSHTMLDirectoryElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDirectoryElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDirectoryElementAttributesFunctions[0].magic = JSHTMLDirectoryElement::CompactAttrNum;
    JSHTMLDirectoryElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLDirectoryElement::getValueProperty;
    JSHTMLDirectoryElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLDirectoryElement::putValueProperty;
    JSHTMLDirectoryElementAttributesFunctions[1].name = "constructor";
    JSHTMLDirectoryElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLDirectoryElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLDirectoryElementAttributesFunctions[1].magic = JSHTMLDirectoryElement::ConstructorAttrNum;
    JSHTMLDirectoryElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLDirectoryElement::getValueProperty;
    JSHTMLDirectoryElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLDirectoryElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLDirectoryElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLDirectoryElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLDirectoryElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLDirectoryElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLDirectoryElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDirectoryElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLDirectoryElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLDirectoryElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLDirectoryElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLDirectoryElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLDirectoryElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLDirectoryElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLDirectoryElementAttributesFunctions, countof(JSHTMLDirectoryElementAttributesFunctions));
}

static JSClassDef JSHTMLDirectoryElementClassDefine;
static bool JSHTMLDirectoryElementClassDefine_initialized = false;

static void init_JSHTMLDirectoryElementClassDefine()
{
    if (JSHTMLDirectoryElementClassDefine_initialized) return;
    JSHTMLDirectoryElementClassDefine_initialized = true;
    memset(&JSHTMLDirectoryElementClassDefine, 0, sizeof(JSHTMLDirectoryElementClassDefine));
    JSHTMLDirectoryElementClassDefine.class_name = "HTMLDirectoryElement";
    JSHTMLDirectoryElementClassDefine.finalizer = JSHTMLDirectoryElement::finalizer;
    JSHTMLDirectoryElementClassDefine.gc_mark = JSHTMLDirectoryElement::mark;
}

JSClassID JSHTMLDirectoryElement::js_class_id = 0;

void JSHTMLDirectoryElement::init(JSContext* ctx)
{
    if (JSHTMLDirectoryElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLDirectoryElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLDirectoryElement::create(JSContext* ctx, HTMLDirectoryElement* impl)
{
    JSHTMLDirectoryElement::init(ctx);
    JSValue _proto = JSHTMLDirectoryElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLDirectoryElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLDirectoryElement* impl = (HTMLDirectoryElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLDirectoryElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLDirectoryElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDirectoryElement* imp = (HTMLDirectoryElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLDirectoryElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLDirectoryElement* imp = (HTMLDirectoryElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLDirectoryElement::getConstructor(JSContext *ctx)
{
    return JSHTMLDirectoryElementConstructor::self(ctx);
}


}
