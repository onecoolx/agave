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
#include "QJSSVGPointList.h"

#include "SVGPointList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPointListAttributesFunctions[1];
static bool JSSVGPointListAttributesFunctions_initialized = false;

static void init_JSSVGPointListAttributesFunctions()
{
    if (JSSVGPointListAttributesFunctions_initialized) return;
    JSSVGPointListAttributesFunctions_initialized = true;
    memset(JSSVGPointListAttributesFunctions, 0, sizeof(JSSVGPointListAttributesFunctions));
    JSSVGPointListAttributesFunctions[0].name = "numberOfItems";
    JSSVGPointListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPointListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPointListAttributesFunctions[0].magic = JSSVGPointList::NumberOfItemsAttrNum;
    JSSVGPointListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPointList::getValueProperty;
    JSSVGPointListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPointListPrototypeFunctions[7];
static bool JSSVGPointListPrototypeFunctions_initialized = false;

static void init_JSSVGPointListPrototypeFunctions()
{
    if (JSSVGPointListPrototypeFunctions_initialized) return;
    JSSVGPointListPrototypeFunctions_initialized = true;
    memset(JSSVGPointListPrototypeFunctions, 0, sizeof(JSSVGPointListPrototypeFunctions));
    JSSVGPointListPrototypeFunctions[0].name = "clear";
    JSSVGPointListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[0].magic = JSSVGPointList::ClearFuncNum;
    JSSVGPointListPrototypeFunctions[0].u.func.length = 0;
    JSSVGPointListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[1].name = "getItem";
    JSSVGPointListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[1].magic = JSSVGPointList::GetItemFuncNum;
    JSSVGPointListPrototypeFunctions[1].u.func.length = 1;
    JSSVGPointListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[2].name = "insertItemBefore";
    JSSVGPointListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[2].magic = JSSVGPointList::InsertItemBeforeFuncNum;
    JSSVGPointListPrototypeFunctions[2].u.func.length = 2;
    JSSVGPointListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[3].name = "initialize";
    JSSVGPointListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[3].magic = JSSVGPointList::InitializeFuncNum;
    JSSVGPointListPrototypeFunctions[3].u.func.length = 1;
    JSSVGPointListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[4].name = "replaceItem";
    JSSVGPointListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[4].magic = JSSVGPointList::ReplaceItemFuncNum;
    JSSVGPointListPrototypeFunctions[4].u.func.length = 2;
    JSSVGPointListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[5].name = "removeItem";
    JSSVGPointListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[5].magic = JSSVGPointList::RemoveItemFuncNum;
    JSSVGPointListPrototypeFunctions[5].u.func.length = 1;
    JSSVGPointListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
    JSSVGPointListPrototypeFunctions[6].name = "appendItem";
    JSSVGPointListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPointListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGPointListPrototypeFunctions[6].magic = JSSVGPointList::AppendItemFuncNum;
    JSSVGPointListPrototypeFunctions[6].u.func.length = 1;
    JSSVGPointListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPointListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGPointListPrototypeFunction::callAsFunction;
}

JSValue JSSVGPointListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPointList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPointListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPointList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPointListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPointListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointListAttributesFunctions, countof(JSSVGPointListAttributesFunctions));
    init_JSSVGPointListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPointListPrototypeFunctions, countof(JSSVGPointListPrototypeFunctions));
}

static JSClassDef JSSVGPointListClassDefine;
static bool JSSVGPointListClassDefine_initialized = false;

static void init_JSSVGPointListClassDefine()
{
    if (JSSVGPointListClassDefine_initialized) return;
    JSSVGPointListClassDefine_initialized = true;
    memset(&JSSVGPointListClassDefine, 0, sizeof(JSSVGPointListClassDefine));
    JSSVGPointListClassDefine.class_name = "SVGPointList";
    JSSVGPointListClassDefine.finalizer = JSSVGPointList::finalizer;
    JSSVGPointListClassDefine.gc_mark = JSSVGPointList::mark;
}

JSClassID JSSVGPointList::js_class_id = 0;

void JSSVGPointList::init(JSContext* ctx)
{
    if (JSSVGPointList::js_class_id == 0) {
        init_JSSVGPointListClassDefine();
        JS_NewClassID(&JSSVGPointList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPointList::js_class_id, &JSSVGPointListClassDefine);
        JS_SetClassProto(ctx, JSSVGPointList::js_class_id, JSSVGPointListPrototype::self(ctx));
    }
}

JSValue JSSVGPointList::create(JSContext* ctx, SVGPointList* impl, SVGElement* context)
{
    JSSVGPointList::init(ctx);
    JSValue _proto = JSSVGPointListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPointList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGPointList* imp = (SVGPointList*)JS_GetOpaque(this_val, JSSVGPointList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPointListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPointList* imp = (SVGPointList*)JS_GetOpaque(this_val, JSSVGPointList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPointList::ClearFuncNum: {
            return JSSVGPointList::clear(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::InitializeFuncNum: {
            return JSSVGPointList::initialize(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::GetItemFuncNum: {
            return JSSVGPointList::getItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::InsertItemBeforeFuncNum: {
            return JSSVGPointList::insertItemBefore(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::ReplaceItemFuncNum: {
            return JSSVGPointList::replaceItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::RemoveItemFuncNum: {
            return JSSVGPointList::removeItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPointList::AppendItemFuncNum: {
            return JSSVGPointList::appendItem(ctx, this_val, argc, argv, imp);
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
