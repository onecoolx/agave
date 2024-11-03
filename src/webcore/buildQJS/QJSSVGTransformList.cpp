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
#include "QJSSVGTransformList.h"

#include "QJSSVGMatrix.h"
#include "QJSSVGTransform.h"
#include "SVGTransform.h"
#include "SVGTransformList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGTransformListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGTransformList::getValueProperty, NULL, JSSVGTransformList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGTransformListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::AppendItemFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("createSVGTransformFromMatrix", 1, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::CreateSVGTransformFromMatrixFuncNum),
    JS_CFUNC_MAGIC_DEF("consolidate", 0, JSSVGTransformListPrototypeFunction::callAsFunction, JSSVGTransformList::ConsolidateFuncNum)
};

JSValue JSSVGTransformListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTransformList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTransformList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGTransformListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformListAttributesFunctions, countof(JSSVGTransformListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformListPrototypeFunctions, countof(JSSVGTransformListPrototypeFunctions));
}

static JSClassDef JSSVGTransformListClassDefine = 
{
    "SVGTransformList",
    .finalizer = JSSVGTransformList::finalizer,
    .gc_mark = JSSVGTransformList::mark,
};

JSClassID JSSVGTransformList::js_class_id = 0;

void JSSVGTransformList::init(JSContext* ctx)
{
    if (JSSVGTransformList::js_class_id == 0) {
        JS_NewClassID(&JSSVGTransformList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTransformList::js_class_id, &JSSVGTransformListClassDefine);
        JS_SetClassProto(ctx, JSSVGTransformList::js_class_id, JSSVGTransformListPrototype::self(ctx));
    }
}

JSValue JSSVGTransformList::create(JSContext* ctx, SVGTransformList* impl, SVGElement* context)
{
    JSSVGTransformList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGTransformListPrototype::self(ctx), JSSVGTransformList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGTransformList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGTransformList* impl = (SVGTransformList*)JS_GetOpaque(val, JSSVGTransformList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGTransformList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGTransformList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGTransformList* imp = (SVGTransformList*)JS_GetOpaque2(ctx, this_val, JSSVGTransformList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGTransformListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTransformList* imp = (SVGTransformList*)JS_GetOpaque2(ctx, this_val, JSSVGTransformList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTransformList::ClearFuncNum: {
            return JSSVGTransformList::clear(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::InitializeFuncNum: {
            return JSSVGTransformList::initialize(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::GetItemFuncNum: {
            return JSSVGTransformList::getItem(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::InsertItemBeforeFuncNum: {
            return JSSVGTransformList::insertItemBefore(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::ReplaceItemFuncNum: {
            return JSSVGTransformList::replaceItem(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::RemoveItemFuncNum: {
            return JSSVGTransformList::removeItem(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::AppendItemFuncNum: {
            return JSSVGTransformList::appendItem(ctx, argc, argv, imp);
        }
        case JSSVGTransformList::CreateSVGTransformFromMatrixFuncNum: {
            AffineTransform matrix = toSVGMatrix(argv[0]);

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGTransform>(imp->createSVGTransformFromMatrix(matrix)), castedThisObj->context());
            return result;
        }
        case JSSVGTransformList::ConsolidateFuncNum: {

            JSValue result = toJS(ctx, new JSSVGPODTypeWrapperCreatorReadOnly<SVGTransform>(imp->consolidate()), castedThisObj->context());
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGTransformList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGTransformList, JSSVGTransformList>(ctx, obj, context);
}

SVGTransformList* toSVGTransformList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGTransformList* impl = (SVGTransformList*)JS_GetOpaque(val, JSSVGTransformList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
