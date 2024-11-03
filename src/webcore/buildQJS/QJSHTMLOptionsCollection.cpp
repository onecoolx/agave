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

#include "QJSHTMLOptionsCollection.h"

#include "ExceptionCode.h"
#include "HTMLOptionsCollection.h"
#include "QJSHTMLOptionElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSHTMLOptionsCollectionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("selectedIndex", JSHTMLOptionsCollection::getValueProperty, JSHTMLOptionsCollection::putValueProperty, JSHTMLOptionsCollection::SelectedIndexAttrNum),
    JS_CGETSET_MAGIC_DEF("length", JSHTMLOptionsCollection::getValueProperty, JSHTMLOptionsCollection::putValueProperty, JSHTMLOptionsCollection::LengthAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSHTMLOptionsCollectionPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("add", 2, JSHTMLOptionsCollectionPrototypeFunction::callAsFunction, JSHTMLOptionsCollection::AddFuncNum)
};

JSValue JSHTMLOptionsCollectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLOptionsCollection.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSHTMLCollectionPrototype::self(ctx));
        JSHTMLOptionsCollectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLOptionsCollection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSHTMLOptionsCollectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionsCollectionAttributesFunctions, countof(JSHTMLOptionsCollectionAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLOptionsCollectionPrototypeFunctions, countof(JSHTMLOptionsCollectionPrototypeFunctions));
}

static JSClassDef JSHTMLOptionsCollectionClassDefine = 
{
    "HTMLOptionsCollection",
    .finalizer = JSHTMLOptionsCollection::finalizer,
    .gc_mark = JSHTMLOptionsCollection::mark,
};

JSClassID JSHTMLOptionsCollection::js_class_id = 0;

void JSHTMLOptionsCollection::init(JSContext* ctx)
{
    if (JSHTMLOptionsCollection::js_class_id == 0) {
        JS_NewClassID(&JSHTMLOptionsCollection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLOptionsCollection::js_class_id, &JSHTMLOptionsCollectionClassDefine);
        JS_SetClassProto(ctx, JSHTMLOptionsCollection::js_class_id, JSHTMLOptionsCollectionPrototype::self(ctx));
    }
}

JSValue JSHTMLOptionsCollection::create(JSContext* ctx, HTMLOptionsCollection* impl)
{
    JSHTMLOptionsCollection::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSHTMLOptionsCollectionPrototype::self(ctx), JSHTMLOptionsCollection::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLOptionsCollection::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLOptionsCollection* impl = (HTMLOptionsCollection*)JS_GetOpaque(val, JSHTMLOptionsCollection::js_class_id);
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
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionsCollection::js_class_id);
            return JS_NewBigUint64(ctx, imp->selectedIndex());
        }
        case LengthAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionsCollection::js_class_id);
            return JSHTMLOptionsCollection::length(ctx, imp);
        }
    }
    return JS_NULL;
}

JSValue JSHTMLOptionsCollection::putValueProperty(JSContext *ctx, JSValueConst this_val, JSValue value, int token)
{
    switch (token) {
        case SelectedIndexAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionsCollection::js_class_id);
            imp->setSelectedIndex(valueToInt32(ctx, value));
            break;
        }
        case LengthAttrNum: {
            HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionsCollection::js_class_id);
            JSHTMLOptionsCollection::setLength(ctx, value, imp);
            break;
        }
    }
    return JS_UNDEFINED;
}

JSValue JSHTMLOptionsCollectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLOptionsCollection* imp = (HTMLOptionsCollection*)JS_GetOpaque2(ctx, this_val, JSHTMLOptionsCollection::js_class_id);
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
        HTMLOptionsCollection* impl = (HTMLOptionsCollection*)JS_GetOpaque(val, JSHTMLOptionsCollection::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
