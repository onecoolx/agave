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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGStringList.h"

#include "ExceptionCode.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGStringListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGStringList::getValueProperty, NULL, JSSVGStringList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGStringListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGStringListPrototypeFunction::callAsFunction, JSSVGStringList::AppendItemFuncNum)
};

JSValue JSSVGStringListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGStringList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGStringListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGStringList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGStringListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStringListAttributesFunctions, countof(JSSVGStringListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStringListPrototypeFunctions, countof(JSSVGStringListPrototypeFunctions));
}

static JSClassDef JSSVGStringListClassDefine = 
{
    "SVGStringList",
    .finalizer = JSSVGStringList::finalizer,
    .gc_mark = JSSVGStringList::mark,
};

JSClassID JSSVGStringList::js_class_id = 0;

void JSSVGStringList::init(JSContext* ctx)
{
    if (JSSVGStringList::js_class_id == 0) {
        JS_NewClassID(&JSSVGStringList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGStringList::js_class_id, &JSSVGStringListClassDefine);
        JS_SetClassProto(ctx, JSSVGStringList::js_class_id, JSSVGStringListPrototype::self(ctx));
    }
}

JSValue JSSVGStringList::create(JSContext* ctx, SVGStringList* impl, SVGElement* context)
{
    JSSVGStringList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGStringListPrototype::self(ctx), JSSVGStringList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGStringList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGStringList* impl = (SVGStringList*)JS_GetOpaque(val, JSSVGStringList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGStringList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGStringList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGStringList* imp = (SVGStringList*)JS_GetOpaque2(ctx, this_val, JSSVGStringList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGStringListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGStringList* imp = (SVGStringList*)JS_GetOpaque2(ctx, this_val, JSSVGStringList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGStringList::ClearFuncNum: {
            ExceptionCode ec = 0;
            imp->clear(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGStringList::InitializeFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->initialize(item, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::GetItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->getItem(index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::InsertItemBeforeFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->insertItemBefore(item, index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::ReplaceItemFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->replaceItem(item, index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::RemoveItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->removeItem(index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::AppendItemFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->appendItem(item, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGStringList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGStringList, JSSVGStringList>(ctx, obj, context);
}

SVGStringList* toSVGStringList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGStringList* impl = (SVGStringList*)JS_GetOpaque(val, JSSVGStringList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
