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

static JSCFunctionListEntry JSSVGComponentTransferFunctionElementAttributesFunctions[8];
static bool JSSVGComponentTransferFunctionElementAttributesFunctions_initialized = false;

static void init_JSSVGComponentTransferFunctionElementAttributesFunctions()
{
    if (JSSVGComponentTransferFunctionElementAttributesFunctions_initialized) return;
    JSSVGComponentTransferFunctionElementAttributesFunctions_initialized = true;
    memset(JSSVGComponentTransferFunctionElementAttributesFunctions, 0, sizeof(JSSVGComponentTransferFunctionElementAttributesFunctions));
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].name = "tableValues";
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].magic = JSSVGComponentTransferFunctionElement::TableValuesAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].name = "amplitude";
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].magic = JSSVGComponentTransferFunctionElement::AmplitudeAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].name = "type";
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].magic = JSSVGComponentTransferFunctionElement::TypeAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].name = "slope";
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].magic = JSSVGComponentTransferFunctionElement::SlopeAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].name = "intercept";
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].magic = JSSVGComponentTransferFunctionElement::InterceptAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].name = "exponent";
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].magic = JSSVGComponentTransferFunctionElement::ExponentAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].name = "offset";
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].magic = JSSVGComponentTransferFunctionElement::OffsetAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].name = "constructor";
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].magic = JSSVGComponentTransferFunctionElement::ConstructorAttrNum;
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGComponentTransferFunctionElement::getValueProperty;
    JSSVGComponentTransferFunctionElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSSVGComponentTransferFunctionElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGComponentTransferFunctionElementConstructorFunctions[6];
static bool JSSVGComponentTransferFunctionElementConstructorFunctions_initialized = false;

static void init_JSSVGComponentTransferFunctionElementConstructorFunctions()
{
    if (JSSVGComponentTransferFunctionElementConstructorFunctions_initialized) return;
    JSSVGComponentTransferFunctionElementConstructorFunctions_initialized = true;
    memset(JSSVGComponentTransferFunctionElementConstructorFunctions, 0, sizeof(JSSVGComponentTransferFunctionElementConstructorFunctions));
    JSSVGComponentTransferFunctionElementConstructorFunctions[0].name = "SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN";
    JSSVGComponentTransferFunctionElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN;
    JSSVGComponentTransferFunctionElementConstructorFunctions[1].name = "SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY";
    JSSVGComponentTransferFunctionElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY;
    JSSVGComponentTransferFunctionElementConstructorFunctions[2].name = "SVG_FECOMPONENTTRANSFER_TYPE_TABLE";
    JSSVGComponentTransferFunctionElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_TABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[3].name = "SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE";
    JSSVGComponentTransferFunctionElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[4].name = "SVG_FECOMPONENTTRANSFER_TYPE_LINEAR";
    JSSVGComponentTransferFunctionElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_LINEAR;
    JSSVGComponentTransferFunctionElementConstructorFunctions[5].name = "SVG_FECOMPONENTTRANSFER_TYPE_GAMMA";
    JSSVGComponentTransferFunctionElementConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementConstructorFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementConstructorFunctions[5].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_GAMMA;
}

JSValue JSSVGComponentTransferFunctionElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGComponentTransferFunctionElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGComponentTransferFunctionElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGComponentTransferFunctionElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGComponentTransferFunctionElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGComponentTransferFunctionElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementConstructorFunctions, countof(JSSVGComponentTransferFunctionElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[6];
static bool JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions()
{
    if (JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions_initialized) return;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions, 0, sizeof(JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions));
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[0].name = "SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_UNKNOWN;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[1].name = "SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_IDENTITY;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[2].name = "SVG_FECOMPONENTTRANSFER_TYPE_TABLE";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_TABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[3].name = "SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_DISCRETE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[4].name = "SVG_FECOMPONENTTRANSFER_TYPE_LINEAR";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_LINEAR;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[5].name = "SVG_FECOMPONENTTRANSFER_TYPE_GAMMA";
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[5].def_type = JS_DEF_PROP_INT32;
    JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions[5].u.i32 = (int32_t)WebCore::SVG_FECOMPONENTTRANSFER_TYPE_GAMMA;
}

JSValue JSSVGComponentTransferFunctionElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGComponentTransferFunctionElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGComponentTransferFunctionElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGComponentTransferFunctionElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGComponentTransferFunctionElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGComponentTransferFunctionElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementAttributesFunctions, countof(JSSVGComponentTransferFunctionElementAttributesFunctions));
    init_JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions, countof(JSSVGComponentTransferFunctionElementPrototypeConstantsFunctions));
}

static JSClassDef JSSVGComponentTransferFunctionElementClassDefine;
static bool JSSVGComponentTransferFunctionElementClassDefine_initialized = false;

static void init_JSSVGComponentTransferFunctionElementClassDefine()
{
    if (JSSVGComponentTransferFunctionElementClassDefine_initialized) return;
    JSSVGComponentTransferFunctionElementClassDefine_initialized = true;
    memset(&JSSVGComponentTransferFunctionElementClassDefine, 0, sizeof(JSSVGComponentTransferFunctionElementClassDefine));
    JSSVGComponentTransferFunctionElementClassDefine.class_name = "SVGComponentTransferFunctionElement";
    JSSVGComponentTransferFunctionElementClassDefine.finalizer = JSSVGComponentTransferFunctionElement::finalizer;
    JSSVGComponentTransferFunctionElementClassDefine.gc_mark = JSSVGComponentTransferFunctionElement::mark;
}

JSClassID JSSVGComponentTransferFunctionElement::js_class_id = 0;

void JSSVGComponentTransferFunctionElement::init(JSContext* ctx)
{
    if (JSSVGComponentTransferFunctionElement::js_class_id == 0) {
        init_JSSVGComponentTransferFunctionElementClassDefine();
        JS_NewClassID(&JSSVGComponentTransferFunctionElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGComponentTransferFunctionElement::js_class_id, &JSSVGComponentTransferFunctionElementClassDefine);
        JS_SetConstructor(ctx, JSSVGComponentTransferFunctionElementConstructor::self(ctx), JSSVGComponentTransferFunctionElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGComponentTransferFunctionElement::js_class_id, JSSVGComponentTransferFunctionElementPrototype::self(ctx));
    }
}

JSValue JSSVGComponentTransferFunctionElement::create(JSContext* ctx, SVGComponentTransferFunctionElement* impl)
{
    JSSVGComponentTransferFunctionElement::init(ctx);
    JSValue _proto = JSSVGComponentTransferFunctionElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGComponentTransferFunctionElement::js_class_id);
    JS_FreeValue(ctx, _proto);
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
    if (!impl)
        return;
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
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case TableValuesAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->tableValuesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SlopeAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->slopeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case InterceptAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->interceptAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case AmplitudeAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->amplitudeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ExponentAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->exponentAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case OffsetAttrNum: {
            SVGComponentTransferFunctionElement* imp = (SVGComponentTransferFunctionElement*)JS_GetOpaque(this_val, JSSVGComponentTransferFunctionElement::js_class_id);
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
