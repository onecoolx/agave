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

#include "QJSCharacterData.h"

#include "CharacterData.h"
#include "ExceptionCode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSCharacterDataAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("data", JSCharacterData::getValueProperty, JSCharacterData::putValueProperty, JSCharacterData::DataAttrNum),
    JS_CGETSET_MAGIC_DEF("length", JSCharacterData::getValueProperty, NULL, JSCharacterData::LengthAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSCharacterData::getValueProperty, NULL, JSCharacterData::ConstructorAttrNum)
};

class JSCharacterDataConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSCharacterDataConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSCharacterDataConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[CharacterData.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSCharacterDataConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CharacterData.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCharacterDataConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSCharacterDataPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("appendData", 1, JSCharacterDataPrototypeFunction::callAsFunction, JSCharacterData::AppendDataFuncNum),
    JS_CFUNC_MAGIC_DEF("insertData", 2, JSCharacterDataPrototypeFunction::callAsFunction, JSCharacterData::InsertDataFuncNum),
    JS_CFUNC_MAGIC_DEF("substringData", 2, JSCharacterDataPrototypeFunction::callAsFunction, JSCharacterData::SubstringDataFuncNum),
    JS_CFUNC_MAGIC_DEF("deleteData", 2, JSCharacterDataPrototypeFunction::callAsFunction, JSCharacterData::DeleteDataFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceData", 3, JSCharacterDataPrototypeFunction::callAsFunction, JSCharacterData::ReplaceDataFuncNum)
};

JSValue JSCharacterDataPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCharacterData.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSCharacterDataPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCharacterData.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSCharacterDataPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSCharacterDataAttributesFunctions, countof(JSCharacterDataAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSCharacterDataPrototypeFunctions, countof(JSCharacterDataPrototypeFunctions));
}

static JSClassDef JSCharacterDataClassDefine = 
{
    "CharacterData",
    .finalizer = JSCharacterData::finalizer,
    .gc_mark = JSCharacterData::mark,
};

JSClassID JSCharacterData::js_class_id = 0;

void JSCharacterData::init(JSContext* ctx)
{
    if (JSCharacterData::js_class_id == 0) {
        JS_NewClassID(&JSCharacterData::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSCharacterData::js_class_id, &JSCharacterDataClassDefine);
        JS_SetConstructor(ctx, JSCharacterDataConstructor::self(ctx), JSCharacterDataPrototype::self(ctx));
        JS_SetClassProto(ctx, JSCharacterData::js_class_id, JSCharacterDataPrototype::self(ctx));
    }
}

JSValue JSCharacterData::create(JSContext* ctx, CharacterData* impl)
{
    JSCharacterData::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSCharacterDataPrototype::self(ctx), JSCharacterData::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCharacterData::finalizer(JSRuntime* rt, JSValue val)
{
    CharacterData* impl = (CharacterData*)JS_GetOpaque(val, JSCharacterData::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSCharacterData::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSEventTargetNode::mark(rt, val, mark_func);
}

JSValue JSCharacterData::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case DataAttrNum: {
            CharacterData* imp = (CharacterData*)JS_GetOpaque2(ctx, this_val, JSCharacterData::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->data()).utf8().data());
        }
        case LengthAttrNum: {
            CharacterData* imp = (CharacterData*)JS_GetOpaque2(ctx, this_val, JSCharacterData::js_class_id);
            return JS_NewBigUint64(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSCharacterData::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DataAttrNum: {
            CharacterData* imp = (CharacterData*)JS_GetOpaque2(ctx, this_val, JSCharacterData::js_class_id);
            ExceptionCode ec = 0;
            imp->setData(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSCharacterData::getConstructor(JSContext *ctx)
{
    return JSCharacterDataConstructor::self(ctx);
}

JSValue JSCharacterDataPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    CharacterData* imp = (CharacterData*)JS_GetOpaque2(ctx, this_val, JSCharacterData::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSCharacterData::SubstringDataFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (offset < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            int length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (length < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = jsStringOrNull(ctx, imp->substringData(offset, length, ec));
            setDOMException(ctx, ec);
            return result;
        }
        case JSCharacterData::AppendDataFuncNum: {
            ExceptionCode ec = 0;
            String data = valueToString(ctx, argv[0]);
            imp->appendData(data, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCharacterData::InsertDataFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (offset < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            String data = valueToString(ctx, argv[1]);
            imp->insertData(offset, data, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCharacterData::DeleteDataFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (offset < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            int length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (length < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            imp->deleteData(offset, length, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSCharacterData::ReplaceDataFuncNum: {
            ExceptionCode ec = 0;
            bool offsetOk;
            int offset = valueToInt32(ctx, argv[0], offsetOk);
            if (!offsetOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (offset < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            bool lengthOk;
            int length = valueToInt32(ctx, argv[1], lengthOk);
            if (!lengthOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            if (length < 0) {
                setDOMException(ctx, INDEX_SIZE_ERR);
                return JS_UNDEFINED;
            }
            String data = valueToString(ctx, argv[2]);
            imp->replaceData(offset, length, data, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


}
