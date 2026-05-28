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

#include "QJSRect.h"

#include "CSSPrimitiveValue.h"
#include "QJSCSSPrimitiveValue.h"
#include "Rect.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSRectAttributesFunctions[5];
static bool JSRectAttributesFunctions_initialized = false;

static void init_JSRectAttributesFunctions()
{
    if (JSRectAttributesFunctions_initialized) return;
    JSRectAttributesFunctions_initialized = true;
    memset(JSRectAttributesFunctions, 0, sizeof(JSRectAttributesFunctions));
    JSRectAttributesFunctions[0].name = "top";
    JSRectAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSRectAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSRectAttributesFunctions[0].magic = JSRect::TopAttrNum;
    JSRectAttributesFunctions[0].u.getset.get.getter_magic = JSRect::getValueProperty;
    JSRectAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSRectAttributesFunctions[1].name = "right";
    JSRectAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSRectAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSRectAttributesFunctions[1].magic = JSRect::RightAttrNum;
    JSRectAttributesFunctions[1].u.getset.get.getter_magic = JSRect::getValueProperty;
    JSRectAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSRectAttributesFunctions[2].name = "bottom";
    JSRectAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSRectAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSRectAttributesFunctions[2].magic = JSRect::BottomAttrNum;
    JSRectAttributesFunctions[2].u.getset.get.getter_magic = JSRect::getValueProperty;
    JSRectAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSRectAttributesFunctions[3].name = "left";
    JSRectAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSRectAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSRectAttributesFunctions[3].magic = JSRect::LeftAttrNum;
    JSRectAttributesFunctions[3].u.getset.get.getter_magic = JSRect::getValueProperty;
    JSRectAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSRectAttributesFunctions[4].name = "constructor";
    JSRectAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSRectAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSRectAttributesFunctions[4].magic = JSRect::ConstructorAttrNum;
    JSRectAttributesFunctions[4].u.getset.get.getter_magic = JSRect::getValueProperty;
    JSRectAttributesFunctions[4].u.getset.set.setter_magic = NULL;
}

class JSRectConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSRectConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Rect.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSRectConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Rect.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSRectConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSRect.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSRectAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSRectAttributesFunctions, countof(JSRectAttributesFunctions));
}

static JSClassDef JSRectClassDefine;
static bool JSRectClassDefine_initialized = false;

static void init_JSRectClassDefine()
{
    if (JSRectClassDefine_initialized) return;
    JSRectClassDefine_initialized = true;
    memset(&JSRectClassDefine, 0, sizeof(JSRectClassDefine));
    JSRectClassDefine.class_name = "Rect";
    JSRectClassDefine.finalizer = JSRect::finalizer;
    JSRectClassDefine.gc_mark = JSRect::mark;
}

JSClassID JSRect::js_class_id = 0;

void JSRect::init(JSContext* ctx)
{
    if (JSRect::js_class_id == 0) {
        init_JSRectClassDefine();
        JS_NewClassID(&JSRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSRect::js_class_id, &JSRectClassDefine);
        JS_SetConstructor(ctx, JSRectConstructor::self(ctx), JSRectPrototype::self(ctx));
        JS_SetClassProto(ctx, JSRect::js_class_id, JSRectPrototype::self(ctx));
    }
}

JSValue JSRect::create(JSContext* ctx, Rect* impl)
{
    JSRect::init(ctx);
    JSValue _proto = JSRectPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSRect::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            Rect* imp = (Rect*)JS_GetOpaque(this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->top()));
        }
        case RightAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque(this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->right()));
        }
        case BottomAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque(this_val, JSRect::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->bottom()));
        }
        case LeftAttrNum: {
            Rect* imp = (Rect*)JS_GetOpaque(this_val, JSRect::js_class_id);
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
