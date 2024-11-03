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

#include "QJSTreeWalker.h"

#include "Node.h"
#include "NodeFilter.h"
#include "QJSNode.h"
#include "QJSNodeFilter.h"
#include "TreeWalker.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSTreeWalkerAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("filter", JSTreeWalker::getValueProperty, NULL, JSTreeWalker::FilterAttrNum),
    JS_CGETSET_MAGIC_DEF("root", JSTreeWalker::getValueProperty, NULL, JSTreeWalker::RootAttrNum),
    JS_CGETSET_MAGIC_DEF("whatToShow", JSTreeWalker::getValueProperty, NULL, JSTreeWalker::WhatToShowAttrNum),
    JS_CGETSET_MAGIC_DEF("expandEntityReferences", JSTreeWalker::getValueProperty, NULL, JSTreeWalker::ExpandEntityReferencesAttrNum),
    JS_CGETSET_MAGIC_DEF("currentNode", JSTreeWalker::getValueProperty, JSTreeWalker::putValueProperty, JSTreeWalker::CurrentNodeAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSTreeWalkerPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("nextSibling", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::NextSiblingFuncNum),
    JS_CFUNC_MAGIC_DEF("parentNode", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::ParentNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("nextNode", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::NextNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("lastChild", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::LastChildFuncNum),
    JS_CFUNC_MAGIC_DEF("previousNode", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::PreviousNodeFuncNum),
    JS_CFUNC_MAGIC_DEF("firstChild", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::FirstChildFuncNum),
    JS_CFUNC_MAGIC_DEF("previousSibling", 0, JSTreeWalkerPrototypeFunction::callAsFunction, JSTreeWalker::PreviousSiblingFuncNum)
};

JSValue JSTreeWalkerPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSTreeWalker.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSTreeWalkerPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSTreeWalker.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSTreeWalkerPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSTreeWalkerAttributesFunctions, countof(JSTreeWalkerAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSTreeWalkerPrototypeFunctions, countof(JSTreeWalkerPrototypeFunctions));
}

static JSClassDef JSTreeWalkerClassDefine = 
{
    "TreeWalker",
    .finalizer = JSTreeWalker::finalizer,
    .gc_mark = JSTreeWalker::mark,
};

JSClassID JSTreeWalker::js_class_id = 0;

void JSTreeWalker::init(JSContext* ctx)
{
    if (JSTreeWalker::js_class_id == 0) {
        JS_NewClassID(&JSTreeWalker::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSTreeWalker::js_class_id, &JSTreeWalkerClassDefine);
        JS_SetClassProto(ctx, JSTreeWalker::js_class_id, JSTreeWalkerPrototype::self(ctx));
    }
}

JSValue JSTreeWalker::create(JSContext* ctx, TreeWalker* impl)
{
    JSTreeWalker::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSTreeWalkerPrototype::self(ctx), JSTreeWalker::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSTreeWalker::finalizer(JSRuntime* rt, JSValue val)
{
    TreeWalker* impl = (TreeWalker*)JS_GetOpaque(val, JSTreeWalker::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSTreeWalker::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RootAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->root()));
        }
        case WhatToShowAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            return JS_NewBigUint64(ctx, imp->whatToShow());
        }
        case FilterAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->filter()));
        }
        case ExpandEntityReferencesAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            return JS_NewBool(ctx, imp->expandEntityReferences() ? 1 : 0);
        }
        case CurrentNodeAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->currentNode()));
        }
    }
    return JS_NULL;
}

JSValue JSTreeWalker::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CurrentNodeAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
            ExceptionCode ec = 0;
            imp->setCurrentNode(toNode(value), ec);
            setDOMException(ctx, ec);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSTreeWalkerPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    TreeWalker* imp = (TreeWalker*)JS_GetOpaque2(ctx, this_val, JSTreeWalker::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSTreeWalker::ParentNodeFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->parentNode()));
            return result;
        }
        case JSTreeWalker::FirstChildFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->firstChild()));
            return result;
        }
        case JSTreeWalker::LastChildFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->lastChild()));
            return result;
        }
        case JSTreeWalker::PreviousSiblingFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->previousSibling()));
            return result;
        }
        case JSTreeWalker::NextSiblingFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->nextSibling()));
            return result;
        }
        case JSTreeWalker::PreviousNodeFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->previousNode()));
            return result;
        }
        case JSTreeWalker::NextNodeFuncNum: {

            JSValue result = toJS(ctx, QJS::getPtr(imp->nextNode()));
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, TreeWalker* obj)
{
    return QJS::cacheDOMObject<TreeWalker, JSTreeWalker>(ctx, obj);
}

TreeWalker* toTreeWalker(JSValue val)
{
    if (JS_IsObject(val)) {
        TreeWalker* impl = (TreeWalker*)JS_GetOpaque(val, JSTreeWalker::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
