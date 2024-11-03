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

static const JSCFunctionListEntry JSDocumentTypeAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("notations", JSDocumentType::getValueProperty, NULL, JSDocumentType::NotationsAttrNum),
    JS_CGETSET_MAGIC_DEF("name", JSDocumentType::getValueProperty, NULL, JSDocumentType::NameAttrNum),
    JS_CGETSET_MAGIC_DEF("entities", JSDocumentType::getValueProperty, NULL, JSDocumentType::EntitiesAttrNum),
    JS_CGETSET_MAGIC_DEF("systemId", JSDocumentType::getValueProperty, NULL, JSDocumentType::SystemIdAttrNum),
    JS_CGETSET_MAGIC_DEF("publicId", JSDocumentType::getValueProperty, NULL, JSDocumentType::PublicIdAttrNum),
    JS_CGETSET_MAGIC_DEF("internalSubset", JSDocumentType::getValueProperty, NULL, JSDocumentType::InternalSubsetAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSDocumentType::getValueProperty, NULL, JSDocumentType::ConstructorAttrNum)
};

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
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDocumentTypeConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DocumentType.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentTypeConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

JSValue JSDocumentTypePrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDocumentType.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSNodePrototype::self(ctx));
        JSDocumentTypePrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDocumentType.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDocumentTypePrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDocumentTypeAttributesFunctions, countof(JSDocumentTypeAttributesFunctions));
}

static JSClassDef JSDocumentTypeClassDefine = 
{
    "DocumentType",
    .finalizer = JSDocumentType::finalizer,
    .gc_mark = JSDocumentType::mark,
};

JSClassID JSDocumentType::js_class_id = 0;

void JSDocumentType::init(JSContext* ctx)
{
    if (JSDocumentType::js_class_id == 0) {
        JS_NewClassID(&JSDocumentType::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDocumentType::js_class_id, &JSDocumentTypeClassDefine);
        JS_SetConstructor(ctx, JSDocumentTypeConstructor::self(ctx), JSDocumentTypePrototype::self(ctx));
        JS_SetClassProto(ctx, JSDocumentType::js_class_id, JSDocumentTypePrototype::self(ctx));
    }
}

JSValue JSDocumentType::create(JSContext* ctx, DocumentType* impl)
{
    JSDocumentType::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDocumentTypePrototype::self(ctx), JSDocumentType::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDocumentType::finalizer(JSRuntime* rt, JSValue val)
{
    DocumentType* impl = (DocumentType*)JS_GetOpaque(val, JSDocumentType::js_class_id);
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
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->name()).utf8().data());
        }
        case EntitiesAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->entities()));
        }
        case NotationsAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->notations()));
        }
        case PublicIdAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
            return jsStringOrNull(ctx, imp->publicId());
        }
        case SystemIdAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
            return jsStringOrNull(ctx, imp->systemId());
        }
        case InternalSubsetAttrNum: {
            DocumentType* imp = (DocumentType*)JS_GetOpaque2(ctx, this_val, JSDocumentType::js_class_id);
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
