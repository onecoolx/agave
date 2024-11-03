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

#include "QJSDOMImplementation.h"

#include "CSSStyleSheet.h"
#include "DOMImplementation.h"
#include "Document.h"
#include "DocumentType.h"
#include "HTMLDocument.h"
#include "QJSCSSStyleSheet.h"
#include "QJSDocument.h"
#include "QJSDocumentType.h"
#include "QJSHTMLDocument.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSDOMImplementationAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSDOMImplementation::getValueProperty, NULL, JSDOMImplementation::ConstructorAttrNum)
};

class JSDOMImplementationConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSDOMImplementationConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

JSValue JSDOMImplementationConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[DOMImplementation.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMImplementationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DOMImplementation.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDOMImplementationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static const JSCFunctionListEntry JSDOMImplementationPrototypeFunctions[] =
{
    JS_CFUNC_MAGIC_DEF("createDocumentType", 3, JSDOMImplementationPrototypeFunction::callAsFunction, JSDOMImplementation::CreateDocumentTypeFuncNum),
    JS_CFUNC_MAGIC_DEF("hasFeature", 2, JSDOMImplementationPrototypeFunction::callAsFunction, JSDOMImplementation::HasFeatureFuncNum),
    JS_CFUNC_MAGIC_DEF("createDocument", 3, JSDOMImplementationPrototypeFunction::callAsFunction, JSDOMImplementation::CreateDocumentFuncNum),
    JS_CFUNC_MAGIC_DEF("createCSSStyleSheet", 2, JSDOMImplementationPrototypeFunction::callAsFunction, JSDOMImplementation::CreateCSSStyleSheetFuncNum),
    JS_CFUNC_MAGIC_DEF("createHTMLDocument", 1, JSDOMImplementationPrototypeFunction::callAsFunction, JSDOMImplementation::CreateHTMLDocumentFuncNum)
};

JSValue JSDOMImplementationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMImplementation.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMImplementationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMImplementation.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSDOMImplementationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMImplementationAttributesFunctions, countof(JSDOMImplementationAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMImplementationPrototypeFunctions, countof(JSDOMImplementationPrototypeFunctions));
}

static JSClassDef JSDOMImplementationClassDefine = 
{
    "DOMImplementation",
    .finalizer = JSDOMImplementation::finalizer,
    .gc_mark = JSDOMImplementation::mark,
};

JSClassID JSDOMImplementation::js_class_id = 0;

void JSDOMImplementation::init(JSContext* ctx)
{
    if (JSDOMImplementation::js_class_id == 0) {
        JS_NewClassID(&JSDOMImplementation::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMImplementation::js_class_id, &JSDOMImplementationClassDefine);
        JS_SetConstructor(ctx, JSDOMImplementationConstructor::self(ctx), JSDOMImplementationPrototype::self(ctx));
        JS_SetClassProto(ctx, JSDOMImplementation::js_class_id, JSDOMImplementationPrototype::self(ctx));
    }
}

JSValue JSDOMImplementation::create(JSContext* ctx, DOMImplementation* impl)
{
    JSDOMImplementation::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSDOMImplementationPrototype::self(ctx), JSDOMImplementation::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSDOMImplementation::finalizer(JSRuntime* rt, JSValue val)
{
    DOMImplementation* impl = (DOMImplementation*)JS_GetOpaque(val, JSDOMImplementation::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSDOMImplementation::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSDOMImplementation::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSDOMImplementation::getConstructor(JSContext *ctx)
{
    return JSDOMImplementationConstructor::self(ctx);
}

JSValue JSDOMImplementationPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    DOMImplementation* imp = (DOMImplementation*)JS_GetOpaque2(ctx, this_val, JSDOMImplementation::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSDOMImplementation::HasFeatureFuncNum: {
            String feature = valueToString(ctx, argv[0]);
            String version = valueToStringWithNullCheck(ctx, argv[1]);

            JSValue result = JS_NewBool(ctx, imp->hasFeature(feature, version) ? 1 : 0);
            return result;
        }
        case JSDOMImplementation::CreateDocumentTypeFuncNum: {
            ExceptionCode ec = 0;
            String qualifiedName = valueToString(ctx, argv[0]);
            String publicId = valueToString(ctx, argv[1]);
            String systemId = valueToString(ctx, argv[2]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createDocumentType(qualifiedName, publicId, systemId, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDOMImplementation::CreateDocumentFuncNum: {
            ExceptionCode ec = 0;
            String namespaceURI = valueToStringWithNullCheck(ctx, argv[0]);
            String qualifiedName = valueToStringWithNullCheck(ctx, argv[1]);
            DocumentType* doctype = toDocumentType(argv[2]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createDocument(namespaceURI, qualifiedName, doctype, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDOMImplementation::CreateCSSStyleSheetFuncNum: {
            ExceptionCode ec = 0;
            String title = valueToString(ctx, argv[0]);
            String media = valueToString(ctx, argv[1]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createCSSStyleSheet(title, media, ec)));
            setDOMException(ctx, ec);
            return result;
        }
        case JSDOMImplementation::CreateHTMLDocumentFuncNum: {
            String title = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->createHTMLDocument(title)));
            return result;
        }
    }
    return JS_NULL;
}

JSValue toJS(JSContext *ctx, DOMImplementation* obj)
{
    return QJS::cacheDOMObject<DOMImplementation, JSDOMImplementation>(ctx, obj);
}

DOMImplementation* toDOMImplementation(JSValue val)
{
    if (JS_IsObject(val)) {
        DOMImplementation* impl = (DOMImplementation*)JS_GetOpaque(val, JSDOMImplementation::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}
