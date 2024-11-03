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


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGComponentTransferFunctionElement.h"

#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedNumberList.h"
#include "SVGComponentTransferFunctionElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static const JSCFunctionListEntry JSSVGComponentTransferFunctionElementAttributesFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("tableValues", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::TableValuesAttrNum),
    JS_CGETSET_MAGIC_DEF("amplitude", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::AmplitudeAttrNum),
    JS_CGETSET_MAGIC_DEF("type", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::TypeAttrNum),
    JS_CGETSET_MAGIC_DEF("slope", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::SlopeAttrNum),
    JS_CGETSET_MAGIC_DEF("intercept", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::InterceptAttrNum),
    JS_CGETSET_MAGIC_DEF("exponent", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::ExponentAttrNum),
    JS_CGETSET_MAGIC_DEF("offset", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::OffsetAttrNum),
    JS_CGETSET_MAGIC_DEF("constructor", JSSVGComponentTransferFunctionElement::getValueProperty, NULL, JSSVGComponentTransferFunctionElement::ConstructorAttrNum)
};

class JSSVGComponentTransferFunctionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGComponentTransferFunctionElementConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static const JSCFunctionListEntry JSSVGComponentTransferFunctionElementConstructorFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_TABLE", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_TABLE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_LINEAR", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_LINEAR),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_GAMMA", JSSVGComponentTransferFunctionElementConstructor::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_GAMMA)
};

JSValue JSSVGComponentTransferFunctionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGComponentTransferFunctionElement.constructor]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGComponentTransferFunctionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGComponentTransferFunctionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGComponentTransferFunctionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementConstructorFunctions, countof(JSSVGComponentTransferFunctionElementConstructorFunctions));
}

/* Functions table */

static const JSCFunctionListEntry JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[] =
{
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_TABLE", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_TABLE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_LINEAR", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_LINEAR),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY),
    JS_CGETSET_MAGIC_DEF("SVG_FECOMPONENTTRANSFER_TYPE_GAMMA", JSSVGComponentTransferFunctionElementPrototype::getValueProperty, NULL, WebCore::SVG_FECOMPONENTTRANSFER_TYPE_GAMMA)
};

JSValue JSSVGComponentTransferFunctionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGComponentTransferFunctionElement.prototype]]");
    if (JS_IsException(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGComponentTransferFunctionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGComponentTransferFunctionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    JS_FreeValue(ctx, obj);
    return obj;
}

void JSSVGComponentTransferFunctionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementAttributesFunctions, countof(JSSVGComponentTransferFunctionElementAttributesFunctions));
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions, countof(JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions));
}

JSValue JSSVGComponentTransferFunctionElementPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGComponentTransferFunctionElementClassDefine = 
{
    "SVGComponentTransferFunctionElement",
    .finalizer = JSSVGComponentTransferFunctionElement::finalizer,
    .gc_mark = JSSVGComponentTransferFunctionElement::mark,
};

JSClassID JSSVGComponentTransferFunctionElement::js_class_id = 0;

void JSSVGComponentTransferFunctionElement::init(JSContext* ctx)
{
    if (JSSVGComponentTransferFunctionElement::js_class_id == 0) {
        JS_NewClassID(&JSSVGComponentTransferFunctionElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGComponentTransferFunctionElement::js_class_id, &JSSVGComponentTransferFunctionElementClassDefine);
        JS_SetConstructor(ctx, JSSVGComponentTransferFunctionElementConstructor::self(ctx), JSSVGComponentTransferFunctionElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGComponentTransferFunctionElement::js_class_id, JSSVGComponentTransferFunctionElementPrototype::self(ctx));
    }
}

JSValue JSSVGComponentTransferFunctionElement::create(JSContext* ctx, SVGComponentTransferFunctionElement* impl)
{
    JSSVGComponentTransferFunctionElement::init(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, JSSVGComponentTransferFunctionElementPrototype::self(ctx), JSSVGComponentTransferFunctionElement::js_class_id);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGComponentTransferFunctionElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGComponentTransferFunctionElement* impl = (SVGComponentTransferFunctionElement*)JS_GetOpaque(val, JSSVGComponentTransferFunctionElement::js_class_id);
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGComponentTransferFunctionElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGComponentTransferFunctionElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case TypeAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case TableValuesAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->tableValuesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SlopeAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->slopeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case InterceptAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->interceptAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case AmplitudeAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->amplitudeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ExponentAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->exponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OffsetAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque2(ctx, this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->offsetAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGComponentTransferFunctionElement::getConstructor(JSContext *ctx)
{
    return JSSVGComponentTransferFunctionElementConstructor::self(ctx);
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
