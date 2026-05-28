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
#include "QJSSVGFEColorMatrixElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumberList.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEColorMatrixElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEColorMatrixElementAttributesFunctions[11];
static bool JSSVGFEColorMatrixElementAttributesFunctions_initialized = false;

static void init_JSSVGFEColorMatrixElementAttributesFunctions()
{
    if (JSSVGFEColorMatrixElementAttributesFunctions_initialized) return;
    JSSVGFEColorMatrixElementAttributesFunctions_initialized = true;
    memset(JSSVGFEColorMatrixElementAttributesFunctions, 0, sizeof(JSSVGFEColorMatrixElementAttributesFunctions));
    JSSVGFEColorMatrixElementAttributesFunctions[0].name = "values";
    JSSVGFEColorMatrixElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[0].magic = JSSVGFEColorMatrixElement::ValuesAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[1].name = "height";
    JSSVGFEColorMatrixElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[1].magic = JSSVGFEColorMatrixElement::HeightAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[2].name = "className";
    JSSVGFEColorMatrixElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[2].magic = JSSVGFEColorMatrixElement::ClassNameAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[3].name = "type";
    JSSVGFEColorMatrixElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[3].magic = JSSVGFEColorMatrixElement::TypeAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[4].name = "in1";
    JSSVGFEColorMatrixElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[4].magic = JSSVGFEColorMatrixElement::In1AttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[5].name = "result";
    JSSVGFEColorMatrixElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[5].magic = JSSVGFEColorMatrixElement::ResultAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[6].name = "x";
    JSSVGFEColorMatrixElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[6].magic = JSSVGFEColorMatrixElement::XAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[7].name = "y";
    JSSVGFEColorMatrixElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[7].magic = JSSVGFEColorMatrixElement::YAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[8].name = "width";
    JSSVGFEColorMatrixElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[8].magic = JSSVGFEColorMatrixElement::WidthAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[9].name = "style";
    JSSVGFEColorMatrixElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[9].magic = JSSVGFEColorMatrixElement::StyleAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFEColorMatrixElementAttributesFunctions[10].name = "constructor";
    JSSVGFEColorMatrixElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEColorMatrixElementAttributesFunctions[10].magic = JSSVGFEColorMatrixElement::ConstructorAttrNum;
    JSSVGFEColorMatrixElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFEColorMatrixElement::getValueProperty;
    JSSVGFEColorMatrixElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
}

class JSSVGFEColorMatrixElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGFEColorMatrixElementConstructorFunctions[5];
static bool JSSVGFEColorMatrixElementConstructorFunctions_initialized = false;

static void init_JSSVGFEColorMatrixElementConstructorFunctions()
{
    if (JSSVGFEColorMatrixElementConstructorFunctions_initialized) return;
    JSSVGFEColorMatrixElementConstructorFunctions_initialized = true;
    memset(JSSVGFEColorMatrixElementConstructorFunctions, 0, sizeof(JSSVGFEColorMatrixElementConstructorFunctions));
    JSSVGFEColorMatrixElementConstructorFunctions[0].name = "SVG_FECOLORMATRIX_TYPE_UNKNOWN";
    JSSVGFEColorMatrixElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementConstructorFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_UNKNOWN;
    JSSVGFEColorMatrixElementConstructorFunctions[1].name = "SVG_FECOLORMATRIX_TYPE_MATRIX";
    JSSVGFEColorMatrixElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementConstructorFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_MATRIX;
    JSSVGFEColorMatrixElementConstructorFunctions[2].name = "SVG_FECOLORMATRIX_TYPE_SATURATE";
    JSSVGFEColorMatrixElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementConstructorFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_SATURATE;
    JSSVGFEColorMatrixElementConstructorFunctions[3].name = "SVG_FECOLORMATRIX_TYPE_HUEROTATE";
    JSSVGFEColorMatrixElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementConstructorFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_HUEROTATE;
    JSSVGFEColorMatrixElementConstructorFunctions[4].name = "SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA";
    JSSVGFEColorMatrixElementConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementConstructorFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementConstructorFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA;
}

JSValue JSSVGFEColorMatrixElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGFEColorMatrixElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGFEColorMatrixElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGFEColorMatrixElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEColorMatrixElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEColorMatrixElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementConstructorFunctions, countof(JSSVGFEColorMatrixElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGFEColorMatrixElementPrototypeConstantsFunctions[5];
static bool JSSVGFEColorMatrixElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGFEColorMatrixElementPrototypeConstantsFunctions()
{
    if (JSSVGFEColorMatrixElementPrototypeConstantsFunctions_initialized) return;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGFEColorMatrixElementPrototypeConstantsFunctions, 0, sizeof(JSSVGFEColorMatrixElementPrototypeConstantsFunctions));
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[0].name = "SVG_FECOLORMATRIX_TYPE_UNKNOWN";
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_UNKNOWN;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[1].name = "SVG_FECOLORMATRIX_TYPE_MATRIX";
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_MATRIX;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[2].name = "SVG_FECOLORMATRIX_TYPE_SATURATE";
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_SATURATE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[3].name = "SVG_FECOLORMATRIX_TYPE_HUEROTATE";
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_HUEROTATE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[4].name = "SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA";
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[4].def_type = JS_DEF_PROP_INT32;
    JSSVGFEColorMatrixElementPrototypeConstantsFunctions[4].u.i32 = (int32_t)WebCore::SVG_FECOLORMATRIX_TYPE_LUMINANCETOALPHA;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEColorMatrixElementPrototypeFunctions[1];
static bool JSSVGFEColorMatrixElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEColorMatrixElementPrototypeFunctions()
{
    if (JSSVGFEColorMatrixElementPrototypeFunctions_initialized) return;
    JSSVGFEColorMatrixElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEColorMatrixElementPrototypeFunctions, 0, sizeof(JSSVGFEColorMatrixElementPrototypeFunctions));
    JSSVGFEColorMatrixElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEColorMatrixElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEColorMatrixElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEColorMatrixElementPrototypeFunctions[0].magic = JSSVGFEColorMatrixElement::GetPresentationAttributeFuncNum;
    JSSVGFEColorMatrixElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEColorMatrixElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEColorMatrixElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEColorMatrixElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEColorMatrixElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEColorMatrixElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEColorMatrixElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEColorMatrixElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEColorMatrixElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEColorMatrixElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementAttributesFunctions, countof(JSSVGFEColorMatrixElementAttributesFunctions));
    init_JSSVGFEColorMatrixElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementPrototypeConstantsFunctions, countof(JSSVGFEColorMatrixElementPrototypeConstantsFunctions));
    init_JSSVGFEColorMatrixElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEColorMatrixElementPrototypeFunctions, countof(JSSVGFEColorMatrixElementPrototypeFunctions));
}

static JSClassDef JSSVGFEColorMatrixElementClassDefine;
static bool JSSVGFEColorMatrixElementClassDefine_initialized = false;

static void init_JSSVGFEColorMatrixElementClassDefine()
{
    if (JSSVGFEColorMatrixElementClassDefine_initialized) return;
    JSSVGFEColorMatrixElementClassDefine_initialized = true;
    memset(&JSSVGFEColorMatrixElementClassDefine, 0, sizeof(JSSVGFEColorMatrixElementClassDefine));
    JSSVGFEColorMatrixElementClassDefine.class_name = "SVGFEColorMatrixElement";
    JSSVGFEColorMatrixElementClassDefine.finalizer = JSSVGFEColorMatrixElement::finalizer;
    JSSVGFEColorMatrixElementClassDefine.gc_mark = JSSVGFEColorMatrixElement::mark;
}

JSClassID JSSVGFEColorMatrixElement::js_class_id = 0;

void JSSVGFEColorMatrixElement::init(JSContext* ctx)
{
    if (JSSVGFEColorMatrixElement::js_class_id == 0) {
        init_JSSVGFEColorMatrixElementClassDefine();
        JS_NewClassID(&JSSVGFEColorMatrixElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEColorMatrixElement::js_class_id, &JSSVGFEColorMatrixElementClassDefine);
        JS_SetConstructor(ctx, JSSVGFEColorMatrixElementConstructor::self(ctx), JSSVGFEColorMatrixElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGFEColorMatrixElement::js_class_id, JSSVGFEColorMatrixElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEColorMatrixElement::create(JSContext* ctx, SVGFEColorMatrixElement* impl)
{
    JSSVGFEColorMatrixElement::init(ctx);
    JSValue _proto = JSSVGFEColorMatrixElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEColorMatrixElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEColorMatrixElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEColorMatrixElement* impl = (SVGFEColorMatrixElement*)JS_GetOpaque(val, JSSVGFEColorMatrixElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEColorMatrixElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEColorMatrixElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case TypeAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->typeAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ValuesAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedNumberList> obj = imp->valuesAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGFEColorMatrixElement::getConstructor(JSContext *ctx)
{
    return JSSVGFEColorMatrixElementConstructor::self(ctx);
}

JSValue JSSVGFEColorMatrixElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEColorMatrixElement* imp = (SVGFEColorMatrixElement*)JS_GetOpaque(this_val, JSSVGFEColorMatrixElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEColorMatrixElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
