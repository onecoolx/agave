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
#include "QJSSVGPointList.h"

#include "SVGPointList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPointListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGPointList::getValueProperty, NULL, JSSVGPointList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPointListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGPointListPrototypeFunction::callAsFunction, JSSVGPointList::AppendItemFuncNum)
};

JSValue JSSVGPointListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPointList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPointListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPointList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPointListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointListAttributesFunctions, countof(JSSVGPointListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointListPrototypeFunctions, countof(JSSVGPointListPrototypeFunctions));
}

static JSClassDef JSSVGPointListClassDefine = 
{
    "SVGPointList",
    .finalizer = JSSVGPointList::finalizer,
    .gc_mark = JSSVGPointList::mark,
};

JSClassID JSSVGPointList::js_class_id = 0;

void JSSVGPointList::init(JSContext* ctx)
{
    if (JSSVGPointList::js_class_id == 0) {
        JS_NewClassID(&JSSVGPointList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPointList::js_class_id, &JSSVGPointListClassDefine);
        JS_SetClassProto(ctx, JSSVGPointList::js_class_id, JSSVGPointListPrototype::self(ctx));
    }
}

JSValue JSSVGPointList::create(JSContext* ctx, SVGPointList* impl, SVGElement* context)
{
    JSSVGPointList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPointListPrototype::self(ctx), JSSVGPointList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPointList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPointList* impl = (SVGPointList*)JS_GetOpaque(val, JSSVGPointList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPointList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPointList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGPointList* imp = (SVGPointList*)JS_GetOpaque2(ctx, this_val, JSSVGPointList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPointListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPointList* imp = (SVGPointList*)JS_GetOpaque2(ctx, this_val, JSSVGPointList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPointList::ClearFuncNum: {
            return JSSVGPointList::clear(ctx, argc, argv, imp);
        }
        case JSSVGPointList::InitializeFuncNum: {
            return JSSVGPointList::initialize(ctx, argc, argv, imp);
        }
        case JSSVGPointList::GetItemFuncNum: {
            return JSSVGPointList::getItem(ctx, argc, argv, imp);
        }
        case JSSVGPointList::InsertItemBeforeFuncNum: {
            return JSSVGPointList::insertItemBefore(ctx, argc, argv, imp);
        }
        case JSSVGPointList::ReplaceItemFuncNum: {
            return JSSVGPointList::replaceItem(ctx, argc, argv, imp);
        }
        case JSSVGPointList::RemoveItemFuncNum: {
            return JSSVGPointList::removeItem(ctx, argc, argv, imp);
        }
        case JSSVGPointList::AppendItemFuncNum: {
            return JSSVGPointList::appendItem(ctx, argc, argv, imp);
        }
    }
    (void)imp;
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGPointList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGPointList, JSSVGPointList>(ctx, obj, context);
}

SVGPointList* toSVGPointList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPointList* impl = (SVGPointList*)JS_GetOpaque(val, JSSVGPointList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
