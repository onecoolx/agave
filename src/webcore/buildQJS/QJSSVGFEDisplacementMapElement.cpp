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
#include "QJSSVGFEDisplacementMapElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEDisplacementMapElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEDisplacementMapElementAttributesFunctions[13];
static bool JSSVGFEDisplacementMapElementAttributesFunctions_initialized = false;

static void init_JSSVGFEDisplacementMapElementAttributesFunctions()
{
    if (JSSVGFEDisplacementMapElementAttributesFunctions_initialized) return;
    JSSVGFEDisplacementMapElementAttributesFunctions_initialized = true;
    memset(JSSVGFEDisplacementMapElementAttributesFunctions, 0, sizeof(JSSVGFEDisplacementMapElementAttributesFunctions));
    JSSVGFEDisplacementMapElementAttributesFunctions[0].name = "height";
    JSSVGFEDisplacementMapElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[0].magic = JSSVGFEDisplacementMapElement::HeightAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[1].name = "constructor";
    JSSVGFEDisplacementMapElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[1].magic = JSSVGFEDisplacementMapElement::ConstructorAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[2].name = "xChannelSelector";
    JSSVGFEDisplacementMapElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[2].magic = JSSVGFEDisplacementMapElement::XChannelSelectorAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[3].name = "width";
    JSSVGFEDisplacementMapElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[3].magic = JSSVGFEDisplacementMapElement::WidthAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[4].name = "in2";
    JSSVGFEDisplacementMapElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[4].magic = JSSVGFEDisplacementMapElement::In2AttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[5].name = "x";
    JSSVGFEDisplacementMapElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[5].magic = JSSVGFEDisplacementMapElement::XAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[6].name = "style";
    JSSVGFEDisplacementMapElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[6].magic = JSSVGFEDisplacementMapElement::StyleAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[7].name = "className";
    JSSVGFEDisplacementMapElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[7].magic = JSSVGFEDisplacementMapElement::ClassNameAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[8].name = "in1";
    JSSVGFEDisplacementMapElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[8].magic = JSSVGFEDisplacementMapElement::In1AttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[9].name = "yChannelSelector";
    JSSVGFEDisplacementMapElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[9].magic = JSSVGFEDisplacementMapElement::YChannelSelectorAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[10].name = "scale";
    JSSVGFEDisplacementMapElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[10].magic = JSSVGFEDisplacementMapElement::ScaleAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[11].name = "y";
    JSSVGFEDisplacementMapElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[11].magic = JSSVGFEDisplacementMapElement::YAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGFEDisplacementMapElementAttributesFunctions[12].name = "result";
    JSSVGFEDisplacementMapElementAttributesFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementAttributesFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDisplacementMapElementAttributesFunctions[12].magic = JSSVGFEDisplacementMapElement::ResultAttrNum;
    JSSVGFEDisplacementMapElementAttributesFunctions[12].u.getset.get.getter_magic = JSSVGFEDisplacementMapElement::getValueProperty;
    JSSVGFEDisplacementMapElementAttributesFunctions[12].u.getset.set.setter_magic = NULL;
}

class JSSVGFEDisplacementMapElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGFEDisplacementMapElementConstructorFunctions[5];
static bool JSSVGFEDisplacementMapElementConstructorFunctions_initialized = false;

static void init_JSSVGFEDisplacementMapElementConstructorFunctions()
{
    if (JSSVGFEDisplacementMapElementConstructorFunctions_initialized) return;
    JSSVGFEDisplacementMapElementConstructorFunctions_initialized = true;
    memset(JSSVGFEDisplacementMapElementConstructorFunctions, 0, sizeof(JSSVGFEDisplacementMapElementConstructorFunctions));
    JSSVGFEDisplacementMapElementConstructorFunctions[0].name = "SVG_CHANNEL_UNKNOWN";
    JSSVGFEDisplacementMapElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementConstructorFunctions[0].u.i32 = (int32_t)WebCore::SVG_CHANNEL_UNKNOWN;
    JSSVGFEDisplacementMapElementConstructorFunctions[1].name = "SVG_CHANNEL_R";
    JSSVGFEDisplacementMapElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementConstructorFunctions[1].u.i32 = (int32_t)WebCore::SVG_CHANNEL_R;
    JSSVGFEDisplacementMapElementConstructorFunctions[2].name = "SVG_CHANNEL_G";
    JSSVGFEDisplacementMapElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementConstructorFunctions[2].u.i32 = (int32_t)WebCore::SVG_CHANNEL_G;
    JSSVGFEDisplacementMapElementConstructorFunctions[3].name = "SVG_CHANNEL_B";
    JSSVGFEDisplacementMapElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementConstructorFunctions[3].u.i32 = (int32_t)WebCore::SVG_CHANNEL_B;
    JSSVGFEDisplacementMapElementConstructorFunctions[4].name = "SVG_CHANNEL_A";
    JSSVGFEDisplacementMapElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementConstructorFunctions[4].u.i32 = (int32_t)WebCore::SVG_CHANNEL_A;
}

JSValue JSSVGFEDisplacementMapElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEDisplacementMapElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEDisplacementMapElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEDisplacementMapElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEDisplacementMapElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEDisplacementMapElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementConstructorFunctions, countof(JSSVGFEDisplacementMapElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[5];
static bool JSSVGFEDisplacementMapElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGFEDisplacementMapElementPrototypeConstantsFunctions()
{
    if (JSSVGFEDisplacementMapElementPrototypeConstantsFunctions_initialized) return;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGFEDisplacementMapElementPrototypeConstantsFunctions, 0, sizeof(JSSVGFEDisplacementMapElementPrototypeConstantsFunctions));
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[0].name = "SVG_CHANNEL_UNKNOWN";
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)WebCore::SVG_CHANNEL_UNKNOWN;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[1].name = "SVG_CHANNEL_R";
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)WebCore::SVG_CHANNEL_R;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[2].name = "SVG_CHANNEL_G";
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)WebCore::SVG_CHANNEL_G;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[3].name = "SVG_CHANNEL_B";
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)WebCore::SVG_CHANNEL_B;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[4].name = "SVG_CHANNEL_A";
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFEDisplacementMapElementPrototypeConstantsFunctions[4].u.i32 = (int32_t)WebCore::SVG_CHANNEL_A;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEDisplacementMapElementPrototypeFunctions[1];
static bool JSSVGFEDisplacementMapElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEDisplacementMapElementPrototypeFunctions()
{
    if (JSSVGFEDisplacementMapElementPrototypeFunctions_initialized) return;
    JSSVGFEDisplacementMapElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEDisplacementMapElementPrototypeFunctions, 0, sizeof(JSSVGFEDisplacementMapElementPrototypeFunctions));
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].magic = JSSVGFEDisplacementMapElement::GetPresentationAttributeFuncNum;
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEDisplacementMapElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEDisplacementMapElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEDisplacementMapElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEDisplacementMapElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEDisplacementMapElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEDisplacementMapElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEDisplacementMapElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEDisplacementMapElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementAttributesFunctions, countof(JSSVGFEDisplacementMapElementAttributesFunctions));
    init_JSSVGFEDisplacementMapElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementPrototypeConstantsFunctions, countof(JSSVGFEDisplacementMapElementPrototypeConstantsFunctions));
    init_JSSVGFEDisplacementMapElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDisplacementMapElementPrototypeFunctions, countof(JSSVGFEDisplacementMapElementPrototypeFunctions));
}

static JSClassDef JSSVGFEDisplacementMapElementClassDefine;
static bool JSSVGFEDisplacementMapElementClassDefine_initialized = false;

static void init_JSSVGFEDisplacementMapElementClassDefine()
{
    if (JSSVGFEDisplacementMapElementClassDefine_initialized) return;
    JSSVGFEDisplacementMapElementClassDefine_initialized = true;
    memset(&JSSVGFEDisplacementMapElementClassDefine, 0, sizeof(JSSVGFEDisplacementMapElementClassDefine));
    JSSVGFEDisplacementMapElementClassDefine.class_name = "SVGFEDisplacementMapElement";
    JSSVGFEDisplacementMapElementClassDefine.finalizer = JSSVGFEDisplacementMapElement::finalizer;
    JSSVGFEDisplacementMapElementClassDefine.gc_mark = JSSVGFEDisplacementMapElement::mark;
}

JSClassID JSSVGFEDisplacementMapElement::js_class_id = 0;

void JSSVGFEDisplacementMapElement::init(JSContext* ctx)
{
    if (JSSVGFEDisplacementMapElement::js_class_id == 0) {
        init_JSSVGFEDisplacementMapElementClassDefine();
        JS_NewClassID(&JSSVGFEDisplacementMapElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEDisplacementMapElement::js_class_id, &JSSVGFEDisplacementMapElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEDisplacementMapElementConstructor::self(ctx), JSSVGFEDisplacementMapElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEDisplacementMapElement::js_class_id, JSSVGFEDisplacementMapElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEDisplacementMapElement::create(JSContext* ctx, SVGFEDisplacementMapElement* impl)
{
    JSSVGFEDisplacementMapElement::init(ctx);
    JSValue _proto = JSSVGFEDisplacementMapElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEDisplacementMapElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEDisplacementMapElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEDisplacementMapElement* impl = (SVGFEDisplacementMapElement*)JS_GetOpaque(val, JSSVGFEDisplacementMapElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEDisplacementMapElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEDisplacementMapElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case In2AttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in2Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case ScaleAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->scaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XChannelSelectorAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->xChannelSelectorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YChannelSelectorAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->yChannelSelectorAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEDisplacementMapElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEDisplacementMapElementConstructor::self(ctx);
}

JSValue JSSVGFEDisplacementMapElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEDisplacementMapElement* imp = (SVGFEDisplacementMapElement*)JS_GetOpaque(this_val, JSSVGFEDisplacementMapElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEDisplacementMapElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
