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

#include "QJSDocumentType.h"

#include "DocumentType.h"
#include "NamedAttrMap.h"
#include "NamedNodeMap.h"
#include "QJSNamedNodeMap.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSDocumentTypeAttributesFunctions[7];
static bool JSDocumentTypeAttributesFunctions_initialized = false;

static void init_JSDocumentTypeAttributesFunctions()
{
    if (JSDocumentTypeAttributesFunctions_initialized) return;
    JSDocumentTypeAttributesFunctions_initialized = true;
    memset(JSDocumentTypeAttributesFunctions, 0, sizeof(JSDocumentTypeAttributesFunctions));
    JSDocumentTypeAttributesFunctions[0].name = "notations";
    JSDocumentTypeAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[0].magic = JSDocumentType::NotationsAttrNum;
    JSDocumentTypeAttributesFunctions[0].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[1].name = "name";
    JSDocumentTypeAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[1].magic = JSDocumentType::NameAttrNum;
    JSDocumentTypeAttributesFunctions[1].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[2].name = "entities";
    JSDocumentTypeAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[2].magic = JSDocumentType::EntitiesAttrNum;
    JSDocumentTypeAttributesFunctions[2].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[3].name = "systemId";
    JSDocumentTypeAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[3].magic = JSDocumentType::SystemIdAttrNum;
    JSDocumentTypeAttributesFunctions[3].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[4].name = "publicId";
    JSDocumentTypeAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[4].magic = JSDocumentType::PublicIdAttrNum;
    JSDocumentTypeAttributesFunctions[4].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[5].name = "internalSubset";
    JSDocumentTypeAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[5].magic = JSDocumentType::InternalSubsetAttrNum;
    JSDocumentTypeAttributesFunctions[5].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSDocumentTypeAttributesFunctions[6].name = "constructor";
    JSDocumentTypeAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSDocumentTypeAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSDocumentTypeAttributesFunctions[6].magic = JSDocumentType::ConstructorAttrNum;
    JSDocumentTypeAttributesFunctions[6].u.getset.get.getter_magic = JSDocumentType::getValueProperty;
    JSDocumentTypeAttributesFunctions[6].u.getset.set.setter_magic = NULL;
}

class JSDocumentTypeConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSDocumentTypeConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSDocumentTypeConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[DocumentType.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentTypeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DocumentType.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentTypeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSDocumentTypePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocumentType.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSDocumentTypePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocumentType.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDocumentTypePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDocumentTypeAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentTypeAttributesFunctions, countof(JSDocumentTypeAttributesFunctions));
}

static JSClassDef JSDocumentTypeClassDefine;
static bool JSDocumentTypeClassDefine_initialized = false;

static void init_JSDocumentTypeClassDefine()
{
    if (JSDocumentTypeClassDefine_initialized) return;
    JSDocumentTypeClassDefine_initialized = true;
    memset(&JSDocumentTypeClassDefine, 0, sizeof(JSDocumentTypeClassDefine));
    JSDocumentTypeClassDefine.class_name = "DocumentType";
    JSDocumentTypeClassDefine.finalizer = JSDocumentType::finalizer;
    JSDocumentTypeClassDefine.gc_mark = JSDocumentType::mark;
}

JSClassID JSDocumentType::js_class_id = 0;

void JSDocumentType::init(JSContext* ctx)
{
    if (JSDocumentType::js_class_id == 0) {
        JSNode::init(ctx);
        JSDocumentType::js_class_id = JSNode::js_class_id;
    }
}

JSValue JSDocumentType::create(JSContext* ctx, DocumentType* impl)
{
    JSDocumentType::init(ctx);
    JSValue _proto = JSDocumentTypePrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSNode::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocumentType::finalizer(JSRuntime* rt, JSValue val)
{
    DocumentType* impl = (DocumentType*)JS_GetOpaque(val, JSNode::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDocumentType::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSNode::mark(rt, val, mark_func);
}

JSValue JSDocumentType::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case NameAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case EntitiesAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->entities()));
        }
        case NotationsAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->notations()));
        }
        case PublicIdAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->systemId());
        }
        case InternalSubsetAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque(this_val, JSNode::js_class_id);
            return jsStringOrNull(ctx, imp->internalSubset());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSDocumentType::getConstructor(JSContext *ctx)
{
    return JSDocumentTypeConstructor::self(ctx);
}


DocumentType* toDocumentType(JSValue val)
{
    if (JS_IsObject(val)) {
        DocumentType* impl = (DocumentType*)JS_GetOpaque(val, JSDocumentType::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
