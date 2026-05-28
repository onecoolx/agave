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

static JSCFunctionListEntry JSTreeWalkerAttributesFunctions[5];
static bool JSTreeWalkerAttributesFunctions_initialized = false;

static void init_JSTreeWalkerAttributesFunctions()
{
    if (JSTreeWalkerAttributesFunctions_initialized) return;
    JSTreeWalkerAttributesFunctions_initialized = true;
    memset(JSTreeWalkerAttributesFunctions, 0, sizeof(JSTreeWalkerAttributesFunctions));
    JSTreeWalkerAttributesFunctions[0].name = "filter";
    JSTreeWalkerAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSTreeWalkerAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSTreeWalkerAttributesFunctions[0].magic = JSTreeWalker::FilterAttrNum;
    JSTreeWalkerAttributesFunctions[0].u.getset.get.getter_magic = JSTreeWalker::getValueProperty;
    JSTreeWalkerAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSTreeWalkerAttributesFunctions[1].name = "root";
    JSTreeWalkerAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSTreeWalkerAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSTreeWalkerAttributesFunctions[1].magic = JSTreeWalker::RootAttrNum;
    JSTreeWalkerAttributesFunctions[1].u.getset.get.getter_magic = JSTreeWalker::getValueProperty;
    JSTreeWalkerAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSTreeWalkerAttributesFunctions[2].name = "whatToShow";
    JSTreeWalkerAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSTreeWalkerAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSTreeWalkerAttributesFunctions[2].magic = JSTreeWalker::WhatToShowAttrNum;
    JSTreeWalkerAttributesFunctions[2].u.getset.get.getter_magic = JSTreeWalker::getValueProperty;
    JSTreeWalkerAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSTreeWalkerAttributesFunctions[3].name = "expandEntityReferences";
    JSTreeWalkerAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSTreeWalkerAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSTreeWalkerAttributesFunctions[3].magic = JSTreeWalker::ExpandEntityReferencesAttrNum;
    JSTreeWalkerAttributesFunctions[3].u.getset.get.getter_magic = JSTreeWalker::getValueProperty;
    JSTreeWalkerAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSTreeWalkerAttributesFunctions[4].name = "currentNode";
    JSTreeWalkerAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSTreeWalkerAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSTreeWalkerAttributesFunctions[4].magic = JSTreeWalker::CurrentNodeAttrNum;
    JSTreeWalkerAttributesFunctions[4].u.getset.get.getter_magic = JSTreeWalker::getValueProperty;
    JSTreeWalkerAttributesFunctions[4].u.getset.set.setter_magic = JSTreeWalker::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSTreeWalkerPrototypeFunctions[7];
static bool JSTreeWalkerPrototypeFunctions_initialized = false;

static void init_JSTreeWalkerPrototypeFunctions()
{
    if (JSTreeWalkerPrototypeFunctions_initialized) return;
    JSTreeWalkerPrototypeFunctions_initialized = true;
    memset(JSTreeWalkerPrototypeFunctions, 0, sizeof(JSTreeWalkerPrototypeFunctions));
    JSTreeWalkerPrototypeFunctions[0].name = "nextSibling";
    JSTreeWalkerPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[0].magic = JSTreeWalker::NextSiblingFuncNum;
    JSTreeWalkerPrototypeFunctions[0].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[0].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[1].name = "parentNode";
    JSTreeWalkerPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[1].magic = JSTreeWalker::ParentNodeFuncNum;
    JSTreeWalkerPrototypeFunctions[1].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[1].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[2].name = "nextNode";
    JSTreeWalkerPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[2].magic = JSTreeWalker::NextNodeFuncNum;
    JSTreeWalkerPrototypeFunctions[2].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[2].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[3].name = "lastChild";
    JSTreeWalkerPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[3].magic = JSTreeWalker::LastChildFuncNum;
    JSTreeWalkerPrototypeFunctions[3].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[3].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[4].name = "previousNode";
    JSTreeWalkerPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[4].magic = JSTreeWalker::PreviousNodeFuncNum;
    JSTreeWalkerPrototypeFunctions[4].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[4].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[5].name = "firstChild";
    JSTreeWalkerPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[5].magic = JSTreeWalker::FirstChildFuncNum;
    JSTreeWalkerPrototypeFunctions[5].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[5].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
    JSTreeWalkerPrototypeFunctions[6].name = "previousSibling";
    JSTreeWalkerPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSTreeWalkerPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSTreeWalkerPrototypeFunctions[6].magic = JSTreeWalker::PreviousSiblingFuncNum;
    JSTreeWalkerPrototypeFunctions[6].u.func.length = 0;
    JSTreeWalkerPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSTreeWalkerPrototypeFunctions[6].u.func.cfunc.generic_magic = JSTreeWalkerPrototypeFunction::callAsFunction;
}

JSValue JSTreeWalkerPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSTreeWalker.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSTreeWalkerPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSTreeWalker.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSTreeWalkerPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSTreeWalkerAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSTreeWalkerAttributesFunctions, countof(JSTreeWalkerAttributesFunctions));
    init_JSTreeWalkerPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSTreeWalkerPrototypeFunctions, countof(JSTreeWalkerPrototypeFunctions));
}

static JSClassDef JSTreeWalkerClassDefine;
static bool JSTreeWalkerClassDefine_initialized = false;

static void init_JSTreeWalkerClassDefine()
{
    if (JSTreeWalkerClassDefine_initialized) return;
    JSTreeWalkerClassDefine_initialized = true;
    memset(&JSTreeWalkerClassDefine, 0, sizeof(JSTreeWalkerClassDefine));
    JSTreeWalkerClassDefine.class_name = "TreeWalker";
    JSTreeWalkerClassDefine.finalizer = JSTreeWalker::finalizer;
    JSTreeWalkerClassDefine.gc_mark = JSTreeWalker::mark;
}

JSClassID JSTreeWalker::js_class_id = 0;

void JSTreeWalker::init(JSContext* ctx)
{
    if (JSTreeWalker::js_class_id == 0) {
        init_JSTreeWalkerClassDefine();
        JS_NewClassID(&JSTreeWalker::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSTreeWalker::js_class_id, &JSTreeWalkerClassDefine);
        JS_SetClassProto(ctx, JSTreeWalker::js_class_id, JSTreeWalkerPrototype::self(ctx));
    }
}

JSValue JSTreeWalker::create(JSContext* ctx, TreeWalker* impl)
{
    JSTreeWalker::init(ctx);
    JSValue _proto = JSTreeWalkerPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSTreeWalker::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

JSValue JSTreeWalker::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case RootAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->root()));
        }
        case WhatToShowAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
            return JS_NewInt32(ctx, imp->whatToShow());
        }
        case FilterAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->filter()));
        }
        case ExpandEntityReferencesAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
            return JS_NewBool(ctx, imp->expandEntityReferences() ? 1 : 0);
        }
        case CurrentNodeAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->currentNode()));
        }
    }
    return JS_NULL;
}

JSValue JSTreeWalker::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case CurrentNodeAttrNum: {
            TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
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
    TreeWalker* imp = (TreeWalker*)JS_GetOpaque(this_val, JSTreeWalker::js_class_id);
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
