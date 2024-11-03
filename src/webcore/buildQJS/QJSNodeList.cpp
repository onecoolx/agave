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

#include "QJSNodeList.h"

#include "ExceptionCode.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSNode.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSNodeListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSNodeList::getValueProperty, NULL, JSNodeList::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSNodeList::getValueProperty, NULL, JSNodeList::ConstructorAttrNum)
};

class JSNodeListConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSNodeListConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSNodeListConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[NodeList.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[NodeList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSNodeListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("item", 1, JSNodeListPrototypeFunction::callAsFunction, JSNodeList::ItemFuncNum)
};

JSValue JSNodeListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeListAttributesFunctions, countof(JSNodeListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeListPrototypeFunctions, countof(JSNodeListPrototypeFunctions));
}

static JSClassDef JSNodeListClassDefine = 
{
    "NodeList",
    .finalizer = JSNodeList::finalizer,
    .gc_mark = JSNodeList::mark,
};

JSClassID JSNodeList::js_class_id = 0;

void JSNodeList::init(JSContext* ctx)
{
    if (JSNodeList::js_class_id == 0) {
        JS_NewClassID(&JSNodeList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeList::js_class_id, &JSNodeListClassDefine);
        JS_SetConstructor(ctx, JSNodeListConstructor::self(ctx), JSNodeListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNodeList::js_class_id, JSNodeListPrototype::self(ctx));
    }
}

JSValue JSNodeList::create(JSContext* ctx, NodeList* impl)
{
    JSNodeList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNodeListPrototype::self(ctx), JSNodeList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNodeList::finalizer(JSRuntime* rt, JSValue val)
{
    NodeList* impl = (NodeList*)JS_GetOpaque(val, JSNodeList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSNodeList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSNodeList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            NodeList* imp = (NodeList*)JS_GetOpaque2(ctx, this_val, JSNodeList::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSNodeList::getConstructor(JSContext *ctx)
{
    return JSNodeListConstructor::self(ctx);
}

JSValue JSNodeListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    NodeList* imp = (NodeList*)JS_GetOpaque2(ctx, this_val, JSNodeList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNodeList::ItemFuncNum: {
            bool indexOk;
            int index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (index < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->item(index)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSNodeList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    NodeList* imp = (NodeList*)JS_GetOpaque2(ctx, this_obj, JSNodeList::js_class_id);
    return toJS(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, NodeList* obj)
{
    return QJS::cacheDOMObject<NodeList, JSNodeList>(ctx, obj);
}

NodeList* toNodeList(JSValue val)
{
    if (JS_IsObject(val)) {
        NodeList* impl = (NodeList*)JS_GetOpaque(val, JSNodeList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
