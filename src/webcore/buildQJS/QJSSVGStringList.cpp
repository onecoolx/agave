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
#include "QJSSVGStringList.h"

#include "ExceptionCode.h"
#include "SVGStringList.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGStringListAttributesFunctions[1];
static bool JSSVGStringListAttributesFunctions_initialized = false;

static void init_JSSVGStringListAttributesFunctions()
{
    if (JSSVGStringListAttributesFunctions_initialized) return;
    JSSVGStringListAttributesFunctions_initialized = true;
    memset(JSSVGStringListAttributesFunctions, 0, sizeof(JSSVGStringListAttributesFunctions));
    JSSVGStringListAttributesFunctions[0].name = "numberOfItems";
    JSSVGStringListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGStringListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGStringListAttributesFunctions[0].magic = JSSVGStringList::NumberOfItemsAttrNum;
    JSSVGStringListAttributesFunctions[0].u.getset.get.getter_magic = JSSVGStringList::getValueProperty;
    JSSVGStringListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGStringListPrototypeFunctions[7];
static bool JSSVGStringListPrototypeFunctions_initialized = false;

static void init_JSSVGStringListPrototypeFunctions()
{
    if (JSSVGStringListPrototypeFunctions_initialized) return;
    JSSVGStringListPrototypeFunctions_initialized = true;
    memset(JSSVGStringListPrototypeFunctions, 0, sizeof(JSSVGStringListPrototypeFunctions));
    JSSVGStringListPrototypeFunctions[0].name = "clear";
    JSSVGStringListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[0].magic = JSSVGStringList::ClearFuncNum;
    JSSVGStringListPrototypeFunctions[0].u.func.length = 0;
    JSSVGStringListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[1].name = "initialize";
    JSSVGStringListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[1].magic = JSSVGStringList::InitializeFuncNum;
    JSSVGStringListPrototypeFunctions[1].u.func.length = 1;
    JSSVGStringListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[2].name = "getItem";
    JSSVGStringListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[2].magic = JSSVGStringList::GetItemFuncNum;
    JSSVGStringListPrototypeFunctions[2].u.func.length = 1;
    JSSVGStringListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[3].name = "insertItemBefore";
    JSSVGStringListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[3].magic = JSSVGStringList::InsertItemBeforeFuncNum;
    JSSVGStringListPrototypeFunctions[3].u.func.length = 2;
    JSSVGStringListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[4].name = "replaceItem";
    JSSVGStringListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[4].magic = JSSVGStringList::ReplaceItemFuncNum;
    JSSVGStringListPrototypeFunctions[4].u.func.length = 2;
    JSSVGStringListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[5].name = "removeItem";
    JSSVGStringListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[5].magic = JSSVGStringList::RemoveItemFuncNum;
    JSSVGStringListPrototypeFunctions[5].u.func.length = 1;
    JSSVGStringListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
    JSSVGStringListPrototypeFunctions[6].name = "appendItem";
    JSSVGStringListPrototypeFunctions[6].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGStringListPrototypeFunctions[6].def_type = JS_DEF_CFUNC;
    JSSVGStringListPrototypeFunctions[6].magic = JSSVGStringList::AppendItemFuncNum;
    JSSVGStringListPrototypeFunctions[6].u.func.length = 1;
    JSSVGStringListPrototypeFunctions[6].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGStringListPrototypeFunctions[6].u.func.cfunc.generic_magic = JSSVGStringListPrototypeFunction::callAsFunction;
}

JSValue JSSVGStringListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGStringList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGStringListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGStringList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGStringListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGStringListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStringListAttributesFunctions, countof(JSSVGStringListAttributesFunctions));
    init_JSSVGStringListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGStringListPrototypeFunctions, countof(JSSVGStringListPrototypeFunctions));
}

static JSClassDef JSSVGStringListClassDefine;
static bool JSSVGStringListClassDefine_initialized = false;

static void init_JSSVGStringListClassDefine()
{
    if (JSSVGStringListClassDefine_initialized) return;
    JSSVGStringListClassDefine_initialized = true;
    memset(&JSSVGStringListClassDefine, 0, sizeof(JSSVGStringListClassDefine));
    JSSVGStringListClassDefine.class_name = "SVGStringList";
    JSSVGStringListClassDefine.finalizer = JSSVGStringList::finalizer;
    JSSVGStringListClassDefine.gc_mark = JSSVGStringList::mark;
}

JSClassID JSSVGStringList::js_class_id = 0;

void JSSVGStringList::init(JSContext* ctx)
{
    if (JSSVGStringList::js_class_id == 0) {
        init_JSSVGStringListClassDefine();
        JS_NewClassID(&JSSVGStringList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGStringList::js_class_id, &JSSVGStringListClassDefine);
        JS_SetClassProto(ctx, JSSVGStringList::js_class_id, JSSVGStringListPrototype::self(ctx));
    }
}

JSValue JSSVGStringList::create(JSContext* ctx, SVGStringList* impl, SVGElement* context)
{
    JSSVGStringList::init(ctx);
    JSValue _proto = JSSVGStringListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGStringList::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGStringList::finalizer(JSRuntime* rt, JSValue val)
{
    SVGStringList* impl = (SVGStringList*)JS_GetOpaque(val, JSSVGStringList::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGStringList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGStringList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NumberOfItemsAttrNum: {
            SVGStringList* imp = (SVGStringList*)JS_GetOpaque(this_val, JSSVGStringList::js_class_id);
            return JS_NewInt32(ctx, imp->numberOfItems());
        }
    }
    return JS_NULL;
}

JSValue JSSVGStringListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGStringList* imp = (SVGStringList*)JS_GetOpaque(this_val, JSSVGStringList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGStringList::ClearFuncNum: {
            ExceptionCode ec = 0;
            imp->clear(ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSSVGStringList::InitializeFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->initialize(item, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::GetItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->getItem(index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::InsertItemBeforeFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->insertItemBefore(item, index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::ReplaceItemFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[1], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->replaceItem(item, index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::RemoveItemFuncNum: {
            ExceptionCode ec = 0;
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = JS_NewString(ctx, ((const String&)imp->removeItem(index, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
        case JSSVGStringList::AppendItemFuncNum: {
            ExceptionCode ec = 0;
            String item = valueToString(ctx, argv[0]);

            JSValue result = JS_NewString(ctx, ((const String&)imp->appendItem(item, ec)).utf8().data());
            setDOMException(ctx, ec);
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, SVGStringList* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGStringList, JSSVGStringList>(ctx, obj, context);
}

SVGStringList* toSVGStringList(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGStringList* impl = (SVGStringList*)JS_GetOpaque(val, JSSVGStringList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
