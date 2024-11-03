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

#include "QJSStyleSheetList.h"

#include "ExceptionCode.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "StyleSheetList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSStyleSheetListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSStyleSheetList::getValueProperty, NULL, JSStyleSheetList::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSStyleSheetList::getValueProperty, NULL, JSStyleSheetList::ConstructorAttrNum)
};

class JSStyleSheetListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSStyleSheetListConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSStyleSheetListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[StyleSheetList.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[StyleSheetList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSStyleSheetListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSStyleSheetListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("item", 1, JSStyleSheetListPrototypeFunction::callAsFunction, JSStyleSheetList::ItemFuncNum)
};

JSValue JSStyleSheetListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSStyleSheetList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSStyleSheetListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSStyleSheetList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSStyleSheetListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetListAttributesFunctions, countof(JSStyleSheetListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSStyleSheetListPrototypeFunctions, countof(JSStyleSheetListPrototypeFunctions));
}

static JSClassDef JSStyleSheetListClassDefine = 
{
    "StyleSheetList",
    .finalizer = JSStyleSheetList::finalizer,
    .gc_mark = JSStyleSheetList::mark,
};

JSClassID JSStyleSheetList::js_class_id = 0;

void JSStyleSheetList::init(JSContext* ctx)
{
    if (JSStyleSheetList::js_class_id == 0) {
        JS_NewClassID(&JSStyleSheetList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSStyleSheetList::js_class_id, &JSStyleSheetListClassDefine);
        JS_SetConstructor(ctx, JSStyleSheetListConstructor::self(ctx), JSStyleSheetListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSStyleSheetList::js_class_id, JSStyleSheetListPrototype::self(ctx));
    }
}

JSValue JSStyleSheetList::create(JSContext* ctx, StyleSheetList* impl)
{
    JSStyleSheetList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSStyleSheetListPrototype::self(ctx), JSStyleSheetList::js_class_id);
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
            StyleSheetList* imp = (StyleSheetList*)JS_GetOpaque2(ctx, this_val, JSStyleSheetList::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
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
    StyleSheetList* imp = (StyleSheetList*)JS_GetOpaque2(ctx, this_val, JSStyleSheetList::js_class_id);
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
