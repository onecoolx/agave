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

static const JSCFunctionListEntry JSSVGExceptionAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGException::getValueProperty, NULL, JSSVGException::ConstructorAttrNum)
};

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

static const JSCFunctionListEntry JSSVGExceptionConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_INVALID_VALUE_ERR", JSSVGExceptionConstructor::getValueProperty, NULL, SVGException::SVG_INVALID_VALUE_ERR),
    JS_CGETSET_MAGIC_DEF("SVG_WRONG_TYPE_ERR", JSSVGExceptionConstructor::getValueProperty, NULL, SVGException::SVG_WRONG_TYPE_ERR),
    JS_CGETSET_MAGIC_DEF("SVG_MATRIX_NOT_INVERTABLE", JSSVGExceptionConstructor::getValueProperty, NULL, SVGException::SVG_MATRIX_NOT_INVERTABLE)
};

JSValue JSSVGExceptionConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGException.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGExceptionConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGException.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGExceptionConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionConstructorFunctions, countof(JSSVGExceptionConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGExceptionPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_INVALID_VALUE_ERR", JSSVGExceptionPrototype::getValueProperty, NULL, SVGException::SVG_INVALID_VALUE_ERR),
    JS_CGETSET_MAGIC_DEF("SVG_WRONG_TYPE_ERR", JSSVGExceptionPrototype::getValueProperty, NULL, SVGException::SVG_WRONG_TYPE_ERR),
    JS_CGETSET_MAGIC_DEF("SVG_MATRIX_NOT_INVERTABLE", JSSVGExceptionPrototype::getValueProperty, NULL, SVGException::SVG_MATRIX_NOT_INVERTABLE)
};

JSValue JSSVGExceptionPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGException.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGExceptionPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGException.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGExceptionPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionAttributesFunctions, countof(JSSVGExceptionAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGExceptionPrototypeConstantsFunctions, countof(JSSVGExceptionPrototypeConstantsFunctions));
}

JSValue JSSVGExceptionPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGExceptionClassDefine = 
{
    "SVGException",
    .finalizer = JSSVGException::finalizer,
    .gc_mark = JSSVGException::mark,
};

JSClassID JSSVGException::js_class_id = 0;

void JSSVGException::init(JSContext* ctx)
{
    if (JSSVGException::js_class_id == 0) {
        JS_NewClassID(&JSSVGException::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGException::js_class_id, &JSSVGExceptionClassDefine);
        JS_SetConstructor(ctx, JSSVGExceptionConstructor::self(ctx), JSSVGExceptionPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGException::js_class_id, JSSVGExceptionPrototype::self(ctx));
    }
}

JSValue JSSVGException::create(JSContext* ctx, SVGException* impl, SVGElement* context)
{
    JSSVGException::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGExceptionPrototype::self(ctx), JSSVGException::js_class_id);
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
