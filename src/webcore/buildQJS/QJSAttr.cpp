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

#include "QJSAttr.h"

#include "Attr.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "Element.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSElement.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSAttrAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("name", JSAttr::getValueProperty, NULL, JSAttr::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("ownerElement", JSAttr::getValueProperty, NULL, JSAttr::OwnerElementAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSAttr::getValueProperty, NULL, JSAttr::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("value", JSAttr::getValueProperty, JSAttr::putValueProperty, JSAttr::ValueAttrNum),
    JS_CGETSET_MAGIC_DEF("specified", JSAttr::getValueProperty, NULL, JSAttr::SpecifiedAttrNum),
    JS_CGETSET_MAGIC_DEF("style", JSAttr::getValueProperty, NULL, JSAttr::StyleAttrNum)
};

class JSAttrConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSAttrConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSAttrConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Attr.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSAttrConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Attr.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSAttrConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSAttrPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSAttr.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSAttrPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSAttr.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSAttrPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSAttrAttributesFunctions, countof(JSAttrAttributesFunctions));
}

static JSClassDef JSAttrClassDefine = 
{
    "Attr",
    .finalizer = JSAttr::finalizer,
    .gc_mark = JSAttr::mark,
};

JSClassID JSAttr::js_class_id = 0;

void JSAttr::init(JSContext* ctx)
{
    if (JSAttr::js_class_id == 0) {
        JS_NewClassID(&JSAttr::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSAttr::js_class_id, &JSAttrClassDefine);
        JS_SetConstructor(ctx, JSAttrConstructor::self(ctx), JSAttrPrototype::self(ctx));
        JS_SetClassProto(ctx, JSAttr::js_class_id, JSAttrPrototype::self(ctx));
    }
}

JSValue JSAttr::create(JSContext* ctx, Attr* impl)
{
    JSAttr::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSAttrPrototype::self(ctx), JSAttr::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSAttr::finalizer(JSRuntime* rt, JSValue val)
{
    Attr* impl = (Attr*)JS_GetOpaque(val, JSAttr::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSAttr::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEventTargetNode::mark(rt, val, mark_func);
}

JSValue JSAttr::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NameAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            return jsStringOrNull(ctx, imp->name());
        }
        case SpecifiedAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            return JS_NewBool(ctx, imp->specified() ? 1 : 0);
        }
        case ValueAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            return jsStringOrNull(ctx, imp->value());
        }
        case OwnerElementAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerElement()));
        }
        case StyleAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSAttr::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case ValueAttrNum: {
            Attr* imp = (Attr*)JS_GetOpaque2(ctx, this_val, JSAttr::js_class_id);
            JSAttr::setValue(ctx, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSAttr::getConstructor(JSContext *ctx)
{
    return JSAttrConstructor::self(ctx);
}


}
