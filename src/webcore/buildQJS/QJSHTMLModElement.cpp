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

#include "QJSHTMLModElement.h"

#include "HTMLModElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLModElementAttributesFunctions[3];
static bool JSHTMLModElementAttributesFunctions_initialized = false;

static void init_JSHTMLModElementAttributesFunctions()
{
    if (JSHTMLModElementAttributesFunctions_initialized) return;
    JSHTMLModElementAttributesFunctions_initialized = true;
    memset(JSHTMLModElementAttributesFunctions, 0, sizeof(JSHTMLModElementAttributesFunctions));
    JSHTMLModElementAttributesFunctions[0].name = "cite";
    JSHTMLModElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLModElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLModElementAttributesFunctions[0].magic = JSHTMLModElement::CiteAttrNum;
    JSHTMLModElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLModElement::getValueProperty;
    JSHTMLModElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLModElement::putValueProperty;
    JSHTMLModElementAttributesFunctions[1].name = "dateTime";
    JSHTMLModElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLModElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLModElementAttributesFunctions[1].magic = JSHTMLModElement::DateTimeAttrNum;
    JSHTMLModElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLModElement::getValueProperty;
    JSHTMLModElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLModElement::putValueProperty;
    JSHTMLModElementAttributesFunctions[2].name = "constructor";
    JSHTMLModElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLModElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLModElementAttributesFunctions[2].magic = JSHTMLModElement::ConstructorAttrNum;
    JSHTMLModElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLModElement::getValueProperty;
    JSHTMLModElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSHTMLModElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLModElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLModElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLModElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLModElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLModElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLModElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLModElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLModElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLModElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLModElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLModElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLModElementAttributesFunctions, countof(JSHTMLModElementAttributesFunctions));
}

JSClassID JSHTMLModElement::js_class_id = 0;

void JSHTMLModElement::init(JSContext* ctx)
{
    if (JSHTMLModElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLModElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLModElement::create(JSContext* ctx, HTMLModElement* impl)
{
    JSHTMLModElement::init(ctx);
    JSValue _proto = JSHTMLModElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLModElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLModElement* impl = (HTMLModElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLModElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLModElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLModElement* imp = (HTMLModElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->cite()).utf8().data());
        }
        case DateTimeAttrNum: {
            HTMLModElement* imp = (HTMLModElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->dateTime()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLModElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CiteAttrNum: {
            HTMLModElement* imp = (HTMLModElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCite(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case DateTimeAttrNum: {
            HTMLModElement* imp = (HTMLModElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setDateTime(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLModElement::getConstructor(JSContext *ctx)
{
    return JSHTMLModElementConstructor::self(ctx);
}


}
