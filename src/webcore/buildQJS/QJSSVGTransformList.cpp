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

static JSCFunctionListEntry JSSVGTransformListAttributesFunctions[1];
static bool JSSVGTransformListAttributesFunctions_initialized = false;

static void init_JSSVGTransformListAttributesFunctions()
{
    if (JSSVGTransformListAttributesFunctions_initialized) return;
    JSSVGTransformListAttributesFunctions_initialized = true;
    memset(JSSVGTransformListAttributesFunctions, 0, sizeof(JSSVGTransformListAttributesFunctions));
    JSSVGTransformListAttributesFunctions[0].name = "numberOfItems";
    JSSVGTransformListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGTransformListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGTransformListAttributesFunctions[0].magic = JSSVGTransformList::NumberOfItemsAttrNum;
    JSSVGTransformListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGTransformList::getValueProperty;
    JSSVGTransformListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGTransformListPrototypeFunctions[9];
static bool JSSVGTransformListPrototypeFunctions_initialized = false;

static void init_JSSVGTransformListPrototypeFunctions()
{
    if (JSSVGTransformListPrototypeFunctions_initialized) return;
    JSSVGTransformListPrototypeFunctions_initialized = true;
    memset(JSSVGTransformListPrototypeFunctions, 0, sizeof(JSSVGTransformListPrototypeFunctions));
    JSSVGTransformListPrototypeFunctions[0].name = "clear";
    JSSVGTransformListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[0].magic = JSSVGTransformList::ClearFuncNum;
    JSSVGTransformListPrototypeFunctions[0].u.func.length = 0;
    JSSVGTransformListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[1].name = "initialize";
    JSSVGTransformListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[1].magic = JSSVGTransformList::InitializeFuncNum;
    JSSVGTransformListPrototypeFunctions[1].u.func.length = 1;
    JSSVGTransformListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[2].name = "getItem";
    JSSVGTransformListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[2].magic = JSSVGTransformList::GetItemFuncNum;
    JSSVGTransformListPrototypeFunctions[2].u.func.length = 1;
    JSSVGTransformListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[3].name = "insertItemBefore";
    JSSVGTransformListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[3].magic = JSSVGTransformList::InsertItemBeforeFuncNum;
    JSSVGTransformListPrototypeFunctions[3].u.func.length = 2;
    JSSVGTransformListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[4].name = "replaceItem";
    JSSVGTransformListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[4].magic = JSSVGTransformList::ReplaceItemFuncNum;
    JSSVGTransformListPrototypeFunctions[4].u.func.length = 2;
    JSSVGTransformListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[5].name = "removeItem";
    JSSVGTransformListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[5].magic = JSSVGTransformList::RemoveItemFuncNum;
    JSSVGTransformListPrototypeFunctions[5].u.func.length = 1;
    JSSVGTransformListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[6].name = "appendItem";
    JSSVGTransformListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[6].magic = JSSVGTransformList::AppendItemFuncNum;
    JSSVGTransformListPrototypeFunctions[6].u.func.length = 1;
    JSSVGTransformListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[7].name = "createSVGTransformFromMatrix";
    JSSVGTransformListPrototypeFunctions[7].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[7].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[7].magic = JSSVGTransformList::CreateSVGTransformFromMatrixFuncNum;
    JSSVGTransformListPrototypeFunctions[7].u.func.length = 1;
    JSSVGTransformListPrototypeFunctions[7].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[7].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
    JSSVGTransformListPrototypeFunctions[8].name = "consolidate";
    JSSVGTransformListPrototypeFunctions[8].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGTransformListPrototypeFunctions[8].def_type = JS_DEF_CFUNC;
    JSSVGTransformListPrototypeFunctions[8].magic = JSSVGTransformList::ConsolidateFuncNum;
    JSSVGTransformListPrototypeFunctions[8].u.func.length = 0;
    JSSVGTransformListPrototypeFunctions[8].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGTransformListPrototypeFunctions[8].u.func.cfunc.generic_magic = JSSVGTransformListPrototypeFunction::callAsFunction;
}

JSValue JSSVGTransformListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGTransformList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGTransformListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGTransformList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGTransformListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGTransformListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformListAttributesFunctions, countof(JSSVGTransformListAttributesFunctions));
    init_JSSVGTransformListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGTransformListPrototypeFunctions, countof(JSSVGTransformListPrototypeFunctions));
}

static JSClassDef JSSVGTransformListClassDefine;
static bool JSSVGTransformListClassDefine_initialized = false;

static void init_JSSVGTransformListClassDefine()
{
    if (JSSVGTransformListClassDefine_initialized) return;
    JSSVGTransformListClassDefine_initialized = true;
    memset(&JSSVGTransformListClassDefine, 0, sizeof(JSSVGTransformListClassDefine));
    JSSVGTransformListClassDefine.class_name = "SVGTransformList";
    JSSVGTransformListClassDefine.finalizer = JSSVGTransformList::finalizer;
    JSSVGTransformListClassDefine.gc_mark = JSSVGTransformList::mark;
}

JSClassID JSSVGTransformList::js_class_id = 0;

void JSSVGTransformList::init(JSContext* ctx)
{
    if (JSSVGTransformList::js_class_id == 0) {
        init_JSSVGTransformListClassDefine();
        JS_NewClassID(&JSSVGTransformList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGTransformList::js_class_id, &JSSVGTransformListClassDefine);
        JS_SetClassProto(ctx, JSSVGTransformList::js_class_id, JSSVGTransformListPrototype::self(ctx));
    }
}

JSValue JSSVGTransformList::create(JSContext* ctx, SVGTransformList* impl, SVGElement* context)
{
    JSSVGTransformList::init(ctx);
    JSValue _proto = JSSVGTransformListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGTransformList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGTransformList* imp = (SVGTransformList*)JS_GetOpaque(this_val, JSSVGTransformList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGTransformListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGTransformList* imp = (SVGTransformList*)JS_GetOpaque(this_val, JSSVGTransformList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGTransformList::ClearFuncNum: {
            return JSSVGTransformList::clear(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::InitializeFuncNum: {
            return JSSVGTransformList::initialize(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::GetItemFuncNum: {
            return JSSVGTransformList::getItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::InsertItemBeforeFuncNum: {
            return JSSVGTransformList::insertItemBefore(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::ReplaceItemFuncNum: {
            return JSSVGTransformList::replaceItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::RemoveItemFuncNum: {
            return JSSVGTransformList::removeItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGTransformList::AppendItemFuncNum: {
            return JSSVGTransformList::appendItem(ctx, this_val, argc, argv, imp);
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
