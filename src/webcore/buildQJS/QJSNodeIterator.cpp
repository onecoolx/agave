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

static const JSCFunctionListEntry JSNodeIteratorAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("root", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::RootAttrNum),
    JS_CGETSET_MAGIC_DEF("whatToShow", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::WhatToShowAttrNum),
    JS_CGETSET_MAGIC_DEF("referenceNode", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::ReferenceNodeAttrNum),
    JS_CGETSET_MAGIC_DEF("filter", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::FilterAttrNum),
    JS_CGETSET_MAGIC_DEF("expandEntityReferences", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::ExpandEntityReferencesAttrNum),
    JS_CGETSET_MAGIC_DEF("pointerBeforeReferenceNode", JSNodeIterator::getValueProperty, NULL, JSNodeIterator::PointerBeforeReferenceNodeAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSNodeIteratorPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("nextNode", 0, JSNodeIteratorPrototypeFunction::callAsFunction, JSNodeIterator::NextNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("previousNode", 0, JSNodeIteratorPrototypeFunction::callAsFunction, JSNodeIterator::PreviousNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("detach", 0, JSNodeIteratorPrototypeFunction::callAsFunction, JSNodeIterator::DetachFuncNum)
};

JSValue JSNodeIteratorPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSNodeIterator.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSNodeIteratorPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSNodeIterator.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSNodeIteratorPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeIteratorAttributesFunctions, countof(JSNodeIteratorAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSNodeIteratorPrototypeFunctions, countof(JSNodeIteratorPrototypeFunctions));
}

static JSClassDef JSNodeIteratorClassDefine = 
{
    "NodeIterator",
    .finalizer = JSNodeIterator::finalizer,
    .gc_mark = JSNodeIterator::mark,
};

JSClassID JSNodeIterator::js_class_id = 0;

void JSNodeIterator::init(JSContext* ctx)
{
    if (JSNodeIterator::js_class_id == 0) {
        JS_NewClassID(&JSNodeIterator::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSNodeIterator::js_class_id, &JSNodeIteratorClassDefine);
        JS_SetClassProto(ctx, JSNodeIterator::js_class_id, JSNodeIteratorPrototype::self(ctx));
    }
}

JSValue JSNodeIterator::create(JSContext* ctx, NodeIterator* impl)
{
    JSNodeIterator::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSNodeIteratorPrototype::self(ctx), JSNodeIterator::js_class_id);
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
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSNodeIterator::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RootAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->root()));
        }
        case WhatToShowAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return JS_NewBigUint64(ctx, imp->whatToShow());
        }
        case FilterAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->filter()));
        }
        case ExpandEntityReferencesAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return JS_NewBool(ctx, imp->expandEntityReferences() ? 1 : 0);
        }
        case ReferenceNodeAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->referenceNode()));
        }
        case PointerBeforeReferenceNodeAttrNum: {
            NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
            return JS_NewBool(ctx, imp->pointerBeforeReferenceNode() ? 1 : 0);
        }
    }
    return JS_NULL;
}

JSValue JSNodeIteratorPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    NodeIterator* imp = (NodeIterator*)JS_GetOpaque2(ctx, this_val, JSNodeIterator::js_class_id);
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
