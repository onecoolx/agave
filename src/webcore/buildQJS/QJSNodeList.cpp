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

#include "QJSNodeList.h"

#include "ExceptionCode.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSNode.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSNodeListAttributesFunctions[2];
static bool JSNodeListAttributesFunctions_initialized = false;

static void init_JSNodeListAttributesFunctions()
{
    if (JSNodeListAttributesFunctions_initialized) return;
    JSNodeListAttributesFunctions_initialized = true;
    memset(JSNodeListAttributesFunctions, 0, sizeof(JSNodeListAttributesFunctions));
    JSNodeListAttributesFunctions[0].name = "length";
    JSNodeListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeListAttributesFunctions[0].magic = JSNodeList::LengthAttrNum;
    JSNodeListAttributesFunctions[0].u.getset.get.getter_magic = JSNodeList::getValueProperty;
    JSNodeListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSNodeListAttributesFunctions[1].name = "constructor";
    JSNodeListAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeListAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeListAttributesFunctions[1].magic = JSNodeList::ConstructorAttrNum;
    JSNodeListAttributesFunctions[1].u.getset.get.getter_magic = JSNodeList::getValueProperty;
    JSNodeListAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeListConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[NodeList.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeListConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSNodeListPrototypeFunctions[1];
static bool JSNodeListPrototypeFunctions_initialized = false;

static void init_JSNodeListPrototypeFunctions()
{
    if (JSNodeListPrototypeFunctions_initialized) return;
    JSNodeListPrototypeFunctions_initialized = true;
    memset(JSNodeListPrototypeFunctions, 0, sizeof(JSNodeListPrototypeFunctions));
    JSNodeListPrototypeFunctions[0].name = "item";
    JSNodeListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodeListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSNodeListPrototypeFunctions[0].magic = JSNodeList::ItemFuncNum;
    JSNodeListPrototypeFunctions[0].u.func.length = 1;
    JSNodeListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodeListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSNodeListPrototypeFunction::callAsFunction;
}

JSValue JSNodeListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeListAttributesFunctions, countof(JSNodeListAttributesFunctions));
    init_JSNodeListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeListPrototypeFunctions, countof(JSNodeListPrototypeFunctions));
}

static int js_nodelist_get_own_property(JSContext *ctx, JSPropertyDescriptor *desc,
                                         JSValueConst obj, JSAtom prop)
{
    NodeList* impl = (NodeList*)JS_GetOpaque(obj, JSNodeList::js_class_id);
    if (!impl)
        return 0;
    const char* str = JS_AtomToCString(ctx, prop);
    if (!str)
        return 0;
    char* end;
    unsigned long index = strtoul(str, &end, 10);
    int is_index = (*end == '\0' && str[0] != '\0');
    JS_FreeCString(ctx, str);
    if (is_index && index < impl->length()) {
        if (desc) {
            desc->flags = JS_PROP_ENUMERABLE;
            desc->value = toJS(ctx, impl->item(index));
            desc->getter = JS_UNDEFINED;
            desc->setter = JS_UNDEFINED;
        }
        return 1;
    }
    return 0;
}

static JSClassExoticMethods js_nodelist_exotic;
static bool js_nodelist_exotic_initialized = false;

static void init_js_nodelist_exotic()
{
    if (js_nodelist_exotic_initialized) return;
    js_nodelist_exotic_initialized = true;
    memset(&js_nodelist_exotic, 0, sizeof(js_nodelist_exotic));
    js_nodelist_exotic.get_own_property = js_nodelist_get_own_property;
}

static JSClassDef JSNodeListClassDefine;
static bool JSNodeListClassDefine_initialized = false;

static void init_JSNodeListClassDefine()
{
    if (JSNodeListClassDefine_initialized) return;
    JSNodeListClassDefine_initialized = true;
    init_js_nodelist_exotic();
    memset(&JSNodeListClassDefine, 0, sizeof(JSNodeListClassDefine));
    JSNodeListClassDefine.class_name = "NodeList";
    JSNodeListClassDefine.finalizer = JSNodeList::finalizer;
    JSNodeListClassDefine.gc_mark = JSNodeList::mark;
    JSNodeListClassDefine.exotic = &js_nodelist_exotic;
}

JSClassID JSNodeList::js_class_id = 0;

void JSNodeList::init(JSContext* ctx)
{
    if (JSNodeList::js_class_id == 0) {
        init_JSNodeListClassDefine();
        JS_NewClassID(&JSNodeList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeList::js_class_id, &JSNodeListClassDefine);
        JS_SetConstructor(ctx, JSNodeListConstructor::self(ctx), JSNodeListPrototype::self(ctx));
        JS_SetClassProto(ctx, JSNodeList::js_class_id, JSNodeListPrototype::self(ctx));
    }
}

JSValue JSNodeList::create(JSContext* ctx, NodeList* impl)
{
    JSNodeList::init(ctx);
    JSValue _proto = JSNodeListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNodeList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            NodeList* imp = (NodeList*)JS_GetOpaque(this_val, JSNodeList::js_class_id);
            return JS_NewInt32(ctx, imp->length());
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
    NodeList* imp = (NodeList*)JS_GetOpaque(this_val, JSNodeList::js_class_id);
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
