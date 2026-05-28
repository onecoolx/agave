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

#include "QJSHTMLMapElement.h"

#include "HTMLCollection.h"
#include "HTMLMapElement.h"
#include "QJSHTMLCollection.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLMapElementAttributesFunctions[3];
static bool JSHTMLMapElementAttributesFunctions_initialized = false;

static void init_JSHTMLMapElementAttributesFunctions()
{
    if (JSHTMLMapElementAttributesFunctions_initialized) return;
    JSHTMLMapElementAttributesFunctions_initialized = true;
    memset(JSHTMLMapElementAttributesFunctions, 0, sizeof(JSHTMLMapElementAttributesFunctions));
    JSHTMLMapElementAttributesFunctions[0].name = "areas";
    JSHTMLMapElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMapElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMapElementAttributesFunctions[0].magic = JSHTMLMapElement::AreasAttrNum;
    JSHTMLMapElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLMapElement::getValueProperty;
    JSHTMLMapElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLMapElementAttributesFunctions[1].name = "name";
    JSHTMLMapElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMapElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMapElementAttributesFunctions[1].magic = JSHTMLMapElement::NameAttrNum;
    JSHTMLMapElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLMapElement::getValueProperty;
    JSHTMLMapElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLMapElement::putValueProperty;
    JSHTMLMapElementAttributesFunctions[2].name = "constructor";
    JSHTMLMapElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLMapElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLMapElementAttributesFunctions[2].magic = JSHTMLMapElement::ConstructorAttrNum;
    JSHTMLMapElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLMapElement::getValueProperty;
    JSHTMLMapElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSHTMLMapElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLMapElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLMapElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLMapElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLMapElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLMapElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMapElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLMapElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLMapElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLMapElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLMapElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLMapElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLMapElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLMapElementAttributesFunctions, countof(JSHTMLMapElementAttributesFunctions));
}

static JSClassDef JSHTMLMapElementClassDefine;
static bool JSHTMLMapElementClassDefine_initialized = false;

static void init_JSHTMLMapElementClassDefine()
{
    if (JSHTMLMapElementClassDefine_initialized) return;
    JSHTMLMapElementClassDefine_initialized = true;
    memset(&JSHTMLMapElementClassDefine, 0, sizeof(JSHTMLMapElementClassDefine));
    JSHTMLMapElementClassDefine.class_name = "HTMLMapElement";
    JSHTMLMapElementClassDefine.finalizer = JSHTMLMapElement::finalizer;
    JSHTMLMapElementClassDefine.gc_mark = JSHTMLMapElement::mark;
}

JSClassID JSHTMLMapElement::js_class_id = 0;

void JSHTMLMapElement::init(JSContext* ctx)
{
    if (JSHTMLMapElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLMapElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLMapElement::create(JSContext* ctx, HTMLMapElement* impl)
{
    JSHTMLMapElement::init(ctx);
    JSValue _proto = JSHTMLMapElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLMapElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLMapElement* impl = (HTMLMapElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLMapElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLMapElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AreasAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->areas()));
        }
        case NameAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLMapElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case NameAttrNum: {
            HTMLMapElement* imp = (HTMLMapElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setName(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLMapElement::getConstructor(JSContext *ctx)
{
    return JSHTMLMapElementConstructor::self(ctx);
}


}
