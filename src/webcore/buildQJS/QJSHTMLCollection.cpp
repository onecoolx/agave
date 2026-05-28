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

#include "QJSHTMLCollection.h"

#include "HTMLCollection.h"
#include "NameNodeList.h"
#include "Node.h"
#include "NodeList.h"
#include "QJSNode.h"
#include "QJSNodeList.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSHTMLCollectionAttributesFunctions[2];
static bool JSHTMLCollectionAttributesFunctions_initialized = false;

static void init_JSHTMLCollectionAttributesFunctions()
{
    if (JSHTMLCollectionAttributesFunctions_initialized) return;
    JSHTMLCollectionAttributesFunctions_initialized = true;
    memset(JSHTMLCollectionAttributesFunctions, 0, sizeof(JSHTMLCollectionAttributesFunctions));
    JSHTMLCollectionAttributesFunctions[0].name = "length";
    JSHTMLCollectionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLCollectionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLCollectionAttributesFunctions[0].magic = JSHTMLCollection::LengthAttrNum;
    JSHTMLCollectionAttributesFunctions[0].u.getset.get.getter_magic = JSHTMLCollection::getValueProperty;
    JSHTMLCollectionAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSHTMLCollectionAttributesFunctions[1].name = "constructor";
    JSHTMLCollectionAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSHTMLCollectionAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSHTMLCollectionAttributesFunctions[1].magic = JSHTMLCollection::ConstructorAttrNum;
    JSHTMLCollectionAttributesFunctions[1].u.getset.get.getter_magic = JSHTMLCollection::getValueProperty;
    JSHTMLCollectionAttributesFunctions[1].u.getset.set.setter_magic = NULL;
}

class JSHTMLCollectionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

JSValue JSHTMLCollectionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[HTMLCollection.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCollectionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[HTMLCollection.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLCollectionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSHTMLCollectionPrototypeFunctions[3];
static bool JSHTMLCollectionPrototypeFunctions_initialized = false;

static void init_JSHTMLCollectionPrototypeFunctions()
{
    if (JSHTMLCollectionPrototypeFunctions_initialized) return;
    JSHTMLCollectionPrototypeFunctions_initialized = true;
    memset(JSHTMLCollectionPrototypeFunctions, 0, sizeof(JSHTMLCollectionPrototypeFunctions));
    JSHTMLCollectionPrototypeFunctions[0].name = "namedItem";
    JSHTMLCollectionPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLCollectionPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSHTMLCollectionPrototypeFunctions[0].magic = JSHTMLCollection::NamedItemFuncNum;
    JSHTMLCollectionPrototypeFunctions[0].u.func.length = 1;
    JSHTMLCollectionPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLCollectionPrototypeFunctions[0].u.func.cfunc.generic_magic = JSHTMLCollectionPrototypeFunction::callAsFunction;
    JSHTMLCollectionPrototypeFunctions[1].name = "item";
    JSHTMLCollectionPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLCollectionPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSHTMLCollectionPrototypeFunctions[1].magic = JSHTMLCollection::ItemFuncNum;
    JSHTMLCollectionPrototypeFunctions[1].u.func.length = 1;
    JSHTMLCollectionPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLCollectionPrototypeFunctions[1].u.func.cfunc.generic_magic = JSHTMLCollectionPrototypeFunction::callAsFunction;
    JSHTMLCollectionPrototypeFunctions[2].name = "tags";
    JSHTMLCollectionPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSHTMLCollectionPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSHTMLCollectionPrototypeFunctions[2].magic = JSHTMLCollection::TagsFuncNum;
    JSHTMLCollectionPrototypeFunctions[2].u.func.length = 1;
    JSHTMLCollectionPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSHTMLCollectionPrototypeFunctions[2].u.func.cfunc.generic_magic = JSHTMLCollectionPrototypeFunction::callAsFunction;
}

JSValue JSHTMLCollectionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSHTMLCollection.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSHTMLCollectionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSHTMLCollection.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSHTMLCollectionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSHTMLCollectionAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCollectionAttributesFunctions, countof(JSHTMLCollectionAttributesFunctions));
    init_JSHTMLCollectionPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSHTMLCollectionPrototypeFunctions, countof(JSHTMLCollectionPrototypeFunctions));
}

static int js_htmlcollection_get_own_property(JSContext *ctx, JSPropertyDescriptor *desc,
                                               JSValueConst obj, JSAtom prop)
{
    HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(obj, JSHTMLCollection::js_class_id);
    if (!impl)
        return 0;
    const char* str = JS_AtomToCString(ctx, prop);
    if (!str)
        return 0;
    char* end;
    unsigned long index = strtoul(str, &end, 10);
    int is_index = (*end == '\0' && str[0] != '\0');
    JS_FreeCString(ctx, str);
    if (is_index && index < impl->length()) {
        if (desc) {
            desc->flags = JS_PROP_ENUMERABLE;
            desc->value = toJS(ctx, impl->item(index));
            desc->getter = JS_UNDEFINED;
            desc->setter = JS_UNDEFINED;
        }
        return 1;
    }
    return 0;
}

static JSClassExoticMethods js_htmlcollection_exotic;
static bool js_htmlcollection_exotic_initialized = false;

static void init_js_htmlcollection_exotic()
{
    if (js_htmlcollection_exotic_initialized) return;
    js_htmlcollection_exotic_initialized = true;
    memset(&js_htmlcollection_exotic, 0, sizeof(js_htmlcollection_exotic));
    js_htmlcollection_exotic.get_own_property = js_htmlcollection_get_own_property;
}

static JSClassDef JSHTMLCollectionClassDefine;
static bool JSHTMLCollectionClassDefine_initialized = false;

static void init_JSHTMLCollectionClassDefine()
{
    if (JSHTMLCollectionClassDefine_initialized) return;
    JSHTMLCollectionClassDefine_initialized = true;
    init_js_htmlcollection_exotic();
    memset(&JSHTMLCollectionClassDefine, 0, sizeof(JSHTMLCollectionClassDefine));
    JSHTMLCollectionClassDefine.class_name = "HTMLCollection";
    JSHTMLCollectionClassDefine.finalizer = JSHTMLCollection::finalizer;
    JSHTMLCollectionClassDefine.gc_mark = JSHTMLCollection::mark;
    JSHTMLCollectionClassDefine.exotic = &js_htmlcollection_exotic;
}

JSClassID JSHTMLCollection::js_class_id = 0;

void JSHTMLCollection::init(JSContext* ctx)
{
    if (JSHTMLCollection::js_class_id == 0) {
        init_JSHTMLCollectionClassDefine();
        JS_NewClassID(&JSHTMLCollection::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSHTMLCollection::js_class_id, &JSHTMLCollectionClassDefine);
        JS_SetClassProto(ctx, JSHTMLCollection::js_class_id, JSHTMLCollectionPrototype::self(ctx));
    }
}

JSValue JSHTMLCollection::create(JSContext* ctx, HTMLCollection* impl)
{
    JSHTMLCollection::init(ctx);
    JSValue _proto = JSHTMLCollectionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSHTMLCollection::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSHTMLCollection::finalizer(JSRuntime* rt, JSValue val)
{
    HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSHTMLCollection::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSHTMLCollection::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case LengthAttrNum: {
            HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
            return JS_NewInt32(ctx, imp->length());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSHTMLCollection::getConstructor(JSContext *ctx)
{
    return JSHTMLCollectionConstructor::self(ctx);
}

JSValue JSHTMLCollectionPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque(this_val, JSHTMLCollection::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSHTMLCollection::ItemFuncNum: {
            return JSHTMLCollection::item(ctx, this_val, argc, argv, imp);
        }
        case JSHTMLCollection::NamedItemFuncNum: {
            return JSHTMLCollection::namedItem(ctx, this_val, argc, argv, imp);
        }
        case JSHTMLCollection::TagsFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->tags(name)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue JSHTMLCollection::indexGetter(JSContext *ctx, JSValueConst this_obj, uint32_t idx)
{
    HTMLCollection* imp = (HTMLCollection*)JS_GetOpaque2(ctx, this_obj, JSHTMLCollection::js_class_id);
    return toJS(ctx, imp->item(idx));
}


HTMLCollection* toHTMLCollection(JSValue val)
{
    if (JS_IsObject(val)) {
        HTMLCollection* impl = (HTMLCollection*)JS_GetOpaque(val, JSHTMLCollection::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
