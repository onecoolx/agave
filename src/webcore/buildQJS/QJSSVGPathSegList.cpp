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
#include "QJSSVGPathSegList.h"

#include "SVGPathSegList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegListAttributesFunctions[1];
static bool JSSVGPathSegListAttributesFunctions_initialized = false;

static void init_JSSVGPathSegListAttributesFunctions()
{
    if (JSSVGPathSegListAttributesFunctions_initialized) return;
    JSSVGPathSegListAttributesFunctions_initialized = true;
    memset(JSSVGPathSegListAttributesFunctions, 0, sizeof(JSSVGPathSegListAttributesFunctions));
    JSSVGPathSegListAttributesFunctions[0].name = "numberOfItems";
    JSSVGPathSegListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegListAttributesFunctions[0].magic = JSSVGPathSegList::NumberOfItemsAttrNum;
    JSSVGPathSegListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSegList::getValueProperty;
    JSSVGPathSegListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGPathSegListPrototypeFunctions[7];
static bool JSSVGPathSegListPrototypeFunctions_initialized = false;

static void init_JSSVGPathSegListPrototypeFunctions()
{
    if (JSSVGPathSegListPrototypeFunctions_initialized) return;
    JSSVGPathSegListPrototypeFunctions_initialized = true;
    memset(JSSVGPathSegListPrototypeFunctions, 0, sizeof(JSSVGPathSegListPrototypeFunctions));
    JSSVGPathSegListPrototypeFunctions[0].name = "clear";
    JSSVGPathSegListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[0].magic = JSSVGPathSegList::ClearFuncNum;
    JSSVGPathSegListPrototypeFunctions[0].u.func.length = 0;
    JSSVGPathSegListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[1].name = "initialize";
    JSSVGPathSegListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[1].magic = JSSVGPathSegList::InitializeFuncNum;
    JSSVGPathSegListPrototypeFunctions[1].u.func.length = 1;
    JSSVGPathSegListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[2].name = "getItem";
    JSSVGPathSegListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[2].magic = JSSVGPathSegList::GetItemFuncNum;
    JSSVGPathSegListPrototypeFunctions[2].u.func.length = 1;
    JSSVGPathSegListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[3].name = "insertItemBefore";
    JSSVGPathSegListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[3].magic = JSSVGPathSegList::InsertItemBeforeFuncNum;
    JSSVGPathSegListPrototypeFunctions[3].u.func.length = 2;
    JSSVGPathSegListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[4].name = "replaceItem";
    JSSVGPathSegListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[4].magic = JSSVGPathSegList::ReplaceItemFuncNum;
    JSSVGPathSegListPrototypeFunctions[4].u.func.length = 2;
    JSSVGPathSegListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[5].name = "removeItem";
    JSSVGPathSegListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[5].magic = JSSVGPathSegList::RemoveItemFuncNum;
    JSSVGPathSegListPrototypeFunctions[5].u.func.length = 1;
    JSSVGPathSegListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
    JSSVGPathSegListPrototypeFunctions[6].name = "appendItem";
    JSSVGPathSegListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGPathSegListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGPathSegListPrototypeFunctions[6].magic = JSSVGPathSegList::AppendItemFuncNum;
    JSSVGPathSegListPrototypeFunctions[6].u.func.length = 1;
    JSSVGPathSegListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGPathSegListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGPathSegListPrototypeFunction::callAsFunction;
}

JSValue JSSVGPathSegListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSegList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSegList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegListAttributesFunctions, countof(JSSVGPathSegListAttributesFunctions));
    init_JSSVGPathSegListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegListPrototypeFunctions, countof(JSSVGPathSegListPrototypeFunctions));
}

static JSClassDef JSSVGPathSegListClassDefine;
static bool JSSVGPathSegListClassDefine_initialized = false;

static void init_JSSVGPathSegListClassDefine()
{
    if (JSSVGPathSegListClassDefine_initialized) return;
    JSSVGPathSegListClassDefine_initialized = true;
    memset(&JSSVGPathSegListClassDefine, 0, sizeof(JSSVGPathSegListClassDefine));
    JSSVGPathSegListClassDefine.class_name = "SVGPathSegList";
    JSSVGPathSegListClassDefine.finalizer = JSSVGPathSegList::finalizer;
    JSSVGPathSegListClassDefine.gc_mark = JSSVGPathSegList::mark;
}

JSClassID JSSVGPathSegList::js_class_id = 0;

void JSSVGPathSegList::init(JSContext* ctx)
{
    if (JSSVGPathSegList::js_class_id == 0) {
        init_JSSVGPathSegListClassDefine();
        JS_NewClassID(&JSSVGPathSegList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSegList::js_class_id, &JSSVGPathSegListClassDefine);
        JS_SetClassProto(ctx, JSSVGPathSegList::js_class_id, JSSVGPathSegListPrototype::self(ctx));
    }
}

JSValue JSSVGPathSegList::create(JSContext* ctx, SVGPathSegList* impl, SVGElement* context)
{
    JSSVGPathSegList::init(ctx);
    JSValue _proto = JSSVGPathSegListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSegList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGPathSegList* imp = (SVGPathSegList*)JS_GetOpaque(this_val, JSSVGPathSegList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGPathSegListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGPathSegList* imp = (SVGPathSegList*)JS_GetOpaque(this_val, JSSVGPathSegList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGPathSegList::ClearFuncNum: {
            return JSSVGPathSegList::clear(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::InitializeFuncNum: {
            return JSSVGPathSegList::initialize(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::GetItemFuncNum: {
            return JSSVGPathSegList::getItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::InsertItemBeforeFuncNum: {
            return JSSVGPathSegList::insertItemBefore(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::ReplaceItemFuncNum: {
            return JSSVGPathSegList::replaceItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::RemoveItemFuncNum: {
            return JSSVGPathSegList::removeItem(ctx, this_val, argc, argv, imp);
        }
        case JSSVGPathSegList::AppendItemFuncNum: {
            return JSSVGPathSegList::appendItem(ctx, this_val, argc, argv, imp);
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
