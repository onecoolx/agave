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

#include "QJSHTMLHRElement.h"

#include "HTMLHRElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLHRElementAttributesFunctions[5];
static bool JSHTMLHRElementAttributesFunctions_initialized = false;

static void init_JSHTMLHRElementAttributesFunctions()
{
    if (JSHTMLHRElementAttributesFunctions_initialized) return;
    JSHTMLHRElementAttributesFunctions_initialized = true;
    memset(JSHTMLHRElementAttributesFunctions, 0, sizeof(JSHTMLHRElementAttributesFunctions));
    JSHTMLHRElementAttributesFunctions[0].name = "align";
    JSHTMLHRElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHRElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHRElementAttributesFunctions[0].magic = JSHTMLHRElement::AlignAttrNum;
    JSHTMLHRElementAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLHRElement::getValueProperty;
    JSHTMLHRElementAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLHRElement::putValueProperty;
    JSHTMLHRElementAttributesFunctions[1].name = "width";
    JSHTMLHRElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHRElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHRElementAttributesFunctions[1].magic = JSHTMLHRElement::WidthAttrNum;
    JSHTMLHRElementAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLHRElement::getValueProperty;
    JSHTMLHRElementAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLHRElement::putValueProperty;
    JSHTMLHRElementAttributesFunctions[2].name = "constructor";
    JSHTMLHRElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHRElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHRElementAttributesFunctions[2].magic = JSHTMLHRElement::ConstructorAttrNum;
    JSHTMLHRElementAttributesFunctions[2].u.getset.get.getter_magic = JSHTMLHRElement::getValueProperty;
    JSHTMLHRElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSHTMLHRElementAttributesFunctions[3].name = "size";
    JSHTMLHRElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHRElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHRElementAttributesFunctions[3].magic = JSHTMLHRElement::SizeAttrNum;
    JSHTMLHRElementAttributesFunctions[3].u.getset.get.getter_magic = JSHTMLHRElement::getValueProperty;
    JSHTMLHRElementAttributesFunctions[3].u.getset.set.setter_magic = JSHTMLHRElement::putValueProperty;
    JSHTMLHRElementAttributesFunctions[4].name = "noShade";
    JSHTMLHRElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLHRElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLHRElementAttributesFunctions[4].magic = JSHTMLHRElement::NoShadeAttrNum;
    JSHTMLHRElementAttributesFunctions[4].u.getset.get.getter_magic = JSHTMLHRElement::getValueProperty;
    JSHTMLHRElementAttributesFunctions[4].u.getset.set.setter_magic = JSHTMLHRElement::putValueProperty;
}

class JSHTMLHRElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLHRElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLHRElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLHRElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLHRElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLHRElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHRElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLHRElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLHRElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLHRElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLHRElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLHRElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLHRElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLHRElementAttributesFunctions, countof(JSHTMLHRElementAttributesFunctions));
}

static JSClassDef JSHTMLHRElementClassDefine;
static bool JSHTMLHRElementClassDefine_initialized = false;

static void init_JSHTMLHRElementClassDefine()
{
    if (JSHTMLHRElementClassDefine_initialized) return;
    JSHTMLHRElementClassDefine_initialized = true;
    memset(&JSHTMLHRElementClassDefine, 0, sizeof(JSHTMLHRElementClassDefine));
    JSHTMLHRElementClassDefine.class_name = "HTMLHRElement";
    JSHTMLHRElementClassDefine.finalizer = JSHTMLHRElement::finalizer;
    JSHTMLHRElementClassDefine.gc_mark = JSHTMLHRElement::mark;
}

JSClassID JSHTMLHRElement::js_class_id = 0;

void JSHTMLHRElement::init(JSContext* ctx)
{
    if (JSHTMLHRElement::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLHRElement::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSHTMLHRElement::create(JSContext* ctx, HTMLHRElement* impl)
{
    JSHTMLHRElement::init(ctx);
    JSValue _proto = JSHTMLHRElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLHRElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLHRElement* impl = (HTMLHRElement*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLHRElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLElement::mark(rt, val, mark_func);
}

JSValue JSHTMLHRElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case NoShadeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewBool(ctx, imp->noShade() ? 1 : 0);
        }
        case SizeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->size()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->width()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLHRElement::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case AlignAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoShadeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setNoShade(valueToBoolean(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setSize(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque(this_val, JSNode::js_class_id);
            imp->setWidth(valueToStringWithNullCheck(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLHRElement::getConstructor(JSContext *ctx)
{
    return JSHTMLHRElementConstructor::self(ctx);
}


}
