/*
 * Copyright (c) 2024, Zhang Ji Peng <onecoolx@gmail.com>
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

#include "QJSHTMLHRElement.h"

#include "HTMLHRElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLHRElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("align", JSHTMLHRElement::getValueProperty, JSHTMLHRElement::putValueProperty, JSHTMLHRElement::AlignAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSHTMLHRElement::getValueProperty, JSHTMLHRElement::putValueProperty, JSHTMLHRElement::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLHRElement::getValueProperty, NULL, JSHTMLHRElement::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("size", JSHTMLHRElement::getValueProperty, JSHTMLHRElement::putValueProperty, JSHTMLHRElement::SizeAttrNum),
    JS_CGETSET_MAGIC_DEF("noShade", JSHTMLHRElement::getValueProperty, JSHTMLHRElement::putValueProperty, JSHTMLHRElement::NoShadeAttrNum)
};

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
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLHRElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLHRElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLHRElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSHTMLHRElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLHRElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLElementPrototype::self(ctx));
        JSHTMLHRElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLHRElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLHRElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLHRElementAttributesFunctions, countof(JSHTMLHRElementAttributesFunctions));
}

static JSClassDef JSHTMLHRElementClassDefine = 
{
    "HTMLHRElement",
    .finalizer = JSHTMLHRElement::finalizer,
    .gc_mark = JSHTMLHRElement::mark,
};

JSClassID JSHTMLHRElement::js_class_id = 0;

void JSHTMLHRElement::init(JSContext* ctx)
{
    if (JSHTMLHRElement::js_class_id == 0) {
        JS_NewClassID(&JSHTMLHRElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLHRElement::js_class_id, &JSHTMLHRElementClassDefine);
        JS_SetConstructor(ctx, JSHTMLHRElementConstructor::self(ctx), JSHTMLHRElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLHRElement::js_class_id, JSHTMLHRElementPrototype::self(ctx));
    }
}

JSValue JSHTMLHRElement::create(JSContext* ctx, HTMLHRElement* impl)
{
    JSHTMLHRElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLHRElementPrototype::self(ctx), JSHTMLHRElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLHRElement::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLHRElement* impl = (HTMLHRElement*)JS_GetOpaque(val, JSHTMLHRElement::js_class_id);
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
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->align()).utf8().data());
        }
        case NoShadeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            return JS_NewBool(ctx, imp->noShade() ? 1 : 0);
        }
        case SizeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->size()).utf8().data());
        }
        case WidthAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
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
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            imp->setAlign(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case NoShadeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            imp->setNoShade(valueToBoolean(ctx, value));
            break;
        }
        case SizeAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
            imp->setSize(valueToStringWithNullCheck(ctx, value));
            break;
        }
        case WidthAttrNum: {
            HTMLHRElement* imp = (HTMLHRElement*)JS_GetOpaque2(ctx, this_val, JSHTMLHRElement::js_class_id);
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
