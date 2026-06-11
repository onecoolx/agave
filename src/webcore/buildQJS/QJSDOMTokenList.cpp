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

#include "QJSDOMTokenList.h"

#include "DOMTokenList.h"
#include "ExceptionCode.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDOMTokenListAttributesFunctions[1];
static bool JSDOMTokenListAttributesFunctions_initialized = false;

static void init_JSDOMTokenListAttributesFunctions()
{
    if (JSDOMTokenListAttributesFunctions_initialized) return;
    JSDOMTokenListAttributesFunctions_initialized = true;
    memset(JSDOMTokenListAttributesFunctions, 0, sizeof(JSDOMTokenListAttributesFunctions));
    JSDOMTokenListAttributesFunctions[0].name = "length";
    JSDOMTokenListAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMTokenListAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMTokenListAttributesFunctions[0].magic = JSDOMTokenList::LengthAttrNum;
    JSDOMTokenListAttributesFunctions[0].u.getset.get.getter_magic = JSDOMTokenList::getValueProperty;
    JSDOMTokenListAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSDOMTokenListPrototypeFunctions[6];
static bool JSDOMTokenListPrototypeFunctions_initialized = false;

static void init_JSDOMTokenListPrototypeFunctions()
{
    if (JSDOMTokenListPrototypeFunctions_initialized) return;
    JSDOMTokenListPrototypeFunctions_initialized = true;
    memset(JSDOMTokenListPrototypeFunctions, 0, sizeof(JSDOMTokenListPrototypeFunctions));
    JSDOMTokenListPrototypeFunctions[0].name = "item";
    JSDOMTokenListPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[0].magic = JSDOMTokenList::ItemFuncNum;
    JSDOMTokenListPrototypeFunctions[0].u.func.length = 1;
    JSDOMTokenListPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[0].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
    JSDOMTokenListPrototypeFunctions[1].name = "contains";
    JSDOMTokenListPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[1].magic = JSDOMTokenList::ContainsFuncNum;
    JSDOMTokenListPrototypeFunctions[1].u.func.length = 1;
    JSDOMTokenListPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[1].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
    JSDOMTokenListPrototypeFunctions[2].name = "add";
    JSDOMTokenListPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[2].magic = JSDOMTokenList::AddFuncNum;
    JSDOMTokenListPrototypeFunctions[2].u.func.length = 1;
    JSDOMTokenListPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[2].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
    JSDOMTokenListPrototypeFunctions[3].name = "remove";
    JSDOMTokenListPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[3].magic = JSDOMTokenList::RemoveFuncNum;
    JSDOMTokenListPrototypeFunctions[3].u.func.length = 1;
    JSDOMTokenListPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[3].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
    JSDOMTokenListPrototypeFunctions[4].name = "toggle";
    JSDOMTokenListPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[4].magic = JSDOMTokenList::ToggleFuncNum;
    JSDOMTokenListPrototypeFunctions[4].u.func.length = 1;
    JSDOMTokenListPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[4].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
    JSDOMTokenListPrototypeFunctions[5].name = "toString";
    JSDOMTokenListPrototypeFunctions[5].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMTokenListPrototypeFunctions[5].def_type = JS_DEF_CFUNC;
    JSDOMTokenListPrototypeFunctions[5].magic = JSDOMTokenList::ToStringFuncNum;
    JSDOMTokenListPrototypeFunctions[5].u.func.length = 0;
    JSDOMTokenListPrototypeFunctions[5].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMTokenListPrototypeFunctions[5].u.func.cfunc.generic_magic = JSDOMTokenListPrototypeFunction::callAsFunction;
}

JSValue JSDOMTokenListPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMTokenList.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMTokenListPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMTokenList.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDOMTokenListPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDOMTokenListAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMTokenListAttributesFunctions, countof(JSDOMTokenListAttributesFunctions));
    init_JSDOMTokenListPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMTokenListPrototypeFunctions, countof(JSDOMTokenListPrototypeFunctions));
}

static JSClassDef JSDOMTokenListClassDefine;
static bool JSDOMTokenListClassDefine_initialized = false;

static void init_JSDOMTokenListClassDefine()
{
    if (JSDOMTokenListClassDefine_initialized) return;
    JSDOMTokenListClassDefine_initialized = true;
    memset(&JSDOMTokenListClassDefine, 0, sizeof(JSDOMTokenListClassDefine));
    JSDOMTokenListClassDefine.class_name = "DOMTokenList";
    JSDOMTokenListClassDefine.finalizer = JSDOMTokenList::finalizer;
    JSDOMTokenListClassDefine.gc_mark = JSDOMTokenList::mark;
}

JSClassID JSDOMTokenList::js_class_id = 0;

void JSDOMTokenList::init(JSContext* ctx)
{
    if (JSDOMTokenList::js_class_id == 0) {
        init_JSDOMTokenListClassDefine();
        JS_NewClassID(&JSDOMTokenList::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMTokenList::js_class_id, &JSDOMTokenListClassDefine);
        JS_SetClassProto(ctx, JSDOMTokenList::js_class_id, JSDOMTokenListPrototype::self(ctx));
    }
}

JSValue JSDOMTokenList::create(JSContext* ctx, DOMTokenList* impl)
{
    JSDOMTokenList::init(ctx);
    JSValue _proto = JSDOMTokenListPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSDOMTokenList::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDOMTokenList::finalizer(JSRuntime* rt, JSValue val)
{
    DOMTokenList* impl = (DOMTokenList*)JS_GetOpaque(val, JSDOMTokenList::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDOMTokenList::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSDOMTokenList::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            DOMTokenList* imp = (DOMTokenList*)JS_GetOpaque(this_val, JSDOMTokenList::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
    }
    return JS_NULL;
}

JSValue JSDOMTokenListPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMTokenList* imp = (DOMTokenList*)JS_GetOpaque(this_val, JSDOMTokenList::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDOMTokenList::ItemFuncNum: {
            bool indexOk;
            unsigned index = valueToInt32(ctx, argv[0], indexOk);
            if (!indexOk) {
                setDOMException(ctx, TYPE_MISMATCH_ERR);
                return JS_UNDEFINED;
            }

            JSValue result = jsStringOrNull(ctx, imp->item(index));
            return result;
        }
        case JSDOMTokenList::ContainsFuncNum: {
            String token = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->contains(token) ? 1 : 0);
            return result;
        }
        case JSDOMTokenList::AddFuncNum: {
            ExceptionCode ec = 0;
            String token = valueToString(ctx, argv[0]);
            imp->add(token, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSDOMTokenList::RemoveFuncNum: {
            ExceptionCode ec = 0;
            String token = valueToString(ctx, argv[0]);
            imp->remove(token, ec);
            setDOMException(ctx, ec);
            return JS_UNDEFINED;
        }
        case JSDOMTokenList::ToggleFuncNum: {
            ExceptionCode ec = 0;
            String token = valueToString(ctx, argv[0]);

            JSValue result = JS_NewBool(ctx, imp->toggle(token, ec) ? 1 : 0);
            setDOMException(ctx, ec);
            return result;
        }
        case JSDOMTokenList::ToStringFuncNum: {

            JSValue result = JS_NewString(ctx, ((const String&)imp->toString()).utf8().data());
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSDOMTokenList::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    DOMTokenList* imp = (DOMTokenList*)JS_GetOpaque2(ctx, this_obj, JSDOMTokenList::js_class_id);
    return jsStringOrNull(ctx, imp->item(idx));
}

JSValue toJS(JSContext *ctx, DOMTokenList* obj)
{
    return QJS::cacheDOMObject<DOMTokenList, JSDOMTokenList>(ctx, obj);
}

DOMTokenList* toDOMTokenList(JSValue val)
{
    if (JS_IsObject(val)) {
        DOMTokenList* impl = (DOMTokenList*)JS_GetOpaque(val, JSDOMTokenList::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
