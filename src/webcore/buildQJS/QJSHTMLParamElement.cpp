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

#include "QJSHTMLParamElement.h"

#include "HTMLParamElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLParamElementAttributesFunctions[5];
static bool JSHTMLParamElementAttributesFunctions_initialized = false;

static void init_JSHTMLParamElementAttributesFunctions()
{
    if (JSHTMLParamElementAttributesFunctions_initialized) return;
    JSHTMLParamElementAttributesFunctions_initialized = true;
    memset(JSHTMLParamElementAttributesFunctions, 0, sizeof(JSHTMLParamElementAttributesFunctions));
    JSHTMLParamElementAttributesFunctions[0].name = "name";
    JSHTMLParamElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParamElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParamElementAttributesFunctions[0].magic = JSHTMLParamElement::NameAttrNum;
    JSHTMLParamElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLParamElement::getValueProperty;
    JSHTMLParamElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLParamElement::putValueProperty;
    JSHTMLParamElementAttributesFunctions[1].name = "type";
    JSHTMLParamElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParamElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParamElementAttributesFunctions[1].magic = JSHTMLParamElement::TypeAttrNum;
    JSHTMLParamElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLParamElement::getValueProperty;
    JSHTMLParamElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLParamElement::putValueProperty;
    JSHTMLParamElementAttributesFunctions[2].name = "value";
    JSHTMLParamElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParamElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParamElementAttributesFunctions[2].magic = JSHTMLParamElement::ValueAttrNum;
    JSHTMLParamElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLParamElement::getValueProperty;
    JSHTMLParamElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLParamElement::putValueProperty;
    JSHTMLParamElementAttributesFunctions[3].name = "valueType";
    JSHTMLParamElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParamElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParamElementAttributesFunctions[3].magic = JSHTMLParamElement::ValueTypeAttrNum;
    JSHTMLParamElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLParamElement::getValueProperty;
    JSHTMLParamElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLParamElement::putValueProperty;
    JSHTMLParamElementAttributesFunctions[4].name = "constructor";
    JSHTMLParamElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLParamElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLParamElementAttributesFunctions[4].magic = JSHTMLParamElement::ConstructorAttrNum;
    JSHTMLParamElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLParamElement::getValueProperty;
    JSHTMLParamElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSHTMLParamElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLParamElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLParamElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLParamElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLParamElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLParamElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLParamElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLParamElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLParamElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLParamElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLParamElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLParamElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLParamElementAttributesFunctions, countof(JSHTMLParamElementAttributesFunctions));
}

JSClassID JSHTMLParamElement::js_class_id = 0;

void JSHTMLParamElement::init(JSContext* ctx)
{
    if (JSHTMLParamElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLParamElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLParamElement::create(JSContext* ctx, HTMLParamElement* impl)
{
    JSHTMLParamElement::init(ctx);
    JSValue _proto = JSHTMLParamElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLParamElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLParamElement* impl = (HTMLParamElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMNodeForDocument(impl->document(), impl);
    impl->deref();
}

void JSHTMLParamElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLParamElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case TypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->type()).utf8().data());
        }
        case ValueAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->value()).utf8().data());
        }
        case ValueTypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->valueType()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLParamElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case TypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setType(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValue(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case ValueTypeAttrNum: {
            HTMLParamElement* imp = (HTMLParamElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setValueType(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLParamElement::getConstructor(JSContext *ctx)
{
    return JSHTMLParamElementConstructor::self(ctx);
}


}
