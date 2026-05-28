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
#include "QJSSVGFEDiffuseLightingElement.h"

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "QJSCSSStyleDeclaration.h"
#include "QJSCSSValue.h"
#include "QJSSVGAnimatedLength.h"
#include "QJSSVGAnimatedNumber.h"
#include "QJSSVGAnimatedString.h"
#include "SVGFEDiffuseLightingElement.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGFEDiffuseLightingElementAttributesFunctions[12];
static bool JSSVGFEDiffuseLightingElementAttributesFunctions_initialized = false;

static void init_JSSVGFEDiffuseLightingElementAttributesFunctions()
{
    if (JSSVGFEDiffuseLightingElementAttributesFunctions_initialized) return;
    JSSVGFEDiffuseLightingElementAttributesFunctions_initialized = true;
    memset(JSSVGFEDiffuseLightingElementAttributesFunctions, 0, sizeof(JSSVGFEDiffuseLightingElementAttributesFunctions));
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].name = "in1";
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].magic = JSSVGFEDiffuseLightingElement::In1AttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].name = "surfaceScale";
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].magic = JSSVGFEDiffuseLightingElement::SurfaceScaleAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].name = "diffuseConstant";
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].magic = JSSVGFEDiffuseLightingElement::DiffuseConstantAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].name = "kernelUnitLengthX";
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].magic = JSSVGFEDiffuseLightingElement::KernelUnitLengthXAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].name = "kernelUnitLengthY";
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].magic = JSSVGFEDiffuseLightingElement::KernelUnitLengthYAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].name = "x";
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].magic = JSSVGFEDiffuseLightingElement::XAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].name = "y";
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].magic = JSSVGFEDiffuseLightingElement::YAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].name = "width";
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].magic = JSSVGFEDiffuseLightingElement::WidthAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].name = "height";
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].magic = JSSVGFEDiffuseLightingElement::HeightAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].name = "result";
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].magic = JSSVGFEDiffuseLightingElement::ResultAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].name = "className";
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].magic = JSSVGFEDiffuseLightingElement::ClassNameAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].name = "style";
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].magic = JSSVGFEDiffuseLightingElement::StyleAttrNum;
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].u.getset.get.getter_magic = JSSVGFEDiffuseLightingElement::getValueProperty;
    JSSVGFEDiffuseLightingElementAttributesFunctions[11].u.getset.set.setter_magic = NULL;
}

/* Prototype functions table */

static JSCFunctionListEntry JSSVGFEDiffuseLightingElementPrototypeFunctions[1];
static bool JSSVGFEDiffuseLightingElementPrototypeFunctions_initialized = false;

static void init_JSSVGFEDiffuseLightingElementPrototypeFunctions()
{
    if (JSSVGFEDiffuseLightingElementPrototypeFunctions_initialized) return;
    JSSVGFEDiffuseLightingElementPrototypeFunctions_initialized = true;
    memset(JSSVGFEDiffuseLightingElementPrototypeFunctions, 0, sizeof(JSSVGFEDiffuseLightingElementPrototypeFunctions));
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].name = "getPresentationAttribute";
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].def_type = JS_DEF_CFUNC;
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].magic = JSSVGFEDiffuseLightingElement::GetPresentationAttributeFuncNum;
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].u.func.length = 1;
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].u.func.cproto = JS_CFUNC_generic_magic;
    JSSVGFEDiffuseLightingElementPrototypeFunctions[0].u.func.cfunc.generic_magic = JSSVGFEDiffuseLightingElementPrototypeFunction::callAsFunction;
}

JSValue JSSVGFEDiffuseLightingElementPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGFEDiffuseLightingElement.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObjectProto(ctx, JSSVGElementPrototype::self(ctx));
        JSSVGFEDiffuseLightingElementPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGFEDiffuseLightingElement.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGFEDiffuseLightingElementPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGFEDiffuseLightingElementAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDiffuseLightingElementAttributesFunctions, countof(JSSVGFEDiffuseLightingElementAttributesFunctions));
    init_JSSVGFEDiffuseLightingElementPrototypeFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGFEDiffuseLightingElementPrototypeFunctions, countof(JSSVGFEDiffuseLightingElementPrototypeFunctions));
}

static JSClassDef JSSVGFEDiffuseLightingElementClassDefine;
static bool JSSVGFEDiffuseLightingElementClassDefine_initialized = false;

static void init_JSSVGFEDiffuseLightingElementClassDefine()
{
    if (JSSVGFEDiffuseLightingElementClassDefine_initialized) return;
    JSSVGFEDiffuseLightingElementClassDefine_initialized = true;
    memset(&JSSVGFEDiffuseLightingElementClassDefine, 0, sizeof(JSSVGFEDiffuseLightingElementClassDefine));
    JSSVGFEDiffuseLightingElementClassDefine.class_name = "SVGFEDiffuseLightingElement";
    JSSVGFEDiffuseLightingElementClassDefine.finalizer = JSSVGFEDiffuseLightingElement::finalizer;
    JSSVGFEDiffuseLightingElementClassDefine.gc_mark = JSSVGFEDiffuseLightingElement::mark;
}

JSClassID JSSVGFEDiffuseLightingElement::js_class_id = 0;

void JSSVGFEDiffuseLightingElement::init(JSContext* ctx)
{
    if (JSSVGFEDiffuseLightingElement::js_class_id == 0) {
        init_JSSVGFEDiffuseLightingElementClassDefine();
        JS_NewClassID(&JSSVGFEDiffuseLightingElement::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGFEDiffuseLightingElement::js_class_id, &JSSVGFEDiffuseLightingElementClassDefine);
        JS_SetClassProto(ctx, JSSVGFEDiffuseLightingElement::js_class_id, JSSVGFEDiffuseLightingElementPrototype::self(ctx));
    }
}

JSValue JSSVGFEDiffuseLightingElement::create(JSContext* ctx, SVGFEDiffuseLightingElement* impl)
{
    JSSVGFEDiffuseLightingElement::init(ctx);
    JSValue _proto = JSSVGFEDiffuseLightingElementPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGFEDiffuseLightingElement::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGFEDiffuseLightingElement::finalizer(JSRuntime* rt, JSValue val)
{
    SVGFEDiffuseLightingElement* impl = (SVGFEDiffuseLightingElement*)JS_GetOpaque(val, JSSVGFEDiffuseLightingElement::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGFEDiffuseLightingElement::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JSSVGElement::mark(rt, val, mark_func);
}

JSValue JSSVGFEDiffuseLightingElement::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case In1AttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->in1Animated();
            return toJS(ctx, obj.get(), imp);
        }
        case SurfaceScaleAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->surfaceScaleAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case DiffuseConstantAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->diffuseConstantAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case KernelUnitLengthXAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->kernelUnitLengthXAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case KernelUnitLengthYAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedNumber> obj = imp->kernelUnitLengthYAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case XAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case YAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case WidthAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case HeightAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ResultAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case ClassNameAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
            return toJS(ctx, obj.get(), imp);
        }
        case StyleAttrNum: {
            SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
            return toJS(ctx, QJS::getPtr(imp->style()));
        }
    }
    return JS_NULL;
}

JSValue JSSVGFEDiffuseLightingElementPrototypeFunction::callAsFunction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst *argv, int token)
{
    SVGFEDiffuseLightingElement* imp = (SVGFEDiffuseLightingElement*)JS_GetOpaque(this_val, JSSVGFEDiffuseLightingElement::js_class_id);
    if (!imp)
        return JS_ThrowTypeError(ctx, "Type error"); 

    switch (token) {
        case JSSVGFEDiffuseLightingElement::GetPresentationAttributeFuncNum: {
            String name = valueToString(ctx, argv[0]);

            JSValue result = toJS(ctx, QJS::getPtr(imp->getPresentationAttribute(name)));
            return result;
        }
    }
    return JS_NULL;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
