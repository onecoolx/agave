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

#include "QJSProcessingInstruction.h"

#include "ProcessingInstruction.h"
#include "QJSStyleSheet.h"
#include "StyleSheet.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSProcessingInstructionAttributesFunctions[4];
static bool JSProcessingInstructionAttributesFunctions_initialized = false;

static void init_JSProcessingInstructionAttributesFunctions()
{
    if (JSProcessingInstructionAttributesFunctions_initialized) return;
    JSProcessingInstructionAttributesFunctions_initialized = true;
    memset(JSProcessingInstructionAttributesFunctions, 0, sizeof(JSProcessingInstructionAttributesFunctions));
    JSProcessingInstructionAttributesFunctions[0].name = "target";
    JSProcessingInstructionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSProcessingInstructionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSProcessingInstructionAttributesFunctions[0].magic = JSProcessingInstruction::TargetAttrNum;
    JSProcessingInstructionAttributesFunctions[0].u.getset.get.getter_magic = JSProcessingInstruction::getValueProperty;
    JSProcessingInstructionAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSProcessingInstructionAttributesFunctions[1].name = "data";
    JSProcessingInstructionAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSProcessingInstructionAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSProcessingInstructionAttributesFunctions[1].magic = JSProcessingInstruction::DataAttrNum;
    JSProcessingInstructionAttributesFunctions[1].u.getset.get.getter_magic = JSProcessingInstruction::getValueProperty;
    JSProcessingInstructionAttributesFunctions[1].u.getset.set.setter_magic = JSProcessingInstruction::putValueProperty;
    JSProcessingInstructionAttributesFunctions[2].name = "sheet";
    JSProcessingInstructionAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSProcessingInstructionAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSProcessingInstructionAttributesFunctions[2].magic = JSProcessingInstruction::SheetAttrNum;
    JSProcessingInstructionAttributesFunctions[2].u.getset.get.getter_magic = JSProcessingInstruction::getValueProperty;
    JSProcessingInstructionAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSProcessingInstructionAttributesFunctions[3].name = "constructor";
    JSProcessingInstructionAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSProcessingInstructionAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSProcessingInstructionAttributesFunctions[3].magic = JSProcessingInstruction::ConstructorAttrNum;
    JSProcessingInstructionAttributesFunctions[3].u.getset.get.getter_magic = JSProcessingInstruction::getValueProperty;
    JSProcessingInstructionAttributesFunctions[3].u.getset.set.setter_magic = NULL;
}

class JSProcessingInstructionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSProcessingInstructionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[ProcessingInstruction.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSProcessingInstructionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[ProcessingInstruction.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSProcessingInstructionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSProcessingInstructionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSProcessingInstruction.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSProcessingInstructionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSProcessingInstruction.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSProcessingInstructionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSProcessingInstructionAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSProcessingInstructionAttributesFunctions, countof(JSProcessingInstructionAttributesFunctions));
}

static JSClassDef JSProcessingInstructionClassDefine;
static bool JSProcessingInstructionClassDefine_initialized = false;

static void init_JSProcessingInstructionClassDefine()
{
    if (JSProcessingInstructionClassDefine_initialized) return;
    JSProcessingInstructionClassDefine_initialized = true;
    memset(&JSProcessingInstructionClassDefine, 0, sizeof(JSProcessingInstructionClassDefine));
    JSProcessingInstructionClassDefine.class_name = "ProcessingInstruction";
    JSProcessingInstructionClassDefine.finalizer = JSProcessingInstruction::finalizer;
    JSProcessingInstructionClassDefine.gc_mark = JSProcessingInstruction::mark;
}

JSClassID JSProcessingInstruction::js_class_id = 0;

void JSProcessingInstruction::init(JSContext* ctx)
{
    if (JSProcessingInstruction::js_class_id == 0) {
        JSNode::init(ctx);
        JSProcessingInstruction::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSProcessingInstruction::create(JSContext* ctx, ProcessingInstruction* impl)
{
    JSProcessingInstruction::init(ctx);
    JSValue _proto = JSProcessingInstructionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSProcessingInstruction::finalizer(JSRuntime* rt, JSValue val)
{
    ProcessingInstruction* impl = (ProcessingInstruction*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
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
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->target());
        }
        case DataAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->data());
        }
        case SheetAttrNum: {
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
            ProcessingInstruction* imp = (ProcessingInstruction*)JS_GetOpaque(this_val, JSNode::js_class_id);
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
