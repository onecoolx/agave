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

#include "QJSCharacterData.h"

#include "CharacterData.h"
#include "ExceptionCode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSCharacterDataAttributesFunctions[3];
static bool JSCharacterDataAttributesFunctions_initialized = false;

static void init_JSCharacterDataAttributesFunctions()
{
    if (JSCharacterDataAttributesFunctions_initialized) return;
    JSCharacterDataAttributesFunctions_initialized = true;
    memset(JSCharacterDataAttributesFunctions, 0, sizeof(JSCharacterDataAttributesFunctions));
    JSCharacterDataAttributesFunctions[0].name = "data";
    JSCharacterDataAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSCharacterDataAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSCharacterDataAttributesFunctions[0].magic = JSCharacterData::DataAttrNum;
    JSCharacterDataAttributesFunctions[0].u.getset.get.getter_magic = JSCharacterData::getValueProperty;
    JSCharacterDataAttributesFunctions[0].u.getset.set.setter_magic = JSCharacterData::putValueProperty;
    JSCharacterDataAttributesFunctions[1].name = "length";
    JSCharacterDataAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSCharacterDataAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSCharacterDataAttributesFunctions[1].magic = JSCharacterData::LengthAttrNum;
    JSCharacterDataAttributesFunctions[1].u.getset.get.getter_magic = JSCharacterData::getValueProperty;
    JSCharacterDataAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSCharacterDataAttributesFunctions[2].name = "constructor";
    JSCharacterDataAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSCharacterDataAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSCharacterDataAttributesFunctions[2].magic = JSCharacterData::ConstructorAttrNum;
    JSCharacterDataAttributesFunctions[2].u.getset.get.getter_magic = JSCharacterData::getValueProperty;
    JSCharacterDataAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSCharacterDataConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[CharacterData.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCharacterDataConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSCharacterDataPrototypeFunctions[5];
static bool JSCharacterDataPrototypeFunctions_initialized = false;

static void init_JSCharacterDataPrototypeFunctions()
{
    if (JSCharacterDataPrototypeFunctions_initialized) return;
    JSCharacterDataPrototypeFunctions_initialized = true;
    memset(JSCharacterDataPrototypeFunctions, 0, sizeof(JSCharacterDataPrototypeFunctions));
    JSCharacterDataPrototypeFunctions[0].name = "appendData";
    JSCharacterDataPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCharacterDataPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSCharacterDataPrototypeFunctions[0].magic = JSCharacterData::AppendDataFuncNum;
    JSCharacterDataPrototypeFunctions[0].u.func.length = 1;
    JSCharacterDataPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSCharacterDataPrototypeFunctions[0].u.func.cfunc.generic_magic = JSCharacterDataPrototypeFunction::callAsFunction;
    JSCharacterDataPrototypeFunctions[1].name = "insertData";
    JSCharacterDataPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCharacterDataPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSCharacterDataPrototypeFunctions[1].magic = JSCharacterData::InsertDataFuncNum;
    JSCharacterDataPrototypeFunctions[1].u.func.length = 2;
    JSCharacterDataPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSCharacterDataPrototypeFunctions[1].u.func.cfunc.generic_magic = JSCharacterDataPrototypeFunction::callAsFunction;
    JSCharacterDataPrototypeFunctions[2].name = "substringData";
    JSCharacterDataPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCharacterDataPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSCharacterDataPrototypeFunctions[2].magic = JSCharacterData::SubstringDataFuncNum;
    JSCharacterDataPrototypeFunctions[2].u.func.length = 2;
    JSCharacterDataPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSCharacterDataPrototypeFunctions[2].u.func.cfunc.generic_magic = JSCharacterDataPrototypeFunction::callAsFunction;
    JSCharacterDataPrototypeFunctions[3].name = "deleteData";
    JSCharacterDataPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCharacterDataPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSCharacterDataPrototypeFunctions[3].magic = JSCharacterData::DeleteDataFuncNum;
    JSCharacterDataPrototypeFunctions[3].u.func.length = 2;
    JSCharacterDataPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSCharacterDataPrototypeFunctions[3].u.func.cfunc.generic_magic = JSCharacterDataPrototypeFunction::callAsFunction;
    JSCharacterDataPrototypeFunctions[4].name = "replaceData";
    JSCharacterDataPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSCharacterDataPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSCharacterDataPrototypeFunctions[4].magic = JSCharacterData::ReplaceDataFuncNum;
    JSCharacterDataPrototypeFunctions[4].u.func.length = 3;
    JSCharacterDataPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSCharacterDataPrototypeFunctions[4].u.func.cfunc.generic_magic = JSCharacterDataPrototypeFunction::callAsFunction;
}

JSValue JSCharacterDataPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSCharacterData.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSEventTargetNodePrototype::self(ctx));
        JSCharacterDataPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSCharacterData.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSCharacterDataPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSCharacterDataAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCharacterDataAttributesFunctions, countof(JSCharacterDataAttributesFunctions));
    init_JSCharacterDataPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSCharacterDataPrototypeFunctions, countof(JSCharacterDataPrototypeFunctions));
}

static JSClassDef JSCharacterDataClassDefine;
static bool JSCharacterDataClassDefine_initialized = false;

static void init_JSCharacterDataClassDefine()
{
    if (JSCharacterDataClassDefine_initialized) return;
    JSCharacterDataClassDefine_initialized = true;
    memset(&JSCharacterDataClassDefine, 0, sizeof(JSCharacterDataClassDefine));
    JSCharacterDataClassDefine.class_name = "CharacterData";
    JSCharacterDataClassDefine.finalizer = JSCharacterData::finalizer;
    JSCharacterDataClassDefine.gc_mark = JSCharacterData::mark;
}

JSClassID JSCharacterData::js_class_id = 0;

void JSCharacterData::init(JSContext* ctx)
{
    if (JSCharacterData::js_class_id == 0) {
        JSNode::init(ctx);
        JSCharacterData::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSCharacterData::create(JSContext* ctx, CharacterData* impl)
{
    JSCharacterData::init(ctx);
    JSValue _proto = JSCharacterDataPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSCharacterData::finalizer(JSRuntime* rt, JSValue val)
{
    CharacterData* impl = (CharacterData*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            CharacterData* imp = (CharacterData*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->data()).utf8().data());
        }
        case LengthAttrNum: {
            CharacterData* imp = (CharacterData*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewInt32(ctx, imp->length());
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
            CharacterData* imp = (CharacterData*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
    CharacterData* imp = (CharacterData*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
