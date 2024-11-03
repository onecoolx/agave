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

#include "QJSScreen.h"

#include "Screen.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSScreenAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("height", JSScreen::getValueProperty, NULL, JSScreen::HeightAttrNum),
    JS_CGETSET_MAGIC_DEF("width", JSScreen::getValueProperty, NULL, JSScreen::WidthAttrNum),
    JS_CGETSET_MAGIC_DEF("availHeight", JSScreen::getValueProperty, NULL, JSScreen::AvailHeightAttrNum),
    JS_CGETSET_MAGIC_DEF("availLeft", JSScreen::getValueProperty, NULL, JSScreen::AvailLeftAttrNum),
    JS_CGETSET_MAGIC_DEF("colorDepth", JSScreen::getValueProperty, NULL, JSScreen::ColorDepthAttrNum),
    JS_CGETSET_MAGIC_DEF("pixelDepth", JSScreen::getValueProperty, NULL, JSScreen::PixelDepthAttrNum),
    JS_CGETSET_MAGIC_DEF("availTop", JSScreen::getValueProperty, NULL, JSScreen::AvailTopAttrNum),
    JS_CGETSET_MAGIC_DEF("availWidth", JSScreen::getValueProperty, NULL, JSScreen::AvailWidthAttrNum)
};

JSValue JSScreenPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSScreen.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSScreenPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSScreen.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSScreenPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSScreenAttributesFunctions, countof(JSScreenAttributesFunctions));
}

static JSClassDef JSScreenClassDefine = 
{
    "Screen",
    .finalizer = JSScreen::finalizer,
    .gc_mark = JSScreen::mark,
};

JSClassID JSScreen::js_class_id = 0;

void JSScreen::init(JSContext* ctx)
{
    if (JSScreen::js_class_id == 0) {
        JS_NewClassID(&JSScreen::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSScreen::js_class_id, &JSScreenClassDefine);
        JS_SetClassProto(ctx, JSScreen::js_class_id, JSScreenPrototype::self(ctx));
    }
}

JSValue JSScreen::create(JSContext* ctx, Screen* impl)
{
    JSScreen::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSScreenPrototype::self(ctx), JSScreen::js_class_id);
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
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->height());
        }
        case WidthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->width());
        }
        case ColorDepthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->colorDepth());
        }
        case PixelDepthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->pixelDepth());
        }
        case AvailLeftAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->availLeft());
        }
        case AvailTopAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->availTop());
        }
        case AvailHeightAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->availHeight());
        }
        case AvailWidthAttrNum: {
            Screen* imp = (Screen*)JS_GetOpaque2(ctx, this_val, JSScreen::js_class_id);
            return JS_NewBigUint64(ctx, imp->availWidth());
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
