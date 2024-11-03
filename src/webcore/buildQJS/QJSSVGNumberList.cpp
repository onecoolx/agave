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
#include "QJSSVGNumberList.h"

#include "ExceptionCode.h"
#include "QJSSVGNumber.h"
#include "SVGNumberList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGNumberListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGNumberList::getValueProperty, NULL, JSSVGNumberList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGNumberListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGNumberListPrototypeFunction::callAsFunction, JSSVGNumberList::AppendItemFuncNum)
};

JSValue JSSVGNumberListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGNumberList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGNumberListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGNumberList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGNumberListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGNumberListAttributesFunctions, countof(JSSVGNumberListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGNumberListPrototypeFunctions, countof(JSSVGNumberListPrototypeFunctions));
}

static JSClassDef JSSVGNumberListClassDefine = 
{
    "SVGNumberList",
    .finalizer = JSSVGNumberList::finalizer,
    .gc_mark = JSSVGNumberList::mark,
};

JSClassID JSSVGNumberList::js_class_id = 0;

void JSSVGNumberList::init(JSContext* ctx)
{
    if (JSSVGNumberList::js_class_id == 0) {
        JS_NewClassID(&JSSVGNumberList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGNumberList::js_class_id, &JSSVGNumberListClassDefine);
        JS_SetClassProto(ctx, JSSVGNumberList::js_class_id, JSSVGNumberListPrototype::self(ctx));
    }
}

JSValue JSSVGNumberList::create(JSContext* ctx, SVGNumberList* impl, SVGElement* context)
{
    JSSVGNumberList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGNumberListPrototype::self(ctx), JSSVGNumberList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGNumberList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGNumberList* impl = (SVGNumberList*)JS_GetOpaque(val, JSSVGNumberList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGNumberList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGNumberList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGNumberList* imp = (SVGNumberList*)JS_GetOpaque2(ctx, this_val, JSSVGNumberList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGNumberListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGNumberList* imp = (SVGNumberList*)JS_GetOpaque2(ctx, this_val, JSSVGNumberList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGNumberList::ClearFuncNum: {
            ExceptionCode ec = 0;
            imp->clear(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGNumberList::InitializeFuncNum: {
            ExceptionCode ec = 0;
            float item = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->initialize(item, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGNumberList::GetItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->getItem(index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGNumberList::InsertItemBeforeFuncNum: {
            ExceptionCode ec = 0;
            float item = valueToFloat(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->insertItemBefore(item, index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGNumberList::ReplaceItemFuncNum: {
            ExceptionCode ec = 0;
            float item = valueToFloat(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->replaceItem(item, index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGNumberList::RemoveItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->removeItem(index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGNumberList::AppendItemFuncNum: {
            ExceptionCode ec = 0;
            float item = valueToFloat(ctx, argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<float>(imp->appendItem(item, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGNumberList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGNumberList, JSSVGNumberList>(ctx, obj, context);
}

SVGNumberList* toSVGNumberList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGNumberList* impl = (SVGNumberList*)JS_GetOpaque(val, JSSVGNumberList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
