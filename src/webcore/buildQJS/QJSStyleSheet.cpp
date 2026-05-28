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

static JSCFunctionListEntry JSStyleSheetAttributesFunctions[8];
static bool JSStyleSheetAttributesFunctions_initialized = false;

static void init_JSStyleSheetAttributesFunctions()
{
    if (JSStyleSheetAttributesFunctions_initialized) return;
    JSStyleSheetAttributesFunctions_initialized = true;
    memset(JSStyleSheetAttributesFunctions, 0, sizeof(JSStyleSheetAttributesFunctions));
    JSStyleSheetAttributesFunctions[0].name = "type";
    JSStyleSheetAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[0].magic = JSStyleSheet::TypeAttrNum;
    JSStyleSheetAttributesFunctions[0].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[1].name = "disabled";
    JSStyleSheetAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[1].magic = JSStyleSheet::DisabledAttrNum;
    JSStyleSheetAttributesFunctions[1].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[1].u.getset.set.setter_magic = JSStyleSheet::putValueProperty;
    JSStyleSheetAttributesFunctions[2].name = "ownerNode";
    JSStyleSheetAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[2].magic = JSStyleSheet::OwnerNodeAttrNum;
    JSStyleSheetAttributesFunctions[2].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[3].name = "parentStyleSheet";
    JSStyleSheetAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[3].magic = JSStyleSheet::ParentStyleSheetAttrNum;
    JSStyleSheetAttributesFunctions[3].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[4].name = "href";
    JSStyleSheetAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[4].magic = JSStyleSheet::HrefAttrNum;
    JSStyleSheetAttributesFunctions[4].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[5].name = "title";
    JSStyleSheetAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[5].magic = JSStyleSheet::TitleAttrNum;
    JSStyleSheetAttributesFunctions[5].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[6].name = "media";
    JSStyleSheetAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[6].magic = JSStyleSheet::MediaAttrNum;
    JSStyleSheetAttributesFunctions[6].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSStyleSheetAttributesFunctions[7].name = "constructor";
    JSStyleSheetAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetAttributesFunctions[7].magic = JSStyleSheet::ConstructorAttrNum;
    JSStyleSheetAttributesFunctions[7].u.getset.get.getter_magic = JSStyleSheet::getValueProperty;
    JSStyleSheetAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSStyleSheetConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSStyleSheetConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[StyleSheet.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[StyleSheet.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSStyleSheetConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSStyleSheetPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSStyleSheet.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSStyleSheet.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSStyleSheetPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSStyleSheetAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetAttributesFunctions, countof(JSStyleSheetAttributesFunctions));
}

static JSClassDef JSStyleSheetClassDefine;
static bool JSStyleSheetClassDefine_initialized = false;

static void init_JSStyleSheetClassDefine()
{
    if (JSStyleSheetClassDefine_initialized) return;
    JSStyleSheetClassDefine_initialized = true;
    memset(&JSStyleSheetClassDefine, 0, sizeof(JSStyleSheetClassDefine));
    JSStyleSheetClassDefine.class_name = "StyleSheet";
    JSStyleSheetClassDefine.finalizer = JSStyleSheet::finalizer;
    JSStyleSheetClassDefine.gc_mark = JSStyleSheet::mark;
}

JSClassID JSStyleSheet::js_class_id = 0;

void JSStyleSheet::init(JSContext* ctx)
{
    if (JSStyleSheet::js_class_id == 0) {
        init_JSStyleSheetClassDefine();
        JS_NewClassID(&JSStyleSheet::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSStyleSheet::js_class_id, &JSStyleSheetClassDefine);
        JS_SetConstructor(ctx, JSStyleSheetConstructor::self(ctx), JSStyleSheetPrototype::self(ctx));
        JS_SetClassProto(ctx, JSStyleSheet::js_class_id, JSStyleSheetPrototype::self(ctx));
    }
}

JSValue JSStyleSheet::create(JSContext* ctx, StyleSheet* impl)
{
    JSStyleSheet::init(ctx);
    JSValue _proto = JSStyleSheetPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSStyleSheet::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->type());
        }
        case DisabledAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return JS_NewBool(ctx, imp->disabled() ? 1 : 0);
        }
        case OwnerNodeAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->ownerNode()));
        }
        case ParentStyleSheetAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->parentStyleSheet()));
        }
        case HrefAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->href());
        }
        case TitleAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
            return jsStringOrNull(ctx, imp->title());
        }
        case MediaAttrNum: {
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
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
            StyleSheet* imp = (StyleSheet*)JS_GetOpaque(this_val, JSStyleSheet::js_class_id);
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
