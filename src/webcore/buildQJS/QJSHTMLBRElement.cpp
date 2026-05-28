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

#include "QJSHTMLBRElement.h"

#include "HTMLBRElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLBRElementAttributesFunctions[2];
static bool JSHTMLBRElementAttributesFunctions_initialized = false;

static void init_JSHTMLBRElementAttributesFunctions()
{
    if (JSHTMLBRElementAttributesFunctions_initialized) return;
    JSHTMLBRElementAttributesFunctions_initialized = true;
    memset(JSHTMLBRElementAttributesFunctions, 0, sizeof(JSHTMLBRElementAttributesFunctions));
    JSHTMLBRElementAttributesFunctions[0].name = "clear";
    JSHTMLBRElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBRElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBRElementAttributesFunctions[0].magic = JSHTMLBRElement::ClearAttrNum;
    JSHTMLBRElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLBRElement::getValueProperty;
    JSHTMLBRElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLBRElement::putValueProperty;
    JSHTMLBRElementAttributesFunctions[1].name = "constructor";
    JSHTMLBRElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBRElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBRElementAttributesFunctions[1].magic = JSHTMLBRElement::ConstructorAttrNum;
    JSHTMLBRElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLBRElement::getValueProperty;
    JSHTMLBRElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLBRElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLBRElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBRElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBRElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBRElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBRElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBRElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBRElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBRElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBRElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBRElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLBRElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBRElementAttributesFunctions, countof(JSHTMLBRElementAttributesFunctions));
}

static JSClassDef JSHTMLBRElementClassDefine;
static bool JSHTMLBRElementClassDefine_initialized = false;

static void init_JSHTMLBRElementClassDefine()
{
    if (JSHTMLBRElementClassDefine_initialized) return;
    JSHTMLBRElementClassDefine_initialized = true;
    memset(&JSHTMLBRElementClassDefine, 0, sizeof(JSHTMLBRElementClassDefine));
    JSHTMLBRElementClassDefine.class_name = "HTMLBRElement";
    JSHTMLBRElementClassDefine.finalizer = JSHTMLBRElement::finalizer;
    JSHTMLBRElementClassDefine.gc_mark = JSHTMLBRElement::mark;
}

JSClassID JSHTMLBRElement::js_class_id = 0;

void JSHTMLBRElement::init(JSContext* ctx)
{
    if (JSHTMLBRElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLBRElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLBRElement::create(JSContext* ctx, HTMLBRElement* impl)
{
    JSHTMLBRElement::init(ctx);
    JSValue _proto = JSHTMLBRElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBRElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBRElement* impl = (HTMLBRElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBRElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBRElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ClearAttrNum: {
            HTMLBRElement* imp = (HTMLBRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->clear()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBRElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ClearAttrNum: {
            HTMLBRElement* imp = (HTMLBRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setClear(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBRElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBRElementConstructor::self(ctx);
}


}
