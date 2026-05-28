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

#include "QJSNodeIterator.h"

#include "Node.h"
#include "NodeFilter.h"
#include "NodeIterator.h"
#include "QJSNode.h"
#include "QJSNodeFilter.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSNodeIteratorAttributesFunctions[6];
static bool JSNodeIteratorAttributesFunctions_initialized = false;

static void init_JSNodeIteratorAttributesFunctions()
{
    if (JSNodeIteratorAttributesFunctions_initialized) return;
    JSNodeIteratorAttributesFunctions_initialized = true;
    memset(JSNodeIteratorAttributesFunctions, 0, sizeof(JSNodeIteratorAttributesFunctions));
    JSNodeIteratorAttributesFunctions[0].name = "root";
    JSNodeIteratorAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[0].magic = JSNodeIterator::RootAttrNum;
    JSNodeIteratorAttributesFunctions[0].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSNodeIteratorAttributesFunctions[1].name = "whatToShow";
    JSNodeIteratorAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[1].magic = JSNodeIterator::WhatToShowAttrNum;
    JSNodeIteratorAttributesFunctions[1].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSNodeIteratorAttributesFunctions[2].name = "filter";
    JSNodeIteratorAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[2].magic = JSNodeIterator::FilterAttrNum;
    JSNodeIteratorAttributesFunctions[2].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSNodeIteratorAttributesFunctions[3].name = "expandEntityReferences";
    JSNodeIteratorAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[3].magic = JSNodeIterator::ExpandEntityReferencesAttrNum;
    JSNodeIteratorAttributesFunctions[3].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSNodeIteratorAttributesFunctions[4].name = "referenceNode";
    JSNodeIteratorAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[4].magic = JSNodeIterator::ReferenceNodeAttrNum;
    JSNodeIteratorAttributesFunctions[4].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSNodeIteratorAttributesFunctions[5].name = "pointerBeforeReferenceNode";
    JSNodeIteratorAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSNodeIteratorAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSNodeIteratorAttributesFunctions[5].magic = JSNodeIterator::PointerBeforeReferenceNodeAttrNum;
    JSNodeIteratorAttributesFunctions[5].u.getset.get.getter_magic = JSNodeIterator::getValueProperty;
    JSNodeIteratorAttributesFunctions[5].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSNodeIteratorPrototypeFunctions[3];
static bool JSNodeIteratorPrototypeFunctions_initialized = false;

static void init_JSNodeIteratorPrototypeFunctions()
{
    if (JSNodeIteratorPrototypeFunctions_initialized) return;
    JSNodeIteratorPrototypeFunctions_initialized = true;
    memset(JSNodeIteratorPrototypeFunctions, 0, sizeof(JSNodeIteratorPrototypeFunctions));
    JSNodeIteratorPrototypeFunctions[0].name = "nextNode";
    JSNodeIteratorPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodeIteratorPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSNodeIteratorPrototypeFunctions[0].magic = JSNodeIterator::NextNodeFuncNum;
    JSNodeIteratorPrototypeFunctions[0].u.func.length = 0;
    JSNodeIteratorPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodeIteratorPrototypeFunctions[0].u.func.cfunc.generic_magic = JSNodeIteratorPrototypeFunction::callAsFunction;
    JSNodeIteratorPrototypeFunctions[1].name = "previousNode";
    JSNodeIteratorPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodeIteratorPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSNodeIteratorPrototypeFunctions[1].magic = JSNodeIterator::PreviousNodeFuncNum;
    JSNodeIteratorPrototypeFunctions[1].u.func.length = 0;
    JSNodeIteratorPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodeIteratorPrototypeFunctions[1].u.func.cfunc.generic_magic = JSNodeIteratorPrototypeFunction::callAsFunction;
    JSNodeIteratorPrototypeFunctions[2].name = "detach";
    JSNodeIteratorPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSNodeIteratorPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSNodeIteratorPrototypeFunctions[2].magic = JSNodeIterator::DetachFuncNum;
    JSNodeIteratorPrototypeFunctions[2].u.func.length = 0;
    JSNodeIteratorPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSNodeIteratorPrototypeFunctions[2].u.func.cfunc.generic_magic = JSNodeIteratorPrototypeFunction::callAsFunction;
}

JSValue JSNodeIteratorPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeIterator.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeIteratorPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeIterator.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSNodeIteratorPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSNodeIteratorAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeIteratorAttributesFunctions, countof(JSNodeIteratorAttributesFunctions));
    init_JSNodeIteratorPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeIteratorPrototypeFunctions, countof(JSNodeIteratorPrototypeFunctions));
}

static JSClassDef JSNodeIteratorClassDefine;
static bool JSNodeIteratorClassDefine_initialized = false;

static void init_JSNodeIteratorClassDefine()
{
    if (JSNodeIteratorClassDefine_initialized) return;
    JSNodeIteratorClassDefine_initialized = true;
    memset(&JSNodeIteratorClassDefine, 0, sizeof(JSNodeIteratorClassDefine));
    JSNodeIteratorClassDefine.class_name = "NodeIterator";
    JSNodeIteratorClassDefine.finalizer = JSNodeIterator::finalizer;
    JSNodeIteratorClassDefine.gc_mark = JSNodeIterator::mark;
}

JSClassID JSNodeIterator::js_class_id = 0;

void JSNodeIterator::init(JSContext* ctx)
{
    if (JSNodeIterator::js_class_id == 0) {
        init_JSNodeIteratorClassDefine();
        JS_NewClassID(&JSNodeIterator::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeIterator::js_class_id, &JSNodeIteratorClassDefine);
        JS_SetClassProto(ctx, JSNodeIterator::js_class_id, JSNodeIteratorPrototype::self(ctx));
    }
}

JSValue JSNodeIterator::create(JSContext* ctx, NodeIterator* impl)
{
    JSNodeIterator::init(ctx);
    JSValue _proto = JSNodeIteratorPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNodeIterator::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSNodeIterator::finalizer(JSRuntime* rt, JSValue val)
{
    NodeIterator* impl = (NodeIterator*)JS_GetOpaque(val, JSNodeIterator::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSNodeIterator::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RootAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->root()));
        }
        case WhatToShowAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return JS_NewInt32(ctx, imp->whatToShow());
        }
        case FilterAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->filter()));
        }
        case ExpandEntityReferencesAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return JS_NewBool(ctx, imp->expandEntityReferences() ? 1 : 0);
        }
        case ReferenceNodeAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->referenceNode()));
        }
        case PointerBeforeReferenceNodeAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
            return JS_NewBool(ctx, imp->pointerBeforeReferenceNode() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSNodeIteratorPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    NodeIterator* imp = (NodeIterator*)JS_GetOpaque(this_val, JSNodeIterator::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSNodeIterator::NextNodeFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->nextNode(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSNodeIterator::PreviousNodeFuncNum: {
            ExceptionCode ec = 0;

            JSValue result = toJS(ctx, QJS::getPtr(imp->previousNode(ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSNodeIterator::DetachFuncNum: {
            imp->detach();
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, NodeIterator* obj)
{
    return QJS::cacheDOMObject<NodeIterator, JSNodeIterator>(ctx, obj);
}

NodeIterator* toNodeIterator(JSValue val)
{
    if (JS_IsObject(val)) {
        NodeIterator* impl = (NodeIterator*)JS_GetOpaque(val, JSNodeIterator::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
