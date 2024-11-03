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


#if ENABLE(XPATH)

#include "QJSXPathResult.h"

#include "ExceptionCode.h"
#include "Node.h"
#include "QJSNode.h"
#include "XPathResult.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSXPathResultAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("resultType", JSXPathResult::getValueProperty, NULL, JSXPathResult::ResultTypeAttrNum),
    JS_CGETSET_MAGIC_DEF("singleNodeValue", JSXPathResult::getValueProperty, NULL, JSXPathResult::SingleNodeValueAttrNum),
    JS_CGETSET_MAGIC_DEF("stringValue", JSXPathResult::getValueProperty, NULL, JSXPathResult::StringValueAttrNum),
    JS_CGETSET_MAGIC_DEF("booleanValue", JSXPathResult::getValueProperty, NULL, JSXPathResult::BooleanValueAttrNum),
    JS_CGETSET_MAGIC_DEF("numberValue", JSXPathResult::getValueProperty, NULL, JSXPathResult::NumberValueAttrNum),
    JS_CGETSET_MAGIC_DEF("invalidIteratorState", JSXPathResult::getValueProperty, NULL, JSXPathResult::InvalidIteratorStateAttrNum),
    JS_CGETSET_MAGIC_DEF("snapshotLength", JSXPathResult::getValueProperty, NULL, JSXPathResult::SnapshotLengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSXPathResult::getValueProperty, NULL, JSXPathResult::ConstructorAttrNum)
};

class JSXPathResultConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSXPathResultConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSXPathResultConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("ANY_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::ANY_TYPE),
    JS_CGETSET_MAGIC_DEF("NUMBER_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::NUMBER_TYPE),
    JS_CGETSET_MAGIC_DEF("ORDERED_NODE_SNAPSHOT_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::ORDERED_NODE_SNAPSHOT_TYPE),
    JS_CGETSET_MAGIC_DEF("FIRST_ORDERED_NODE_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::FIRST_ORDERED_NODE_TYPE),
    JS_CGETSET_MAGIC_DEF("BOOLEAN_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::BOOLEAN_TYPE),
    JS_CGETSET_MAGIC_DEF("STRING_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::STRING_TYPE),
    JS_CGETSET_MAGIC_DEF("UNORDERED_NODE_ITERATOR_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::UNORDERED_NODE_ITERATOR_TYPE),
    JS_CGETSET_MAGIC_DEF("ORDERED_NODE_ITERATOR_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::ORDERED_NODE_ITERATOR_TYPE),
    JS_CGETSET_MAGIC_DEF("UNORDERED_NODE_SNAPSHOT_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::UNORDERED_NODE_SNAPSHOT_TYPE),
    JS_CGETSET_MAGIC_DEF("ANY_UNORDERED_NODE_TYPE", JSXPathResultConstructor::getValueProperty, NULL, XPathResult::ANY_UNORDERED_NODE_TYPE)
};

JSValue JSXPathResultConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[XPathResult.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathResultConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[XPathResult.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSXPathResultConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultConstructorFunctions, countof(JSXPathResultConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSXPathResultPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("ANY_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::ANY_TYPE),
    JS_CGETSET_MAGIC_DEF("NUMBER_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::NUMBER_TYPE),
    JS_CGETSET_MAGIC_DEF("ORDERED_NODE_SNAPSHOT_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::ORDERED_NODE_SNAPSHOT_TYPE),
    JS_CGETSET_MAGIC_DEF("FIRST_ORDERED_NODE_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::FIRST_ORDERED_NODE_TYPE),
    JS_CGETSET_MAGIC_DEF("BOOLEAN_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::BOOLEAN_TYPE),
    JS_CGETSET_MAGIC_DEF("STRING_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::STRING_TYPE),
    JS_CGETSET_MAGIC_DEF("UNORDERED_NODE_ITERATOR_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::UNORDERED_NODE_ITERATOR_TYPE),
    JS_CGETSET_MAGIC_DEF("ORDERED_NODE_ITERATOR_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::ORDERED_NODE_ITERATOR_TYPE),
    JS_CGETSET_MAGIC_DEF("UNORDERED_NODE_SNAPSHOT_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::UNORDERED_NODE_SNAPSHOT_TYPE),
    JS_CGETSET_MAGIC_DEF("ANY_UNORDERED_NODE_TYPE", JSXPathResultPrototype::getValueProperty, NULL, XPathResult::ANY_UNORDERED_NODE_TYPE)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSXPathResultPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("snapshotItem", 1, JSXPathResultPrototypeFunction::callAsFunction, JSXPathResult::SnapshotItemFuncNum),
    JS_CFUNC_MAGIC_DEF("iterateNext", 0, JSXPathResultPrototypeFunction::callAsFunction, JSXPathResult::IterateNextFuncNum)
};

JSValue JSXPathResultPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXPathResult.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathResultPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXPathResult.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSXPathResultPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultAttributesFunctions, countof(JSXPathResultAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultPrototypeConstantsFunctions, countof(JSXPathResultPrototypeConstantsFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultPrototypeFunctions, countof(JSXPathResultPrototypeFunctions));
}

JSValue JSXPathResultPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSXPathResultClassDefine = 
{
    "XPathResult",
    .finalizer = JSXPathResult::finalizer,
    .gc_mark = JSXPathResult::mark,
};

JSClassID JSXPathResult::js_class_id = 0;

void JSXPathResult::init(JSContext* ctx)
{
    if (JSXPathResult::js_class_id == 0) {
        JS_NewClassID(&JSXPathResult::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXPathResult::js_class_id, &JSXPathResultClassDefine);
        JS_SetConstructor(ctx, JSXPathResultConstructor::self(ctx), JSXPathResultPrototype::self(ctx));
        JS_SetClassProto(ctx, JSXPathResult::js_class_id, JSXPathResultPrototype::self(ctx));
    }
}

JSValue JSXPathResult::create(JSContext* ctx, XPathResult* impl)
{
    JSXPathResult::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSXPathResultPrototype::self(ctx), JSXPathResult::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSXPathResult::finalizer(JSRuntime* rt, JSValue val)
{
    XPathResult* impl = (XPathResult*)JS_GetOpaque(val, JSXPathResult::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSXPathResult::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSXPathResult::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ResultTypeAttrNum: {
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            return JS_NewBigUint64(ctx, imp->resultType());
        }
        case NumberValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewBigUint64(ctx, imp->numberValue(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case StringValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewString(ctx, ((const String&)imp->stringValue(ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case BooleanValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewBool(ctx, imp->booleanValue(ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case SingleNodeValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->singleNodeValue(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case InvalidIteratorStateAttrNum: {
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            return JS_NewBool(ctx, imp->invalidIteratorState() ? 1 : 0);
        }
        case SnapshotLengthAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewBigUint64(ctx, imp->snapshotLength(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSXPathResult::getConstructor(JSContext *ctx)
{
    return JSXPathResultConstructor::self(ctx);
}

JSValue JSXPathResultPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    XPathResult* imp = (XPathResult*)JS_GetOpaque2(ctx, this_val, JSXPathResult::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSXPathResult::IterateNextFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->iterateNext(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSXPathResult::SnapshotItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, QJS::getPtr(imp->snapshotItem(index, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, XPathResult* obj)
{
    return QJS::cacheDOMObject<XPathResult, JSXPathResult>(ctx, obj);
}

XPathResult* toXPathResult(JSValue val)
{
    if (JS_IsObject(val)) {
        XPathResult* impl = (XPathResult*)JS_GetOpaque(val, JSXPathResult::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(XPATH)
