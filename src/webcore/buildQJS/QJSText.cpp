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

#include "QJSText.h"

#include "ExceptionCode.h"
#include "QJSText.h"
#include "Text.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSTextAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSText::getValueProperty, NULL, JSText::ConstructorAttrNum)
};

class JSTextConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSTextConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSTextConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[Text.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSTextConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[Text.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSTextConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSTextPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("splitText", 1, JSTextPrototypeFunction::callAsFunction, JSText::SplitTextFuncNum)
};

JSValue JSTextPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSText.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSCharacterDataPrototype::self(ctx));
        JSTextPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSText.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSTextPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSTextAttributesFunctions, countof(JSTextAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSTextPrototypeFunctions, countof(JSTextPrototypeFunctions));
}

static JSClassDef JSTextClassDefine = 
{
    "Text",
    .finalizer = JSText::finalizer,
    .gc_mark = JSText::mark,
};

JSClassID JSText::js_class_id = 0;

void JSText::init(JSContext* ctx)
{
    if (JSText::js_class_id == 0) {
        JS_NewClassID(&JSText::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSText::js_class_id, &JSTextClassDefine);
        JS_SetConstructor(ctx, JSTextConstructor::self(ctx), JSTextPrototype::self(ctx));
        JS_SetClassProto(ctx, JSText::js_class_id, JSTextPrototype::self(ctx));
    }
}

JSValue JSText::create(JSContext* ctx, Text* impl)
{
    JSText::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSTextPrototype::self(ctx), JSText::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSText::finalizer(JSRuntime* rt, JSValue val)
{
    Text* impl = (Text*)JS_GetOpaque(val, JSText::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSText::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSCharacterData::mark(rt, val, mark_func);
}

JSValue JSText::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSText::getConstructor(JSContext *ctx)
{
    return JSTextConstructor::self(ctx);
}

JSValue JSTextPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    Text* imp = (Text*)JS_GetOpaque2(ctx, this_val, JSText::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSText::SplitTextFuncNum: {
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

            JSValue result = toJS(ctx, QJS::getPtr(imp->splitText(offset, ec)));
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}


}
