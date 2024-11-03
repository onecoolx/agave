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

#include "QJSProcessingInstruction.h"

#include "ProcessingInstruction.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSProcessingInstructionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSProcessingInstruction::getValueProperty, NULL, JSProcessingInstruction::ConstructorAttrNum),
    JS_CGETSET_MAGIC_DEF("target", JSProcessingInstruction::getValueProperty, NULL, JSProcessingInstruction::TargetAttrNum),
    JS_CGETSET_MAGIC_DEF("data", JSProcessingInstruction::getValueProperty, JSProcessingInstruction::putValueProperty, JSProcessingInstruction::DataAttrNum),
    JS_CGETSET_MAGIC_DEF("sheet", JSProcessingInstruction::getValueProperty, NULL, JSProcessingInstruction::SheetAttrNum)
};

class JSProcessingInstructionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSProcessingInstructionConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSProcessingInstructionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[ProcessingInstruction.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSProcessingInstructionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[ProcessingInstruction.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSProcessingInstructionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSProcessingInstructionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSProcessingInstruction.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSProcessingInstructionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSProcessingInstruction.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSProcessingInstructionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSProcessingInstructionAttributesFunctions, countof(JSProcessingInstructionAttributesFunctions));
}

static JSClassDef JSProcessingInstructionClassDefine = 
{
    "ProcessingInstruction",
    .finalizer = JSProcessingInstruction::finalizer,
    .gc_mark = JSProcessingInstruction::mark,
};

JSClassID JSProcessingInstruction::js_class_id = 0;

void JSProcessingInstruction::init(JSContext* ctx)
{
    if (JSProcessingInstruction::js_class_id == 0) {
        JS_NewClassID(&JSProcessingInstruction::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSProcessingInstruction::js_class_id, &JSProcessingInstructionClassDefine);
        JS_SetConstructor(ctx, JSProcessingInstructionConstructor::self(ctx), JSProcessingInstructionPrototype::self(ctx));
        JS_SetClassProto(ctx, JSProcessingInstruction::js_class_id, JSProcessingInstructionPrototype::self(ctx));
    }
}

JSValue JSProcessingInstruction::create(JSContext* ctx, ProcessingInstruction* impl)
{
    JSProcessingInstruction::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSProcessingInstructionPrototype::self(ctx), JSProcessingInstruction::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSProcessingInstruction::finalizer(JSRuntime* rt, JSValue val)
{
    ProcessingInstruction* impl = (ProcessingInstruction*)JS_GetOpaque(val, JSProcessingInstruction::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSProcessingInstruction::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSNode::mark(rt, val, mark_func);
}

JSValue JSProcessingInstruction::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TargetAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque2(ctx, this_val, JSProcessingInstruction::js_class_id);
            return jsStringOrNull(ctx, imp->target());
        }
        case DataAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque2(ctx, this_val, JSProcessingInstruction::js_class_id);
            return jsStringOrNull(ctx, imp->data());
        }
        case SheetAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque2(ctx, this_val, JSProcessingInstruction::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->sheet()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSProcessingInstruction::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case DataAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque2(ctx, this_val, JSProcessingInstruction::js_class_id);
            ExceptionCode ec = 0;
            imp->setData(valueToStringWithNullCheck(ctx, value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSProcessingInstruction::getConstructor(JSContext *ctx)
{
    return JSProcessingInstructionConstructor::self(ctx);
}


}
