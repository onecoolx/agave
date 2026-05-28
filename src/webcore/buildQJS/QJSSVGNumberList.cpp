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
#include "QJSSVGNumberList.h"

#include "ExceptionCode.h"
#include "QJSSVGNumber.h"
#include "SVGNumberList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGNumberListAttributesFunctions[1];
static bool JSSVGNumberListAttributesFunctions_initialized = false;

static void init_JSSVGNumberListAttributesFunctions()
{
    if (JSSVGNumberListAttributesFunctions_initialized) return;
    JSSVGNumberListAttributesFunctions_initialized = true;
    memset(JSSVGNumberListAttributesFunctions, 0, sizeof(JSSVGNumberListAttributesFunctions));
    JSSVGNumberListAttributesFunctions[0].name = "numberOfItems";
    JSSVGNumberListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGNumberListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGNumberListAttributesFunctions[0].magic = JSSVGNumberList::NumberOfItemsAttrNum;
    JSSVGNumberListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGNumberList::getValueProperty;
    JSSVGNumberListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGNumberListPrototypeFunctions[7];
static bool JSSVGNumberListPrototypeFunctions_initialized = false;

static void init_JSSVGNumberListPrototypeFunctions()
{
    if (JSSVGNumberListPrototypeFunctions_initialized) return;
    JSSVGNumberListPrototypeFunctions_initialized = true;
    memset(JSSVGNumberListPrototypeFunctions, 0, sizeof(JSSVGNumberListPrototypeFunctions));
    JSSVGNumberListPrototypeFunctions[0].name = "clear";
    JSSVGNumberListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[0].magic = JSSVGNumberList::ClearFuncNum;
    JSSVGNumberListPrototypeFunctions[0].u.func.length = 0;
    JSSVGNumberListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[1].name = "initialize";
    JSSVGNumberListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[1].magic = JSSVGNumberList::InitializeFuncNum;
    JSSVGNumberListPrototypeFunctions[1].u.func.length = 1;
    JSSVGNumberListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[2].name = "getItem";
    JSSVGNumberListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[2].magic = JSSVGNumberList::GetItemFuncNum;
    JSSVGNumberListPrototypeFunctions[2].u.func.length = 1;
    JSSVGNumberListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[3].name = "insertItemBefore";
    JSSVGNumberListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[3].magic = JSSVGNumberList::InsertItemBeforeFuncNum;
    JSSVGNumberListPrototypeFunctions[3].u.func.length = 2;
    JSSVGNumberListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[4].name = "replaceItem";
    JSSVGNumberListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[4].magic = JSSVGNumberList::ReplaceItemFuncNum;
    JSSVGNumberListPrototypeFunctions[4].u.func.length = 2;
    JSSVGNumberListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[5].name = "removeItem";
    JSSVGNumberListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[5].magic = JSSVGNumberList::RemoveItemFuncNum;
    JSSVGNumberListPrototypeFunctions[5].u.func.length = 1;
    JSSVGNumberListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
    JSSVGNumberListPrototypeFunctions[6].name = "appendItem";
    JSSVGNumberListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGNumberListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGNumberListPrototypeFunctions[6].magic = JSSVGNumberList::AppendItemFuncNum;
    JSSVGNumberListPrototypeFunctions[6].u.func.length = 1;
    JSSVGNumberListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGNumberListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGNumberListPrototypeFunction::callAsFunction;
}

JSValue JSSVGNumberListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGNumberList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGNumberListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGNumberList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGNumberListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGNumberListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGNumberListAttributesFunctions, countof(JSSVGNumberListAttributesFunctions));
    init_JSSVGNumberListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGNumberListPrototypeFunctions, countof(JSSVGNumberListPrototypeFunctions));
}

static JSClassDef JSSVGNumberListClassDefine;
static bool JSSVGNumberListClassDefine_initialized = false;

static void init_JSSVGNumberListClassDefine()
{
    if (JSSVGNumberListClassDefine_initialized) return;
    JSSVGNumberListClassDefine_initialized = true;
    memset(&JSSVGNumberListClassDefine, 0, sizeof(JSSVGNumberListClassDefine));
    JSSVGNumberListClassDefine.class_name = "SVGNumberList";
    JSSVGNumberListClassDefine.finalizer = JSSVGNumberList::finalizer;
    JSSVGNumberListClassDefine.gc_mark = JSSVGNumberList::mark;
}

JSClassID JSSVGNumberList::js_class_id = 0;

void JSSVGNumberList::init(JSContext* ctx)
{
    if (JSSVGNumberList::js_class_id == 0) {
        init_JSSVGNumberListClassDefine();
        JS_NewClassID(&JSSVGNumberList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGNumberList::js_class_id, &JSSVGNumberListClassDefine);
        JS_SetClassProto(ctx, JSSVGNumberList::js_class_id, JSSVGNumberListPrototype::self(ctx));
    }
}

JSValue JSSVGNumberList::create(JSContext* ctx, SVGNumberList* impl, SVGElement* context)
{
    JSSVGNumberList::init(ctx);
    JSValue _proto = JSSVGNumberListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGNumberList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGNumberList* imp = (SVGNumberList*)JS_GetOpaque(this_val, JSSVGNumberList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGNumberListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGNumberList* imp = (SVGNumberList*)JS_GetOpaque(this_val, JSSVGNumberList::js_class_id);
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
