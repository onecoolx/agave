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

#include "QJSRect.h"

#include "CSSPrimitiveValue.h"
#include "QJSCSSPrimitiveValue.h"
#include "Rect.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSRectAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("right", JSRect::getValueProperty, NULL, JSRect::RightAttrNum),
    JS_CGETSET_MAGIC_DEF("top", JSRect::getValueProperty, NULL, JSRect::TopAttrNum),
    JS_CGETSET_MAGIC_DEF("left", JSRect::getValueProperty, NULL, JSRect::LeftAttrNum),
    JS_CGETSET_MAGIC_DEF("bottom", JSRect::getValueProperty, NULL, JSRect::BottomAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSRect::getValueProperty, NULL, JSRect::ConstructorAttrNum)
};

class JSRectConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSRectConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSRectConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Rect.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRectConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Rect.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRectConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSRect.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSRectAttributesFunctions, countof(JSRectAttributesFunctions));
}

static JSClassDef JSRectClassDefine = 
{
    "Rect",
    .finalizer = JSRect::finalizer,
    .gc_mark = JSRect::mark,
};

JSClassID JSRect::js_class_id = 0;

void JSRect::init(JSContext* ctx)
{
    if (JSRect::js_class_id == 0) {
        JS_NewClassID(&JSRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRect::js_class_id, &JSRectClassDefine);
        JS_SetConstructor(ctx, JSRectConstructor::self(ctx), JSRectPrototype::self(ctx));
        JS_SetClassProto(ctx, JSRect::js_class_id, JSRectPrototype::self(ctx));
    }
}

JSValue JSRect::create(JSContext* ctx, Rect* impl)
{
    JSRect::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSRectPrototype::self(ctx), JSRect::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSRect::finalizer(JSRuntime* rt, JSValue val)
{
    Rect* impl = (Rect*)JS_GetOpaque(val, JSRect::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSRect::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSRect::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TopAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque2(ctx, this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->top()));
        }
        case RightAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque2(ctx, this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->right()));
        }
        case BottomAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque2(ctx, this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->bottom()));
        }
        case LeftAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque2(ctx, this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->left()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSRect::getConstructor(JSContext *ctx)
{
    return JSRectConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, Rect* obj)
{
    return QJS::cacheDOMObject<Rect, JSRect>(ctx, obj);
}

Rect* toRect(JSValue val)
{
    if (JS_IsObject(val)) {
        Rect* impl = (Rect*)JS_GetOpaque(val, JSRect::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
