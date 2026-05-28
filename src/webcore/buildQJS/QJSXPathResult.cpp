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

static JSCFunctionListEntry JSXPathResultAttributesFunctions[8];
static bool JSXPathResultAttributesFunctions_initialized = false;

static void init_JSXPathResultAttributesFunctions()
{
    if (JSXPathResultAttributesFunctions_initialized) return;
    JSXPathResultAttributesFunctions_initialized = true;
    memset(JSXPathResultAttributesFunctions, 0, sizeof(JSXPathResultAttributesFunctions));
    JSXPathResultAttributesFunctions[0].name = "resultType";
    JSXPathResultAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[0].magic = JSXPathResult::ResultTypeAttrNum;
    JSXPathResultAttributesFunctions[0].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[1].name = "singleNodeValue";
    JSXPathResultAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[1].magic = JSXPathResult::SingleNodeValueAttrNum;
    JSXPathResultAttributesFunctions[1].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[2].name = "stringValue";
    JSXPathResultAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[2].magic = JSXPathResult::StringValueAttrNum;
    JSXPathResultAttributesFunctions[2].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[3].name = "booleanValue";
    JSXPathResultAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[3].magic = JSXPathResult::BooleanValueAttrNum;
    JSXPathResultAttributesFunctions[3].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[4].name = "numberValue";
    JSXPathResultAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[4].magic = JSXPathResult::NumberValueAttrNum;
    JSXPathResultAttributesFunctions[4].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[5].name = "invalidIteratorState";
    JSXPathResultAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[5].magic = JSXPathResult::InvalidIteratorStateAttrNum;
    JSXPathResultAttributesFunctions[5].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[6].name = "snapshotLength";
    JSXPathResultAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[6].magic = JSXPathResult::SnapshotLengthAttrNum;
    JSXPathResultAttributesFunctions[6].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSXPathResultAttributesFunctions[7].name = "constructor";
    JSXPathResultAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSXPathResultAttributesFunctions[7].magic = JSXPathResult::ConstructorAttrNum;
    JSXPathResultAttributesFunctions[7].u.getset.get.getter_magic = JSXPathResult::getValueProperty;
    JSXPathResultAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSXPathResultConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSXPathResultConstructorFunctions[10];
static bool JSXPathResultConstructorFunctions_initialized = false;

static void init_JSXPathResultConstructorFunctions()
{
    if (JSXPathResultConstructorFunctions_initialized) return;
    JSXPathResultConstructorFunctions_initialized = true;
    memset(JSXPathResultConstructorFunctions, 0, sizeof(JSXPathResultConstructorFunctions));
    JSXPathResultConstructorFunctions[0].name = "ANY_TYPE";
    JSXPathResultConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[0].u.i32 = (int32_t)XPathResult::ANY_TYPE;
    JSXPathResultConstructorFunctions[1].name = "NUMBER_TYPE";
    JSXPathResultConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[1].u.i32 = (int32_t)XPathResult::NUMBER_TYPE;
    JSXPathResultConstructorFunctions[2].name = "STRING_TYPE";
    JSXPathResultConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[2].u.i32 = (int32_t)XPathResult::STRING_TYPE;
    JSXPathResultConstructorFunctions[3].name = "BOOLEAN_TYPE";
    JSXPathResultConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[3].u.i32 = (int32_t)XPathResult::BOOLEAN_TYPE;
    JSXPathResultConstructorFunctions[4].name = "UNORDERED_NODE_ITERATOR_TYPE";
    JSXPathResultConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[4].u.i32 = (int32_t)XPathResult::UNORDERED_NODE_ITERATOR_TYPE;
    JSXPathResultConstructorFunctions[5].name = "ORDERED_NODE_ITERATOR_TYPE";
    JSXPathResultConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[5].u.i32 = (int32_t)XPathResult::ORDERED_NODE_ITERATOR_TYPE;
    JSXPathResultConstructorFunctions[6].name = "UNORDERED_NODE_SNAPSHOT_TYPE";
    JSXPathResultConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[6].u.i32 = (int32_t)XPathResult::UNORDERED_NODE_SNAPSHOT_TYPE;
    JSXPathResultConstructorFunctions[7].name = "ORDERED_NODE_SNAPSHOT_TYPE";
    JSXPathResultConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[7].u.i32 = (int32_t)XPathResult::ORDERED_NODE_SNAPSHOT_TYPE;
    JSXPathResultConstructorFunctions[8].name = "ANY_UNORDERED_NODE_TYPE";
    JSXPathResultConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[8].u.i32 = (int32_t)XPathResult::ANY_UNORDERED_NODE_TYPE;
    JSXPathResultConstructorFunctions[9].name = "FIRST_ORDERED_NODE_TYPE";
    JSXPathResultConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultConstructorFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSXPathResultConstructorFunctions[9].u.i32 = (int32_t)XPathResult::FIRST_ORDERED_NODE_TYPE;
}

JSValue JSXPathResultConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[XPathResult.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathResultConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[XPathResult.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSXPathResultConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSXPathResultConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultConstructorFunctions, countof(JSXPathResultConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSXPathResultPrototypeConstantsFunctions[10];
static bool JSXPathResultPrototypeConstantsFunctions_initialized = false;

static void init_JSXPathResultPrototypeConstantsFunctions()
{
    if (JSXPathResultPrototypeConstantsFunctions_initialized) return;
    JSXPathResultPrototypeConstantsFunctions_initialized = true;
    memset(JSXPathResultPrototypeConstantsFunctions, 0, sizeof(JSXPathResultPrototypeConstantsFunctions));
    JSXPathResultPrototypeConstantsFunctions[0].name = "ANY_TYPE";
    JSXPathResultPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[0].u.i32 = (int32_t)XPathResult::ANY_TYPE;
    JSXPathResultPrototypeConstantsFunctions[1].name = "NUMBER_TYPE";
    JSXPathResultPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[1].u.i32 = (int32_t)XPathResult::NUMBER_TYPE;
    JSXPathResultPrototypeConstantsFunctions[2].name = "STRING_TYPE";
    JSXPathResultPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[2].u.i32 = (int32_t)XPathResult::STRING_TYPE;
    JSXPathResultPrototypeConstantsFunctions[3].name = "BOOLEAN_TYPE";
    JSXPathResultPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[3].u.i32 = (int32_t)XPathResult::BOOLEAN_TYPE;
    JSXPathResultPrototypeConstantsFunctions[4].name = "UNORDERED_NODE_ITERATOR_TYPE";
    JSXPathResultPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[4].u.i32 = (int32_t)XPathResult::UNORDERED_NODE_ITERATOR_TYPE;
    JSXPathResultPrototypeConstantsFunctions[5].name = "ORDERED_NODE_ITERATOR_TYPE";
    JSXPathResultPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[5].u.i32 = (int32_t)XPathResult::ORDERED_NODE_ITERATOR_TYPE;
    JSXPathResultPrototypeConstantsFunctions[6].name = "UNORDERED_NODE_SNAPSHOT_TYPE";
    JSXPathResultPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[6].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[6].u.i32 = (int32_t)XPathResult::UNORDERED_NODE_SNAPSHOT_TYPE;
    JSXPathResultPrototypeConstantsFunctions[7].name = "ORDERED_NODE_SNAPSHOT_TYPE";
    JSXPathResultPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[7].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[7].u.i32 = (int32_t)XPathResult::ORDERED_NODE_SNAPSHOT_TYPE;
    JSXPathResultPrototypeConstantsFunctions[8].name = "ANY_UNORDERED_NODE_TYPE";
    JSXPathResultPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[8].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[8].u.i32 = (int32_t)XPathResult::ANY_UNORDERED_NODE_TYPE;
    JSXPathResultPrototypeConstantsFunctions[9].name = "FIRST_ORDERED_NODE_TYPE";
    JSXPathResultPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeConstantsFunctions[9].def_type = JS_DEF_PROP_INT32;
    JSXPathResultPrototypeConstantsFunctions[9].u.i32 = (int32_t)XPathResult::FIRST_ORDERED_NODE_TYPE;
}

/* Prototype functions table */

static JSCFunctionListEntry JSXPathResultPrototypeFunctions[2];
static bool JSXPathResultPrototypeFunctions_initialized = false;

static void init_JSXPathResultPrototypeFunctions()
{
    if (JSXPathResultPrototypeFunctions_initialized) return;
    JSXPathResultPrototypeFunctions_initialized = true;
    memset(JSXPathResultPrototypeFunctions, 0, sizeof(JSXPathResultPrototypeFunctions));
    JSXPathResultPrototypeFunctions[0].name = "snapshotItem";
    JSXPathResultPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSXPathResultPrototypeFunctions[0].magic = JSXPathResult::SnapshotItemFuncNum;
    JSXPathResultPrototypeFunctions[0].u.func.length = 1;
    JSXPathResultPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathResultPrototypeFunctions[0].u.func.cfunc.generic_magic = JSXPathResultPrototypeFunction::callAsFunction;
    JSXPathResultPrototypeFunctions[1].name = "iterateNext";
    JSXPathResultPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSXPathResultPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSXPathResultPrototypeFunctions[1].magic = JSXPathResult::IterateNextFuncNum;
    JSXPathResultPrototypeFunctions[1].u.func.length = 0;
    JSXPathResultPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSXPathResultPrototypeFunctions[1].u.func.cfunc.generic_magic = JSXPathResultPrototypeFunction::callAsFunction;
}

JSValue JSXPathResultPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSXPathResult.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSXPathResultPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSXPathResult.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSXPathResultPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSXPathResultAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultAttributesFunctions, countof(JSXPathResultAttributesFunctions));
    init_JSXPathResultPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultPrototypeConstantsFunctions, countof(JSXPathResultPrototypeConstantsFunctions));
    init_JSXPathResultPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSXPathResultPrototypeFunctions, countof(JSXPathResultPrototypeFunctions));
}

static JSClassDef JSXPathResultClassDefine;
static bool JSXPathResultClassDefine_initialized = false;

static void init_JSXPathResultClassDefine()
{
    if (JSXPathResultClassDefine_initialized) return;
    JSXPathResultClassDefine_initialized = true;
    memset(&JSXPathResultClassDefine, 0, sizeof(JSXPathResultClassDefine));
    JSXPathResultClassDefine.class_name = "XPathResult";
    JSXPathResultClassDefine.finalizer = JSXPathResult::finalizer;
    JSXPathResultClassDefine.gc_mark = JSXPathResult::mark;
}

JSClassID JSXPathResult::js_class_id = 0;

void JSXPathResult::init(JSContext* ctx)
{
    if (JSXPathResult::js_class_id == 0) {
        init_JSXPathResultClassDefine();
        JS_NewClassID(&JSXPathResult::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSXPathResult::js_class_id, &JSXPathResultClassDefine);
        JS_SetConstructor(ctx, JSXPathResultConstructor::self(ctx), JSXPathResultPrototype::self(ctx));
        JS_SetClassProto(ctx, JSXPathResult::js_class_id, JSXPathResultPrototype::self(ctx));
    }
}

JSValue JSXPathResult::create(JSContext* ctx, XPathResult* impl)
{
    JSXPathResult::init(ctx);
    JSValue _proto = JSXPathResultPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSXPathResult::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            return JS_NewInt32(ctx, imp->resultType());
        }
        case NumberValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewInt32(ctx, imp->numberValue(ec));
            setDOMException(ctx, ec);
            return result;
        }
        case StringValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewString(ctx, ((const String&)imp->stringValue(ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case BooleanValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewBool(ctx, imp->booleanValue(ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case SingleNodeValueAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            JSValue result = toJS(ctx, QJS::getPtr(imp->singleNodeValue(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case InvalidIteratorStateAttrNum: {
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            return JS_NewBool(ctx, imp->invalidIteratorState() ? 1 : 0);
        }
        case SnapshotLengthAttrNum: {
            ExceptionCode ec = 0;
            XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
            JSValue result = JS_NewInt32(ctx, imp->snapshotLength(ec));
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
    XPathResult* imp = (XPathResult*)JS_GetOpaque(this_val, JSXPathResult::js_class_id);
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
