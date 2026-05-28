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

#include "QJSHTMLOptionsCollection.h"

#include "ExceptionCode.h"
#include "HTMLOptionsCollection.h"
#include "QJSHTMLCollection.h"
#include "QJSHTMLOptionElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLOptionsCollectionAttributesFunctions[2];
static bool JSHTMLOptionsCollectionAttributesFunctions_initialized = false;

static void init_JSHTMLOptionsCollectionAttributesFunctions()
{
    if (JSHTMLOptionsCollectionAttributesFunctions_initialized) return;
    JSHTMLOptionsCollectionAttributesFunctions_initialized = true;
    memset(JSHTMLOptionsCollectionAttributesFunctions, 0, sizeof(JSHTMLOptionsCollectionAttributesFunctions));
    JSHTMLOptionsCollectionAttributesFunctions[0].name = "selectedIndex";
    JSHTMLOptionsCollectionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionsCollectionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionsCollectionAttributesFunctions[0].magic = JSHTMLOptionsCollection::SelectedIndexAttrNum;
    JSHTMLOptionsCollectionAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLOptionsCollection::getValueProperty;
    JSHTMLOptionsCollectionAttributesFunctions[0].u.getset.set.setter_magic = JSHTMLOptionsCollection::putValueProperty;
    JSHTMLOptionsCollectionAttributesFunctions[1].name = "length";
    JSHTMLOptionsCollectionAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLOptionsCollectionAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLOptionsCollectionAttributesFunctions[1].magic = JSHTMLOptionsCollection::LengthAttrNum;
    JSHTMLOptionsCollectionAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLOptionsCollection::getValueProperty;
    JSHTMLOptionsCollectionAttributesFunctions[1].u.getset.set.setter_magic = JSHTMLOptionsCollection::putValueProperty;
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLOptionsCollectionPrototypeFunctions[1];
static bool JSHTMLOptionsCollectionPrototypeFunctions_initialized = false;

static void init_JSHTMLOptionsCollectionPrototypeFunctions()
{
    if (JSHTMLOptionsCollectionPrototypeFunctions_initialized) return;
    JSHTMLOptionsCollectionPrototypeFunctions_initialized = true;
    memset(JSHTMLOptionsCollectionPrototypeFunctions, 0, sizeof(JSHTMLOptionsCollectionPrototypeFunctions));
    JSHTMLOptionsCollectionPrototypeFunctions[0].name = "add";
    JSHTMLOptionsCollectionPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLOptionsCollectionPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLOptionsCollectionPrototypeFunctions[0].magic = JSHTMLOptionsCollection::AddFuncNum;
    JSHTMLOptionsCollectionPrototypeFunctions[0].u.func.length = 2;
    JSHTMLOptionsCollectionPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLOptionsCollectionPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLOptionsCollectionPrototypeFunction::callAsFunction;
}

JSValue JSHTMLOptionsCollectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOptionsCollection.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLCollectionPrototype::self(ctx));
        JSHTMLOptionsCollectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOptionsCollection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLOptionsCollectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLOptionsCollectionAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionsCollectionAttributesFunctions, countof(JSHTMLOptionsCollectionAttributesFunctions));
    init_JSHTMLOptionsCollectionPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionsCollectionPrototypeFunctions, countof(JSHTMLOptionsCollectionPrototypeFunctions));
}

static JSClassDef JSHTMLOptionsCollectionClassDefine;
static bool JSHTMLOptionsCollectionClassDefine_initialized = false;

static void init_JSHTMLOptionsCollectionClassDefine()
{
    if (JSHTMLOptionsCollectionClassDefine_initialized) return;
    JSHTMLOptionsCollectionClassDefine_initialized = true;
    memset(&JSHTMLOptionsCollectionClassDefine, 0, sizeof(JSHTMLOptionsCollectionClassDefine));
    JSHTMLOptionsCollectionClassDefine.class_name = "HTMLOptionsCollection";
    JSHTMLOptionsCollectionClassDefine.finalizer = JSHTMLOptionsCollection::finalizer;
    JSHTMLOptionsCollectionClassDefine.gc_mark = JSHTMLOptionsCollection::mark;
}

JSClassID JSHTMLOptionsCollection::js_class_id = 0;

void JSHTMLOptionsCollection::init(JSContext* ctx)
{
    if (JSHTMLOptionsCollection::js_class_id == 0) {
        JSNode::init(ctx);
        JSHTMLCollection::init(ctx); JSHTMLOptionsCollection::js_class_id = JSHTMLCollection::js_class_id;
    }
}

JSValue JSHTMLOptionsCollection::create(JSContext* ctx, HTMLOptionsCollection* impl)
{
    JSHTMLOptionsCollection::init(ctx);
    JSValue _proto = JSHTMLOptionsCollectionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSHTMLCollection::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOptionsCollection::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOptionsCollection* impl = (HTMLOptionsCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLOptionsCollection::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSHTMLCollection::mark(rt, val, mark_func);
}

JSValue JSHTMLOptionsCollection::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case SelectedIndexAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
            return JS_NewInt32(ctx, imp->selectedIndex());
        }
        case LengthAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
            return JSHTMLOptionsCollection::length(ctx, this_val, imp);
        }
    }
    return JS_NULL;
}

JSValue JSHTMLOptionsCollection::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case SelectedIndexAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
            imp->setSelectedIndex(valueToInt32(ctx, value));
            break;
        }
        case LengthAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
            JSHTMLOptionsCollection::setLength(ctx, this_val, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLOptionsCollectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLOptionsCollection::AddFuncNum: {
            ExceptionCode ec = 0;
            HTMLOptionElement* option = toHTMLOptionElement(argv[0]);
            if (argc < 2) {
                  imp->add(option, ec);
                  setDOMException(ctx, ec);
                  return JS_UNDEFINED;
            }

            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }
            imp->add(option, index, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
    }
    return JS_NULL;
}


HTMLOptionsCollection* toHTMLOptionsCollection(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLOptionsCollection* impl = (HTMLOptionsCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
