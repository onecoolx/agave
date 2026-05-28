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

static JSCFunctionListEntry JSDOMImplementationAttributesFunctions[1];
static bool JSDOMImplementationAttributesFunctions_initialized = false;

static void init_JSDOMImplementationAttributesFunctions()
{
    if (JSDOMImplementationAttributesFunctions_initialized) return;
    JSDOMImplementationAttributesFunctions_initialized = true;
    memset(JSDOMImplementationAttributesFunctions, 0, sizeof(JSDOMImplementationAttributesFunctions));
    JSDOMImplementationAttributesFunctions[0].name = "constructor";
    JSDOMImplementationAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSDOMImplementationAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSDOMImplementationAttributesFunctions[0].magic = JSDOMImplementation::ConstructorAttrNum;
    JSDOMImplementationAttributesFunctions[0].u.getset.get.getter_magic = JSDOMImplementation::getValueProperty;
    JSDOMImplementationAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

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
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMImplementationConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[DOMImplementation.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDOMImplementationConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
}

/* Prototype functions table */

static JSCFunctionListEntry JSDOMImplementationPrototypeFunctions[5];
static bool JSDOMImplementationPrototypeFunctions_initialized = false;

static void init_JSDOMImplementationPrototypeFunctions()
{
    if (JSDOMImplementationPrototypeFunctions_initialized) return;
    JSDOMImplementationPrototypeFunctions_initialized = true;
    memset(JSDOMImplementationPrototypeFunctions, 0, sizeof(JSDOMImplementationPrototypeFunctions));
    JSDOMImplementationPrototypeFunctions[0].name = "createDocumentType";
    JSDOMImplementationPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMImplementationPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSDOMImplementationPrototypeFunctions[0].magic = JSDOMImplementation::CreateDocumentTypeFuncNum;
    JSDOMImplementationPrototypeFunctions[0].u.func.length = 3;
    JSDOMImplementationPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMImplementationPrototypeFunctions[0].u.func.cfunc.generic_magic = JSDOMImplementationPrototypeFunction::callAsFunction;
    JSDOMImplementationPrototypeFunctions[1].name = "hasFeature";
    JSDOMImplementationPrototypeFunctions[1].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMImplementationPrototypeFunctions[1].def_type = JS_DEF_CFUNC;
    JSDOMImplementationPrototypeFunctions[1].magic = JSDOMImplementation::HasFeatureFuncNum;
    JSDOMImplementationPrototypeFunctions[1].u.func.length = 2;
    JSDOMImplementationPrototypeFunctions[1].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMImplementationPrototypeFunctions[1].u.func.cfunc.generic_magic = JSDOMImplementationPrototypeFunction::callAsFunction;
    JSDOMImplementationPrototypeFunctions[2].name = "createDocument";
    JSDOMImplementationPrototypeFunctions[2].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMImplementationPrototypeFunctions[2].def_type = JS_DEF_CFUNC;
    JSDOMImplementationPrototypeFunctions[2].magic = JSDOMImplementation::CreateDocumentFuncNum;
    JSDOMImplementationPrototypeFunctions[2].u.func.length = 3;
    JSDOMImplementationPrototypeFunctions[2].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMImplementationPrototypeFunctions[2].u.func.cfunc.generic_magic = JSDOMImplementationPrototypeFunction::callAsFunction;
    JSDOMImplementationPrototypeFunctions[3].name = "createCSSStyleSheet";
    JSDOMImplementationPrototypeFunctions[3].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMImplementationPrototypeFunctions[3].def_type = JS_DEF_CFUNC;
    JSDOMImplementationPrototypeFunctions[3].magic = JSDOMImplementation::CreateCSSStyleSheetFuncNum;
    JSDOMImplementationPrototypeFunctions[3].u.func.length = 2;
    JSDOMImplementationPrototypeFunctions[3].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMImplementationPrototypeFunctions[3].u.func.cfunc.generic_magic = JSDOMImplementationPrototypeFunction::callAsFunction;
    JSDOMImplementationPrototypeFunctions[4].name = "createHTMLDocument";
    JSDOMImplementationPrototypeFunctions[4].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSDOMImplementationPrototypeFunctions[4].def_type = JS_DEF_CFUNC;
    JSDOMImplementationPrototypeFunctions[4].magic = JSDOMImplementation::CreateHTMLDocumentFuncNum;
    JSDOMImplementationPrototypeFunctions[4].u.func.length = 1;
    JSDOMImplementationPrototypeFunctions[4].u.func.cproto = JS_CFUNC_generic_magic;
    JSDOMImplementationPrototypeFunctions[4].u.func.cfunc.generic_magic = JSDOMImplementationPrototypeFunction::callAsFunction;
}

JSValue JSDOMImplementationPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSDOMImplementation.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSDOMImplementationPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSDOMImplementation.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSDOMImplementationPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSDOMImplementationAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMImplementationAttributesFunctions, countof(JSDOMImplementationAttributesFunctions));
    init_JSDOMImplementationPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSDOMImplementationPrototypeFunctions, countof(JSDOMImplementationPrototypeFunctions));
}

static JSClassDef JSDOMImplementationClassDefine;
static bool JSDOMImplementationClassDefine_initialized = false;

static void init_JSDOMImplementationClassDefine()
{
    if (JSDOMImplementationClassDefine_initialized) return;
    JSDOMImplementationClassDefine_initialized = true;
    memset(&JSDOMImplementationClassDefine, 0, sizeof(JSDOMImplementationClassDefine));
    JSDOMImplementationClassDefine.class_name = "DOMImplementation";
    JSDOMImplementationClassDefine.finalizer = JSDOMImplementation::finalizer;
    JSDOMImplementationClassDefine.gc_mark = JSDOMImplementation::mark;
}

JSClassID JSDOMImplementation::js_class_id = 0;

void JSDOMImplementation::init(JSContext* ctx)
{
    if (JSDOMImplementation::js_class_id == 0) {
        init_JSDOMImplementationClassDefine();
        JS_NewClassID(&JSDOMImplementation::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSDOMImplementation::js_class_id, &JSDOMImplementationClassDefine);
        JS_SetConstructor(ctx, JSDOMImplementationConstructor::self(ctx), JSDOMImplementationPrototype::self(ctx));
        JS_SetClassProto(ctx, JSDOMImplementation::js_class_id, JSDOMImplementationPrototype::self(ctx));
    }
}

JSValue JSDOMImplementation::create(JSContext* ctx, DOMImplementation* impl)
{
    JSDOMImplementation::init(ctx);
    JSValue _proto = JSDOMImplementationPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSDOMImplementation::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
    DOMImplementation* imp = (DOMImplementation*)JS_GetOpaque(this_val, JSDOMImplementation::js_class_id);
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
