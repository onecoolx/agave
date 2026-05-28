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

#include "QJSScreen.h"

#include "Screen.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSScreenAttributesFunctions[8];
static bool JSScreenAttributesFunctions_initialized = false;

static void init_JSScreenAttributesFunctions()
{
    if (JSScreenAttributesFunctions_initialized) return;
    JSScreenAttributesFunctions_initialized = true;
    memset(JSScreenAttributesFunctions, 0, sizeof(JSScreenAttributesFunctions));
    JSScreenAttributesFunctions[0].name = "height";
    JSScreenAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[0].magic = JSScreen::HeightAttrNum;
    JSScreenAttributesFunctions[0].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[1].name = "width";
    JSScreenAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[1].magic = JSScreen::WidthAttrNum;
    JSScreenAttributesFunctions[1].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[2].name = "availHeight";
    JSScreenAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[2].magic = JSScreen::AvailHeightAttrNum;
    JSScreenAttributesFunctions[2].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[3].name = "availLeft";
    JSScreenAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[3].magic = JSScreen::AvailLeftAttrNum;
    JSScreenAttributesFunctions[3].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[4].name = "colorDepth";
    JSScreenAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[4].magic = JSScreen::ColorDepthAttrNum;
    JSScreenAttributesFunctions[4].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[5].name = "pixelDepth";
    JSScreenAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[5].magic = JSScreen::PixelDepthAttrNum;
    JSScreenAttributesFunctions[5].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[6].name = "availTop";
    JSScreenAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[6].magic = JSScreen::AvailTopAttrNum;
    JSScreenAttributesFunctions[6].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSScreenAttributesFunctions[7].name = "availWidth";
    JSScreenAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSScreenAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSScreenAttributesFunctions[7].magic = JSScreen::AvailWidthAttrNum;
    JSScreenAttributesFunctions[7].u.getset.get.getter_magic = JSScreen::getValueProperty;
    JSScreenAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

JSValue JSScreenPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSScreen.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSScreenPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSScreen.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSScreenPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSScreenAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSScreenAttributesFunctions, countof(JSScreenAttributesFunctions));
}

static JSClassDef JSScreenClassDefine;
static bool JSScreenClassDefine_initialized = false;

static void init_JSScreenClassDefine()
{
    if (JSScreenClassDefine_initialized) return;
    JSScreenClassDefine_initialized = true;
    memset(&JSScreenClassDefine, 0, sizeof(JSScreenClassDefine));
    JSScreenClassDefine.class_name = "Screen";
    JSScreenClassDefine.finalizer = JSScreen::finalizer;
    JSScreenClassDefine.gc_mark = JSScreen::mark;
}

JSClassID JSScreen::js_class_id = 0;

void JSScreen::init(JSContext* ctx)
{
    if (JSScreen::js_class_id == 0) {
        init_JSScreenClassDefine();
        JS_NewClassID(&JSScreen::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSScreen::js_class_id, &JSScreenClassDefine);
        JS_SetClassProto(ctx, JSScreen::js_class_id, JSScreenPrototype::self(ctx));
    }
}

JSValue JSScreen::create(JSContext* ctx, Screen* impl)
{
    JSScreen::init(ctx);
    JSValue _proto = JSScreenPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSScreen::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSScreen::finalizer(JSRuntime* rt, JSValue val)
{
    Screen* impl = (Screen*)JS_GetOpaque(val, JSScreen::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSScreen::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSScreen::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case HeightAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->height());
        }
        case WidthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->width());
        }
        case ColorDepthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->colorDepth());
        }
        case PixelDepthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->pixelDepth());
        }
        case AvailLeftAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->availLeft());
        }
        case AvailTopAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->availTop());
        }
        case AvailHeightAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->availHeight());
        }
        case AvailWidthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque(this_val, JSScreen::js_class_id);
            return JS_NewInt32(ctx, imp->availWidth());
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, Screen* obj)
{
    return QJS::cacheDOMObject<Screen, JSScreen>(ctx, obj);
}

Screen* toScreen(JSValue val)
{
    if (JS_IsObject(val)) {
        Screen* impl = (Screen*)JS_GetOpaque(val, JSScreen::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
