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

#include "QJSHTMLCollection.h"

#include "HTMLCollection.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSNode.h"
#include "QJSNodeList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLCollectionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSHTMLCollection::getValueProperty, NULL, JSHTMLCollection::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSHTMLCollection::getValueProperty, NULL, JSHTMLCollection::ConstructorAttrNum)
};

class JSHTMLCollectionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSHTMLCollectionConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSHTMLCollectionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLCollection.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCollectionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLCollection.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLCollectionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLCollectionPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("namedItem", 1, JSHTMLCollectionPrototypeFunction::callAsFunction, JSHTMLCollection::NamedItemFuncNum),
    JS_CFUNC_MAGIC_DEF("item", 1, JSHTMLCollectionPrototypeFunction::callAsFunction, JSHTMLCollection::ItemFuncNum),
    JS_CFUNC_MAGIC_DEF("tags", 1, JSHTMLCollectionPrototypeFunction::callAsFunction, JSHTMLCollection::TagsFuncNum)
};

JSValue JSHTMLCollectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLCollection.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCollectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLCollection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLCollectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCollectionAttributesFunctions, countof(JSHTMLCollectionAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCollectionPrototypeFunctions, countof(JSHTMLCollectionPrototypeFunctions));
}

static JSClassDef JSHTMLCollectionClassDefine = 
{
    "HTMLCollection",
    .finalizer = JSHTMLCollection::finalizer,
    .gc_mark = JSHTMLCollection::mark,
};

JSClassID JSHTMLCollection::js_class_id = 0;

void JSHTMLCollection::init(JSContext* ctx)
{
    if (JSHTMLCollection::js_class_id == 0) {
        JS_NewClassID(&JSHTMLCollection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLCollection::js_class_id, &JSHTMLCollectionClassDefine);
        JS_SetConstructor(ctx, JSHTMLCollectionConstructor::self(ctx), JSHTMLCollectionPrototype::self(ctx));
        JS_SetClassProto(ctx, JSHTMLCollection::js_class_id, JSHTMLCollectionPrototype::self(ctx));
    }
}

JSValue JSHTMLCollection::create(JSContext* ctx, HTMLCollection* impl)
{
    JSHTMLCollection::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLCollectionPrototype::self(ctx), JSHTMLCollection::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLCollection::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLCollection::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSHTMLCollection::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLCollection::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLCollection::getConstructor(JSContext *ctx)
{
    return JSHTMLCollectionConstructor::self(ctx);
}

JSValue JSHTMLCollectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLCollection::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLCollection::ItemFuncNum: {
            return JSHTMLCollection::item(ctx, argc, argv, imp);
        }
        case JSHTMLCollection::NamedItemFuncNum: {
            return JSHTMLCollection::namedItem(ctx, argc, argv, imp);
        }
        case JSHTMLCollection::TagsFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->tags(name)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSHTMLCollection::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque2(ctx, this_obj, JSHTMLCollection::js_class_id);
    return toJS(ctx, imp->item(idx));
}


HTMLCollection* toHTMLCollection(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
