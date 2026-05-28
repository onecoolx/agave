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
#include "QJSSVGGradientElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedBoolean.h"
#include "QJSSVGAnimatedEnumeration.h"
#include "QJSSVGAnimatedString.h"
#include "QJSSVGAnimatedTransformList.h"
#include "SVGGradientElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGGradientElementAttributesFunctions[8];
static bool JSSVGGradientElementAttributesFunctions_initialized = false;

static void init_JSSVGGradientElementAttributesFunctions()
{
    if (JSSVGGradientElementAttributesFunctions_initialized) return;
    JSSVGGradientElementAttributesFunctions_initialized = true;
    memset(JSSVGGradientElementAttributesFunctions, 0, sizeof(JSSVGGradientElementAttributesFunctions));
    JSSVGGradientElementAttributesFunctions[0].name = "href";
    JSSVGGradientElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[0].magic = JSSVGGradientElement::HrefAttrNum;
    JSSVGGradientElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[1].name = "externalResourcesRequired";
    JSSVGGradientElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[1].magic = JSSVGGradientElement::ExternalResourcesRequiredAttrNum;
    JSSVGGradientElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[2].name = "className";
    JSSVGGradientElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[2].magic = JSSVGGradientElement::ClassNameAttrNum;
    JSSVGGradientElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[3].name = "style";
    JSSVGGradientElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[3].magic = JSSVGGradientElement::StyleAttrNum;
    JSSVGGradientElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[4].name = "spreadMethod";
    JSSVGGradientElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[4].magic = JSSVGGradientElement::SpreadMethodAttrNum;
    JSSVGGradientElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[5].name = "gradientUnits";
    JSSVGGradientElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[5].magic = JSSVGGradientElement::GradientUnitsAttrNum;
    JSSVGGradientElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[6].name = "gradientTransform";
    JSSVGGradientElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[6].magic = JSSVGGradientElement::GradientTransformAttrNum;
    JSSVGGradientElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGGradientElementAttributesFunctions[7].name = "constructor";
    JSSVGGradientElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGGradientElementAttributesFunctions[7].magic = JSSVGGradientElement::ConstructorAttrNum;
    JSSVGGradientElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGGradientElement::getValueProperty;
    JSSVGGradientElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
}

class JSSVGGradientElementConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
};

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGGradientElementConstructorFunctions[4];
static bool JSSVGGradientElementConstructorFunctions_initialized = false;

static void init_JSSVGGradientElementConstructorFunctions()
{
    if (JSSVGGradientElementConstructorFunctions_initialized) return;
    JSSVGGradientElementConstructorFunctions_initialized = true;
    memset(JSSVGGradientElementConstructorFunctions, 0, sizeof(JSSVGGradientElementConstructorFunctions));
    JSSVGGradientElementConstructorFunctions[0].name = "SVG_SPREADMETHOD_UNKNOWN";
    JSSVGGradientElementConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementConstructorFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementConstructorFunctions[0].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN;
    JSSVGGradientElementConstructorFunctions[1].name = "SVG_SPREADMETHOD_PAD";
    JSSVGGradientElementConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementConstructorFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementConstructorFunctions[1].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_PAD;
    JSSVGGradientElementConstructorFunctions[2].name = "SVG_SPREADMETHOD_REFLECT";
    JSSVGGradientElementConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementConstructorFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementConstructorFunctions[2].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_REFLECT;
    JSSVGGradientElementConstructorFunctions[3].name = "SVG_SPREADMETHOD_REPEAT";
    JSSVGGradientElementConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementConstructorFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementConstructorFunctions[3].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_REPEAT;
}

JSValue JSSVGGradientElementConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGGradientElement.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGGradientElementConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGGradientElement.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGGradientElementConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGGradientElementConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementConstructorFunctions, countof(JSSVGGradientElementConstructorFunctions));
}

/* Constants table */

static JSCFunctionListEntry JSSVGGradientElementPrototypeConstantsFunctions[4];
static bool JSSVGGradientElementPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGGradientElementPrototypeConstantsFunctions()
{
    if (JSSVGGradientElementPrototypeConstantsFunctions_initialized) return;
    JSSVGGradientElementPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGGradientElementPrototypeConstantsFunctions, 0, sizeof(JSSVGGradientElementPrototypeConstantsFunctions));
    JSSVGGradientElementPrototypeConstantsFunctions[0].name = "SVG_SPREADMETHOD_UNKNOWN";
    JSSVGGradientElementPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementPrototypeConstantsFunctions[0].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementPrototypeConstantsFunctions[0].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_UNKNOWN;
    JSSVGGradientElementPrototypeConstantsFunctions[1].name = "SVG_SPREADMETHOD_PAD";
    JSSVGGradientElementPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementPrototypeConstantsFunctions[1].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementPrototypeConstantsFunctions[1].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_PAD;
    JSSVGGradientElementPrototypeConstantsFunctions[2].name = "SVG_SPREADMETHOD_REFLECT";
    JSSVGGradientElementPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementPrototypeConstantsFunctions[2].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementPrototypeConstantsFunctions[2].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_REFLECT;
    JSSVGGradientElementPrototypeConstantsFunctions[3].name = "SVG_SPREADMETHOD_REPEAT";
    JSSVGGradientElementPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGGradientElementPrototypeConstantsFunctions[3].def_type = JS_DEF_PROP_INT32;
    JSSVGGradientElementPrototypeConstantsFunctions[3].u.i32 = (int32_t)SVGGradientElement::SVG_SPREADMETHOD_REPEAT;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGGradientElementPrototypeFunctions[1];
static bool JSSVGGradientElementPrototypeFunctions_initialized = false;

static void init_JSSVGGradientElementPrototypeFunctions()
{
    if (JSSVGGradientElementPrototypeFunctions_initialized) return;
    JSSVGGradientElementPrototypeFunctions_initialized = true;
    memset(JSSVGGradientElementPrototypeFunctions, 0, sizeof(JSSVGGradientElementPrototypeFunctions));
    JSSVGGradientElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGGradientElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGGradientElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGGradientElementPrototypeFunctions[0].magic = JSSVGGradientElement::GetPresentationAttributeFuncNum;
    JSSVGGradientElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGGradientElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGGradientElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGGradientElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGGradientElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGGradientElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGGradientElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGGradientElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGGradientElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGGradientElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementAttributesFunctions, countof(JSSVGGradientElementAttributesFunctions));
    init_JSSVGGradientElementPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementPrototypeConstantsFunctions, countof(JSSVGGradientElementPrototypeConstantsFunctions));
    init_JSSVGGradientElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGGradientElementPrototypeFunctions, countof(JSSVGGradientElementPrototypeFunctions));
}

static JSClassDef JSSVGGradientElementClassDefine;
static bool JSSVGGradientElementClassDefine_initialized = false;

static void init_JSSVGGradientElementClassDefine()
{
    if (JSSVGGradientElementClassDefine_initialized) return;
    JSSVGGradientElementClassDefine_initialized = true;
    memset(&JSSVGGradientElementClassDefine, 0, sizeof(JSSVGGradientElementClassDefine));
    JSSVGGradientElementClassDefine.class_name = "SVGGradientElement";
    JSSVGGradientElementClassDefine.finalizer = JSSVGGradientElement::finalizer;
    JSSVGGradientElementClassDefine.gc_mark = JSSVGGradientElement::mark;
}

JSClassID JSSVGGradientElement::js_class_id = 0;

void JSSVGGradientElement::init(JSContext* ctx)
{
    if (JSSVGGradientElement::js_class_id == 0) {
        init_JSSVGGradientElementClassDefine();
        JS_NewClassID(&JSSVGGradientElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGGradientElement::js_class_id, &JSSVGGradientElementClassDefine);
        JS_SetConstructor(ctx, JSSVGGradientElementConstructor::self(ctx), JSSVGGradientElementPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGGradientElement::js_class_id, JSSVGGradientElementPrototype::self(ctx));
    }
}

JSValue JSSVGGradientElement::create(JSContext* ctx, SVGGradientElement* impl)
{
    JSSVGGradientElement::init(ctx);
    JSValue _proto = JSSVGGradientElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGGradientElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGGradientElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGGradientElement* impl = (SVGGradientElement*)JS_GetOpaque(val, JSSVGGradientElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGGradientElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGGradientElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case GradientUnitsAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->gradientUnitsAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case GradientTransformAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedTransformList> obj = imp->gradientTransformAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case SpreadMethodAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedEnumeration> obj = imp->spreadMethodAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HrefAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ExternalResourcesRequiredAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGGradientElement::getConstructor(JSContext *ctx)
{
    return JSSVGGradientElementConstructor::self(ctx);
}

JSValue JSSVGGradientElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGGradientElement* imp = (SVGGradientElement*)JS_GetOpaque(this_val, JSSVGGradientElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGGradientElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG)
