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

#include "QJSHTMLOListElement.h"

#include "HTMLOListElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLOListElementAttributesFunctions[4];
static bool JSHTMLOListElementAttributesFunctions_initialized = false;

static void init_JSHTMLOListElementAttributesFunctions()
{
    if (JSHTMLOListElementAttributesFunctions_initialized) return;
    JSHTMLOListElementAttributesFunctions_initialized = true;
    memset(JSHTMLOListElementAttributesFunctions, 0, sizeof(JSHTMLOListElementAttributesFunctions));
    JSHTMLOListElementAttributesFunctions[0].name = "compact";
    JSHTMLOListElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOListElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOListElementAttributesFunctions[0].magic = JSHTMLOListElement::CompactAttrNum;
    JSHTMLOListElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLOListElement::getValueProperty;
    JSHTMLOListElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLOListElement::putValueProperty;
    JSHTMLOListElementAttributesFunctions[1].name = "start";
    JSHTMLOListElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOListElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOListElementAttributesFunctions[1].magic = JSHTMLOListElement::StartAttrNum;
    JSHTMLOListElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLOListElement::getValueProperty;
    JSHTMLOListElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLOListElement::putValueProperty;
    JSHTMLOListElementAttributesFunctions[2].name = "type";
    JSHTMLOListElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOListElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOListElementAttributesFunctions[2].magic = JSHTMLOListElement::TypeAttrNum;
    JSHTMLOListElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLOListElement::getValueProperty;
    JSHTMLOListElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLOListElement::putValueProperty;
    JSHTMLOListElementAttributesFunctions[3].name = "constructor";
    JSHTMLOListElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOListElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOListElementAttributesFunctions[3].magic = JSHTMLOListElement::ConstructorAttrNum;
    JSHTMLOListElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLOListElement::getValueProperty;
    JSHTMLOListElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSHTMLOListElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLOListElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLOListElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLOListElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLOListElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLOListElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLOListElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOListElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLOListElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOListElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLOListElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLOListElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOListElementAttributesFunctions, countof(JSHTMLOListElementAttributesFunctions));
}

JSClassID JSHTMLOListElement::js_class_id = 0;

void JSHTMLOListElement::init(JSContext* ctx)
{
    if (JSHTMLOListElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLOListElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLOListElement::create(JSContext* ctx, HTMLOListElement* impl)
{
    JSHTMLOListElement::init(ctx);
    JSValue _proto = JSHTMLOListElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOListElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOListElement* impl = (HTMLOListElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLOListElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLOListElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->compact() ? 1 : 0);
        }
        case StartAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->start());
        }
        case TypeAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLOListElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CompactAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setCompact(valueToBoolean(ctx, value));
            break;
        }
        case StartAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setStart(valueToInt32(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLOListElement* imp = (HTMLOListElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLOListElement::getConstructor(JSContext *ctx)
{
    return JSHTMLOListElementConstructor::self(ctx);
}


}
