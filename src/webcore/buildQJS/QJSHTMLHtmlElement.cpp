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

#include "QJSHTMLHtmlElement.h"

#include "HTMLHtmlElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLHtmlElementAttributesFunctions[2];
static bool JSHTMLHtmlElementAttributesFunctions_initialized = false;

static void init_JSHTMLHtmlElementAttributesFunctions()
{
    if (JSHTMLHtmlElementAttributesFunctions_initialized) return;
    JSHTMLHtmlElementAttributesFunctions_initialized = true;
    memset(JSHTMLHtmlElementAttributesFunctions, 0, sizeof(JSHTMLHtmlElementAttributesFunctions));
    JSHTMLHtmlElementAttributesFunctions[0].name = "version";
    JSHTMLHtmlElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHtmlElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHtmlElementAttributesFunctions[0].magic = JSHTMLHtmlElement::VersionAttrNum;
    JSHTMLHtmlElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLHtmlElement::getValueProperty;
    JSHTMLHtmlElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLHtmlElement::putValueProperty;
    JSHTMLHtmlElementAttributesFunctions[1].name = "constructor";
    JSHTMLHtmlElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHtmlElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHtmlElementAttributesFunctions[1].magic = JSHTMLHtmlElement::ConstructorAttrNum;
    JSHTMLHtmlElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLHtmlElement::getValueProperty;
    JSHTMLHtmlElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLHtmlElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLHtmlElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLHtmlElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLHtmlElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLHtmlElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLHtmlElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHtmlElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLHtmlElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLHtmlElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLHtmlElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLHtmlElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHtmlElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLHtmlElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLHtmlElementAttributesFunctions, countof(JSHTMLHtmlElementAttributesFunctions));
}

static JSClassDef JSHTMLHtmlElementClassDefine;
static bool JSHTMLHtmlElementClassDefine_initialized = false;

static void init_JSHTMLHtmlElementClassDefine()
{
    if (JSHTMLHtmlElementClassDefine_initialized) return;
    JSHTMLHtmlElementClassDefine_initialized = true;
    memset(&JSHTMLHtmlElementClassDefine, 0, sizeof(JSHTMLHtmlElementClassDefine));
    JSHTMLHtmlElementClassDefine.class_name = "HTMLHtmlElement";
    JSHTMLHtmlElementClassDefine.finalizer = JSHTMLHtmlElement::finalizer;
    JSHTMLHtmlElementClassDefine.gc_mark = JSHTMLHtmlElement::mark;
}

JSClassID JSHTMLHtmlElement::js_class_id = 0;

void JSHTMLHtmlElement::init(JSContext* ctx)
{
    if (JSHTMLHtmlElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLHtmlElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLHtmlElement::create(JSContext* ctx, HTMLHtmlElement* impl)
{
    JSHTMLHtmlElement::init(ctx);
    JSValue _proto = JSHTMLHtmlElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLHtmlElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLHtmlElement* impl = (HTMLHtmlElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLHtmlElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLHtmlElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case VersionAttrNum: {
            HTMLHtmlElement* imp = (HTMLHtmlElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->version()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLHtmlElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case VersionAttrNum: {
            HTMLHtmlElement* imp = (HTMLHtmlElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setVersion(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLHtmlElement::getConstructor(JSContext *ctx)
{
    return JSHTMLHtmlElementConstructor::self(ctx);
}


}
