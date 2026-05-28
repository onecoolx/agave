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

#include "QJSStyleSheetList.h"

#include "ExceptionCode.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "StyleSheetList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSStyleSheetListAttributesFunctions[2];
static bool JSStyleSheetListAttributesFunctions_initialized = false;

static void init_JSStyleSheetListAttributesFunctions()
{
    if (JSStyleSheetListAttributesFunctions_initialized) return;
    JSStyleSheetListAttributesFunctions_initialized = true;
    memset(JSStyleSheetListAttributesFunctions, 0, sizeof(JSStyleSheetListAttributesFunctions));
    JSStyleSheetListAttributesFunctions[0].name = "length";
    JSStyleSheetListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetListAttributesFunctions[0].magic = JSStyleSheetList::LengthAttrNum;
    JSStyleSheetListAttributesFunctions[0].u.getset.get.getter_magic = JSStyleSheetList::getValueProperty;
    JSStyleSheetListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSStyleSheetListAttributesFunctions[1].name = "constructor";
    JSStyleSheetListAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSStyleSheetListAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSStyleSheetListAttributesFunctions[1].magic = JSStyleSheetList::ConstructorAttrNum;
    JSStyleSheetListAttributesFunctions[1].u.getset.get.getter_magic = JSStyleSheetList::getValueProperty;
    JSStyleSheetListAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSStyleSheetListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSStyleSheetListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[StyleSheetList.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[StyleSheetList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSStyleSheetListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSStyleSheetListPrototypeFunctions[1];
static bool JSStyleSheetListPrototypeFunctions_initialized = false;

static void init_JSStyleSheetListPrototypeFunctions()
{
    if (JSStyleSheetListPrototypeFunctions_initialized) return;
    JSStyleSheetListPrototypeFunctions_initialized = true;
    memset(JSStyleSheetListPrototypeFunctions, 0, sizeof(JSStyleSheetListPrototypeFunctions));
    JSStyleSheetListPrototypeFunctions[0].name = "item";
    JSStyleSheetListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSStyleSheetListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSStyleSheetListPrototypeFunctions[0].magic = JSStyleSheetList::ItemFuncNum;
    JSStyleSheetListPrototypeFunctions[0].u.func.length = 1;
    JSStyleSheetListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSStyleSheetListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSStyleSheetListPrototypeFunction::callAsFunction;
}

JSValue JSStyleSheetListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSStyleSheetList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSStyleSheetList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSStyleSheetListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSStyleSheetListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetListAttributesFunctions, countof(JSStyleSheetListAttributesFunctions));
    init_JSStyleSheetListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetListPrototypeFunctions, countof(JSStyleSheetListPrototypeFunctions));
}

static JSClassDef JSStyleSheetListClassDefine;
static bool JSStyleSheetListClassDefine_initialized = false;

static void init_JSStyleSheetListClassDefine()
{
    if (JSStyleSheetListClassDefine_initialized) return;
    JSStyleSheetListClassDefine_initialized = true;
    memset(&JSStyleSheetListClassDefine, 0, sizeof(JSStyleSheetListClassDefine));
    JSStyleSheetListClassDefine.class_name = "StyleSheetList";
    JSStyleSheetListClassDefine.finalizer = JSStyleSheetList::finalizer;
    JSStyleSheetListClassDefine.gc_mark = JSStyleSheetList::mark;
}

JSClassID JSStyleSheetList::js_class_id = 0;

void JSStyleSheetList::init(JSContext* ctx)
{
    if (JSStyleSheetList::js_class_id == 0) {
        init_JSStyleSheetListClassDefine();
        JS_NewClassID(&JSStyleSheetList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSStyleSheetList::js_class_id, &JSStyleSheetListClassDefine);
        JS_SetConstructor(ctx, JSStyleSheetListConstructor::self(ctx), JSStyleSheetListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSStyleSheetList::js_class_id, JSStyleSheetListPrototype::self(ctx));
    }
}

JSValue JSStyleSheetList::create(JSContext* ctx, StyleSheetList* impl)
{
    JSStyleSheetList::init(ctx);
    JSValue _proto = JSStyleSheetListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSStyleSheetList::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSStyleSheetList::finalizer(JSRuntime* rt, JSValue val)
{
    StyleSheetList* impl = (StyleSheetList*)JS_GetOpaque(val, JSStyleSheetList::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSStyleSheetList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSStyleSheetList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            StyleSheetList* imp = (StyleSheetList*)JS_GetOpaque(this_val, JSStyleSheetList::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSStyleSheetList::getConstructor(JSContext *ctx)
{
    return JSStyleSheetListConstructor::self(ctx);
}

JSValue JSStyleSheetListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    StyleSheetList* imp = (StyleSheetList*)JS_GetOpaque(this_val, JSStyleSheetList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSStyleSheetList::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->item(index)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSStyleSheetList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    StyleSheetList* imp = (StyleSheetList*)JS_GetOpaque2(ctx, this_obj, JSStyleSheetList::js_class_id);
    return toJS(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, StyleSheetList* obj)
{
    return QJS::cacheDOMObject<StyleSheetList, JSStyleSheetList>(ctx, obj);
}

StyleSheetList* toStyleSheetList(JSValue val)
{
    if (JS_IsObject(val)) {
        StyleSheetList* impl = (StyleSheetList*)JS_GetOpaque(val, JSStyleSheetList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
