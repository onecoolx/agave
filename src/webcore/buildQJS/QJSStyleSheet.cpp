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

#include "QJSStyleSheet.h"

#include "MediaList.h"
#include "Node.h"
#include "QJSMediaList.h"
#include "QJSNode.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSStyleSheetAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("href", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::HrefAttrNum),
    JS_CGETSET_MAGIC_DEF("disabled", JSStyleSheet::getValueProperty, JSStyleSheet::putValueProperty, JSStyleSheet::DisabledAttrNum),
    JS_CGETSET_MAGIC_DEF("parentStyleSheet", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::ParentStyleSheetAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("ownerNode", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::OwnerNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("media", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::MediaAttrNum),
    JS_CGETSET_MAGIC_DEF("title", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::TitleAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSStyleSheet::getValueProperty, NULL, JSStyleSheet::ConstructorAttrNum)
};

class JSStyleSheetConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSStyleSheetConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSStyleSheetConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[StyleSheet.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[StyleSheet.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSStyleSheetConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSStyleSheetPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSStyleSheet.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSStyleSheet.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSStyleSheetPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetAttributesFunctions, countof(JSStyleSheetAttributesFunctions));
}

static JSClassDef JSStyleSheetClassDefine = 
{
    "StyleSheet",
    .finalizer = JSStyleSheet::finalizer,
    .gc_mark = JSStyleSheet::mark,
};

JSClassID JSStyleSheet::js_class_id = 0;

void JSStyleSheet::init(JSContext* ctx)
{
    if (JSStyleSheet::js_class_id == 0) {
        JS_NewClassID(&JSStyleSheet::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSStyleSheet::js_class_id, &JSStyleSheetClassDefine);
        JS_SetConstructor(ctx, JSStyleSheetConstructor::self(ctx), JSStyleSheetPrototype::self(ctx));
        JS_SetClassProto(ctx, JSStyleSheet::js_class_id, JSStyleSheetPrototype::self(ctx));
    }
}

JSValue JSStyleSheet::create(JSContext* ctx, StyleSheet* impl)
{
    JSStyleSheet::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSStyleSheetPrototype::self(ctx), JSStyleSheet::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSStyleSheet::finalizer(JSRuntime* rt, JSValue val)
{
    StyleSheet* impl = (StyleSheet*)JS_GetOpaque(val, JSStyleSheet::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSStyleSheet::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSStyleSheet::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->type());
        }
        case DisabledAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case OwnerNodeAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerNode()));
        }
        case ParentStyleSheetAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentStyleSheet()));
        }
        case HrefAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->href());
        }
        case TitleAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->title());
        }
        case MediaAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->media()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSStyleSheet::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DisabledAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque2(ctx, this_val, JSStyleSheet::js_class_id);
            imp->setDisabled(valueToBoolean(ctx, value));
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSStyleSheet::getConstructor(JSContext *ctx)
{
    return JSStyleSheetConstructor::self(ctx);
}


StyleSheet* toStyleSheet(JSValue val)
{
    if (JS_IsObject(val)) {
        StyleSheet* impl = (StyleSheet*)JS_GetOpaque(val, JSStyleSheet::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
