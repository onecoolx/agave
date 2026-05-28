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

#include "QJSHTMLFontElement.h"

#include "HTMLFontElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLFontElementAttributesFunctions[4];
static bool JSHTMLFontElementAttributesFunctions_initialized = false;

static void init_JSHTMLFontElementAttributesFunctions()
{
    if (JSHTMLFontElementAttributesFunctions_initialized) return;
    JSHTMLFontElementAttributesFunctions_initialized = true;
    memset(JSHTMLFontElementAttributesFunctions, 0, sizeof(JSHTMLFontElementAttributesFunctions));
    JSHTMLFontElementAttributesFunctions[0].name = "size";
    JSHTMLFontElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFontElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFontElementAttributesFunctions[0].magic = JSHTMLFontElement::SizeAttrNum;
    JSHTMLFontElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLFontElement::getValueProperty;
    JSHTMLFontElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLFontElement::putValueProperty;
    JSHTMLFontElementAttributesFunctions[1].name = "color";
    JSHTMLFontElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFontElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFontElementAttributesFunctions[1].magic = JSHTMLFontElement::ColorAttrNum;
    JSHTMLFontElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLFontElement::getValueProperty;
    JSHTMLFontElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLFontElement::putValueProperty;
    JSHTMLFontElementAttributesFunctions[2].name = "face";
    JSHTMLFontElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFontElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFontElementAttributesFunctions[2].magic = JSHTMLFontElement::FaceAttrNum;
    JSHTMLFontElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLFontElement::getValueProperty;
    JSHTMLFontElementAttributesFunctions[2].u.getset.set.setter_magic = JSHTMLFontElement::putValueProperty;
    JSHTMLFontElementAttributesFunctions[3].name = "constructor";
    JSHTMLFontElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLFontElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLFontElementAttributesFunctions[3].magic = JSHTMLFontElement::ConstructorAttrNum;
    JSHTMLFontElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLFontElement::getValueProperty;
    JSHTMLFontElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSHTMLFontElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLFontElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLFontElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLFontElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLFontElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLFontElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFontElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLFontElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLFontElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLFontElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLFontElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLFontElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLFontElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLFontElementAttributesFunctions, countof(JSHTMLFontElementAttributesFunctions));
}

static JSClassDef JSHTMLFontElementClassDefine;
static bool JSHTMLFontElementClassDefine_initialized = false;

static void init_JSHTMLFontElementClassDefine()
{
    if (JSHTMLFontElementClassDefine_initialized) return;
    JSHTMLFontElementClassDefine_initialized = true;
    memset(&JSHTMLFontElementClassDefine, 0, sizeof(JSHTMLFontElementClassDefine));
    JSHTMLFontElementClassDefine.class_name = "HTMLFontElement";
    JSHTMLFontElementClassDefine.finalizer = JSHTMLFontElement::finalizer;
    JSHTMLFontElementClassDefine.gc_mark = JSHTMLFontElement::mark;
}

JSClassID JSHTMLFontElement::js_class_id = 0;

void JSHTMLFontElement::init(JSContext* ctx)
{
    if (JSHTMLFontElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLFontElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLFontElement::create(JSContext* ctx, HTMLFontElement* impl)
{
    JSHTMLFontElement::init(ctx);
    JSValue _proto = JSHTMLFontElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLFontElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLFontElement* impl = (HTMLFontElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLFontElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLFontElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->color()).utf8().data());
        }
        case FaceAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->face()).utf8().data());
        }
        case SizeAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->size()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLFontElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ColorAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setColor(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case FaceAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setFace(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLFontElement* imp = (HTMLFontElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSize(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLFontElement::getConstructor(JSContext *ctx)
{
    return JSHTMLFontElementConstructor::self(ctx);
}


}
