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
#include "QJSSVGLengthList.h"

#include "ExceptionCode.h"
#include "QJSSVGLength.h"
#include "SVGLength.h"
#include "SVGLengthList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGLengthListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGLengthList::getValueProperty, NULL, JSSVGLengthList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGLengthListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGLengthListPrototypeFunction::callAsFunction, JSSVGLengthList::AppendItemFuncNum)
};

JSValue JSSVGLengthListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLengthList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLengthList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGLengthListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthListAttributesFunctions, countof(JSSVGLengthListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthListPrototypeFunctions, countof(JSSVGLengthListPrototypeFunctions));
}

static JSClassDef JSSVGLengthListClassDefine = 
{
    "SVGLengthList",
    .finalizer = JSSVGLengthList::finalizer,
    .gc_mark = JSSVGLengthList::mark,
};

JSClassID JSSVGLengthList::js_class_id = 0;

void JSSVGLengthList::init(JSContext* ctx)
{
    if (JSSVGLengthList::js_class_id == 0) {
        JS_NewClassID(&JSSVGLengthList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLengthList::js_class_id, &JSSVGLengthListClassDefine);
        JS_SetClassProto(ctx, JSSVGLengthList::js_class_id, JSSVGLengthListPrototype::self(ctx));
    }
}

JSValue JSSVGLengthList::create(JSContext* ctx, SVGLengthList* impl, SVGElement* context)
{
    JSSVGLengthList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGLengthListPrototype::self(ctx), JSSVGLengthList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGLengthList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGLengthList* impl = (SVGLengthList*)JS_GetOpaque(val, JSSVGLengthList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGLengthList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGLengthList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGLengthList* imp = (SVGLengthList*)JS_GetOpaque2(ctx, this_val, JSSVGLengthList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGLengthListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGLengthList* imp = (SVGLengthList*)JS_GetOpaque2(ctx, this_val, JSSVGLengthList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGLengthList::ClearFuncNum: {
            ExceptionCode ec = 0;
            imp->clear(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGLengthList::InitializeFuncNum: {
            ExceptionCode ec = 0;
            SVGLength item = toSVGLength(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->initialize(item, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGLengthList::GetItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->getItem(index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGLengthList::InsertItemBeforeFuncNum: {
            ExceptionCode ec = 0;
            SVGLength item = toSVGLength(argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->insertItemBefore(item, index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGLengthList::ReplaceItemFuncNum: {
            ExceptionCode ec = 0;
            SVGLength item = toSVGLength(argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->replaceItem(item, index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGLengthList::RemoveItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->removeItem(index, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGLengthList::AppendItemFuncNum: {
            ExceptionCode ec = 0;
            SVGLength item = toSVGLength(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGLength>(imp->appendItem(item, ec)), castedThisObj->context());
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGLengthList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGLengthList, JSSVGLengthList>(ctx, obj, context);
}

SVGLengthList* toSVGLengthList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGLengthList* impl = (SVGLengthList*)JS_GetOpaque(val, JSSVGLengthList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
