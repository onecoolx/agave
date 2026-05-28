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

#include "QJSHTMLBaseFontElement.h"

#include "HTMLBaseFontElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLBaseFontElementAttributesFunctions[4];
static bool JSHTMLBaseFontElementAttributesFunctions_initialized = false;

static void init_JSHTMLBaseFontElementAttributesFunctions()
{
    if (JSHTMLBaseFontElementAttributesFunctions_initialized) return;
    JSHTMLBaseFontElementAttributesFunctions_initialized = true;
    memset(JSHTMLBaseFontElementAttributesFunctions, 0, sizeof(JSHTMLBaseFontElementAttributesFunctions));
    JSHTMLBaseFontElementAttributesFunctions[0].name = "color";
    JSHTMLBaseFontElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseFontElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseFontElementAttributesFunctions[0].magic = JSHTMLBaseFontElement::ColorAttrNum;
    JSHTMLBaseFontElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLBaseFontElement::getValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLBaseFontElement::putValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[1].name = "face";
    JSHTMLBaseFontElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseFontElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseFontElementAttributesFunctions[1].magic = JSHTMLBaseFontElement::FaceAttrNum;
    JSHTMLBaseFontElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLBaseFontElement::getValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLBaseFontElement::putValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[2].name = "size";
    JSHTMLBaseFontElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseFontElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseFontElementAttributesFunctions[2].magic = JSHTMLBaseFontElement::SizeAttrNum;
    JSHTMLBaseFontElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLBaseFontElement::getValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLBaseFontElement::putValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[3].name = "constructor";
    JSHTMLBaseFontElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLBaseFontElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLBaseFontElementAttributesFunctions[3].magic = JSHTMLBaseFontElement::ConstructorAttrNum;
    JSHTMLBaseFontElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLBaseFontElement::getValueProperty;
    JSHTMLBaseFontElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSHTMLBaseFontElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLBaseFontElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLBaseFontElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLBaseFontElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLBaseFontElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBaseFontElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLBaseFontElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLBaseFontElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLBaseFontElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLBaseFontElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLBaseFontElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLBaseFontElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLBaseFontElementAttributesFunctions, countof(JSHTMLBaseFontElementAttributesFunctions));
}

JSClassID JSHTMLBaseFontElement::js_class_id = 0;

void JSHTMLBaseFontElement::init(JSContext* ctx)
{
    if (JSHTMLBaseFontElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLBaseFontElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLBaseFontElement::create(JSContext* ctx, HTMLBaseFontElement* impl)
{
    JSHTMLBaseFontElement::init(ctx);
    JSValue _proto = JSHTMLBaseFontElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLBaseFontElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLBaseFontElement* impl = (HTMLBaseFontElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLBaseFontElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLBaseFontElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->color()).utf8().data());
        }
        case FaceAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->face()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->size());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLBaseFontElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FaceAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLBaseFontElement* imp = (HTMLBaseFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSize(valueToInt32(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLBaseFontElement::getConstructor(JSContext *ctx)
{
    return JSHTMLBaseFontElementConstructor::self(ctx);
}


}
