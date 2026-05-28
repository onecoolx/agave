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
#include "QJSSVGLengthList.h"

#include "ExceptionCode.h"
#include "QJSSVGLength.h"
#include "SVGLength.h"
#include "SVGLengthList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGLengthListAttributesFunctions[1];
static bool JSSVGLengthListAttributesFunctions_initialized = false;

static void init_JSSVGLengthListAttributesFunctions()
{
    if (JSSVGLengthListAttributesFunctions_initialized) return;
    JSSVGLengthListAttributesFunctions_initialized = true;
    memset(JSSVGLengthListAttributesFunctions, 0, sizeof(JSSVGLengthListAttributesFunctions));
    JSSVGLengthListAttributesFunctions[0].name = "numberOfItems";
    JSSVGLengthListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGLengthListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGLengthListAttributesFunctions[0].magic = JSSVGLengthList::NumberOfItemsAttrNum;
    JSSVGLengthListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGLengthList::getValueProperty;
    JSSVGLengthListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGLengthListPrototypeFunctions[7];
static bool JSSVGLengthListPrototypeFunctions_initialized = false;

static void init_JSSVGLengthListPrototypeFunctions()
{
    if (JSSVGLengthListPrototypeFunctions_initialized) return;
    JSSVGLengthListPrototypeFunctions_initialized = true;
    memset(JSSVGLengthListPrototypeFunctions, 0, sizeof(JSSVGLengthListPrototypeFunctions));
    JSSVGLengthListPrototypeFunctions[0].name = "clear";
    JSSVGLengthListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[0].magic = JSSVGLengthList::ClearFuncNum;
    JSSVGLengthListPrototypeFunctions[0].u.func.length = 0;
    JSSVGLengthListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[1].name = "getItem";
    JSSVGLengthListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[1].magic = JSSVGLengthList::GetItemFuncNum;
    JSSVGLengthListPrototypeFunctions[1].u.func.length = 1;
    JSSVGLengthListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[2].name = "insertItemBefore";
    JSSVGLengthListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[2].magic = JSSVGLengthList::InsertItemBeforeFuncNum;
    JSSVGLengthListPrototypeFunctions[2].u.func.length = 2;
    JSSVGLengthListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[3].name = "initialize";
    JSSVGLengthListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[3].magic = JSSVGLengthList::InitializeFuncNum;
    JSSVGLengthListPrototypeFunctions[3].u.func.length = 1;
    JSSVGLengthListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[4].name = "replaceItem";
    JSSVGLengthListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[4].magic = JSSVGLengthList::ReplaceItemFuncNum;
    JSSVGLengthListPrototypeFunctions[4].u.func.length = 2;
    JSSVGLengthListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[5].name = "removeItem";
    JSSVGLengthListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[5].magic = JSSVGLengthList::RemoveItemFuncNum;
    JSSVGLengthListPrototypeFunctions[5].u.func.length = 1;
    JSSVGLengthListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
    JSSVGLengthListPrototypeFunctions[6].name = "appendItem";
    JSSVGLengthListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGLengthListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGLengthListPrototypeFunctions[6].magic = JSSVGLengthList::AppendItemFuncNum;
    JSSVGLengthListPrototypeFunctions[6].u.func.length = 1;
    JSSVGLengthListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGLengthListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGLengthListPrototypeFunction::callAsFunction;
}

JSValue JSSVGLengthListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGLengthList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGLengthListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGLengthList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGLengthListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGLengthListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthListAttributesFunctions, countof(JSSVGLengthListAttributesFunctions));
    init_JSSVGLengthListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGLengthListPrototypeFunctions, countof(JSSVGLengthListPrototypeFunctions));
}

static JSClassDef JSSVGLengthListClassDefine;
static bool JSSVGLengthListClassDefine_initialized = false;

static void init_JSSVGLengthListClassDefine()
{
    if (JSSVGLengthListClassDefine_initialized) return;
    JSSVGLengthListClassDefine_initialized = true;
    memset(&JSSVGLengthListClassDefine, 0, sizeof(JSSVGLengthListClassDefine));
    JSSVGLengthListClassDefine.class_name = "SVGLengthList";
    JSSVGLengthListClassDefine.finalizer = JSSVGLengthList::finalizer;
    JSSVGLengthListClassDefine.gc_mark = JSSVGLengthList::mark;
}

JSClassID JSSVGLengthList::js_class_id = 0;

void JSSVGLengthList::init(JSContext* ctx)
{
    if (JSSVGLengthList::js_class_id == 0) {
        init_JSSVGLengthListClassDefine();
        JS_NewClassID(&JSSVGLengthList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGLengthList::js_class_id, &JSSVGLengthListClassDefine);
        JS_SetClassProto(ctx, JSSVGLengthList::js_class_id, JSSVGLengthListPrototype::self(ctx));
    }
}

JSValue JSSVGLengthList::create(JSContext* ctx, SVGLengthList* impl, SVGElement* context)
{
    JSSVGLengthList::init(ctx);
    JSValue _proto = JSSVGLengthListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGLengthList::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGLengthList* imp = (SVGLengthList*)JS_GetOpaque(this_val, JSSVGLengthList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGLengthListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGLengthList* imp = (SVGLengthList*)JS_GetOpaque(this_val, JSSVGLengthList::js_class_id);
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
