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

#include "QJSMediaList.h"

#include "ExceptionCode.h"
#include "MediaList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSMediaListAttributesFunctions[3];
static bool JSMediaListAttributesFunctions_initialized = false;

static void init_JSMediaListAttributesFunctions()
{
    if (JSMediaListAttributesFunctions_initialized) return;
    JSMediaListAttributesFunctions_initialized = true;
    memset(JSMediaListAttributesFunctions, 0, sizeof(JSMediaListAttributesFunctions));
    JSMediaListAttributesFunctions[0].name = "constructor";
    JSMediaListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSMediaListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSMediaListAttributesFunctions[0].magic = JSMediaList::ConstructorAttrNum;
    JSMediaListAttributesFunctions[0].u.getset.get.getter_magic = JSMediaList::getValueProperty;
    JSMediaListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSMediaListAttributesFunctions[1].name = "length";
    JSMediaListAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSMediaListAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSMediaListAttributesFunctions[1].magic = JSMediaList::LengthAttrNum;
    JSMediaListAttributesFunctions[1].u.getset.get.getter_magic = JSMediaList::getValueProperty;
    JSMediaListAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSMediaListAttributesFunctions[2].name = "mediaText";
    JSMediaListAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSMediaListAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSMediaListAttributesFunctions[2].magic = JSMediaList::MediaTextAttrNum;
    JSMediaListAttributesFunctions[2].u.getset.get.getter_magic = JSMediaList::getValueProperty;
    JSMediaListAttributesFunctions[2].u.getset.set.setter_magic = JSMediaList::putValueProperty;
}

class JSMediaListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSMediaListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[MediaList.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSMediaListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[MediaList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSMediaListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSMediaListPrototypeFunctions[3];
static bool JSMediaListPrototypeFunctions_initialized = false;

static void init_JSMediaListPrototypeFunctions()
{
    if (JSMediaListPrototypeFunctions_initialized) return;
    JSMediaListPrototypeFunctions_initialized = true;
    memset(JSMediaListPrototypeFunctions, 0, sizeof(JSMediaListPrototypeFunctions));
    JSMediaListPrototypeFunctions[0].name = "deleteMedium";
    JSMediaListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSMediaListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSMediaListPrototypeFunctions[0].magic = JSMediaList::DeleteMediumFuncNum;
    JSMediaListPrototypeFunctions[0].u.func.length = 1;
    JSMediaListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSMediaListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSMediaListPrototypeFunction::callAsFunction;
    JSMediaListPrototypeFunctions[1].name = "item";
    JSMediaListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSMediaListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSMediaListPrototypeFunctions[1].magic = JSMediaList::ItemFuncNum;
    JSMediaListPrototypeFunctions[1].u.func.length = 1;
    JSMediaListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSMediaListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSMediaListPrototypeFunction::callAsFunction;
    JSMediaListPrototypeFunctions[2].name = "appendMedium";
    JSMediaListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSMediaListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSMediaListPrototypeFunctions[2].magic = JSMediaList::AppendMediumFuncNum;
    JSMediaListPrototypeFunctions[2].u.func.length = 1;
    JSMediaListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSMediaListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSMediaListPrototypeFunction::callAsFunction;
}

JSValue JSMediaListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSMediaList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSMediaListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSMediaList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSMediaListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSMediaListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMediaListAttributesFunctions, countof(JSMediaListAttributesFunctions));
    init_JSMediaListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSMediaListPrototypeFunctions, countof(JSMediaListPrototypeFunctions));
}

static JSClassDef JSMediaListClassDefine;
static bool JSMediaListClassDefine_initialized = false;

static void init_JSMediaListClassDefine()
{
    if (JSMediaListClassDefine_initialized) return;
    JSMediaListClassDefine_initialized = true;
    memset(&JSMediaListClassDefine, 0, sizeof(JSMediaListClassDefine));
    JSMediaListClassDefine.class_name = "MediaList";
    JSMediaListClassDefine.finalizer = JSMediaList::finalizer;
    JSMediaListClassDefine.gc_mark = JSMediaList::mark;
}

JSClassID JSMediaList::js_class_id = 0;

void JSMediaList::init(JSContext* ctx)
{
    if (JSMediaList::js_class_id == 0) {
        init_JSMediaListClassDefine();
        JS_NewClassID(&JSMediaList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSMediaList::js_class_id, &JSMediaListClassDefine);
        JS_SetConstructor(ctx, JSMediaListConstructor::self(ctx), JSMediaListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSMediaList::js_class_id, JSMediaListPrototype::self(ctx));
    }
}

JSValue JSMediaList::create(JSContext* ctx, MediaList* impl)
{
    JSMediaList::init(ctx);
    JSValue _proto = JSMediaListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSMediaList::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSMediaList::finalizer(JSRuntime* rt, JSValue val)
{
    MediaList* impl = (MediaList*)JS_GetOpaque(val, JSMediaList::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSMediaList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSMediaList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case MediaTextAttrNum: {
            MediaList* imp = (MediaList*)JS_GetOpaque(this_val, JSMediaList::js_class_id);
            return jsStringOrNull(ctx, imp->mediaText());
        }
        case LengthAttrNum: {
            MediaList* imp = (MediaList*)JS_GetOpaque(this_val, JSMediaList::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSMediaList::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case MediaTextAttrNum: {
            MediaList* imp = (MediaList*)JS_GetOpaque(this_val, JSMediaList::js_class_id);
            ExceptionCode ec = 0;
            imp->setMediaText(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSMediaList::getConstructor(JSContext *ctx)
{
    return JSMediaListConstructor::self(ctx);
}

JSValue JSMediaListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    MediaList* imp = (MediaList*)JS_GetOpaque(this_val, JSMediaList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSMediaList::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = jsStringOrNull(ctx, imp->item(index));
            return result;
        }
        case JSMediaList::DeleteMediumFuncNum: {
            ExceptionCode ec = 0;
            String oldMedium = valueToString(ctx, argv[0]);
            imp->deleteMedium(oldMedium, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSMediaList::AppendMediumFuncNum: {
            ExceptionCode ec = 0;
            String newMedium = valueToString(ctx, argv[0]);
            imp->appendMedium(newMedium, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue JSMediaList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    MediaList* imp = (MediaList*)JS_GetOpaque2(ctx, this_obj, JSMediaList::js_class_id);
    return jsStringOrNull(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, MediaList* obj)
{
    return QJS::cacheDOMObject<MediaList, JSMediaList>(ctx, obj);
}

MediaList* toMediaList(JSValue val)
{
    if (JS_IsObject(val)) {
        MediaList* impl = (MediaList*)JS_GetOpaque(val, JSMediaList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
