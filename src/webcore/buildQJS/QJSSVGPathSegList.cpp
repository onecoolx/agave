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
#include "QJSSVGPathSegList.h"

#include "SVGPathSegList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGPathSegListAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("numberOfItems", JSSVGPathSegList::getValueProperty, NULL, JSSVGPathSegList::NumberOfItemsAttrNum)
};

/* Prototype functions table */

static const JSCFunctionListEntry JSSVGPathSegListPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("clear", 0, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::ClearFuncNum),
    JS_CFUNC_MAGIC_DEF("getItem", 1, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::GetItemFuncNum),
    JS_CFUNC_MAGIC_DEF("insertItemBefore", 2, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::InsertItemBeforeFuncNum),
    JS_CFUNC_MAGIC_DEF("initialize", 1, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::InitializeFuncNum),
    JS_CFUNC_MAGIC_DEF("replaceItem", 2, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::ReplaceItemFuncNum),
    JS_CFUNC_MAGIC_DEF("removeItem", 1, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::RemoveItemFuncNum),
    JS_CFUNC_MAGIC_DEF("appendItem", 1, JSSVGPathSegListPrototypeFunction::callAsFunction, JSSVGPathSegList::AppendItemFuncNum)
};

JSValue JSSVGPathSegListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegList.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGPathSegListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegListAttributesFunctions, countof(JSSVGPathSegListAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegListPrototypeFunctions, countof(JSSVGPathSegListPrototypeFunctions));
}

static JSClassDef JSSVGPathSegListClassDefine = 
{
    "SVGPathSegList",
    .finalizer = JSSVGPathSegList::finalizer,
    .gc_mark = JSSVGPathSegList::mark,
};

JSClassID JSSVGPathSegList::js_class_id = 0;

void JSSVGPathSegList::init(JSContext* ctx)
{
    if (JSSVGPathSegList::js_class_id == 0) {
        JS_NewClassID(&JSSVGPathSegList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegList::js_class_id, &JSSVGPathSegListClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegList::js_class_id, JSSVGPathSegListPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegList::create(JSContext* ctx, SVGPathSegList* impl, SVGElement* context)
{
    JSSVGPathSegList::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGPathSegListPrototype::self(ctx), JSSVGPathSegList::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSegList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSegList* impl = (SVGPathSegList*)JS_GetOpaque(val, JSSVGPathSegList::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSegList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPathSegList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGPathSegList* imp = (SVGPathSegList*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegList::js_class_id);
            return JS_NewBigUint64(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPathSegList* imp = (SVGPathSegList*)JS_GetOpaque2(ctx, this_val, JSSVGPathSegList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPathSegList::ClearFuncNum: {
            return JSSVGPathSegList::clear(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::InitializeFuncNum: {
            return JSSVGPathSegList::initialize(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::GetItemFuncNum: {
            return JSSVGPathSegList::getItem(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::InsertItemBeforeFuncNum: {
            return JSSVGPathSegList::insertItemBefore(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::ReplaceItemFuncNum: {
            return JSSVGPathSegList::replaceItem(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::RemoveItemFuncNum: {
            return JSSVGPathSegList::removeItem(ctx, argc, argv, imp);
        }
        case JSSVGPathSegList::AppendItemFuncNum: {
            return JSSVGPathSegList::appendItem(ctx, argc, argv, imp);
        }
    }
    (void)imp;
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGPathSegList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGPathSegList, JSSVGPathSegList>(ctx, obj, context);
}

SVGPathSegList* toSVGPathSegList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPathSegList* impl = (SVGPathSegList*)JS_GetOpaque(val, JSSVGPathSegList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
