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

#include "QJSNamedNodeMap.h"

#include "ExceptionCode.h"
#include "NamedNodeMap.h"
#include "Node.h"
#include "QJSNode.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSNamedNodeMapAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("length", JSNamedNodeMap::getValueProperty, NULL, JSNamedNodeMap::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSNamedNodeMap::getValueProperty, NULL, JSNamedNodeMap::ConstructorAttrNum)
};

class JSNamedNodeMapConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSNamedNodeMapConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSNamedNodeMapConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[NamedNodeMap.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNamedNodeMapConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[NamedNodeMap.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNamedNodeMapConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSNamedNodeMapPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("getNamedItem", 1, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::GetNamedItemFuncNum),
    JS_CFUNC_MAGIC_DEF("setNamedItem", 1, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::SetNamedItemFuncNum),
    JS_CFUNC_MAGIC_DEF("getNamedItemNS", 2, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::GetNamedItemNSFuncNum),
    JS_CFUNC_MAGIC_DEF("removeNamedItem", 1, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::RemoveNamedItemFuncNum),
    JS_CFUNC_MAGIC_DEF("item", 1, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::ItemFuncNum),
    JS_CFUNC_MAGIC_DEF("setNamedItemNS", 1, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::SetNamedItemNSFuncNum),
    JS_CFUNC_MAGIC_DEF("removeNamedItemNS", 2, JSNamedNodeMapPrototypeFunction::callAsFunction, JSNamedNodeMap::RemoveNamedItemNSFuncNum)
};

JSValue JSNamedNodeMapPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNamedNodeMap.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNamedNodeMapPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNamedNodeMap.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNamedNodeMapPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNamedNodeMapAttributesFunctions, countof(JSNamedNodeMapAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNamedNodeMapPrototypeFunctions, countof(JSNamedNodeMapPrototypeFunctions));
}

static JSClassDef JSNamedNodeMapClassDefine = 
{
    "NamedNodeMap",
    .finalizer = JSNamedNodeMap::finalizer,
    .gc_mark = JSNamedNodeMap::mark,
};

JSClassID JSNamedNodeMap::js_class_id = 0;

void JSNamedNodeMap::init(JSContext* ctx)
{
    if (JSNamedNodeMap::js_class_id == 0) {
        JS_NewClassID(&JSNamedNodeMap::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNamedNodeMap::js_class_id, &JSNamedNodeMapClassDefine);
        JS_SetConstructor(ctx, JSNamedNodeMapConstructor::self(ctx), JSNamedNodeMapPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNamedNodeMap::js_class_id, JSNamedNodeMapPrototype::self(ctx));
    }
}

JSValue JSNamedNodeMap::create(JSContext* ctx, NamedNodeMap* impl)
{
    JSNamedNodeMap::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNamedNodeMapPrototype::self(ctx), JSNamedNodeMap::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNamedNodeMap::finalizer(JSRuntime* rt, JSValue val)
{
    NamedNodeMap* impl = (NamedNodeMap*)JS_GetOpaque(val, JSNamedNodeMap::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSNamedNodeMap::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSNamedNodeMap::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            NamedNodeMap* imp = (NamedNodeMap*)JS_GetOpaque2(ctx, this_val, JSNamedNodeMap::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSNamedNodeMap::getConstructor(JSContext *ctx)
{
    return JSNamedNodeMapConstructor::self(ctx);
}

JSValue JSNamedNodeMapPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    NamedNodeMap* imp = (NamedNodeMap*)JS_GetOpaque2(ctx, this_val, JSNamedNodeMap::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNamedNodeMap::GetNamedItemFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getNamedItem(name)));
            return result;
        }
        case JSNamedNodeMap::SetNamedItemFuncNum: {
            ExceptionCode ec = 0;
            Node* node = toNode(argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->setNamedItem(node, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSNamedNodeMap::RemoveNamedItemFuncNum: {
            ExceptionCode ec = 0;
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->removeNamedItem(name, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSNamedNodeMap::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->item(index)));
            return result;
        }
        case JSNamedNodeMap::GetNamedItemNSFuncNum: {
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getNamedItemNS(namespaceURI, localName)));
            return result;
        }
        case JSNamedNodeMap::SetNamedItemNSFuncNum: {
            ExceptionCode ec = 0;
            Node* node = toNode(argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->setNamedItemNS(node, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSNamedNodeMap::RemoveNamedItemNSFuncNum: {
            ExceptionCode ec = 0;
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String localName = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->removeNamedItemNS(namespaceURI, localName, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSNamedNodeMap::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    NamedNodeMap* imp = (NamedNodeMap*)JS_GetOpaque2(ctx, this_obj, JSNamedNodeMap::js_class_id);
    return toJS(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, NamedNodeMap* obj)
{
    return QJS::cacheDOMObject<NamedNodeMap, JSNamedNodeMap>(ctx, obj);
}

NamedNodeMap* toNamedNodeMap(JSValue val)
{
    if (JS_IsObject(val)) {
        NamedNodeMap* impl = (NamedNodeMap*)JS_GetOpaque(val, JSNamedNodeMap::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
