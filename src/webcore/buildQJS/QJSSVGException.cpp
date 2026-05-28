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
#include "QJSSVGException.h"

#include "SVGException.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGExceptionAttributesFunctions[1];
static bool JSSVGExceptionAttributesFunctions_initialized = false;

static void init_JSSVGExceptionAttributesFunctions()
{
    if (JSSVGExceptionAttributesFunctions_initialized) return;
    JSSVGExceptionAttributesFunctions_initialized = true;
    memset(JSSVGExceptionAttributesFunctions, 0, sizeof(JSSVGExceptionAttributesFunctions));
    JSSVGExceptionAttributesFunctions[0].name = "constructor";
    JSSVGExceptionAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionAttributesFunctions[0].magic = JSSVGException::ConstructorAttrNum;
    JSSVGExceptionAttributesFunctions[0].u.getset.get.getter_magic = JSSVGException::getValueProperty;
    JSSVGExceptionAttributesFunctions[0].u.getset.set.setter_magic = NULL;
}

class JSSVGExceptionConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGExceptionConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGExceptionConstructorFunctions[3];
static bool JSSVGExceptionConstructorFunctions_initialized = false;

static void init_JSSVGExceptionConstructorFunctions()
{
    if (JSSVGExceptionConstructorFunctions_initialized) return;
    JSSVGExceptionConstructorFunctions_initialized = true;
    memset(JSSVGExceptionConstructorFunctions, 0, sizeof(JSSVGExceptionConstructorFunctions));
    JSSVGExceptionConstructorFunctions[0].name = "SVG_INVALID_VALUE_ERR";
    JSSVGExceptionConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionConstructorFunctions[0].magic = SVGException::SVG_INVALID_VALUE_ERR;
    JSSVGExceptionConstructorFunctions[0].u.getset.get.getter_magic = JSSVGExceptionConstructor::getValueProperty;
    JSSVGExceptionConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGExceptionConstructorFunctions[1].name = "SVG_WRONG_TYPE_ERR";
    JSSVGExceptionConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionConstructorFunctions[1].magic = SVGException::SVG_WRONG_TYPE_ERR;
    JSSVGExceptionConstructorFunctions[1].u.getset.get.getter_magic = JSSVGExceptionConstructor::getValueProperty;
    JSSVGExceptionConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGExceptionConstructorFunctions[2].name = "SVG_MATRIX_NOT_INVERTABLE";
    JSSVGExceptionConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionConstructorFunctions[2].magic = SVGException::SVG_MATRIX_NOT_INVERTABLE;
    JSSVGExceptionConstructorFunctions[2].u.getset.get.getter_magic = JSSVGExceptionConstructor::getValueProperty;
    JSSVGExceptionConstructorFunctions[2].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGExceptionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGException.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGExceptionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGException.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGExceptionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGExceptionConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionConstructorFunctions, countof(JSSVGExceptionConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGExceptionPrototypeConstantsFunctions[3];
static bool JSSVGExceptionPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGExceptionPrototypeConstantsFunctions()
{
    if (JSSVGExceptionPrototypeConstantsFunctions_initialized) return;
    JSSVGExceptionPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGExceptionPrototypeConstantsFunctions, 0, sizeof(JSSVGExceptionPrototypeConstantsFunctions));
    JSSVGExceptionPrototypeConstantsFunctions[0].name = "SVG_INVALID_VALUE_ERR";
    JSSVGExceptionPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionPrototypeConstantsFunctions[0].magic = SVGException::SVG_INVALID_VALUE_ERR;
    JSSVGExceptionPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGExceptionPrototype::getValueProperty;
    JSSVGExceptionPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGExceptionPrototypeConstantsFunctions[1].name = "SVG_WRONG_TYPE_ERR";
    JSSVGExceptionPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionPrototypeConstantsFunctions[1].magic = SVGException::SVG_WRONG_TYPE_ERR;
    JSSVGExceptionPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGExceptionPrototype::getValueProperty;
    JSSVGExceptionPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGExceptionPrototypeConstantsFunctions[2].name = "SVG_MATRIX_NOT_INVERTABLE";
    JSSVGExceptionPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGExceptionPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGExceptionPrototypeConstantsFunctions[2].magic = SVGException::SVG_MATRIX_NOT_INVERTABLE;
    JSSVGExceptionPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGExceptionPrototype::getValueProperty;
    JSSVGExceptionPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGExceptionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGException.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGExceptionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGException.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGExceptionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGExceptionAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionAttributesFunctions, countof(JSSVGExceptionAttributesFunctions));
    init_JSSVGExceptionPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionPrototypeConstantsFunctions, countof(JSSVGExceptionPrototypeConstantsFunctions));
}

JSValue JSSVGExceptionPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGExceptionClassDefine;
static bool JSSVGExceptionClassDefine_initialized = false;

static void init_JSSVGExceptionClassDefine()
{
    if (JSSVGExceptionClassDefine_initialized) return;
    JSSVGExceptionClassDefine_initialized = true;
    memset(&JSSVGExceptionClassDefine, 0, sizeof(JSSVGExceptionClassDefine));
    JSSVGExceptionClassDefine.class_name = "SVGException";
    JSSVGExceptionClassDefine.finalizer = JSSVGException::finalizer;
    JSSVGExceptionClassDefine.gc_mark = JSSVGException::mark;
}

JSClassID JSSVGException::js_class_id = 0;

void JSSVGException::init(JSContext* ctx)
{
    if (JSSVGException::js_class_id == 0) {
        init_JSSVGExceptionClassDefine();
        JS_NewClassID(&JSSVGException::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGException::js_class_id, &JSSVGExceptionClassDefine);
        JS_SetConstructor(ctx, JSSVGExceptionConstructor::self(ctx), JSSVGExceptionPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGException::js_class_id, JSSVGExceptionPrototype::self(ctx));
    }
}

JSValue JSSVGException::create(JSContext* ctx, SVGException* impl, SVGElement* context)
{
    JSSVGException::init(ctx);
    JSValue _proto = JSSVGExceptionPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGException::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGException::finalizer(JSRuntime* rt, JSValue val)
{
    SVGException* impl = (SVGException*)JS_GetOpaque(val, JSSVGException::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGException::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGException::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGException::getConstructor(JSContext *ctx)
{
    return JSSVGExceptionConstructor::self(ctx);
}

JSValue toJS(JSContext *ctx, SVGException* obj, SVGElement* context)
{
    return QJS::cacheSVGDOMObject<SVGException, JSSVGException>(ctx, obj, context);
}

SVGException* toSVGException(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGException* impl = (SVGException*)JS_GetOpaque(val, JSSVGException::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
