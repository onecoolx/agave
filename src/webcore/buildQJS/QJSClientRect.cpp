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

#include "QJSClientRect.h"

#include "ClientRect.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSClientRectAttributesFunctions[8];
static bool JSClientRectAttributesFunctions_initialized = false;

static void init_JSClientRectAttributesFunctions()
{
    if (JSClientRectAttributesFunctions_initialized) return;
    JSClientRectAttributesFunctions_initialized = true;
    memset(JSClientRectAttributesFunctions, 0, sizeof(JSClientRectAttributesFunctions));
    JSClientRectAttributesFunctions[0].name = "top";
    JSClientRectAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[0].magic = JSClientRect::TopAttrNum;
    JSClientRectAttributesFunctions[0].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[1].name = "right";
    JSClientRectAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[1].magic = JSClientRect::RightAttrNum;
    JSClientRectAttributesFunctions[1].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[2].name = "bottom";
    JSClientRectAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[2].magic = JSClientRect::BottomAttrNum;
    JSClientRectAttributesFunctions[2].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[3].name = "left";
    JSClientRectAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[3].magic = JSClientRect::LeftAttrNum;
    JSClientRectAttributesFunctions[3].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[4].name = "width";
    JSClientRectAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[4].magic = JSClientRect::WidthAttrNum;
    JSClientRectAttributesFunctions[4].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[5].name = "height";
    JSClientRectAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[5].magic = JSClientRect::HeightAttrNum;
    JSClientRectAttributesFunctions[5].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[6].name = "x";
    JSClientRectAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[6].magic = JSClientRect::XAttrNum;
    JSClientRectAttributesFunctions[6].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSClientRectAttributesFunctions[7].name = "y";
    JSClientRectAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSClientRectAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSClientRectAttributesFunctions[7].magic = JSClientRect::YAttrNum;
    JSClientRectAttributesFunctions[7].u.getset.get.getter_magic = JSClientRect::getValueProperty;
    JSClientRectAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

JSValue JSClientRectPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSClientRect.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSClientRectPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSClientRect.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSClientRectPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSClientRectAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSClientRectAttributesFunctions, countof(JSClientRectAttributesFunctions));
}

static JSClassDef JSClientRectClassDefine;
static bool JSClientRectClassDefine_initialized = false;

static void init_JSClientRectClassDefine()
{
    if (JSClientRectClassDefine_initialized) return;
    JSClientRectClassDefine_initialized = true;
    memset(&JSClientRectClassDefine, 0, sizeof(JSClientRectClassDefine));
    JSClientRectClassDefine.class_name = "ClientRect";
    JSClientRectClassDefine.finalizer = JSClientRect::finalizer;
    JSClientRectClassDefine.gc_mark = JSClientRect::mark;
}

JSClassID JSClientRect::js_class_id = 0;

void JSClientRect::init(JSContext* ctx)
{
    if (JSClientRect::js_class_id == 0) {
        init_JSClientRectClassDefine();
        JS_NewClassID(&JSClientRect::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSClientRect::js_class_id, &JSClientRectClassDefine);
        JS_SetClassProto(ctx, JSClientRect::js_class_id, JSClientRectPrototype::self(ctx));
    }
}

JSValue JSClientRect::create(JSContext* ctx, ClientRect* impl)
{
    JSClientRect::init(ctx);
    JSValue _proto = JSClientRectPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSClientRect::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSClientRect::finalizer(JSRuntime* rt, JSValue val)
{
    ClientRect* impl = (ClientRect*)JS_GetOpaque(val, JSClientRect::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSClientRect::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSClientRect::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TopAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->top());
        }
        case RightAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->right());
        }
        case BottomAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->bottom());
        }
        case LeftAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->left());
        }
        case WidthAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case HeightAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->height());
        }
        case XAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->x());
        }
        case YAttrNum: {
            ClientRect* imp = (ClientRect*)JS_GetOpaque(this_val, JSClientRect::js_class_id);
            return JS_NewInt32(ctx, imp->y());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, ClientRect* obj)
{
    return QJS::cacheDOMObject<ClientRect, JSClientRect>(ctx, obj);
}

ClientRect* toClientRect(JSValue val)
{
    if (JS_IsObject(val)) {
        ClientRect* impl = (ClientRect*)JS_GetOpaque(val, JSClientRect::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
